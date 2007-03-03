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
#include <qradiobutton.h> 
#include <qpushbutton.h> 
#include <qpopupmenu.h> 
#include <qcursor.h> 

#include "ActivityObjectDialog.h"
#include "ActivityObjectData.h"
#include "BrowserActivityObject.h"
#include "BrowserView.h"
#include "BrowserClass.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "GenerationSettings.h"
#include "strutil.h"
#include "BodyDialog.h"

QSize ActivityObjectDialog::previous_size;

ActivityObjectDialog::ActivityObjectDialog(ActivityObjectData * d, const char * what,
					   QStringList & st)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), data(d) {
  d->browser_node->edit_start();
  
  if (d->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  visit = !hasOkButton();
  setCaption(QString(what) + " dialog");
  
  QGrid * grid;
  QHBox * htab;
  QString s;
    
  // general tab
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("name :", grid);
  edname = new LineEdit(data->name(), grid);
  edname->setReadOnly(visit);

  QFont font = edname->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(data->stereotype));
  if (! visit) {
    edstereotype->insertStringList(st);
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  
  QSizePolicy sp = edstereotype->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
      
  connect(new SmallPushButton("type :", grid), SIGNAL(clicked()),
	  this, SLOT(menu_type()));
  
  edtype = new QComboBox(!visit, grid);
  edtype->insertItem(data->get_type().get_full_type());
  if (!visit) {
    BrowserClass::instances(nodes);
    nodes.full_names(list);
    
    edtype->insertStringList(GenerationSettings::basic_types());
    offset = edtype->count();
    edtype->insertStringList(list);
    edtype->setAutoCompletion(TRUE);
  }
  edtype->setCurrentItem(0);
  edtype->setSizePolicy(sp);
  
  new QLabel("multiplicity : ", grid);
  htab = new QHBox(grid);
  edmultiplicity = new QComboBox(!visit, htab);
  edmultiplicity->setSizePolicy(sp);
  edmultiplicity->insertItem(data->get_multiplicity());
  if (!visit) {
    edmultiplicity->insertItem("1");
    edmultiplicity->insertItem("0..1");
    edmultiplicity->insertItem("*");
    edmultiplicity->insertItem("1..*");
  }
  
  new QLabel("   ordering : ", htab);
  edordering = new QComboBox(FALSE, htab);
  
  UmlOrderingKind o = data->get_ordering();
  
  edordering->insertItem(stringify(o));
  if (!visit) {
    if (o != UmlUnordered)
      edordering->insertItem(stringify(UmlUnordered));
    if (o != UmlOrdered)
      edordering->insertItem(stringify(UmlOrdered));
    if (o != UmlLifo)
      edordering->insertItem(stringify(UmlLifo));
    if (o != UmlFifo)
      edordering->insertItem(stringify(UmlFifo));
  }

  new QLabel("in state : ", grid);
  htab = new QHBox(grid);
  edin_state = new LineEdit(data->in_state, htab);
  edin_state->setReadOnly(visit);
       
  new QLabel("  ", htab);  
  is_control_cb = new QCheckBox("is_control", htab);
  if (data->is_control)
    is_control_cb->setChecked(TRUE);
  is_control_cb->setDisabled(visit);
      
  QVBox * vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (! visit) {
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  }
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(data->browser_node->get_comment());
  comment->setFont(font);
  
  addTab(grid, "Uml");
  
  // UML / OCL
  init_tab(eduml_selection, data->uml_selection, "Ocl",
	   SLOT(edit_uml_selection()));

  // C++
  init_tab(edcpp_selection, data->cpp_selection, "C++",
	   SLOT(edit_cpp_selection()));

  // Java
  init_tab(edjava_selection, data->java_selection, "Java",
	   SLOT(edit_java_selection()));
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(data->browser_node, grid, visit);
  addTab(grid, "Properties");
}

void ActivityObjectDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

ActivityObjectDialog::~ActivityObjectDialog() {
  data->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void ActivityObjectDialog::init_tab(MultiLineEdit *& ed, const char * v,
				    const char * lbl, const char * sl) {
  bool visit = !hasOkButton();
  QGrid * grid = new QGrid(2, this);

  grid->setMargin(5);
  grid->setSpacing(5);
  
  QVBox * vtab = new QVBox(grid);
  
  new QLabel("selection : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()), this, sl);
  ed = new MultiLineEdit(grid);

  QFont font = ed->font();

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  ed->setFont(font);
  ed->setText(v);
  if (visit)
    ed->setReadOnly(TRUE);
  
  addTab(grid, lbl);
}

void ActivityObjectDialog::menu_type() {
  QPopupMenu m(0);

  m.insertItem("Choose", -1);
  m.insertSeparator();
  
  int index = list.findIndex(edtype->currentText().stripWhiteSpace());
  
  if (index != -1)
    m.insertItem("Select in browser", 0);
  
  BrowserNode * bn = 0;
  
  if (! visit) {
    bn = BrowserView::selected_item();
    
    if ((bn->get_type() == UmlClass) && !bn->deletedp())
      m.insertItem("Choose class selected in browser", 1);
    else
      bn = 0;
  }
  
  if ((index != -1) || (bn != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      nodes.at(index)->select_in_browser();
      break;
    case 1:
      edtype->setCurrentItem(list.findIndex(bn->full_name(TRUE)) + offset);
    }
  }
}

void ActivityObjectDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       data, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void ActivityObjectDialog::post_edit_description(ActivityObjectDialog * d, QString s)
{
  d->comment->setText(s);
}

#define DEF_EDIT(lang, Lang) \
void ActivityObjectDialog::edit_##lang##_selection() {	\
  edit(ed##lang##_selection->text(), "object_"#lang"_selection", data, Lang,  \
       this, (post_edit) post_edit_##lang##_##selection, edits);	\
} \
  \
void ActivityObjectDialog::post_edit_##lang##_selection(ActivityObjectDialog * d, QString s) \
{ \
  d->ed##lang##_selection->setText(s); \
}

DEF_EDIT(uml, TxtEdit)
DEF_EDIT(cpp, CppEdit)
DEF_EDIT(java, JavaEdit)

void ActivityObjectDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = data->browser_node;
  QString s = edname->text().stripWhiteSpace();
  
  bn->set_name(s);
  data->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
  
  AType t;
  
  s = edtype->currentText().stripWhiteSpace();
  if (!s.isEmpty()) {
    int index = list.findIndex(s);
    
    if (index >= 0)
      t.type = (BrowserClass *) nodes.at(index);
    else
      t.explicit_type = s;
  }
  data->set_type(t);
  data->multiplicity =
    edmultiplicity->currentText().stripWhiteSpace();
  data->ordering = ordering(edordering->currentText());
  data->is_control = is_control_cb->isChecked();
  data->in_state = edin_state->text().stripWhiteSpace();
  data->uml_selection = eduml_selection->text().stripWhiteSpace();
  data->cpp_selection = edcpp_selection->text().stripWhiteSpace();
  data->java_selection = edjava_selection->text().stripWhiteSpace();
  
  bn->set_comment(comment->text());
  UmlWindow::set_commented(bn);
  
  kvtable->update(bn);
  
  bn->modified();
  bn->package_modified();
  data->modified();
  
  QTabDialog::accept();
}
