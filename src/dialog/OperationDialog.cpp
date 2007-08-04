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

#include <stdio.h>

#include <qcursor.h>
#include <qgrid.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qradiobutton.h> 
#include <qpopupmenu.h> 
#include <qpushbutton.h> 
#include <qlayout.h>
#include <qtimer.h>

#include "OperationDialog.h"
#include "OperationData.h"
#include "ParamData.h"
#include "BrowserOperation.h"
#include "ClassData.h"
#include "BrowserAttribute.h"
#include "AttributeData.h"
#include "BrowserRelation.h"
#include "RelationData.h"
#include "BrowserView.h"
#include "UmlDesktop.h"
#include "BrowserClass.h"
#include "KeyValueTable.h"
#include "ComboItem.h"
#include "myio.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "GenerationSettings.h"
#include "BrowserView.h"
#include "strutil.h"
#include "BodyDialog.h"
#include "AnnotationDialog.h"
#include "Tool.h"

QSize OperationDialog::previous_size;

OperationDialog::OperationDialog(OperationData * o, DrawingLanguage l)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), oper(o),
      cl((ClassData *) ((BrowserClass *) o->browser_node->parent())->get_data()) {
  o->browser_node->edit_start();
  
  if (o->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  setCaption("Operation dialog");

  visit = !hasOkButton();
  
  QString stereotype = cl->get_stereotype();
  QString st;
  
  st = GenerationSettings::cpp_class_stereotype(stereotype);
  cpp_undef = (st == "enum") || (st == "typedef");
  
  st = GenerationSettings::java_class_stereotype(stereotype);
  java_undef = (st == "enum_pattern") || (st == "typedef");
  
  st = GenerationSettings::idl_class_stereotype(stereotype);
  idl_undef = (st == "enum") || (st == "typedef");
  
  // to manage get_ set_, search corresp attr/rel
  
  get_of_attr = 0;
  set_of_attr = 0;
  get_of_rel = 0;
  set_of_rel = 0;
  
  if (o->is_get_or_set) {
    BrowserOperation * br_op = (BrowserOperation *) o->browser_node;
    QListViewItem * child;
    
    for (child = br_op->parent()->firstChild();
	 child != 0;
	 child = child->nextSibling()) {
      BrowserNode * br = (BrowserNode *) child;
      
      if (!br->deletedp()) {
	if (br->get_type() == UmlAttribute) {
	  if (((BrowserAttribute *) br)->get_get_oper() == br_op) {
	    get_of_attr = (AttributeData *) br->get_data();
	    break;
	  }
	  if (((BrowserAttribute *) br)->get_set_oper() == br_op) {
	    set_of_attr = (AttributeData *) br->get_data();
	    break;
	  }
	}
	else if (IsaRelation(br->get_type())) {
	  if (((BrowserRelation *) br)->get_get_oper() == br_op) {
	    get_of_rel = (RelationData *) br->get_data();
	    is_rel_a = get_of_rel->is_a((BrowserRelation *) br);
	    break;
	  }
	  if (((BrowserRelation *) br)->get_set_oper() == br_op) {
	    set_of_rel = (RelationData *) br->get_data();
	    is_rel_a = set_of_rel->is_a((BrowserRelation *) br);
	    break;
	  }
	}
      }
    }
  }
    
  // general tab
  
  QGrid * grid;
  QHBox * htab;
  QButtonGroup * bg;
    
  grid = new QGrid(2, this);
  umltab = grid;
  grid->setSpacing(5);
  grid->setMargin(5);
  
  new QLabel("class : ", grid);
  new QLabel(((BrowserNode *) o->get_browser_node()->parent())->full_name(TRUE),
	     grid);
  
  new QLabel("name : ", grid);
  edname = new LineEdit(o->name(), grid);
  edname->setReadOnly(visit);
  
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(oper->stereotype));
  if (o->is_get_or_set)
    edstereotype->setEnabled(FALSE);
  else if (! visit) {
    edstereotype->insertStringList(BrowserOperation::default_stereotypes());
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  
  QSizePolicy sp = edstereotype->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);

  if (o->is_get_or_set)
    new QLabel("value type : ", grid);
  else
    connect(new SmallPushButton("value type :", grid), SIGNAL(clicked()),
	    this, SLOT(menu_returntype()));
  
  BrowserClass::instances(nodes);
  nodes.full_names(list);

  edreturn_type = new QComboBox(!visit, grid);
  edreturn_type->insertItem(o->get_return_type().get_full_type());
  if (o->is_get_or_set)
    edreturn_type->setEnabled(FALSE);
  else if (! visit) {
    edreturn_type->insertStringList(GenerationSettings::basic_types());
    edreturn_type_offset = edreturn_type->count();
    edreturn_type->insertStringList(list);
    edreturn_type->setAutoCompletion(TRUE);
    view = o->browser_node->container(UmlClass);
  }
  edreturn_type->setCurrentItem(0);
  
  edreturn_type->setSizePolicy(sp);

  new QLabel(grid);
  
  htab = new QHBox(grid);
  bg = uml_visibility.init(htab, o->get_uml_visibility(), TRUE);
  if (visit)
    bg->setEnabled(FALSE);
  htab->setStretchFactor(bg, 1000);
  
  htab->setStretchFactor(new QLabel("      ", htab), 0);
  
  bg = new QButtonGroup(2, QGroupBox::Horizontal, QString::null, htab);
  htab->setStretchFactor(bg, 1000);
  classoperation_cb = new QCheckBox("class operation", bg);
  classoperation_cb->setDisabled(visit);
  abstract_cb = new QCheckBox("abstract", bg);
  abstract_cb->setDisabled(visit);
  if (o->get_isa_class_operation())
    classoperation_cb->setChecked(TRUE);
  else if (o->get_is_abstract())
    abstract_cb->setChecked(TRUE);
  if (o->is_get_or_set)
    bg->setEnabled(FALSE);
  else if (! visit) {
    connect(classoperation_cb, SIGNAL(toggled(bool)),
	    SLOT(classoper_toggled(bool)));
    connect(abstract_cb, SIGNAL(toggled(bool)),
	    SLOT(abstract_toggled(bool)));
  }
  
  new QLabel("parameters : ", grid);
  table = new ParamsTable(o, grid, list, visit);
  if (o->is_get_or_set)
    table->setEnabled(FALSE);
  
  new QLabel("exceptions : ", grid);
  etable = new ExceptionsTable(o, grid, list, visit);
  
  QVBox * vtab = new QVBox(grid);
  
  new QLabel("description :", vtab);
  if (! visit) {
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
    connect(new SmallPushButton("Default", vtab), SIGNAL(clicked()),
	    this, SLOT(default_description()));
  }
  comment = new MultiLineEdit(grid);
  comment->setText(o->get_browser_node()->get_comment());
  QFont font = comment->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  comment->setFont(font);
  comment->setReadOnly(visit);
  
  vtab = new QVBox(grid);
  new QLabel("constraint :", vtab);
  if (! visit) {
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_constraint()));
  }
  constraint = new MultiLineEdit(grid);
  constraint->setReadOnly(visit);
  constraint->setText(o->constraint);
  constraint->setFont(font);
  
  addTab(grid, "Uml");
  
  // C++
  
  if (! cpp_undef) {
    grid = new QGrid(2, this);
    cpptab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);

    if (visit || !o->is_get_or_set)
      new QLabel(grid);
    else {
      cppfrozen_cb = new QCheckBox("frozen", grid);
      if (o->cpp_get_set_frozen)
	cppfrozen_cb->setChecked(TRUE);
    }

    htab = new QHBox(grid);
    
    bg = cpp_visibility.init(htab, o->get_cpp_visibility(), FALSE, 0, "follow uml");
    if (visit)
      bg->setEnabled(FALSE);
    
    htab->setStretchFactor(new QLabel("      ", htab), 0);
    
    bg = new QButtonGroup(5, QGroupBox::Horizontal, QString::null, htab);
    const_cb = new QCheckBox("const", bg);
    volatile_cb = new QCheckBox("volatile", bg);
    friend_cb = new QCheckBox("friend", bg);
    virtual_cb = new QCheckBox("virtual", bg);
    inline_cb = new QCheckBox("inline", bg);
    if (o->get_cpp_const())
      const_cb->setChecked(TRUE);
    if (o->get_is_volatile())
      volatile_cb->setChecked(TRUE);
    if (o->get_cpp_friend())
      friend_cb->setChecked(TRUE);
    if (o->get_cpp_virtual())
      virtual_cb->setChecked(TRUE);
    if (o->get_cpp_inline())
      inline_cb->setChecked(TRUE);
    if (visit) {
      const_cb->setDisabled(TRUE);
      volatile_cb->setDisabled(TRUE);
      virtual_cb->setDisabled(TRUE);
      friend_cb->setDisabled(TRUE);
      inline_cb->setDisabled(TRUE);
    }
    else {
      connect(const_cb, SIGNAL(toggled(bool)),
	      SLOT(const_volatile_toggled(bool)));
      connect(volatile_cb, SIGNAL(toggled(bool)),
	      SLOT(const_volatile_toggled(bool)));
      connect(friend_cb, SIGNAL(toggled(bool)),
	      SLOT(friend_toggled(bool)));
      connect(virtual_cb, SIGNAL(toggled(bool)),
	      SLOT(virtual_toggled(bool)));
      connect(inline_cb, SIGNAL(toggled(bool)),
	      SLOT(inline_toggled(bool)));
    }
    
    if (o->is_get_or_set) {
      new QLabel("Name form : ", grid);
      edcppnamespec = new LineEdit(grid);
      edcppnamespec->setText(o->cpp_name_spec);
      if (visit)
	edcppnamespec->setReadOnly(TRUE);
      else {
	connect(edcppnamespec, SIGNAL(textChanged(const QString &)), this, SLOT(cpp_update_decl()));
	connect(edcppnamespec, SIGNAL(textChanged(const QString &)), this, SLOT(cpp_update_def()));
      }
    }
    else
      edcppnamespec = 0;
    
    new QLabel("Declaration : ", grid);
    edcppdecl = new MultiLineEdit(grid);
    edcppdecl->setText(o->get_cppdecl());
    edcppdecl->setFont(font);
    if (visit)
      edcppdecl->setReadOnly(TRUE);
    else
      connect(edcppdecl, SIGNAL(textChanged()), this, SLOT(cpp_update_decl()));
    
    new QLabel("Result after\nsubstitution : ", grid);
    showcppdecl = new MultiLineEdit(grid);
    showcppdecl->setReadOnly(TRUE);
    showcppdecl->setFont(font);
    
    if (! visit) {
      new QLabel(grid);
      htab = new QHBox(grid);  
      connect(new QPushButton("Default declaration", htab), SIGNAL(pressed()),
	      this, SLOT(cpp_default_decl()));
      if (!o->is_get_or_set)
	connect(new QPushButton("From definition", htab), SIGNAL(pressed()),
		this, SLOT(cpp_decl_from_def()));
      connect(new QPushButton("Not generated in C++", htab), SIGNAL(pressed()),
	      this, SLOT(cpp_unmapped_decl()));
      connect(new QPushButton("Edit parameters", htab), SIGNAL(clicked()),
	      this, SLOT(cpp_edit_param_decl()));
    }
    
    new QLabel("Definition :", grid);
    edcppdef = new MultiLineEdit(grid);
    edcppdef->setText(o->get_cppdef());
    edcppdef->setFont(font);
    if (visit)
      edcppdef->setReadOnly(TRUE);
    else
      connect(edcppdef, SIGNAL(textChanged()), this, SLOT(cpp_update_def()));
    
    new QLabel("Result after\nsubstitution : ", grid);
    showcppdef = new MultiLineEdit(grid);
    showcppdef->setReadOnly(TRUE);
    showcppdef->setFont(font);
    
    editcppbody = new QPushButton((visit || (preserve_bodies())) ? "Show body" : "Edit body", grid);
    connect(editcppbody, SIGNAL(clicked()), this, SLOT(cpp_edit_body()));
    
    char * b = o->get_body(TRUE);
    
    if (b != 0) {
      cppbody = oldcppbody = b;
      delete [] b;
    }
        
    if (! visit) {
      htab = new QHBox(grid);  
      connect(new QPushButton("Default definition", htab), SIGNAL(pressed ()),
	      this, SLOT(cpp_default_def()));
      if (!o->is_get_or_set)
	connect(new QPushButton("From declaration", htab), SIGNAL(pressed ()),
		this, SLOT(cpp_def_from_decl()));
      connect(new QPushButton("Not generated in C++", htab), SIGNAL(pressed ()),
	      this, SLOT(cpp_unmapped_def()));
      connect(new QPushButton("Edit parameters", htab), SIGNAL(clicked()),
	      this, SLOT(cpp_edit_param_def()));
    }
    
    addTab(grid, "C++");
    
    cl->get_class_spec(templates, cl_names, templates_tmplop, cl_names_tmplop);
  }
  else
    cpptab = 0;
  
  // Java
  
  if (! java_undef) {
    grid = new QGrid(2, this);
    javatab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);
    
    if (visit || !o->is_get_or_set)
      new QLabel(grid);
    else {
      javafrozen_cb = new QCheckBox("frozen", grid);
      if (o->java_get_set_frozen)
	javafrozen_cb->setChecked(TRUE);
    }

    bg = new QButtonGroup(2, QGroupBox::Horizontal, QString::null, grid);
    final_cb = new QCheckBox("final", bg);
    if (o->get_java_final())
      final_cb->setChecked(TRUE);
    if (visit)
      final_cb->setDisabled(TRUE);
    else
      connect(final_cb, SIGNAL(toggled(bool)),
	      SLOT(finalsynchronized_toggled(bool)));

    synchronized_cb = new QCheckBox("synchronized", bg);
    if (o->get_java_synchronized())
      synchronized_cb->setChecked(TRUE);
    if (visit)
      synchronized_cb->setDisabled(TRUE);
    else
      connect(synchronized_cb, SIGNAL(toggled(bool)),
	      SLOT(finalsynchronized_toggled(bool)));
    
    if (o->is_get_or_set) {
      new QLabel("Name form : ", grid);
      htab = new QHBox(grid);
      edjavanamespec = new LineEdit(htab);
      edjavanamespec->setText(o->java_name_spec);
      if (visit)
	edjavanamespec->setDisabled(TRUE);
      else {
	connect(edjavanamespec, SIGNAL(textChanged(const QString &)), this, SLOT(java_update_def()));
      }
    }
    else
      edjavanamespec = 0;
    
    new QLabel("Definition :", grid);
    edjavadef = new MultiLineEdit(grid);
    edjavadef->setText(o->get_javadef());
    edjavadef->setFont(font);
    if (visit)
      edjavadef->setReadOnly(TRUE);
    else
      connect(edjavadef, SIGNAL(textChanged()), this, SLOT(java_update_def()));
    
    new QLabel("Result after\nsubstitution : ", grid);
    showjavadef = new MultiLineEdit(grid);
    showjavadef->setReadOnly(TRUE);
    showjavadef->setFont(font);
    
    editjavabody = new QPushButton((visit || (preserve_bodies())) ? "Show body" : "Edit body", grid);
    connect(editjavabody, SIGNAL(clicked()), this, SLOT(java_edit_body()));
    
    char * b = o->get_body(FALSE);
    
    if (b != 0) {
      javabody = oldjavabody = b;
      delete [] b;
    }
        
    htab = new QHBox(grid);  

    if (! visit) {
      connect(new QPushButton("Default definition", htab), SIGNAL(pressed ()),
	      this, SLOT(java_default_def()));
      connect(new QPushButton("Not generated in Java", htab), SIGNAL(pressed ()),
	      this, SLOT(java_unmapped_def()));
      
    }
    
    javaannotation = (const char *) o->java_annotation;
    editjavaannotation =
      new QPushButton((visit) ? "Show annotation" : "Edit annotation",
		      htab);
    connect(editjavaannotation, SIGNAL(clicked ()),
	    this, SLOT(java_edit_annotation()));
    
    addTab(grid, "Java");
  }
  else
    javatab = 0;
    
  // IDL
  
  if (! idl_undef) {
    grid = new QGrid(2, this);
    idltab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);
    
    if (visit || !o->is_get_or_set)
      new QLabel(grid);
    else {
      idlfrozen_cb = new QCheckBox("frozen", grid);
      if (o->idl_get_set_frozen)
	idlfrozen_cb->setChecked(TRUE);
    }

    bg = new QButtonGroup(1, QGroupBox::Horizontal, QString::null, grid);
    oneway_cb = new QCheckBox("oneway", bg);
    if (o->get_idl_oneway())
      oneway_cb->setChecked(TRUE);
    if (visit)
      oneway_cb->setDisabled(TRUE);
    else
      connect(oneway_cb, SIGNAL(toggled(bool)),
	      SLOT(oneway_toggled(bool)));
    
    if (o->is_get_or_set) {
      new QLabel("Name form : ", grid);
      htab = new QHBox(grid);
      edidlnamespec = new LineEdit(htab);
      edidlnamespec->setText(o->idl_name_spec);
      if (visit)
	edidlnamespec->setReadOnly(TRUE);
      else {
	connect(edidlnamespec, SIGNAL(textChanged(const QString &)), this, SLOT(idl_update_decl()));
      }
    }
    else
      edidlnamespec = 0;
    
    new QLabel("Declaration :", grid);
    edidldecl = new MultiLineEdit(grid);
    edidldecl->setText(o->get_idldecl());
    edidldecl->setFont(font);
    if (visit)
      edidldecl->setReadOnly(TRUE);
    else
      connect(edidldecl, SIGNAL(textChanged()), this, SLOT(idl_update_decl()));
    
    new QLabel("Result after\nsubstitution : ", grid);
    showidldecl = new MultiLineEdit(grid);
    showidldecl->setReadOnly(TRUE);
    showidldecl->setFont(font);
    
    if (! visit) {
      new QLabel(grid);
      htab = new QHBox(grid);  
      connect(new QPushButton("Default declaration", htab), SIGNAL(pressed ()),
	      this, SLOT(idl_default_def()));
      connect(new QPushButton("Not generated in Idl", htab), SIGNAL(pressed ()),
	      this, SLOT(idl_unmapped_def()));
    }
    
    addTab(grid, "Idl");
  }
  else
    idltab = 0;
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(o->get_browser_node(), grid, visit);
  addTab(grid, "Properties");
  
  //
  
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(update_all_tabs(QWidget *)));
  
  switch (l) {
  case CppView:
    if (! cpp_undef)
      QTimer::singleShot(100, this, SLOT(cpp_edit_body()));
    break;
  case JavaView:
    if (! java_undef)
      QTimer::singleShot(100, this, SLOT(java_edit_body()));
    break;
  default:
    break;
  }
}

