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
#include <qcursor.h>

#include "BrowserUseCase.h"
#include "UseCaseData.h"
#include "BrowserSeqDiagram.h"
#include "BrowserColDiagram.h"
#include "BrowserSimpleRelation.h"
#include "SimpleRelationData.h"
#include "BrowserUseCaseDiagram.h"
#include "BrowserObjectDiagram.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "BrowserClass.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "ReferenceDialog.h"
#include "DialogUtil.h"
#include "mu.h"

IdDict<BrowserUseCase> BrowserUseCase::all(257);
QStringList BrowserUseCase::its_default_stereotypes;	// unicode
QStringList BrowserUseCase::relations_default_stereotypes[UmlRelations];	// unicode

BrowserUseCase::BrowserUseCase(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserUseCase>(all, id)  {
  make();
  is_modified = (id == 0);
}

BrowserUseCase::BrowserUseCase(const BrowserUseCase * model,
			       BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserUseCase>(all, 0) {
  def = new UseCaseData(model->def, this);
  associated_diagram = 0;
  comment = model->comment;
  usecasediagram_settings = model->usecasediagram_settings;
  sequencediagram_settings = model->sequencediagram_settings;
  collaborationdiagram_settings = model->collaborationdiagram_settings;
  objectdiagram_settings = model->objectdiagram_settings;
  note_color = model->note_color;
  fragment_color = model->fragment_color;
  subject_color = model->subject_color;
  duration_color = model->duration_color;
  continuation_color = model->continuation_color;
  usecase_color = model->usecase_color;
  package_color = model->package_color;
  
  is_modified = true;
}

BrowserUseCase::BrowserUseCase(int id)
    : BrowserNode(), Labeled<BrowserUseCase>(all, id) {
  make();
  is_modified = (id == 0);
}

BrowserUseCase::~BrowserUseCase() {
  all.remove(get_ident());
  delete def;
}

void BrowserUseCase::make() {
  def = new UseCaseData;
  def->set_browser_node(this);
  
  associated_diagram = 0;
    
  note_color = UmlDefaultColor;
  duration_color = UmlDefaultColor;
  continuation_color = UmlDefaultColor;
  usecase_color = UmlDefaultColor;
  package_color = UmlDefaultColor;
  fragment_color = UmlDefaultColor;
  subject_color = UmlDefaultColor;	   
}

BrowserNode * BrowserUseCase::duplicate(BrowserNode * p, QString name) {
  BrowserUseCase * result = new BrowserUseCase(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

void BrowserUseCase::clear(bool old)
{
  all.clear(old);
  BrowserSimpleRelation::clear(old);
}

void BrowserUseCase::update_idmax_for_root()
{
  all.update_idmax_for_root();
  BrowserSimpleRelation::update_idmax_for_root();
}
    
void BrowserUseCase::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
  BrowserNode::renumber(phase);
}

const QPixmap* BrowserUseCase::pixmap(int) const {
  return (deletedp()) ? DeletedUseCaseIcon : UseCaseIcon;
}

QString BrowserUseCase::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  if (p.isEmpty()) 
    return QString((const char *) name);
  else if (rev)
    return name + "   [" + p + "]";
  else
    return p + "::" + name;
}

void BrowserUseCase::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  BrowserNode * item_above = 0;
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_read_only && (edition_number == 0)) {
      m.setWhatsThis(m.insertItem("New use case diagram", 0),
		     "to add a <em>use case diagram</em>");
      m.setWhatsThis(m.insertItem("New sequence diagram", 1),
		     "to add a <em>sequence diagram</em>");
      m.setWhatsThis(m.insertItem("New collaboration diagram", 2),
		     "to add a <em>collaboration diagram</em>");
      m.setWhatsThis(m.insertItem("New object diagram", 13),
		     "to add a <em>object diagram</em>");
      m.insertSeparator();
      m.setWhatsThis(m.insertItem("New use case", 3),
		     "to add a <em>use case</em>");
      m.setWhatsThis(m.insertItem("New actor", 4),
		     "to add an <em>actor</em>");
      m.setWhatsThis(m.insertItem("New class", 5),
		     "to add a <em>class</em>");
      m.insertSeparator();
    }
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Edit", 6),
		     "to edit the <em>use case</em>");
      if (!is_read_only) {
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Edit drawing settings", 7),
		       "to set how the sub <em>diagrams</em>'s items must be drawed");
	if (edition_number == 0) {
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
	  
	  if (item_above != 0) {
	    switch (item_above->get_type()) {
	    case UmlUseCase:
	    case UmlUseCaseView:
	      if (!((BrowserNode *) item_above)->wrong_child_name(get_name(), UmlUseCase, TRUE, FALSE)) {
		m.insertSeparator();
		m.setWhatsThis(m.insertItem(QString("set it nested in ")
					    + item_above->get_name(),
					    11),
			       "to set it nested in the item above");
	      }
	    default:
	      break;
	    }
	  }
	  m.insertSeparator();
	  m.setWhatsThis(m.insertItem("Delete", 8),
			 "to delete the <em>use case and its sub items</em>. \
Note that you can undelete them after");
	}
      }
    }
    m.insertSeparator();
    m.setWhatsThis(m.insertItem("Referenced by", 12),
		   "to know who reference the <i>use case</i> \
through a relation");
    mark_menu(m, "use case", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("Undelete", 9),
		   "undelete the <em>use case</em>. \
Do not undelete its sub items");
    m.setWhatsThis(m.insertItem("Undelete recursively", 10),
		   "undelete the <em>use case</em> and its sub items");
  }
  
  exec_menu_choice(m.exec(QCursor::pos()), item_above);
}

