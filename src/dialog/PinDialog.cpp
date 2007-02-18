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
#include <qmessagebox.h>
#include <qpopupmenu.h> 
#include <qcursor.h> 

#include "PinDialog.h"
#include "PinData.h"
#include "BrowserClass.h"
#include "BrowserPin.h"
#include "BrowserView.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "GenerationSettings.h"
#include "strutil.h"
#include "BodyDialog.h"

QSize PinDialog::previous_size;

PinDialog::PinDialog(PinData * pi)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), pin(pi) {
  pi->browser_node->edit_start();
  
  if (pi->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  visit = !hasOkButton();
  setCaption("Pin dialog");
  
  QGrid * grid;
  QHBox * htab;
  QString s;
    
  // general tab
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("name :", grid);
  edname = new LineEdit(pi->name(), grid);
  edname->setReadOnly(visit);

  QFont font = edname->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(pi->stereotype));
  if (! visit) {
    edstereotype->insertStringList(BrowserPin::default_stereotypes());
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  
  QSizePolicy sp = edstereotype->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
    
  connect(new SmallPushButton("type :", grid), SIGNAL(clicked()),
	  this, SLOT(menu_type()));
  
  edtype = new QComboBox(!visit, grid);
  edtype->insertItem(pi->get_type().get_full_type());
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
  
  new QLabel("direction :", grid);
  htab = new QHBox(grid);
  eddir = new QComboBox(FALSE, htab);
  
  UmlParamDirection dir = pi->get_dir();
  
  eddir->insertItem(stringify(dir));
  if (! visit) {
    // note : inout not allowed
    if (dir != UmlIn)
      eddir->insertItem(stringify(UmlIn));
    if (dir != UmlOut)
      eddir->insertItem(stringify(UmlOut));
    if (dir != UmlReturn)
      eddir->insertItem(stringify(UmlReturn));
  }
  
  new QLabel("   multiplicity : ", htab);
  edmultiplicity = new QComboBox(!visit, htab);
  edmultiplicity->setSizePolicy(sp);
  edmultiplicity->insertItem(pi->get_multiplicity());
  if (!visit) {
    edmultiplicity->insertItem("1");
    edmultiplicity->insertItem("0..1");
    edmultiplicity->insertItem("*");
    edmultiplicity->insertItem("1..*");
  }
  
  new QLabel("   ordering : ", htab);
  edordering = new QComboBox(FALSE, htab);
  
  UmlOrderingKind o = pi->get_ordering();
  
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
    
  new QLabel("   effect : ", htab);
  edeffect = new QComboBox(FALSE, htab);
  
  UmlParamEffect e = pi->get_effect();
  
  edeffect->insertItem(stringify(e));
  if (!visit) {
    if (e != UmlNoEffect)
      edeffect->insertItem(stringify(UmlNoEffect));
    if (e != UmlCreate)
      edeffect->insertItem(stringify(UmlCreate));
    if (e != UmlRead)
      edeffect->insertItem(stringify(UmlRead));
    if (e != UmlUpdate)
      edeffect->insertItem(stringify(UmlUpdate));
    if (e != UmlDelete)
      edeffect->insertItem(stringify(UmlDelete));
  }
    
  new QLabel("in state : ", grid);
  edin_state = new LineEdit(pi->in_state, grid);
  edin_state->setReadOnly(visit);
       
  new QLabel(grid);
  htab = new QHBox(grid);
  QButtonGroup * bg = 
    new QButtonGroup(2, QGroupBox::Horizontal, QString::null, htab);
  
  is_control_cb = new QCheckBox("is_control", bg);
  if (pi->is_control)
    is_control_cb->setChecked(TRUE);
  is_control_cb->setDisabled(visit);
  
  unique_cb = new QCheckBox("unique", bg);
  if (pi->unique)
    unique_cb->setChecked(TRUE);
  unique_cb->setDisabled(visit);
  
  bg = 
    new QButtonGroup(3, QGroupBox::Horizontal, QString::null, htab);
  bg->setExclusive(TRUE);
  
  standard_rb = new QRadioButton("standard", bg);
  exception_rb = new QRadioButton("exception", bg);
  stream_rb = new QRadioButton("stream", bg);
  
  if (pi->exception)
    exception_rb->setChecked(TRUE);
  else if (pi->stream)
    stream_rb->setChecked(TRUE);
  else
    standard_rb->setChecked(TRUE);
  
  QVBox * vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (! visit) {
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  }
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(pi->browser_node->get_comment());
  comment->setFont(font);
  
  addTab(grid, "Uml");
  
  init_tab(eduml_selection, pin->uml_selection, "Ocl",
	   SLOT(edit_uml_selection()));

  // C++
  init_tab(edcpp_selection, pin->cpp_selection, "C++",
	   SLOT(edit_cpp_selection()));

  // Java
  init_tab(edjava_selection, pin->java_selection, "Java",
	   SLOT(edit_java_selection()));
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(pi->browser_node, grid, visit);
  addTab(grid, "Properties");
}

PinDialog::~PinDialog() {
  pin->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void PinDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void PinDialog::init_tab(MultiLineEdit *& ed, const char * v,
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

void PinDialog::menu_type() {
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

#define DEF_EDIT(lang, Lang) \
void PinDialog::edit_##lang##_selection() {	\
  edit(ed##lang##_selection->text(), "object_"#lang"_selection", pin, Lang,  \
       this, (post_edit) post_edit_##lang##_##selection, edits);	\
} \
  \
void PinDialog::post_edit_##lang##_selection(PinDialog * d, QString s) \
{ \
  d->ed##lang##_selection->setText(s); \
}

DEF_EDIT(uml, TxtEdit)
DEF_EDIT(cpp, CppEdit)
DEF_EDIT(java, JavaEdit)

void PinDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = pin->browser_node;
  QString s;
  
  s = edname->text().stripWhiteSpace();
  if ((s != pin->name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlActivityPin,
						       bn->allow_spaces(),
						       bn->allow_empty()))
    QMessageBox::critical(0, "Error", s + "\n\nillegal name or already used");
  else {
    // check consistency
    UmlParamDirection dir = direction(eddir->currentText());
    bool exception = exception_rb->isChecked();
    UmlParamEffect effect = ::effect(edeffect->currentText());
    QString err;
    
    if ((dir == UmlIn) && exception)
      err = "An input pin cannot be an exception.\n";
    switch (effect) {
    case UmlDelete:
      if ((dir != UmlIn) && (dir != UmlInOut))
	err += "Only in and inout pin may have a delete effect.";
      break;
    case UmlCreate:
      if ((dir != UmlOut) && (dir != UmlInOut) && (dir != UmlReturn))
	err += "Only out, inout and return pin may have a create effect.";
      break;
    default:
      break;
    }
    
    if (!err.isEmpty())
      QMessageBox::critical(0, "Error", err);
    else {
      bn->set_name(s);
      pin->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
      
      AType t;
      
      s = edtype->currentText().stripWhiteSpace();
      if (!s.isEmpty()) {
	int index = list.findIndex(s);
	
	if (index >= 0)
	  t.type = (BrowserClass *) nodes.at(index);
	else
	  t.explicit_type = s;
      }
      pin->set_type(t);
      pin->dir = dir;
      pin->multiplicity =
	edmultiplicity->currentText().stripWhiteSpace();
      pin->ordering = ordering(edordering->currentText());
      pin->effect = effect;
      pin->is_control = is_control_cb->isChecked();
      pin->unique = unique_cb->isChecked();
      pin->in_state = edin_state->text().stripWhiteSpace();
      pin->exception = exception;
      pin->stream = stream_rb->isChecked();
      pin->uml_selection = eduml_selection->text().stripWhiteSpace();
      pin->cpp_selection = edcpp_selection->text().stripWhiteSpace();
      pin->java_selection = edjava_selection->text().stripWhiteSpace();
      
      bn->set_comment(comment->text());
      UmlWindow::set_commented(bn);
      
      kvtable->update(bn);
      
      bn->modified();
      bn->package_modified();
      pin->modified();
      
      QTabDialog::accept();
    }
  }
}

void PinDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       pin, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void PinDialog::post_edit_description(PinDialog * d, QString s)
{
  d->comment->setText(s);
}
