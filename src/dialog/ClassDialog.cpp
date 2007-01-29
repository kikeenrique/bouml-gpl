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

#include <stdio.h>

#include <qcursor.h>
#include <qsplitter.h> 
#include <qgrid.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qcheckbox.h>
#include <qpopupmenu.h> 
#include <qgroupbox.h> 
#include <qbuttongroup.h>
#include <qpushbutton.h> 
#include <qmessagebox.h>

#include "ClassDialog.h"
#include "ClassData.h"
#include "BrowserClass.h"
#include "KeyValueTable.h"
#include "ComboItem.h"
#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "GenerationSettings.h"
#include "BrowserRelation.h"
#include "RelationData.h"
#include "ActualParamData.h"
#include "AttributeData.h"
#include "OperationData.h"
#include "ExtraMemberData.h"
#include "BrowserArtifact.h"
#include "strutil.h"
#include "BodyDialog.h"
#include "AnnotationDialog.h"

QSize ClassDialog::previous_size;

ClassDialog::ClassDialog(ClassData * c)
    : QTabDialog(0, 0, TRUE), cl(c) {
  if (c->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }
  
  setCaption("Class dialog");

  bool visit = !hasOkButton();
  
  if (! visit) {
    BrowserClass::instances(nodes);
    nodes.full_names(node_names);
  }
  
  BrowserClass * bn = (BrowserClass *) c->get_browser_node();

  QHBox * htab;
  QVBox * vtab;
  QGrid * grid;
  QSplitter * split;
  QGroupBox * bg;
  QLabel * lbl1;
  QLabel * lbl2;
  QLabel * lbl3;
  QLabel * lbl4;
  QFont font;
  
  // general tab
  
  grid = new QGrid(2, this);
  grid->setSpacing(5);
  grid->setMargin(5);
  
  new QLabel("name : ", grid);
  edname = new LineEdit(cl->name(), grid);
  edname->setReadOnly(visit);
  
  new QLabel("stereotype : ", grid);
    
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(cl->get_stereotype()));
  if (! visit) {
    edstereotype->insertStringList(BrowserClass::default_stereotypes());
    edstereotype->setAutoCompletion(TRUE);
    connect(edstereotype, SIGNAL(activated(const QString &)),
	    this, SLOT(edStereotypeActivated(const QString &)));
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
  
  
  new QLabel(grid);
  htab = new QHBox(grid);
  opt_bg = new QGroupBox(3, QGroupBox::Horizontal, QString::null, htab);
  abstract_cb = new QCheckBox("abstract", opt_bg);
  if (cl->get_is_abstract()) {
    abstract_cb->setChecked(TRUE);
    if (bn->have_abstract_operation())
      abstract_cb->setEnabled(FALSE);
  }
  abstract_cb->setDisabled(visit);

  QButtonGroup * bgv;
  
  if (bn->nestedp()) {
    bgv = uml_visibility.init(htab, cl->get_uml_visibility(), TRUE);
    if (visit)
      bgv->setEnabled(FALSE);
    htab->setStretchFactor(bgv, 1000);  
  }
  
  BrowserNodeList inh;
  
  basetypelbl = new QLabel("base type : ", grid);
  edbasetype = new QComboBox(!visit, grid);
  if (cl->browser_node->children(inh, UmlGeneralisation, UmlRealize),
      inh.count() != 0) {
    // first inheritance is taken in all cases
    edbasetype->insertItem(((RelationData *) inh.at(0)->get_data())->get_end_class()->full_name(TRUE));
    edbasetype->setEnabled(FALSE);
  }
  else if (! visit) {
    QString bt = cl->get_base_type().get_full_type();
    
    if (bt.isEmpty() &&		// no base_type, try with a dependency
	(cl->browser_node->children(inh, UmlDependency), inh.count() != 0))
      bt = ((RelationData *) inh.at(0)->get_data())->get_end_class()->full_name(TRUE);
    
    edbasetype->insertItem(bt);
    edbasetype->insertStringList(GenerationSettings::basic_types());
    edbasetype->insertStringList(node_names);
    edbasetype->setAutoCompletion(TRUE);
  }
  edbasetype->setCurrentItem(0);
  edbasetype->setSizePolicy(sp);
  
  if (!bn->nestedp()) {
    BrowserNode * bc = bn->get_associated_artifact();
    
    if (visit) {
      if ((bc != 0) && !bc->deletedp()) {
	new QLabel("artifact : ", grid);
	artifact = new QComboBox(FALSE, grid);
	artifact->insertItem(bc->full_name(TRUE));
      }
      else
	artifact = 0;
    }
    else {      
      BrowserArtifact::instances(artifacts, "source");
      if (!artifacts.isEmpty()) {
	QStringList artifact_names;
	
	artifacts.full_names(artifact_names);
	new QLabel("artifact : ", grid);
	artifact = new QComboBox(FALSE, grid);
	artifact->insertItem("");
	artifact->insertStringList(artifact_names);
	
	if (bc != 0) {
	  if (bc->deletedp()) {
	    QString fn = bc->full_name(TRUE);
	    
	    artifact->insertItem(fn);
	    artifacts.append(bc);
	    artifact->setCurrentItem(artifacts.count());
	  }
	  else
	    artifact->setCurrentItem(artifacts.find(bc) + 1);
	}
	else
	  artifact->setCurrentItem(0);
      }
      else
	artifact = 0;
    }
  }
  else
    artifact = 0;
  
  vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (! visit) {
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
    connect(new SmallPushButton("Default", vtab), SIGNAL(clicked()),
	    this, SLOT(default_description()));
  }
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(bn->get_comment());
  comment->setFont(font);
  
  addTab(grid, "Uml");
  
  // parametrized tab
  
  parametrized_vtab = new QVBox(this);
  
  parametrized_vtab->setMargin(5);
  
  (new QLabel("\nEnter formals in case the class is parametrized\n", 
	      parametrized_vtab))
    ->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  
  formals_table =
    new FormalParamsTable(c, parametrized_vtab, node_names, visit);
  
  addTab(parametrized_vtab, "Parametrized");
  
  // instantiate tab
  
  if (cl->get_n_actualparams() != 0) {
    instantiate_vtab = new QVBox(this);
  
    instantiate_vtab->setMargin(5);
  
    (new QLabel("\nSpecify actuals else formals default value will be used\n",
		instantiate_vtab))
      ->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  
    actuals_table =
      new ActualParamsTable(c, instantiate_vtab, node_names, visit);
  
    addTab(instantiate_vtab, "Instantiate");
  }
  else
    instantiate_vtab = 0;
  
  // C++
  
  split = new QSplitter(Vertical, this);
  split->setOpaqueResize(TRUE);
  
  vtab = new QVBox(split); 
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  lbl1 = new QLabel(htab);
  bg = new QGroupBox(1, QGroupBox::Horizontal, QString::null, htab);
  cpp_external_cb = new QCheckBox("external", bg);
  if (cl->cpp_is_external())
    cpp_external_cb->setChecked(TRUE);
  if (visit)
    cpp_external_cb->setDisabled(TRUE);
  else
    connect(cpp_external_cb, SIGNAL(toggled(bool)),
	    SLOT(cpp_update_decl()));
  
  if (bn->nestedp()) {
    bgv = cpp_visibility.init(htab, cl->get_cpp_visibility(),
			      FALSE, 0, "follow uml");
    
    if (visit)
      bgv->setEnabled(FALSE);
  }
  
  htab = new QHBox(vtab); 
  htab->setMargin(5);  
  lbl2 = new QLabel("Declaration : ", htab);
  edcppdecl = new MultiLineEdit(htab);
  edcppdecl->setText(c->cpp_decl);
  font = edcppdecl->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  edcppdecl->setFont(font);
  if (visit)
    edcppdecl->setReadOnly(TRUE);
  else
    connect(edcppdecl, SIGNAL(textChanged()), this, SLOT(cpp_update_decl()));

  vtab = new QVBox(split); 
  
  htab = new QHBox(vtab); 
  htab->setMargin(5);  
  lbl3 = new QLabel("Result after\nsubstitution : ", htab);
  showcppdecl = new MultiLineEdit(htab);
  showcppdecl->setReadOnly(TRUE);
  showcppdecl->setFont(font);

  if (visit)
    same_width(lbl1, lbl2, lbl3);
  else {
    htab = new QHBox(vtab); 
    lbl4 = new QLabel(htab);
    connect(new QPushButton("Default declaration", htab), SIGNAL(pressed ()),
	    this, SLOT(cpp_default_decl()));
    connect(new QPushButton("Not generated in C++", htab), SIGNAL(pressed ()),
	    this, SLOT(cpp_unmapped_decl()));
  
    same_width(lbl1, lbl2, lbl3, lbl4);
  }
  
  addTab(split, "C++");
  
  // Java
  
  split = new QSplitter(Vertical, this);
  split->setOpaqueResize(TRUE);
  
  vtab = new QVBox(split); 
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  lbl1 = new QLabel(htab);
  bg = new QGroupBox(3, QGroupBox::Horizontal, QString::null, htab);
  java_public_cb = new QCheckBox("public", bg);
  if (cl->java_is_public())
    java_public_cb->setChecked(TRUE);
  java_final_cb = new QCheckBox("final", bg);
  if (cl->java_is_final())
    java_final_cb->setChecked(TRUE);
  java_external_cb = new QCheckBox("external", bg);
  if (cl->java_is_external())
    java_external_cb->setChecked(TRUE);
  if (visit) {
    java_public_cb->setDisabled(TRUE);
    java_final_cb->setDisabled(TRUE);
    java_external_cb->setDisabled(TRUE);
  }
  else {
    connect(java_public_cb, SIGNAL(toggled(bool)),
	    SLOT(java_update_decl()));
    connect(java_final_cb, SIGNAL(toggled(bool)),
	    SLOT(java_update_decl()));
    connect(java_external_cb, SIGNAL(toggled(bool)),
	    SLOT(java_update_decl()));
  }
  
  htab = new QHBox(vtab); 
  htab->setMargin(5);  
  lbl2 = new QLabel("Definition : ", htab);
  edjavadecl = new MultiLineEdit(htab);
  edjavadecl->setText(c->java_decl);
  edjavadecl->setFont(font);
  if (visit)
    edjavadecl->setReadOnly(TRUE);
  else
    connect(edjavadecl, SIGNAL(textChanged()), this, SLOT(java_update_decl()));

  vtab = new QVBox(split); 
  
  htab = new QHBox(vtab); 
  htab->setMargin(5);  
  lbl3 = new QLabel("Result after\nsubstitution : ", htab);
  showjavadecl = new MultiLineEdit(htab);
  showjavadecl->setReadOnly(TRUE);
  showjavadecl->setFont(font);

  htab = new QHBox(vtab); 
  lbl4 = new QLabel(htab);

  if (!visit) {
    connect(new QPushButton("Default definition", htab), SIGNAL(pressed ()),
	    this, SLOT(java_default_decl()));
    connect(new QPushButton("Not generated in Java", htab), SIGNAL(pressed ()),
	    this, SLOT(java_unmapped_decl()));
          
  }
  
  same_width(lbl1, lbl2, lbl3, lbl4);
  javaannotation = (const char *) c->java_annotation;
  editjavaannotation =
    new QPushButton((visit) ? "Show annotation" : "Edit annotation",
		    htab);
  connect(editjavaannotation, SIGNAL(clicked ()),
	  this, SLOT(java_edit_annotation()));

  addTab(split, "Java");
  
  // IDL
  
  split = new QSplitter(Vertical, this);
  split->setOpaqueResize(TRUE);
  
  vtab = new QVBox(split); 
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  switch_bg = new QGroupBox(2, QGroupBox::Horizontal, QString::null, htab);
  new QLabel("switch type : ", switch_bg);
  edswitch_type = new QComboBox(!visit, switch_bg);
  if (!visit) {
    edswitch_type->setAutoCompletion(TRUE);
    edswitch_type->insertItem(cl->get_switch_type().get_full_type());
    edswitch_type->insertStringList(GenerationSettings::basic_types());
    
    QListIterator<BrowserNode> it(nodes);
    
    while (it.current() != 0) {
      QString st =
	idl_stereotype(((ClassData *) (it.current()->get_data()))->get_stereotype());
      
      if (st == "enum")
	edswitch_type->insertItem(it.current()->full_name(TRUE));
      
      ++it;
    }
  }
  edswitch_type->setCurrentItem(0);
  edswitch_type->setSizePolicy(sp);
  
  htab = new QHBox(vtab);
  htab->setMargin(5);
  lbl1 = new QLabel(htab);
  bg = new QGroupBox(3, QGroupBox::Horizontal, QString::null, htab);
  idl_external_cb = new QCheckBox("external", bg);
  if (cl->idl_is_external())
    idl_external_cb->setChecked(TRUE);
  idl_local_cb = new QCheckBox("local", bg);
  idl_custom_cb = new QCheckBox("custom", bg);
  if (cl->idl_is_local())
    idl_local_cb->setChecked(TRUE);
  if (cl->idl_is_custom())
    idl_custom_cb->setChecked(TRUE);
  if (visit) {
    idl_local_cb->setDisabled(TRUE);
    java_final_cb->setDisabled(TRUE);
    java_external_cb->setDisabled(TRUE);
  }
  else {
    connect(idl_local_cb, SIGNAL(toggled(bool)),
	    SLOT(idl_update_decl()));
    connect(idl_custom_cb, SIGNAL(toggled(bool)),
	    SLOT(idl_update_decl()));
    connect(idl_external_cb, SIGNAL(toggled(bool)),
	    SLOT(idl_update_decl()));
  }
  
  htab = new QHBox(vtab); 
  htab->setMargin(5);  
  lbl2 = new QLabel("Declaration : ", htab);
  edidldecl = new MultiLineEdit(htab);
  edidldecl->setText(c->idl_decl);
  edidldecl->setFont(font);
  if (visit)
    edidldecl->setReadOnly(TRUE);
  else
    connect(edidldecl, SIGNAL(textChanged()), this, SLOT(idl_update_decl()));

  vtab = new QVBox(split); 
  
  htab = new QHBox(vtab); 
  htab->setMargin(5);  
  lbl3 = new QLabel("Result after\nsubstitution : ", htab);
  showidldecl = new MultiLineEdit(htab);
  showidldecl->setReadOnly(TRUE);
  showidldecl->setFont(font);

  if (visit)
    same_width(lbl1, lbl2, lbl3);
  else {
    htab = new QHBox(vtab); 
    lbl4 = new QLabel(htab);
    connect(new QPushButton("Default declaration", htab), SIGNAL(pressed ()),
	    this, SLOT(idl_default_decl()));
    connect(new QPushButton("Not generated in IDL", htab), SIGNAL(pressed ()),
	    this, SLOT(idl_unmapped_decl()));
    
    same_width(lbl1, lbl2, lbl3, lbl4);
  }
  
  addTab(split, "IDL");
  
  // USER : list key - value
  
  vtab = new QVBox(this);
  kvtable = new KeyValuesTable(bn, vtab, visit);
  addTab(vtab, "Properties");
  
  //
  
  QString stereotype = fromUnicode(edstereotype->currentText());
  
  current_cpp_stereotype = cpp_stereotype(stereotype);
  current_java_stereotype = java_stereotype(stereotype);
  current_idl_stereotype = idl_stereotype(stereotype);
  
  edStereotypeActivated(stereotype);

  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(update_all_tabs()));
}

void ClassDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

ClassDialog::~ClassDialog() {
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

QString ClassDialog::cpp_stereotype(const QString & stereotype)
{
  QString s = GenerationSettings::cpp_class_stereotype(stereotype);
  
  return ((s == "struct") || (s == "union") || (s == "enum") ||
	  (s == "typedef") || (s == "ignored"))
    ? s : QString("class");
}

QString ClassDialog::java_stereotype(const QString & stereotype)
{
  QString s = GenerationSettings::java_class_stereotype(stereotype);
  
  return ((s == "interface") || (s == "@interface") ||
	  (s == "enum") || (s == "enum_pattern") ||
	  (s == "ignored"))
    ? s : QString("class");
}

QString ClassDialog::idl_stereotype(const QString & stereotype)
{
  QString s = GenerationSettings::idl_class_stereotype(stereotype);
  
  return ((s == "struct") || (s == "union") || (s == "enum") ||
	  (s == "typedef") || (s == "exception") ||
	  (s == "ignored") || (s == "interface"))
    ? s : QString("valuetype");
}

void ClassDialog::default_description() {
  comment->setText(GenerationSettings::default_class_description());
}

void ClassDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       cl, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void ClassDialog::post_edit_description(ClassDialog * d, QString s)
{
  d->comment->setText(s);
}

void ClassDialog::edStereotypeActivated(const QString & s) {
  QString stereotype = s.stripWhiteSpace();
  
  if (stereotype == "typedef") {
    basetypelbl->show();
    edbasetype->show();
    opt_bg->hide();
  }
  else {
    basetypelbl->hide();
    edbasetype->hide();
    if ((stereotype != "enum") && (stereotype != "enum_pattern"))
      opt_bg->show();
    else
      opt_bg->hide();
  }
  parametrized_vtab->setEnabled((stereotype != "enum") && (stereotype != "enum_pattern") && (stereotype != "typedef"));
  if (instantiate_vtab)
    instantiate_vtab->setEnabled((stereotype != "enum") && (stereotype != "enum_pattern") && (stereotype != "union"));
  
  if (current_cpp_stereotype != cpp_stereotype(stereotype))
    cpp_default_decl();
  
  if (current_java_stereotype != java_stereotype(stereotype))
    java_default_decl();
  
  QString idl_st = idl_stereotype(stereotype);
  
  switch_bg->setEnabled(idl_st == "union");
  idl_local_cb->setEnabled(idl_st == "interface");
  idl_custom_cb->setEnabled(idl_st == "valuetype");
  
  if (idl_st != current_idl_stereotype)
    idl_default_decl();
}

void ClassDialog::update_all_tabs() {
  formals_table->forceUpdateCells();
  if (cl->get_n_actualparams() != 0)
    actuals_table->forceUpdateCells();

  cpp_update_decl();
  java_update_decl();
  idl_update_decl();
}

static void cpp_generate_inherit(QString & s, ClassData * cl,
				 ActualParamsTable * actuals_table,
				 BrowserNodeList & nodes,
				 QStringList & node_names) {  
  BrowserNodeList inh;
  const char * sep = " : ";
  
  cl->get_browser_node()->children(inh, UmlGeneralisation, UmlRealize);
  
  QListIterator<BrowserNode> it(inh);
  
  while (it.current() != 0) {
    RelationData * r = (RelationData *) it.current()->get_data();
    
    if (r->get_cppdecl_a()[0]) {
      s += sep;
      s += stringify((r->get_cpp_visibility_a() != UmlDefaultVisibility)
		     ? r->get_cpp_visibility_a() : r->get_uml_visibility_a());
      s += ((r->get_cpp_virtual_inheritance()) ? " virtual " : " ");
      
      BrowserClass * mother = r->get_end_class();
      
      s += mother->get_name();
      sep = ", ";
      if (actuals_table != 0)
	actuals_table->generate(s, cl, mother, TRUE, nodes, node_names);
    }
    
    ++it;
  }
}

static void cpp_generate_typedef_type(QString & s, ClassData * cl,
				      ActualParamsTable * actuals_table,
				      QString basetype,
				      BrowserNodeList & nodes,
				      QStringList & node_names) {
  BrowserNodeList inh;
  
  cl->get_browser_node()->children(inh, UmlGeneralisation, UmlRealize);
  
  QListIterator<BrowserNode> it(inh);
  
  while (it.current() != 0) {
    RelationData * r = (RelationData *) it.current()->get_data();
    /*if (r->get_cppdecl_a()[0])*/ {
      BrowserClass * mother = r->get_end_class();
      
      if (mother->full_name(TRUE) == basetype) {
	s += mother->get_name();
	if (actuals_table != 0)
	  actuals_table->generate(s, cl, mother, TRUE, nodes, node_names);
	return;
      }
    }
    
    ++it;
  }
  
  s += GenerationSettings::cpp_type(type(basetype, node_names, nodes));
}

static void generate_members(BrowserNode * cl, QString & s,
			     const char * (AttributeData::* att_f)() const,
			     const char * (OperationData::* oper_f)() const,
			     const char * (RelationData::* rel_a_f)() const,
			     const char * (RelationData::* rel_b_f)() const,
			     const char * (ExtraMemberData::* ex_f)() const,
			     const char * (ClassData::* class_f)() const,
			     QString (* ste_f)(const QString &),
			     bool idl_union = FALSE,
			     bool only_items = FALSE,
			     bool except_items = FALSE) 
{
  QListViewItem * child;
  
  for (child = cl->firstChild(); child; child = child->nextSibling()) {
    QString pre =  "...";
    QString decl;
    QString post;
    
    switch (((BrowserNode *) child)->get_type()) {
    case UmlDependency:
      if (! only_items) {
	RelationData * rel = (RelationData *) ((BrowserNode *) child)->get_data();
	
	if (rel->is_a((BrowserRelation *) child)
	    && (!strcmp(rel->get_stereotype(), "friend"))) {
	  ClassData * cld = (ClassData *) rel->get_end_class()->get_data();
	  QString st = cld->get_stereotype();
	  
	  if (st.isEmpty())
	    st = "class";
	  
	  s += "...friend " + st + " " +
	    true_name(rel->get_end_class()->get_name(), cld->get_cppdecl());
	  
	  post = "\n";
	}
      }
      // no break !
    case UmlGeneralisation:
    case UmlRealize:
      continue;
    case UmlAttribute:
      {
         // without the var 'at' G++ 3.2 generate a wrong code
	 AttributeData * at = (AttributeData *) ((BrowserNode *) child)->get_data();
	 const char * st = at->get_stereotype();
	 
	 if (only_items) {
	   if (strcmp(st, "attribute") == 0)
	     continue;
	 }
	 else if (except_items) {
	   if (strcmp(st, "attribute") != 0)
	     continue;
	 }

         decl = (at->*att_f)();
      
         if (idl_union)
	   pre = QString("...case ") + at->get_idlcase() + " ... ";
	  
         post = "\n";
      }
      break;
    case UmlOperation:
      if (! only_items) {
         // without the var 'op' G++ 3.2 generate a wrong code
	 OperationData * op = (OperationData *) ((BrowserNode *) child)->get_data();

         decl = (op->*oper_f)();
         post = "()\n";
      }
      break;
    case UmlExtraMember:
      if (! only_items) {
         // without the var 'emd' G++ 3.2 generate a wrong code
	 ExtraMemberData * emd = (ExtraMemberData *) ((BrowserNode *) child)->get_data();

	 s += (emd->*ex_f)();
	 s += '\n';
	 continue;
      }
    case UmlClass:
      if (! only_items) {
         // without the var 'cl' G++ 3.2 generate a wrong code
	 ClassData * cl = (ClassData *) ((BrowserNode *) child)->get_data();

         decl = (cl->*class_f)();
	 pre = "..." + ste_f(cl->get_stereotype()) + ' ';
         post = (pre.left(11) == "...typedef ") ? "\n" : " {...}\n";
      }
      break;
    default: // relations
      if (! only_items) {
	RelationData * re = (RelationData *) ((BrowserNode *) child)->get_data();

        decl = (re->is_a((BrowserRelation *) child))
	  ? (re->*rel_a_f)()
	  : (re->*rel_b_f)();
        post = "\n";
      }
    }
    
    if (! decl.isEmpty()) {
      s += pre;
      s += true_name(((BrowserNode *) child)->get_name(), decl);
      s += post;
    }
  }
}

static void cpp_generate_inlines(BrowserNode * cl, const QString & cl_name,
				 const QString & cl_decl, QString & s) 
{
  ClassData * cd = (ClassData *) cl->get_data();
  bool templp = (cd->get_n_formalparams() != 0) || (cl_name.find('<') != -1);
  QString templates;
  QString names;
  QString templates_tmplop;
  QString names_tmplop;
  
  if (((BrowserNode *) cl->parent())->get_type() == UmlClass) {
    ((ClassData *) ((BrowserNode *) cl->parent())->get_data())
      ->get_class_spec(templates, names, templates_tmplop, names_tmplop);
    names += "::";
  }
  
  QString t1;
  QString t2;
  
  cd->get_template_prefixes(t1, t2);
  templates_tmplop = templates + "template<>\n";
  templates += t1;
  names_tmplop = names + true_name(cl_name, cl_decl);
  names = names_tmplop + t2;
  
  QString prefix = '\n' + templates + "..." + names + "::";
  QString prefix_tmplop = '\n' + templates_tmplop + "..." + names_tmplop + "<...>::";
  QListViewItem * child;
  
  for (child = cl->firstChild(); child; child = child->nextSibling()) {
    switch (((BrowserNode *) child)->get_type()) {
    case UmlAttribute:
      {
	AttributeData * d = (AttributeData *) ((BrowserNode *) child)->get_data();
	const char * decl = d->get_cppdecl();
	
	if (templp &&
	    d->get_isa_class_attribute() &&
	    decl[0]) {
	  s += prefix;
	  s += true_name(((BrowserNode *) child)->get_name(), decl);
	  s += "\n\n";
	}
      }
      break;
    case UmlClass:
      cpp_generate_inlines((BrowserNode *) child,
			   ((BrowserNode *) child)->get_name(),
			   ((ClassData *) ((BrowserNode *) child)->get_data())->get_cppdecl(),
			   s);

      break;
    default:
      // relation
      {
	RelationData * re = (RelationData *) ((BrowserNode *) child)->get_data();

        if (re->is_a((BrowserRelation *) child)) {
	  const char * decl = re->get_cppdecl_a();
	  
	  if (templp &&
	      re->get_isa_class_relation_a() &&
	      decl[0]) {
	    s += prefix;
	    s += true_name(re->get_role_a(), decl);
	    s += "\n\n";
	  }
	}
	else {
	  const char * decl = re->get_cppdecl_b();
	  
	  if (templp &&
	      re->get_isa_class_relation_b() &&
	      decl[0]) {
	    s += prefix;
	    s += true_name(re->get_role_b(), decl);
	    s += "\n";
	  }
	}
      }
    case UmlDependency:
    case UmlGeneralisation:
    case UmlRealize:
      break;
    case UmlOperation:
      {
	OperationData * d = (OperationData *) ((BrowserNode *) child)->get_data();
	const char * def = d->get_cppdef();
	
	if ((templp || d->get_cpp_inline()) && def[0]) {
	  s += ((d->is_template_operation()) ? prefix_tmplop : prefix);
	  s += true_name(((BrowserNode *) child)->get_name(), def);
	  s += "(...){...}\n";
	}
      }
      break;
    case UmlExtraMember:
      {
	ExtraMemberData * d = (ExtraMemberData *) ((BrowserNode *) child)->get_data();
	const char * def = d->get_cpp_def();
	
	if ((templp || d->get_cpp_inline()) && def[0]) {
	  s += '\n' + def + '\n';
	}
      }
    }
  }
}

void ClassDialog::cpp_generate_decl(QString & s, ClassData * cl,
				    QString def, QString name,
				    QString stereotype,
				    QString basetype, QString comment,
				    ActualParamsTable * actuals,
				    FormalParamsTable * formals,
				    BrowserNodeList & nodes,
				    QStringList & node_names,
				    KeyValuesTable * kvt)
{
  const char * p = def;
  
  if (p == 0)
    return;
  
  QString indent = "";
  bool a_typedef = (stereotype == "typedef");
  bool an_enum = (stereotype == "enum");

  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  if (*p != '#')
    s += indent;
  
  const char * pp = 0;
  
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
      manage_comment(comment, p, pp);
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment, p, pp);
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += name;
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p && (*p != '#'))
	s += indent;
    }
    else if (a_typedef) {
      if (!strncmp(p, "${type}", 7)) {
	p += 7;
	cpp_generate_typedef_type(s, cl, actuals, basetype, nodes, node_names);
      }
      else
	s += *p++;
    }
    else if (an_enum) {
      if (!strncmp(p, "${items}", 8)) {
	p += 8;
	generate_members(cl->browser_node, s,
			 &AttributeData::get_cppdecl, &OperationData::get_cppdecl, 
			 &RelationData::get_cppdecl_a, &RelationData::get_cppdecl_b,
			 &ExtraMemberData::get_cpp_decl, &ClassData::get_cppdecl, 
			 cpp_stereotype);
      }
      else
	s += *p++;
    }
    else if (!strncmp(p, "${template}", 11)) {
      p += 11;
      formals->generate_cpp(s, nodes, node_names, name);
    }
    else if (!strncmp(p, "${inherit}", 10)) {
      p += 10;
      cpp_generate_inherit(s, cl, actuals, nodes, node_names);
    }
    else if (!strncmp(p, "${members}", 10)) {
      p += 10;
      generate_members(cl->browser_node, s,
		       &AttributeData::get_cppdecl, &OperationData::get_cppdecl, 
		       &RelationData::get_cppdecl_a, &RelationData::get_cppdecl_b,
		       &ExtraMemberData::get_cpp_decl, &ClassData::get_cppdecl,
		       cpp_stereotype);
    }
    else if (!strncmp(p, "${inlines}", 10)) {
      p += 10;
      cpp_generate_inlines(cl->browser_node, name, def, s);
    }
    else if (*p == '@')
      manage_alias(cl->browser_node, p, s, kvt);
    else
      s += *p++;
  }
}
				    
