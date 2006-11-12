// *************************************************************************
//
// Copyright (C) 2004-2006 Bruno PAGES  All rights reserved.
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <math.h>
#include <qpainter.h>

#include "DiagramCanvas.h"
#include "BrowserDiagram.h"
#include "ArrowCanvas.h"
#include "LabelCanvas.h"
#include "UmlWindow.h"
#include "myio.h"

DiagramCanvas::DiagramCanvas(BrowserNode * bn, UmlCanvas * canvas,
			     int x, int y, int wi, int he, int id)
    : QCanvasRectangle(canvas), DiagramItem(id, canvas), 
      browser_node(bn), label(0) {
  double zoom = canvas->zoom();
  
  if (wi >= 0) {
    width_scale100 = wi;
    wi  = (int) (wi * zoom);
  }
  else {
    wi = -wi;
    width_scale100 = wi;
  }
  
  if (he >= 0) {
    height_scale100 = he;
    he = (int) (he * zoom);
  }
  else {
    he = -he;
    height_scale100 = he;
  }
  
  setSize(wi, he);
  setX(x);
  setY(y);
    
  QPoint c = center();
  
  center_x_scale100 = (int) (c.x() / zoom);
  center_y_scale100 = (int) (c.y() / zoom);
  
  if (bn != 0) {
    // must be created after setX/Y whose call moveBy
    QFontMetrics fm(canvas->get_font(UmlNormalFont));
    
    label = new LabelCanvas(bn->get_name(), canvas,
			    x + (wi - fm.width(bn->get_name()))/2,
			    y + he);
  }
  
  setZ(DIAGRAMCANVAS_Z);
  
  if (canvas->paste())
    canvas->select(this);
}

DiagramCanvas::DiagramCanvas(UmlCanvas * canvas, int id)
  : QCanvasRectangle(canvas), DiagramItem(id, canvas),
    browser_node(0), label(0) {
  
  if (canvas->paste())
    canvas->select(this);
}

DiagramCanvas::~DiagramCanvas() {
}

void DiagramCanvas::delete_it() {
  while (! lines.isEmpty())
    lines.first()->delete_it();	// will remove the line
  
  if (label != 0)
    ((UmlCanvas *) canvas())->del(label);
  
  ((UmlCanvas *) canvas())->del(this);
}

BrowserNode * DiagramCanvas::get_bn() const {
  return browser_node;
}

UmlCanvas * DiagramCanvas::the_canvas() const {
  return (UmlCanvas *) canvas();
}

void DiagramCanvas::setVisible(bool yes) {
  QCanvasRectangle::setVisible(yes);
  if (label)
    label->setVisible(yes);
}

