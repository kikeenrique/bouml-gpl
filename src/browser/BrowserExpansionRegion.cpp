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
#include <qpainter.h>
#include <qcursor.h>

#include "BrowserExpansionRegion.h"
#include "ExpansionRegionData.h"
#include "BrowserExpansionNode.h"
#include "BrowserActivityNode.h"
#include "BrowserActivityAction.h"
#include "BrowserActivityObject.h"
#include "BrowserInterruptibleActivityRegion.h"
#include "ActivityActionData.h"
#ifndef WIN32
#warning
#endif
/*
#include "BrowserPartition.h"
*/
#include "BrowserActivityDiagram.h"
#include "UmlDrag.h"
#include "UmlPixmap.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "DialogUtil.h"
#include "mu.h"

IdDict<BrowserExpansionRegion> BrowserExpansionRegion::all;
QStringList BrowserExpansionRegion::its_default_stereotypes;	// unicode

BrowserExpansionRegion::BrowserExpansionRegion(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserExpansionRegion>(all, id),
      def(new ExpansionRegionData), associated_diagram(0) {
  def->set_browser_node(this);
}

BrowserExpansionRegion::BrowserExpansionRegion(int id)
    : BrowserNode(), Labeled<BrowserExpansionRegion>(all, id),
      def(new ExpansionRegionData), associated_diagram(0) {
  // not yet read
  def->set_browser_node(this);
}

BrowserExpansionRegion::BrowserExpansionRegion(const BrowserExpansionRegion * model,
					       BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserExpansionRegion>(all, 0),
      associated_diagram(0) {
  def = new ExpansionRegionData(model->def, this);
  comment = model->comment;
}

BrowserNode * BrowserExpansionRegion::duplicate(BrowserNode * p, QString name) {
  BrowserNode * result = new BrowserExpansionRegion(this, p);
  
  result->set_name((const char *) name);
  result->update_stereotype(TRUE);
  
  return result;
}

BrowserExpansionRegion::~BrowserExpansionRegion() {
  all.remove(get_ident());
  delete def;
}

void BrowserExpansionRegion::clear(bool old)
{
  all.clear(old);
}

void BrowserExpansionRegion::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserExpansionRegion::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserExpansionRegion::pixmap(int) const {
  if (deletedp()) 
    return DeletedExpansionRegionIcon;
  else if (is_marked)
    return ExpansionRegionMarkedIcon;
  else
    return ExpansionRegionIcon;
}

BrowserExpansionRegion *
  BrowserExpansionRegion::add_expansionregion(BrowserNode * future_parent)
{
  QString name;
  
  return (!future_parent->enter_child_name(name, "enter expansion region's \nname (may be empty) : ",
					   UmlExpansionRegion, TRUE, TRUE))
    
    ? 0
    : add_expansionregion(future_parent, name);
}

BrowserExpansionRegion *
  BrowserExpansionRegion::add_expansionregion(BrowserNode * future_parent,
					      const char * name)
{
  BrowserExpansionRegion * r = new BrowserExpansionRegion(name, future_parent);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return r;
}

BrowserExpansionRegion * 
  BrowserExpansionRegion::get_expansionregion(BrowserNode * parent)
{
  BrowserNodeList l;
  QListViewItem * child;
      
  for (child = parent->firstChild(); child != 0; child = child->nextSibling())
    if (!((BrowserNode *) child)->deletedp() &&
	(((BrowserNode *) child)->get_type() == UmlExpansionRegion))
      l.append((BrowserNode *) child);
  
  BrowserNode * old;
  QString name;
  
  if (!parent->enter_child_name(name, "enter expansion region's \nname (may be empty) : ",
				UmlExpansionRegion, l, &old,
				TRUE, TRUE))
    return 0;
    
  return (old != 0)
    ? ((BrowserExpansionRegion *) old)
    : add_expansionregion(parent, name);
}

void BrowserExpansionRegion::menu() {
  QString s = name;
  
  if (s.isEmpty())
    s = "expansion region";
  
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  BrowserNode * item_above = 0;
  
  m.insertItem(new MenuTitle(s, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_read_only) {
      m.setWhatsThis(m.insertItem("New expansion node", 0),
		     "to add an <em>expansion node</em> to the <em>expansion region</em>");
      m.setWhatsThis(m.insertItem("New nested expansion region", 1),
		     "to add a nested <em>expansion region</em>");
      m.setWhatsThis(m.insertItem("New interruptible activity region", 2),
		     "to add an <em>interruptible expansion region Region</em>");
#ifndef WIN32
#warning partition
#endif
      /*
      m.setWhatsThis(m.insertItem("Add partition", 3),
		     "to add a <em>Partition</em> to the <em>region</em>");
		     */
      m.setWhatsThis(m.insertItem("Add activity action", 6),
		     "to add an <em>activity action</em> to the <em>region</em>");
      m.setWhatsThis(m.insertItem("Add object node", 7),
		     "to add an <em>activity object node</em> to the <em>region</em>");
      m.insertSeparator();
    }
    m.setWhatsThis(m.insertItem("Edit", 4),
		   "to edit the <em>expansion region</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only) {
      m.setWhatsThis(m.insertItem("Duplicate", 5),
		     "to copy the <em>expansion region</em> in a new one");
      
      
      item_above = (BrowserNode *) parent()->firstChild();
      if (item_above == this)
	item_above = 0;
      else {
	for (;;) {
	  BrowserNode * next = (BrowserNode *) item_above->nextSibling();
	  
	  if (next == this)
	    break;
	  item_above = (BrowserNode *) next;
	}
      }
      
      if ((item_above != 0) &&
	  ((item_above->get_type() == UmlInterruptibleActivityRegion) ||
	  (item_above->get_type() == UmlExpansionRegion)))
	m.setWhatsThis(m.insertItem("Set it nested in region above", 12),
		       "to set it a <em>sub region</em> of the <em>region</em> above");
      
      if ((((BrowserNode *) parent())->get_type() == UmlInterruptibleActivityRegion) ||
	  (((BrowserNode *) parent())->get_type() == UmlExpansionRegion))
	m.setWhatsThis(m.insertItem("Extract it from current parent region", 13),
		       "to stop to be nested in current parent region");
      m.insertSeparator();

      if (edition_number == 0)
	m.setWhatsThis(m.insertItem("Delete", 8),
		       "to delete the <em>expansion region</em>. \
Note that you can undelete it after");
    }
    mark_menu(m, "expansion region", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("Undelete", 10),
		   "to undelete the <em>expansion region</em>");
 
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	m.setWhatsThis(m.insertItem("Undelete recursively", 11),
		       "undelete the expansion region and its children");
	break;
      }
    }
  }
  
  exec_menu_choice(m.exec(QCursor::pos()), item_above);
}

