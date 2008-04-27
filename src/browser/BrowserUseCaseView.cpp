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





#include <qpopupmenu.h> 
#include <qcursor.h>

#include "BrowserUseCaseView.h"
#include "SimpleData.h"
#include "BrowserUseCase.h"
#include "BrowserSeqDiagram.h"
#include "BrowserColDiagram.h"
#include "BrowserUseCaseDiagram.h"
#include "BrowserObjectDiagram.h"
#include "BrowserClass.h"
#include "BrowserClassInstance.h"
#include "BrowserState.h"
#include "BrowserActivity.h"
#include "SettingsDialog.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "DialogUtil.h"
#include "ProfiledStereotypes.h"
#include "mu.h"

IdDict<BrowserUseCaseView> BrowserUseCaseView::all(__FILE__);
QStringList BrowserUseCaseView::its_default_stereotypes;	// unicode

BrowserUseCaseView::BrowserUseCaseView(QString s, BrowserNode * p, int id)
    : BrowserNode(s, p), Labeled<BrowserUseCaseView>(all, id) {
  def = new SimpleData;
  def->set_browser_node(this);
  
  note_color = UmlDefaultColor;
  duration_color = UmlDefaultColor;
  continuation_color = UmlDefaultColor;
  usecase_color = UmlDefaultColor;
  package_color = UmlDefaultColor;
  fragment_color = UmlDefaultColor;
  subject_color = UmlDefaultColor;
  class_color = UmlDefaultColor;
  state_color = UmlDefaultColor;
  stateaction_color = UmlDefaultColor;
  activity_color = UmlDefaultColor;
  activityregion_color = UmlDefaultColor;
  activityaction_color = UmlDefaultColor;
  parameterpin_color = UmlDefaultColor;
}

