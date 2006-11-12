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
#include <qmessagebox.h>
#include <qpushbutton.h> 

#include "TransitionDialog.h"
#include "TransitionData.h"
#include "BrowserTransition.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "strutil.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"

QSize TransitionDialog::previous_size;

TransitionDialog::TransitionDialog(TransitionData * r)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), rel(r) {
  r->browser_node->edit_start();
  
  if (r->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  setCaption("Transition dialog");
  visit = !hasOkButton();  

  BrowserNode * bn = rel->browser_node;
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
  edstereotype->insertItem(toUnicode(rel->get_stereotype()));
  if (!visit) {
    //edstereotype->insertStringList(rel->get_start()->default_stereotypes(type));
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
  //comment->setFont(font);
  
  addTab(grid, "Uml");

  // UML / OCL
  init_tab(uml, rel->uml, "Ocl", SLOT(edit_uml_trigger()),
	   SLOT(edit_uml_guard()), SLOT(edit_uml_expr()));

  // CPP
  init_tab(cpp, rel->cpp, "C++", SLOT(edit_cpp_trigger()),
	   SLOT(edit_cpp_guard()), SLOT(edit_cpp_expr()));

  // Java
  init_tab(java, rel->java, "Java", SLOT(edit_java_trigger()),
	   SLOT(edit_java_guard()), SLOT(edit_java_expr()));
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(bn, grid, visit);
  addTab(grid, "Properties");
}

TransitionDialog::~TransitionDialog() {
  rel->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void TransitionDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void TransitionDialog::init_tab(TransDialog & d, TransDef & td,
				const char * lbl, const char * sl_trigger,
				const char * sl_guard, const char * sl_expr) {
  QGrid * grid = new QGrid(2, this);
  QVBox * vtab;

  grid->setMargin(5);
  grid->setSpacing(5);
  
  vtab = new QVBox(grid);
  new QLabel("trigger : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, sl_trigger);
  d.edtrigger = new MultiLineEdit(grid);

  QFont font = d.edtrigger->font();

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  d.edtrigger->setFont(font);
  d.edtrigger->setText(td.trigger);
  if (visit)
    d.edtrigger->setReadOnly(TRUE);
  
  vtab = new QVBox(grid);
  new QLabel("guard\nconstraint : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, sl_guard);
  d.edguard = new MultiLineEdit(grid);
  d.edguard->setFont(font);
  d.edguard->setText(td.guard);
  if (visit)
    d.edguard->setReadOnly(TRUE);
  
  vtab = new QVBox(grid);
  new QLabel("activity\nexpression : ", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, sl_expr);
  d.edexpr = new MultiLineEdit(grid);
  d.edexpr->setFont(font);
  d.edexpr->setText(td.expr);
  if (visit)
    d.edexpr->setReadOnly(TRUE);
  
  addTab(grid, lbl);
}

void TransitionDialog::edit_description() {
  edit(comment->text(), "transition_description", rel, TxtEdit,
       this, (post_edit) post_edit_description, edits);
}

void TransitionDialog::post_edit_description(TransitionDialog * d, QString s)
{
  d->comment->setText(s);
}

#define DEF_EDIT(attr, lang, Lang) \
void TransitionDialog::edit_##lang##_##attr() {	\
  edit(lang.ed##attr->text(), "transition_"#lang"_"#attr, rel, Lang,  \
       this, (post_edit) post_edit_##lang##_##attr, edits);	\
} \
  \
void TransitionDialog::post_edit_##lang##_##attr(TransitionDialog * d, QString s) \
{ \
  d->lang.ed##attr->setText(s); \
}

DEF_EDIT(trigger, uml, TxtEdit)
DEF_EDIT(trigger, cpp, CppEdit)
DEF_EDIT(trigger, java, JavaEdit)

DEF_EDIT(guard, uml, TxtEdit)
DEF_EDIT(guard, cpp, CppEdit)
DEF_EDIT(guard, java, JavaEdit)

DEF_EDIT(expr, uml, TxtEdit)
DEF_EDIT(expr, cpp, CppEdit)
DEF_EDIT(expr, java, JavaEdit)

void TransitionDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = rel->browser_node;
  QString s;
  
  s = edname->text().stripWhiteSpace();
  if (s.isEmpty())
    s = "<transition>";
  
  if ((s != bn->get_name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlTransition,
						       bn->allow_spaces(),
						       bn->allow_empty()))
    QMessageBox::critical(0, "Error", s + "\n\nillegal name or already used");
  else {  
    bn->set_name(s);
    rel->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    
    uml.accept(rel->uml);  
    cpp.accept(rel->cpp);  
    java.accept(rel->java);    
    
    bn->set_comment(comment->text());
    UmlWindow::set_commented(bn);
    
    kvtable->update(bn);
    
    bn->modified();
    bn->package_modified();
    rel->modified();
    
    QTabDialog::accept();
  }
}

void TransDialog::accept(TransDef & td) {
  td.trigger = edtrigger->text();
  td.guard = edguard->text();
  td.expr = edexpr->text();
}