OperationDialog::~OperationDialog() {
  oper->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void OperationDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void OperationDialog::menu_returntype() {
  QPopupMenu m(0);

  m.insertItem("Choose", -1);
  m.insertSeparator();
  
  int index = list.findIndex(edreturn_type->currentText().stripWhiteSpace());
  
  if (index != -1)
    m.insertItem("Select in browser", 0);
  
  BrowserNode * bn = 0;
  
  if (! visit) {
    bn = BrowserView::selected_item();
    
    if ((bn != 0) && (bn->get_type() == UmlClass) && !bn->deletedp())
      m.insertItem("Choose class selected in browser", 1);
    else
      bn = 0;
    
    m.insertItem("Create class and choose it", 2);
  }
  
  if (!visit || (index != -1) || (bn != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      nodes.at(index)->select_in_browser();
      break;
    case 2:
      bn = BrowserClass::add_class(view);
      if (bn == 0)
	return;
      bn->select_in_browser();
      // no break
    case 1:
      {
	QString s = bn->full_name(TRUE);
	
	if ((index = list.findIndex(s)) == -1) {
	  // new class, may be created through an other dialog
	  index = 0;
	  QStringList::Iterator iter = list.begin();
	  QStringList::Iterator iter_end = list.end();
	  
	  while ((iter != iter_end) && (*iter < s)) {
	    ++iter;
	    index += 1;
	  }
	  nodes.insert((unsigned) index, bn);
	  list.insert(iter, s);
	  edreturn_type->insertItem(s, index + edreturn_type_offset);
	}
      }
      edreturn_type->setCurrentItem(index + edreturn_type_offset);
    }
  }
}

void OperationDialog::default_description() {
  comment->setText(GenerationSettings::default_operation_description());
}

void OperationDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       oper, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void OperationDialog::post_edit_description(OperationDialog * d, QString s)
{
  d->comment->setText(s);
}

void OperationDialog::edit_constraint() {
  edit(constraint->text(), edname->text().stripWhiteSpace() + "_constraint",
       oper, TxtEdit, this, (post_edit) post_edit_constraint, edits);
}

void OperationDialog::post_edit_constraint(OperationDialog * d, QString s)
{
  d->constraint->setText(s);
}

void OperationDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = oper->browser_node;
  QString s = edname->text().stripWhiteSpace();
  
  if ((s != oper->name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlOperation,
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical("Error", s + "\n\nillegal name or already used");
  else {  
    bn->set_name(s);
    
    if (!oper->is_get_or_set) {
      AType t;
      
      s = edreturn_type->currentText().stripWhiteSpace();
      
      if (! s.isEmpty()) {
	int index = list.findIndex(edreturn_type->currentText());
	
	if (index >= 0)
	  t.type = (BrowserClass *) nodes.at(index);
	else
	  t.explicit_type = s;
      }
      oper->set_return_type(t);
      
      oper->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    }
    
    oper->uml_visibility = uml_visibility.value();
    
    oper->isa_class_operation = classoperation_cb->isChecked();
    oper->set_is_abstract(abstract_cb->isChecked());
    
    table->update(oper, nodes);
    etable->update(oper, nodes);
    
    bn->set_comment(comment->text());
    UmlWindow::update_comment_if_needed(bn);
  
    oper->constraint = constraint->stripWhiteSpaceText();
    
    // C++
    
    if (cpp_undef) {
      oper->cpp_decl = QString::null;
      oper->cpp_def.assign(QString::null, TRUE);
      if (!oldcppbody.isEmpty())
	oper->new_body(QString::null, TRUE);
    }
    else {
      if (oper->is_get_or_set) {
	oper->cpp_name_spec = edcppnamespec->text().stripWhiteSpace();
	oper->cpp_body.length = 0;
	oper->cpp_get_set_frozen = cppfrozen_cb->isChecked();
      }
      if (!abstract_cb->isChecked() && 
	  (edcppdef->text().find("${body}") != -1)) {
	if (cppbody != oldcppbody)
	  oper->new_body(cppbody, TRUE);
      }
      else if(!oldcppbody.isEmpty())
	oper->new_body(QString::null, TRUE);
      oper->cpp_decl = edcppdecl->text();
      oper->cpp_visibility = cpp_visibility.value();
      
      oper->cpp_const = const_cb->isChecked();
      oper->is_volatile = volatile_cb->isChecked();
      oper->cpp_friend = friend_cb->isChecked();
      oper->cpp_virtual = virtual_cb->isChecked();
      oper->cpp_inline = inline_cb->isChecked();
      oper->cpp_def.assign(edcppdef->text(),
			   abstract_cb->isChecked() ||
			   (edcppdef->text().find("${body}") != -1));
    }
    
    // java
    
    if (java_undef) {
      oper->java_def.assign(QString::null, TRUE);
      if (!oldjavabody.isEmpty())
	oper->new_body(QString::null, FALSE);
    }
    else {
      if (oper->is_get_or_set) {
	oper->java_name_spec = edjavanamespec->text().stripWhiteSpace();
	oper->java_get_set_frozen = javafrozen_cb->isChecked();
      }
      
      QString ste = GenerationSettings::java_class_stereotype(cl->get_stereotype());
      bool interf = (ste == "interface") || (ste == "@interface");
      
      if (!abstract_cb->isChecked() && !interf &&
	  (edjavadef->text().find("${body}") != -1)) {
	if (javabody != oldjavabody)
	  oper->new_body(javabody, FALSE);
      }
      else if (!oldjavabody.isEmpty())
	oper->new_body(QString::null, FALSE);
      oper->java_final = final_cb->isChecked();
      oper->java_synchronized = synchronized_cb->isChecked();
      oper->java_def.assign(edjavadef->text(),
			    abstract_cb->isChecked() || interf ||
			    (edjavadef->text().find("${body}") != -1));
      
      oper->java_annotation = javaannotation;
    }
    
    // idl
    
    if (idl_undef)
      oper->idl_decl = QString::null;
    else {
      if (oper->is_get_or_set) {
	oper->idl_name_spec = edidlnamespec->text().stripWhiteSpace();
	oper->idl_get_set_frozen = idlfrozen_cb->isChecked();
      }
      oper->idl_oneway = oneway_cb->isChecked();
      oper->idl_decl = edidldecl->text();
    }
    
    // user
    
    kvtable->update(bn);
    
    bn->modified();
    bn->package_modified();
    oper->modified();
    
    QTabDialog::accept();
  }
}

void OperationDialog::classoper_toggled(bool on) {
  if (on) {
    abstract_cb->setChecked(FALSE);
    if (! cpp_undef)
      virtual_cb->setChecked(FALSE);
  }
}

void OperationDialog::abstract_toggled(bool on) {
  if (on) {
    classoperation_cb->setChecked(FALSE);
    if (! cpp_undef)
      virtual_cb->setChecked(TRUE);
  }
}

void OperationDialog::update_all_tabs(QWidget * w) {
  table->forceUpdateCells();
  etable->forceUpdateCells();
  
  if (w == umltab) {
    if (! visit)
      edname->setFocus();
  }
  else if (w == cpptab) {
    cpp_update_decl();
    cpp_update_def();
    if (! visit)
      edcppdecl->setFocus();
  }
  else if (w == javatab) {
    java_update_def();
    if (! visit)
      edjavadef->setFocus();
  }
  else if (w == idltab) {
    idl_update_decl();
    if (! visit)
      edidldecl->setFocus();
  }
}

// C++

void OperationDialog::const_volatile_toggled(bool) {
  cpp_update_decl();
  cpp_update_def();
}

void OperationDialog::virtual_toggled(bool on) {
  if (on)
    friend_cb->setChecked(FALSE);
  cpp_update_decl();
  cpp_update_def();
}

void OperationDialog::friend_toggled(bool on) {
  if (on) {
    inline_cb->setChecked(FALSE);
    virtual_cb->setChecked(FALSE);
  }
  cpp_update_decl();
  cpp_update_def();
}

void OperationDialog::inline_toggled(bool on) {
  if (on)
    friend_cb->setChecked(FALSE);
  cpp_update_decl();
  cpp_update_def();
}

void OperationDialog::cpp_default_decl() {
  if (oper->is_get_or_set) {
    QCString decl;
    QCString def;
    
    if (get_of_attr != 0)
      oper->update_cpp_get_of(decl, def, get_of_attr->get_browser_node()->get_name(),
			      get_of_attr->get_cppdecl(),
			      get_of_attr->get_isa_const_attribute(),
			      QString::null);
    else if (set_of_attr != 0)
      oper->update_cpp_set_of(decl, def, set_of_attr->get_browser_node()->get_name(),
			      set_of_attr->get_cppdecl(),
			      set_of_attr->get_isa_const_attribute(),
			      QString::null);
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_cpp_get_of(decl, def, get_of_rel->get_role_a(), 
				get_of_rel->get_cppdecl_a(), 
				get_of_rel->get_isa_const_relation_a(), 
				get_of_rel->get_multiplicity_a());
      else
	oper->update_cpp_get_of(decl, def, get_of_rel->get_role_b(), 
				get_of_rel->get_cppdecl_b(), 
				get_of_rel->get_isa_const_relation_b(), 
				get_of_rel->get_multiplicity_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_cpp_set_of(decl, def, set_of_rel->get_role_a(), 
				set_of_rel->get_cppdecl_a(), 
				set_of_rel->get_isa_const_relation_a(), 
				set_of_rel->get_multiplicity_a());
      else
	oper->update_cpp_set_of(decl, def, set_of_rel->get_role_b(), 
				set_of_rel->get_cppdecl_b(), 
				set_of_rel->get_isa_const_relation_b(), 
				set_of_rel->get_multiplicity_b());
    }

    edcppdecl->setText(decl);
  }
  else {
    QString s = oper->default_cpp_decl(edname->text().stripWhiteSpace());
    
    GenerationSettings::set_cpp_return_type(the_type(edreturn_type->currentText()
						     .stripWhiteSpace(),
						     list, nodes),
					    s);
    
    int index;
    QString params;
    int nparams = (int) table->nparams();
    QString sep;
    
    for (index = 0; index != nparams; index += 1, sep = ", ")
      params += sep + GenerationSettings::cpp(the_type(table->type(index),
						       list, nodes),
					      table->dir(index), index);
    
    if ((index = s.find("${)}")) != -1)
      s.insert(index, params);
    
    edcppdecl->setText(s);
  }
  
  cpp_update_decl();
}

void OperationDialog::cpp_unmapped_decl() {
  edcppdecl->setText(QString::null);
  showcppdecl->setText(QString::null);

  cpp_unmapped_def();
}

void OperationDialog::cpp_decl_from_def() {
  QString dcl = oper->default_cpp_decl(edname->text().stripWhiteSpace());
  QString def = edcppdef->text();
  int index1;
  int index2;
  int index3;
  int index4;
  
  // copy args
  if (((index1 = def.find("${(}") + 4) == (-1 + 4)) ||
      ((index2 = def.find("${)}", index1)) == -1) ||
      ((index3 = dcl.find("${(}") + 4) == (-1 + 4)) ||
      ((index4 = dcl.find("${)}", index3)) == -1))
      // wrong specification(s)
    return;
  
  if (index4 != index3)
    dcl.remove(index3, index4 - index3);
  if (index2 != index1)
    dcl.insert(index3, def.mid(index1, index2 - index1));

  // copy return type modifications
  if (((index1 = def.find("${type}")) != -1) &&
      ((index2 = def.find("${class}", index1 + 7)) != -1) &&
      ((index3 = dcl.find("${type}")) != -1) &&
      ((index4 = dcl.find("${name}", index3 + 7)) != -1)) {
    // valid specification
    while ((index1 != 0) && (def.at(index1 - 1) != QChar('}')))
      index1 -= 1;
    while ((index3 != 0) && (dcl.at(index3 - 1) != QChar('}')))
      index3 -= 1;
    dcl.remove(index3, index4 - index3);
    dcl.insert(index3, def.mid(index1, index2 - index1));
  }

  // manage const
  if ((def.find("${const}") == -1) && // const removed
      ((index1 = dcl.find("${const}")) != -1))
    // remove const
    dcl.remove(index1, 8);

  // manage throw
  if ((def.find("${throw}") == -1) && // throw removed
      ((index1 = dcl.find("${throw}")) != -1))
    // remove throw
    dcl.remove(index1, 8);

  // update decl
  edcppdecl->setText(dcl);
  cpp_update_decl();
}

void OperationDialog::cpp_edit_param_decl() {
  QString form = edcppdecl->text();
  int index;

  if (((index = form.find("${(}")) != 0) &&
      (form.find("${)}", index + 4) != 0)) {
    CppParamsDialog d(table, edcppdecl);
    
    if (d.exec() == QDialog::Accepted)
      cpp_update_decl();
  }
  else
    msg_warning("Bouml", "wrong specification");
}

void OperationDialog::manage_cpp_type(unsigned rank, QString & s)
{
  if (rank < table->nparams()) 
    s += get_cpp_name(the_type(table->type(rank), list, nodes));
  else {
    s += "${t";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::manage_var(unsigned rank, QString & s)
{
  if (rank < table->nparams()) 
    s += table->name(rank);
  else {
    s += "${p";
    s += QString::number(rank);
    s += '}';
  }
}

QString OperationDialog::compute_name(LineEdit * spec) {
  if (spec != 0) {
    QString s = spec->text().stripWhiteSpace();
    int index;
    
    if ((index = s.find("${name}")) != -1)
      s.replace(index, 7, ((BrowserOperation *) oper->browser_node)->get_of_name());
    else if ((index = s.find("${Name}")) != -1)
      s.replace(index, 7,
		capitalize(((BrowserOperation *) oper->browser_node)->get_of_name()));
    return s;
  }
  else
    return edname->text();
}

void OperationDialog::manage_cpp_exceptions(QString & s)
{
  int index;
  int ne = etable->nexceptions();
  const char * sep = " throw (";
	
  for (index = 0; index != ne; index += 1) {
    s += sep;
    s += GenerationSettings::cpp_type(type(etable->type(index), list, nodes));
    sep = ", ";
  }
  
  if (index != 0)
    s += ')';
  else if (GenerationSettings::cpp_oper_force_throw())
    s += " throw ()";
}

void OperationDialog::cpp_update_decl() {
  cpp_visibility.update_default(uml_visibility);
  
  // do NOT write
  //	const char * p = edcppdecl->text();
  // because the QString is immediatly destroyed !
  QString def = edcppdecl->text();
  const char * p = def;
  const char * pp = 0;
  QString indent = "";
  QString s;
  unsigned rank;

  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  if (*p != '#')
    s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      if (*p != '#')
	s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp,
		     GenerationSettings::cpp_javadoc_style());
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${friend}", 9)) {
      p += 9;
      if (friend_cb->isChecked())
	s += "friend ";
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (classoperation_cb->isChecked())
	s += "static ";
    }
    else if (!strncmp(p, "${inline}", 9)) {
      p += 9;
      if (inline_cb->isChecked())
	s += "inline ";
    }
    else if (!strncmp(p, "${virtual}", 10)) {
      p += 10;
      if (virtual_cb->isChecked())
	s += "virtual ";
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_cpp_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
				 list, nodes));
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += compute_name(edcppnamespec);
    }
    else if (!strncmp(p, "${class}", 8)) {
      // to be placed in the description
      p += 8;
      s += cl->get_browser_node()->get_name();
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
    }
    else if (!strncmp(p, "${const}", 8)) {
      p += 8;
      if (const_cb->isChecked())
	s += " const";
    }
    else if (!strncmp(p, "${volatile}", 11)) {
      p += 11;
      if (volatile_cb->isChecked())
	s += " volatile";
    }
    else if (!strncmp(p, "${throw}", 8)) {
      p += 8;
      manage_cpp_exceptions(s);
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (abstract_cb->isChecked())
	s += " = 0";
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      manage_cpp_type(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      manage_var(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (oper->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::cpp_relationattribute_stereotype(oper->stereotype);
    }
    else if (oper->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::cpp_type(((BrowserOperation *) oper->browser_node)
					->get_of_association().get_type());
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p && (*p != '#'))
	s += indent;
    }
    else if (*p == '@')
      manage_alias(oper->browser_node, p, s, kvtable);
    else
      s += *p++;
  }

  showcppdecl->setText(s);
}

QString OperationDialog::cpp_decl(const BrowserOperation * op, bool withname)
{
  OperationData * d = (OperationData *) op->get_data();
  QCString decl = d->cpp_decl;
  
  remove_comments(decl);
  remove_preprocessor(decl);
  
  const char * p = decl;
  bool afterparams = FALSE;
  QString s;
  unsigned rank;

  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${friend}", 9))
      p += 9;
    else if (!strncmp(p, "${static}", 9))
      p += 9;
    else if (!strncmp(p, "${inline}", 9))
      p += 9;
    else if (!strncmp(p, "${virtual}", 10))
      p += 10;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_cpp_name(d->return_type);
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += op->compute_name(d->cpp_name_spec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
      afterparams = TRUE;
    }
    else if (!strncmp(p, "${const}", 8))
      p += 8;
    else if (!strncmp(p, "${volatile}", 11))
      p += 11;
    else if (!strncmp(p, "${throw}", 8))
      p += 8;
    else if (!strncmp(p, "${abstract}", 11)) 
      break;
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      if (rank < d->nparams) 
	s += GenerationSettings::cpp_type(d->params[rank].get_type().get_type());
      else {
	s += "${t";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      if (withname) {
	if (rank < d->nparams) 
	  s += d->params[rank].get_name();
	else {
	  s += "${p";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (d->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::cpp_relationattribute_stereotype(d->stereotype);
    }
    else if (d->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::cpp_type(op->get_of_association().get_type());
    }
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if ((*p == '{') ||
	     (*p == ';') ||
	     ((*p == ':') && afterparams))
      break;
    else if (*p == '@')
      manage_alias(op, p, s, 0);
    else
      s += *p++;
  }

  return s;
}

void OperationDialog::cpp_default_def() {
  if (oper->is_get_or_set) {
    QCString decl;
    QCString def;
    
    if (get_of_attr != 0)
      oper->update_cpp_get_of(decl, def, get_of_attr->get_browser_node()->get_name(),
			      get_of_attr->get_cppdecl(),
			      get_of_attr->get_isa_const_attribute(),
			      QString::null);
    else if (set_of_attr != 0)
      oper->update_cpp_set_of(decl, def, set_of_attr->get_browser_node()->get_name(),
			      set_of_attr->get_cppdecl(),
			      set_of_attr->get_isa_const_attribute(),
			      QString::null);
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_cpp_get_of(decl, def, get_of_rel->get_role_a(), 
				get_of_rel->get_cppdecl_a(), 
				get_of_rel->get_isa_const_relation_a(), 
				get_of_rel->get_multiplicity_a());
      else
	oper->update_cpp_get_of(decl, def, get_of_rel->get_role_b(), 
				get_of_rel->get_cppdecl_b(), 
				get_of_rel->get_isa_const_relation_b(), 
				get_of_rel->get_multiplicity_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_cpp_set_of(decl, def, set_of_rel->get_role_a(), 
				set_of_rel->get_cppdecl_a(), 
				set_of_rel->get_isa_const_relation_a(), 
				set_of_rel->get_multiplicity_a());
      else
	oper->update_cpp_set_of(decl, def, set_of_rel->get_role_b(), 
				set_of_rel->get_cppdecl_b(), 
				set_of_rel->get_isa_const_relation_b(), 
				set_of_rel->get_multiplicity_b());
    }

    edcppdef->setText(def);
  }
  else {
    QString s = oper->default_cpp_def(edname->text().stripWhiteSpace());
    
    GenerationSettings::set_cpp_return_type(the_type(edreturn_type->currentText()
						     .stripWhiteSpace(),
						     list, nodes),
					    s);
    
    QString params;
    int index;
    int nparams = (int) table->nparams();
    QString sep;
    
    for (index = 0; index != nparams; index += 1, sep = ", ")
      params += sep + GenerationSettings::cpp(the_type(table->type(index),
						       list, nodes),
					      table->dir(index), index);
    
    if ((index = s.find("${)}")) != -1)
      s.insert(index, params);
    
    edcppdef->setText(s);
  }
  cpp_update_def();
}

void OperationDialog::cpp_unmapped_def() {
  edcppdef->setText(QString::null);
  showcppdef->setText(QString::null);
}

void OperationDialog::cpp_def_from_decl() {
  
  QString dcl = edcppdecl->text();
  QString def;
  
  // manage abstract
  if ((dcl.find("${abstract}") == -1) || // abstract removed
      !abstract_cb->isChecked()) {
    // probably not abstract
    int index1;
    int index2;
    int index3;
    int index4;
    
    def = oper->default_cpp_def(edname->text().stripWhiteSpace());
    
    // copy args
    if (((index1 = dcl.find("${(}") + 4) == (-1 + 4)) ||
	((index2 = dcl.find("${)}", index1)) == -1) ||
	((index3 = def.find("${(}") + 4) == (-1 + 4)) ||
	((index4 = def.find("${)}", index3)) == -1))
      // wrong specification(s)
      return;
    
    if (index4 != index3)
      def.remove(index3, index4 - index3);
    if (index2 != index1)
      def.insert(index3, dcl.mid(index1, index2 - index1));
    
    // copy return type modifications
    if (((index1 = dcl.find("${type}")) != -1) &&
	((index2 = dcl.find("${name}", index1 + 7)) != -1) &&
	((index3 = def.find("${type}")) != -1) &&
	((index4 = def.find("${class}", index3 + 7)) != -1)) {
      // valid specifications
      while ((index1 != 0) && (dcl.at(index1 - 1) != QChar('}')))
	index1 -= 1;
      while ((index3 != 0) && (def.at(index3 - 1) != QChar('}')))
	index3 -= 1;
      def.remove(index3, index4 - index3);
      def.insert(index3, dcl.mid(index1, index2 - index1));
    }
    
    // manage const
    if ((dcl.find("${const}") == -1) && // const removed
	((index1 = def.find("${const}")) != -1))
      // remove const
      def.remove(index1, 8);
    
    // manage throw
    if ((dcl.find("${throw}") == -1) && // throws removed
	((index1 = def.find("${throw}")) != -1))
      // remove throw
      def.remove(index1, 8);
  }
  
  // update def
  edcppdef->setText(def);
  cpp_update_def();
}

void OperationDialog::cpp_edit_param_def() {
  QString form = edcppdef->text();
  int index;

  if (((index = form.find("${(}")) != 0) &&
      (form.find("${)}", index + 4) != 0)) {
    CppParamsDialog d(table, edcppdef);
    
    d.raise();
    if (d.exec() == QDialog::Accepted)
      cpp_update_def();
  }
  else
    msg_warning("Bouml", "wrong specification");
}

// return TRUE if stop on ${commnt} or ${description}
bool insert_template(const QString & tm, const char *& p, 
		     QString & s, const QString & indent)
{
  // search the beginning of the definition/declaration in p;
  for (;;) {
    if (*p == 0)
      return FALSE;
    
    if (*p == '\n') {
      s += *p++;
      if (*p != '#')
	s += indent;
    }
    if (*p <= ' ')
      s += *p++;
    else if (*p == '/') {
      if (p[1] == '/') {
	// comment
	s += *p++;
	do s += *p++;
	while (*p && (*p != '\n'));
      }
      else if (p[1] == '*') {
	/* comment */
	s += *p++;
	do s += *p++;
	while (*p && ((*p != '*') || (p[1] != '/')));
	s += "*/";
	p += 2;
      }
      else
	break;
    }
    else if (*p == '#') {
      do {
	s += *p++;
	if (*p == '\\') {
	  s += *p++;
	  s += *p++;
	}
	else if ((*p == '/') && (p[1] == '*')) {
	  /* comment */
	  s += *p++;
	  do s += *p++;
	  while (*p && ((*p != '*') || (p[1] != '/')));
	  s += "*/";
	  p += 2;
	}
      } while (*p && (*p != '\n'));
    }
    else if ((strncmp(p, "${comment}", 10) == 0) ||
	     (strncmp(p, "${description}", 14) == 0))
      return TRUE;
    else
      break;
  }
  
  s += tm;
  return FALSE;
}

void OperationDialog::cpp_update_def() {
  QString def = edcppdef->text();
  bool template_oper = edcppdecl->text().isEmpty()
    && oper->is_template_operation(def);
  QString s;
  
  if (!abstract_cb->isChecked()) {
    // do NOT write
    //	const char * p = edcppdef->text()
    // because the QString is immediatly destroyed !
    const char * p = def;
    const char * pp = 0;
    QString indent = "";
    unsigned rank;
    
    while ((*p == ' ') || (*p == '\t'))
      indent += *p++;
    
    bool re_template = !templates.isEmpty() &&
      insert_template((template_oper) ? templates_tmplop : templates, 
		      p, s, indent);
    
    if (*p != '#')
      s += indent;
  
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;

	if (re_template)
	  s += templates;

	if (*p == 0)
	  break;
	if (*p != '#')
	  s += indent;
      }
      
      if (!strncmp(p, "${comment}", 10)) {
	if (!manage_comment(comment->text(), p, pp,
			    GenerationSettings::cpp_javadoc_style())
	    && re_template)
	  s += templates;
      }
      else if (!strncmp(p, "${description}", 14)) {
	if (!manage_description(comment->text(), p, pp) && re_template)
	  s += templates;
      }
      else if (!strncmp(p, "${inline}", 9)) {
	p += 9;
	if (inline_cb->isChecked())
	  s += "inline ";
      }
      else if (!strncmp(p, "${type}", 7)) {
	p += 7;
	s += get_cpp_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
				   list, nodes));
      }
      else if (!strncmp(p, "${class}", 8)) {
	p += 8;
	if (friend_cb->isChecked() && !strncmp(p, "::", 2))
	  p += 2;
	else
	  s += ((template_oper) ? cl_names_tmplop : cl_names);
      }
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	s += compute_name(edcppnamespec);
      }
      else if (!strncmp(p, "${(}", 4)) {
	p += 4;
	s += '(';
      }
      else if (!strncmp(p, "${)}", 4)) {
	p += 4;
	s += ')';
      }
      else if (!strncmp(p, "${const}", 8)) {
	p += 8;
	if (const_cb->isChecked())
	  s += " const";
      }
      else if (!strncmp(p, "${volatile}", 11)) {
	p += 11;
	if (volatile_cb->isChecked())
	  s += " volatile";
      }
      else if (!strncmp(p, "${throw}", 8)) {
	p += 8;
      manage_cpp_exceptions(s);
      }
      else if (!strncmp(p, "${staticnl}", 11)) {
	p += 11;
	if (classoperation_cb->isChecked()) {
	  s += '\n';
	  s += indent;
	}
	else
	  s += ' ';
      }
      else if (sscanf(p, "${t%u}", &rank) == 1) {
	manage_cpp_type(rank, s);
	p = strchr(p, '}') + 1;
      }
      else if (sscanf(p, "${p%u}", &rank) == 1) {
	manage_var(rank, s);
	p = strchr(p, '}') + 1;
      }
      else if (oper->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
	p += 13;
	// get/set with multiplicity > 1
	s += GenerationSettings::cpp_relationattribute_stereotype(oper->stereotype);
      }
      else if (oper->is_get_or_set && !strncmp(p, "${association}", 14)) {
	p += 14;
	// get/set with multiplicity > 1
	s += GenerationSettings::cpp_type(((BrowserOperation *) oper->browser_node)
					  ->get_of_association().get_type());
      }
      else if (*p == '\n') {
	s += *p++;
	if (*p && (*p != '#'))
	  s += indent;
      }
      else if (*p == '@')
	manage_alias(oper->browser_node, p, s, kvtable);
      else
	s += *p++;
    }
    
    editcppbody->setEnabled(def.find("${body}") != -1);
  }
  else
    editcppbody->setEnabled(FALSE);
  
  showcppdef->setText(s);
}

