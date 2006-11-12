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

#include "BrowserActivity.h"
#include "ActivityData.h"
#include "BrowserFlow.h"
#include "BrowserSimpleRelation.h"
#include "BrowserParameter.h"
#include "BrowserActivityNode.h"
#include "BrowserActivityAction.h"
#include "BrowserActivityObject.h"
#include "BrowserExpansionRegion.h"
#include "BrowserInterruptibleActivityRegion.h"
#include "ActivityActionData.h"
#ifndef WIN32
#warning
#endif
/*
#include "BrowserPartition.h"
*/
#include "BrowserActivityDiagram.h"
#include "ReferenceDialog.h"
#include "UmlDrag.h"
#include "UmlPixmap.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserActivity> BrowserActivity::all;
QStringList BrowserActivity::its_default_stereotypes;	// unicode

BrowserActivity::BrowserActivity(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserActivity>(all, id),
      def(new ActivityData), associated_diagram(0) {
  def->set_browser_node(this);
}

BrowserActivity::BrowserActivity(int id)
    : BrowserNode(), Labeled<BrowserActivity>(all, id),
      def(new ActivityData), associated_diagram(0) {
  // not yet read
  def->set_browser_node(this);
}

BrowserActivity::BrowserActivity(const BrowserActivity * model, BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserActivity>(all, 0), associated_diagram(0) {
  def = new ActivityData(model->def, this);
  comment = model->comment;
}

BrowserNode * BrowserActivity::duplicate(BrowserNode * p, QString name) {
  BrowserNode * result = new BrowserActivity(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype(TRUE);
  
  return result;
}

BrowserActivity::~BrowserActivity() {
  all.remove(get_ident());
  delete def;
}

void BrowserActivity::clear(bool old)
{
  all.clear(old);
}

void BrowserActivity::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserActivity::referenced_by(QList<BrowserNode> & l) {
  BrowserNode::referenced_by(l);
  BrowserActivityAction::compute_referenced_by(l, this);
}

void BrowserActivity::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserActivity::pixmap(int) const {
  return (deletedp()) ? DeletedActivityIcon : ActivityIcon;
}

BrowserActivity * BrowserActivity::add_activity(BrowserNode * future_parent)
{
  QString name;
  
  return (!future_parent->enter_child_name(name, "enter activity's name : ",
					   UmlActivity, TRUE, FALSE))
    
    ? 0
    : add_activity(future_parent, name);
}

BrowserActivity * BrowserActivity::add_activity(BrowserNode * future_parent,
						const char * name)
{
  BrowserActivity * r = new BrowserActivity(name, future_parent);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return r;
}

BrowserActivity * BrowserActivity::get_activity(BrowserNode * parent)
{
  BrowserNodeList l;
  QListViewItem * child;
      
  for (child = parent->firstChild(); child != 0; child = child->nextSibling())
    if (!((BrowserNode *) child)->deletedp() &&
	(((BrowserNode *) child)->get_type() == UmlActivity))
      l.append((BrowserNode *) child);
  
  BrowserNode * old;
  QString name;
  
  if (!parent->enter_child_name(name, "enter activity's name : ",
				UmlActivity, l, &old,
				TRUE, FALSE))
    return 0;
    
  if (old != 0)
    return ((BrowserActivity *) old);
  
  BrowserActivity * r = new BrowserActivity(name, parent);
  
  parent->setOpen(TRUE);
  parent->package_modified();
  
  return r;
}

BrowserNode * BrowserActivity::add_parameter(BrowserParameter * param,
					     const char * s) {
  QString name = s;
  
  if ((s != 0) ||
      enter_child_name(name, "enter parameter's name : ",
		       UmlParameter, TRUE, FALSE)) {
    param = (param == 0) ? new BrowserParameter(name, this)
			 : (BrowserParameter *) param->duplicate(this, name);
    
    setOpen(TRUE);
    def->modified();
    package_modified();
    param->select_in_browser();
    
    return param;
  }
  
  return 0;
}

void BrowserActivity::add_activity_diagram() {
  (new BrowserActivityDiagram(child_random_name("Diagram"), this))->select_in_browser();
}

void BrowserActivity::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(def->definition(FALSE), m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_read_only) {
      m.setWhatsThis(m.insertItem("new activity diagram", 0),
		     "to add a <em>activity diagram</em>");
      m.setWhatsThis(m.insertItem("add parameter", 1),
		     "to add a <em>Parameter</em> to the <em>activity</em>");
      m.setWhatsThis(m.insertItem("add interruptible activity region", 2),
		     "to add an <em>Interruptible Activity Region</em> to the <em>activity</em>");
      m.setWhatsThis(m.insertItem("new expansion region", 3),
		     "to add a nested <em>expansion region</em>");
#if 0
      m.setWhatsThis(m.insertItem("add partition", 4),
		     "to add a <em>Partition</em> to the <em>activity</em>");
#endif
      m.setWhatsThis(m.insertItem("add activity action", 7),
		     "to add an <em>activity action</em> to the <em>activity</em>");
      m.setWhatsThis(m.insertItem("add object node", 8),
		     "to add an <em>activity object node</em> to the <em>activity</em>");
      m.insertSeparator();
    }
    m.setWhatsThis(m.insertItem("edit", 5),
		   "to edit the <em>artivity</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only) {
      m.setWhatsThis(m.insertItem("duplicate", 6),
		     "to copy the <em>activity</em> in a new one");
      m.insertSeparator();
      if (edition_number == 0)
	m.setWhatsThis(m.insertItem("delete", 9),
		       "to delete the <em>activity</em>. \
Note that you can undelete it after");
    }
    m.insertSeparator();
    m.setWhatsThis(m.insertItem("referenced by", 12),
		   "to know who reference the <i>activity</i>");
    mark_menu(m, "activity", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("undelete", 10),
		   "to undelete the <em>activity</em>");
 
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	m.setWhatsThis(m.insertItem("undelete recursively", 11),
		       "undelete the activity and its children");
	break;
      }
    }
  }
  
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    add_activity_diagram();
    break;
  case 1:
    add_parameter(0, 0);
    return;
  case 2:
    BrowserInterruptibleActivityRegion::add_interruptibleactivityregion(this);
    break;
  case 3:
    BrowserExpansionRegion::add_expansionregion(this);
    break;
  case 4:
