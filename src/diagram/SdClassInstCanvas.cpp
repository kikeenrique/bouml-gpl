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
#include <qpopupmenu.h> 
#include <qpainter.h> 

#include "SdClassInstCanvas.h"
#include "BrowserDiagram.h"
#include "BrowserClass.h"
#include "UmlCanvas.h"
#include "ClassData.h"
#include "InstanceDialog.h"
#include "SettingsDialog.h"
#include "Settings.h"
#include "myio.h"
#include "MenuTitle.h"

SdClassInstCanvas::SdClassInstCanvas(BrowserClass * t, UmlCanvas * canvas,
				     int x, int id)
    : SdObjCanvas(0, canvas, x, CLASSINST_CANVAS_MIN_SIZE, 1, id),
      ClassInstCanvas(t) {
  browser_node = canvas->browser_diagram();
  itscolor = UmlDefaultColor;
 
  compute_size();
  
  connect(cl->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(cl->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

SdClassInstCanvas::~SdClassInstCanvas() {
}

void SdClassInstCanvas::delete_it() {
  disconnect(cl->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  SdObjCanvas::delete_it();
}

void SdClassInstCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void SdClassInstCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

void SdClassInstCanvas::compute_size() {
  int w, h;
  
  if (strcmp(cl->get_stereotype(), "actor") == 0) {
    QFontMetrics fm(the_canvas()->get_font(UmlNormalUnderlinedFont));
    double zoom = the_canvas()->zoom();
    // divide original size by two to have the same height
    // for all the seq diag obj
    double actor_size = ACTOR_SIZE / 2.0 * zoom;
    
    h = fm.height() + (int) (actor_size + 8 * zoom);
    
    int minw = (int) actor_size;
    
    w = fm.width(cl->get_name());
    
    if (w < minw)
      w = minw;
    
    w += (int) (8 * zoom);
  }
  else
    ClassInstCanvas::compute_size(w, h, the_canvas());
  
  resize(w, h);
}

void SdClassInstCanvas::modified() {
  if (visible()) {
    int w = width();
    
    hide();
    compute_size();
    DiagramCanvas::moveBy((w-width())/2, 0);
    show();
    canvas()->update();
    package_modified();
  }
}

void SdClassInstCanvas::draw(QPainter & p) {
  if (visible()) {
    if (strcmp(cl->get_stereotype(), "actor") == 0) {
      double zoom = the_canvas()->zoom();
      // divide original size by two to have the same height
      // for all the seq diag obj
      double actor_size = ACTOR_SIZE / 2.0 * zoom;
      QRect ra = rect();
      
      ra.setHeight((int) actor_size);
      ra.setLeft(ra.left() + 
		 (int) ((ra.width() - actor_size)/2));
      ra.setWidth(ra.height());
      draw_actor(&p, ra);
      ra = rect();
      ra.setTop(ra.top() + (int) actor_size);
      
      QColor bckgrnd = p.backgroundColor();
      
      p.setBackgroundMode(QObject::TransparentMode);
      p.setFont(the_canvas()->get_font(UmlNormalFont));
      p.drawText(ra, QObject::AlignCenter, cl->get_name());
      p.setBackgroundColor(bckgrnd);
    }
    else
      ClassInstCanvas::draw(p, the_canvas(), rect());

    if (selected())
      show_mark(p, rect());
  }
}

UmlCode SdClassInstCanvas::type() const {
  return UmlClass;
}

BrowserClass * SdClassInstCanvas::get_type() {
  return cl;
}

void SdClassInstCanvas::open() {
  InstanceDialog d(this, "class : ", "Class instance dialog");
  
  if (d.exec() == QDialog::Accepted)
    modified();
}

void SdClassInstCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle(full_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  if (is_mortal())
    m.insertItem("Become immortal", 5);
  else
    m.insertItem("Become mortal", 6);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 3);
  m.insertSeparator();
  m.insertItem("Select class in browser", 7);
  m.insertSeparator();
  m.insertItem("Remove from view", 4);
  
  switch (m.exec(QCursor::pos())) {
  case 0:
    upper();
    modified();
    return;
  case 1:
    lower();
    modified();
    return;
  case 2:
    open();	// call modified()
    return;
  case 3:
    edit_drawing_settings();
    return;
  case 4:
    delete_it();
    break;
  case 5:
    set_mortal(FALSE);
    break;
  case 6:
    set_mortal(TRUE);
    break;
  case 7:
    cl->select_in_browser();
    return;
  default:
    return;
  }
  
  package_modified();
}

void SdClassInstCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser") { 
    cl->select_in_browser();
    return;
  }
  else if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Edit drawing settings") {
    edit_drawing_settings();
    return;
  }
  else if (s == "Edit") {
    open();  // call modified then package_modified
    return;
  }
  else 
    return;

  modified();
}

void SdClassInstCanvas::edit_drawing_settings() {
  QArray<StateSpec> st(1);
  QArray<ColorSpec> co(1);
  
  st[0].set("write name:type \nhorizontally", &write_horizontally);
  co[0].set("class instance color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();
}

bool SdClassInstCanvas::has_drawing_settings() const {
  return TRUE;
}

void SdClassInstCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<StateSpec> st(1);
  QArray<ColorSpec> co(1);
  Uml3States write_horizontally;
  UmlColor itscolor;
  
  st[0].set("write name:type \nhorizontally", &write_horizontally);
  co[0].set("class instance color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      if (st[0].name != 0)
	((SdClassInstCanvas *) it.current())->write_horizontally =
	  write_horizontally;
      if (co[0].name != 0)
	((SdClassInstCanvas *) it.current())->itscolor = itscolor;
      ((SdClassInstCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

void SdClassInstCanvas::set_type(BrowserClass * t) {
  if (t != cl) {
    disconnect(cl->get_data(), 0, this, 0);
    cl = t;
    connect(cl->get_data(), SIGNAL(changed()), this, SLOT(modified()));
    connect(cl->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  }
}

void SdClassInstCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "classinstance_ref " << get_ident() << " // "
      << full_name();
  else {
    nl_indent(st);
    st << "classinstance " << get_ident() << ' ';
    cl->save(st, TRUE, warning);
    nl_indent(st);
    if (itscolor != UmlDefaultColor)
      st << "  color " << stringify(itscolor);
    if (write_horizontally != UmlDefaultState)
      st << "  write_horizontally " << stringify(write_horizontally);
    st << "  name ";
    save_string(get_name(), st);
    st << ' ';
    SdObjCanvas::save(st);
  }
}

SdClassInstCanvas * SdClassInstCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "classinstance_ref"))
    return ((SdClassInstCanvas *) dict_get(read_id(st), "classinstance", canvas));
  else if (!strcmp(k, "classinstance")) {
    int id = read_id(st);
    SdClassInstCanvas * result =
      new SdClassInstCanvas(BrowserClass::read_ref(st), canvas, 0, id);

    k = read_keyword(st);
    
    read_color(st, "color", result->itscolor, k);
    
    if (!strcmp(k, "write_horizontally") ||
	!strcmp(k, "write_horizontaly")) {
      result->write_horizontally = state(read_keyword(st));
      k = read_keyword(st);
    }

    if (strcmp(k, "name"))
      wrong_keyword(k, "name");
    result->set_name(read_string(st));
    result->compute_size();
    result->SdObjCanvas::read(st);
    result->show();
    return result;
  }
  else
    return 0;
}

void SdClassInstCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(cl->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

void SdClassInstCanvas::history_load(QBuffer & b) {
  SdObjCanvas::history_load(b);
  connect(cl->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(cl->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}
