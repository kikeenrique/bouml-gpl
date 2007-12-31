// *************************************************************************
//
// Copyleft 2004-2008 Bruno PAGES  .
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

#include "BrowserState.h"
#include "StateData.h"
#include "BrowserTransition.h"
#include "BrowserPseudoState.h"
#include "BrowserStateAction.h"
#include "BrowserRegion.h"
#include "BrowserStateDiagram.h"
#include "TransitionData.h"
#include "BrowserActivityAction.h"
#include "ReferenceDialog.h"
#include "UmlDrag.h"
#include "UmlPixmap.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "DialogUtil.h"
#include "mu.h"

IdDict<BrowserState> BrowserState::all(257, __FILE__);
QStringList BrowserState::its_default_stereotypes;	// unicode

BrowserState::BrowserState(QString s, BrowserNode * p, StateData * d, int id)
    : BrowserNode(s, p), Labeled<BrowserState>(all, id),
      def(d), associated_diagram(0) {
  def->set_browser_node(this);
}

BrowserState::BrowserState(int id)
    : BrowserNode(), Labeled<BrowserState>(all, id),
      def(new StateData), associated_diagram(0) {
  // not yet read
  def->set_browser_node(this);
}

BrowserState::BrowserState(const BrowserState * model, BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserState>(all, 0), associated_diagram(0) {
  def = new StateData(model->def, this);
  comment = model->comment;
}

BrowserNode * BrowserState::duplicate(BrowserNode * p, QString name) {
  BrowserNode * result = new BrowserState(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype(TRUE);
  
  return result;
}

BrowserState::~BrowserState() {
  all.remove(get_ident());
  delete def;
}

void BrowserState::clear(bool old)
{
  all.clear(old);
  BrowserTransition::clear(old);
  BrowserRegion::clear(old);
  BrowserPseudoState::clear(old);
  BrowserStateAction::clear(old);
}

void BrowserState::update_idmax_for_root()
{
  all.update_idmax_for_root();
  BrowserTransition::update_idmax_for_root();
  BrowserRegion::update_idmax_for_root();
  BrowserPseudoState::update_idmax_for_root();
  BrowserStateAction::update_idmax_for_root();
}
    
void BrowserState::referenced_by(QList<BrowserNode> & l) {
  BrowserNode::referenced_by(l);
  BrowserTransition::compute_referenced_by(l, this);
  BrowserActivityAction::compute_referenced_by(l, this);
}

void BrowserState::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserState::pixmap(int) const {
  return (deletedp()) ? DeletedStateIcon : StateIcon;
}

BrowserTransition * BrowserState::add_transition(BrowserNode * end) {
  BrowserTransition * result = new BrowserTransition(this, end);
  
  modified();
  return result;
}

BasicData * BrowserState::add_relation(UmlCode, BrowserNode * end) {
  return add_transition(end)->get_data();
}

BrowserState * BrowserState::add_state(BrowserNode * future_parent,
				       bool machine)
{
  QString name;
  
  if (!future_parent->enter_child_name(name, "enter state's name : ",
				       UmlState, TRUE, FALSE))
    
    return 0;
  
  BrowserState * st = add_state(future_parent, name);
  
  if (machine)
    st->def->set_stereotype((future_parent->get_type() == UmlState)
			    ? "submachine" : "machine");
  
  return st;
}

BrowserState * BrowserState::add_state(BrowserNode * future_parent,
				       QString name)
{
  return new BrowserState(name, future_parent, new StateData());
}

BrowserState * BrowserState::get_state(BrowserNode * parent)
{
  BrowserNodeList l;
  QListViewItem * child;
      
  for (child = parent->firstChild(); child != 0; child = child->nextSibling())
    if (!((BrowserNode *) child)->deletedp() &&
	(((BrowserNode *) child)->get_type() == UmlState))
      l.append((BrowserNode *) child);
  
  BrowserNode * old;
  QString name;
  
  if (!parent->enter_child_name(name, "enter state's name : ",
				UmlState, l, &old,
				TRUE, FALSE))
    return 0;
    
  if (old != 0)
    return ((BrowserState *) old);
  
  StateData * sd = new StateData();
  BrowserState * r = new BrowserState(name, parent, sd);
  
  sd->set_browser_node(r);
  parent->setOpen(TRUE);
  parent->package_modified();
  
  return r;
}

static bool is_machine(const BrowserState * s)
{
  switch (((BrowserNode *) s->parent())->get_type()) {
  case UmlState:
  case UmlRegion:
    return FALSE;
    break;
  default:
    return TRUE;
  }
}

void BrowserState::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  QString what;
  BrowserNode * item_above = 0;
  bool mach = is_machine(this);
  
  if (mach)
    what = "state machine";
  else
    what = (!strcmp(get_stereotype(), "submachine"))
      ? "state submachine"
      : "state";
  
  m.insertItem(new MenuTitle(def->definition(FALSE), m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_read_only) {
      m.setWhatsThis(m.insertItem("New state diagram", 0),
		     "to add a <em>state diagram</em>");
      if (mach) {
	m.setWhatsThis(m.insertItem("New submachine", 1),
		       "to add a <em>submachine</em> to the <em>machine</em>");
	m.setWhatsThis(m.insertItem("New state", 2),
		       "to add a <em>state</em> to the <em>machine</em>");
      }
      else if (what == "state")
	m.setWhatsThis(m.insertItem("New nested state", 2),
		       "to add a <em>nested state</em> to the <em>state</em>");
      else
	m.setWhatsThis(m.insertItem("New state", 2),
		       "to add a <em>state</em> to the <em>submachine</em>");
      m.setWhatsThis(m.insertItem("New region", 11),
		     "to add a <em>region</em>");
    }
    m.insertSeparator();
    m.setWhatsThis(m.insertItem("Edit", 3),
		   "to edit the <em>" + what + "</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only) {
      m.setWhatsThis(m.insertItem("Duplicate", 4),
		     "to copy the <em>" + what + "</em> in a new one");
      
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
	  ((item_above->get_type() == UmlState) ||
	   (item_above->get_type() == UmlRegion)) &&
	  !((BrowserNode *) item_above)->wrong_child_name(get_name(), UmlState, TRUE, FALSE))
	m.setWhatsThis(m.insertItem(QString("set it nested in ")
				    + item_above->get_name(),
				    5),
		       "to set it a <em>sub state</em> of the <em>item</em> above");
      
      if ((nestedp() || (((BrowserNode *) parent())->get_type() == UmlRegion)) &&
	  !((BrowserNode *) parent()->parent())->wrong_child_name(get_name(), UmlState, TRUE, FALSE))
	m.setWhatsThis(m.insertItem(QString("Extract it from ")
				    + ((BrowserNode *) parent())->get_name(),
				    6),
		       QString("to stop to be nested in ") +
		       ((BrowserNode *) parent())->get_name());
      m.insertSeparator();
      if (edition_number == 0)
	m.setWhatsThis(m.insertItem("Delete", 7),
		       "to delete the <em>" + what + "</em>. \
Note that you can undelete it after");
    }
    m.setWhatsThis(m.insertItem("Referenced by", 10),
		   "to know who reference the <i>state</i> \
through a transition");
    mark_menu(m, what, 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("Undelete", 8),
		   "to undelete the <em>" + what + "</em>");
 
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	m.setWhatsThis(m.insertItem("Undelete recursively", 9),
		       "undelete the " + what + " and its children");
	break;
      }
    }
  }
  
  exec_menu_choice(m.exec(QCursor::pos()), item_above, mach);
}