BrowserUseCaseView::BrowserUseCaseView(const BrowserUseCaseView * model,
				       BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserUseCaseView>(all, 0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
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
  class_color = model->class_color;
  state_color = model->state_color;
  stateaction_color = model->stateaction_color;
  activity_color = model->activity_color;
  activityregion_color = model->activityregion_color;
  activityaction_color = model->activityaction_color;
  parameterpin_color = model->parameterpin_color;
}

BrowserUseCaseView::~BrowserUseCaseView() {
  all.remove(get_ident());
}

BrowserNode * BrowserUseCaseView::duplicate(BrowserNode * p, QString name) {
  BrowserUseCaseView * result = new BrowserUseCaseView(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

BrowserUseCaseView* BrowserUseCaseView::add_use_case_view(BrowserNode * future_parent)
{
  QString name;
  
  if (future_parent->enter_child_name(name, "enter use case view's name : ",
				      UmlUseCaseView, TRUE, FALSE))
    return new BrowserUseCaseView(name, future_parent);
  else
    return 0;
}

void BrowserUseCaseView::clear(bool old)
{
  all.clear(old);
  BrowserUseCase::clear(old);
}

void BrowserUseCaseView::update_idmax_for_root()
{
  all.update_idmax_for_root();
  BrowserUseCase::update_idmax_for_root();
}
    
void BrowserUseCaseView::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
  BrowserNode::renumber(phase);
}

const QPixmap* BrowserUseCaseView::pixmap(int) const {
  return (deletedp()) ? DeletedUseCaseViewIcon : UseCaseViewIcon;
}

void BrowserUseCaseView::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_read_only && (edition_number == 0)) {
      m.setWhatsThis(m.insertItem("New use case diagram", 0),
		     "to add a <em>use case diagram</em>");
      m.setWhatsThis(m.insertItem("New sequence diagram", 1),
		     "to add a <em>sequence diagram</em>");
      m.setWhatsThis(m.insertItem("New communication diagram", 2),
		     "to add a <em>communication diagram</em>");
      m.setWhatsThis(m.insertItem("New object diagram", 13),
		     "to add a <em>object diagram</em>");
      m.setWhatsThis(m.insertItem("New use case", 3),
		     "to add a <em>use case</em>");
      m.setWhatsThis(m.insertItem("New actor", 4),
		     "to add an <em>actor</em>");
      m.setWhatsThis(m.insertItem("New class", 5),
		     "to add a <em>class</em>");
      m.setWhatsThis(m.insertItem("New class instance", 14),
		     "to add a <em>class instance</em>");
      m.setWhatsThis(m.insertItem("New state machine", 15),
		     "to add a <em>state machine</em>");
      m.setWhatsThis(m.insertItem("New activity", 16),
		     "to add an <em>activity</em>");
      m.insertSeparator();
      m.setWhatsThis(m.insertItem("New sub use case view", 6),
		     "to add a sub <em>use case view</em>");
      m.insertSeparator();
    }
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Edit", 8),
		     "to edit the <em>use case view</em>");
      if (!is_read_only) {
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Edit drawing settings", 9),
		       "to set how the sub <em>diagrams</em>'s items must be drawn");
	if (edition_number == 0) {
	  m.insertSeparator();
	  m.setWhatsThis(m.insertItem("Delete", 10),
			 "to delete the <em>use case view</em> and its sub items. \
Note that you can undelete them after");
	}
      }
    }
    mark_menu(m, "use case view", 90);
    ProfiledStereotypes::menu(m, this, 99990);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("Undelete", 11),
		   "undelete the <em>use case view</em>. \
Do not undelete its sub items");
    m.setWhatsThis(m.insertItem("Undelete recursively", 12),
		   "undelete the <em>use case view</em> and its sub items");
  }
  
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserUseCaseView::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    {
      BrowserUseCaseDiagram * d = 
	BrowserUseCaseDiagram::add_use_case_diagram(this);
      
      if (d == 0)
	return;
      d->select_in_browser();
    }
    break;
  case 1:
    {
      BrowserSeqDiagram * d = 
	BrowserSeqDiagram::add_sequence_diagram(this);
      
      if (d == 0)
	return;
      d->select_in_browser();
    }
    break;
  case 2:
    {
      BrowserColDiagram * d = 
	BrowserColDiagram::add_collaboration_diagram(this);
      
      if (d == 0)
	return;
      d->select_in_browser();
    }
    break;
  case 3:
    {
      BrowserUseCase *  uc = BrowserUseCase::add_use_case(this);
      
      if (uc == 0)
	return;
      uc->select_in_browser();
    }
    break;
  case 4:
    {
      BrowserClass * a = BrowserClass::add_class(FALSE, this);
      
      if (a == 0)
	return;
      
      a->get_data()->set_stereotype("actor");
      a->select_in_browser();
    }
    break;
  case 5:
    {
      BrowserClass * c = BrowserClass::add_class(FALSE, this);
      
      if (c != 0)
	c->select_in_browser();
    }
    break;
  case 6:
    {
      BrowserUseCaseView * v = 
	BrowserUseCaseView::add_use_case_view(this);
      
      if (v == 0)
	return;
      v->select_in_browser();
    }
    break;
  case 8:
    edit("Use case view", its_default_stereotypes);
    return;
  case 9:
    {
      QArray<StateSpec> st;
      QArray<ColorSpec> co(14);
      
      usecasediagram_settings.complete(st, FALSE);
      objectdiagram_settings.complete(st, FALSE);
      sequencediagram_settings.complete(st, FALSE);
      collaborationdiagram_settings.complete(st, FALSE);
      statediagram_settings.complete(st, FALSE);
      activitydiagram_settings.complete(st, FALSE);
      
      co[0].set("note color", &note_color);
      co[1].set("use case color", &usecase_color);
      co[2].set("package color", &package_color);
      co[3].set("fragment color", &fragment_color);
      co[4].set("subject color", &subject_color);
      co[5].set("duration color", &duration_color);
      co[6].set("continuation color", &continuation_color);
      co[7].set("class color", &class_color);
      co[8].set("state color", &state_color);
      co[9].set("state action color", &stateaction_color);
      co[10].set("activity color", &activity_color);
      co[11].set("activity region color", &activityregion_color);
      co[12].set("activity action color", &activityaction_color);
      co[13].set("parameter and pin color", &parameterpin_color);

      SettingsDialog dialog(&st, &co, FALSE, FALSE);
      
      dialog.raise();
      if (dialog.exec() != QDialog::Accepted)
	return;
      DrawingSettings::modified();
    }
    break;
  case 10:
    delete_it();
    break;
  case 11:
    undelete(FALSE);
    break;
  case 12:
    undelete(TRUE);
    break;
  case 13:
    {
      BrowserObjectDiagram * d = 
	BrowserObjectDiagram::add_object_diagram(this);
      
      if (d == 0)
	return;
      d->select_in_browser();
    }
    break;
  case 14:
    {
      BrowserClassInstance * c = 
	BrowserClassInstance::add_classinstance(this);
      
      if (c != 0)
	c->select_in_browser();
    }
    return; // package_modified called
  case 15:
    {
      BrowserState * st = BrowserState::add_state(this, (bool) TRUE);
      
      if (st == 0)
	return;
      st->select_in_browser();
    }
    break;
  case 16:
    {
      BrowserActivity * a = BrowserActivity::add_activity(this);
      
      if (a != 0)
	a->select_in_browser();
    }
    return; // package_modified called
  default:
    if (rank >= 99990)
      ProfiledStereotypes::choiceManagement(this, rank - 99990);
    else if (rank >= 100)
      ToolCom::run(Tool::command(rank - 100), this);
    else
      mark_management(rank - 90);
    return;
  }
  
  package_modified();
}