void ClassDialog::cpp_update_decl() {
  QString s;
  
  if (cl->browser_node->nestedp())
    cpp_visibility.update_default(uml_visibility);
  
  if (cpp_external_cb->isChecked())
    showcppdecl->setEnabled(FALSE);
  else {
    showcppdecl->setEnabled(TRUE);
    
    cpp_generate_decl(s, cl, edcppdecl->text().stripWhiteSpace(),
		      edname->text().stripWhiteSpace(), current_cpp_stereotype,
		      edbasetype->currentText().stripWhiteSpace(),
		      comment->text(), (instantiate_vtab != 0) ? actuals_table : 0,
		      formals_table, nodes, node_names, kvtable);
  }
  
  showcppdecl->setText(s);
}

void ClassDialog::cpp_default_decl() {
  if (cpp_external_cb->isChecked())
    edcppdecl->setText(GenerationSettings::cpp_default_external_class_decl());
  else {
    current_cpp_stereotype =
      cpp_stereotype(edstereotype->currentText().stripWhiteSpace());
    
    if (current_cpp_stereotype == "struct")
      edcppdecl->setText(GenerationSettings::cpp_default_struct_decl());
    else if (current_cpp_stereotype == "union")
      edcppdecl->setText(GenerationSettings::cpp_default_union_decl());
    else if (current_cpp_stereotype == "enum")
      edcppdecl->setText(GenerationSettings::cpp_default_enum_decl());
    else if (current_cpp_stereotype == "typedef")
      edcppdecl->setText(GenerationSettings::cpp_default_typedef_decl());
    else if (current_cpp_stereotype == "ignored")
      edcppdecl->setText(QString::null);
    else
      edcppdecl->setText(GenerationSettings::cpp_default_class_decl());
    
    cpp_update_decl();
  }
}

