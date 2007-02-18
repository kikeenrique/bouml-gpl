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
#include <qmessagebox.h>
#include <qcursor.h>

#include "BrowserClassView.h"
#include "ClassViewDialog.h"
#include "BrowserClassDiagram.h"
#include "BrowserColDiagram.h"
#include "BrowserSeqDiagram.h"
#include "BrowserObjectDiagram.h"
#include "BrowserDeploymentView.h"
#include "BrowserClass.h"
#include "BrowserState.h"
#include "BrowserActivity.h"
#include "BrowserActivityAction.h"
#include "SimpleData.h"
#include "ClassData.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "mu.h"

IdDict<BrowserClassView> BrowserClassView::all;
QStringList BrowserClassView::its_default_stereotypes;	// unicode

BrowserClassView::BrowserClassView(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserClassView>(all, id),
      associated_deployment_view(0) {
  def = new SimpleData;
  def->set_browser_node(this);
  
  class_color = UmlDefaultColor;
  note_color = UmlDefaultColor;
  fragment_color = UmlDefaultColor;
  package_color = UmlDefaultColor;
  duration_color = UmlDefaultColor;
  continuation_color = UmlDefaultColor;
  state_color = UmlDefaultColor;
  stateaction_color = UmlDefaultColor;
  activity_color = UmlDefaultColor;
  activityregion_color = UmlDefaultColor;
  activityaction_color = UmlDefaultColor;
  parameterpin_color = UmlDefaultColor;
}

BrowserClassView::BrowserClassView(const BrowserClassView * model,
				   BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserClassView>(all, 0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
  classdiagram_settings = model->classdiagram_settings;
  sequencediagram_settings = model->sequencediagram_settings;
  collaborationdiagram_settings = model->collaborationdiagram_settings;
  objectdiagram_settings = model->objectdiagram_settings;
  statediagram_settings = model->statediagram_settings;
  activitydiagram_settings = model->activitydiagram_settings;
  class_settings = model->class_settings;
  class_color = model->class_color;
  note_color = model->note_color;
  fragment_color = model->fragment_color;
  package_color = model->package_color;
  duration_color = model->duration_color;
  continuation_color = model->continuation_color;
  state_color = model->state_color;
  stateaction_color = model->stateaction_color;
  activity_color = model->activity_color;
  activityregion_color = model->activityregion_color;
  activityaction_color = model->activityaction_color;
  parameterpin_color = model->parameterpin_color;
}

BrowserClassView::~BrowserClassView() {
  all.remove(get_ident());
}

BrowserNode * BrowserClassView::duplicate(BrowserNode * p, QString name) {
  BrowserClassView * result = new BrowserClassView(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

void BrowserClassView::clear(bool old)
{
  all.clear(old);
  BrowserClass::clear(old);
}

void BrowserClassView::update_idmax_for_root()
{
  all.update_idmax_for_root();
  BrowserClass::update_idmax_for_root();
}
    
void BrowserClassView::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
  BrowserNode::renumber(phase);
}

const QPixmap* BrowserClassView::pixmap(int) const {
  return (deletedp()) ? DeletedClassViewIcon : ClassViewIcon;
}

QString BrowserClassView::full_name(bool rev, bool itself) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  if (! itself)
    return p; // to goes up
  else if (p.isEmpty()) 
    return QString((const char *) name);
  else if (rev)
    return name + "   [" + p + "]";
  else
    return p + "::" + name;
}

void BrowserClassView::menu() {
  QPopupMenu m(0);
  QPopupMenu subm(0);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_read_only && (edition_number == 0)) {
      m.setWhatsThis(m.insertItem("New class diagram", 0),
		     "to add a <em>class diagram</em>");
      m.setWhatsThis(m.insertItem("New sequence diagram", 1),
		     "to add a <em>sequence diagram</em>");
      m.setWhatsThis(m.insertItem("New collaboration diagram", 2),
		     "to add a <em>collaboration diagram</em>");
      m.setWhatsThis(m.insertItem("New object diagram", 15),
		     "to add a <em>object diagram</em>");
      m.setWhatsThis(m.insertItem("New class", 3),
		     "to add a <em>class</em>");
      m.setWhatsThis(m.insertItem("New state machine", 4),
		     "to add a <em>state machine</em>");
      m.setWhatsThis(m.insertItem("New activity", 16),
		     "to add an <em>activity</em>");
      m.insertSeparator();
    }
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Edit", 5),
		     "to edit the <em>class view</em>");
      if (!is_read_only) {
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Edit class settings", 6),
		       "to set the sub classes's settings");
	m.setWhatsThis(m.insertItem("Edit drawing settings", 7),
		       "to set how the sub <em>class diagrams</em>'s items must be drawed");
	if (edition_number == 0) {
	  m.insertSeparator();
	  m.setWhatsThis(m.insertItem("Delete", 8),
			 "to delete the <em>class view</em> and its sub items. \
Note that you can undelete them after");
	}
      }
    }
    mark_menu(m, "use case view", 90);
    m.insertSeparator();
    m.insertItem("Generate", &subm);
    subm.insertItem("C++", 11);
    subm.insertItem("Java", 12);
    subm.insertItem("Idl", 13);
    
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("Undelete", 9),
		   "undelete the <em>class view</em>. \
Do not undelete its sub items");
    m.setWhatsThis(m.insertItem("Undelete recursively", 10),
		   "undelete the <em>class view</em> and its sub items");
  }
  
  if (associated_deployment_view != 0) {
    m.insertSeparator();
    m.setWhatsThis(m.insertItem("Select associated deployment view", 14),
		   "to select the associated <em>deployment view</em>");
  }
  
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserClassView::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    add_class_diagram();
    break;
  case 1:
    add_sequence_diagram();
    break;
  case 2:
    add_collaboration_diagram();
    break;
  case 3:
    {
      BrowserClass * cl = BrowserClass::add_class(this);
      
      if (cl != 0)
	cl->select_in_browser();
    }
    break;
  case 4:
    {
      BrowserState * st = BrowserState::add_state(this, (bool) TRUE);
      
      if (st != 0)
	st->select_in_browser();
    }
    return;	// modified called
  case 5:
    (new ClassViewDialog(get_data()))->show();
    return;
  case 6:
    if (! class_settings.edit(UmlClassView))
      return;
    break;
  case 7:
    {
      QArray<StateSpec> st;
      QArray<ColorSpec> co(11);
      
      classdiagram_settings.complete(st, UmlClassView);
      sequencediagram_settings.complete(st, FALSE);
      collaborationdiagram_settings.complete(st, FALSE);
      objectdiagram_settings.complete(st, FALSE);
      statediagram_settings.complete(st, FALSE);
      activitydiagram_settings.complete(st, FALSE);
      
      co[0].set("class color", &class_color);
      co[1].set("note color", &note_color);
      co[2].set("package color", &package_color);
      co[3].set("fragment color", &fragment_color);
      co[4].set("duration color", &duration_color);
      co[5].set("continuation color", &continuation_color);
      co[6].set("state color", &state_color);
      co[7].set("state action color", &stateaction_color);
      co[8].set("activity color", &activity_color);
      co[8].set("activity region color", &activityregion_color);
      co[9].set("activity action color", &activityaction_color);
      co[10].set("parameter and pin color", &parameterpin_color);

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
    {
      bool preserve = preserve_bodies();
      
      ToolCom::run((verbose_generation()) 
		   ? ((preserve) ? "cpp_generator -v -p" : "cpp_generator -v")
		   : ((preserve) ? "cpp_generator -p" : "cpp_generator"),
		   this);
    }
    return;
  case 12:
    {
      bool preserve = preserve_bodies();
      
      ToolCom::run((verbose_generation()) 
		   ? ((preserve) ? "java_generator -v -p" : "java_generator -v")
		   : ((preserve) ? "java_generator -p" : "java_generator"), 
		   this);
    }
    return;
  case 13:
    ToolCom::run((verbose_generation()) ? "idl_generator -v" : "idl_generator", this);
    return;
  case 14:
    associated_deployment_view->select_in_browser();
    return;
  case 15:
    add_object_diagram();
    break;
  case 16:
    
    {
      BrowserActivity * a = BrowserActivity::add_activity(this);
      
      if (a != 0)
	a->select_in_browser();
    }
    return; // package_modified called
  default:
    if (rank >= 100)
      ToolCom::run(Tool::command(rank - 100), this);
    else
      mark_management(rank - 90);
    return;
  }
  
  package_modified();
}