void BrowserUseCase::exec_menu_choice(int rank, BrowserNode * item_above) {
  switch (rank) {
  case 0:
    add_use_case_diagram();
    break;
  case 1:
    add_sequence_diagram();
    break;
  case 2:
    add_collaboration_diagram();
    break;
  case 3:
    {
      BrowserUseCase * uc = add_use_case(this);
      
      if (uc != 0)
	uc->select_in_browser();
    }
    break;
  case 4:
    {
      BrowserClass * a = BrowserClass::add_class(this);
      
      if (a != 0)
	a->get_data()->set_stereotype("actor");
	a->select_in_browser();
    }
    break;
  case 5:
    {
      BrowserClass * cl = BrowserClass::add_class(this);
      
      if (cl != 0)
	cl->select_in_browser();
    }
    break;
  case 6:
    def->edit();
    return;
  case 7:
    {
      QArray<StateSpec> st;
      QArray<ColorSpec> co(7);
      
      usecasediagram_settings.complete(st, FALSE);
      sequencediagram_settings.complete(st, FALSE);
      collaborationdiagram_settings.complete(st, FALSE);
      objectdiagram_settings.complete(st, FALSE);
      
      co[0].set("note color", &note_color);
      co[1].set("use case color", &usecase_color);
      co[2].set("package color", &package_color);
      co[3].set("fragment color", &fragment_color);
      co[4].set("subject color", &subject_color);
      co[5].set("duration color", &duration_color);
      co[6].set("continuation color", &continuation_color);

      SettingsDialog dialog(&st, &co, FALSE, FALSE);
      
      dialog.raise();
      if (dialog.exec() != QDialog::Accepted)
	return;
      DrawingSettings::modified();
    }
    break;
  case 8:
    delete_it();
    break;
  case 9:
    undelete(FALSE);
    break;
  case 10:
    undelete(TRUE);
    break;
  case 11:
    parent()->takeItem(this);
    item_above->insertItem(this);
    item_above->setOpen(TRUE);
    break;
  case 12:
    ReferenceDialog::show(this);
    return;
  case 13:
    add_object_diagram();
    break;
  default:
    if (rank >= 100)
      ToolCom::run(Tool::command(rank - 100), this);
    else
      mark_management(rank - 90);
    return;
  }
  
  package_modified();
}

