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
#include <qcursor.h>
#include <qpopupmenu.h> 

#include "UcClassCanvas.h"
#include "RelationCanvas.h"
#include "RelationData.h"
#include "BasicData.h"
#include "UmlPixmap.h"
#include "BrowserClass.h"
#include "LabelCanvas.h"
#include "BrowserUseCaseDiagram.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"

UcClassCanvas::UcClassCanvas(BrowserNode * bn, UmlCanvas * canvas,
			     int x, int y, int id)
    : DiagramCanvas(bn, canvas, x, y, ACTOR_CANVAS_SIZE,
		    ACTOR_CANVAS_SIZE, id) {
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}

UcClassCanvas::~UcClassCanvas() {
}

void UcClassCanvas::delete_it() {
  disconnect(browser_node->get_data(), 0, this, 0);
  DiagramCanvas::delete_it();
}

void UcClassCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void UcClassCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void UcClassCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  p.setBackgroundMode(QObject::OpaqueMode);
  
  QRect r = rect();
  
  draw_actor(&p, r);
  
  if (selected())
    show_mark(p, r);
}

void UcClassCanvas::modified() {
  label->set_name(browser_node->get_name());
  force_self_rel_visible();
  package_modified();
}

UmlCode UcClassCanvas::type() const {
  return UmlClass;
}

void UcClassCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool UcClassCanvas::alignable() const {
  return TRUE;
}

bool UcClassCanvas::copyable() const {
  return selected();
}

void UcClassCanvas::open() {
  browser_node->open(FALSE);
}

void UcClassCanvas::menu(const QPoint&) {

  QPopupMenu m(0);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("upper", 0);
  m.insertItem("lower", 1);
  m.insertSeparator();
  if (browser_node->is_writable()) {
    m.insertItem("edit", 7);
    m.insertSeparator();
  }
  m.insertItem("select in browser",2);
  if (linked())
    m.insertItem("select linked items", 3);
  m.insertSeparator();
  m.insertItem("remove from view",5);
  if (browser_node->is_writable())
    m.insertItem("delete from model", 6);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, UmlClass, 10))
    m.insertItem("tool", &toolm);

  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    upper();
    hide();
    show();
    break;
  case 1:
    lower();
    hide();
    show();
    break;
  case 2:
    browser_node->select_in_browser();
    return;
  case 3:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 5:
    // remove from view
    delete_it();
    break;
  case 6:
    // delete from model
    browser_node->delete_it();	// will delete the canvas
    break;
  case 7:
    browser_node->open(TRUE);
    break;
  default:
    if (rank >= 10)
      ToolCom::run(Tool::command(rank - 10), browser_node);
    return;
  }
  
  package_modified();
}

const char * UcClassCanvas::may_start(UmlCode & c) const {
  switch (c) {
  case UmlDirectionalAssociation:
  case UmlAssociation:
  case UmlAnchor:
    return 0;
  case UmlGeneralisation:
  case UmlDependency:
    return (browser_node->is_writable()) ? 0 : "read only";
  default:
    return "illegal";
  }
}

const char * UcClassCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlUseCase:
    return ((l == UmlAssociation) || (l == UmlDirectionalAssociation))
      ? 0 : "illegal";
  case UmlClass:
    return ((l == UmlGeneralisation) || (l == UmlDependency))
      ? ((BrowserClass *) browser_node)
	->may_connect(l, (BrowserClass *) ((const UcClassCanvas *) dest)->browser_node)
      : "illegal";
  case UmlNote:
  case UmlIcon:
    return (l == UmlAnchor) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

bool UcClassCanvas::has_relation(BasicData * def) const {
  QListIterator<ArrowCanvas> it(lines);
	
  while (it.current()) {
    if (IsaRelation(it.current()->type()) &&
	(((RelationCanvas *) it.current())->get_data() == def))
      return TRUE;
    ++it;
  }
  
  return FALSE;
}

void UcClassCanvas::connexion(UmlCode action, DiagramItem * dest,
			      const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if ((dest->type() == UmlClass) && IsaRelation(action))
    a = new RelationCanvas(the_canvas(), this, dest, 0, action, 0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

bool UcClassCanvas::move_with_its_package() const {
  return TRUE;
}

void UcClassCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "classcanvas_ref " << get_ident() << " // " << browser_node->get_name();
  else {
    nl_indent(st);
    st << "classcanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    nl_indent(st);
    save_xyz(st, this, "  xyz");
    save_xy(st, label, " label_xy");
  }
}

UcClassCanvas * UcClassCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "classcanvas_ref"))
    return ((UcClassCanvas *) dict_get(read_id(st), "classcanvas", canvas));
  else if (!strcmp(k, "classcanvas")) {
    int id = read_id(st);
    BrowserNode * br = BrowserClass::read_ref(st);
    
    read_keyword(st, "xyz");
    
    int x = (int) read_double(st);
    UcClassCanvas * result =
      new UcClassCanvas(br, canvas, x, (int) read_double(st), id);
    
    result->setZ(read_double(st));
    
    k = read_keyword(st);
    read_xy(st, result->label);
    result->label->set_center100();
    
    if (!strcmp(k, "label_xyz"))
      // old version
      read_double(st);
    else if (strcmp(k, "label_xy"))
      wrong_keyword(k, "label_xy/label_xyz");

    result->show();
    return result;
  }
  else
    return 0;
}

void UcClassCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(browser_node->get_data(), 0, this, 0);
}

void UcClassCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}
