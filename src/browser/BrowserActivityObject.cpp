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

#include "BrowserActivityObject.h"
#include "ActivityObjectData.h"
#include "BrowserActivityDiagram.h"
#include "BrowserActivityNode.h"
#include "ParameterData.h"
#include "BrowserFlow.h"
#include "BrowserSimpleRelation.h"
#include "BrowserClass.h"
#include "PinData.h"
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

IdDict<BrowserActivityObject> BrowserActivityObject::all(257, __FILE__);
QStringList BrowserActivityObject::its_default_stereotypes;	// unicode

BrowserActivityObject::BrowserActivityObject(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserActivityObject>(all, id),
      def(new ActivityObjectData), associated_diagram(0) {
  def->set_browser_node(this);
}

BrowserActivityObject::BrowserActivityObject(int id)
    : BrowserNode(), Labeled<BrowserActivityObject>(all, id),
      def(new ActivityObjectData), associated_diagram(0) {
  // not yet read
  def->set_browser_node(this);
}

BrowserActivityObject::BrowserActivityObject(const BrowserActivityObject * model, BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserActivityObject>(all, 0), associated_diagram(0) {
  def = new ActivityObjectData(model->def, this);
  comment = model->comment;
}

BrowserNode * BrowserActivityObject::duplicate(BrowserNode * p, QString name) {
  BrowserNode * result = new BrowserActivityObject(this, p);
  
  result->set_name((const char *) name);
  result->update_stereotype();
  
  return result;
}

BrowserActivityObject::~BrowserActivityObject() {
  all.remove(get_ident());
  delete def;
}

void BrowserActivityObject::clear(bool old)
{
  all.clear(old);
}

void BrowserActivityObject::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserActivityObject::referenced_by(QList<BrowserNode> & l) {
  BrowserNode::referenced_by(l);
  BrowserFlow::compute_referenced_by(l, this);
  BrowserSimpleRelation::compute_referenced_by(l, this);
}

void BrowserActivityObject::compute_referenced_by(QList<BrowserNode> & l,
						  BrowserNode * target)
{
  IdIterator<BrowserActivityObject> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp()) {
      if (it.current()->def->get_type().type == target)
	l.append(it.current());
    }
    ++it;
  }
}

void BrowserActivityObject::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap * BrowserActivityObject::pixmap(int) const {
  if (deletedp()) 
    return DeletedActionIcon;
  else if (is_marked)
    return ActionMarkedIcon;
  else
    return ActionIcon;
}

// add flow or dependency
BasicData * BrowserActivityObject::add_relation(UmlCode l, BrowserNode * end) {
  if (l == UmlFlow)
    return (new BrowserFlow(this, end))->get_data();
  else
    return BrowserNode::add_relation(l, end);
}

// a flow/dependency may be added in all the cases
const char * BrowserActivityObject::may_start() const {
  return 0;
}

// connexion by a flow or dependency
const char * BrowserActivityObject::may_connect(UmlCode & l, const BrowserNode * dest) const {
  switch (l) {
  case UmlFlow:
    {
      BrowserNode * bn = dest->get_container(UmlActivity);
      
      if ((bn != 0) && (get_container(UmlActivity) != bn))
	return "not in the same activity";
    }
    
    switch (dest->get_type()) {
    case InitialAN:
      return "initial node can't have incoming flow";
    case ForkAN:  // theo all input and output must be control/data exclusively
      return (((BrowserActivityNode *) dest)->target_of_flow())
	? "fork can't have several incoming flow"
	: 0;
    case FlowFinalAN:
    case ActivityFinalAN:
      return (! def->get_is_control())
	? "object can't have outgoing control flow (not 'is_control')"
	: 0;
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

	if (data->get_is_control() != def->get_is_control())
	  return (def->get_is_control())
	    ? "pin can't accept control flow (not 'is_control')"
	    : "pin can't accept data flow (is 'is_control')";
	else
	  return 0;
      }
    case UmlExpansionNode:
      {
	ActivityObjectData * data = (ActivityObjectData *) dest->get_data();

	if (data->get_is_control() != def->get_is_control())
	  return (def->get_is_control())
	    ? "expansion node can't accept control flow (not 'is_control')"
	    : "expansion node can't accept data flow (is 'is_control')";
	else
	  return 0;
      }
    case UmlActivityAction:
    case DecisionAN:	      // theo all input and
    case MergeAN:	      // output must be
    case JoinAN:	      // control/data exclusively
      return 0;
    default:
      return "illegal";
    }
  case UmlDependency:
    l = UmlDependOn;
    // no break;
  case UmlDependOn:
    switch (dest->get_type()) {
    case UmlPackage:
    case UmlActivity:
    case UmlActivityAction:
    case UmlActivityObject:
      return 0;
    default:
      return "illegal";
    }
  default:
      return "illegal";
  }
}