void BrowserUseCase::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (!is_read_only && (edition_number == 0)) {
      if (s == "New use case diagram")
	choice = 0;
      else if (s == "New sequence diagram")
	choice = 1;
      else if (s == "New collaboration diagram")
	choice = 2;
      else if (s == "New object diagram")
	choice = 13;
      else if (s == "New use case")
	choice = 3;
      else if (s == "New actor")
	choice = 4;
      else if (s == "New class")
	choice = 5;
    }
    if (!is_edited) {
      if (s == "Edit")
	choice = 6;
      if (!is_read_only) {
	if (s == "Edit drawing settings")
	  choice = 7;
	if (edition_number == 0) {
	  if (s == "Delete")
	    choice = 8;
	}
      }
    }
    if (s == "Referenced by")
      choice = 12;
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0)) {
    if (s == "Undelete")
      choice = 9;
    else if (s == "Undelete recursively")
      choice = 10;
  }
  
  exec_menu_choice(choice, 0);
}

void BrowserUseCase::add_sequence_diagram() {
  (new BrowserSeqDiagram(child_random_name("Sequence Diagram"), this))
    ->select_in_browser();
}

void BrowserUseCase::add_collaboration_diagram() {
  (new BrowserColDiagram(child_random_name("Collaboration Diagram"), this))
    ->select_in_browser();
}

void BrowserUseCase::add_object_diagram() {
  (new BrowserObjectDiagram(child_random_name("Object Diagram"), this))
    ->select_in_browser();
}

void BrowserUseCase::add_use_case_diagram() {
  (new BrowserUseCaseDiagram(child_random_name("Use Case Diagram"), this))
    ->select_in_browser();
}

BrowserNodeList & BrowserUseCase::instances(BrowserNodeList & result) {
  IdIterator<BrowserUseCase> it(all);
  
  while (it.current() != 0) {
    if (!it.current()->deletedp())
      result.append(it.current());
    ++it;
  }

  result.sort();
  
  return result;
}

BrowserUseCase * BrowserUseCase::add_use_case(BrowserNode * future_parent) {
  QString name;
  
  if (!future_parent->enter_child_name(name, "enter use case's name : ",
				       UmlUseCase, TRUE, FALSE))
    return 0;
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  
  return new BrowserUseCase(name, future_parent);
}

BrowserNode * BrowserUseCase::get_use_case(BrowserNode * future_parent) {
  BrowserNode * old;
  QString name;
  BrowserNodeList nodes;
  
  if (!future_parent->enter_child_name(name, "enter use case's name : ",
				       UmlUseCase, instances(nodes),
				       &old, TRUE, FALSE))
    return 0;

  if (old != 0)
    return old;
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  return new BrowserUseCase(name, future_parent);
}

void BrowserUseCase::open(bool force_edit) {
  if (!force_edit &&
      (associated_diagram != 0) &&
      !associated_diagram->deletedp())
    associated_diagram->open(FALSE);
  else if (!is_edited)
    def->edit();
}

UmlCode BrowserUseCase::get_type() const {
  return UmlUseCase;
}

BasicData * BrowserUseCase::get_data() const {
  return def;
}

void BrowserUseCase::get_sequencediagramsettings(SequenceDiagramSettings & r) const {
  if (! sequencediagram_settings.complete(r))
    ((BrowserNode *) parent())->get_sequencediagramsettings(r);
}