void BrowserClassView::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (!is_read_only && (edition_number == 0)) {
      if (s == "New class diagram")
	choice = 0;
      else if (s == "New sequence diagram")
	choice = 1;
      else if (s == "New collaboration diagram")
	choice = 2;
      else if (s == "New object diagram")
	choice = 15;
      else if (s == "New class")
	choice = 3;
      else if (s == "New state machine")
	choice = 4;
      else if (s == "New activity")
	choice = 16;
    }
    if (!is_edited) {
      if (s == "Edit")
	choice = 5;
      if (!is_read_only) {
	if (s == "Edit class settings")
	  choice = 6;
	else if (s == "Edit drawing settings")
	  choice = 7;
	if (edition_number == 0) {
	  if (s == "Delete")
	    choice = 8;
	}
      }
    }
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
    if (s == "Generate C++")
      choice = 11;
    else if (s == "Generate Java")
      choice = 12;
    else if (s == "Generate Idl")
      choice = 13;
  }
  else if (!is_read_only && (edition_number == 0)) {
    if (s == "Undelete")
      choice = 9;
    else if (s == "Undelete recursively")
      choice = 10;
  }
  
  if (associated_deployment_view != 0) {
    if (s == "Select associated deployment view")
      choice = 14;
  }
  
  exec_menu_choice(choice);
}

