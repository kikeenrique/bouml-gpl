// *************************************************************************
//
// Copyright (C) 2004-2006 Bruno PAGES  All rights reserved.
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

#include "ExtraMemberDialog.h"
#include "ExtraMemberData.h"
#include "BrowserExtraMember.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "strutil.h"

QSize ExtraMemberDialog::previous_size;

ExtraMemberDialog::ExtraMemberDialog(ExtraMemberData * ex)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), emd(ex) {
  ex->browser_node->edit_start();
  
  if (ex->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  bool visit = !hasOkButton();

  setCaption("Extra Class Member dialog");
  
  QGrid * grid;
    
  // general tab
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("name :", grid);
  edname = new LineEdit(ex->name(), grid);
  edname->setReadOnly(visit);

  QFont font = edname->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  
  new QLabel("stereotype :", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(ex->get_stereotype()));
  edstereotype->setCurrentItem(0);
  if (! visit)
    edstereotype->setAutoCompletion(TRUE);
  
  QSizePolicy sp = edstereotype->sizePolicy();
  
  edstereotype->setSizePolicy(sp);
  
  QVBox * vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(ex->get_browser_node()->get_comment());
  comment->setFont(font);
  
  addTab(grid, "Uml");
  
  // C++
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(grid);
  QButtonGroup * gp = 
    new QButtonGroup(1, QGroupBox::Horizontal, QString::null, grid);
  inline_cb = new QCheckBox("inline", gp);
  inline_cb->setDisabled(visit);
  if (ex->cpp_inline)
    inline_cb->setChecked(TRUE);
  
  vtab = new QVBox(grid);
  new QLabel("C++ \ndeclaration :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_cpp_decl()));
  edcpp_decl = new MultiLineEdit(grid);
  edcpp_decl->setReadOnly(visit);
  edcpp_decl->setText(ex->cpp_decl);
  edcpp_decl->setFont(font);
    
  vtab = new QVBox(grid);
  new QLabel("C++ \ndefinition :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_cpp_def()));
  edcpp_def = new MultiLineEdit(grid);
  edcpp_def->setReadOnly(visit);
  edcpp_def->setText(ex->cpp_def);
  edcpp_def->setFont(font);
    
  addTab(grid, "C++");
  
  // Java
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  vtab = new QVBox(grid);
  new QLabel("Java :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_java_decl()));
  edjava_decl = new MultiLineEdit(grid);
  edjava_decl->setReadOnly(visit);
  edjava_decl->setText(ex->java_decl);
  edjava_decl->setFont(font);
    
  addTab(grid, "Java");
  
  // IDL
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  vtab = new QVBox(grid);
  new QLabel("Idl :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_idl_decl()));
  edidl_decl = new MultiLineEdit(grid);
  edidl_decl->setReadOnly(visit);
  edidl_decl->setText(ex->idl_decl);
  edidl_decl->setFont(font);
    
  addTab(grid, "Idl");
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(ex->get_browser_node(), grid, visit);
  addTab(grid, "Properties");
}

void ExtraMemberDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

ExtraMemberDialog::~ExtraMemberDialog() {
  emd->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void ExtraMemberDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = emd->browser_node;
  QString s;
  
  s = edname->text().stripWhiteSpace();
  bn->set_name(s);
  
  emd->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
  emd->cpp_decl = edcpp_decl->text();
  emd->cpp_def = edcpp_def->text();
  emd->cpp_inline = inline_cb->isChecked();
  emd->java_decl = edjava_decl->text();
  emd->idl_decl = edidl_decl->text();
  
  bn->set_comment(comment->text());
  UmlWindow::set_commented(bn);
  
  kvtable->update(bn);
  
  bn->modified();
  bn->package_modified();
  emd->modified();
  
  QTabDialog::accept();
}

void ExtraMemberDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       emd, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void ExtraMemberDialog::post_edit_description(ExtraMemberDialog * d, QString s)
{
  d->comment->setText(s);
}

void ExtraMemberDialog::edit_cpp_decl() {
  edit(edcpp_decl->text(), edname->text().stripWhiteSpace() + "_class_extra_member_decl",
       emd, CppEdit, this, (post_edit) post_edit_cpp_decl, edits);
}

void ExtraMemberDialog::post_edit_cpp_decl(ExtraMemberDialog * d, QString s)
{
  d->edcpp_decl->setText(s);
}

void ExtraMemberDialog::edit_cpp_def() {
  edit(edcpp_def->text(), edname->text().stripWhiteSpace() + "_class_extra_member_def",
       emd, CppEdit, this, (post_edit) post_edit_cpp_def, edits);
}

void ExtraMemberDialog::post_edit_cpp_def(ExtraMemberDialog * d, QString s)
{
  d->edcpp_def->setText(s);
}

void ExtraMemberDialog::edit_java_decl() {
  edit(edjava_decl->text(), edname->text().stripWhiteSpace() + "_class_extra_member",
       emd, JavaEdit, this, (post_edit) post_edit_java_decl, edits);
}

void ExtraMemberDialog::post_edit_java_decl(ExtraMemberDialog * d, QString s)
{
  d->edjava_decl->setText(s);
}

void ExtraMemberDialog::edit_idl_decl() {
  edit(edidl_decl->text(), edname->text().stripWhiteSpace() + "_class_extra_member",
       emd, TxtEdit, this, (post_edit) post_edit_idl_decl, edits);
}

void ExtraMemberDialog::post_edit_idl_decl(ExtraMemberDialog * d, QString s)
{
  d->edidl_decl->setText(s);
}

