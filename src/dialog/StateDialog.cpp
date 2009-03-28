// *************************************************************************
//
// Copyleft 2004-2009 Bruno PAGES  .
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





#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h> 
#include <qpopupmenu.h> 
#include <qcursor.h> 

#include "StateDialog.h"
#include "StateData.h"
#include "BrowserState.h"
#include "BrowserOperation.h"
#include "BrowserView.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "strutil.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "GenerationSettings.h"
#include "ProfiledStereotypes.h"

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
    edstereotype->insertStringList(ProfiledStereotypes::defaults(UmlState));
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
    
  connect(new SmallPushButton("specification :", grid), SIGNAL(clicked()),
	  this, SLOT(menu_specification()));    
  edspecification = new QComboBox(FALSE, grid);
  edspecification->setSizePolicy(sp);
  if (visit) {
    if (state->get_specification() == 0)
      edspecification->insertItem("");
    else
      edspecification->insertItem(state->get_specification()->full_name(TRUE));
  }
  else {
    edspecification->insertItem("");
    edspecification->setAutoCompletion(TRUE);
    BrowserOperation::instances(opers);
    opers.full_names(list);
    edspecification->insertStringList(list);
    edspecification->setCurrentItem((state->get_specification() == 0)
				    ? 0
				    : opers.findRef(state->get_specification()) + 1);
  }
  
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
	   SLOT(edit_uml_exit()), SLOT(edit_uml_activity()), TRUE);

  // CPP
  init_tab(cpptab, cpp, state->cpp, "C++", SLOT(edit_cpp_entry()),
	   SLOT(edit_cpp_exit()), SLOT(edit_cpp_activity()),
	   GenerationSettings::cpp_get_default_defs());

  // Java
  init_tab(javatab, java, state->java, "Java", SLOT(edit_java_entry()),
	   SLOT(edit_java_exit()), SLOT(edit_java_activity()),
	   GenerationSettings::java_get_default_defs());
  
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

void StateDialog::menu_specification() {
  QPopupMenu m(0);

  m.insertItem("Choose", -1);
  m.insertSeparator();
  
  int index = list.findIndex(edspecification->currentText().stripWhiteSpace());
  
  if (index != -1)
    m.insertItem("Select in browser", 0);
  
  BrowserNode * bn = 0;
  
  if (! visit) {
    bn = BrowserView::selected_item();
    
    if ((bn != 0) && (bn->get_type() == UmlOperation) && !bn->deletedp())
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
      {
	QString s = bn->full_name(TRUE);
	
	if ((index = list.findIndex(s)) == -1) {
	  // new operation, may be created through an other dialog
	  index = 0;
	  QStringList::Iterator iter = list.begin();
	  QStringList::Iterator iter_end = list.end();
	  
	  while ((iter != iter_end) && (*iter < s)) {
	    ++iter;
	    index += 1;
	  }
	  opers.insert((unsigned) index, bn);
	  list.insert(iter, s);
	  edspecification->insertItem(s, index + 1);
	}
      }
      edspecification->setCurrentItem(index + 1);
      break;
    default:
      break;
    }
  }
}

void StateDialog::init_tab(QWidget *& tab, StDialog & d, StateBehavior & st,
			   const char * lbl, const char * sl_enbeh,
			   const char * sl_exbeh, const char * sl_beh,
			   bool enabled) {
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
  
  if (! enabled)
    removePage(grid);
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
  if (!check_edits(edits) || !kvtable->check_unique())
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
    
    bool newst = state->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    int index = list.findIndex(edspecification->currentText().stripWhiteSpace());
    
    state->set_specification((index != -1)
			     ? (BrowserOperation *) opers.at(index)
			     : 0);
      
    uml.accept(state->uml);  
    cpp.accept(state->cpp);  
    java.accept(state->java);    
    
    bn->set_comment(comment->text());
    UmlWindow::update_comment_if_needed(bn);
    
    kvtable->update(bn);
    
    ProfiledStereotypes::modified(bn, newst);
    
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
