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

#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h> 

#include "StateDialog.h"
#include "StateData.h"
#include "BrowserState.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "strutil.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"

QSize StateDialog::previous_size;

StateDialog::StateDialog(StateData * d)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), state(d) {
  d->browser_node->edit_start();
  
  if (d->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  setCaption("State dialog");
  visit = !hasOkButton();  

  BrowserNode * bn = state->browser_node;
  QGrid * grid;
  
  //
  // general tab
  //
  
  grid = new QGrid(2, this);
  umltab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("name : ", grid);
  edname = new LineEdit(bn->get_name(), grid);
  edname->setReadOnly(visit);
    
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(state->get_stereotype()));
  if (!visit) {
    edstereotype->insertStringList(BrowserState::default_stereotypes());
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
  
  QVBox * vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(bn->get_comment());
  
  addTab(grid, "Uml");

  // UML / OCL
  init_tab(ocltab, uml, state->uml, "Ocl", SLOT(edit_uml_entry()),
	   SLOT(edit_uml_exit()), SLOT(edit_uml_activity()));

  // CPP
  init_tab(cpptab, cpp, state->cpp, "C++", SLOT(edit_cpp_entry()),
	   SLOT(edit_cpp_exit()), SLOT(edit_cpp_activity()));

  // Java
  init_tab(javatab, java, state->java, "Java", SLOT(edit_java_entry()),
	   SLOT(edit_java_exit()), SLOT(edit_java_activity()));
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(bn, grid, visit);
  addTab(grid, "Properties");
  
  //
    
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(change_tabs(QWidget *)));
}

StateDialog::~StateDialog() {
  state->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void StateDialog::change_tabs(QWidget * w) {
  if (!visit) {
    if (w == umltab)
      edname->setFocus();
    else if (w == ocltab)
      uml.edentry->setFocus();
    else if (w == cpptab)
      cpp.edentry->setFocus();
    else if (w == javatab)
      java.edentry->setFocus();
  }
}

void StateDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void StateDialog::init_tab(QWidget *& tab, StDialog & d, StateBehavior & st,
			   const char * lbl, const char * sl_enbeh,
			   const char * sl_exbeh, const char * sl_beh) {
  QGrid * grid = new QGrid(2, this);
  QVBox * vtab;

  tab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);
  
  vtab = new QVBox(grid);
  new QLabel("Entry\nbehavior : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()), this, sl_enbeh);
  d.edentry = new MultiLineEdit(grid);

  QFont font = d.edentry->font();

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  d.edentry->setFont(font);
  d.edentry->setText(st.on_entry);
  if (visit)
    d.edentry->setReadOnly(TRUE);
  
  vtab = new QVBox(grid);
  new QLabel("Exit\nbehavior : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()), this, sl_exbeh);
  d.edexit = new MultiLineEdit(grid);
  d.edexit->setFont(font);
  d.edexit->setText(st.on_exit);
  if (visit)
    d.edexit->setReadOnly(TRUE);
  
  vtab = new QVBox(grid);
  new QLabel("Do\nbehavior : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()), this, sl_beh);
  d.edactivity = new MultiLineEdit(grid);
  d.edactivity->setFont(font);
  d.edactivity->setText(st.do_activity);
  if (visit)
    d.edactivity->setReadOnly(TRUE);
  
  addTab(grid, lbl);
}

void StateDialog::edit_description() {
  edit(comment->text(), "state_description", state, TxtEdit,
       this, (post_edit) post_edit_description, edits);
}

void StateDialog::post_edit_description(StateDialog * d, QString s)
{
  d->comment->setText(s);
}

#define DEF_EDIT(attr, lang, Lang) \
void StateDialog::edit_##lang##_##attr() {	\
  edit(lang.ed##attr->text(), "state_"#lang"_"#attr, state, Lang,  \
       this, (post_edit) post_edit_##lang##_##attr, edits);	\
} \
  \
void StateDialog::post_edit_##lang##_##attr(StateDialog * d, QString s) \
{ \
  d->lang.ed##attr->setText(s); \
}

DEF_EDIT(entry, uml, TxtEdit)
DEF_EDIT(entry, cpp, CppEdit)
DEF_EDIT(entry, java, JavaEdit)

DEF_EDIT(exit, uml, TxtEdit)
DEF_EDIT(exit, cpp, CppEdit)
DEF_EDIT(exit, java, JavaEdit)

DEF_EDIT(activity, uml, TxtEdit)
DEF_EDIT(activity, cpp, CppEdit)
DEF_EDIT(activity, java, JavaEdit)

void StateDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = state->browser_node;
  QString s;
  
  s = edname->text().stripWhiteSpace();
  if ((s != bn->get_name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlState,
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical("Error", s + "\n\nillegal name or already used");
  else {  
    bn->set_name(s);
    state->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    
    uml.accept(state->uml);  
    cpp.accept(state->cpp);  
    java.accept(state->java);    
    
    bn->set_comment(comment->text());
    UmlWindow::update_comment_if_needed(bn);
    
    kvtable->update(bn);
    
    bn->modified();
    bn->package_modified();
    state->modified();
    
    QTabDialog::accept();
  }
}

void StDialog::accept(StateBehavior & st) {
  st.on_entry = edentry->text();
  st.on_exit = edexit->text();
  st.do_activity = edactivity->text();
}