#ifndef WIN32
#warning
#endif
    //add_partition(this);
    return;
  case 5:
    open(FALSE);
    return;
  case 6:
    {
      QString name;
      
      if (((BrowserNode *) parent())->enter_child_name(name, "enter activity's name : ",
						       UmlActivity, TRUE, FALSE))
	duplicate((BrowserNode *) parent(), name)->select_in_browser();
    }
    break;
  case 7:
    {
      BrowserActivityAction * r =
	BrowserActivityAction::add_activityaction(this, 0);
      
      if (r != 0)
	r->select_in_browser();
    }
    return;
  case 8:
    {
      BrowserActivityObject * r =
	BrowserActivityObject::add_activityobject(this, 0);
      
      if (r != 0)
	r->select_in_browser();
    }
    return;
  case 9:
    delete_it();
    break;
  case 10:
    BrowserNode::undelete(FALSE);
    break;
  case 11:
    BrowserNode::undelete(TRUE);
    break;
  case 12:
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

void BrowserActivity::open(bool force_edit) {
  if (!force_edit &&
      (associated_diagram != 0) &&
      !associated_diagram->deletedp())
    associated_diagram->open(FALSE);
  else if (!is_edited)
    def->edit();
}

void BrowserActivity::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
  // to update activity drawing
  def->modified();
}

UmlCode BrowserActivity::get_type() const {
  return UmlActivity;
}

BasicData * BrowserActivity::get_data() const {
  return def;
}

QString BrowserActivity::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  return (rev) ? name + "   [" + p + "]"
	       : p + "::" + name;
}

BrowserNodeList & BrowserActivity::instances(BrowserNodeList & result, bool sort)
{
  IdIterator<BrowserActivity> it(all);
  BrowserActivity * a;
  
  while ((a = it.current()) != 0) {
    if (!a->deletedp())
      result.append(a);
    ++it;
  }
  
  if (sort)
    result.sort();
  
  return result;
}

BrowserNode * BrowserActivity::get_associated() const {
  return associated_diagram;
}

void BrowserActivity::set_associated_diagram(BrowserActivityDiagram * dg,
					     bool on_read) {
  if (associated_diagram != dg) {
    if (associated_diagram != 0)
      QObject::disconnect(associated_diagram->get_data(), SIGNAL(deleted()),
			  def, SLOT(on_delete()));
    if ((associated_diagram = dg) != 0)
      QObject::connect(associated_diagram->get_data(), SIGNAL(deleted()),
		       def, SLOT(on_delete()));
    
    if (! on_read)
      package_modified();
  }
}