void ClassDialog::cpp_unmapped_decl() {
  edcppdecl->setText(QString::null);
  showcppdecl->setText(QString::null);
}

void ClassDialog::cpp_generate_members_def(const BrowserNode * cl, QString & s) 
{
  ClassData * cd = (ClassData *) cl->get_data();
  QString templates;
  QString names;
  QString templates_tmplop;
  QString names_tmplop;
  
  cd->get_class_spec(templates, names, templates_tmplop, names_tmplop);
  
  QString prefix = templates + "..." + names + "::";
  QString prefix_tmplop = templates_tmplop + "..." + names_tmplop + "<...>::";
  bool templ = !templates.isEmpty() || (strchr(cl->get_name(), '<') != 0);
  QListViewItem * child;
  
  for (child = cl->firstChild(); child; child = child->nextSibling())
    ((BrowserNode *) child)->member_cpp_def(prefix, prefix_tmplop, s, templ);
}
    
static void java_generate_extends(QString & s, const QString & stereotype,
				  ClassData * cl, ActualParamsTable * actuals_table,
				  BrowserNodeList & nodes, QStringList & node_names) {
  BrowserNodeList inh;
  const char * sep = " extends ";
    
  cl->get_browser_node()->children(inh, UmlGeneralisation, UmlRealize);
  
  QListIterator<BrowserNode> it(inh);
  
  while (it.current() != 0) {
    RelationData * r = 
      (RelationData *) ((BrowserRelation *) it.current())->get_data();
    
    if (r->get_javadecl_a()[0]) {
      bool gen = FALSE;
      BrowserClass * mother = r->get_end_class();
      QString other_stereotype =
	ClassDialog::java_stereotype(((ClassData *) mother->get_data())->get_stereotype());
      
      if ((stereotype == "interface") ||
	  (stereotype == "@interface")) {
	gen = TRUE;
	s += sep;
	
	if ((other_stereotype != "interface") &&
	    (other_stereotype != "@interface"))
	  s += "!!!!!";
      }
      else if ((other_stereotype != "interface") &&
	       (other_stereotype != "@interface")) {
	gen = TRUE;
	s += sep;
	
	if ((*sep == ',') ||
	    (stereotype == "union") || (stereotype == "enum") || (stereotype == "enum_pattern") ||
	    (other_stereotype == "union") || (other_stereotype == "enum"))
	  s += "!!!!!";
      }
      
      if (gen) {
	s += mother->get_name();
	sep = ", ";
	if (actuals_table != 0)
	  actuals_table->generate(s, cl, mother, FALSE, nodes, node_names);
      }
    }
    
    ++it;
  }
}

