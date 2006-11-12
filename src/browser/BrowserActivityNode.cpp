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

#include "BrowserActivityNode.h"
#include "BrowserFlow.h"
#include "FlowData.h"
#include "ParameterData.h"
#include "ActivityObjectData.h"
#include "PinData.h"
#include "ReferenceDialog.h"
#include "UmlDrag.h"
#include "UmlPixmap.h"
#include "BasicDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserActivityNode> BrowserActivityNode::all(257);
QStringList BrowserActivityNode::its_default_stereotypes;	// unicode

BrowserActivityNode::BrowserActivityNode(UmlCode c, QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserActivityNode>(all, id),
      kind(c), def(new SimpleData) {
  def->set_browser_node(this);
}

BrowserActivityNode::BrowserActivityNode(int id)
    : BrowserNode(), Labeled<BrowserActivityNode>(all, id),
      kind(UmlActivityNode), def(new SimpleData) {
  // not yet read
  def->set_browser_node(this);
}

BrowserActivityNode::BrowserActivityNode(const BrowserActivityNode * model, BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserActivityNode>(all, 0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
  kind = model->kind;
}

BrowserNode * BrowserActivityNode::duplicate(BrowserNode * p, QString name) {
  BrowserNode * result = new BrowserActivityNode(this, p);
  
  result->set_name(name);
  result->update_stereotype();
  return result;
}

BrowserActivityNode::~BrowserActivityNode() {
  all.remove(get_ident());
  delete def;
}

void BrowserActivityNode::clear(bool old)
{
  all.clear(old);
}

void BrowserActivityNode::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserActivityNode::referenced_by(QList<BrowserNode> & l) {
  BrowserNode::referenced_by(l);
  BrowserFlow::compute_referenced_by(l, this);
}

void BrowserActivityNode::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserActivityNode::pixmap(int) const {
  switch (kind) {
  case InitialAN:
    if (deletedp()) 
      return DeletedInitialIcon;
    else if (is_marked)
      return InitialMarkedIcon;
    else
      return InitialIcon;
  case ActivityFinalAN:
    if (deletedp())
      return DeletedFinalIcon;
    else if (is_marked)
      return FinalMarkedIcon;
    else
      return FinalIcon;
  case FlowFinalAN:
    return (deletedp()) ? DeletedExitPointIcon : ExitPointIcon;
  case DecisionAN:
    if (deletedp())
      return DeletedDecisionIcon;
    else if (is_marked)
      return DecisionMarkedIcon;
    else
      return DecisionIcon;
  case MergeAN:
    if (deletedp())
      return DeletedMergeIcon;
    else if (is_marked)
      return MergeMarkedIcon;
    else
      return MergeIcon;
  case ForkAN:
    if (deletedp())
      return DeletedForkIcon;
    else if (is_marked)
      return ForkMarkedIcon;
    else
      return ForkIcon;
  default: // JoinAN:
    if (deletedp())
      return DeletedJoinIcon;
    else if (is_marked)
      return JoinMarkedIcon;
    else
      return JoinIcon;
  }
}

bool BrowserActivityNode::target_of_flow() {
  QList<BrowserNode> l;

  BrowserFlow::compute_referenced_by(l, this);
  return !l.isEmpty();
}

// add flow (dependency not allowed)
BasicData * BrowserActivityNode::add_relation(UmlCode, BrowserNode * end) {
  return (new BrowserFlow(this, end))->get_data();
}

// a flow may be added (dependency not allowed) ?
const char * BrowserActivityNode::may_start() const {
  switch (get_type()) {
  case FlowFinalAN:
  case ActivityFinalAN:
    return "can't have outgoing flow";
  case InitialAN:
  case MergeAN:	      // theo all input and output must
  case JoinAN:	      // be control/data exclusively
    // only one flow is allowed
    {
      QListViewItem * child;
      
      for (child = firstChild(); child != 0; child = child->nextSibling())
	if ((((BrowserNode *) child)->get_type() == UmlFlow) &&
	    (! ((BrowserNode *) child)->deletedp()))
	  return "can't have several outgoing flows";
    }
    // no break
  default:
    return 0;
  }
}

// connexion by a flow (dependency not allowed)
const char * BrowserActivityNode::may_connect(const BrowserNode * dest) const {
  BrowserNode * bn = dest->get_container(UmlActivity);
  
  if ((bn != 0) && (get_container(UmlActivity) != bn))
    return "not in the same activity";

  BasicData * data = dest->get_data();

  switch (dest->get_type()) {
  case InitialAN:
    return "initial node can't have incoming flow";
  case ForkAN:
    return (((BrowserActivityNode *) dest)->target_of_flow())
      ? "fork can't have several incoming flow"
      : 0;
  case UmlParameter:
    if (((ParameterData *) data)->get_dir() == UmlIn)
      return "an input parameter can't have incoming flows";
    else if (! ((ParameterData *) data)->get_is_control())
      return "parameter can't accept control flow (not 'is_control')";
    else
      return 0;
  case UmlActivityObject:
  case UmlExpansionNode:
    return (!((ActivityObjectData *) data)->get_is_control())
      ? "can't accept control flow (not 'is_control')"
      : 0;
  case UmlActivityPin:
    return (!((PinData *) data)->get_is_control())
      ? "pin can't accept control flow (not 'is_control')"
      : 0;
  case UmlActivityAction:
  case FlowFinalAN:
  case ActivityFinalAN:
  case DecisionAN:
  case MergeAN:	      // theo all input and output must
  case JoinAN:	      // be control/data exclusively
    return 0;
  default:
    return "illegal";
  }
}

BrowserActivityNode *
BrowserActivityNode::add_activitynode(BrowserNode * future_parent,
				      UmlCode c, const char * name)
{
  BrowserActivityNode * r =
    new BrowserActivityNode(c, name, future_parent);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return r;
}

BrowserActivityNode * BrowserActivityNode::get_activitynode(BrowserNode * future_parent, UmlCode c) {
  BrowserActivityNode * r = new BrowserActivityNode(c, "", future_parent);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return r;
}

void BrowserActivityNode::menu() {
  QString s = stringify(kind);
  int index = s.find("_");
  
  if (index != -1)
    s.replace(index, 1, " ");
  
  QPopupMenu m(0, "Activity node");
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(s, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    m.setWhatsThis(m.insertItem("edit", 1),
		   "to edit the <em>" + s + "</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only) {
      m.setWhatsThis(m.insertItem("duplicate", 2),
		     "to copy the <em>" + s + "</em> in a new one");
      m.insertSeparator();
      if (edition_number == 0)
	m.setWhatsThis(m.insertItem("delete", 3),
		       "to delete the <em>" + s + "</em>. \
Note that you can undelete it after");
    }
    m.setWhatsThis(m.insertItem("referenced by", 5),
		   "to know who reference the <i>" + s + "</i> \
through a flow");
    mark_menu(m, s, 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("undelete", 4),
		   "to undelete the <em>" + s + "</em>");
  }
  
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 1:
    open(TRUE);
    return;
  case 2:
    duplicate((BrowserNode *) parent())->select_in_browser();
    break;
  case 3:
    delete_it();
    break;
  case 4:
    BrowserNode::undelete(FALSE);
    break;
  case 5:
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

void BrowserActivityNode::open(bool) {
  if (!is_edited) {
    QString s = stringify(kind);
    int index = 0;
    
    while ((index = s.find("_")) != -1)
      s.replace(index, 1, " ");
    
    static QSize previous_size;
    
    (new BasicDialog(get_data(), s, its_default_stereotypes,
		     previous_size))->show();
  }
}

void BrowserActivityNode::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserActivityNode::get_type() const {
  return kind;
}

BasicData * BrowserActivityNode::get_data() const {
  return def;
}

bool BrowserActivityNode::allow_empty() const {
  return TRUE;
}

QString BrowserActivityNode::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString s = name;

  if (s.isEmpty())
    s = "<anonymous activity node>";

  return (rev) ? s + "   [" + p + "]"
	       : p + "::" + s;
}

void BrowserActivityNode::init()
{
  its_default_stereotypes.clear();
}

// unicode
const QStringList & BrowserActivityNode::default_stereotypes()
{
  return its_default_stereotypes;
}

bool BrowserActivityNode::api_compatible(unsigned v) const {
  return (v > 24);
}

bool BrowserActivityNode::tool_cmd(ToolCom * com, const char * args) {
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

void BrowserActivityNode::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserFlow::drag_key(this)))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserActivityNode::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserActivityNode::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserFlow::drag_key(this)))
    e->accept();
  else
    e->ignore();
}