void BrowserActivity::init()
{
  its_default_stereotypes.clear();

  BrowserParameter::init();
  BrowserActivityAction::init();
  BrowserActivityObject::init();
  BrowserActivityNode::init();
  BrowserInterruptibleActivityRegion::init();
  BrowserExpansionRegion::init();
  BrowserFlow::init();
}

// unicode
const QStringList & BrowserActivity::default_stereotypes()
{
  return its_default_stereotypes;
}

bool BrowserActivity::api_compatible(unsigned v) const {
  return (v > 24);
}

QValueList<BrowserParameter *> BrowserActivity::get_params() const {
  QValueList<BrowserParameter *> l;
  QListViewItem * child = firstChild();
  
  while (child != 0) {
    if (!((BrowserNode *) child)->deletedp() &&
	(((BrowserNode *) child)->get_type() == UmlParameter))
      l.append((BrowserParameter *) child);

    child = child->nextSibling();
  }

  return l;
}

bool BrowserActivity::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd: 
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	UmlCode k = com->get_kind(args);
	
	switch (k) {
	case UmlActivityDiagram:
	  if (wrong_child_name(args, UmlActivityDiagram, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserActivityDiagram(args, this))->write_id(com);
	  break;
	case UmlParameter:
	  if (wrong_child_name(args, UmlParameter, TRUE, FALSE))
	    ok = FALSE;
	  else
	    add_parameter(0, args)->write_id(com);
	  break;
	case UmlInterruptibleActivityRegion:
	  (new BrowserInterruptibleActivityRegion(args, this))->write_id(com);
	  break;
	case UmlExpansionRegion:
	  (new BrowserExpansionRegion(args, this))->write_id(com);
	  break;
	case UmlActivityObject:
	  (new BrowserActivityObject(args, this))->write_id(com);
	  break;
#ifndef WIN32
#warning
#endif
	  /*
	case UmlPartition:
	  (BrowserRegion::add_partition(this, args))->write_id(com);
	  break;
	  */
	default:
	  if (IsaActivityNode(k))
	    (new BrowserActivityNode(k, args, this))->write_id(com);
	  else if (IsaActivityAction(k))
	    (new BrowserActivityAction(args, this, (UmlActionKind) (k - UmlActivityAction)))->write_id(com);
	  else
	    ok = FALSE;
	  break;
	}
      }
      
      if (! ok)
	com->write_id(0);
      else
	package_modified();
      
      return TRUE;
    }
  case setAssocDiagramCmd:
    if (is_read_only && !root_permission())
      com->write_ack(FALSE);
    else {
      set_associated_diagram((BrowserActivityDiagram *) com->get_id(args));
      com->write_ack(TRUE);
    }
    return TRUE;
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

bool BrowserActivity::may_contains_them(const QList<BrowserNode> & l,
					bool & duplicable) const {
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    switch (it.current()->get_type()) {
    case UmlPartition:
      duplicable = FALSE;
      // no break !
    case UmlInterruptibleActivityRegion:
    case UmlExpansionRegion:
    case UmlParameter:
    case UmlActivityDiagram:
    case UmlActivityAction:
    case UmlActivityObject:
    case InitialAN:
    case FlowFinalAN:
    case ActivityFinalAN:
    case DecisionAN:
    case MergeAN:
    case ForkAN:
    case JoinAN:
    case UmlDependOn:
      return (((const BrowserNode *) it.current()->get_container(UmlActivity)) == this);
    default:
      return FALSE;
    }
    
    if (! may_contains(it.current(), FALSE))
      return FALSE;
    
    duplicable = may_contains_it(it.current());
  }
  
  return TRUE;
}

// connexion by a dependency
const char * BrowserActivity::may_connect(const BrowserNode * dest) const {
  switch (dest->get_type()) {
  case UmlPackage:
  case UmlActivity:
  case UmlActivityAction:
  case UmlActivityObject:
    return 0;
  default:
    return "illegal";
  }
}

void BrowserActivity::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserParameter::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityNode::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityAction::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityObject::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserInterruptibleActivityRegion::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserExpansionRegion::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)) ||
#ifndef WIN32
#warning
#endif
  /*
      UmlDrag::canDecode(e, BrowserPartition::drag_key(this)) ||
      */
      UmlDrag::canDecode(e, BrowserActivityDiagram::drag_key(this)))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserActivity::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserActivity::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserParameter::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityNode::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityAction::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityObject::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserInterruptibleActivityRegion::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserExpansionRegion::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)) ||
