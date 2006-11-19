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
#include <qmessagebox.h>
#include <qpainter.h>
#include <qcursor.h>

#include "BrowserExpansionNode.h"
#include "ActivityObjectData.h"
#include "BrowserExpansionRegion.h"
#include "BrowserFlow.h"
#include "BrowserClass.h"
#include "BrowserActivityNode.h"
#include "ParameterData.h"
#include "ReferenceDialog.h"
#include "PinData.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserExpansionNode> BrowserExpansionNode::all(257);
QStringList BrowserExpansionNode::its_default_stereotypes;	// unicode

BrowserExpansionNode::BrowserExpansionNode(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserExpansionNode>(all, id),
      def(new ActivityObjectData) {
  def->set_browser_node(this);
}

BrowserExpansionNode::BrowserExpansionNode(int id)
    : BrowserNode(), Labeled<BrowserExpansionNode>(all, id),
      def(new ActivityObjectData) {
  // not yet read
  def->set_browser_node(this);
}

BrowserExpansionNode::BrowserExpansionNode(const BrowserExpansionNode * model, BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserExpansionNode>(all, 0) {
  def = new ActivityObjectData(model->def, this);
  comment = model->comment;
}

BrowserExpansionNode *
  BrowserExpansionNode::get_expansionnode(BrowserNode * future_parent)
{
  BrowserNode * old = 0;
  QString name;
  
  if (!future_parent->enter_child_name(name, "enter expansion node's \nname (may be empty) : ",
				       UmlExpansionNode, TRUE, TRUE))
    return 0;
    
  if (old != 0)
    return ((BrowserExpansionNode *) old);
  
  BrowserExpansionNode * r = new BrowserExpansionNode(name, future_parent);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return r;
}

BrowserNode * BrowserExpansionNode::duplicate(BrowserNode * p, QString name) {
  BrowserNode * result = new BrowserExpansionNode(this, p);
  
  result->set_name((const char *) name);
  result->update_stereotype();
  
  return result;
}

BrowserExpansionNode::~BrowserExpansionNode() {
  all.remove(get_ident());
  delete def;
}

void BrowserExpansionNode::clear(bool old)
{
  all.clear(old);
}

void BrowserExpansionNode::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserExpansionNode::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserExpansionNode::pixmap(int) const {
  if (deletedp())
    return DeletedExpansionNodeIcon;
  else if (is_marked)
    return ExpansionNodeMarkedIcon;
  else
    return ExpansionNodeIcon;
}

// add flow (dependency not allowed)
BasicData * BrowserExpansionNode::add_relation(UmlCode, BrowserNode * end) {
  return (new BrowserFlow(this, end))->get_data();
}

// a flow may be added in all the cases
const char * BrowserExpansionNode::may_start() const {
  return 0;
}

// connexion by a flow
const char * BrowserExpansionNode::may_connect(const BrowserNode * dest) const {
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
  case UmlActivityPin:
    {
      PinData * data = (PinData *) dest->get_data();

      if (data->get_is_control() != def->get_is_control())
	return (def->get_is_control())
	  ? "pin can't accept control flow (not 'is_control')"
	  : "pin can't accept data flow (is 'is_control')";
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

void BrowserExpansionNode::menu() {
  QString s = name;
  
  if (s.isEmpty())
    s = "expansion node";
  
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(s, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_edited)
    m.setWhatsThis(m.insertItem("Edit", 0),
		   "to edit the <em>expansion node</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only && (edition_number == 0)) {
      m.setWhatsThis(m.insertItem("Duplicate", 1),
		     "to copy the <em>expansion node</em> in a new one");
      m.insertSeparator();
      m.setWhatsThis(m.insertItem("Delete", 2),
		     "to delete the <em>expansion node</em>. \
Note that you can undelete it after");
    }
    m.setWhatsThis(m.insertItem("Referenced by", 4),
		   "to know who reference the <i>expansion node</i> \
through a flow");
    mark_menu(m, "expansion node", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("Undelete", 3),
		   "to undelete the <em>expansion node</em>");
  
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    open(FALSE);
    return;
  case 1:
    {
      QString name;
      
      if (((BrowserNode *) parent())->enter_child_name(name, "enter expansion node's \nname (may be empty) : ",
						       UmlExpansionNode, TRUE, TRUE))
	duplicate((BrowserNode *) parent(), name)->select_in_browser();
    }
    break;
  case 2:
    delete_it();
    break;
  case 3:
    undelete(FALSE);
    break;
  case 4:
    ReferenceDialog::show(this);
    return;
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

void BrowserExpansionNode::open(bool) {
  if (!is_edited)
    def->edit("Expansion Node", its_default_stereotypes);
}

void BrowserExpansionNode::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserExpansionNode::get_type() const {
  return UmlExpansionNode;
}

BasicData * BrowserExpansionNode::get_data() const {
  return def;
}

bool BrowserExpansionNode::allow_empty() const {
  return TRUE;
}

QString BrowserExpansionNode::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString s = name;

  if (s.isEmpty())
    s = "<anonymous expansion node>";

  return (rev) ? s + "   [" + p + "]"
	       : p + "::" + s;
}

void BrowserExpansionNode::referenced_by(QList<BrowserNode> & l) {
  BrowserNode::referenced_by(l);
  BrowserFlow::compute_referenced_by(l, this);
}

void BrowserExpansionNode::compute_referenced_by(QList<BrowserNode> & l,
						 BrowserNode * target)
{
  IdIterator<BrowserExpansionNode> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp()) {
      const AType & t = it.current()->def->get_type();
      
      if (t.type == target)
	l.append(it.current());
    }
    ++it;
  }
}

bool BrowserExpansionNode::tool_cmd(ToolCom * com, const char * args) {
  return (def->tool_cmd(com, args, this, comment) ||
	  BrowserNode::tool_cmd(com, args));
}

void BrowserExpansionNode::DragMoveEvent(QDragMoveEvent * e) {
  ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserExpansionNode::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserExpansionNode::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserExpansionNode::drag_key() const {
  return QString::number(UmlParameter)
      + "#" + QString::number((unsigned long) parent());
}

QString BrowserExpansionNode::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserExpansionNode::drag_key(BrowserNode * p)
{
  return QString::number(UmlParameter)
    + "#" + QString::number((unsigned long) p);
}

void BrowserExpansionNode::init()
{
  its_default_stereotypes.clear();
}

// unicode
const QStringList & BrowserExpansionNode::default_stereotypes()
{
  return its_default_stereotypes;
}

void BrowserExpansionNode::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "expansionnode_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserExpansionNode::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "expansionnode_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserExpansionNode::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "expansionnode_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "expansionnode " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning, "expansion node");
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

BrowserExpansionNode * BrowserExpansionNode::read_ref(char * & st)
{
  read_keyword(st, "expansionnode_ref");
  
  int id = read_id(st);
  BrowserExpansionNode * result = all[id];
  
  return (result == 0)
    ? new BrowserExpansionNode(id)
    : result;
}

BrowserExpansionNode *
  BrowserExpansionNode::read(char * & st, char * k,
			     BrowserNode * parent)
{
  BrowserExpansionNode * result;
  int id;
  
  if (!strcmp(k, "expansionnode_ref")) {
    if ((result = all[id = read_id(st)]) == 0)
      result = new BrowserExpansionNode(id);
    return result;
  }
  else if (!strcmp(k, "expansionnode")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    k = read_keyword(st);
    
    if ((result = all[id]) == 0) {
      result = new BrowserExpansionNode(s, parent, id);
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
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserExpansionNode::get_it(const char * k, int id)
{
  return (!strcmp(k, "expansionnode_ref")) ? all[id] : 0;
}
