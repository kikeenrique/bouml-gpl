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

#include <qpopupmenu.h> 
#include <qcursor.h>

#include "CodClassInstCanvas.h"
#include "CodSelfLinkCanvas.h"
#include "BrowserClass.h"
#include "BrowserDiagram.h"
#include "UmlCanvas.h"
#include "ClassData.h"
#include "InstanceDialog.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "ColDiagramView.h"

CodClassInstCanvas::CodClassInstCanvas(BrowserClass * t, UmlCanvas * canvas,
				       int x, int y, int id)
    : CodObjCanvas(0, canvas, x, y, CLASSINST_CANVAS_MIN_SIZE, 1, id),
      ClassInstCanvas(t) {
  browser_node = canvas->browser_diagram();
  itscolor = UmlDefaultColor;
  compute_size();
  
  connect(cl->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(cl->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

CodClassInstCanvas::~CodClassInstCanvas() {
}

void CodClassInstCanvas::delete_it() {
  disconnect(cl->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  CodObjCanvas::delete_it();
}

void CodClassInstCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void CodClassInstCanvas::compute_size() {
  int wi, he;
  
  ClassInstCanvas::compute_size(wi, he, the_canvas(), TRUE);
  resize(wi, he);
}

void CodClassInstCanvas::modified() {
  if (visible()) {
    hide();
    hide_lines();
    compute_size();
    show();
    update_show_lines();
    if (self_link)
      self_link->update_pos();
    canvas()->update();
    package_modified();
  }
}

void CodClassInstCanvas::draw(QPainter & p) {
  if (visible()) {
    ClassInstCanvas::draw(p, the_canvas(), rect(), TRUE);

    if (selected())
      show_mark(p, rect());
  }
}

UmlCode CodClassInstCanvas::type() const {
  return UmlClass;
}

BrowserClass * CodClassInstCanvas::get_type() {
  return cl;
}

QString CodClassInstCanvas::get_full_name() const {
  return full_name();
}

void CodClassInstCanvas::open() {
  InstanceDialog d(this, "class : ", "Class instance dialog");
  
  d.raise();
  if (d.exec() == QDialog::Accepted)
    modified();
}

void CodClassInstCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle(full_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("Edit", 3);
  m.insertSeparator();
  m.insertItem("Select class in browser", 4);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  m.insertItem("Remove from view", 6);
  
  switch (m.exec(QCursor::pos())) {
  case 0:
    upper();
    modified();	// call package_modified
    return;
  case 1:
    lower();
    modified();	// call package_modified
    return;
  case 2:
    edit_drawing_settings();
    return;
  case 3:
    open();	// call package_modified
    return;
  case 4:
    cl->select_in_browser();
    return;
  case 5:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 6:
    delete_it();
    ((ColDiagramView *) the_canvas()->get_view())->update_msgs();
    package_modified();
  default:
    return;
  }
}

void CodClassInstCanvas::apply_shortcut(QString s) {
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

void CodClassInstCanvas::edit_drawing_settings() {
  QArray<StateSpec> st(1);
  QArray<ColorSpec> co(1);
  
  st[0].set("write name:type \nhorizontally", &write_horizontally);
  co[0].set("class instance color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() != QDialog::Accepted)
    return;
  modified();	// call package_modified
}

bool CodClassInstCanvas::has_drawing_settings() const {
  return TRUE;
}

void CodClassInstCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
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
	((CodClassInstCanvas *) it.current())->write_horizontally =
	  write_horizontally;
      if (co[0].name != 0)
	((CodClassInstCanvas *) it.current())->itscolor = itscolor;
      ((CodClassInstCanvas *) it.current())->modified();	// call package_modified()
    }
  }  
}

void CodClassInstCanvas::set_type(BrowserClass * t) {
  if (t != cl) {
    disconnect(cl->get_data(), 0, this, 0);
    cl = t;
    connect(cl->get_data(), SIGNAL(changed()), this, SLOT(modified()));
    connect(cl->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  }
}

bool CodClassInstCanvas::alignable() const {
  return TRUE;
}

bool CodClassInstCanvas::copyable() const {
  return selected();
}

void CodClassInstCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "classinstance_ref " << get_ident() << " // "
      << full_name();
  else {
    nl_indent(st);
    st << "classinstance " << get_ident() << ' ';
    cl->save(st, TRUE, warning);
    nl_indent(st);
    if (itscolor != UmlDefaultColor)
      st << "  color " << stringify(itscolor) << ' ';
    if (write_horizontally != UmlDefaultState)
      st << "  write_horizontally " << stringify(write_horizontally) << ' ';
    save_xyz(st, this, "  xyz");
    st << " name ";
    save_string(get_name(), st);
    st << ' ';
    save_xyz(st, this, " xyz");
  }
}

CodClassInstCanvas * CodClassInstCanvas::read(char * & st, UmlCanvas * canvas,
					      char * k)
{
  if (!strcmp(k, "classinstance_ref"))
    return ((CodClassInstCanvas *) dict_get(read_id(st), "classinstance", canvas));
  else if (!strcmp(k, "classinstance")) {
    int id = read_id(st);
    BrowserClass * cl = BrowserClass::read_ref(st);
    
    k = read_keyword(st);
    
    UmlColor co = UmlDefaultColor;
    
    read_color(st, "color", co, k);	// updates k
    
    Uml3States ho;
    
    if (!strcmp(k, "write_horizontally") || 
	!strcmp(k, "write_horizontaly")) {
      ho = state(read_keyword(st));
      k = read_keyword(st);
    }
    else
      ho = UmlDefaultState;
    
    if (strcmp(k, "xyz"))
      wrong_keyword(k, "xyz");
    
    int x = (int) read_double(st);
    CodClassInstCanvas * result =
      new CodClassInstCanvas(cl, canvas, x, (int) read_double(st), id);

    result->setZ(read_double(st));
    read_keyword(st, "name");
    result->set_name(read_string(st));
    result->itscolor = co;
    result->write_horizontally = ho;
    read_keyword(st, "xyz");
    read_xyz(st, result);
    result->compute_size();
    result->set_center100();
    result->show();
    return result;
  }
  else
    return 0;
}

void CodClassInstCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(browser_node->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

void CodClassInstCanvas::history_load(QBuffer & b) {
  CodObjCanvas::history_load(b);
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}