static QString add_profile(QString b)
{
  b.insert(0, "##\t");

  int index = 3;
  int index2;

  while ((index2 = b.find('\n', index)) != -1) {
    b.insert(index2 + 1, "##\t");
    index = index2 + 4;
  }

  b += "\n##\t---- these lines will be automatically removed ----\n";

  return b;
}

static QString remove_profile(QString b)
{
  while ((b.length() > 3) && (b.mid(0, 3) == "##\t")) {
    int index = b.find('\n', 3);

    if (index == -1)
      return QString::null;

    b.remove(0, index + 1);
  }

  return b;
}

void OperationDialog::cpp_edit_body() {
  QString b;

  if (add_operation_profile())
    b = add_profile(showcppdef->text()) + cppbody;
  else
    b = cppbody;

  edit(b, edname->text().stripWhiteSpace() + "_body",
       oper, CppEdit, this, 
       (preserve_bodies()) ? (post_edit) 0
			   : (post_edit) post_cpp_edit_body,
       edits);
}

void OperationDialog::post_cpp_edit_body(OperationDialog * d, QString s)
{
  d->cppbody = (add_operation_profile()) ? remove_profile(s) : s;
}

// Java

void OperationDialog::finalsynchronized_toggled(bool) {
  java_update_def();
}

void OperationDialog::manage_java_type(unsigned rank, QString & s)
{
  if (rank < table->nparams()) 
    s += get_java_name(the_type(table->type(rank), list, nodes));
  else {
    s += "${t";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::manage_java_exceptions(QString & s)
{
  int index;
  int ne = etable->nexceptions();
  const char * sep = " throws ";
	
  for (index = 0; index != ne; index += 1) {
    s += sep;
    s += GenerationSettings::java_type(type(etable->type(index), list, nodes));
    sep = ", ";
  }
  
  if (index != 0)
    s += ' ';
}

void OperationDialog::java_default_def() {
  if (oper->is_get_or_set) {
    QCString def;
    
    if (get_of_attr != 0)
      oper->update_java_get_of(def, get_of_attr->get_browser_node()->get_name(),
			       get_of_attr->get_javadecl());
    else if (set_of_attr != 0)
      oper->update_java_set_of(def, set_of_attr->get_browser_node()->get_name(),
			       set_of_attr->get_javadecl());
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_java_get_of(def, get_of_rel->get_role_a(), 
				 get_of_rel->get_javadecl_a());
      else
	oper->update_java_get_of(def, get_of_rel->get_role_b(), 
				 get_of_rel->get_javadecl_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_java_set_of(def, set_of_rel->get_role_a(), 
				 set_of_rel->get_javadecl_a());
      else
	oper->update_java_set_of(def, set_of_rel->get_role_b(), 
				 set_of_rel->get_javadecl_b());
    }

    edjavadef->setText(def);
  }
  else {
    QString s = oper->default_java_def(edname->text().stripWhiteSpace());
    QString params;
    int index;
    int nparams = (int) table->nparams();
    const char * sep;
    
    for (index = 0, sep = ""; index != nparams; index += 1, sep = ", ") {
      QString p;
      
      p.sprintf("%s${t%d} ${p%d}", sep, index, index);
      params += p;
    }
    
    if ((index = s.find("${)}")) != -1)
      s.insert(index, params);
    
    edjavadef->setText(s);
  }
  
  java_update_def();
}

void OperationDialog::java_unmapped_def() {
  edjavadef->setText(QString::null);
  showjavadef->setText(QString::null);
}

void OperationDialog::java_update_def() {
  // do NOT write
  //	const char * p = edjavadef->text();
  // because the QString is immediatly destroyed !
  QString def = edjavadef->text();
  QString ste = GenerationSettings::java_class_stereotype(cl->get_stereotype());
  bool interf = (ste == "interface") || (ste == "@interface");
  bool nobody = (abstract_cb->isChecked() || interf);
  const char * p = def;
  const char * pp = 0;
  const char * afterparam = 0;
  QString indent = "";
  QString s;
  unsigned rank;
  
  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp,
		     GenerationSettings::java_javadoc_style());
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (final_cb->isChecked())
	s += "final ";
    }
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      if (uml_visibility.value() != UmlPackageVisibility)
	s += uml_visibility.state() + ' ';
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (classoperation_cb->isChecked())
	s += "static ";
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (abstract_cb->isChecked() && !interf)
	s += "abstract ";
    }
    else if (!strncmp(p, "${synchronized}", 15)) {
      p += 15;
      if (synchronized_cb->isChecked())
	s += "synchronized ";
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_java_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
				  list, nodes));
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += compute_name(edjavanamespec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      afterparam = p;
      s += ')';
    }
    else if (!strncmp(p, "${throws}", 9)) {
      manage_java_exceptions(s);
      p += 9;
    }
    else if (!strncmp(p, "${staticnl}", 11)) {
      p += 11;
      if (classoperation_cb->isChecked()) {
	s += '\n';
	s += indent;
      }
      else
	s += ' ';
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      manage_java_type(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      manage_var(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (oper->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::java_relationattribute_stereotype(oper->stereotype);
    }
    else if (oper->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::java_type(((BrowserOperation *) oper->browser_node)
					 ->get_of_association().get_type());
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p)
	s += indent;
    }
    else if ((*p == '{') && nobody) {
      if ((afterparam != 0) &&
	  (ste == "@interface") &&
	  (strstr(afterparam, "default") != 0)){
	s += '{';
	p += 1;
      }
      else {
	s += ";";
	break;
      }
    }
    else if (!strncmp(p, "${@}", 4)) {
      p += 4;
      if (pp != 0)
	s += "${@}";
      else if (! javaannotation.isEmpty()) {
	pp =p;
	p = javaannotation;
      }
    }
    else if (*p == '@')
      manage_alias(oper->browser_node, p, s, kvtable);
    else
      s += *p++;
  }
    
  editjavabody->setEnabled(!nobody && (def.find("${body}") != -1));
  editjavaannotation->setEnabled(def.find("${@}") != -1);  
  
  showjavadef->setText(s);
}

