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

#include "BrowserUseCaseView.h"
#include "SimpleData.h"
#include "BrowserUseCase.h"
#include "BrowserSeqDiagram.h"
#include "BrowserColDiagram.h"
#include "BrowserUseCaseDiagram.h"
#include "BrowserObjectDiagram.h"
#include "BrowserClass.h"
#include "SettingsDialog.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "DialogUtil.h"
#include "mu.h"

IdDict<BrowserUseCaseView> BrowserUseCaseView::all;
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
  classinstance_color = UmlDefaultColor;
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
  classinstance_color = model->classinstance_color;
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
      m.setWhatsThis(m.insertItem("New collaboration diagram", 2),
		     "to add a <em>collaboration diagram</em>");
      m.setWhatsThis(m.insertItem("New object diagram", 13),
		     "to add a <em>object diagram</em>");
      m.setWhatsThis(m.insertItem("New use case", 3),
		     "to add a <em>use case</em>");
      m.setWhatsThis(m.insertItem("New actor", 4),
		     "to add an <em>actor</em>");
      m.setWhatsThis(m.insertItem("New class", 5),
		     "to add a <em>class</em>");
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
		       "to set how the sub <em>diagrams</em>'s items must be drawed");
	if (edition_number == 0) {
	  m.insertSeparator();
	  m.setWhatsThis(m.insertItem("Delete", 10),
			 "to delete the <em>use case view</em> and its sub items. \
Note that you can undelete them after");
	}
      }
    }
    mark_menu(m, "use case view", 90);
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
      BrowserUseCase *  uc = BrowserUseCase::add_use_case(this);
      
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
      BrowserClass * c = BrowserClass::add_class(this);
      
      if (c != 0)
	c->select_in_browser();
    }
    break;
  case 6:
    add_use_cases_list();
    break;
  case 8:
    edit("Use case view", its_default_stereotypes);
    return;
  case 9:
    {
      QArray<StateSpec> st;
      QArray<ColorSpec> co(8);
      
      usecasediagram_settings.complete(st, FALSE);
      objectdiagram_settings.complete(st, FALSE);
      sequencediagram_settings.complete(st, FALSE);
      collaborationdiagram_settings.complete(st, FALSE);
      
      co[0].set("note color", &note_color);
      co[1].set("use case color", &usecase_color);
      co[2].set("package color", &package_color);
      co[3].set("fragment color", &fragment_color);
      co[4].set("subject color", &subject_color);
      co[5].set("duration color", &duration_color);
      co[6].set("continuation color", &continuation_color);
      co[7].set("class instance color", &usecase_color);

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

void BrowserUseCaseView::apply_shortcut(QString s) {
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
      else if (s == "New use case view")
	choice = 6;
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

void BrowserUseCaseView::add_sequence_diagram() {
  (new BrowserSeqDiagram(child_random_name("Sequence Diagram"), this))
    ->select_in_browser();
}

void BrowserUseCaseView::add_collaboration_diagram() {
  (new BrowserColDiagram(child_random_name("Collaboration Diagram"), this))
    ->select_in_browser();
}

void BrowserUseCaseView::add_object_diagram() {
  (new BrowserObjectDiagram(child_random_name("Object Diagram"), this))
    ->select_in_browser();
}

void BrowserUseCaseView::add_use_case_diagram() {
  (new BrowserUseCaseDiagram(child_random_name("Use Case Diagram"), this))
    ->select_in_browser();
}

void BrowserUseCaseView::add_use_cases_list() {
  (new BrowserUseCaseView(child_random_name("Use Case View"), this))
    ->select_in_browser();
}

UmlCode BrowserUseCaseView::get_type() const {
  return UmlUseCaseView;
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
  case UmlClass:	// instance
    c = classinstance_color;
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
  default:
    // UmlObjectDiagram
    v = objectdiagram_settings.shadow;
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
	    (BrowserClass::add_class(this, args))->write_id(com);
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
      UmlDrag::canDecode(e, UmlUseCase) ||
      UmlDrag::canDecode(e, UmlUseCaseDiagram) ||
      UmlDrag::canDecode(e, UmlSeqDiagram) ||
      UmlDrag::canDecode(e, UmlColDiagram) ||
      UmlDrag::canDecode(e, UmlObjectDiagram) ||
      UmlDrag::canDecode(e, UmlUseCaseView))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserUseCaseView::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, UmlClass) ||
      UmlDrag::canDecode(e, UmlUseCase) ||
      UmlDrag::canDecode(e, UmlUseCaseDiagram) ||
      UmlDrag::canDecode(e, UmlSeqDiagram) ||
      UmlDrag::canDecode(e, UmlColDiagram) ||
      UmlDrag::canDecode(e, UmlObjectDiagram) ||
      UmlDrag::canDecode(e, UmlUseCaseView))
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
    case UmlUseCase:
    case UmlUseCaseDiagram:
    case UmlSeqDiagram:
    case UmlColDiagram:
    case UmlObjectDiagram:
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