void BrowserClassView::open(bool) {
  if (!is_edited)
    (new ClassViewDialog(get_data()))->show();
}

void BrowserClassView::add_class_diagram() {
  (new BrowserClassDiagram(child_random_name("Diagram"), this))->select_in_browser();
}

void BrowserClassView::add_object_diagram() {
  (new BrowserObjectDiagram(child_random_name("Object Diagram"), this))
    ->select_in_browser();
}

void BrowserClassView::add_sequence_diagram() {
  (new BrowserSeqDiagram(child_random_name("Sequence Diagram"), this))
    ->select_in_browser();
}

void BrowserClassView::add_collaboration_diagram() {
  (new BrowserColDiagram(child_random_name("Collaboration Diagram"), this))
    ->select_in_browser();
}

UmlCode BrowserClassView::get_type() const {
  return UmlClassView;
}

BasicData * BrowserClassView::get_data() const {
  return def;
}


BrowserNode * BrowserClassView::get_associated() const {
  return associated_deployment_view;
}

void BrowserClassView::set_associated_deploymentview(BrowserDeploymentView * cv,
						    bool on_read) {
  if (associated_deployment_view != cv) {
    if (associated_deployment_view != 0)
      QObject::disconnect(associated_deployment_view->get_data(), SIGNAL(deleted()), 
			  def, SLOT(on_delete()));
    if ((associated_deployment_view = cv) != 0)
      QObject::connect(associated_deployment_view->get_data(), SIGNAL(deleted()), 
		       def, SLOT(on_delete()));
    
    if (! on_read)
      package_modified();
  }
}

void BrowserClassView::on_delete() {
  if (associated_deployment_view &&
      associated_deployment_view->deletedp())
    associated_deployment_view = 0;
}


