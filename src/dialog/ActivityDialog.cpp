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





#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h> 
#include <qcheckbox.h> 
#include <qbuttongroup.h> 

#include "ActivityDialog.h"
#include "ActivityData.h"
#include "BrowserActivity.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "strutil.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "GenerationSettings.h"
#include "ProfiledStereotypes.h"

QSize ActivityDialog::previous_size;

ActivityDialog::ActivityDialog(ActivityData * d)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), activity(d) {
  d->browser_node->edit_start();
  
  if (d->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  setCaption("Activity dialog");
  visit = !hasOkButton();  

  BrowserNode * bn = activity->browser_node;
  QGrid * grid;
  
  //
  // general tab
  //
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("name : ", grid);
  edname = new LineEdit(bn->get_name(), grid);
  edname->setReadOnly(visit);
    
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(activity->get_stereotype()));
  if (!visit) {
    edstereotype->insertStringList(BrowserActivity::default_stereotypes());
    edstereotype->insertStringList(ProfiledStereotypes::defaults(UmlActivity));
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
  
  new QLabel(grid);
  QButtonGroup * bg = 
    new QButtonGroup(2, Qt::Horizontal, QString::null, grid);
  
  readonly_cb = new QCheckBox("read only", bg);
  if (activity->read_only)
    readonly_cb->setChecked(TRUE);
  readonly_cb->setDisabled(visit);
  
  singlexec_cb = new QCheckBox("single execution", bg);
  if (activity->single_execution)
    singlexec_cb->setChecked(TRUE);
  singlexec_cb->setDisabled(visit);
  
  QVBox * vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(bn->get_comment());
  //comment->setFont(font);
  
  addTab(grid, "Uml");

  // UML / OCL
  init_tab(uml, activity->uml_condition, "Ocl", TRUE);

  // CPP
  init_tab(cpp, activity->cpp_condition, "C++",
	   GenerationSettings::cpp_get_default_defs());

  // Java
  init_tab(java, activity->java_condition, "Java",
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

void ActivityDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

ActivityDialog::~ActivityDialog() {
  activity->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void ActivityDialog::change_tabs(QWidget *) {
  if (!visit)
    edname->setFocus();
}

void ActivityDialog::init_tab(CondDialog & d, InfoData & cd,
			      const char * lbl, bool enabled) {
  QGrid * grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("Pre\ncondition : ", grid);
  d.edpre = new MultiLineEdit(grid);
  d.edpre->setText(cd.first);
  if (visit)
    d.edpre->setReadOnly(TRUE);
  
  new QLabel("Post\ncondition : ", grid);
  d.edpost = new MultiLineEdit(grid);
  d.edpost->setText(cd.second);
  if (visit)
    d.edpost->setReadOnly(TRUE);
  
  addTab(grid, lbl);
  
  if (! enabled)
    removePage(grid);
}

void ActivityDialog::edit_description() {
  edit(comment->text(), "activity_description", activity, TxtEdit,
       this, (post_edit) post_edit_description, edits);
}

void ActivityDialog::post_edit_description(ActivityDialog * d, QString s)
{
  d->comment->setText(s);
}

void ActivityDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = activity->browser_node;
  QString s;
  
  s = edname->text().stripWhiteSpace();
  if ((s != bn->get_name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlActivity,
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical("Error", s + "\n\nillegal name or already used");
  else {  
    bn->set_name(s);

    bool newst = activity->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    
    activity->read_only = readonly_cb->isChecked();
    activity->single_execution = singlexec_cb->isChecked();
    
    uml.accept(activity->uml_condition);  
    cpp.accept(activity->cpp_condition);  
    java.accept(activity->java_condition);    
    
    bn->set_comment(comment->text());
    UmlWindow::update_comment_if_needed(bn);
    
    kvtable->update(bn);
    
    bn->modified();
    bn->package_modified();
    activity->modified();
    
    ProfiledStereotypes::modified(bn, newst);
    QTabDialog::accept();
  }
}

void CondDialog::accept(InfoData & cond) {
  cond.first = edpre->text().stripWhiteSpace();
  cond.second = edpost->text().stripWhiteSpace();
}