#ifndef WIN32
#warning
#endif
  /*
      UmlDrag::canDecode(e, BrowserPartition::drag_key(this)) ||
      */
      UmlDrag::canDecode(e, BrowserActivityDiagram::drag_key(this)))
    e->accept();
  else
    e->ignore();
}

void BrowserActivity::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, BrowserParameter::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserActivityNode::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserActivityAction::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserActivityObject::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserInterruptibleActivityRegion::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserExpansionRegion::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserSimpleRelation::drag_key(this))) != 0) ||
#ifndef WIN32
#warning
#endif
  /*
       ((bn = UmlDrag::decode(e, BrowserPartition::drag_key(this))) != 0) ||
       */
       ((bn = UmlDrag::decode(e, BrowserActivityDiagram::drag_key(this))) != 0)) &&
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

void BrowserActivity::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "activity_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserActivity::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "activity_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserActivity::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "activity_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "activity " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);

    if (associated_diagram != 0) {
      if (associated_diagram->deletedp()) {
        warning += QString("<p>activity <b>") + full_name() +
	  "</b>'s associated diagram <b>" +
	  associated_diagram->full_name() + "</b> is deleted\n";
      }
      else {
	nl_indent(st);
	st << "associated_diagram ";
	associated_diagram->save(st, TRUE, warning);
      }
    }
    
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

BrowserActivity * BrowserActivity::read_ref(char * & st)
{
  read_keyword(st, "activity_ref");
  
  int id = read_id(st);
  BrowserActivity * result = all[id];
  
  return (result == 0)
    ? new BrowserActivity(id)
    : result;
}

BrowserActivity * BrowserActivity::read(char * & st, char * k,
					BrowserNode * parent)
{
  BrowserActivity * result;
  int id;
  
  if (!strcmp(k, "activity_ref")) {
    id = read_id(st);
    result = all[id];
    
    return (result == 0)
      ? new BrowserActivity(id)
      : result;
  }
  else if (!strcmp(k, "activity")) {
    id = read_id(st);
    result = all[id];
    
    if (result == 0)
      result = new BrowserActivity(read_string(st), parent, id);
    else {
      result->set_parent(parent);
      result->set_name(read_string(st));
    }
    
    k = read_keyword(st);
    result->def->read(st, k);

    if (!strcmp(k, "associated_diagram")) {
      result->set_associated_diagram(BrowserActivityDiagram::read_ref(st, read_keyword(st)),
				     TRUE);
      k = read_keyword(st);
    }
    
    result->BrowserNode::read(st, k);
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    if (strcmp(k, "end")) {
      while (BrowserParameter::read(st, k, result) ||
	     BrowserActivityNode::read(st, k, result) ||
	     BrowserActivityAction::read(st, k, result) ||
	     BrowserActivityObject::read(st, k, result) ||
	     BrowserInterruptibleActivityRegion::read(st, k, result) ||
	     BrowserExpansionRegion::read(st, k, result) ||
#ifndef WIN32
#warning
#endif
	     /*
	     BrowserPartition::read(st, k, result) ||
	     */
	     BrowserActivityDiagram::read(st, k, result) ||
	     BrowserSimpleRelation::read(st, k, result))
	k = read_keyword(st);
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserActivity::get_it(const char * k, int id)
{
  if (!strcmp(k, "activity_ref"))
    return all[id];
  
  BrowserNode * r;
  
  if (((r = BrowserParameter::get_it(k, id)) == 0) && 
      ((r = BrowserActivityNode::get_it(k, id)) == 0) && 
      ((r = BrowserActivityAction::get_it(k, id)) == 0) && 
      ((r = BrowserActivityObject::get_it(k, id)) == 0) && 
      ((r = BrowserInterruptibleActivityRegion::get_it(k, id)) == 0) && 
      ((r = BrowserExpansionRegion::get_it(k, id)) == 0) &&
      ((r = BrowserFlow::get_it(k, id)) == 0) &&
      ((r = BrowserSimpleRelation::get_it(k, id)) == 0)
#ifndef WIN32
#warning
#endif
  /*
      ((r = BrowserPartition::get_it(k, id)) == 0) &&
  */
      )
    r = BrowserActivityDiagram::get_it(k, id);
  return r;
}