void BrowserActivityNode::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, BrowserFlow::drag_key(this))) != 0)) &&
      (bn != after) && (bn != this)) {
    if (may_contains(bn, FALSE)) 
      move(bn, after);
    else {
      QMessageBox::critical(0, "Error", "Forbiden");
      e->ignore();
    }
  }
  else if ((bn == 0) && (after == 0))
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserActivityNode::drag_key() const {
  return QString::number(UmlActivityNode)
    + "#" + QString::number((unsigned long) get_container(UmlActivity));
}

QString BrowserActivityNode::drag_postfix() const {
  return "#" + QString::number((unsigned long) get_container(UmlActivity));
}

QString BrowserActivityNode::drag_key(BrowserNode * p)
{
  return QString::number(UmlActivityNode)
    + "#" + QString::number((unsigned long) p->get_container(UmlActivity));
}

void BrowserActivityNode::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "activitynode_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserActivityNode::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "activitynode_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserActivityNode::save(QTextStream & st, bool ref, QString & warning) {
  if (ref) {
    st << "activitynode_ref " << get_ident() << " // " << stringify(kind);
    if (!allow_spaces())
      st << ' ' << get_name();
  }
  else {
    nl_indent(st);
    st << "activitynode " << get_ident() << ' ' << stringify(kind) << ' ';
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

BrowserActivityNode * BrowserActivityNode::read_ref(char * & st)
{
  read_keyword(st, "activitynode_ref");
  
  int id = read_id(st);
  BrowserActivityNode * result = all[id];
  
  return (result == 0)
    ? new BrowserActivityNode(id)
    : result;
}

BrowserActivityNode * BrowserActivityNode::read(char * & st, char * k,
						BrowserNode * parent)
{
  BrowserActivityNode * result;
  int id;
  
  if (!strcmp(k, "activitynode_ref")) {
    id = read_id(st);
    result = all[id];
    
    return (result == 0)
      ? new BrowserActivityNode(id)
      : result;
  }
  else if (!strcmp(k, "activitynode")) {
    id = read_id(st);
    UmlCode c = activity_node_kind(read_keyword(st));
    result = all[id];
    
    if (result == 0)
      result = 
	new BrowserActivityNode(c, read_string(st), parent, id);
    else {
      result->kind = c;
      result->set_parent(parent);
      result->set_name(read_string(st));
    }
    
    k = read_keyword(st);
    result->def->read(st, k);
    
    result->BrowserNode::read(st, k);
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    if (strcmp(k, "end")) {
      while (BrowserFlow::read(st, k, result))
	k = read_keyword(st);
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserActivityNode::get_it(const char * k, int id)
{
  // flow managed in activity
  return (!strcmp(k, "activitynode_ref")) ? all[id] : 0;
}