void BrowserUseCaseView::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserUseCaseView::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if ((((bn = UmlDrag::decode(e, UmlClass)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlUseCase)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlSeqDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlColDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlObjectDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlUseCaseDiagram)) != 0) ||
       ((bn = UmlDrag::decode(e, UmlUseCaseView)) != 0)) &&
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
    
    bool nl = FALSE;
    
    save_color(st, "duration_color", duration_color, nl);
    save_color(st, "continuation_color", continuation_color, nl);
    save_color(st, "note_color", note_color, nl);
    save_color(st, "usecase_color", usecase_color, nl);
    save_color(st, "package_color", package_color, nl);
    save_color(st, "fragment_color", fragment_color, nl);
    save_color(st, "subject_color", subject_color, nl);
    save_color(st, "classinstance_color", classinstance_color, nl);
    
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
    BrowserUseCaseView * result = all[id];
    
    if (result != 0) {
      msg_critical("Error", "reload not yet implemented");
      
      throw 0;
    }
    else {
      result = new BrowserUseCaseView(read_string(st), parent, id);
    
      result->is_read_only = !in_import() && read_only_file() || 
	(user_id() != 0) && result->is_api_base();
      
      k = read_keyword(st);
      
      result->def->read(st, k);						// updates k
      result->usecasediagram_settings.read(st, k);			// updates k
      result->sequencediagram_settings.read(st, k);			// updates k
      result->collaborationdiagram_settings.read(st, k);		// updates k
      if (read_file_format() >= 25)
	result->objectdiagram_settings.read(st, k);		// updates k
      read_color(st, "duration_color", result->duration_color, k);	// updates k
      read_color(st, "continuation_color", result->continuation_color, k);	// updates k
      read_color(st, "note_color", result->note_color, k);		// updates k
      read_color(st, "usecase_color", result->usecase_color, k);	// updates k
      read_color(st, "package_color", result->package_color, k);	// updates k
      read_color(st, "fragment_color", result->fragment_color, k);	// updates k
      read_color(st, "subject_color", result->subject_color, k);	// updates k
      read_color(st, "classinstance_color", result->classinstance_color, k);	// updates k
      result->BrowserNode::read(st, k);	// updates k
      
      if (strcmp(k, "end")) {
	while (BrowserUseCaseDiagram::read(st, k, result) ||
	       BrowserUseCase::read(st, k, result) ||
	       BrowserUseCaseView::read(st, k, result, recursive) ||
	       BrowserClass::read(st, k, result) ||
	       BrowserSeqDiagram::read(st, k, result) ||
	       BrowserColDiagram::read(st, k, result) ||
	       BrowserObjectDiagram::read(st, k, result))
	  k = read_keyword(st);
	
	if (strcmp(k, "end"))
	  wrong_keyword(k, "end");
      }
      
      return result;
    }
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
    // class, seq diag, col diag managed in class view
    r = BrowserUseCase::get_it(k, id);
  
  return r;
}
