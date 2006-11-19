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

#include <qcursor.h>
#include <qpainter.h>
#include <qpopupmenu.h> 

#include "SdContinuationCanvas.h"
#include "ContinuationDialog.h"
#include "UmlGlobal.h"
#include "UmlCanvas.h"
#include "BrowserDiagram.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "DialogUtil.h"

SdContinuationCanvas::SdContinuationCanvas(UmlCanvas * canvas, int x, int y, int id)
    : DiagramCanvas(0, canvas, x, y, CONTINUATION_CANVAS_MIN_SIZE,
		    CONTINUATION_CANVAS_MIN_SIZE, id) {
  browser_node = canvas->browser_diagram();
  itscolor = UmlDefaultColor;
  check_size();
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

SdContinuationCanvas::~SdContinuationCanvas() {
}

void SdContinuationCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  DiagramCanvas::delete_it();
}


void SdContinuationCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  QColor bckgrnd = p.backgroundColor();
  QBrush brsh = p.brush();

  p.setBackgroundMode((used_color == UmlTransparent) ? QObject::TransparentMode : QObject::OpaqueMode);

  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  
  if (used_color != UmlTransparent) 
    p.setBrush(co);
  
  p.drawRoundRect(r, 50, 50);
  
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  p.drawText(r, QObject::AlignCenter, name);
	     
  p.setBackgroundColor(bckgrnd);
  p.setBrush(brsh);
  
  if (selected())
    show_mark(p, r);
}

UmlCode SdContinuationCanvas::type() const {
  return UmlContinuation;
}

void SdContinuationCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

bool SdContinuationCanvas::alignable() const {
  return TRUE;
}

bool SdContinuationCanvas::copyable() const {
  return selected();
}

void SdContinuationCanvas::open() {
  ContinuationDialog d(name);
  
  d.exec();
  modified();
}

void SdContinuationCanvas::check_size() {
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  
  min_height = fm.height() * 2;
  min_width = fm.width((name.isEmpty()) ? QString("X") : name) + min_height;
  
  int min = (int) (CONTINUATION_CANVAS_MIN_SIZE * the_canvas()->zoom());

  if (min_width < min)
    min_width  = min;
  if (min_height < min)
    min_height  = min;
  
  // warning : do NOT check if (width() < w)
  // because te resize must be done to set data on scale change
  DiagramCanvas::resize((width() > min_width) ? width() : min_width,
			(height() > min_height) ? height() : min_height);

  used_color = (itscolor == UmlDefaultColor)
    ?  the_canvas()->browser_diagram()->get_color(UmlContinuation)
    : itscolor;
}

void SdContinuationCanvas::change_scale() {
  double scale = the_canvas()->zoom();
  
  QCanvasRectangle::setVisible(FALSE);
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  check_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);  
}

void SdContinuationCanvas::modified() {
  // force son reaffichage
  hide();
  check_size();
  show();
  canvas()->update();
  package_modified();
}

void SdContinuationCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Continuation", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 3);
  if (linked()) {
    m.insertSeparator();
    m.insertItem("Select linked items", 4);
  }
  m.insertSeparator();
  m.insertItem("Remove from view",5);

  int index = m.exec(QCursor::pos());
  
  switch (index) {
  case 0:
    upper();
    modified();	// call package_modified()
    return;
  case 1:
    lower();
    modified();	// call package_modified()
    return;
  case 2:
    open();
    // modified then package_modified already called
    return;
  case 3:
    {
      QArray<ColorSpec> co(1);
      
      co[0].set("continuation color", &itscolor);

      SettingsDialog dialog(0, &co, FALSE, TRUE);
      
      dialog.raise();
      if (dialog.exec() == QDialog::Accepted)
	modified();
      return;
    }
    break;
  case 4:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 5:
    delete_it();
    break;
  default:
    return;
  }
  
  package_modified();
}

bool SdContinuationCanvas::has_drawing_settings() const {
  return TRUE;
}

void SdContinuationCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  
  co[0].set("continuation color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if ((dialog.exec() == QDialog::Accepted) && (co[0].name != 0)) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      ((SdContinuationCanvas *) it.current())->itscolor = itscolor;
      ((SdContinuationCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

const char * SdContinuationCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : "illegal";
}

const char * SdContinuationCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlAnchor:
  case UmlLabel:
  case UmlArrowPoint:
  case UmlLifeLine:
    return "illegal";
  default:
    return (l == UmlAnchor) ? 0 : "illegal";
  }
}

aCorner SdContinuationCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void SdContinuationCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width, min_height);
}

void SdContinuationCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref) {
    st << "continuation_ref " << get_ident();
  }
  else {
    nl_indent(st);
    
    st << "continuation " << get_ident() << ' ';
    save_string(name, st);
    nl_indent(st);
    if (itscolor != UmlDefaultColor)
      st << "  color " << stringify(itscolor);
    save_xyzwh(st, this, "  xyzwh");
  }
}

SdContinuationCanvas * SdContinuationCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "continuation_ref"))
    return (SdContinuationCanvas *) dict_get(read_id(st), "continuation", canvas);
  else if (!strcmp(k, "continuation")) {
    int id = read_id(st);
    SdContinuationCanvas * result = new SdContinuationCanvas(canvas, 0, 0, id);
    
    result->name = read_string(st);
    
    k = read_keyword(st);
    read_color(st, "color", result->itscolor, k);
    
    if (strcmp(k, "xyzwh"))
      wrong_keyword(k, "xyzwh");
    
    read_xyzwh(st, result);
    result->check_size();
    result->width_scale100 = result->width();
    result->height_scale100 = result->height();
    result->set_center100();
    result->show();
    return result;
  }
  else
    return 0;
}

void SdContinuationCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(min_width, b);
  ::save(min_height, b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void SdContinuationCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(min_width, b);
  ::load(min_height, b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
  
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

void SdContinuationCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