void BrowserState::exec_menu_choice(int rank,
				    BrowserNode * item_above,
				    bool mach) {
  switch (rank) {
  case 0:
    {
      BrowserStateDiagram * d = 
	BrowserStateDiagram::add_state_diagram(this);
      
      if (d == 0)
	return;
      d->select_in_browser();
    }
    break;
  case 1:
    add_state(this, (bool) TRUE);
    return;
  case 2:
    add_state(this, (bool) FALSE);
    return;
  case 3:
    open(FALSE);
    return;
  case 4:
    {
      QString name;
      QString what;
      
      if (mach)
	what = "state machine";
      else
	what = (!strcmp(get_stereotype(), "submachine"))
	  ? "state submachine"
	  : "state";
      
      if (((BrowserNode *) parent())->enter_child_name(name, "enter " + what + "'s name : ",
						       UmlState, TRUE, FALSE))
	duplicate((BrowserNode *) parent(), name)->select_in_browser();
    }
    break;
  case 5:
    parent()->takeItem(this);
    item_above->insertItem(this);
    item_above->setOpen(TRUE);
    break;
  case 6:
    {
      QListViewItem * p = parent();
      
      p->takeItem(this);
      moveItem(p);
    }
    break;
  case 7:
    delete_it();
    break;
  case 8:
    BrowserNode::undelete(FALSE);
    break;
  case 9:
    BrowserNode::undelete(TRUE);
    break;
  case 10:
    ReferenceDialog::show(this);
    return;
  case 11:
    BrowserRegion::add_region(this);
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

void BrowserState::apply_shortcut(QString s) {
  int choice = -1;
  bool mach = is_machine(this);
  
  if (!deletedp()) {
    if (!is_read_only) {
      if (s == "New state diagram")
	choice = 0;
      if (mach) {
	if (s == "New submachine")
	  choice = 1;
      }
      if (s == "New state")
	choice = 2;
      else if (s == "New region")
	choice = 11;
    }
    if (s == "Edit")
      choice = 3;
    if (!is_read_only) {
      if (s == "Duplicate")
	choice = 4;

      if (edition_number == 0)
	if (s == "Delete")
	  choice = 7;
    }
    if (s == "Referenced by")
      choice = 10;
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0)) {
    if (s == "Undelete")
      choice = 8;
 
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	if (s == "Undelete recursively")
	  choice = 9;
	break;
      }
    }
  }
  
  exec_menu_choice(choice, 0, mach);
}