QString OperationDialog::java_decl(const BrowserOperation * op, bool withname)
{
  OperationData * d = (OperationData *) op->get_data();
  QCString decl = d->java_def;
  
  remove_comments(decl);
  
  const char * p = decl;
  QString s;
  unsigned rank;

  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${final}", 8))
      p += 8;
    else if (!strncmp(p, "${visibility}", 13))
      p += 13;
    else if (!strncmp(p, "${static}", 9))
      p += 9;
    else if (!strncmp(p, "${abstract}", 11))
      p += 11;
    else if (!strncmp(p, "${synchronized}", 15))
      p += 15;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_java_name(d->return_type);
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += op->compute_name(d->java_name_spec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
    }
    else if (!strncmp(p, "${throws}", 9))
      p += 9;
    else if (!strncmp(p, "${staticnl}", 11)) {
      p += 11;
      s += ' ';
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      if (rank < d->nparams) 
	s += GenerationSettings::java_type(d->params[rank].get_type().get_type());
      else {
	s += "${t";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      if (withname) {
	if (rank < d->nparams) 
	  s += d->params[rank].get_name();
	else {
	  s += "${p";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (!strncmp(p, "${@}", 4))
      p += 4;
    else if (d->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::java_relationattribute_stereotype(d->stereotype);
    }
    else if (d->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::java_type(op->get_of_association().get_type());
    }
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if ((*p == '{') || (*p == ';'))
      break;
    else if (*p == '@')
      manage_alias(op, p, s, 0);
    else
      s += *p++;
  }
  
  return s;
}

void OperationDialog::java_edit_body() {
  QString b;

  if (add_operation_profile())
    b = add_profile(showjavadef->text()) + javabody;
  else
    b = javabody;

  edit(b, edname->text().stripWhiteSpace() + "_body",
       oper, JavaEdit, this, 
       (preserve_bodies()) ? (post_edit) 0
			   : (post_edit) post_java_edit_body,
       edits);
}

void OperationDialog::post_java_edit_body(OperationDialog * d, QString s)
{
  d->javabody = (add_operation_profile()) ? remove_profile(s) : s;
}

void OperationDialog::java_edit_annotation() {
  AnnotationDialog dialog(javaannotation, !hasOkButton());
  
  if (dialog.exec() == QDialog::Accepted)
    java_update_def();
}
  
// Idl

void OperationDialog::oneway_toggled(bool) {
  idl_update_decl();
}

void OperationDialog::manage_idl_type(unsigned rank, QString & s)
{
  if (rank < table->nparams()) 
    s += get_java_name(the_type(table->type(rank), list, nodes));
  else {
    s += "${t";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::manage_dir(unsigned rank, QString & s)
{
  if (rank < table->nparams()) {
    switch (table->dir(rank)) {
    case UmlIn:
      s += "in";
      break;
    case UmlOut:
      s += "out";
      break;
    default:
      // can't be return
      s += "inout";
    }
  }
  else {
    s += "${d";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::manage_idl_exceptions(QString & s)
{
  int index;
  int ne = etable->nexceptions();
  const char * sep = " raises (";
	
  for (index = 0; index != ne; index += 1) {
    s += sep;
    s += GenerationSettings::idl_type(type(etable->type(index), list, nodes));
    sep = ", ";
  }
  
  if (index != 0)
    s += ')';
}

void OperationDialog::idl_default_def() {
  if (oper->is_get_or_set) {
    QCString decl;
    
    if (get_of_attr != 0)
      oper->update_idl_get_of(decl, get_of_attr->get_idldecl(), QString::null);
    else if (set_of_attr != 0)
      oper->update_idl_set_of(decl, set_of_attr->get_idldecl(), QString::null);
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_idl_get_of(decl, get_of_rel->get_idldecl_a(),
				get_of_rel->get_multiplicity_a());
      else
	oper->update_idl_get_of(decl, get_of_rel->get_idldecl_b(),
				get_of_rel->get_multiplicity_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_idl_set_of(decl, set_of_rel->get_idldecl_a(),
				set_of_rel->get_multiplicity_a());
      else
	oper->update_idl_set_of(decl, set_of_rel->get_idldecl_b(),
				set_of_rel->get_multiplicity_b());
    }

    edidldecl->setText(decl);
  }
  else {
    QString s = oper->default_idl_decl(edname->text().stripWhiteSpace());
    QString params;
    int index;
    int nparams = (int) table->nparams();
    const char * sep;
    
    for (index = 0, sep = ""; index != nparams; index += 1, sep = ", ") {
      QString p;
      
      p.sprintf("%s${d%d} ${t%d} ${p%d}", sep, index, index, index);
      params += p;
    }
    
    if ((index = s.find("${)}")) != -1)
      s.insert(index, params);
    
    edidldecl->setText(s);
  }
  
  idl_update_decl();
}

void OperationDialog::idl_unmapped_def() {
  edidldecl->setText(QString::null);
  showidldecl->setText(QString::null);
}

void OperationDialog::idl_update_decl() {  
  // do NOT write
  //	const char * p = edidldecl->text();
  // because the QString is immediatly destroyed !
  QString def = edidldecl->text();
  const char * p = def;
  const char * pp = 0;
  QString indent = "";
  QString s;
  unsigned rank;
  
  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp, FALSE);
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${oneway}", 9)) {
      p += 9;
      if (oneway_cb->isChecked())
	s += "oneway ";
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_idl_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
				 list, nodes));
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += compute_name(edidlnamespec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
    }
    else if (sscanf(p, "${d%u}", &rank) == 1) {
      manage_dir(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      manage_idl_type(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      manage_var(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (oper->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::idl_relationattribute_stereotype(oper->stereotype);
    }
    else if (oper->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::idl_type(((BrowserOperation *) oper->browser_node)
					->get_of_association().get_type());
    }
    else if (!strncmp(p, "${raisesnl}", 11)) {
      if (etable->nexceptions() != 0) {
	s += '\n';
	s += indent;
	s += "  ";
      }
      p += 11;
    }
    else if (!strncmp(p, "${raises}", 9)) {
      manage_idl_exceptions(s);
      p += 9;
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p)
	s += indent;
    }
    else if (*p == '@')
      manage_alias(oper->browser_node, p, s, kvtable);
    else
      s += *p++;
  }
  
  showidldecl->setText(s);
}

QString OperationDialog::idl_decl(const BrowserOperation * op,
				  bool withdir, bool withname)
{
  OperationData * d = (OperationData *) op->get_data();
  QCString decl = d->idl_decl;
  
  remove_comments(decl);
  
  const char * p = decl;
  QString s;
  unsigned rank;

  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${oneway}", 9))
      p += 9;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_java_name(d->return_type);
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += op->compute_name(d->idl_name_spec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
    }
    else if (sscanf(p, "${d%u}", &rank) == 1) {
      if (withdir) {
	if (rank < d->nparams) 
	  switch (d->params[rank].get_dir()) {
	  case UmlIn:
	    s += "in";
	    break;
	  case UmlOut:
	    s += "out";
	    break;
	  default:
	    // can't be return
	    s += "inout";
	}
	else {
	  s += "${d";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      if (withname) {
	if (rank < d->nparams) 
	  s += GenerationSettings::idl_type(d->params[rank].get_type().get_type());
	else {
	  s += "${t";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      if (rank < d->nparams) 
	s += d->params[rank].get_name();
      else {
	s += "${p";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (d->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::idl_relationattribute_stereotype(d->stereotype);
    }
    else if (d->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::idl_type(op->get_of_association().get_type());
    }
    else if (!strncmp(p, "${raisesnl}", 11)) {
      p += 11;
      s += ' ';
    }
    else if (!strncmp(p, "${raises}", 9))
      p += 9;
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if (*p == ';')
      break;
    else if (*p == '@')
      manage_alias(op, p, s, 0);
    else
      s += *p++;
  }
  
  return s;
}

//
// ParamTable
//

// copy/cut/paste
QString ParamsTable::dir_copy;
QString ParamsTable::name_copy;
QString ParamsTable::type_copy;
QString ParamsTable::default_value_copy;

static QStringList DirList;

ParamsTable::ParamsTable(OperationData * o, QWidget * parent,
			 const QStringList & list, bool visit)
    : MyTable(o->get_n_params() + 1, (visit) ? 4 : 5, parent), types(list) {
  int index;
  int sup = o->get_n_params();
    
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, "Direction");
  horizontalHeader()->setLabel(1, "Name");
  horizontalHeader()->setLabel(2, "Type");
  horizontalHeader()->setLabel(3, "Default value");
  
  if (visit) {
    for (index = 0; index < sup; index += 1){
      setItem(index, 0, new TableItem(this, QTableItem::Never, stringify(o->get_param_dir(index))));
      setItem(index, 1, new TableItem(this, QTableItem::Never, o->get_param_name(index)));
      setItem(index, 2, new TableItem(this, QTableItem::Never, o->get_param_type(index).get_full_type()));
      setItem(index, 3, new TableItem(this, QTableItem::Never, o->get_param_default_value(index)));
    }
  }
  else {
    horizontalHeader()->setLabel(4, "do");
    
    alltypes = GenerationSettings::basic_types();
    for (QStringList::ConstIterator it = list.begin(); it != list.end(); ++it)
      alltypes.append(*it);
    
    if (DirList.count() == 0) {
      // Made DirList without be linked with the items order
      DirList.append(QString::null);
      DirList.append(QString::null);
      DirList.append(QString::null);
      DirList[UmlInOut] = stringify(UmlInOut);
      DirList[UmlIn] = stringify(UmlIn);
      DirList[UmlOut] = stringify(UmlOut);
      // can't be return
    }
    
    for (index = 0; index < sup; index += 1){
      setItem(index, 0, new ComboItem(this, stringify(o->get_param_dir(index)), DirList, FALSE));
      setText(index, 1, o->get_param_name(index));
      setItem(index, 2, new ComboItem(this, o->get_param_type(index).get_full_type(), alltypes));
      setText(index, 3, o->get_param_default_value(index));
      setText(index, 4, QString::null);
    }
    
    setItem(index, 0, new ComboItem(this, stringify(UmlInOut), DirList, FALSE));
    setText(index, 1, QString::null);
    setItem(index, 2, new ComboItem(this, QString::null, alltypes));
    setText(index, 3, QString::null);
    setText(index, 4, QString::null);
  }
  
  adjustColumn(0);
  //setColumnStretchable (1, TRUE);
  horizontalHeader()->setResizeEnabled(TRUE, 1);
  setColumnStretchable (2, TRUE);
  //setColumnStretchable (3, TRUE);
  horizontalHeader()->setResizeEnabled(TRUE, 3);
  
  if (! visit) {
    adjustColumn(4);
    setColumnStretchable (4, FALSE);
    
    connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	    this, SLOT(button_pressed(int, int, int, const QPoint &)));
    connect(this, SIGNAL(valueChanged(int, int)),
	    this, SLOT(value_changed(int, int)));
  }
}

void ParamsTable::activateNextCell() {
  int row = currentRow();
  int col = currentColumn();
    
  if (col == 3) {
    // goto next line
    if (++row == numRows()) {
      // adds a new line
      setNumRows(row + 1);
      setItem(row, 0, new ComboItem(this, stringify(UmlInOut), DirList, FALSE));
      setText(row, 1, QString::null);
      setItem(row, 2, new ComboItem(this, QString::null, alltypes));
      setText(row, 3, QString::null);
      setText(row, 4, QString::null);
    }
    setCurrentCell(row, 0);
  }
  else
    setCurrentCell(row, col + 1);
}

void ParamsTable::value_changed(int row, int col) {
  if ((row == (numRows() - 1)) && (col != 0) && !text(row, col).isEmpty())
    insert_row_after(row);
}

void ParamsTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == 4) {
    char s[16];
    
    sprintf(s, "param %d", row + 1);
    
    QPopupMenu m;
    m.insertItem(s, -1);
    m.insertSeparator();
    m.insertItem("Insert param before", 0);
    m.insertItem("Insert param after", 1);
    m.insertSeparator();
    m.insertItem("Delete param", 2);
    m.insertSeparator();
    m.insertItem("Copy param", 3);
    m.insertItem("Cut param", 4);
    m.insertItem("Paste param", 5);
    m.insertSeparator();

    QPopupMenu mv;
    int rank;
    
    for (rank = 0; rank != numRows(); rank += 1)
      if (rank != row)
	mv.insertItem(QString::number(rank + 1), 10 + rank);
    
    m.insertItem("Move param", &mv);
    
    switch (rank = m.exec(QCursor::pos())) {
    case 0:
      insert_row_before(row);
      break;
    case 1:
      insert_row_after(row);
      break;
    case 2:
      delete_row(row);
      break;
    case 3:
      copy_row(row);
      break;
    case 4:
      cut_row(row);
      break;
    case 5:
      paste_row(row);
      break;
    default:
      if (rank >= 10)
	move_row(row, rank - 10);
      break;
    }
  }
}

void ParamsTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    QTableItem * it;
    
    it = item(index - 1, 0);
    takeItem(it);
    setItem(index, 0, it);
    setText(index, 1, text(index - 1, 1));
    it = item(index - 1, 2);
    takeItem(it);
    setItem(index, 2, it);
    setText(index, 3, text(index - 1, 3));
  }
  
  setItem(row, 0, new ComboItem(this, stringify(UmlInOut), DirList, FALSE));
  setText(row, 1, QString::null);
  setItem(row, 2, new ComboItem(this, QString::null, alltypes));
  setText(row, 3, QString::null);
  setText(row, 4, QString::null);
 }

void ParamsTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    QTableItem * it;
    
    it = item(index - 1, 0);
    takeItem(it);
    setItem(index, 0, it);
    setText(index, 1, text(index - 1, 1));
    it = item(index - 1, 2);
    takeItem(it);
    setItem(index, 2, it);
    setText(index, 3, text(index - 1, 3));
  }
  
  setItem(row + 1, 0, new ComboItem(this, stringify(UmlInOut), DirList, FALSE));
  setText(row + 1, 1, QString::null);
  setItem(row + 1, 2, new ComboItem(this, QString::null, alltypes));
  setText(row + 1, 3, QString::null);
  setText(row + 1, 4, QString::null);
 }

void ParamsTable::delete_row(int row) {
  int n = numRows();
  int index;

  clearCellWidget(row, 1);
    
  if (row == (n - 1)) {
    // the last line : empty it
    setItem(row, 0, new ComboItem(this, stringify(UmlInOut), DirList, FALSE));
    setText(row, 1, QString::null);
    setItem(row, 2, new ComboItem(this, QString::null, alltypes));
    setText(row, 3, QString::null);
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      QTableItem * it;
      
      it = item(index + 1, 0);
      takeItem(it);
      setItem(index, 0, it);
      setText(index, 1, text(index + 1, 1));
      it = item(index + 1, 2);
      takeItem(it);
      setItem(index, 2, it);
      setText(index, 3, text(index + 1, 3));
    }
    setNumRows(n - 1);
  }
}

void ParamsTable::copy_row(int row) {
  dir_copy = text(row, 0);
  name_copy = text(row, 1);
  type_copy = text(row, 2);
  default_value_copy = text(row, 3);
}

void ParamsTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void ParamsTable::paste_row(int row) {
  setText(row, 0, dir_copy);
  setText(row, 1, name_copy);
  setText(row, 2, type_copy);
  setText(row, 3, default_value_copy);
  
  if ((row == (numRows() - 1)) &&
      (!dir_copy.isEmpty() ||
       !name_copy.isEmpty() ||
       !type_copy.isEmpty()))
    insert_row_after(row);
}

void ParamsTable::move_row(int from, int to) {
  QString save_dir_copy = dir_copy;
  QString save_name_copy = name_copy;
  QString save_type_copy = type_copy;
  QString save_default_value_copy = default_value_copy;
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  dir_copy = save_dir_copy;
  name_copy = save_name_copy;
  type_copy = save_type_copy;
  default_value_copy = save_default_value_copy;
}

void ParamsTable::update(OperationData * oper,
			 BrowserNodeList & nodes) {
  forceUpdateCells();
  
  int n = numRows();
  int index;
  
  if (text(n - 1, 1).isEmpty())
    n -= 1;
  
  oper->set_n_params(n);
  
  for (index = 0; index != n; index += 1) {    
    oper->set_param_dir(index, (UmlParamDirection) DirList.findIndex(text(index, 0)));
			
    oper->set_param_name(index, text(index, 1).stripWhiteSpace());
    
    AType t;
    QString s = text(index, 2).stripWhiteSpace();
    
    if (! s.isEmpty()) {
      int rank = types.findIndex(s);
      
      if (rank != -1)
	t.type = (BrowserClass *) nodes.at(rank);
      else
	t.explicit_type = s;
    }
     
    oper->set_param_type(index, t);
    
    oper->set_param_default_value(index, text(index, 3).stripWhiteSpace());
  }
}

unsigned ParamsTable::nparams() const {
  int n = numRows();
  
  return (text(n - 1, 1).isEmpty()) ? n -= 1 : n;
}

QString ParamsTable::name(unsigned rank) const {
  return text(rank, 1);
}

QString ParamsTable::type(unsigned rank) const {
  return text(rank, 2);
}

UmlParamDirection ParamsTable::dir(unsigned rank) const {
  return (UmlParamDirection) DirList.findIndex(text(rank, 0));
}

QString ParamsTable::value(unsigned rank) const {
  return text(rank, 3);
}

//
// ExceptionTable
//

// copy/cut/paste
QString ExceptionsTable::type_copy;

ExceptionsTable::ExceptionsTable(OperationData * o, QWidget * parent,
				 const QStringList & list, bool visit)
    : MyTable(o->get_n_exceptions() + 1, (visit) ? 1 : 2, parent), types(list) {
  int index;
  int sup = o->get_n_exceptions();
  
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, "Type");
  
  if (visit) {
    for (index = 0; index < sup; index += 1)
      setItem(index, 0, new TableItem(this, QTableItem::Never, o->get_exception(index).get_full_type()));
  }
  else {
    horizontalHeader()->setLabel(1, "do");
    
    for (index = 0; index < sup; index += 1){
      setItem(index, 0, new ComboItem(this, o->get_exception(index).get_full_type(), types));
      setText(index, 1, QString::null);
    }
    
    setItem(index, 0, new ComboItem(this, QString::null, types));
    setText(index, 1, QString::null);
  }
  
  setColumnStretchable (0, TRUE);
  
  if (! visit) {
    adjustColumn(1);
    setColumnStretchable (1, FALSE);
    
    connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	    this, SLOT(button_pressed(int, int, int, const QPoint &)));
    connect(this, SIGNAL(valueChanged(int, int)),
	    this, SLOT(value_changed(int, int)));
  }
}