BrowserActivityObject *
BrowserActivityObject::add_activityobject(BrowserNode * future_parent,
					  const char * s)
{
  QString name;
  
  if (s != 0)
    name = s;
  else if (!future_parent->enter_child_name(name, "enter activity object's \nname (may be empty) : ",
					    UmlActivityObject, TRUE, TRUE))
    return 0;
  
  BrowserActivityObject * r =
    new BrowserActivityObject(name, future_parent);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return r;
}

bool BrowserActivityObject::same_name(const QString &, UmlCode) const {
  // same name allowed
  return FALSE;
}

BrowserActivityObject * BrowserActivityObject::get_activityobject(BrowserNode * future_parent)
{
  BrowserNodeList l;
  QListViewItem * child;
      
  for (child = future_parent->firstChild(); child != 0; child = child->nextSibling())
    if (!((BrowserNode *) child)->deletedp() &&
	(((BrowserNode *) child)->get_type() == UmlActivityObject) &&
	(((BrowserNode *) child)->get_name()[0] != 0))
      l.append((BrowserNode *) child);
  
  BrowserNode * old = 0;
  QString name;
  
  if (!future_parent->enter_child_name(name, "enter activity object's \nname (may be empty) : ",
				       UmlActivityObject, l, &old, TRUE, TRUE))
    return 0;
    
  if (old != 0)
    return ((BrowserActivityObject *) old);
  
  BrowserActivityObject * r =
    new BrowserActivityObject(name, future_parent);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return r;
}

void BrowserActivityObject::menu() {
  QPopupMenu m(0, "Activity object");
  QPopupMenu toolm(0);
  QString s = name;

  if (s.isEmpty())
    s = "activity object";
  
  m.insertItem(new MenuTitle(s, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    m.setWhatsThis(m.insertItem("Edit", 1),
		   "to edit the <em>" + s + "</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only) {
      m.setWhatsThis(m.insertItem("Duplicate", 2),
		     "to copy the <em>activity object</em> in a new one");
      m.insertSeparator();
      if (edition_number == 0)
	m.setWhatsThis(m.insertItem("Delete", 3),
		       "to delete the <em>activity object</em>. \
Note that you can undelete it after");
    }
    m.setWhatsThis(m.insertItem("Referenced by", 4),
		   "to know who reference the <i>activity object</i> \
through a flow or dependency");
    mark_menu(m, s, 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("Undelete", 5),
		   "to undelete the <em>activity object</em>");
   
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	m.setWhatsThis(m.insertItem("Undelete recursively", 6),
		       "undelete the <em>activity object</em> and its expansion nodes and \
<em>flows</em> or <em>dependencies</em>(except if the class at the other side is also deleted)");
	break;
      }
    }
  }
  
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserActivityObject::exec_menu_choice(int rank) {
  switch (rank) {
  case 1:
    open(TRUE);
    return;
  case 2:
    {
      QString name;
      
      if (((BrowserNode *) parent())->enter_child_name(name, "enter activity object's \nname (may be empty) : ",
						       get_type(), TRUE, TRUE))
	duplicate((BrowserNode *) parent(), name)->select_in_browser();
    }
    break;
  case 3:
    delete_it();
    break;
  case 4:
    ReferenceDialog::show(this);
    return;
  case 5:
    BrowserNode::undelete(FALSE);
    break;
  case 6:
    BrowserNode::undelete(TRUE);
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

void BrowserActivityObject::apply_shortcut(QString s) {
  int choice = -1;
  
  if (!deletedp()) {
    if (s == "Edit")
      choice = 1;
    if (!is_read_only) {
      if (s == "Duplicate")
	choice = 2;
      else if (edition_number == 0)
	if (s == "Delete")
	  choice = 3;
    }
    if (s == "Referenced by")
      choice = 4;
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0)) {
    if (s == "Undelete")
      choice = 5;
   
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	if (s == "Undelete recursively")
	  choice = 6;
	break;
      }
    }
  }
  
  exec_menu_choice(choice);
}

void BrowserActivityObject::open(bool force_edit) {
  if (!force_edit &&
      (associated_diagram != 0) &&
      !associated_diagram->deletedp())
    associated_diagram->open(FALSE);
  else if (!is_edited)
    def->edit("Activity Object", its_default_stereotypes);
}

void BrowserActivityObject::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
  // to update drawing
  def->modified();
}

UmlCode BrowserActivityObject::get_type() const {
  return UmlActivityObject;
}

int BrowserActivityObject::get_identifier() const {
  return get_ident();
}

BasicData * BrowserActivityObject::get_data() const {
  return def;
}

bool BrowserActivityObject::allow_empty() const {
  return TRUE;
}

QString BrowserActivityObject::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString s = name;

  if (s.isEmpty())
    s = "<anonymous activity object>";

  return (rev) ? s + "   [" + p + "]"
	       : p + "::" + s;
}

BrowserNode * BrowserActivityObject::get_associated() const {
  return associated_diagram;
}