void BrowserClassView::get_classdiagramsettings(ClassDiagramSettings & r) const {
  if (! classdiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_classdiagramsettings(r);
}

void BrowserClassView::get_sequencediagramsettings(SequenceDiagramSettings & r) const {
  if (! sequencediagram_settings.complete(r))
    ((BrowserNode *) parent())->get_sequencediagramsettings(r);
}

void BrowserClassView::get_collaborationdiagramsettings(CollaborationDiagramSettings & r) const {
  if (! collaborationdiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_collaborationdiagramsettings(r);
}

void BrowserClassView::get_objectdiagramsettings(ObjectDiagramSettings & r) const {
  if (! objectdiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_objectdiagramsettings(r);
}

void BrowserClassView::get_statediagramsettings(StateDiagramSettings & r) const {
  if (! statediagram_settings.complete(r))
    ((BrowserNode *) parent())->get_statediagramsettings(r);
}

void BrowserClassView::get_statedrawingsettings(StateDrawingSettings & result) const {
  if (!statediagram_settings.statedrawingsettings.complete(result))
    ((BrowserNode *) parent())->get_statedrawingsettings(result);
}

void BrowserClassView::get_activitydiagramsettings(ActivityDiagramSettings & r) const {
  if (! activitydiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_activitydiagramsettings(r);
}

void BrowserClassView::get_activitydrawingsettings(ActivityDrawingSettings & result) const {
  if (!activitydiagram_settings.activitydrawingsettings.complete(result))
    ((BrowserNode *) parent())->get_activitydrawingsettings(result);
}

UmlColor BrowserClassView::get_color(UmlCode who) const {
  UmlColor c;
  
  switch (who) {
  case UmlClass:
    c = class_color;
    break;
  case UmlNote:
    c = note_color;
    break;
  case UmlFragment:
    c = fragment_color;
    break;
  case UmlContinuation:
    c = continuation_color;
    break;
  case UmlActivityDuration:
    c = duration_color;
    break;
  case UmlState:
    c = state_color;
    break;
  case UmlStateAction:
    c = stateaction_color;
    break;
  case UmlActivity:
    c = activity_color;
    break;
  case UmlInterruptibleActivityRegion:
  case UmlExpansionRegion:
    c = activityregion_color;
    break;
  case UmlActivityAction:
    c = activityaction_color;
    break;
  case UmlExpansionNode:
  case UmlParameter:
  case UmlActivityPin:
    c = parameterpin_color;
    break;
  default: // UmlPackage
    c = package_color;
    break;
  }
  
  return (c != UmlDefaultColor)
    ? c
    : ((BrowserNode *) parent())->get_color(who);
}

bool BrowserClassView::get_shadow(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlClassDiagram:
    v = classdiagram_settings.shadow;
    break;
  case UmlSeqDiagram:
    v = sequencediagram_settings.shadow;
    break;
  case UmlColDiagram:
    v = collaborationdiagram_settings.shadow;
    break;
  case UmlObjectDiagram:
    v = objectdiagram_settings.shadow;
    break;
  case UmlStateDiagram:
    v = statediagram_settings.shadow;
    break;
  default:
    //UmlActivityDiagram
    v = activitydiagram_settings.shadow;
    break;
  }
  
  switch (v) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_shadow(who);
  }
}

UmlVisibility BrowserClassView::get_visibility(UmlCode who) const {
  UmlVisibility v;
  
  switch (who) {
  case UmlAttribute:
    v = class_settings.attribute_visibility;
    break;
  case UmlRelations:
    v = class_settings.relation_visibility;
    break;
  default:	// UmlOperation
    v = class_settings.operation_visibility;
    break;
  }
  
  return (v != UmlDefaultVisibility)
    ? v
    : ((BrowserNode *) parent())->get_visibility(who);
}

bool BrowserClassView::get_auto_label_position(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlClassDiagram:
    v = classdiagram_settings.auto_label_position;
    break;
  case UmlStateDiagram:
    v = statediagram_settings.auto_label_position;
    break;
  case UmlObjectDiagram:
    v = objectdiagram_settings.auto_label_position;
    break;
  case UmlActivityDiagram:
    v = activitydiagram_settings.auto_label_position;
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

bool BrowserClassView::get_write_label_horizontally(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlStateDiagram:
    v = statediagram_settings.write_label_horizontally;
    break;
  case UmlActivityDiagram:
    v = activitydiagram_settings.write_label_horizontally;
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
    return ((BrowserNode *) parent())->get_write_label_horizontally(who);
  }
}

bool BrowserClassView::get_show_trans_definition(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlStateDiagram:
    v = statediagram_settings.show_trans_definition;
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
    return ((BrowserNode *) parent())->get_show_trans_definition(who);
  }
}

bool BrowserClassView::get_show_opaque_action_definition(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlActivityDiagram:
    v = activitydiagram_settings.show_opaque_action_definition;
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
    return ((BrowserNode *) parent())->get_show_opaque_action_definition(who);
  }
}