void ExceptionsTable::activateNextCell() {
  int row = currentRow();
    
  // goto next line
  if (++row == numRows()) {
    // adds a new line
    setNumRows(row + 1);
    setItem(row, 0, new ComboItem(this, QString::null, types));
    setText(row, 1, QString::null);
  }
  setCurrentCell(row, 0);
}

void ExceptionsTable::value_changed(int row, int col) {
  if ((row == (numRows() - 1)) && !text(row, col).isEmpty())
    insert_row_after(row);
}

void ExceptionsTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == 1) {
    char s[16];
    
    sprintf(s, "exception %d", row + 1);
    
    QPopupMenu m;
    m.insertItem(s, -1);
    m.insertSeparator();
    m.insertItem("Insert exception before", 0);
    m.insertItem("Insert exception after", 1);
    m.insertSeparator();
    m.insertItem("Delete exception", 2);
    m.insertSeparator();
    m.insertItem("Copy exception", 3);
    m.insertItem("Cut exception", 4);
    m.insertItem("Paste exception", 5);
    m.insertSeparator();

    QPopupMenu mv;
    int rank;
    
    for (rank = 0; rank != numRows(); rank += 1)
      if (rank != row)
	mv.insertItem(QString::number(rank + 1), 10 + rank);
    
    m.insertItem("Move exception", &mv);
    
    switch (rank = m.exec(QCursor::pos())) {
    case 0:
      insert_row_before(row);
      break;
    case 1:
      insert_row_after(row);
      break;
    case 2:
      delete_row(row);
      break;
    case 3:
      copy_row(row);
      break;
    case 4:
      cut_row(row);
      break;
    case 5:
      paste_row(row);
      break;
    default:
      if (rank >= 10)
	move_row(row, rank - 10);
      break;
    }
  }
}

void ExceptionsTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    QTableItem * it = item(index - 1, 0);
    
    takeItem(it);
    setItem(index, 0, it);
  }
  
  setItem(row, 0, new ComboItem(this, QString::null, types));
  setText(row, 1, QString::null);
 }

void ExceptionsTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    QTableItem * it = item(index - 1, 0);
    
    takeItem(it);
    setItem(index, 0, it);
  }
  
  setItem(row + 1, 0, new ComboItem(this, QString::null, types));
  setText(row + 1, 1, QString::null);
 }

void ExceptionsTable::delete_row(int row) {
  int n = numRows();
  int index;

  clearCellWidget(row, 1);
    
  if (row == (n - 1)) {
    // the last line : empty it
    setItem(0, 0, new ComboItem(this, QString::null, types));
    setText(0, 1, QString::null);
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      QTableItem * it = item(index + 1, 0);
      takeItem(it);
      setItem(index, 0, it);
    }
    setNumRows(n - 1);
  }
}

void ExceptionsTable::copy_row(int row) {
  type_copy = text(row, 0);
}

void ExceptionsTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void ExceptionsTable::paste_row(int row) {
  setText(row, 0, type_copy);
  
  if ((row == (numRows() - 1)) && !type_copy.isEmpty())
    insert_row_after(row);
}

