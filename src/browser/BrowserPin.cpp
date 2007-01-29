// *************************************************************************
//
// Copyright (C) 2004-2007 Bruno PAGES  All rights reserved.
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
#include <qpainter.h>
#include <qcursor.h>

#include "BrowserPin.h"
#include "PinData.h"
#include "BrowserActivityAction.h"
#include "ActivityActionData.h"
#include "BrowserFlow.h"
#include "BrowserClass.h"
#include "ParameterData.h"
#include "BrowserActivityNode.h"
#include "ActivityObjectData.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserPin> BrowserPin::all(257);
QStringList BrowserPin::its_default_stereotypes;	// unicode

BrowserPin::BrowserPin(QString s, BrowserNode * p, PinData * d, int id)
    : BrowserNode(s, p), Labeled<BrowserPin>(all, id), def(d) {
}

BrowserPin::BrowserPin(int id)
    : BrowserNode(), Labeled<BrowserPin>(all, id),
      def(new PinData) {
  // not yet read
  def->set_browser_node(this);
}

BrowserPin::BrowserPin(const BrowserPin * model, BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserPin>(all, 0) {
  def = new PinData(model->def, this);
  comment = model->comment;
}

BrowserNode * BrowserPin::duplicate(BrowserNode * p, QString name) {
  BrowserNode * result = new BrowserPin(this, p);
  
  result->set_name(name);
  result->update_stereotype();
  
  return result;
}

BrowserPin::~BrowserPin() {
  all.remove(get_ident());
  delete def;
}

void BrowserPin::clear(bool old)
{
  all.clear(old);
}

void BrowserPin::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserPin::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserPin::pixmap(int) const {
  if (deletedp())
    return DeletedPinIcon;
  else if (markedp())
    return PinMarkedIcon;
  else
    return PinIcon;
}

void BrowserPin::compute_referenced_by(QList<BrowserNode> & l,
				       BrowserNode * target)
{
  IdIterator<BrowserPin> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp()) {
      const AType & t = it.current()->def->get_type();
      
      if (t.type == target)
	l.append(it.current());
    }
    ++it;
  }
}

void BrowserPin::new_one(BrowserNode * future_parent, 
			 const char * name, const PinParamData & pd)
{
  PinData * d = new PinData(pd);
  BrowserPin * r = new BrowserPin(name, future_parent, d);
  
  d->set_browser_node(r);
}


BrowserPin * BrowserPin::add_pin(BrowserPin * pin, BrowserNode * future_parent)
{
  QString name;
  
  if (!future_parent->enter_child_name(name, "enter pin's \nname (may be empty) : ",
				       UmlActivityPin, TRUE, TRUE))
    return 0;
      
  BrowserPin * r;

  if (pin == 0) {
    PinData * d = new PinData;
  
    r = new BrowserPin(name, future_parent, d);
    d->set_browser_node(r);
  }
  else
    r = (BrowserPin *) pin->duplicate(future_parent, name);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return r;
}

// add flow (dependency not allowed)
BasicData * BrowserPin::add_relation(UmlCode, BrowserNode * end) {
  return (new BrowserFlow(this, end))->get_data();
}

// a flow may be always added  (dependency not allowed) ?
const char * BrowserPin::may_start() const {
  switch (def->get_dir()) {
  case UmlOut:
  case UmlReturn:
    return 0;
  default:
    return "can't have outgoing flow";
  }
}

// connexion by a flow (dependency not allowed)
const char * BrowserPin::may_connect(const BrowserNode * dest) const {
  BrowserNode * bn = dest->get_container(UmlActivity);
  
  if ((bn != 0) && (get_container(UmlActivity) != bn))
    return "not in the same activity";

  switch (dest->get_type()) {
  case InitialAN:
    return "initial node can't have incoming flow";
  case ForkAN:  // theo all input and output must be control/data exclusively
    if (((BrowserActivityNode *) dest)->target_of_flow())
      return "fork can't have several incoming flow";
    else
      return 0;
  case FlowFinalAN:
  case ActivityFinalAN:
    if (! def->get_is_control())
      return "can't have outgoing control flow (not 'is_control')";
    else
      return 0;
  case UmlParameter:
    {
      ParameterData * data = (ParameterData *) dest->get_data();

      if (data->get_dir() == UmlIn)
	return "an input parameter can't have incoming flows";
      else if (data->get_is_control() != def->get_is_control())
	return (def->get_is_control())
	? "parameter can't accept control flow (not 'is_control')"
	: "parameter can't accept data flow (is 'is_control')";
      else
	return 0;
    }
  case UmlActivityPin:
    {
      PinData * data = (PinData *) dest->get_data();

      if (data->get_dir() != UmlIn)
	return "target pin can't have incoming flow";

      if (data->get_is_control() != def->get_is_control())
	return (def->get_is_control())
	  ? "target pin can't accept control flow (not 'is_control')"
	  : "target pin can't accept data flow (is 'is_control')";
      else
	return 0;
    }
  case UmlExpansionNode:
  case UmlActivityObject:
    {
      ActivityObjectData * data = (ActivityObjectData *) dest->get_data();

      if (data->get_is_control() != def->get_is_control())
	return (def->get_is_control())
	  ? "can't accept control flow (not 'is_control')"
	  : "can't accept data flow (is 'is_control')";
      else
	return 0;
    }
  case UmlActivityAction:
  case DecisionAN:    // theo all input and
  case MergeAN:	      // output must be
  case JoinAN:	      // control/data exclusively
    return 0;
  default:
    return "illegal";
  }
}

