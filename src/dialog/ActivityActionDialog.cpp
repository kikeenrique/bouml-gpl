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

#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qpushbutton.h> 
#include <qpopupmenu.h> 
#include <qcursor.h> 

#include "ActivityActionDialog.h"
#include "BrowserActivityAction.h"
#include "BrowserClass.h"
#include "BrowserOperation.h"
#include "OperationData.h"
#include "BrowserState.h"
#include "BrowserActivity.h"
#include "KeyValueTable.h"
#include "UmlPixmap.h"
#include "DialogUtil.h"
#include "strutil.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "UmlWindow.h"
#include "BrowserView.h"

QSize ActivityActionDialog::previous_size;

static QString pretty_kind(UmlActionKind k)
{
  QString s = stringify(k);
  int index = 0;

  while ((index = s.find('_', index)) != -1)
    s.replace(index, 1, " ");

  return s;
}

static QGrid * mkgrid(QTabDialog * d, const char * name = 0)
{
  QGrid * grid = new QGrid(2, d, name);

  grid->setMargin(5);
  grid->setSpacing(5);

  return grid;
}

ActivityActionDialog::ActivityActionDialog(ActivityActionData * a)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), act(a) {
  a->browser_node->edit_start();
  
  if (a->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  setCaption("Activity Action dialog");
  visit = !hasOkButton();  
  
  current_type = act->get_action_kind();

  BrowserActivityAction * action = 
    (BrowserActivityAction *) a->get_browser_node();
  QGrid * grid;
  
  //
  // general tab
  //
  
  grid = mkgrid(this);
    
  new QLabel("name : ", grid);
  edname = new LineEdit(action->get_name(), grid);
  edname->setReadOnly(visit);
    
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(action->get_stereotype()));
  if (!visit) {
    edstereotype->insertStringList(BrowserActivityAction::default_stereotypes());
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);

  new QLabel("kind : ", grid);
  edtype = new QComboBox(FALSE, grid);
      
  if (visit) {
    // action kind cannot be changed
    edtype->insertItem(pretty_kind(current_type));
  }
  else {
    int index;

    for (index = UmlOpaqueAction; index <= UmlValueSpecificationAction; index += 1) {
      edtype->insertItem(pretty_kind((UmlActionKind) index));
      if (index == (int) current_type)
	edtype->setCurrentItem(index);
    }
  
    connect(edtype, SIGNAL(activated(int)), this, SLOT(edTypeActivated(int)));
  }

  QVBox * vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(action->get_comment());
 
  addTab(grid, "Uml");
  
  //
  // action kind specific
  //
  
  BrowserClass::instances(classes);

  BrowserNode * c;

  for (c = classes.first(); c != 0; c = classes.next()) {
    QListViewItem * child;
    
    for (child = c->firstChild(); child; child = child->nextSibling()) {
      if (!((BrowserNode *) child)->deletedp()) {
	switch (((BrowserNode *) child)->get_type()) {
	case UmlAssociation:
	case UmlDirectionalAssociation:
	case UmlAggregation:
	case UmlAggregationByValue:
	case UmlDirectionalAggregation:
	case UmlDirectionalAggregationByValue:
	case UmlAttribute:
	  classes_with_var.append(c);
	  break;
	case UmlOperation:
	  classes_with_oper.append(c);
	  break;
	default:
	  break;
	}
      }
    }
  }

  classes_with_oper.full_names(class_with_oper_names);
  classes_with_var.full_names(class_with_var_names);

  BrowserState::instances(behaviors, FALSE);
  BrowserActivity::instances(behaviors, TRUE);
  behaviors.full_names(behavior_names);

#define K(k) (current_type == Uml##k) ? ((k *) act->action) : 0

  opaque.init(this, act, K(OpaqueAction), edits, visit);
  acceptevent.init(this, act, K(AcceptEventAction), visit);
  sendobject.init(this, act, K(SendObjectAction), visit);
  unmarshall.init(this, act, K(UnmarshallAction), visit);
  valuespecification.init(this, act, K(ValueSpecificationAction), edits, visit);
  sendsignal.init(this, act, K(SendSignalAction), visit);
  broadcastsignal.init(this, act, K(BroadcastSignalAction), visit);
  readvariablevalue.init(this, act, K(ReadVariableValueAction),
			 classes_with_var, class_with_var_names, visit);
  clearvariablevalue.init(this, act, K(ClearVariableValueAction),
			  classes_with_var, class_with_var_names, visit);
  writevariablevalue.init(this, act, K(WriteVariableValueAction),
			  classes_with_var, class_with_var_names, visit);
  addvariablevalue.init(this, act, K(AddVariableValueAction),
			classes_with_var, class_with_var_names, visit);
  removevariablevalue.init(this, act, K(RemoveVariableValueAction),
			   classes_with_var, class_with_var_names, visit);
  calloperation.init(this, act, K(CallOperationAction),
		     classes_with_oper, class_with_oper_names, visit);
  callbehavior.init(this, act, K(CallBehaviorAction),
		    behaviors, behavior_names, visit);

#undef K

  //
  // USER : list key - value
  //
  
  grid = mkgrid(this);
  
  kvtable = new KeyValuesTable(action, grid, visit);
  addTab(grid, "Properties");
}

void ActivityActionDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

ActivityActionDialog::~ActivityActionDialog() {
  act->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void ActivityActionDialog::edit_description() {
  edit(comment->text(), "activity_action_description", act, TxtEdit,
       this, (post_edit) post_edit_description, edits);
}

void ActivityActionDialog::post_edit_description(ActivityActionDialog * d,
						 QString s)
{
  d->comment->setText(s);
}

AnyActionDialog & ActivityActionDialog::get_dialog(int k) {
  switch (k) {
  case UmlOpaqueAction:
    return opaque;
  case UmlAcceptEventAction:
    return acceptevent;
  case UmlReadVariableValueAction:
    return readvariablevalue;
  case UmlClearVariableValueAction:
    return clearvariablevalue;
  case UmlWriteVariableValueAction:
    return writevariablevalue;
  case UmlAddVariableValueAction:
    return addvariablevalue;
  case UmlRemoveVariableValueAction:
    return removevariablevalue;
  case UmlCallBehaviorAction:
    return callbehavior;
  case UmlCallOperationAction:
    return calloperation;
  case UmlSendObjectAction:
    return sendobject;
  case UmlSendSignalAction:
    return sendsignal;
  case UmlBroadcastSignalAction:
    return broadcastsignal;
  case UmlUnmarshallAction:
    return unmarshall;
  default: // UmlValueSpecificationAction
    return valuespecification;
  }
}

void ActivityActionDialog::edTypeActivated(int k) {
  get_dialog(k).goes(get_dialog(current_type));
  current_type = (UmlActionKind) k;
}

void ActivityActionDialog::accept() {
  if (!check_edits(edits))
    return;

  BrowserActivityAction * bn = (BrowserActivityAction *) act->browser_node;
  QString s;
  
  s = edname->text().stripWhiteSpace();
  if ((s != bn->get_name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlActivityAction,
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical("Error", s + "\n\nillegal name or already used");
  else {  
    act->undepend();
    bn->set_name(s);
    act->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    
    QString ocl_pre;
    QString ocl_post;
    QString cpp_pre;
    QString cpp_post;
    QString java_pre;
    QString java_post;
    bool changed = (act->get_action_kind() != current_type);

    if (changed)
      act->set_action_kind(current_type);

    switch (current_type) {
#define CASE(t, v)  \
    case Uml##t :   \
      v.get_cond(ocl_pre, ocl_post, cpp_pre, cpp_post, java_pre, java_post);  \
      if (v.update((t *) act->action) || changed) { \
	/* pins changed */	\
	bn->set_pins(); \
      }	\
      break

    CASE(OpaqueAction, opaque);
    CASE(AcceptEventAction, acceptevent);
    CASE(SendObjectAction, sendobject);
    CASE(UnmarshallAction, unmarshall);
    CASE(ValueSpecificationAction, valuespecification);
    CASE(SendSignalAction, sendsignal);
    CASE(BroadcastSignalAction, broadcastsignal);
    CASE(ReadVariableValueAction, readvariablevalue);
    CASE(ClearVariableValueAction, clearvariablevalue);
    CASE(WriteVariableValueAction, writevariablevalue);
    CASE(AddVariableValueAction, addvariablevalue);
    CASE(RemoveVariableValueAction, removevariablevalue);
    CASE(CallOperationAction, calloperation);
    CASE(CallBehaviorAction, callbehavior);

#undef CASE

    default:
      break;
    }

    act->uml_condition.first = ocl_pre;
    act->uml_condition.second = ocl_post;
    act->cpp_condition.first = cpp_pre;
    act->cpp_condition.second = cpp_post;
    act->java_condition.first = java_pre;
    act->java_condition.second = java_post;

    act->update_depend();

    bn->set_comment(comment->text());
    UmlWindow::set_commented(bn);
    
    kvtable->update(bn);
    
    bn->modified();   // call action->modified()
    bn->package_modified();
    
    QTabDialog::accept();
  }
}

// ActionCondDialog

void ActionCondDialog::init(QGrid * grid, ActivityActionData * d,
			    DrawingLanguage lang, bool visit) {
  new QLabel("Pre\ncondition : ", grid);
  edpre = new MultiLineEdit(grid);
  edpre->setText(d->get_precond(lang));

  new QLabel("Post\ncondition : ", grid);
  edpost = new MultiLineEdit(grid);
  edpost->setText(d->get_postcond(lang));

  if (visit) {
    edpre->setReadOnly(TRUE);
    edpost->setReadOnly(TRUE);
  }
}

void ActionCondDialog::get(QString & pre, QString & post) const {
  pre = edpre->text().stripWhiteSpace();
  post = edpost->text().stripWhiteSpace();
}

void ActionCondDialog::set(QString pre, QString post) {
  edpre->setText(pre);
  edpost->setText(post);
}


// AnyActionDialog

AnyActionDialog::AnyActionDialog()
    : ocl_grid(0), cpp_grid(0), java_grid(0) {
}

void AnyActionDialog::init(QTabDialog * t, ActivityActionData * act,
			   void * d, bool visit) {
  // just add the pre/post condition
  td = t;

  // ocl

  ocl_grid = mkgrid(t);
  ocl_cond.init(ocl_grid, act, UmlView, visit);

  if (d != 0)
    ocl_cond.set(act->get_precond(UmlView), act->get_postcond(UmlView));

  // note : must add then remove rather than doing nothing
  // else some sub widget will be wrong placed
  t->addTab(ocl_grid, "Ocl");

  if (d == 0)
    t->removePage(ocl_grid);

  // cpp & java
  init_cpp(t, act, d, visit);
  init_java(t, act, d, visit);
}

void AnyActionDialog::init_cpp(QTabDialog * t, ActivityActionData * act,
			       void * d, bool visit) {
  // cpp

  cpp_grid = mkgrid(t);
  cpp_cond.init(cpp_grid, act, CppView, visit);

  if (d != 0)
    cpp_cond.set(act->get_precond(CppView), act->get_postcond(CppView));

  // note : must add then remove rather than doing nothing
  // else some sub widget will be wrong placed
  t->addTab(cpp_grid, "C++");

  if (d == 0)
    t->removePage(cpp_grid);
}

void AnyActionDialog::init_java(QTabDialog * t, ActivityActionData * act,
				void * d, bool visit) {
  // java

  java_grid = mkgrid(t);
  java_cond.init(java_grid, act, JavaView, visit);

  if (d != 0)
    java_cond.set(act->get_precond(JavaView), act->get_postcond(JavaView));

  // note : must add then remove rather than doing nothing
  // else some sub widget will be wrong placed
  t->addTab(java_grid, "Java");

  if (d == 0)
    t->removePage(java_grid);
}

bool AnyActionDialog::update(void *) {
  // do nothing
  return FALSE;
}

void AnyActionDialog::goes(AnyActionDialog & previous) {
  QString pre;
  QString post;

  previous.ocl_cond.get(pre, post);
  ocl_cond.set(pre, post);
  td->insertTab(ocl_grid, ocl_grid->name("Ocl"), 1);
  td->removePage(previous.ocl_grid);

  previous.cpp_cond.get(pre, post);
  cpp_cond.set(pre, post);
  td->insertTab(cpp_grid, cpp_grid->name("C++"), 2);
  td->removePage(previous.cpp_grid);

  previous.java_cond.get(pre, post);
  java_cond.set(pre, post);
  td->insertTab(java_grid, java_grid->name("Java"), 3);
  td->removePage(previous.java_grid);
}

void AnyActionDialog::get_cond(QString & ocl_pre, QString & ocl_post, 
			       QString & cpp_pre, QString & cpp_post, 
			       QString & java_pre, QString & java_post) const {
  ocl_cond.get(ocl_pre, ocl_post);
  cpp_cond.get(cpp_pre, cpp_post);
  java_cond.get(java_pre, java_post);
}

// opaque

void OpaqueDialog::init(QTabDialog * t, ActivityActionData * act,
			OpaqueAction * d, QList<BodyDialog> & e, bool visit) {
  edits = &e;
  td = t;

  // ocl

  ocl_grid = mkgrid(t);

  QVBox * vtab = new QVBox(ocl_grid);

  new QLabel("Behavior : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_ocl()));
  ocl_beh = new MultiLineEdit(ocl_grid);

  if (visit)
    ocl_beh->setReadOnly(TRUE);

  ocl_cond.init(ocl_grid, act, UmlView, visit);
  
  if (d != 0) {
    ocl_cond.set(act->get_precond(UmlView), act->get_postcond(UmlView));
    ocl_beh->setText(d->uml_behavior);
  }
  
  t->addTab(ocl_grid, "Ocl");

  if (d == 0)
    t->removePage(ocl_grid);

  // cpp

  cpp_grid = mkgrid(t);

  vtab = new QVBox(cpp_grid);
  new QLabel("Behavior : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_cpp()));
  cpp_beh = new MultiLineEdit(cpp_grid);

  if (visit)
    cpp_beh->setReadOnly(TRUE);

  cpp_cond.init(cpp_grid, act, CppView, visit);
  
  if (d != 0) {
    cpp_cond.set(act->get_precond(CppView), act->get_postcond(CppView));
    cpp_beh->setText(d->cpp_behavior);
  }

  t->addTab(cpp_grid, "C++");

  if (d == 0)
    t->removePage(cpp_grid);

  // java

  java_grid = mkgrid(t);

  vtab = new QVBox(java_grid);
  new QLabel("Behavior : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_java()));
  java_beh = new MultiLineEdit(java_grid);

  if (visit)
    java_beh->setReadOnly(TRUE);

  java_cond.init(java_grid, act, JavaView, visit);
  
  if (d != 0) {
    java_cond.set(act->get_precond(JavaView), act->get_postcond(JavaView));
    java_beh->setText(d->java_behavior);
  }

  t->addTab(java_grid, "Java");

  if (d == 0)
    t->removePage(java_grid);
}

bool OpaqueDialog::update(OpaqueAction * a) {
  a->uml_behavior = ocl_beh->text().stripWhiteSpace();
  a->cpp_behavior = cpp_beh->text().stripWhiteSpace();
  a->java_behavior = java_beh->text().stripWhiteSpace();

  return FALSE;
}

void OpaqueDialog::edit_ocl() {
  edit(ocl_beh->text(), "activity_action_ocl_behavior", this, TxtEdit,
       td, (post_edit) post_edit_ocl, *edits);
}

void OpaqueDialog::post_edit_ocl(ActivityActionDialog * d, QString s)
{
  d->opaque_dialog().ocl_beh->setText(s);
}

void OpaqueDialog::edit_cpp() {
  edit(cpp_beh->text(), "activity_action_cpp_behavior", this, TxtEdit,
       td, (post_edit) post_edit_cpp, *edits);
}

void OpaqueDialog::post_edit_cpp(ActivityActionDialog * d, QString s)
{
  d->opaque_dialog().cpp_beh->setText(s);
}

void OpaqueDialog::edit_java() {
  edit(java_beh->text(), "activity_action_java_behavior", this, TxtEdit,
       td, (post_edit) post_edit_java, *edits);
}

void OpaqueDialog::post_edit_java(ActivityActionDialog * d, QString s)
{
  d->opaque_dialog().java_beh->setText(s);
}

// Accept Event

void AcceptEventDialog::init(QTabDialog * t, ActivityActionData * act,
			     AcceptEventAction * d, bool visit) {
  td = t;

  // ocl

  ocl_grid = mkgrid(t, "trigger - Ocl");

  new QLabel("", ocl_grid);
  QButtonGroup * grp = 
    new QButtonGroup(2, QGroupBox::Horizontal, QString::null, ocl_grid);

  unmarshall_cb = new QCheckBox("unmarshall", grp);
  unmarshall_cb->setDisabled(visit);

  timeevent_cb = new QCheckBox("time event", grp);
  timeevent_cb->setDisabled(visit);

  new QLabel("trigger : ", ocl_grid);
  uml_trigger = new LineEdit(ocl_grid);
  uml_trigger->setReadOnly(visit);

  ocl_cond.init(ocl_grid, act, UmlView, visit);
  if (d != 0) {
    ocl_cond.set(act->get_precond(UmlView), act->get_postcond(UmlView));
    unmarshall_cb->setChecked(d->unmarshall);
    timeevent_cb->setChecked(d->timeevent);
    uml_trigger->setText(d->uml_trigger);
  }

  t->addTab(ocl_grid, "trigger - Ocl");

  if (d == 0)
    t->removePage(ocl_grid);

  // cpp

  cpp_grid = mkgrid(t, "trigger - C++");

  new QLabel("trigger : ", cpp_grid);
  cpp_trigger = new LineEdit(cpp_grid);

  cpp_cond.init(cpp_grid, act, CppView, visit);

  if (d != 0) {
    cpp_cond.set(act->get_precond(CppView), act->get_postcond(CppView));
    cpp_trigger->setText(d->cpp_trigger);
  }

  t->addTab(cpp_grid, "trigger - C++");

  if (d == 0)
    t->removePage(cpp_grid);

  // java

  java_grid = mkgrid(t, "trigger - Java");

  new QLabel("trigger : ", java_grid);
  java_trigger = new LineEdit(java_grid);

  java_cond.init(java_grid, act, JavaView, visit);

  if (d != 0) {
    java_cond.set(act->get_precond(JavaView), act->get_postcond(JavaView));
    java_trigger->setText(d->java_trigger);
  }

  t->addTab(java_grid, "trigger - Java");

  if (d == 0)
    t->removePage(java_grid);
}

bool AcceptEventDialog::update(AcceptEventAction * a) {
  a->unmarshall = unmarshall_cb->isChecked();
  a->timeevent = timeevent_cb->isChecked();
  a->uml_trigger = uml_trigger->text().stripWhiteSpace();
  a->cpp_trigger = cpp_trigger->text().stripWhiteSpace();
  a->java_trigger = java_trigger->text().stripWhiteSpace();

  return FALSE;
}

// value specification

void ValueSpecificationDialog::init(QTabDialog * t, ActivityActionData * act,
				    ValueSpecificationAction * d,
				    QList<BodyDialog> & e, bool visit) {
  edits = &e;
  td = t;

  // ocl

  ocl_grid = mkgrid(t, "value - Ocl");

  QVBox * vtab = new QVBox(ocl_grid);

  new QLabel("Value : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_ocl()));
  ocl_val = new MultiLineEdit(ocl_grid);

  if (visit)
    ocl_val->setReadOnly(TRUE);

  ocl_cond.init(ocl_grid, act, UmlView, visit);
  
  if (d != 0) {
    ocl_cond.set(act->get_precond(UmlView), act->get_postcond(UmlView));
    ocl_val->setText(d->uml_value);
  }
  
  t->addTab(ocl_grid, "value - Ocl");

  if (d == 0)
    t->removePage(ocl_grid);

  // cpp

  cpp_grid = mkgrid(t, "value - C++");

  vtab = new QVBox(cpp_grid);
  new QLabel("Value : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_cpp()));
  cpp_val = new MultiLineEdit(cpp_grid);

  if (visit)
    cpp_val->setReadOnly(TRUE);

  cpp_cond.init(cpp_grid, act, CppView, visit);
  
  if (d != 0) {
    cpp_cond.set(act->get_precond(CppView), act->get_postcond(CppView));
    cpp_val->setText(d->cpp_value);
  }

  t->addTab(cpp_grid, "value - C++");

  if (d == 0)
    t->removePage(cpp_grid);

  // java

  java_grid = mkgrid(t, "value - Java");

  vtab = new QVBox(java_grid);
  new QLabel("Value : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_java()));
  java_val = new MultiLineEdit(java_grid);

  if (visit)
    java_val->setReadOnly(TRUE);

  java_cond.init(java_grid, act, JavaView, visit);
  
  if (d != 0) {
    java_cond.set(act->get_precond(JavaView), act->get_postcond(JavaView));
    java_val->setText(d->java_value);
  }

  t->addTab(java_grid, "value - Java");

  if (d == 0)
    t->removePage(java_grid);
}

bool ValueSpecificationDialog::update(ValueSpecificationAction * a) {
  a->uml_value = ocl_val->text().stripWhiteSpace();
  a->cpp_value = cpp_val->text().stripWhiteSpace();
  a->java_value = java_val->text().stripWhiteSpace();

  return FALSE;
}

void ValueSpecificationDialog::edit_ocl() {
  edit(ocl_val->text(), "activity_action_ocl_value", this, TxtEdit,
       td, (post_edit) post_edit_ocl, *edits);
}

void ValueSpecificationDialog::post_edit_ocl(ActivityActionDialog * d, QString s)
{
  d->valuespecification_dialog().ocl_val->setText(s);
}

void ValueSpecificationDialog::edit_cpp() {
  edit(cpp_val->text(), "activity_action_cpp_value", this, TxtEdit,
       td, (post_edit) post_edit_cpp, *edits);
}

void ValueSpecificationDialog::post_edit_cpp(ActivityActionDialog * d, QString s)
{
  d->valuespecification_dialog().cpp_val->setText(s);
}

void ValueSpecificationDialog::edit_java() {
  edit(java_val->text(), "activity_action_java_value", this, TxtEdit,
       td, (post_edit) post_edit_java, *edits);
}

void ValueSpecificationDialog::post_edit_java(ActivityActionDialog * d, QString s)
{
  d->valuespecification_dialog().java_val->setText(s);
}

// Send signal

void SendSignalDialog::init(QTabDialog * t, ActivityActionData * act,
			    SendSignalAction * d, bool visit) {
  td = t;

  // ocl

  ocl_grid = mkgrid(t, "signal - Ocl");

  new QLabel("signal : ", ocl_grid);
  ocl_signal = new LineEdit(ocl_grid);
  ocl_signal->setReadOnly(visit);

  ocl_cond.init(ocl_grid, act, UmlView, visit);
  if (d != 0) {
    ocl_cond.set(act->get_precond(UmlView), act->get_postcond(UmlView));
    ocl_signal->setText(d->ocl_signal);
  }

  t->addTab(ocl_grid, "signal - Ocl");

  if (d == 0)
    t->removePage(ocl_grid);

  // cpp

  cpp_grid = mkgrid(t, "signal - C++");

  new QLabel("signal : ", cpp_grid);
  cpp_signal = new LineEdit(cpp_grid);

  cpp_cond.init(cpp_grid, act, CppView, visit);

  if (d != 0) {
    cpp_cond.set(act->get_precond(CppView), act->get_postcond(CppView));
    cpp_signal->setText(d->cpp_signal);
  }

  t->addTab(cpp_grid, "signal - C++");

  if (d == 0)
    t->removePage(cpp_grid);

  // java

  java_grid = mkgrid(t, "signal - Java");

  new QLabel("signal : ", java_grid);
  java_signal = new LineEdit(java_grid);

  java_cond.init(java_grid, act, JavaView, visit);

  if (d != 0) {
    java_cond.set(act->get_precond(JavaView), act->get_postcond(JavaView));
    java_signal->setText(d->java_signal);
  }

  t->addTab(java_grid, "signal - Java");

  if (d == 0)
    t->removePage(java_grid);
}

bool SendSignalDialog::update(SendSignalAction * a) {
  a->ocl_signal = ocl_signal->text().stripWhiteSpace();
  a->cpp_signal = cpp_signal->text().stripWhiteSpace();
  a->java_signal = java_signal->text().stripWhiteSpace();

  return FALSE;
}

// access & change variable value working classes

void AccessVariableValueDialog::init(QTabDialog * t, ActivityActionData * act,
				     AccessVariableValueAction * d,
				     BrowserNodeList & cl,
				     QStringList & clstr, bool ro) {
  td = t;
  classes = &cl;
  class_names = &clstr;
  visit = ro;

  // ocl

  if (ocl_grid == 0)
    ocl_grid = mkgrid(t, "variable - Ocl");

  new QLabel("class : ", ocl_grid);
  class_co = new QComboBox(FALSE, ocl_grid);
  
  connect(new SmallPushButton("variable :", ocl_grid), SIGNAL(clicked()),
	  this, SLOT(menu_var()));
  var_co = new QComboBox(FALSE, ocl_grid);
  
  ocl_cond.init(ocl_grid, act, UmlView, visit);

  if (!visit) {
    class_co->insertItem("");
    var_co->insertItem("");
    class_co->setAutoCompletion(TRUE);
    var_co->setAutoCompletion(TRUE);
    
    class_co->insertStringList(*class_names);
    connect(class_co, SIGNAL(activated(int)), this, SLOT(classChanged(int)));
  }

  if ((d != 0) && (d->variable != 0)) {
    if (visit) {
      class_co->insertItem(((BrowserNode *) d->variable->parent())->full_name(TRUE));
      var_co->insertItem(*(d->variable->pixmap(0)), d->variable->get_name());
      class_co->setCurrentItem(0);
      var_co->setCurrentItem(0);
    }
    else
      set(d->variable);

    ocl_cond.set(act->get_precond(UmlView), act->get_postcond(UmlView));
  }

  // note : must add then remove rather than doing nothing
  // else some sub widget will be wrong placed
  t->addTab(ocl_grid, "variable - Ocl");

  if (d == 0)
    t->removePage(ocl_grid);

  // cpp & java
  init_cpp(t, act, d, visit);
  init_java(t, act, d, visit);
}

void AccessVariableValueDialog::classChanged(int) {
  var_co->clear();
  var_co->insertItem("");

  int index = class_names->findIndex(class_co->currentText());

  if (index != -1)
    insert_vars((BrowserClass *) classes->at(index));
}

void AccessVariableValueDialog::insert_vars(BrowserClass * c) {
  // var_co contains "" and optionally a var's name
  vars.clear();
  var_names.clear();

  QListViewItem * child;
  
  for (child = c->firstChild(); child; child = child->nextSibling()) {
    if (!((BrowserNode *) child)->deletedp()) {
      switch (((BrowserNode *) child)->get_type()) {
      case UmlAssociation:
      case UmlDirectionalAssociation:
      case UmlAggregation:
      case UmlAggregationByValue:
      case UmlDirectionalAggregation:
      case UmlDirectionalAggregationByValue:
      case UmlAttribute:
	vars.append((BrowserNode *) child);
	var_names.append(((BrowserNode *) child)->get_name());
	var_co->insertItem(*(((BrowserNode *) child)->pixmap(0)),
			   ((BrowserNode *) child)->get_name());
	break;
      default:
	break;
      }
    }
  }
}

bool AccessVariableValueDialog::update(AccessVariableValueAction * a) {
  int index = var_names.findIndex(var_co->currentText());

  BrowserNode * old = a->variable;

  a->variable = (index != -1) ? vars.at(index) : 0;

  return old != a->variable;
}

void AccessVariableValueDialog::menu_var() {
  QPopupMenu m(0);

  m.insertItem("Choose", -1);
  m.insertSeparator();
  
  int index = var_names.findIndex(var_co->currentText());
  
  if (index != -1)
    m.insertItem("Select in browser", 0);
  
  BrowserNode * bn = 0;
  
  if (! visit) {
    bn = BrowserView::selected_item();
    
    switch (bn->get_type()) {
    case UmlAssociation:
    case UmlDirectionalAssociation:
    case UmlAggregation:
    case UmlAggregationByValue:
    case UmlDirectionalAggregation:
    case UmlDirectionalAggregationByValue:
    case UmlAttribute:
      if (!bn->deletedp()) {
	m.insertItem("Choose variable selected in browser", 1);
	break;
      }
      // no break
    default:
      bn = 0;
    }
  }
  
  if ((index != -1) || (bn != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      vars.at(index)->select_in_browser();
      break;
    case 1:
      var_co->clear();
      var_co->insertItem("");
      set(bn);
    }
  }
}

void AccessVariableValueDialog::set(BrowserNode * bn) {
  BrowserClass * cl = (BrowserClass *) bn->parent();
  
  class_co->setCurrentItem(class_names->findIndex(cl->full_name(TRUE)) + 1);
  insert_vars(cl);
  var_co->setCurrentItem(var_names.findIndex(bn->get_name()) + 1);
}

void ChangeVariableValueDialog::init(QTabDialog * t, ActivityActionData * act,
				     ChangeVariableValueAction * d,
				     const char * flg_name, BrowserNodeList & cl,
				     QStringList & clstr, bool visit) {
  ocl_grid = mkgrid(t, "variable - Ocl");
  
  new QLabel("", ocl_grid);
  QButtonGroup * grp = 
    new QButtonGroup(2, QGroupBox::Horizontal, QString::null, ocl_grid);

  flag_cb = new QCheckBox(flg_name, grp);
  flag_cb->setDisabled(visit);
  
  AccessVariableValueDialog::init(t, act, d, cl, clstr, visit);

  if ((d != 0) && (d->variable != 0))
    flag_cb->setChecked(d->flag);
}

bool ChangeVariableValueDialog::update(ChangeVariableValueAction * a) {
  a->flag = flag_cb->isChecked();
  return AccessVariableValueDialog::update(a);
}

// add variable value

void AddVariableValueDialog::init(QTabDialog * t, ActivityActionData * act,
				  AddVariableValueAction * d, BrowserNodeList & cl,
				  QStringList & clstr, bool visit) {
  ChangeVariableValueDialog::init(t, act, d, "replace all", cl, clstr, visit);
}

// remove variable value

void RemoveVariableValueDialog::init(QTabDialog * t, ActivityActionData * act,
				     RemoveVariableValueAction * d, BrowserNodeList & cl,
				  QStringList & clstr, bool visit) {
  ChangeVariableValueDialog::init(t, act, d, "remove duplicates", cl, clstr, visit);
}

// call operation

void CallOperationDialog::init(QTabDialog * t, ActivityActionData * act,
			       CallOperationAction * d, BrowserNodeList & cl,
			       QStringList & clstr, bool ro) {
  td = t;
  classes = &cl;
  class_names = &clstr;
  visit = ro;

  // ocl

  ocl_grid = mkgrid(t, "operation - Ocl");

  new QLabel("", ocl_grid);
  QButtonGroup * grp = 
    new QButtonGroup(2, QGroupBox::Horizontal, QString::null, ocl_grid);

  synchronous_cb = new QCheckBox("synchronous", grp);
  synchronous_cb->setDisabled(visit);
  
  new QLabel("class : ", ocl_grid);
  class_co = new QComboBox(FALSE, ocl_grid);

  connect(new SmallPushButton("operation :", ocl_grid), SIGNAL(clicked()),
	  this, SLOT(menu_oper()));
  oper_co = new QComboBox(FALSE, ocl_grid);
  
  ocl_cond.init(ocl_grid, act, UmlView, visit);

  if (! visit) {
    class_co->insertItem("");
    oper_co->insertItem("");
    class_co->setAutoCompletion(TRUE);
    oper_co->setAutoCompletion(TRUE);

    class_co->insertStringList(*class_names);
    connect(class_co, SIGNAL(activated(int)), this, SLOT(classChanged(int)));
  }
  
  if ((d != 0) && (d->operation != 0)) {
    synchronous_cb->setChecked(d->synchronous);
    
    if (visit) {
      class_co->insertItem(((BrowserNode *) d->operation->parent())->full_name(TRUE));
      oper_co->insertItem(((OperationData *) d->operation->get_data())->definition(TRUE));
      class_co->setCurrentItem(0);
      oper_co->setCurrentItem(0);
    }
    else
      set(d->operation);
    
    ocl_cond.set(act->get_precond(UmlView), act->get_postcond(UmlView));
  }

  // note : must add then remove rather than doing nothing
  // else some sub widget will be wrong placed
  t->addTab(ocl_grid, "operation - Ocl");

  if (d == 0)
    t->removePage(ocl_grid);

  // cpp & java
  init_cpp(t, act, d, visit);
  init_java(t, act, d, visit);
}

void CallOperationDialog::classChanged(int) {
  oper_co->clear();
  oper_co->insertItem("");

  int index = class_names->findIndex(class_co->currentText());

  if (index != -1)
    insert_opers((BrowserClass *) classes->at(index));
}

void CallOperationDialog::insert_opers(BrowserClass * c) {
  // oper_co contains "" and optionally an oper's name
  opers.clear();
  oper_names.clear();

  QListViewItem * child;
  
  for (child = c->firstChild(); child; child = child->nextSibling()) {
    if (!((BrowserNode *) child)->deletedp() &&
	(((BrowserNode *) child)->get_type() == UmlOperation)) {
      QString s = ((OperationData *) ((BrowserNode *) child)->get_data())->definition(TRUE);
      
      opers.append((BrowserNode *) child);      
      oper_names.append(s);
      oper_co->insertItem(s);
    }
  }
}

bool CallOperationDialog::update(CallOperationAction * a) {
  a->synchronous = synchronous_cb->isChecked();

  int index = oper_names.findIndex(oper_co->currentText());

  BrowserNode * old = a->operation;

  a->operation = (index != -1) ? (BrowserOperation *) opers.at(index) : 0;

  return old != a->operation;
}

void CallOperationDialog::set(BrowserNode * bn) {
  BrowserClass * cl = (BrowserClass *) bn->parent();
  
  class_co->setCurrentItem(class_names->findIndex(cl->full_name(TRUE)) + 1);
  insert_opers(cl);
  oper_co->setCurrentItem(oper_names.findIndex(((OperationData *) bn->get_data())->definition(TRUE)) + 1);
}

void CallOperationDialog::menu_oper() {
  QPopupMenu m(0);

  m.insertItem("Choose", -1);
  m.insertSeparator();
  
  int index = oper_names.findIndex(oper_co->currentText());
  
  if (index != -1)
    m.insertItem("Select in browser", 0);
  
  BrowserNode * bn = 0;
  
  if (! visit) {
    bn = BrowserView::selected_item();
    
    if ((bn->get_type() == UmlOperation) && !bn->deletedp())
      m.insertItem("Choose operation selected in browser", 1);
    else
      bn = 0;
  }
  
  if ((index != -1) || (bn != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      opers.at(index)->select_in_browser();
      break;
    case 1:
      oper_co->clear();
      oper_co->insertItem("");
      set(bn);
    }
  }
}

// call behavior

void CallBehaviorDialog::init(QTabDialog * t, ActivityActionData * act,
			       CallBehaviorAction * d, BrowserNodeList & beh,
			       QStringList & behstr, bool ro) {
  td = t;
  nodes = &beh;
  node_names = &behstr;
  visit = ro;

  // ocl

  ocl_grid = mkgrid(t, "behavior - Ocl");

  new QLabel("", ocl_grid);
  QButtonGroup * grp = 
    new QButtonGroup(2, QGroupBox::Horizontal, QString::null, ocl_grid);

  synchronous_cb = new QCheckBox("synchronous", grp);
  synchronous_cb->setDisabled(visit);
  
  connect(new SmallPushButton("behavior :", ocl_grid), SIGNAL(clicked()),
	  this, SLOT(menu_beh()));
  behavior_co = new QComboBox(FALSE, ocl_grid);
  
  ocl_cond.init(ocl_grid, act, UmlView, visit);

  if (! visit) {
    behavior_co->insertItem("");
    behavior_co->setAutoCompletion(TRUE);

    QListIterator<BrowserNode> iter_node(*nodes);
    QStringList::Iterator iter_str = node_names->begin();
    
    for (; iter_node.current(); ++iter_node, ++iter_str)
      behavior_co->insertItem(*(iter_node.current()->pixmap(0)), *iter_str);
  }
  
  if ((d != 0) && (d->behavior != 0)) {
    synchronous_cb->setChecked(d->synchronous);
    
    if (visit) {
      behavior_co->insertItem(*(d->behavior->pixmap(0)), d->behavior->full_name(TRUE));
      behavior_co->setCurrentItem(0);
    }
    else
      behavior_co->setCurrentItem(node_names->findIndex(d->behavior->full_name(TRUE)) + 1);
      
    ocl_cond.set(act->get_precond(UmlView), act->get_postcond(UmlView));
  }

  if (!visit) {
    behavior_co->setAutoCompletion(TRUE);
  }

  // note : must add then remove rather than doing nothing
  // else some sub widget will be wrong placed
  t->addTab(ocl_grid, "behavior - Ocl");

  if (d == 0)
    t->removePage(ocl_grid);

  // cpp & java
  init_cpp(t, act, d, visit);
  init_java(t, act, d, visit);
}

bool CallBehaviorDialog::update(CallBehaviorAction * a) {
  a->synchronous = synchronous_cb->isChecked();

  int index = node_names->findIndex(behavior_co->currentText());

  BrowserNode * old = a->behavior;

  a->behavior = (index != -1) ? nodes->at(index) : 0;

  return old != a->behavior;
}

void CallBehaviorDialog::menu_beh() {
  QPopupMenu m(0);

  m.insertItem("Choose", -1);
  m.insertSeparator();
  
  int index = node_names->findIndex(behavior_co->currentText());
  
  if (index != -1)
    m.insertItem("Select in browser", 0);
  
  BrowserNode * bn = 0;
  
  if (! visit) {
    bn = BrowserView::selected_item();
    
    switch (bn->get_type()) {
    case UmlState:
    case UmlActivity:
      if (!bn->deletedp()) {
	m.insertItem("Choose behavior selected in browser", 1);
	break;
      }
      // no break
    default:
      bn = 0;
    }
  }
  
  if ((index != -1) || (bn != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      nodes->at(index)->select_in_browser();
      break;
    case 1:
      behavior_co->setCurrentItem(node_names->findIndex(bn->full_name(TRUE)) + 1);
    }
  }
}