void ExceptionsTable::move_row(int from, int to) {
  QString save_type_copy = type_copy;
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  type_copy = save_type_copy;
}

void ExceptionsTable::update(OperationData * oper,
			     BrowserNodeList & nodes) {
  forceUpdateCells();
  
  int n = numRows();
  int index;
  
  if (text(n - 1, 0).isEmpty())
    n -= 1;
  
  oper->set_n_exceptions(n);
  
  for (index = 0; index != n; index += 1) {   
    AType t;
    QString s = text(index, 0).stripWhiteSpace();
    
    if (!s.isEmpty()) {
      int rank = types.findIndex(s);
    
      if (rank != -1) 
	t.type = (BrowserClass *) nodes.at(rank);
      else
	t.explicit_type = s;
    }
    oper->set_exception(index, t);
  }
}

unsigned ExceptionsTable::nexceptions() const {
  int n = numRows();
  
  return (text(n - 1, 0).isEmpty()) ? n -= 1 : n;
}

QString ExceptionsTable::type(unsigned rank) const {
  return text(rank, 0);
}

//
// CppParamTable
//

// copy/cut/paste
QString CppParamsTable::copied[7];	// copy/cut/paste

static QStringList SpecifierList;
static QStringList TypeRankList;
static QStringList PtrList;
static QStringList ParamRankList;

CppParamsTable::CppParamsTable(ParamsTable * p, MultiLineEdit * f, QWidget * parent)
    : MyTable(0, 7, parent), params(p), edform(f) {
    
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, "Name");
  horizontalHeader()->setLabel(1, "Specifier");
  horizontalHeader()->setLabel(2, "${t<i>}");
  horizontalHeader()->setLabel(3, "Pointer");
  horizontalHeader()->setLabel(4, "${p<i>}");
  horizontalHeader()->setLabel(5, "Modifier");
  horizontalHeader()->setLabel(6, "do");
  setColumnStretchable (0, TRUE);
  setColumnStretchable (1, TRUE);
  setColumnStretchable (2, TRUE);
  adjustColumn(3);
  setColumnStretchable (4, TRUE);
  setColumnStretchable (5, TRUE);
  adjustColumn(6);
  setColumnStretchable (6, FALSE);
  
  QString form = edform->text();
  //the presence of ${(} and ${)} was checked
  int form_index = form.find("${(}") + 4;
  int tbl_index = 0;
    
  while (extract(tbl_index, form_index, form)) {
    setText(tbl_index, 6, QString::null);
    tbl_index += 1;
  }
  
  if (tbl_index == 0)
    insert_row_before(0);
    
  connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	  this, SLOT(button_pressed(int, int, int, const QPoint &)));
  
  if (SpecifierList.isEmpty()) {
    SpecifierList.append("");
    SpecifierList.append("const");
    SpecifierList.append("volatile");
  }
  
  if (PtrList.isEmpty()) {
    PtrList.append("");
    PtrList.append("*");
    PtrList.append("&");
    PtrList.append("*&");
  }
  
  TypeRankList.clear();
  ParamRankList.clear();
  
  for (int rank = 0; rank != params->numRows(); rank += 1) {
    if (!params->name(rank).isEmpty() || !params->type(rank).isEmpty()) {
      QString s;
      
      s.sprintf("${t%u}", rank);
      TypeRankList.append(s);
      s.sprintf("${p%u}", rank);
      ParamRankList.append(s);
    }
  }
}

void CppParamsTable::init_row(int row) {
  setItem(row, 0, new QTableItem(this, QTableItem::Never, QString::null));
  setItem(row, 1, new ComboItem(this, QString::null, SpecifierList));
  setItem(row, 2, new ComboItem(this, QString::null, TypeRankList));
  setItem(row, 3, new ComboItem(this, QString::null, PtrList));
  setItem(row, 4, new ComboItem(this, QString::null, ParamRankList));
  setText(row, 5, QString::null);
  setText(row, 6, QString::null);
}

static int bypass_string(const char * s, int index)
{
  // index is just after the "
  for (;;) {
    switch (s[index]) {
    case '"':
      return index + 1;
    case '\\':
      if (s[index + 1] == 0)
	return index + 1;
      index += 2;
      break;
    case 0:
      return index;
    default:
      index += 1;
    }
  }
}

static int bypass_char(const char * s, int index)
{
  // index is just after the '
  for (;;) {
    switch (s[index]) {
    case '\'':
      return index + 1;
    case '\\':
      if (s[index + 1] == 0)
	return index + 1;
      index += 2;
      break;
    case 0:
      return index;
    default:
      index += 1;
    }
  }
}

static int bypass_cpp_comment(const char * s, int index)
{
  // index is just after the //
  char * p = strchr(s, '\n');
  
  return (p == 0)
    ? index + strlen(s + index)
    : p - s;
}

static int bypass_c_comment(const char * s, int index)
{
  // index is just after the /*
  for (;;) {
    switch (s[index]) {
    case '*':
      if (s[index + 1] == '/')
	return index + 2;
      break;
    case 0:
      return index;
    }
    index += 1;
  }
}