static void java_generate_implements(QString & s, const QString & stereotype,
				     ClassData * cl, ActualParamsTable * actuals_table,
				     BrowserNodeList & nodes, QStringList & node_names) {
  BrowserNodeList inh;
  const char * sep = " implements ";
    
  cl->get_browser_node()->children(inh, UmlGeneralisation, UmlRealize);
  
  QListIterator<BrowserNode> it(inh);
  
  while (it.current() != 0) {
    RelationData * r = 
      (RelationData *) ((BrowserRelation *) it.current())->get_data();
    
    if (r->get_javadecl_a()[0]) {
      BrowserClass * mother = r->get_end_class();
      QString other_stereotype =
	ClassDialog::java_stereotype(((ClassData *) mother->get_data())->get_stereotype());
      
      if ((other_stereotype == "interface") || 
	  (other_stereotype == "@interface")) {
	s += sep;
	
	if ((stereotype == "union") || (stereotype == "enum_pattern"))
	  s += "!!!!!";
	
	s += mother->get_name();
	sep = ", ";
	if (actuals_table != 0)
	  actuals_table->generate(s, cl, mother, FALSE, nodes, node_names);
      }
    }
    
    ++it;
  }
}

void ClassDialog::java_generate_decl(QString & s, ClassData * cl, QString def,
				     QString annotation, QString name, 
				     QString stereotype,
				     QString comment, bool is_public,
				     bool is_final, bool is_abstract,
				     ActualParamsTable * actuals,
				     FormalParamsTable * formals,
				     BrowserNodeList & nodes,
				     QStringList & node_names,
				     KeyValuesTable * kvt)
{
  const char * p = def;
  
  if (p == 0)
    return;
  
  QString indent = "";

  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  s += indent;
  
  const char * pp = 0;
  
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
      manage_comment(comment, p, pp);
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment, p, pp);
    else if (!strncmp(p, "${public}", 9)) {
      p += 9;
      if (is_public)
	s += "public ";
    }
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (is_final)
	s += "final ";
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (is_abstract)
	s += "abstract ";
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += name;
      formals->generate_java(s, nodes, node_names);
    }
    else if (!strncmp(p, "${extends}", 10)) {
      p += 10;
      java_generate_extends(s, stereotype, cl, actuals, nodes, node_names);
    }
    else if (!strncmp(p, "${implements}", 13)) {
      p += 13;
      java_generate_implements(s, stereotype, cl, actuals, nodes, node_names);
    }
    else if (!strncmp(p, "${members}", 10)) {
      p += 10;
      generate_members(cl->browser_node, s,
		       &AttributeData::get_javadecl, &OperationData::get_javadef, 
		       &RelationData::get_javadecl_a, &RelationData::get_javadecl_b,
		       &ExtraMemberData::get_java_decl, &ClassData::get_javadecl,
		       java_stereotype, FALSE, FALSE, (stereotype == "enum"));
    }
    else if (!strncmp(p, "${items}", 8)) {
      p += 8;
      generate_members(cl->browser_node, s,
		       &AttributeData::get_javadecl, &OperationData::get_javadef, 
		       &RelationData::get_javadecl_a, &RelationData::get_javadecl_b,
		       &ExtraMemberData::get_java_decl, &ClassData::get_javadecl,
		       java_stereotype, FALSE, TRUE, FALSE);
    }
    else if (!strncmp(p, "${cases}", 8)) {
      p += 8;
      generate_members(cl->browser_node, s,
		       &AttributeData::get_javadecl, &OperationData::get_javadef, 
		       &RelationData::get_javadecl_a, &RelationData::get_javadecl_b,
		       &ExtraMemberData::get_java_decl, &ClassData::get_javadecl,
		       java_stereotype);
    }
    else if (!strncmp(p, "${@}", 4)) {
      p += 4;
      if (pp != 0)
	s += "${@}";
      else if (! annotation.isEmpty()) {
	pp =p;
	p = annotation;
      }
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p)
	s += indent;
    }
    else if (*p == '@')
      manage_alias(cl->browser_node, p, s, kvt);
    else
      s += *p++;
  }
}

