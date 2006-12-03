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

#include <qpainter.h>
#include <qintdict.h> 

#include "DiagramItem.h"
#include "DiagramCanvas.h"
#include "ArrowCanvas.h"
#include "LabelCanvas.h"
#include "BrowserNode.h"
#include "myio.h"

QList<DiagramItem> DiagramItem::Undefined;

DiagramItem::DiagramItem(int id, UmlCanvas * canvas)
    : Labeled<DiagramItem>(canvas->get_all_items(), id) {
}

DiagramItem::~DiagramItem() {
}

BrowserNode * DiagramItem::get_bn() const {
  return 0;
}

void DiagramItem::delete_available(bool &, bool &) const {
}

void DiagramItem::remove(bool) {
  delete_it();
}

void DiagramItem::hide_lines() {
  QListIterator<ArrowCanvas> it(lines);
  
  for (; it.current(); ++it)
    it.current()->hide();
}

void DiagramItem::update_show_lines() {
  QListIterator<ArrowCanvas> it(lines);
  
  for (; it.current(); ++it) {
    it.current()->update_pos();
    it.current()->show();
    it.current()->update_geometry();
  }
}

void DiagramItem::remove_line(ArrowCanvas * l) {
  lines.remove(l);
}

void DiagramItem::check_line(ArrowCanvas *) {
  // does nothing
}

DiagramItem::LineDirection DiagramItem::allowed_direction(UmlCode) {
  return DiagramItem::All;
}

bool DiagramItem::alignable() const {
  return FALSE;
}

bool DiagramItem::copyable() const {
  return FALSE;
}

aCorner DiagramItem::on_resize_point(const QPoint &) {
  return NoCorner;
}

void DiagramItem::resize(aCorner, int, int) {
  // never called but must be defined
}

void DiagramItem::update() {
  // does nothing
}

bool DiagramItem::connexion(UmlCode, const QPoint &, const QPoint &) {
  // not allowed
  return FALSE;
}
bool DiagramItem::may_connect(UmlCode) const {
  // not allowed
  return FALSE;
}

void DiagramItem::post_connexion(UmlCode, DiagramItem *) {
  // does nothing
}

void DiagramItem::post_load()
{
  while (! Undefined.isEmpty())
    Undefined.take()->delete_it();
}

BasicData * DiagramItem::add_relation(UmlCode t, DiagramItem * end) {
  return get_bn()->add_relation(t, end->get_bn());
}

bool DiagramItem::has_relation(BasicData * def) const {
  // manage only SimpleRelations
  QListIterator<ArrowCanvas> it(lines);
	
  while (it.current()) {
    if (IsaSimpleRelation(it.current()->type()) &&
	(it.current()->get_data() == def))
      return TRUE;
    ++it;
  }
  
  return FALSE;
}

bool DiagramItem::has_relation(UmlCode t, BasicData * def) const {
  QListIterator<ArrowCanvas> it(lines);
	
  while (it.current()) {
    if ((it.current()->type() == t) &&
	(it.current()->get_data() == def))
      return TRUE;
    ++it;
  }
  
  return FALSE;
}

void DiagramItem::prepare_for_move(bool) {
  // does nothing
}

bool DiagramItem::move_with_its_package() const {
  return FALSE;
}

void DiagramItem::select_associated() {
  QListIterator<ArrowCanvas> it(lines);
  
  for (; it.current(); ++it)
    it.current()->select_associated();
}

void DiagramItem::unassociate(DiagramItem *) {
  // does nothing at this level
  // for Component and Artifact association
}

void show_mark(QPainter & p, const QRect & r)
{
  int h = (SELECT_SQUARE_SIZE < r.height())
    ? SELECT_SQUARE_SIZE : r.height() - 1;
  int w = (SELECT_SQUARE_SIZE < r.width())
    ? SELECT_SQUARE_SIZE : r.width() - 1;

  p.fillRect(r.left(), r.top(), w, h, QObject::black);
  p.fillRect(r.right() - w, r.top(), w, h, QObject::black);
  p.fillRect(r.left(), r.bottom() - h, w, h, QObject::black);
  p.fillRect(r.right() - w, r.bottom() - h, w, h, QObject::black);
}

aCorner on_resize_point(const QPoint & p, const QRect & r)
{
  if ((p - r.topLeft()).manhattanLength() < SELECT_SQUARE_SIZE*2)
    return TopLeft;
  if ((p - r.topRight()).manhattanLength() < SELECT_SQUARE_SIZE*2)
    return TopRight;
  if ((p - r.bottomLeft()).manhattanLength() < SELECT_SQUARE_SIZE*2)
    return BottomLeft;
  if ((p - r.bottomRight()).manhattanLength() < SELECT_SQUARE_SIZE*2)
    return BottomRight;
  
  return NoCorner; 
}

DiagramItem * QCanvasItemToDiagramItem(QCanvasItem * ci)
{
  if (isa_arrow(ci))
    return ((ArrowCanvas *) ci);
  else if (isa_label(ci))
    return ((LabelCanvas *) ci);
  else
    return (isa_alien(ci)) 
      ? 0
      : ((DiagramCanvas *) ci);
}

//

DiagramItem * DiagramItem::dict_get(int id, const char * kind,
				    UmlCanvas * canvas)
{
  DiagramItem * result = canvas->get_all_items()[id];
  
  if (result == 0)
    unknown_ref(kind, id);
  
  return result;
}

// calcule p pour qu il soit sur le bord exterieur de this
// sur la ligne centre de this - other
void DiagramItem::shift(QPoint & p, QPoint other) const {
  p = center();
  int x = p.x();
  int y = p.y();
  int ox = other.x();
  int oy = other.y();

  for (;;) {
    int mx = (x + ox) / 2;
    int my = (y + oy) / 2;
    
    if (((mx == x) || (mx == ox)) &&
	((my == y) || (my == oy))) {
      p.setX(mx);
      p.setY(my);
      return;
    }

    if (! contains(mx, my)) {
      ox = mx;
      oy = my;
    }
    else {
      x = mx;
      y = my;
    }
  }
}


//

bool DiagramItem::has_drawing_settings() const {
  return false;
}

void DiagramItem::edit_drawing_settings(QList<DiagramItem> &) {
  // never called
}

//

void DiagramItem::apply_shortcut(QString) {
  // do nothing
}

//

DiagramItemList::DiagramItemList(QCanvasItemList l) {
  QCanvasItemList::Iterator it;
  
  for (it = l.begin(); it != l.end(); ++it) {
    if ((*it)->visible()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*it);
      
      if (di != 0)
	append(di);
    }
  }
}

DiagramItemList::~DiagramItemList() {
}

int DiagramItemList::compareItems(QCollection::Item i1,
				  QCollection::Item i2) {
  // i1 <> i2 !
  return (((DiagramItem *) i1)->get_ident() >
	  ((DiagramItem *) i2)->get_ident())
    ? 1 : -1;
}