DrawingLanguage BrowserClassView::get_language(UmlCode who) const {
  DrawingLanguage v;
  
  switch (who) {
  case UmlStateDiagram:
    v = statediagram_settings.statedrawingsettings.drawing_language;
    break;
  case UmlActivityDiagram:
    v = activitydiagram_settings.activitydrawingsettings.drawing_language;
    break;
  default:
    // error
    return UmlView;
  }
  
  return (v != DefaultDrawingLanguage)
    ? v
    : ((BrowserNode *) parent())->get_language(who);
}

bool BrowserClassView::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd: 
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	switch (com->get_kind(args)) {
	case UmlClassDiagram:
	  if (wrong_child_name(args, UmlClassDiagram, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserClassDiagram(args, this))->write_id(com);
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
	case UmlClass:
	  if (wrong_child_name(args, UmlClass, FALSE, FALSE))
	    ok = FALSE;
	  else
	    (BrowserClass::add_class(this, args))->write_id(com);
	  break;
	case UmlState:
	  if (wrong_child_name(args, UmlState, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (BrowserState::add_state(this, args))->write_id(com);
	  break;
	case UmlActivity:
	  if (wrong_child_name(args, UmlActivity, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (BrowserActivity::add_activity(this, args))->write_id(com);
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
  case setAssocViewCmd:
    if (is_read_only && !root_permission())
      com->write_ack(FALSE);
    else {
      set_associated_deploymentview((BrowserDeploymentView *) com->get_id(args));
      com->write_ack(TRUE);
    }
    return TRUE;
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

void BrowserClassView::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlClass) ||
      UmlDrag::canDecode(e, UmlClassDiagram) ||
      UmlDrag::canDecode(e, UmlSeqDiagram) ||
      UmlDrag::canDecode(e, UmlColDiagram) ||
      UmlDrag::canDecode(e, UmlObjectDiagram) ||
      UmlDrag::canDecode(e, UmlState) ||
      UmlDrag::canDecode(e, UmlStateDiagram) ||
      UmlDrag::canDecode(e, UmlActivity) ||
      UmlDrag::canDecode(e, UmlActivityDiagram))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserClassView::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlClass) ||
      UmlDrag::canDecode(e, UmlClassDiagram) ||
      UmlDrag::canDecode(e, UmlSeqDiagram) ||
      UmlDrag::canDecode(e, UmlColDiagram) ||
      UmlDrag::canDecode(e, UmlObjectDiagram) ||
      UmlDrag::canDecode(e, UmlState) ||
      UmlDrag::canDecode(e, UmlStateDiagram) ||
      UmlDrag::canDecode(e, UmlActivity) ||
      UmlDrag::canDecode(e, UmlActivityDiagram))
    e->accept();
  else
    e->ignore();
}

bool BrowserClassView::may_contains_them(const QList<BrowserNode> & l,
					 bool & duplicable) const {
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    switch (it.current()->get_type()) {
    case UmlClass:
    case UmlClassDiagram:
    case UmlSeqDiagram:
    case UmlColDiagram:
    case UmlObjectDiagram:
    case UmlState:
    case UmlStateDiagram:
    case UmlActivity:
    case UmlActivityDiagram:
      if (! may_contains(it.current(), FALSE))
	return FALSE;
      break;
    default:
      return FALSE;
    }
    
    duplicable = may_contains_it(it.current());
  }
  
  return TRUE;
}

