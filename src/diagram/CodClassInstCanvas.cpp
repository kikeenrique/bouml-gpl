// *************************************************************************
//
// Copyleft 2004-2007 Bruno PAGES  .
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
#include "BrowserClassInstance.h"
#include "ClassInstanceData.h"
#include "BrowserDiagram.h"
#include "UmlCanvas.h"
#include "ClassData.h"
#include "InstanceDialog.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "strutil.h"
#include "MenuTitle.h"
#include "ColDiagramView.h"
#include "ToolCom.h"

CodClassInstCanvas::CodClassInstCanvas(BrowserNode * bn, UmlCanvas * canvas,
				       int x, int y, int id)
    : CodObjCanvas(0, canvas, x, y, CLASSINST_CANVAS_MIN_SIZE, 1, id),
      ClassInstCanvas() {
  // bn may a class instance or a class
  browser_node = bn;
  itscolor = UmlDefaultColor;
  
  if (id == 0)
    // not on read
    compute_size();
  
  BasicData * d = bn->get_data();
  
  connect(d, SIGNAL(changed()), this, SLOT(modified()));
  connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

CodClassInstCanvas::~CodClassInstCanvas() {
}

void CodClassInstCanvas::delete_it() {
  BasicData * d = browser_node->get_data();

  disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
  disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  
  CodObjCanvas::delete_it();
  
  ((ColDiagramView *) the_canvas()->get_view())->update_msgs();
}

void CodClassInstCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void CodClassInstCanvas::remove(bool from_model) {
  if (!from_model || (browser_node->get_type() == UmlClass))
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void CodClassInstCanvas::compute_size() {
  int wi, he;
  
  ClassInstCanvas::compute_size(wi, he, the_canvas());
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
    ClassInstCanvas::draw(p, the_canvas(), rect());

    if (selected())
      show_mark(p, rect());
  }
}

// all cases
QString CodClassInstCanvas::get_name() const {
  return (browser_node->get_type() == UmlClass)
    ? iname
    : QString(browser_node->get_name());
}

// out of model case
void CodClassInstCanvas::set_name(const QString & s) {
  iname = s;
}

// UmlClass or UmlClassInstance
UmlCode CodClassInstCanvas::type() const {
  return browser_node->get_type();
}

// return class, all cases
BrowserNode * CodClassInstCanvas::get_type() const {
  return (browser_node->get_type() == UmlClass)
    ? browser_node
    : ((BrowserNode *)
       ((ClassInstanceData *) browser_node->get_data())->get_class());
}

// out of model case
void CodClassInstCanvas::set_type(BrowserNode * t) {
  if (t != browser_node) {
    disconnect(browser_node->get_data(), 0, this, 0);
    browser_node = t;
    connect(t->get_data(), SIGNAL(changed()), this, SLOT(modified()));
    connect(t->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  }
}

BrowserNodeList& CodClassInstCanvas::get_types(BrowserNodeList& r) const {
  return BrowserClass::instances(r);
}

BrowserClass * CodClassInstCanvas::get_class() const {
  return (BrowserClass *) get_type();
}

BrowserNode * CodClassInstCanvas::container(UmlCode c) const {
  return the_canvas()->browser_diagram()->container(c);
}

void CodClassInstCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  if (browser_node->get_type() == UmlClassInstance)
    in_model |=  browser_node->is_writable();
}

QString CodClassInstCanvas::get_full_name() const {
  return full_name();
}

void CodClassInstCanvas::open() {
  if (browser_node->get_type() == UmlClass) {
    InstanceDialog d(this, "class", UmlClass);
    
    d.raise();
    if (d.exec() == QDialog::Accepted)
      modified();
  }
  else
    browser_node->open(FALSE);
}

void CodClassInstCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  bool modelized = (browser_node->get_type() == UmlClassInstance);
  
  m.insertItem(new MenuTitle(full_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("Edit", 3);
  m.insertSeparator();
  if (modelized)
    m.insertItem("Select in browser", 4);
  m.insertItem("Select class in browser", 5);
  if (linked())
    m.insertItem("Select linked items", 6);
  m.insertSeparator();
  if (modelized)
    m.insertItem("Exit from model", 9);
  else {
    if (container(UmlClass)->is_writable())
      m.insertItem("Insert in model", 10);
    m.insertItem("Replace it", 11);
  }
  m.insertSeparator();
  m.insertItem("Remove from view", 7);
  if (modelized && browser_node->is_writable())
    m.insertItem("Delete from model", 8);
  
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
    browser_node->select_in_browser();
    return;
  case 5:
    get_type()->select_in_browser();
    return;
  case 6:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 7:
    delete_it();
    break;
  case 8:
    //delete from model
    browser_node->delete_it();	// will delete the canvas
    break;
  case 9:
    {
      BasicData * d = browser_node->get_data();
      
      disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
      disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
      
      iname = browser_node->get_name();
      browser_node = ((ClassInstanceData *) d)->get_class();
      d = browser_node->get_data();
      connect(d, SIGNAL(changed()), this, SLOT(modified()));
      connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
    }
    break;
  case 10:
    {
      BasicData * d = browser_node->get_data();
      
      disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
      disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
      
      browser_node =
	new BrowserClassInstance(iname, (BrowserClass *) browser_node,
				 container(UmlClass));
      d = browser_node->get_data();
      connect(d, SIGNAL(changed()), this, SLOT(modified()));
      connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
    }
    break;
  case 11:
    {
      BrowserNode * bn = 
	BrowserClassInstance::get_classinstance((BrowserClass *) browser_node);
      
      if (bn == 0)
	return;
      
      BasicData * d = browser_node->get_data();
      
      disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
      disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
      
      browser_node = bn;
      d = browser_node->get_data();
      connect(d, SIGNAL(changed()), this, SLOT(modified()));
      connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
      modified();	// call package_modified
      return;
    }
    break;
  default:
    return;
  }
  
  package_modified();
}

void CodClassInstCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser") { 
    browser_node->select_in_browser();
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
  else if (browser_node->get_type() == UmlClass) {
    nl_indent(st);
    st << "classinstance " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    nl_indent(st);
    ClassInstCanvas::save(st);
    st << " name ";
    save_string(iname, st);
    st << ' ';
    save_xyz(st, this, " xyz");
  }
  else {
    nl_indent(st);
    st << "classinstancecanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);

    indent(+1);
    
    nl_indent(st);
    save_xyz(st, this, "xyz");
    ClassInstCanvas::save(st);
    nl_indent(st);
    st << "end";
    
    indent(-1);
  }
}