void BrowserActivityObject::set_associated_diagram(BrowserActivityDiagram * dg,
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

void BrowserActivityObject::on_delete() {
  if (associated_diagram && associated_diagram->deletedp())
    associated_diagram = 0;
}

void BrowserActivityObject::init()
{
  its_default_stereotypes.clear();

  its_default_stereotypes.append("datastore");
  its_default_stereotypes.append("centralBuffer");
}

// unicode
const QStringList & BrowserActivityObject::default_stereotypes()
{
  return its_default_stereotypes;
}

bool BrowserActivityObject::api_compatible(unsigned v) const {
  return (v > 24);
}

bool BrowserActivityObject::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd: 
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	UmlCode k = com->get_kind(args);
	
	switch (k) {
	// dependency not yet managed
	case UmlFlow:
	  {
	    BrowserNode * end = (BrowserNode *) com->get_id(args);
	    
	    if (may_connect(k, end) == 0)
	      (new BrowserFlow(this, end))->write_id(com);
	    else
	      ok = FALSE;
	  }
	  break;
	case UmlSimpleRelations:
	  {
	    UmlCode c;
	    
	    if (!com->get_relation_kind(c, args))
	      ok = FALSE;
	    else {
	      BrowserNode * end = (BrowserNode *) com->get_id(args);
	      
	      if (may_connect(c, end) == 0)
		add_relation(c, end)->get_browser_node()->write_id(com);
	      else
		ok = FALSE;
	    }
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

bool BrowserActivityObject::may_contains_them(const QList<BrowserNode> & l,
					      bool & duplicable) const {
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    switch (it.current()->get_type()) {
    case UmlFlow:
    case UmlDependOn:
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

void BrowserActivityObject::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserFlow::drag_key(this)) ||
      UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this))) {
    if (!is_read_only)
      e->accept();
    else
      e->ignore();
  }
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserActivityObject::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserActivityObject::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (!is_read_only &&
      (UmlDrag::canDecode(e, BrowserFlow::drag_key(this)) ||
       UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this))))
    e->accept();
  else
    e->ignore();
}

void BrowserActivityObject::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, BrowserFlow::drag_key(this))) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserSimpleRelation::drag_key(this))) != 0)) &&
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

QString BrowserActivityObject::drag_key() const {
  return QString::number(UmlActivityObject)
    + "#" + QString::number((unsigned long) get_container(UmlActivity));
}

QString BrowserActivityObject::drag_postfix() const {
  return "#" + QString::number((unsigned long) get_container(UmlActivity));
}

QString BrowserActivityObject::drag_key(BrowserNode * p)
{
  return QString::number(UmlActivityObject)
    + "#" + QString::number((unsigned long) p->get_container(UmlActivity));
}

void BrowserActivityObject::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "activityobject_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserActivityObject::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "activityobject_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserActivityObject::save(QTextStream & st, bool ref, QString & warning) {
  if (ref) {
    st << "activityobject_ref " << get_ident()
      << " // activity object " << get_name();
  }
  else {
    nl_indent(st);
    st << "activityobject " << get_ident() << ' ';
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

BrowserActivityObject * BrowserActivityObject::read_ref(char * & st)
{
  read_keyword(st, "activityobject_ref");
  
  int id = read_id(st);
  BrowserActivityObject * result = all[id];
  
  return (result == 0)
    ? new BrowserActivityObject(id)
    : result;
}

BrowserActivityObject * BrowserActivityObject::read(char * & st, char * k,
						    BrowserNode * parent)
{
  BrowserActivityObject * result;
  int id;
  
  if (!strcmp(k, "activityobject_ref")) {
    id = read_id(st);
    result = all[id];
    
    return (result == 0)
      ? new BrowserActivityObject(id)
      : result;
  }
  else if (!strcmp(k, "activityobject")) {
    id = read_id(st);
    result = all[id];
    
    if (result == 0)
      result = new BrowserActivityObject(read_string(st), parent, id);
    else if (result->is_defined) {
      BrowserActivityObject * already_exist = result;

      result = new BrowserActivityObject(read_string(st), parent, id);

      already_exist->must_change_id(all);
      already_exist->unconsistent_fixed("activity object", result);
    }
    else {
      result->set_parent(parent);
      result->set_name(read_string(st));
    }
    
    result->is_defined = TRUE;
    k = read_keyword(st);
    result->def->read(st, k);
    
    if (!strcmp(k, "associated_diagram")) {
      result->set_associated_diagram(BrowserActivityDiagram::read_ref(st, read_keyword(st)),
				     TRUE);
      k = read_keyword(st);
    }
    
    result->BrowserNode::read(st, k);
    
    if (strcmp(k, "end")) {
      while (BrowserFlow::read(st, k, result) ||
	     BrowserSimpleRelation::read(st, k, result))
	k = read_keyword(st);
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }

    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserActivityObject::get_it(const char * k, int id)
{
  // flow managed in activity
  return (!strcmp(k, "activityobject_ref")) ? all[id] : 0;
}