void BrowserState::open(bool force_edit) {
  if (!force_edit &&
      (associated_diagram != 0) &&
      !associated_diagram->deletedp())
    associated_diagram->open(FALSE);
  else if (!is_edited)
    def->edit();
}

void BrowserState::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
  // to update regions drawing
  def->modified();
}

UmlCode BrowserState::get_type() const {
  return UmlState;
}

int BrowserState::get_identifier() const {
  return get_ident();
}

BasicData * BrowserState::get_data() const {
  return def;
}

QString BrowserState::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  return (rev) ? name + "   [" + p + "]"
	       : p + "::" + name;
}

BrowserNodeList & BrowserState::instances(BrowserNodeList & result, bool sort)
{
  IdIterator<BrowserState> it(all);
  BrowserState * st;
  
  while ((st = it.current()) != 0) {
    if (!st->deletedp())
      result.append(st);
    ++it;
  }
  
  if (sort)
    result.sort();
  
  return result;
}

BrowserNode * BrowserState::get_associated() const {
  return associated_diagram;
}

void BrowserState::set_associated_diagram(BrowserStateDiagram * dg,
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

void BrowserState::on_delete() {
  if (associated_diagram && associated_diagram->deletedp())
    associated_diagram = 0;
}

void BrowserState::init()
{
  its_default_stereotypes.clear();
  its_default_stereotypes.append("machine");
  its_default_stereotypes.append("submachine");
  its_default_stereotypes.append("top");
  
  BrowserStateAction::init();
  BrowserPseudoState::init();
  BrowserRegion::init();
}

// unicode
const QStringList & BrowserState::default_stereotypes()
{
  return its_default_stereotypes;
}

bool BrowserState::api_compatible(unsigned v) const {
  return (v > 14);
}

bool BrowserState::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd: 
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	UmlCode k = com->get_kind(args);
	
	switch (k) {
	case UmlStateDiagram:
	  if (wrong_child_name(args, UmlStateDiagram, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserStateDiagram(args, this))->write_id(com);
	  break;
	case UmlState:
	  if (wrong_child_name(args, UmlState, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (BrowserState::add_state(this, args))->write_id(com);
	  break;
	case UmlRegion:
	  (BrowserRegion::add_region(this, args))->write_id(com);
	  break;
	case UmlTransition:
	  {
	    BrowserNode * end = (BrowserNode *) com->get_id(args);
	    k = end->get_type();
	    
	    if ((k == UmlState) ||
		(k == UmlStateAction) ||
		IsaPseudoState(k))
	      (add_transition(end))->write_id(com);
	    else
	      ok = FALSE;
	  }
	  break;
	case UmlStateAction:
	  BrowserStateAction::add_stateaction(this)->write_id(com);
	  break;
	default:
	  if (IsaPseudoState(k) &&
	      !wrong_child_name(args, k, TRUE,
				BrowserPseudoState::allow_empty(k)))
	    BrowserPseudoState::add_pseudostate(this, k, args)->write_id(com);
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
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

bool BrowserState::may_contains_them(const QList<BrowserNode> & l,
				     bool & duplicable) const {
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    switch (it.current()->get_type()) {
    case UmlState:
      break;
    case UmlTransition:
      // no break !
    case UmlRegion:
      duplicable = FALSE;
      // no break !
    case UmlStateDiagram:
    case InitialPS:
    case EntryPointPS:
    case FinalPS:
    case TerminatePS:
    case ExitPointPS:
    case DeepHistoryPS:
    case ShallowHistoryPS:
    case JunctionPS:
    case ChoicePS:
    case ForkPS:
    case JoinPS:
      return (((const BrowserNode *) it.current()->parent()) == this);
    default:
      return FALSE;
    }
    
    if (! may_contains(it.current(), FALSE))
      return FALSE;
    
    duplicable = may_contains_it(it.current());
  }
  
  return TRUE;
}

void BrowserState::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserState::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserRegion::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserPseudoState::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserStateAction::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserStateDiagram::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserTransition::drag_key(this)))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserState::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserState::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserState::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserRegion::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserPseudoState::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserStateAction::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserStateDiagram::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserTransition::drag_key(this)))
    e->accept();
  else
    e->ignore();
}