void DiagramCanvas::change_scale() {
  double scale = the_canvas()->zoom();
  
  QCanvasRectangle::setVisible(FALSE);
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void DiagramCanvas::recenter() {
  double scale = the_canvas()->zoom();
  QPoint c = center();
  
  QCanvasRectangle::moveBy(((int) (center_x_scale100 * scale + 0.5)) - c.x(),
			   ((int) (center_y_scale100 * scale + 0.5)) - c.y());
}

void DiagramCanvas::set_center100() {
  QPoint c = center();
  double scale = the_canvas()->zoom();
    
  center_x_scale100  = (int) (c.x()/scale);
  center_y_scale100  = (int) (c.y()/scale);
}

void DiagramCanvas::moveBy(double dx, double dy) {
  hide_lines();

  QCanvasRectangle::moveBy(dx, dy);

  if (! ((UmlCanvas *) canvas())->do_zoom())
    set_center100();
  
  if ((label != 0) && !label->selected())
    label->moveBy(dx, dy);

  update_show_lines();
}

void DiagramCanvas::setSelected(bool yes) {
  if (browser_node) {
    if (yes)
      UmlWindow::set_commented(browser_node);
    else
      UmlWindow::set_commented(0);
  }
  QCanvasRectangle::setSelected(yes);
}

void DiagramCanvas::select_associated() {
  if (!selected()) {
    the_canvas()->select(this);
    if ((label != 0) && !label->selected())
      the_canvas()->select(label);
    DiagramItem::select_associated();
  }
}

QPoint DiagramCanvas::center() const {
  return rect().center();
}

QRect DiagramCanvas::rect() const {
  return QCanvasRectangle::rect();
}

bool DiagramCanvas::isSelected() const {
  return selected();
}

bool DiagramCanvas::contains(int x, int y) const {
  return rect().contains(x, y);
}

void DiagramCanvas::connexion(UmlCode action, DiagramItem * dest,
			      const QPoint &, const QPoint &) {
  ArrowCanvas * a = 
    new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

void DiagramCanvas::resize(int wi, int he) {
  setSize(wi, he);
  
  if (! ((UmlCanvas *) canvas())->do_zoom()) {
    double zoom = the_canvas()->zoom();
    
    width_scale100 = (int) (wi / zoom);
    height_scale100 = (int) (he / zoom);
    
    QPoint c = center();
    
    center_x_scale100  = (int) (c.x() / zoom);
    center_y_scale100  = (int) (c.y() / zoom);
  }
}

// min_width and min_height must take into account the current zoom
void DiagramCanvas::resize(aCorner c, int dx, int dy,
			   int min_width, int min_height,
			   bool odd, bool stay_centered) {
  hide_lines();
  
  switch (c) {
  case ::TopLeft:
    QCanvasRectangle::moveBy(dx, dy);
    dx = -dx;
    dy = -dy;
    break;
  case ::TopRight:
    QCanvasRectangle::moveBy(0, dy);
    dy = -dy;
    break;
  case ::BottomLeft:
    QCanvasRectangle::moveBy(dx, 0);
    dx = -dx;
    break;
  default:
    break;
  }
  
  if (stay_centered) {
    dx += dx;
    dy += dy;
  }
  
  int wi = width() + dx;
  int he = height() + dy;
  
  if (wi < min_width)
    wi = min_width;

  if (he < min_height)
    he = min_height;
  
  if (odd) {
    wi |= 1;
    he |= 1;
  }
	    
  resize(wi, he);

  update_show_lines();
  
  canvas()->update();
}

void DiagramCanvas::prepare_for_move(bool on_resize) {
  if (! on_resize) {
    // select self relations
    QListIterator<ArrowCanvas> it(lines);
    
    while (it.current()) {
      if (it.current()->get_start() == it.current()->get_end())
	// note : selected 2 times (from the start and the end)
	it.current()->select_associated();
      ++it;
    }
  }
}

void DiagramCanvas::force_self_rel_visible() {
  QListIterator<ArrowCanvas> it(lines);
  QRect r = rect();
  
  // add a marging
  r.setLeft(r.left() - 5);
  r.setRight(r.right() + 5);
  r.setTop(r.top() - 5);
  r.setBottom(r.bottom() + 5);
  
  while (it.current()) {
    if (it.current()->get_start() == it.current()->get_end())
      // self relation
      it.current()->move_outside(r);
    
    ++it;
  }
}

void DiagramCanvas::upper() {
  double max_z = 0;
  QCanvasItemList l = collisions(FALSE);
  QCanvasItemList::Iterator it;
  
  for (it = l.begin(); it != l.end(); it++)
    if (!isa_alien(*it) &&
	(*it != this) &&
	(*it)->visible() &&
	((*it)->z() > max_z))
      max_z = (*it)->z();
  
  if ((z() - max_z) < 5)
    set_z(((int) max_z) + 5);	// (int) to manage Template & label
}

void DiagramCanvas::lower() {
  QCanvasItemList l = collisions(FALSE);
  QCanvasItemList::Iterator it;
  double min_z = 1e100;
  
  for (it = l.begin(); it != l.end(); it++)
    if ((QCanvasItemToDiagramItem(*it) != 0) &&
	(*it != this) &&
	(*it)->visible() &&
	((*it)->z() < min_z))
      min_z = (*it)->z();
  
  if (z() < min_z)
    // already ok
    ;
  else if (min_z > 15)
    set_z(((int) min_z) - 5);	// (int) to manage Template & labels
  else {
    // other canvas go up
    double incr = ((int) z()) + 5;
    
    for (it = l.begin(); it != l.end(); it++) {
      DiagramItem * di = QCanvasItemToDiagramItem(*it);
      
      if ((di != 0) && (*it != this))
	di->set_z((*it)->z() + incr);
    }
  }
}

double DiagramCanvas::get_z() const {
  return z();
}

void DiagramCanvas::set_z(double z) {
  setZ(z);
  
  if (label != 0)
    label->setZ(z + 0.5);
}

void DiagramCanvas::package_modified() const {
  the_canvas()->browser_diagram()->package_modified();
}

void DiagramCanvas::draw_actor(QPainter * p, QRect r)
{
  // the 'original' size is 40 x 40
  double scale = r.width() / 40.0;
  int i,j;
  
  p->setBackgroundMode(QObject::TransparentMode);
  
  // head
  i = (int) (13 * scale);
  p->drawEllipse(r.left() + (int) (13 * scale + 0.5), 
		 r.top(), i, i);
  
  // arm
  j = r.top() + (int) (18 * scale);
  p->drawLine(r.left() + (int) (8 * scale), j,
	      r.left() + (int) (30 * scale), j);
  
  // body
  i = r.left() + (int) (19 * scale);
  j = r.top() + (int) (28 * scale);
  p->drawLine(i, r.top() + (int) (13 * scale),
	      i, j);
  
  // legs
  p->drawLine(i, j, r.left() + (int) (9 * scale), r.bottom());
  p->drawLine(i, j, r.left() + (int) (29 * scale), r.bottom());
}

void DiagramCanvas::draw_control_icon(QPainter & p, QRect & r,
				      const QColor & co, double zoom)
{
  int sz = (int) (CONTROL_WIDTH * zoom);
  int cx = (r.left() + r.right()) /2;
  int dv = (int) ((CONTROL_HEIGHT - CONTROL_WIDTH) * zoom);
  int cy = r.top() + dv;
  QBrush brsh = p.brush();
  
  p.setBrush(co);
  
  p.drawEllipse(cx - sz/2, cy, sz, sz);
  p.drawLine(cx, cy, cx + dv, cy - dv);
  p.drawLine(cx, cy, cx + dv, cy + dv);
  
  p.setBrush(brsh);
}

void DiagramCanvas::draw_boundary_icon(QPainter & p, QRect & r,
				       const QColor & co, double zoom)
{
  int he = (int) (BOUNDARY_HEIGHT * zoom);
  int cx = (r.left() + r.right() - he) /2;
  int cy = r.top() + he/2;
  QBrush brsh = p.brush();
  
  p.setBrush(co);
  
  p.drawEllipse(cx, r.top(), he, he);
  
  int wi = he/2 - 2;
  
  p.drawLine(cx - wi, cy, cx, cy);
  p.drawLine(cx - wi, r.top(), cx - wi, r.top() + he);
  
  p.setBrush(brsh);
}

 void DiagramCanvas::draw_entity_icon(QPainter & p, QRect & r,
				      const QColor & co, double zoom)
{
  int sz = (int) (ENTITY_SIZE * zoom);
  int left = (r.left() + r.right() - sz)/2;
  QBrush brsh = p.brush();
  
  p.setBrush(co);
    
  p.drawEllipse(left, r.top(), sz, sz);
  p.drawLine(left, r.top() + sz, left + sz, r.top() + sz);
  
  p.setBrush(brsh);
}

double DiagramCanvas::compute_angle(double delta_x, double delta_y)
{
  if (delta_x == 0)
    return (delta_y > 0) ? 90 : 270;
  else {
    double angle = atan(delta_y / delta_x) * (180 / 3.1415927);	// -90 .. 90
    
    if (delta_x < 0)
      angle += 180;
    else if (angle < 0)
      angle += 360;

    return angle;
  }
}

void DiagramCanvas::history_save(QBuffer & b) const {
  ::save(this, b);
  ::save(center_x_scale100, b);
  ::save(center_y_scale100, b);
  ::save(x(), b);
  ::save(y(), b);
  ::save(z(), b);
}

void DiagramCanvas::history_load(QBuffer & b) {
  ::load(center_x_scale100, b);
  ::load(center_y_scale100, b);
  
  double dx = load_double(b) - x();

  QCanvasRectangle::moveBy(dx, load_double(b) - y());
  QCanvasRectangle::setZ(load_double(b));
  
  QCanvasItem::setSelected(FALSE);
  QCanvasItem::setVisible(TRUE);
}

void DiagramCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
}

