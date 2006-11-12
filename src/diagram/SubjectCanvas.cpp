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

#include "SubjectCanvas.h"
#include "MyInputDialog.h"
#include "UmlGlobal.h"
#include "UmlCanvas.h"
#include "BrowserDiagram.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "strutil.h"

SubjectCanvas::SubjectCanvas(UmlCanvas * canvas, int x, int y, int id)
    : DiagramCanvas(0, canvas, x, y, SUBJECT_CANVAS_MIN_SIZE,
		    SUBJECT_CANVAS_MIN_SIZE, id) {
  browser_node = canvas->browser_diagram();
  itscolor = UmlDefaultColor;
  check_size();
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

SubjectCanvas::~SubjectCanvas() {
}

void SubjectCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  DiagramCanvas::delete_it();
}


void SubjectCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  QColor bckgrnd = p.backgroundColor();

  p.setBackgroundMode((used_color == UmlTransparent) ? QObject::TransparentMode : QObject::OpaqueMode);

  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  p.setFont(the_canvas()->get_font(UmlNormalBoldFont));
  
  if (used_color != UmlTransparent) p.fillRect(r, co);
  p.drawRect(r);
  
  r.setTop(r.top() + (int) (2*the_canvas()->zoom()));
  p.drawText(r, QObject::AlignHCenter + QObject::AlignTop, name);
  p.setBackgroundColor(bckgrnd);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode SubjectCanvas::type() const {
  return UmlSubject;
}

void SubjectCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

bool SubjectCanvas::alignable() const {
  return TRUE;
}

bool SubjectCanvas::copyable() const {
  return selected();
}

void SubjectCanvas::open() {
  bool ok;
  QString s = MyInputDialog::getText("Subject dialog", "subject : ",
				     fromUnicode(name), ok);
  
  if (ok) {
    name = toUnicode(s);
    modified();
  }
}

void SubjectCanvas::check_size() {
  QFontMetrics fm(the_canvas()->get_font(UmlNormalBoldFont));
  
  min_width = fm.width((name.isEmpty()) ? QString("X") : name)
    + (int) (8 * the_canvas()->zoom());
  min_height = fm.height() * 2;
  
  int min = (int) (SUBJECT_CANVAS_MIN_SIZE * the_canvas()->zoom());
  
  if (min_width < min)
    min_width = min;
  if (min_height < min)
    min_height = min;
  
  // force odd width and height for line alignment
  min_width |= 1;
  min_height |= 1;
    
  // warning : do NOT check if ((width() < min_width) || (height() < min_height))
  // because te resize must be done to set data on scale change
  DiagramCanvas::resize((width() > min_width) ? width() : min_width,
			(height() > min_height) ? height() : min_height);

  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlSubject)
    : itscolor;
}

void SubjectCanvas::change_scale() {
  double scale = the_canvas()->zoom();
  
  QCanvasRectangle::setVisible(FALSE);
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  check_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);  
}

void SubjectCanvas::modified() {
  // force son reaffichage
  hide();
  check_size();
  show();
  canvas()->update();
  package_modified();
}

void SubjectCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Subject", m.font()), -1);
  m.insertSeparator();
  m.insertItem("upper", 0);
  m.insertItem("lower", 1);
  m.insertSeparator();
  m.insertItem("edit", 2);
  m.insertSeparator();
  m.insertItem("edit drawing settings", 3);
  if (linked()) {
    m.insertSeparator();
    m.insertItem("select linked items", 4);
  }
  m.insertSeparator();
  m.insertItem("remove from view",5);

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
      
      co[0].set("subject color", &itscolor);

      SettingsDialog dialog(0, &co, FALSE, TRUE);
      
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

const char * SubjectCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : "illegal";
}

const char * SubjectCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
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

aCorner SubjectCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void SubjectCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width, min_height);
}

void SubjectCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref) {
    st << "subject_ref " << get_ident();
  }
  else {
    nl_indent(st);
    
    st << "subject " << get_ident() << ' ';
    save_string(fromUnicode(name), st);
    nl_indent(st);
    if (itscolor != UmlDefaultColor)
      st << "  color " << stringify(itscolor);
    save_xyzwh(st, this, "  xyzwh");
  }
}

SubjectCanvas * SubjectCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "subject_ref"))
    return (SubjectCanvas *) dict_get(read_id(st), "subject", canvas);
  else if (!strcmp(k, "subject")) {
    int id = read_id(st);
    SubjectCanvas * result = new SubjectCanvas(canvas, 0, 0, id);
    
    result->name = toUnicode(read_string(st));
    
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


void SubjectCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(min_width, b);
  ::save(min_height, b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void SubjectCanvas::history_load(QBuffer & b) {
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

void SubjectCanvas::history_hide() {
  DiagramCanvas::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