void BrowserState::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, BrowserPseudoState::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserStateAction::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserRegion::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserState::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserStateDiagram::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserTransition::drag_key(this))) != 0)) &&
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

QString BrowserState::drag_key() const {
  return (is_machine(this))
    ? QString::number(UmlState)
    : QString::number(UmlState)
      + "#" + QString::number((unsigned long) parent());
}

QString BrowserState::drag_postfix() const {
  return (is_machine(this))
    ? QString::null
    : "#" + QString::number((unsigned long) parent());
}

QString BrowserState::drag_key(BrowserNode * p)
{
  return QString::number(UmlState)
    + "#" + QString::number((unsigned long) p);
}

void BrowserState::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "state_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserState::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "state_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserState::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "state_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "state " << get_ident() << ' ';
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

BrowserState * BrowserState::read_ref(char * & st)
{
  read_keyword(st, "state_ref");
  
  int id = read_id(st);
  BrowserState * result = all[id];
  
  return (result == 0)
    ? new BrowserState(id)
    : result;
}

BrowserState * BrowserState::read(char * & st, char * k,
				  BrowserNode * parent)
{
  BrowserState * result;
  int id;
  
  if (!strcmp(k, "state_ref")) {
    id = read_id(st);
    result = all[id];
    
    return (result == 0)
      ? new BrowserState(id)
      : result;
  }
  else if (!strcmp(k, "state")) {
    id = read_id(st);
    result = all[id];
    
    if (result == 0)
      result = new BrowserState(read_string(st), parent, new StateData, id);
    else if (result->is_defined) {
      BrowserState * already_exist = result;

      result = new BrowserState(read_string(st), parent, new StateData, id);

      already_exist->must_change_id(all);
      already_exist->unconsistent_fixed("state", result);
    }
    else {
      result->set_parent(parent);
      result->set_name(read_string(st));
    }
    
    result->is_defined = TRUE;
    k = read_keyword(st);
    result->def->read(st, k);

    if (!strcmp(k, "associated_diagram")) {
      result->set_associated_diagram(BrowserStateDiagram::read_ref(st, read_keyword(st)),
				     TRUE);
      k = read_keyword(st);
    }
    
    
    result->BrowserNode::read(st, k);
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    if (strcmp(k, "end")) {
      while (BrowserState::read(st, k, result) ||
	     BrowserRegion::read(st, k, result) ||
	     BrowserPseudoState::read(st, k, result) ||
	     BrowserStateAction::read(st, k, result) ||
	     BrowserStateDiagram::read(st, k, result) ||
	     BrowserTransition::read(st, k, result))
	k = read_keyword(st);
      
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserState::get_it(const char * k, int id)
{
  if (!strcmp(k, "state_ref"))
    return all[id];
  
  BrowserNode * r;
  
  if (((r = BrowserRegion::get_it(k, id)) == 0) && 
      ((r = BrowserPseudoState::get_it(k, id)) == 0) && 
      ((r = BrowserStateAction::get_it(k, id)) == 0) && 
      ((r = BrowserStateDiagram::get_it(k, id)) == 0))
    r = BrowserTransition::get_it(k, id);
  
  return r;
}