void BrowserUseCase::get_collaborationdiagramsettings(CollaborationDiagramSettings & r) const {
  if (! collaborationdiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_collaborationdiagramsettings(r);
}

void BrowserUseCase::get_objectdiagramsettings(ObjectDiagramSettings & r) const {
  if (! objectdiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_objectdiagramsettings(r);
}

void BrowserUseCase::get_usecasediagramsettings(UseCaseDiagramSettings & r) const {
  if (! usecasediagram_settings.complete(r))
    ((BrowserNode *) parent())->get_usecasediagramsettings(r);
}

UmlColor BrowserUseCase::get_color(UmlCode who) const {
  UmlColor c;
  
  switch (who) {
  case UmlNote:
    c = note_color;
    break;
  case UmlUseCase:
    c = usecase_color;
    break;
  case UmlPackage:
    c = package_color;
    break;
  case UmlFragment:
    c = fragment_color;
    break;
  case UmlSubject:
    c = subject_color;
    break;
  case UmlContinuation:
    c = continuation_color;
    break;
  default:	// UmlDuration
    c = duration_color;
  }
  
  return (c != UmlDefaultColor)
    ? c
    : ((BrowserNode *) parent())->get_color(who);
}

bool BrowserUseCase::get_auto_label_position(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlUseCaseDiagram:
    v = usecasediagram_settings.auto_label_position;
    break;
  case UmlObjectDiagram:
    v = objectdiagram_settings.auto_label_position;
    break;
  default:
    // error
    return FALSE;
  }
  
  switch (v) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_auto_label_position(who);
  }
}

BrowserNode * BrowserUseCase::get_associated() const {
  return associated_diagram;
}