void BrowserExpansionRegion::exec_menu_choice(int rank,
					      BrowserNode * item_above) {
  switch (rank) {
  case 0:
    add_expansionnode();
    return;
  case 1:
    add_expansionregion(this);
    break;
  case 2:
    BrowserInterruptibleActivityRegion::add_interruptibleactivityregion(this);
    break;
  case 3:
#ifndef WIN32
#warning
#endif
    //add_partition(this);
    return;
  case 4:
    open(FALSE);
    return;
  case 5:
    {
      QString name;
      
      if (((BrowserNode *) parent())->enter_child_name(name, "enter expansion region's \nname (may be empty) : ",
						       UmlExpansionRegion, TRUE, TRUE))
	duplicate((BrowserNode *) parent(), name)->select_in_browser();
    }
    break;
  case 6:
    {
      BrowserActivityAction * r =
	BrowserActivityAction::add_activityaction(this, 0);
      
      if (r != 0)
	r->select_in_browser();
    }
    return;
  case 7:
    {
      BrowserActivityObject * r =
	BrowserActivityObject::add_activityobject(this, 0);
      
      if (r != 0)
	r->select_in_browser();
    }
    return;
  case 8:
    delete_it();
    break;
    /*
  case 9:
    ReferenceDialog::show(this);
    return;
    */
  case 10:
    BrowserNode::undelete(FALSE);
    break;
  case 11:
    BrowserNode::undelete(TRUE);
    break;
  case 12:
    parent()->takeItem(this);
    item_above->insertItem(this);
    item_above->setOpen(TRUE);
    break;
  case 13:
    {
      QListViewItem * p = parent();
      
      p->takeItem(this);
      moveItem(p);
    }
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

void BrowserExpansionRegion::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (!is_read_only) {
      if (s == "New expansion node")
	choice = 0;
      else if (s == "New expansion region")
	choice = 1;
      else if (s == "New interruptible activity region")
	choice = 2;
#ifndef WIN32
#warning partition
#endif
      /*
      m.setWhatsThis(m.insertItem("Add partition", 3),
		     "to add a <em>Partition</em> to the <em>region</em>");
		     */
      else if (s == "Add activity action")
	choice = 6;
      else if (s == "Add object node")
	choice = 7;
    }
    if (s == "Edit")
      choice = 4;
    if (!is_read_only) {
      if (s == "Duplicate")
	choice = 5;
      if (edition_number == 0)
	if (s == "Delete")
	  choice = 8;
    }
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0)) {
    if (s == "Undelete")
      choice = 10;
 
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	if (s == "Undelete recursively")
	  choice = 11;
	break;
      }
    }
  }
  
  exec_menu_choice(choice, 0);
}