void BrowserUseCaseView::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (!is_read_only && (edition_number == 0)) {
      if (s == "New use case diagram")
	choice = 0;
      else if (s == "New sequence diagram")
	choice = 1;
      else if (s == "New communication diagram")
	choice = 2;
      else if (s == "New object diagram")
	choice = 13;
      else if (s == "New use case")
	choice = 3;
      else if (s == "New actor")
	choice = 4;
      else if (s == "New class")
	choice = 5;
      else if (s == "New class instance")
	choice = 14;
      else if (s == "New use case view")
	choice = 6;
      else if (s == "New state machine")
	choice = 15;
      else if (s == "New activity")
	choice = 16;
    }
    if (!is_edited) {
      if (s == "Edit")
	choice = 8;
      if (!is_read_only) {
	if (s == "Edit drawing settings")
	  choice = 9;
	if (edition_number == 0) {
	  if (s == "Delete")
	    choice = 10;
	}
      }
    }
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0)) {
    if (s == "Undelete")
      choice = 11;
    else if (s == "Undelete recursively")
      choice = 12;
  }
  
  exec_menu_choice(choice);
}

void BrowserUseCaseView::open(bool) {
  if (!is_edited)
    edit("Use Case view", its_default_stereotypes);
}

UmlCode BrowserUseCaseView::get_type() const {
  return UmlUseCaseView;
}

int BrowserUseCaseView::get_identifier() const {
  return get_ident();
}

BasicData * BrowserUseCaseView::get_data() const {
  return def;
}

void BrowserUseCaseView::get_usecasediagramsettings(UseCaseDiagramSettings & r) const {
  if (! usecasediagram_settings.complete(r))
    ((BrowserNode *) parent())->get_usecasediagramsettings(r);
}

void BrowserUseCaseView::get_sequencediagramsettings(SequenceDiagramSettings & r) const {
  if (! sequencediagram_settings.complete(r))
    ((BrowserNode *) parent())->get_sequencediagramsettings(r);
}