void ClassDialog::java_update_decl() {
  QString s;
  
  if (java_external_cb->isChecked())
    showjavadecl->setEnabled(FALSE);
  else {
    showjavadecl->setEnabled(TRUE);
    
    QString def = edjavadecl->text().stripWhiteSpace();

    java_generate_decl(s, cl, def, javaannotation,
		       edname->text().stripWhiteSpace(),
		       current_java_stereotype, comment->text(),
		       java_public_cb->isChecked(), java_final_cb->isChecked(),
		       abstract_cb->isChecked(), actuals_table,
		       formals_table, nodes, node_names, kvtable);

    editjavaannotation->setEnabled(def.find("${@}") != -1);
  }
  
  showjavadecl->setText(s);
}

void ClassDialog::java_default_decl() {
  if (java_external_cb->isChecked())
    edjavadecl->setText(GenerationSettings::java_default_external_class_decl());
  else {
    current_java_stereotype =
      java_stereotype(edstereotype->currentText().stripWhiteSpace());
    
    if (current_java_stereotype == "enum")
      edjavadecl->setText(GenerationSettings::java_default_enum_decl());
    else if (current_java_stereotype == "enum_pattern")
      edjavadecl->setText(GenerationSettings::java_default_enum_pattern_decl());
    else if (current_java_stereotype == "interface")
      edjavadecl->setText(GenerationSettings::java_default_interface_decl());
    else if (current_java_stereotype == "@interface") {
      QString s = GenerationSettings::java_default_interface_decl();
      int index = s.find("interface");
      
      if (index != -1)
	s.insert(index, '@');
      edjavadecl->setText(s);
    }
    else if (current_java_stereotype == "ignored")
      edjavadecl->setText(QString::null);
    else
      edjavadecl->setText(GenerationSettings::java_default_class_decl());
  
    java_update_decl();
  }
}

void ClassDialog::java_unmapped_decl() {
  edjavadecl->setText(QString::null);
  showjavadecl->setText(QString::null);
}

void ClassDialog::java_edit_annotation() {
  AnnotationDialog dialog(javaannotation, !hasOkButton());
  
  if (dialog.exec() == QDialog::Accepted)
    java_update_decl();
}
  
static void idl_generate_inherit(QString & s, QString st, ClassData * cl) {
  if ((st == "union") || (st == "enum"))
    return;
  
  BrowserNodeList inh;
  bool is_valuetype = (st == "valuetype");
  const char * sep = " : ";
  bool already = FALSE;
  
  cl->get_browser_node()->children(inh, UmlGeneralisation, UmlRealize);
  
  QListIterator<BrowserNode> it(inh);

  while (it.current() != 0) {
    RelationData * r = (RelationData *) it.current()->get_data();
    
    if (r->get_idldecl_a()[0]) {
      BrowserClass * mother = r->get_end_class();      
      QString other_st = 
	ClassDialog::idl_stereotype(mother->get_stereotype());
      
      if ((other_st != "ignored") &&
	  (other_st != "union") &&
	  (other_st != "struct") &&
	  (other_st != "enum") &&
	  (other_st != "typedef")) {
	if (is_valuetype) {
	  if (other_st == "valuetype") {
	    s += sep;
	    
	    if (r->get_idl_truncatable())
	      s += "truncatable ";
	  }
	  else {
	    // other_st == "interface"
	    if (!already) {
	      already = TRUE;
	      s += " supports ";
	    }
	    else
	      s += sep;
	  }
	}
	else
	  s += sep;
	
	s += mother->get_name();
	sep = ", ";
      }
    }
    
    ++it;
  }
}

void ClassDialog::idl_generate_decl(QString & s, ClassData * cl, QString def,
				    QString name, QString stereotype,
				    QString basetype, QString comment,
				    QString switch_type, bool is_local,
				    bool is_custom, bool is_abstract,
				    KeyValuesTable * kvt)
{
  const char * p = def;
  
  if (p == 0)
    return;
  
  QString indent = "";

  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  if (*p != '#')
    s += indent;
  
  const char * pp = 0;
  
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
      manage_comment(comment, p, pp);
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment, p, pp);
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (is_abstract)
	s += "abstract ";
    }
    else if (!strncmp(p, "${local}", 8)) {
      p += 8;
      if (is_local)
	s += "local ";
    }
    else if (!strncmp(p, "${custom}", 9)) {
      p += 9;
      if (is_custom)
	s += "custom ";
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += name;
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p && (*p != '#'))
	s += indent;
    }
    else if (stereotype == "typedef") {
      if (!strncmp(p, "${type}", 7)) {
	p += 7;
	s += GenerationSettings::idl_type(basetype);
      }
      else
	s += *p++;
    }
    else if (!strncmp(p, "${inherit}", 10)) {
      p += 10;
      idl_generate_inherit(s, stereotype, cl);
    }
    else if (!strncmp(p, "${members}", 10)) {
      p += 10;
      generate_members(cl->browser_node, s,
		       &AttributeData::get_idldecl, &OperationData::get_idldecl, 
		       &RelationData::get_idldecl_a, &RelationData::get_idldecl_b,
		       &ExtraMemberData::get_idl_decl, &ClassData::get_idldecl,
		       idl_stereotype, stereotype == "union");
    }
    else if (!strncmp(p, "${items}", 8)) {
      p += 8;
      generate_members(cl->browser_node, s,
		       &AttributeData::get_idldecl, &OperationData::get_idldecl, 
		       &RelationData::get_idldecl_a, &RelationData::get_idldecl_b,
		       &ExtraMemberData::get_idl_decl, &ClassData::get_idldecl,
		       idl_stereotype);
    }
    else if (!strncmp(p, "${switch}", 9)) {
      p += 9;
      s += switch_type;
    }
    else if (*p == '@')
      manage_alias(cl->browser_node, p, s, kvt);
    else
      s += *p++;
  }
}

void ClassDialog::idl_update_decl() {
  QString s;
  
  if (idl_external_cb->isChecked())
    showidldecl->setEnabled(FALSE);
  else {
    showidldecl->setEnabled(TRUE);
    
    idl_generate_decl(s, cl, edidldecl->text().stripWhiteSpace(),
		      edname->text().stripWhiteSpace(), current_idl_stereotype, 
		      type(edbasetype->currentText().stripWhiteSpace(),
			   node_names, nodes),
		      comment->text(),
		      edswitch_type->currentText().stripWhiteSpace(),
		      idl_local_cb->isChecked(), idl_custom_cb->isChecked(),
		      abstract_cb->isChecked(), kvtable);
  }
  
  showidldecl->setText(s);
}