BrowserNode *
  BrowserExpansionRegion::add_expansionnode() {
  QString name;
  
  if (enter_child_name(name, "enter expansion node's \nname (may be empty) : ",
		       UmlExpansionNode, TRUE, TRUE)) {
    BrowserExpansionNode * node = new BrowserExpansionNode(name, this);
    
    setOpen(TRUE);
    def->modified();
    package_modified();
    node->select_in_browser();
    
    return node;
  }
  
  return 0;
}

void BrowserExpansionRegion::open(bool force_edit) {
  if (!force_edit &&
      (associated_diagram != 0) &&
      !associated_diagram->deletedp())
    associated_diagram->open(FALSE);
  else if (!is_edited)
    def->edit();
}

void BrowserExpansionRegion::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
  // to update drawing
  def->modified();
}

UmlCode BrowserExpansionRegion::get_type() const {
  return UmlExpansionRegion;
}

BasicData * BrowserExpansionRegion::get_data() const {
  return def;
}

bool BrowserExpansionRegion::allow_empty() const {
  return TRUE;
}

QString BrowserExpansionRegion::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString s = name;

  if (s.isEmpty())
    s = "<anonymous expansion region>";

  return (rev) ? s + "   [" + p + "]"
	       : p + "::" + s;
}

BrowserNode * BrowserExpansionRegion::get_associated() const {
  return associated_diagram;
}

void BrowserExpansionRegion::set_associated_diagram(BrowserActivityDiagram * dg,
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

void BrowserExpansionRegion::on_delete() {
  if (associated_diagram && associated_diagram->deletedp())
    associated_diagram = 0;
}

void BrowserExpansionRegion::init()
{
  its_default_stereotypes.clear();

  BrowserExpansionNode::init();
}

// unicode
const QStringList & BrowserExpansionRegion::default_stereotypes()
{
  return its_default_stereotypes;
}

bool BrowserExpansionRegion::api_compatible(unsigned v) const {
  return (v > 24);
}

QValueList<BrowserExpansionNode *> BrowserExpansionRegion::get_nodes() const {
  QValueList<BrowserExpansionNode *> l;
  QListViewItem * child = firstChild();
  
  while (child != 0) {
    if (!((BrowserNode *) child)->deletedp() &&
	(((BrowserNode *) child)->get_type() == UmlExpansionNode))
      l.append((BrowserExpansionNode *) child);

    child = child->nextSibling();
  }

  return l;
}

bool BrowserExpansionRegion::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd: 
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	UmlCode k = com->get_kind(args);
	
	switch (k) {
	case UmlExpansionNode:
	  if (wrong_child_name(args, UmlExpansionNode, TRUE, TRUE))
	    ok = FALSE;
	  else
	    (new BrowserExpansionNode(args, this))->write_id(com);
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

bool BrowserExpansionRegion::may_contains_them(const QList<BrowserNode> & l,
					       bool & duplicable) const {
  BrowserNode * activity = get_container(UmlActivity);
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    switch (it.current()->get_type()) {
    case UmlPartition:
      duplicable = FALSE;
      // no break !
    case UmlInterruptibleActivityRegion:
    case UmlExpansionRegion:
    case UmlExpansionNode:
    case UmlActivityAction:
    case UmlActivityObject:
    case InitialAN:
    case FlowFinalAN:
    case ActivityFinalAN:
    case DecisionAN:
    case MergeAN:
    case ForkAN:
    case JoinAN:
      return (((const BrowserNode *) it.current()->get_container(UmlActivity)) == activity);
    default:
      return FALSE;
    }
    
    if (! may_contains(it.current(), FALSE))
      return FALSE;
    
    duplicable = may_contains_it(it.current());
  }
  
  return TRUE;
}

void BrowserExpansionRegion::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserExpansionNode::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityNode::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityAction::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserInterruptibleActivityRegion::drag_key(this)) ||
#ifndef WIN32
#warning
#endif
  /*
      UmlDrag::canDecode(e, BrowserPartition::drag_key(this)) ||
      */
      UmlDrag::canDecode(e, BrowserExpansionRegion::drag_key(this)))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserExpansionRegion::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserExpansionRegion::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserExpansionNode::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityNode::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserActivityAction::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserInterruptibleActivityRegion::drag_key(this)) ||