void BrowserUseCase::set_associated_diagram(BrowserUseCaseDiagram * dg,
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

void BrowserUseCase::on_delete() {
  if (associated_diagram && associated_diagram->deletedp())
    associated_diagram = 0;
}

bool BrowserUseCase::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd:
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	switch (com->get_kind(args)) {
	case UmlUseCaseDiagram:
	  if (wrong_child_name(args, UmlUseCaseDiagram, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserUseCaseDiagram(args, this))->write_id(com);
	  break;
	case UmlSeqDiagram:
	  if (wrong_child_name(args, UmlSeqDiagram, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserSeqDiagram(args, this))->write_id(com);
	  break;
	case UmlColDiagram:
	  if (wrong_child_name(args, UmlColDiagram, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserColDiagram(args, this))->write_id(com);
	  break;
	case UmlObjectDiagram:
	  if (wrong_child_name(args, UmlObjectDiagram, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserObjectDiagram(args, this))->write_id(com);
	  break;
	case UmlUseCase:
	  if (wrong_child_name(args, UmlUseCase, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserUseCase(args, this))->write_id(com);
	  break;
	case UmlClass:
	  if (wrong_child_name(args, UmlClass, FALSE, FALSE))
	    ok = FALSE;
	  else
	    (BrowserClass::add_class(this, args))->write_id(com);
	  break;
	case UmlSimpleRelations:
	  {
	    UmlCode c;
	    
	    if (!com->get_relation_kind(c, args))
	      ok = FALSE;
	    else {
	      BrowserNode * end = (BrowserNode *) com->get_id(args);
	      
	      if (end->get_type() == UmlUseCase) {
		switch (c) {
		case UmlDependency:
		  add_relation(UmlDependOn, end)->get_browser_node()->write_id(com);
		  break;
		case UmlGeneralisation:
		  add_relation(UmlInherit, end)->get_browser_node()->write_id(com);
		  break;
		default:
		  ok = FALSE;
		}
	      }
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
      else {
	package_modified();
      }
      
      return TRUE;
    }
  case setAssocDiagramCmd:
    if (is_read_only && !root_permission())
      com->write_ack(FALSE);
    else {
      set_associated_diagram((BrowserUseCaseDiagram *) com->get_id(args));
      com->write_ack(TRUE);
    }
    return TRUE;
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

void BrowserUseCase::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlUseCaseDiagram) ||
      UmlDrag::canDecode(e, UmlSeqDiagram) ||
      UmlDrag::canDecode(e, UmlColDiagram) ||
      UmlDrag::canDecode(e, UmlObjectDiagram) ||
      UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserUseCase::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlUseCaseDiagram) ||
      UmlDrag::canDecode(e, UmlSeqDiagram) ||
      UmlDrag::canDecode(e, UmlColDiagram) ||
      UmlDrag::canDecode(e, UmlObjectDiagram) ||
      UmlDrag::canDecode(e, UmlUseCase) ||
      UmlDrag::canDecode(e, UmlClass) ||
      UmlDrag::canDecode(e, BrowserSimpleRelation::drag_key(this)))
    e->accept();
  else
    e->ignore();
}

bool BrowserUseCase::may_contains_them(const QList<BrowserNode> & l,
				       bool & duplicable) const {
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    switch (it.current()->get_type()) {
    case UmlUseCaseDiagram:
    case UmlSeqDiagram:
    case UmlColDiagram:
    case UmlObjectDiagram:
    case UmlUseCase:
    case UmlClass:
      break;
    default:
      if (!IsaSimpleRelation(it.current()->get_type()) ||
	  (((const BrowserNode *) it.current()->parent()) != this))
	return FALSE;
      duplicable = FALSE;
    }
    
    if (! may_contains(it.current(), FALSE))
      return FALSE;
    
    duplicable = may_contains_it(it.current());
  }
  
  return TRUE;
}

void BrowserUseCase::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserUseCase::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, UmlUseCaseDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlSeqDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlColDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlObjectDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlUseCase)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlClass)) != 0) ||
       ((bn = UmlDrag::decode(e, BrowserSimpleRelation::drag_key(this))) != 0)) &&
      (bn != after) && (bn != this)) {
    if (may_contains(bn, bn->get_type() == UmlUseCase)) {
      BrowserNode * x = this;
      
      if ((after == 0) &&
	  (bn->get_type() == UmlUseCase) &&
	  ((BrowserNode *) parent())->may_contains(bn, TRUE)) {
	// have choice
	QPopupMenu m(0);
  
	m.insertItem(new MenuTitle(bn->get_name() + QString(" moving"),
				   m.font()), -1);
	m.insertSeparator();
	m.insertItem("In " + QString(get_name()), 1);
	m.insertItem("After " + QString(get_name()), 2);
	
	switch (m.exec(QCursor::pos())) {
	case 1:
	  break;
	case 2:
	  after = this;
	  x = (BrowserNode *) parent();
	  break;
	default:
	  e->ignore();
	  return;
	}
      }
      BrowserNode * old_parent = (BrowserNode *) bn->parent();
      
      if (after)
	bn->moveItem(after);
      else {
	bn->parent()->takeItem(bn);
	x->insertItem(bn);
      }
      x->package_modified();
      if (old_parent != x)
	old_parent->package_modified();
    }
    else if (after == 0)
      ((BrowserNode *) parent())->DropAfterEvent(e, this);
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

// unicode
const QStringList & BrowserUseCase::default_stereotypes() {
  return its_default_stereotypes;
}

const QStringList & BrowserUseCase::default_stereotypes(UmlCode arrow) {
  if (IsaRelation(arrow))
    return relations_default_stereotypes[arrow];
  
  switch (arrow) {
  case UmlDirectionalAssociation:
    return relations_default_stereotypes[UmlAssociation];
  case UmlDependOn:
    return relations_default_stereotypes[UmlDependency];
  case UmlInherit:
    return relations_default_stereotypes[UmlGeneralisation];
  default:
    {
      static QStringList empty;
      
      return empty;
    }
  }
}

void BrowserUseCase::init()
{
  its_default_stereotypes.append("realization");
  
  int r;
  
  for (r = 0; r != UmlRelations; r += 1)
    relations_default_stereotypes[r].clear();
  
  relations_default_stereotypes[UmlDependency].append("include");
  relations_default_stereotypes[UmlDependency].append("extend");
}

void BrowserUseCase::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "use_case_stereotypes";
  save_unicode_string_list(its_default_stereotypes, st);
  nl_indent(st);  
  BrowserNode::save_stereotypes(st, relations_default_stereotypes);
}