void ClassDialog::idl_default_decl() {
  if (idl_external_cb->isChecked())
    edidldecl->setText(GenerationSettings::idl_default_external_class_decl());
  else {
    current_idl_stereotype =
      idl_stereotype(edstereotype->currentText().stripWhiteSpace());
    
    if (current_idl_stereotype == "struct")
      edidldecl->setText(GenerationSettings::idl_default_struct_decl());
    else if (current_idl_stereotype == "union")
      edidldecl->setText(GenerationSettings::idl_default_union_decl());
    else if (current_idl_stereotype == "enum")
      edidldecl->setText(GenerationSettings::idl_default_enum_decl());
    else if (current_idl_stereotype == "exception")
      edidldecl->setText(GenerationSettings::idl_default_exception_decl());
    else if (current_idl_stereotype == "typedef")
      edidldecl->setText(GenerationSettings::idl_default_typedef_decl());
    else if (current_idl_stereotype == "interface")
      edidldecl->setText(GenerationSettings::idl_default_interface_decl());
    else if (current_idl_stereotype == "ignored")
      edidldecl->setText(QString::null);
    else
      edidldecl->setText(GenerationSettings::idl_default_valuetype_decl());
    
    idl_update_decl();
  }
}

void ClassDialog::idl_unmapped_decl() {
  edidldecl->setText(QString::null);
  showidldecl->setText(QString::null);
}

void ClassDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserClass * bn = (BrowserClass *) cl->get_browser_node();
  QString s;
  
  s = edname->text().stripWhiteSpace();
  if (s != cl->name()) {
    if (((BrowserNode *) bn->parent())->wrong_child_name(s, UmlClass,
							 bn->allow_spaces(),
							 bn->allow_empty())) {
      QMessageBox::critical(0, "Error", s + "\n\nillegal name or already used");
      return;
    }
    
    bn->set_name(s);
  }
  
  s = fromUnicode(edstereotype->currentText().stripWhiteSpace());
  cl->set_stereotype(s);
  
  if (s == "typedef")
    cl->base_type = the_type(edbasetype->currentText().stripWhiteSpace(),
			     node_names, nodes);
  
  cl->is_abstract = abstract_cb->isChecked();
  
  if (bn->nestedp())
    cl->uml_visibility = uml_visibility.value();
  
  if (artifact != 0) {
    int index = artifact->currentItem();
    
    bn->set_associated_artifact((index == 0)
				 ? 0 // "" : no artifact
				 : ((BrowserArtifact *) artifacts.at(index - 1)));
  }
  
  formals_table->update(cl, nodes);
  
  if (cl->get_n_actualparams() != 0)
    actuals_table->update(cl, nodes);
  
  bn->set_comment(comment->text());
  UmlWindow::set_commented(bn);
  
  cl->cpp_external = cpp_external_cb->isChecked();
  cl->cpp_decl = edcppdecl->text();
  if (bn->nestedp())
    cl->cpp_visibility = cpp_visibility.value();
  
  cl->java_decl = edjavadecl->text();
  cl->java_public = java_public_cb->isChecked();
  cl->java_final = java_final_cb->isChecked();
  cl->java_external = java_external_cb->isChecked();
  cl->java_annotation = javaannotation;
  
  cl->idl_decl = edidldecl->text();  
  cl->idl_local = idl_local_cb->isChecked();
  cl->idl_custom = idl_custom_cb->isChecked();
  cl->idl_external = idl_external_cb->isChecked();
  if (switch_bg->isEnabled()) {
    AType swt;
    
    s = edswitch_type->currentText().stripWhiteSpace();
    
    if (! s.isEmpty()) {
      int index = node_names.findIndex(s);
      
      if (index >= 0)
	swt.type = (BrowserClass *) nodes.at(index);
      else
	swt.explicit_type = s;
    }
    
    cl->set_switch_type(swt);
  }
  
  kvtable->update(bn);
  
  bn->modified();
  bn->package_modified();
  cl->modified();
  
  QTabDialog::accept();
}

//
// Formal params
//

// copy/cut/paste
QString FormalParamsTable::name_copy;
QString FormalParamsTable::type_copy;
QString FormalParamsTable::default_value_copy;
QString FormalParamsTable::extends_copy;

FormalParamsTable::FormalParamsTable(ClassData * cl, QWidget * parent,
				     const QStringList & node_names, bool visit)
    : MyTable(cl->get_n_formalparams() + 1, (visit) ? 4 : 5, parent),
      types(node_names) {
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, "Type (C++)");
  horizontalHeader()->setLabel(1, "Name");
  horizontalHeader()->setLabel(2, "Default value (C++)");
  horizontalHeader()->setLabel(3, "Extends (Java)");
  if (! visit)
    horizontalHeader()->setLabel(4, "do");
  
  int index;
  int sup = cl->get_n_formalparams();
  
  if (visit) {
    for (index = 0; index < sup; index += 1) {
      setItem(index, 0, new TableItem(this, QTableItem::Never, cl->get_formalparam_type(index)));
      setItem(index, 1, new TableItem(this, QTableItem::Never, cl->get_formalparam_name(index)));
      setItem(index, 2, new TableItem(this, QTableItem::Never,
				      cl->get_formalparam_default_value(index, !node_names.isEmpty())));
      setItem(index, 3, new TableItem(this, QTableItem::Never,
				      cl->get_formalparam_extends(index, !node_names.isEmpty())));
    }
  }
  else {
    for (index = 0; index < sup; index += 1){
      setText(index, 0, cl->get_formalparam_type(index));
      setText(index, 1, cl->get_formalparam_name(index));
      setItem(index, 2, new ComboItem(this, cl->get_formalparam_default_value(index, TRUE), types));
      setItem(index, 3, new ComboItem(this, cl->get_formalparam_extends(index, TRUE), types));
      setText(index, 4, QString::null);
    }
    
    setText(index, 0, "class");
    setText(index, 1, QString::null);
    setItem(index, 2, new ComboItem(this, QString::null, types));
    setItem(index, 3, new ComboItem(this, QString::null, types));
    setText(index, 4, QString::null);
  }
  
  setColumnStretchable (0, TRUE);
  setColumnStretchable (1, TRUE);
  setColumnStretchable (2, TRUE);
  setColumnStretchable (3, TRUE);
  if (! visit) {
    adjustColumn(4);
    setColumnStretchable (4, FALSE);

    connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	    this, SLOT(button_pressed(int, int, int, const QPoint &)));
    connect(this, SIGNAL(valueChanged(int, int)),
	    this, SLOT(value_changed(int, int)));
  }
}

void FormalParamsTable::activateNextCell() {
  int row = currentRow();
  int col = currentColumn();
    
  if (col == 3) {
    // goto next line
    if (++row == numRows()) {
      // adds a new line
      setNumRows(row + 1);
      setText(row, 0, "class");
      setText(row, 1, QString::null);
      setItem(row, 2, new ComboItem(this, QString::null, types));
      setItem(row, 3, new ComboItem(this, QString::null, types));
      setText(row, 4, QString::null);
    }
    setCurrentCell(row, 0);
  }
  else
    setCurrentCell(row, col + 1);
}

void FormalParamsTable::value_changed(int row, int col) {
  if ((row == (numRows() - 1)) && !text(row, col).isEmpty())
    insert_row_after(row);
}

void FormalParamsTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == 4) {
    char s[16];
    
    sprintf(s, "formal param %d", row + 1);
    
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

void FormalParamsTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    setText(index, 0, text(index - 1, 0));
    setText(index, 1, text(index - 1, 1));
    
    QTableItem * it;
    
    it = item(index - 1, 2);
    takeItem(it);
    setItem(index, 2, it);
    
    it = item(index - 1, 3);
    takeItem(it);
    setItem(index, 3, it);
  }
  
  setText(row, 0, "class");
  setText(row, 1, QString::null);
  setItem(row, 2, new ComboItem(this, QString::null, types));
  setItem(row, 3, new ComboItem(this, QString::null, types));
  setText(row, 4, QString::null);
 }

void FormalParamsTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    setText(index, 0, text(index - 1, 0));
    setText(index, 1, text(index - 1, 1));
    
    QTableItem * it;
    
    it = item(index - 1, 2);
    takeItem(it);
    setItem(index, 2, it);
    
    it = item(index - 1, 3);
    takeItem(it);
    setItem(index, 3, it);
  }
  
  setText(row + 1, 0, "class");
  setText(row + 1, 1, QString::null);
  setItem(row + 1, 2, new ComboItem(this, QString::null, types));
  setItem(row + 1, 3, new ComboItem(this, QString::null, types));
  setText(row + 1, 4, QString::null);
 }

void FormalParamsTable::delete_row(int row) {
  int n = numRows();
  int index;

  clearCellWidget(row, 3);
    
  if (n == 1) {
    // the alone line : empty it
    setText(0, 0, "class");
    setText(0, 1, QString::null);
    setItem(0, 2, new ComboItem(this, QString::null, types));
    setItem(0, 3, new ComboItem(this, QString::null, types));
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      setText(index, 0, text(index + 1, 0));
      setText(index, 1, text(index + 1, 1));

      QTableItem * it;
      
      it = item(index + 1, 2);
      takeItem(it);
      setItem(index, 2, it);
      
      it = item(index + 1, 3);
      takeItem(it);
      setItem(index, 3, it);
    }
    setNumRows(n - 1);
  }
}

void FormalParamsTable::copy_row(int row) {
  type_copy = text(row, 0);
  name_copy = text(row, 1);
  default_value_copy = text(row, 2);
  extends_copy = text(row, 3);
}

void FormalParamsTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void FormalParamsTable::paste_row(int row) {
  setText(row, 0, type_copy);
  setText(row, 1, name_copy);
  setText(row, 2, default_value_copy);
  setText(row, 3, extends_copy);
  
  if ((row == (numRows() - 1)) && !name_copy.isEmpty())
    insert_row_after(row);
}

void FormalParamsTable::move_row(int from, int to) {
  QString save_type_copy = type_copy;
  QString save_name_copy = name_copy;
  QString save_default_value_copy = default_value_copy;
  QString save_extends_copy = extends_copy;
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  type_copy = save_type_copy;
  name_copy = save_name_copy;
  default_value_copy = save_default_value_copy;
  extends_copy = save_extends_copy;
}

void FormalParamsTable::generate_cpp(QString & s, BrowserNodeList & nodes,
				     QStringList & node_names,
				     const QString & cl_name) {
  int n = numRows();
  
  if (text(n - 1, 1).stripWhiteSpace().isEmpty())
    n -= 1;
  
  if (n != 0) {
    const char * sep = "template<";
    
    for (int index = 0; index != n; index += 1) {
      s += sep;
      s += text(index, 0).stripWhiteSpace();
      s += ' ';
      s += text(index, 1).stripWhiteSpace();
      sep = ", ";

      QString v = type(text(index, 2).stripWhiteSpace(), node_names, nodes);
    
      if (! v.isEmpty()) {
	s += " = ";
	
	s += v;
      }
    }

    s += "> ";
  }
  else if (cl_name.find('<') != -1)
    s += "template<> ";
}

void FormalParamsTable::generate_java(QString & s, BrowserNodeList & nodes,
				      QStringList & node_names) {
  int n = numRows();
  
  if (text(n - 1, 1).stripWhiteSpace().isEmpty())
    n -= 1;
  
  if (n != 0) {
    const char * sep = "<";
    
    for (int index = 0; index != n; index += 1) {
      s += sep;
      s += text(index, 1).stripWhiteSpace();
      
      QString ex = type(text(index, 3).stripWhiteSpace(), node_names, nodes);
      
      if (! ex.isEmpty()) {
	s += " extends ";
	s += ex;
      }
      
      sep = ", ";
    }

    s += "> ";
  }
}

void FormalParamsTable::update(ClassData * cl, BrowserNodeList & nodes) {
  forceUpdateCells();
  
  int n = numRows();
  int index;
  
  if (text(n - 1, 1).stripWhiteSpace().isEmpty())
    n -= 1;
  
  cl->set_n_formalparams(n);
  
  for (index = 0; index != n; index += 1) {
    cl->set_formalparam_type(index, text(index, 0).stripWhiteSpace());
    cl->set_formalparam_name(index, text(index, 1).stripWhiteSpace());
    
    int rank;
    
    rank = types.findIndex(text(index, 2).stripWhiteSpace());
    if (rank != -1) 
      cl->set_formalparam_default_value(index, ((BrowserClass *) nodes.at(rank)),
					QString::null);
    else
      cl->set_formalparam_default_value(index, 0, text(index, 2).stripWhiteSpace());
    
    rank = types.findIndex(text(index, 3).stripWhiteSpace());
    if (rank != -1) 
      cl->set_formalparam_extends(index, ((BrowserClass *) nodes.at(rank)),
				  QString::null);
    else
      cl->set_formalparam_extends(index, 0, text(index, 3).stripWhiteSpace());
  }
}

//
// Actual params
//

ActualParamsTable::ActualParamsTable(ClassData * cl, QWidget * parent,
				     const QStringList & node_names, bool visit)
    : MyTable(cl->get_n_actualparams(), 2, parent), types(node_names) {
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(FALSE);
  horizontalHeader()->setLabel(0, "inherited class formal");
  horizontalHeader()->setLabel(1, "actual");
  
  int index;
  int sup = cl->get_n_actualparams();
  
  for (index = 0; index < sup; index += 1){
    setItem(index, 0, new TableItem(this, QTableItem::Never, cl->get_actualparam_name(index)));
    if (visit)
      setItem(index, 1, new TableItem(this, QTableItem::Never, cl->get_actualparam_value(index)));
    else
      setItem(index, 1, new ComboItem(this, cl->get_actualparam_value(index), types));
  }
  
  adjustColumn(0);
  setColumnStretchable (0, FALSE);
  setColumnStretchable (1, TRUE);
}

#if 0
void ActualParamsTable::activateNextCell() {
  int row = currentRow();
    
  if (row != numRows())
    // goto next line
    setCurrentCell(row + 1, 1);
}
#endif

void ActualParamsTable::update(ClassData * cl, BrowserNodeList & nodes) {
  forceUpdateCells();
  
  int n = numRows();
  int index;
  
  for (index = 0; index != n; index += 1)
    cl->set_actualparam_value(index, 
			      the_type(text(index, 1).stripWhiteSpace(),
				       types, nodes));
}

void ActualParamsTable::generate(QString & s, ClassData * cl,
				 BrowserNode * parent, bool cpp,
				 BrowserNodeList & nodes,
				 QStringList & node_names) {
  ActualParamData * actual;
  unsigned index;
  
  for (actual = cl->actuals.first(), index = 0;
       actual != 0;
       actual = cl->actuals.next(), index += 1) {
    if (actual->get_class() == parent)
      // find;
      break;
  }
  
  int n = ((ClassData *) parent->get_data())->get_n_formalparams();
  int nth = 0;
  const char * sep = "<";
  
  // progress on still present formals
  while (actual && (nth < n) && (actual->get_class() == parent)) {
    QString t = type(text(index, 1).stripWhiteSpace(), node_names, nodes);
    
    s += sep;
    s += (cpp) ? GenerationSettings::cpp_type(t) 
	       : GenerationSettings::java_type(t);
    sep = ", ";
    actual = cl->actuals.next();
    nth += 1;
    index += 1;
  }
  
  if (nth != 0)
    s += '>';
}