void BrowserClassView::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserClassView::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, UmlClass)) != 0) ||
      (((bn = UmlDrag::decode(e, UmlClassDiagram)) != 0)) ||
      (((bn = UmlDrag::decode(e, UmlSeqDiagram)) != 0)) ||
      (((bn = UmlDrag::decode(e, UmlColDiagram)) != 0)) ||
      (((bn = UmlDrag::decode(e, UmlObjectDiagram)) != 0)) ||
      (((bn = UmlDrag::decode(e, UmlState)) != 0)) ||
      (((bn = UmlDrag::decode(e, UmlStateDiagram)) != 0)) ||
      (((bn = UmlDrag::decode(e, UmlActivity)) != 0)) ||
      (((bn = UmlDrag::decode(e, UmlActivityDiagram)) != 0))) &&
      (bn != after) && (bn != this)) {
    if (may_contains(bn, FALSE)) {
      BrowserNode * old_parent = (BrowserNode *) bn->parent();
      
      if (after)
	bn->moveItem(after);
      else {
	bn->parent()->takeItem(bn);
	insertItem(bn);
      }
      package_modified();
      if (old_parent != this)
	old_parent->package_modified();
    }
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

// unicode
const QStringList & BrowserClassView::default_stereotypes()
{
  return its_default_stereotypes;
}

void BrowserClassView::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "classview_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserClassView::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "classview_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserClassView::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "classview_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "classview " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    
    def->save(st, warning);
    nl_indent(st);
    st << "//class diagram settings";
    classdiagram_settings.save(st);
    nl_indent(st);
    st << "//collaboration diagram settings";
    collaborationdiagram_settings.save(st);
    nl_indent(st);
    st << "//object diagram settings";
    objectdiagram_settings.save(st);
    nl_indent(st);
    st << "//sequence diagram settings";
    sequencediagram_settings.save(st);
    nl_indent(st);
    st << "//state diagram settings";
    statediagram_settings.save(st);
    nl_indent(st);
    st << "//class settings";
    class_settings.save(st);
    nl_indent(st);
    st << "//activity diagram settings";
    activitydiagram_settings.save(st);
    nl_indent(st);
    
    bool nl = FALSE;
    
    save_color(st, "class_color", class_color, nl);
    save_color(st, "note_color", note_color, nl);
    save_color(st, "package_color", package_color, nl);
    save_color(st, "fragment_color", fragment_color, nl);
    save_color(st, "duration_color", duration_color, nl);
    save_color(st, "continuation_color", continuation_color, nl);
    save_color(st, "state_color", state_color, nl);
    save_color(st, "stateaction_color", stateaction_color, nl);
    save_color(st, "activity_color", activity_color, nl);
    save_color(st, "activityregion_color", activityregion_color, nl);
    save_color(st, "activityaction_color", activityaction_color, nl);
    save_color(st, "parameterpin_color", parameterpin_color, nl);
    
    if (associated_deployment_view != 0) {
      nl_indent(st);
      st << "associated_deployment_view ";
      associated_deployment_view->save(st, TRUE, warning);
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
	else {
	  // set 'not modified' to delete the associated file on exit
	  ((BrowserNode *) child)->unmodified();
	  
	  if ((child = child->nextSibling()) == 0)
	    break;
	}
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

BrowserClassView * BrowserClassView::read(char * & st, char * k,
					  BrowserNode * parent,
					  bool /*recursive*/)
{
  if (!strcmp(k, "classview")) {
    int id = read_id(st);
    
    BrowserClassView * r = all[id];
    
    if (r != 0) {
      QMessageBox::critical(0, "Error", "reload not yet implemented");
      
      throw 0;
    }
    else {
      r = new BrowserClassView(read_string(st), parent, id);
      
      r->is_read_only = !in_import() && read_only_file() || 
	(user_id() != 0) && r->is_api_base();
      
      k = read_keyword(st);
      
      r->def->read(st, k);					// updates k
      r->classdiagram_settings.read(st, k);			// updates k
      if (read_file_format() >= 6) {
	r->collaborationdiagram_settings.read(st, k);		// updates k
	if (read_file_format() >= 25)
	  r->objectdiagram_settings.read(st, k);		// updates k
	r->sequencediagram_settings.read(st, k);		// updates k
      }
      if (read_file_format() >= 21)
	r->statediagram_settings.read(st, k);			// updates k
      r->class_settings.read(st, k);				// updates k
      if (read_file_format() >= 26)
	r->activitydiagram_settings.read(st, k);		// updates k
      read_color(st, "class_color", r->class_color, k);		// updates k
      read_color(st, "note_color", r->note_color, k);		// updates k
      read_color(st, "package_color", r->package_color, k);	// updates k
      read_color(st, "fragment_color", r->fragment_color, k);	// updates k
      read_color(st, "duration_color", r->duration_color, k);	// updates k
      read_color(st, "continuation_color", r->continuation_color, k);	// updates k
      read_color(st, "state_color", r->state_color, k);		// updates k
      read_color(st, "stateaction_color", r->stateaction_color, k);		// updates k
      read_color(st, "activity_color", r->activity_color, k);		// updates k
      read_color(st, "activityregion_color", r->activityregion_color, k);		// updates k
      read_color(st, "activityaction_color", r->activityaction_color, k);		// updates k
      read_color(st, "parameterpin_color", r->parameterpin_color, k);		// updates k
      if ((read_file_format() < 20) && !strcmp(k, "associated_componentview")) {
	// old format
	r->set_associated_deploymentview(BrowserDeploymentView::read_ref(st, "deploymentview_ref"),
					 TRUE);
	k = read_keyword(st);
      }
      else if (!strcmp(k, "associated_deployment_view") ||
	       ((read_file_format() < 20) && !strcmp(k, "associated_component_view"))) {
	r->set_associated_deploymentview(BrowserDeploymentView::read_ref(st, read_keyword(st)),
					 TRUE);
	k = read_keyword(st);
      }
      
      r->BrowserNode::read(st, k);				// updates k
      
      if (strcmp(k, "end")) {
	while (BrowserClassDiagram::read(st, k, r) ||
	       BrowserColDiagram::read(st, k, r) ||
	       BrowserSeqDiagram::read(st, k, r) ||
	       BrowserObjectDiagram::read(st, k, r) ||
	       BrowserClass::read(st, k, r) ||
	       BrowserState::read(st, k, r) ||
	       BrowserActivity::read(st, k, r) ||
	       BrowserActivityAction::read(st, k, r))
	  k = read_keyword(st);
	
	if (strcmp(k, "end"))
	  wrong_keyword(k, "end");
      }
      
      return r;
    }
  }
  else
    return 0;
}

BrowserNode * BrowserClassView::get_it(const char * k, int id)
{
  if (!strcmp(k, "classview_ref"))
    return all[id];
  
  BrowserNode * r;
  
  if (((r = BrowserClassDiagram::get_it(k, id)) == 0) && 
      ((r = BrowserColDiagram::get_it(k, id)) == 0) && 
      ((r = BrowserSeqDiagram::get_it(k, id)) == 0) &&
      ((r = BrowserObjectDiagram::get_it(k, id)) == 0) &&
      ((r = BrowserClass::get_it(k, id)) == 0) &&
      ((r = BrowserState::get_it(k, id)) == 0))
    r = BrowserActivity::get_it(k, id);
  
  return r;
}