void BrowserUseCaseView::get_collaborationdiagramsettings(CollaborationDiagramSettings & r) const {
  if (! collaborationdiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_collaborationdiagramsettings(r);
}

void BrowserUseCaseView::get_objectdiagramsettings(ObjectDiagramSettings & r) const {
  if (! objectdiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_objectdiagramsettings(r);
}

void BrowserUseCaseView::get_statediagramsettings(StateDiagramSettings & r) const {
  if (! statediagram_settings.complete(r))
    ((BrowserNode *) parent())->get_statediagramsettings(r);
}

void BrowserUseCaseView::get_statedrawingsettings(StateDrawingSettings & result) const {
  if (!statediagram_settings.statedrawingsettings.complete(result))
    ((BrowserNode *) parent())->get_statedrawingsettings(result);
}

void BrowserUseCaseView::get_activitydiagramsettings(ActivityDiagramSettings & r) const {
  if (! activitydiagram_settings.complete(r))
    ((BrowserNode *) parent())->get_activitydiagramsettings(r);
}

void BrowserUseCaseView::get_activitydrawingsettings(ActivityDrawingSettings & result) const {
  if (!activitydiagram_settings.activitydrawingsettings.complete(result))
    ((BrowserNode *) parent())->get_activitydrawingsettings(result);
}

void BrowserUseCaseView::get_simpleclassdiagramsettings(SimpleClassDiagramSettings & r) const {
  if (!usecasediagram_settings.complete(r))
    ((BrowserNode *) parent())->get_simpleclassdiagramsettings(r);
}

UmlColor BrowserUseCaseView::get_color(UmlCode who) const {
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
  case UmlClass:
    c = class_color;
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
  default:	// UmlDuration
    c = duration_color;
  }
  
  return (c != UmlDefaultColor)
    ? c
    : ((BrowserNode *) parent())->get_color(who);
}

bool BrowserUseCaseView::get_shadow(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlUseCaseDiagram:
    v = usecasediagram_settings.shadow;
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

bool BrowserUseCaseView::get_draw_all_relations(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlUseCaseDiagram:
    v = usecasediagram_settings.draw_all_relations;
    break;
  case UmlSeqDiagram:
    v = sequencediagram_settings.draw_all_relations;
    break;
  case UmlColDiagram:
    v = collaborationdiagram_settings.draw_all_relations;
    break;
  case UmlObjectDiagram:
    v = objectdiagram_settings.draw_all_relations;
    break;
  case UmlStateDiagram:
    v = statediagram_settings.draw_all_relations;
    break;
  default:
    //UmlActivityDiagram
    v = activitydiagram_settings.draw_all_relations;
    break;
  }
  
  switch (v) {
  case UmlYes:
    return TRUE;
  case UmlNo:
    return FALSE;
  default:
    return ((BrowserNode *) parent())->get_draw_all_relations(who);
  }
}

bool BrowserUseCaseView::get_classinstwritehorizontally(UmlCode k) const {
  Uml3States h;
  
  switch (k) {
  case UmlSeqDiagram:
    h = sequencediagram_settings.write_horizontally;
    break;
  case UmlColDiagram:
    h = collaborationdiagram_settings.write_horizontally;
    break;
  default:
    // UmlObjectDiagram
    h = objectdiagram_settings.write_horizontally;
    break;
  }
  
  return (h == UmlDefaultState)
    ? ((BrowserNode *) parent())->get_classinstwritehorizontally(k)
    : (h == UmlYes);
}

bool BrowserUseCaseView::get_auto_label_position(UmlCode who) const {
  Uml3States v;
  
  switch (who) {
  case UmlUseCaseDiagram:
    v = usecasediagram_settings.auto_label_position;
    break;
  case UmlObjectDiagram:
    v = objectdiagram_settings.auto_label_position;
    break;
  case UmlStateDiagram:
    v = statediagram_settings.auto_label_position;
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

bool BrowserUseCaseView::get_write_label_horizontally(UmlCode who) const {
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

bool BrowserUseCaseView::get_show_trans_definition(UmlCode who) const {
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

bool BrowserUseCaseView::get_show_opaque_action_definition(UmlCode who) const {
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

DrawingLanguage BrowserUseCaseView::get_language(UmlCode who) const {
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

bool BrowserUseCaseView::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd:
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	switch (com->get_kind(args)) {
	case UmlUseCase:
	  if (wrong_child_name(args, UmlUseCase, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserUseCase(args, this))->write_id(com);
	  break;
	case UmlUseCaseDiagram:
	  if (wrong_child_name(args, UmlUseCaseDiagram, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserUseCaseDiagram(args, this))->write_id(com);
	  break;
	case UmlUseCaseView:
	  if (wrong_child_name(args, UmlUseCaseView, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (new BrowserUseCaseView(args, this))->write_id(com);
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
	    (BrowserClass::add_class(FALSE, this, args))->write_id(com);
	  break;
	case UmlClassInstance:
	  BrowserClassInstance::add_from_tool(this, com, args);
	  break;
	case UmlState:
	  if (wrong_child_name(args, UmlState, TRUE, FALSE))
	    ok = FALSE;
	  else
	    // force QString(args) else call add_state(BrowserNode, bool) !
	    (BrowserState::add_state(this, QString(args)))->write_id(com);
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
      else {
	package_modified();
      }
      
      return TRUE;
    }
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

void BrowserUseCaseView::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlClass) ||
      UmlDrag::canDecode(e, UmlClassInstance) ||
      UmlDrag::canDecode(e, UmlUseCase) ||
      UmlDrag::canDecode(e, UmlUseCaseDiagram) ||
      UmlDrag::canDecode(e, UmlSeqDiagram) ||
      UmlDrag::canDecode(e, UmlColDiagram) ||
      UmlDrag::canDecode(e, UmlObjectDiagram) ||
      UmlDrag::canDecode(e, UmlUseCaseView) ||
      UmlDrag::canDecode(e, UmlState) ||
      UmlDrag::canDecode(e, UmlActivity)) {
    if (!is_read_only)
      e->accept();
    else
      e->ignore();
  }
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserUseCaseView::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (!is_read_only &&
      (UmlDrag::canDecode(e, UmlClass) ||
       UmlDrag::canDecode(e, UmlClassInstance) ||
       UmlDrag::canDecode(e, UmlUseCase) ||
       UmlDrag::canDecode(e, UmlUseCaseDiagram) ||
       UmlDrag::canDecode(e, UmlSeqDiagram) ||
       UmlDrag::canDecode(e, UmlColDiagram) ||
       UmlDrag::canDecode(e, UmlObjectDiagram) ||
       UmlDrag::canDecode(e, UmlUseCaseView) ||
       UmlDrag::canDecode(e, UmlState) ||
       UmlDrag::canDecode(e, UmlActivity)))
    e->accept();
  else
    e->ignore();
}

bool BrowserUseCaseView::may_contains_them(const QList<BrowserNode> & l,
					   bool & duplicable) const {
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    switch (it.current()->get_type()) {
    case UmlUseCaseView:
      duplicable = FALSE;
      // no break
    case UmlClass:
    case UmlClassInstance:
    case UmlUseCase:
    case UmlUseCaseDiagram:
    case UmlSeqDiagram:
    case UmlColDiagram:
    case UmlObjectDiagram:
    case UmlState:
    case UmlActivity:
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

BrowserNode * BrowserUseCaseView::container(UmlCode c) const {
  // currently only for class, state machine and activity
  return (c == UmlClass)
    ? (BrowserNode *) this
    : ((BrowserNode *) parent())->container(c);
}

void BrowserUseCaseView::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserUseCaseView::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, UmlClass)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlClassInstance)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlUseCase)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlSeqDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlColDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlObjectDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlUseCaseDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlUseCaseView)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlState)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlActivity)) != 0)) &&
      (bn != this) && (bn != this)) {
    if (may_contains(bn, bn->get_type() == UmlUseCaseView)) {
      BrowserNode * x = this;
      
      if ((after == 0) &&
	  (bn->get_type() == UmlUseCaseView) &&
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
      if (old_parent != x) {
	old_parent->package_modified();
	bn->modified();
      }
    }
    else if (after == 0)
      ((BrowserNode *) parent())->DropAfterEvent(e, this);
    else {
      msg_critical("Error", "Forbidden");
      e->ignore();
    }
  }
  else if ((bn == 0) && (after == 0))
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

void BrowserUseCaseView::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "usecaseview_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserUseCaseView::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "usecaseview_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserUseCaseView::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "usecaseview_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "usecaseview " << get_ident() << ' ';
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
    nl_indent(st);
    st << "//state diagram settings";
    statediagram_settings.save(st);
    nl_indent(st);
    st << "//activity diagram settings";
    activitydiagram_settings.save(st);
    
    bool nl = FALSE;
    
    save_color(st, "duration_color", duration_color, nl);
    save_color(st, "continuation_color", continuation_color, nl);
    save_color(st, "note_color", note_color, nl);
    save_color(st, "usecase_color", usecase_color, nl);
    save_color(st, "package_color", package_color, nl);
    save_color(st, "fragment_color", fragment_color, nl);
    save_color(st, "subject_color", subject_color, nl);
    save_color(st, "class_color", class_color, nl);
    save_color(st, "state_color", state_color, nl);
    save_color(st, "stateaction_color", stateaction_color, nl);
    save_color(st, "activity_color", activity_color, nl);
    save_color(st, "activityregion_color", activityregion_color, nl);
    save_color(st, "activityaction_color", activityaction_color, nl);
    save_color(st, "parameterpin_color", parameterpin_color, nl);
    
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

BrowserUseCaseView * BrowserUseCaseView::read(char * & st, char * k,
					      BrowserNode * parent, bool recursive)
{
  if (!strcmp(k, "usecaseview")) {
    int id = read_id(st);
    BrowserUseCaseView * already_exist = all[id];
    BrowserUseCaseView * result =
      new BrowserUseCaseView(read_string(st), parent, id);

    if (already_exist != 0) {
      already_exist->must_change_id(all);
      already_exist->unconsistent_fixed("use case views", result);
    }
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
      
    k = read_keyword(st);
      
    result->def->read(st, k);						// updates k
    result->usecasediagram_settings.read(st, k);			// updates k
    result->sequencediagram_settings.read(st, k);			// updates k
    result->collaborationdiagram_settings.read(st, k);		// updates k
    if (read_file_format() >= 25)
      result->objectdiagram_settings.read(st, k);		// updates k
    if (read_file_format() >= 43) {
      result->statediagram_settings.read(st, k);		// updates k
      result->activitydiagram_settings.read(st, k);		// updates k
    }
    read_color(st, "duration", result->duration_color, k);	// old, updates k
    read_color(st, "duration_color", result->duration_color, k);	// updates k
    read_color(st, "continuation_color", result->continuation_color, k);	// updates k
    read_color(st, "note_color", result->note_color, k);		// updates k
    read_color(st, "usecase_color", result->usecase_color, k);	// updates k
    read_color(st, "package_color", result->package_color, k);	// updates k
    read_color(st, "fragment_color", result->fragment_color, k);	// updates k
    read_color(st, "subject_color", result->subject_color, k);	// updates k
    if (read_file_format() >= 52)
      read_color(st, "class_color", result->class_color, k);	// updates k
    else
      read_color(st, "classinstance_color", result->class_color, k);	// updates k
    if (read_file_format() >= 43) {
      read_color(st, "state_color", result->state_color, k);		// updates k
      read_color(st, "stateaction_color", result->stateaction_color, k);		// updates k
      read_color(st, "activity_color", result->activity_color, k);		// updates k
      read_color(st, "activityregion_color", result->activityregion_color, k);		// updates k
      read_color(st, "activityaction_color", result->activityaction_color, k);		// updates k
      read_color(st, "parameterpin_color", result->parameterpin_color, k);		// updates k
    }    
    result->BrowserNode::read(st, k);	// updates k
      
    if (strcmp(k, "end")) {
      while (BrowserUseCaseDiagram::read(st, k, result) ||
	     BrowserUseCase::read(st, k, result) ||
	     BrowserUseCaseView::read(st, k, result, recursive) ||
	     BrowserClass::read(st, k, result) ||
	     BrowserClassInstance::read(st, k, result) ||
	     BrowserSeqDiagram::read(st, k, result) ||
	     BrowserColDiagram::read(st, k, result) ||
	     BrowserObjectDiagram::read(st, k, result) ||
	     BrowserState::read(st, k, result) ||
	     BrowserActivity::read(st, k, result))
	k = read_keyword(st);
	
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserUseCaseView::get_it(const char * k, int id)
{
  if (!strcmp(k, "usecaseview_ref"))
    return all[id];
  
  BrowserNode * r;
  
  if ((r = BrowserUseCaseDiagram::get_it(k, id)) == 0)
    // class, seq diag, col diag, state and activity managed in class view
    r = BrowserUseCase::get_it(k, id);
  
  return r;
}