CodClassInstCanvas * CodClassInstCanvas::read(char * & st, UmlCanvas * canvas,
					      char * k)
{
  if (!strcmp(k, "classinstance_ref"))
    return ((CodClassInstCanvas *) dict_get(read_id(st), "classinstance", canvas));
  else if (!strcmp(k, "classinstance")) {
    // old release and graphic instance
    int id = read_id(st);
    BrowserClass * cl = BrowserClass::read_ref(st);
    CodClassInstCanvas * result =
      new CodClassInstCanvas(cl, canvas, 0, 0, id);
   
    result->ClassInstCanvas::read(st, k);
    
    if (!strcmp(k, "xyz")) {
      read_double(st);
      read_double(st);
      read_double(st);
      k = read_keyword(st);
    }
    if (strcmp(k, "name"))
      wrong_keyword(k, "name");
    result->iname = read_string(st);
    read_keyword(st, "xyz");
    read_xyz(st, result);
    result->compute_size();
    result->set_center100();
    result->show();
    
    return result;
  }
  else if (!strcmp(k, "classinstancecanvas")) {
    int id = read_id(st);
    BrowserClassInstance * icl = BrowserClassInstance::read_ref(st);

    read_keyword(st, "xyz");
    
    int x = (int) read_double(st);
    CodClassInstCanvas * result =
      new CodClassInstCanvas(icl, canvas, x, (int) read_double(st), id);

    result->setZ(read_double(st));
   
    result->ClassInstCanvas::read(st, k);
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");

    if (result->get_type() != 0) {
      // not a deleted instance
      result->compute_size();
      result->set_center100();
      result->show();
    }
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

// for plug outs

void CodClassInstCanvas::send(ToolCom * com, QCanvasItemList & all)
{
  QList<CodClassInstCanvas> l;
  QCanvasItemList::Iterator cit;

  for (cit = all.begin(); cit != all.end(); ++cit) {
    DiagramItem *di = QCanvasItemToDiagramItem(*cit);
    
    if ((di != 0) && (*cit)->visible()) {
       switch (di->type()) {
       case UmlClass:
       case UmlClassInstance:
	 l.append((CodClassInstCanvas *) di);
	 break;
       default:
	 break;
       }
    }
  }

  com->write_unsigned(l.count());
  
  QListIterator<CodClassInstCanvas> it(l);
  
  for (; it.current(); ++it) {
    CodClassInstCanvas * i = it.current();
    
    com->write_unsigned((unsigned) i->get_ident());
    if (i->browser_node->get_type() == UmlClass) {      
      com->write_id(0);
      
      QCString s = fromUnicode(i->iname);
      
      com->write_string((const char *) s);
    }
    i->browser_node->write_id(com);
  }
}