#ifndef WIN32
#warning
#endif
  /*
      UmlDrag::canDecode(e, BrowserPartition::drag_key(this)) ||
      */
      UmlDrag::canDecode(e, BrowserExpansionRegion::drag_key(this)))
    e->accept();
  else
    e->ignore();
}

void BrowserExpansionRegion::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, BrowserExpansionNode::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserActivityNode::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserActivityAction::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserInterruptibleActivityRegion::drag_key(this))) != 0) ||
#ifndef WIN32
#warning
#endif
  /*
       ((bn = UmlDrag::decode(e, BrowserPartition::drag_key(this))) != 0) ||
       */
       ((bn = UmlDrag::decode(e, BrowserExpansionRegion::drag_key(this))) != 0)) &&
      (bn != after) && (bn != this)) {
    if (may_contains(bn, FALSE)) 
      move(bn, after);
    else {
      msg_critical("Error", "Forbiden");
      e->ignore();
    }
  }
  else if ((bn == 0) && (after == 0))
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserExpansionRegion::drag_key() const {
  return QString::number(UmlExpansionRegion)
      + "#" + QString::number((unsigned long) get_container(UmlActivity));
}

QString BrowserExpansionRegion::drag_postfix() const {
  return "#" + QString::number((unsigned long) get_container(UmlActivity));
}

QString BrowserExpansionRegion::drag_key(BrowserNode * p)
{
  return QString::number(UmlExpansionRegion)
    + "#" + QString::number((unsigned long) p->get_container(UmlActivity));
}

void BrowserExpansionRegion::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "expansionregion_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserExpansionRegion::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "expansionregion_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserExpansionRegion::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "expansionregion_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "expansionregion " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);

    if (associated_diagram != 0) {
      nl_indent(st);
      st << "associated_diagram ";
      associated_diagram->save(st, TRUE, warning);
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

BrowserExpansionRegion * BrowserExpansionRegion::read_ref(char * & st)
{
  read_keyword(st, "expansionregion_ref");
  
  int id = read_id(st);
  BrowserExpansionRegion * result = all[id];
  
  return (result == 0)
    ? new BrowserExpansionRegion(id)
    : result;
}

BrowserExpansionRegion * BrowserExpansionRegion::read(char * & st, char * k,
				  BrowserNode * parent)
{
  BrowserExpansionRegion * result;
  int id;
  
  if (!strcmp(k, "expansionregion_ref")) {
    id = read_id(st);
    result = all[id];
    
    return (result == 0)
      ? new BrowserExpansionRegion(id)
      : result;
  }
  else if (!strcmp(k, "expansionregion")) {
    id = read_id(st);
    result = all[id];
    
    if (result == 0)
      result = new BrowserExpansionRegion(read_string(st), parent, id);
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
      while (BrowserExpansionNode::read(st, k, result) ||
	     BrowserActivityNode::read(st, k, result) ||
	     BrowserActivityObject::read(st, k, result) ||
	     BrowserActivityAction::read(st, k, result) ||
	     BrowserInterruptibleActivityRegion::read(st, k, result) ||
#ifndef WIN32
#warning
#endif
	     /*
	     BrowserPartition::read(st, k, result) ||
	     */
	     BrowserExpansionRegion::read(st, k, result))
	k = read_keyword(st);
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserExpansionRegion::get_it(const char * k, int id)
{
  return (!strcmp(k, "expansionregion_ref"))
    ? all[id]
    : BrowserExpansionNode::get_it(k, id);
}