void BrowserPin::menu() {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle((name.isEmpty()) ? "pin" : name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_edited)
    m.setWhatsThis(m.insertItem("Edit", 0),
		   "to edit the <em>pin</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only && (edition_number == 0)) {
      if (((ActivityActionData *) ((BrowserNode *) parent())->get_data())->may_add_pin()) {
	m.setWhatsThis(m.insertItem("Duplicate", 1),
		       "to copy the <em>pin</em> in a new one");
	m.insertSeparator();
      }
      m.setWhatsThis(m.insertItem("Delete", 2),
		     "to delete the <em>pin</em>. \
Note that you can undelete it after");
    }
    mark_menu(m, "pin", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only &&
	   (edition_number == 0) &&
	   ((ActivityActionData *) ((BrowserNode *) parent())->get_data())->may_add_pin())
    m.setWhatsThis(m.insertItem("Undelete", 3),
		   "to undelete the <em>pin</em>");
  
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserPin::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    open(FALSE);
    return;
  case 1:
    add_pin(this, (BrowserNode *) parent());
    return;
  case 2:
    delete_it();
    break;
  case 3:
    undelete(FALSE);
    break;
  default:
    if (rank >= 100)
      ToolCom::run(Tool::command(rank - 100), this);
    else
      mark_management(rank - 90);
    return;
  }
  ((BrowserNode *) parent())->modified();
  package_modified();
}

void BrowserPin::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (!is_edited)
      if (s == "Edit")
	choice = 0;
    if (!is_read_only && (edition_number == 0)) {
      if (((ActivityActionData *) ((BrowserNode *) parent())->get_data())->may_add_pin()) {
	if (s == "Duplicate")
	  choice = 1;
      }
      if (s == "Delete")
	choice = 2;
    }
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only &&
	   (edition_number == 0) &&
	   ((ActivityActionData *) ((BrowserNode *) parent())->get_data())->may_add_pin())
    if (s == "Undelete")
      choice = 3;
  
  exec_menu_choice(choice);
}

void BrowserPin::open(bool) {
  if (!is_edited)
    def->edit();
}

void BrowserPin::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserPin::get_type() const {
  return UmlActivityPin;
}

BasicData * BrowserPin::get_data() const {
  return def;
}

bool BrowserPin::allow_empty() const {
  return TRUE;
}

QString BrowserPin::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString s = name;

  if (s.isEmpty())
    s = "<anonymous pin>";

  return (rev) ? s + "   [" + p + "]"
	       : p + "::" + s;
}

bool BrowserPin::api_compatible(unsigned v) const {
  return (v > 24);
}

bool BrowserPin::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd: 
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	UmlCode k = com->get_kind(args);
	
	switch (k) {
	case UmlFlow:
	  {
	    BrowserNode * end = (BrowserNode *) com->get_id(args);
	    
	    if (may_connect(end) == 0)
	      (new BrowserFlow(this, end))->write_id(com);
	    else
	      ok = FALSE;
	  }
	  break;
	default:
	  ok = FALSE;
	}
      }
      
      if (! ok)
	com->write_id(0);
      else
	package_modified();
      
      return TRUE;
    }
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

void BrowserPin::DragMoveEvent(QDragMoveEvent * e) {
  ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserPin::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserPin::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserPin::drag_key() const {
  return QString::number(UmlActivityPin)
      + "#" + QString::number((unsigned long) parent());
}

QString BrowserPin::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserPin::drag_key(BrowserNode * p)
{
  return QString::number(UmlActivityPin)
    + "#" + QString::number((unsigned long) p);
}

void BrowserPin::init()
{
  its_default_stereotypes.clear();
}

// unicode
const QStringList & BrowserPin::default_stereotypes()
{
  return its_default_stereotypes;
}

void BrowserPin::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "pin_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserPin::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "pin_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserPin::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "pin_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "pin " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    BrowserNode::save(st);

    // saves the sub elts
      
    QListViewItem * child = firstChild();
    
    if (child != 0) {
      for (;;) {
	if (! ((BrowserNode *) child)->deletedp()) {
	  ((BrowserNode *) child)->save(st, FALSE, warning);
	  
	  child = child->nextSibling();
	  if (child != 0)
	    st << '\n';
	  else
	    break;
	}
	else if ((child = child->nextSibling()) == 0)
	  break;
      }
    }

    indent(-1);
    nl_indent(st);
    st << "end";
    
    // for saveAs
    if (! is_api_base())
      is_read_only = FALSE;
  }
}

BrowserPin * BrowserPin::read_ref(char * & st)
{
  read_keyword(st, "pin_ref");
  
  int id = read_id(st);
  BrowserPin * result = all[id];
  
  return (result == 0)
    ? new BrowserPin(id)
    : result;
}

BrowserPin * BrowserPin::read(char * & st, char * k, BrowserNode * parent)
{
  BrowserPin * result;
  int id;
  
  if (!strcmp(k, "pin_ref")) {
    if ((result = all[id = read_id(st)]) == 0)
      result = new BrowserPin(id);
    return result;
  }
  else if (!strcmp(k, "pin")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    k = read_keyword(st);
    
    if ((result = all[id]) == 0) {
      result = new BrowserPin(s, parent, new PinData, id);
      result->def->read(st, k);	// updates k2
    }
    else {
      result->def->read(st, k);	// updates k2
      result->set_parent(parent);
      result->set_name(s);
    }
    
    result->BrowserNode::read(st, k);
    
    if (strcmp(k, "end")) {
      while (BrowserFlow::read(st, k, result))
	k = read_keyword(st);
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    result->is_read_only = (!in_import() && read_only_file()) || 
      (user_id() != 0) && result->is_api_base();
    result->def->set_browser_node(result);
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserPin::get_it(const char * k, int id)
{
  return (!strcmp(k, "pin_ref")) ? all[id] : 0;
}