void BrowserUseCase::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "use_case_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    BrowserNode::read_stereotypes(st, relations_default_stereotypes);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserUseCase::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "usecase_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "usecase " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    nl_indent(st);
    st << "//use case diagram settings";
    usecasediagram_settings.save(st);
    nl_indent(st);
    st << "//sequence diagram settings";
    sequencediagram_settings.save(st);
    nl_indent(st);
    st << "//collaboration diagram settings";
    collaborationdiagram_settings.save(st);
    nl_indent(st);
    st << "//object diagram settings";
    objectdiagram_settings.save(st);
  
    bool nl = FALSE;
    
    save_color(st, "duration_color", duration_color, nl);
    save_color(st, "continuation_color", continuation_color, nl);
    save_color(st, "note_color", note_color, nl);
    save_color(st, "usecase_color", usecase_color, nl);
    save_color(st, "package_color", package_color, nl);
    save_color(st, "fragment_color", fragment_color, nl);
    save_color(st, "subject_color", subject_color, nl);
  
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

BrowserUseCase * BrowserUseCase::read_ref(char * & st)
{
  read_keyword(st, "usecase_ref");
  
  int id = read_id(st);
  BrowserUseCase * result = all[id];
  
  return (result == 0)
    ? new BrowserUseCase(id)
    : result;
}

BrowserUseCase * BrowserUseCase::read(char * & st, char * k,
				      BrowserNode * parent)
{
  BrowserUseCase * result;
  int id;
  
  if (!strcmp(k, "usecase_ref")) {
    id = read_id(st);
    result = all[id];
    
    return (result == 0)
      ? new BrowserUseCase(id)
      : result;
  }
  else if (!strcmp(k, "usecase")) {
    id = read_id(st);
    result = all[id];
    
    if (result == 0)
      result = 
	new BrowserUseCase(read_string(st), parent, id);
    else {
      result->set_parent(parent);
      result->set_name(read_string(st));
    }
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    k = read_keyword(st);    
    result->def->read(st, k);				// updates k
    result->usecasediagram_settings.read(st, k);	// updates k
    result->sequencediagram_settings.read(st, k);	// updates k
    result->collaborationdiagram_settings.read(st, k);	// updates k
    if (read_file_format() >= 25)
      result->objectdiagram_settings.read(st, k);	// updates k
    read_color(st, "duration_color", result->duration_color, k);	// updates k
    read_color(st, "continuation_color", result->continuation_color, k);	// updates k
    read_color(st, "note_color", result->note_color, k);	// updates k
    read_color(st, "usecase_color", result->usecase_color, k);	// updates k
    read_color(st, "package_color", result->package_color, k);	// updates k
    read_color(st, "fragment_color", result->fragment_color, k);	// updates k
    read_color(st, "subject_color", result->subject_color, k);	// updates k
    
    if (!strcmp(k, "associated_usecase_diagram")) {
      // old format
      result->set_associated_diagram(BrowserUseCaseDiagram::read_ref(st, "usecasediagram_ref"),
				     TRUE);
      k = read_keyword(st);
    }
    else if (!strcmp(k, "associated_diagram")) {
      result->set_associated_diagram(BrowserUseCaseDiagram::read_ref(st, read_keyword(st)),
				     TRUE);
      k = read_keyword(st);
    }
    
    result->BrowserNode::read(st, k);			// updates k
    
    if (strcmp(k, "end")) {
      while (BrowserUseCase::read(st, k, result) ||
	     BrowserClass::read(st, k, result) ||
	     BrowserObjectDiagram::read(st, k, result) ||
	     BrowserSeqDiagram::read(st, k, result) ||
	     BrowserColDiagram::read(st, k, result) ||
	     BrowserUseCaseDiagram::read(st, k, result) ||
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

BrowserNode * BrowserUseCase::get_it(const char * k, int id)
{
  if (!strcmp(k, "usecase_ref"))
    return all[id];
  
  BrowserNode * r;
  
  if ((r = BrowserUseCaseDiagram::get_it(k, id)) == 0)
    r = BrowserSimpleRelation::get_it(k, id);
  
  return r;
}