static int supOf(const char * s, int index)
{
  // return the index after of the parameter form
  // s at least contains ${)}
  int ouvr = 0;
  
  for (;;) {
    switch (s[index]) {
    case '$':
      if (strncmp(s + index, "${)}", 4) == 0)
	return index;
      index += 1;
      break;
    case '(':
    case '[':
    case '{':
      ouvr += 1;
      index += 1;
      break;
    case ')':
    case ']':
    case '}':
      ouvr -= 1;
      index += 1;
      break;
    case '"':
      index = bypass_string(s, index + 1);
      break;
    case '\'':
      index = bypass_char(s, index + 1);
      break;
    case '/':
      switch (s[index+1]) {
      case '/':
	index = bypass_cpp_comment(s, index + 2);
	break;
	case '*':
	index = bypass_c_comment(s, index + 2);
	break;
      default:
	index += 1;
      }
      break;
    case 0:
      // in case ${)} is in a comment etc ...
      return index;
    case ',':
      if (ouvr == 0)
	return index;
      // no break
    default:
      index += 1;
    }
  }
}

bool CppParamsTable::extract(int tblindex, int & strindex, QString s) {
  // s at least contains ${)}
  while (s.at(strindex).isSpace())
    strindex += 1;
  
  int sup = supOf(s, strindex);
  
  if (s.mid(strindex, sup - strindex).stripWhiteSpace().isEmpty())
    return FALSE;
  
  QString specifier;
  QString t_i;
  QString ptr;
  QString p_i;
  QString modifier;
  int index = s.find("${t", strindex);
  
  if ((index == -1) || (index >= sup)) {
    // no $type
    index = s.find("${p", strindex);
    
    if ((index == -1) || (index >= sup))
      // no $type, no $name
      specifier = s.mid(strindex, sup - strindex).stripWhiteSpace();
    else {
      // no $type
      t_i = s.mid(strindex, index - strindex).stripWhiteSpace();
      
      // extract ptr
      int ip = t_i.find('*');
      int ir = t_i.find('&');

      if (ip != -1) {
	if ((ir != -1) && (ir < ip)) {
	  ptr = t_i.mid(ir).stripWhiteSpace();
	  t_i = t_i.left(ir).stripWhiteSpace();
	}
	else  {
	  ptr = t_i.mid(ip).stripWhiteSpace();
	  t_i = t_i.left(ip).stripWhiteSpace();
	}
      }
      else if (ir != -1) {
	ptr = t_i.mid(ir).stripWhiteSpace();
	t_i = t_i.left(ir).stripWhiteSpace();
      }
      
      if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
	return FALSE;
      else {
	strindex += 1;
	p_i = s.mid(index, strindex - index);
	modifier = s.mid(strindex, sup - strindex).stripWhiteSpace();
      }
    }
  }
  else {
    specifier = s.mid(strindex, index - strindex).stripWhiteSpace();

    if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
      return FALSE;
  
    strindex += 1;
    t_i = s.mid(index, strindex - index);
    
    index = s.find("${p", strindex);
    
    if ((index == -1) || (index >= sup))
      // unnamed
      modifier = s.mid(strindex, sup - strindex).stripWhiteSpace();
    else {
      ptr = s.mid(strindex, index - strindex).stripWhiteSpace();
      if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
	return FALSE;
      else {
	strindex += 1;
	p_i = s.mid(index, strindex - index);
	modifier = s.mid(strindex, sup - strindex).stripWhiteSpace();
      }
    }
  }
  
  setNumRows(tblindex + 1);

  setItem(tblindex, 1, new ComboItem(this, specifier, SpecifierList));
  setItem(tblindex, 2, new ComboItem(this, t_i, TypeRankList));
  setItem(tblindex, 3, new ComboItem(this, ptr, PtrList));
  setItem(tblindex, 4, new ComboItem(this, p_i, ParamRankList));
  setText(tblindex, 5, modifier);
  
  strindex = (s.at(sup) == QChar(',')) ? sup + 1 : sup;
    
  return TRUE;
}

void CppParamsTable::setItem(int row, int col, QTableItem * item) {
  QTable::setItem(row, col, item);
  
  if ((col == 2) || (col == 4))
    update_name(row);
}

void CppParamsTable::setCurrentCell(int row, int col) {
  QTable::setCurrentCell(row, col);
  update_names();
}

void CppParamsTable::update_names() {
  int n = numRows();
  int row;

  for (row = 0; row != n; row += 1)
    update_name(row);
}

void CppParamsTable::update_name(int row) {
  bool t_set;
  bool p_set;
  unsigned t_i;
  unsigned p_i;
  
  if (!text(row, 2).isEmpty() &&
      (sscanf((const char *) text(row, 2), "${t%u}", &t_i) == 1))
    t_set = TRUE;
  else
    t_set = FALSE;
  
  if (!text(row, 4).isEmpty() &&
      (sscanf((const char *) text(row, 4), "${p%u}", &p_i) == 1))
    p_set = TRUE;
  else
    p_set = FALSE;
  
  if (t_set) {
    if (p_set)
      QTable::setItem(row, 0,
		      new QTableItem(this, QTableItem::Never,
				     ((t_i == p_i) && (t_i < params->nparams()))
				     ? params->name(t_i) : QString::null));
    else
      QTable::setItem(row, 0,
		      new QTableItem(this, QTableItem::Never,
				     (t_i < params->nparams())
				     ? params->name(t_i) : QString::null));
  }
  else
    QTable::setItem(row, 0,
		    new QTableItem(this, QTableItem::Never,
				   (p_set && (p_i < params->nparams()))
				   ? params->name(p_i) : QString::null));
}

void CppParamsTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == 6) {
    char s[16];
    
    sprintf(s, "param %d", row + 1);
    
    QPopupMenu m;
    m.insertItem(s, -1);
    m.insertSeparator();
    m.insertItem("Insert param before", 0);
    m.insertItem("Insert param after", 1);
    m.insertSeparator();
    m.insertItem("Delete param", 2);
    m.insertSeparator();
    m.insertItem("Copy param", 3);
    m.insertItem("Cut param", 4);
    m.insertItem("Paste param", 5);
    m.insertSeparator();
    
    QPopupMenu mv;
    int rank;
    
    for (rank = 0; rank != numRows(); rank += 1)
      if (rank != row)
	mv.insertItem(QString::number(rank + 1), 10 + rank);
    
    m.insertItem("Move param", &mv);
    m.insertSeparator();
    
    QPopupMenu rk;
    int t_i;
    int p_i;
    
    if (text(row, 2).isEmpty() ||
	(sscanf((const char *) text(row, 2), "${t%d}", &t_i) != 1))
      t_i = -1;
    if (text(row, 4).isEmpty() ||
	(sscanf((const char *) text(row, 4), "${p%d}", &p_i) != 1))
      p_i = -1;
    
    for (rank = 0; rank != params->numRows(); rank += 1)
      if ((!params->name(rank).isEmpty() || !params->type(rank).isEmpty()) &&
	  ((rank != t_i) || (rank != p_i)))
	rk.insertItem(QString::number(rank), 100 + rank);
    
    m.insertItem("Set rank <i>", &rk);
    
    switch (rank = m.exec(QCursor::pos())) {
    case 0:
      insert_row_before(row);
      break;
    case 1:
      insert_row_after(row);
      break;
    case 2:
      delete_row(row);
      break;
    case 3:
      copy_row(row);
      break;
    case 4:
      cut_row(row);
      break;
    case 5:
      paste_row(row);
      break;
    default:
      if (rank >= 100) {
	char s[16];
	
	if (t_i != -1) {
	  sprintf(s, "${t%d}", rank - 100);
	  setItem(row, 2, new ComboItem(this, s, TypeRankList));
	}
	
	if (p_i != -1) {
	  sprintf(s, "${p%d}", rank - 100);
	  setItem(row, 4, new ComboItem(this, s, ParamRankList));
	}
      }
      else if (rank >= 10)
	move_row(row, rank - 10);
      break;
    }
  }
}

void CppParamsTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  int col;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    for (col = 0; col != 5; col += 1) {
      QTableItem * it = item(index - 1, col);
      
      takeItem(it);
      setItem(index, col, it);
    }
    setText(index, 5, text(index - 1, 5));
    setText(index, 6, text(index - 1, 6));
  }

  init_row(row);
 }

void CppParamsTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  int col;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    for (col = 0; col != 5; col += 1) {
      QTableItem * it = item(index - 1, col);
      
      takeItem(it);
      setItem(index, col, it);
    }
    setText(index, 5, text(index - 1, col));
    setText(index, 6, text(index - 1, col));
  }

  init_row(row + 1);
 }

void CppParamsTable::delete_row(int row) {
  int n = numRows();
  int index;
  int col;

  clearCellWidget(row, 1);
    
  if (row == (n - 1)) {
    // the last line : empty it
    init_row(row);
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      for (col = 0; col != 5; col += 1) {
	QTableItem * it = item(index + 1, col);
	
	takeItem(it);
	setItem(index, col, it);
      }
      setText(index, 5, text(index + 1, col));
      setText(index, 6, text(index + 1, col));
    }
    
    setNumRows(n - 1);
  }
}

void CppParamsTable::copy_row(int row) {
  int col;
  
  for (col = 0; col != 7; col += 1)
    copied[col] = text(row, col);
}

void CppParamsTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void CppParamsTable::paste_row(int row) {
  int col;
  
  for (col = 0; col != 7; col += 1)
    setText(row, col, copied[col]);
}

void CppParamsTable::move_row(int from, int to) {
  int col;
  QString save_copied[7];
  
  for (col = 0; col != 7; col += 1)
    save_copied[col] = copied[col];
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  for (col = 0; col != 7; col += 1)
    copied[col] = save_copied[col];
}

void CppParamsTable::update_edform() {
  forceUpdateCells();
  
  QString s;
  const char * sep = "";
  
  int n = numRows();
  int index;
  
  for (index = 0; index != n; index += 1) {
    QString p;
    int col;
    
    for (col = 1; col != 5; col += 1) {
      if (!text(index, col).isEmpty()) {
	if (p.isEmpty())
	  p += text(index, col);
	else
	  p += " " + text(index, col);
      }
    }
    
    if (!text(index, 5).isEmpty()) {
      if (p.isEmpty() || (text(index, 5).at(0) == QChar('[')))
	p += text(index, 5);
      else
	p += " " + text(index, 5);
    }
    
    p = p.stripWhiteSpace();
    if (! p.isEmpty()) {
      s += sep + p;
      sep = ", ";
    }
  }
  
  QString form = edform->text();
  
  index = form.find("${(}");
  
  form.replace(index + 4, form.find("${)}") - index - 4, s);
  edform->setText(form);
}

//
// CppParamsDialog
//

QSize CppParamsDialog::previous_size;

CppParamsDialog::CppParamsDialog(ParamsTable * params, MultiLineEdit * form)
    : QDialog(0, "C++ parameters dialog", TRUE) {
  setCaption("C++ parameters dialog");

  QVBoxLayout * vbox = new QVBoxLayout(this); 
  
  vbox->setMargin(5);

  tbl = new CppParamsTable(params, form, this);
  vbox->addWidget(tbl);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * accept = new QPushButton("&OK", this);
  QPushButton * cancel = new QPushButton("&Cancel", this);
  QSize bs(cancel->sizeHint());
  
  accept->setDefault(TRUE);
  accept->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  hbox->addWidget(accept);
  hbox->addWidget(cancel);
    
  connect(accept, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

CppParamsDialog::~CppParamsDialog() {
  previous_size = size();
}

void CppParamsDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}
  

void CppParamsDialog::accept() {
  tbl->update_edform();
  
  QDialog::accept();
}
