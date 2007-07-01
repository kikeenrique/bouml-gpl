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

#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qradiobutton.h> 
#include <qpushbutton.h> 
#include <qsplitter.h> 

#include "RelationDialog.h"
#include "RelationData.h"
#include "BrowserRelation.h"
#include "ClassData.h"
#include "BrowserView.h"
#include "BrowserClass.h"
#include "KeyValueTable.h"
#include "UmlPixmap.h"
#include "DialogUtil.h"
#include "ClassDialog.h"
#include "GenerationSettings.h"
#include "strutil.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "AnnotationDialog.h"

QSize RelationDialog::previous_size;

static const struct {
  UmlCode type;
  const char * lbl;
  QPixmap ** pix;
} RelTypes[] = {
  {UmlGeneralisation, "generalisation", &generalisationButton },
  {UmlDependency, "dependency", &dependencyButton},
  {UmlAssociation, "association", &associationButton},
  {UmlDirectionalAssociation, "unidirectional association", &directionalAssociationButton},
  {UmlAggregation, "aggregation", &aggregationButton},
  {UmlAggregationByValue, "aggregation by value", &aggregationByValueButton},
  {UmlDirectionalAggregation, "directional aggregation", &directionalAggregationButton},
  {UmlDirectionalAggregationByValue, "directional aggregation by value", &directionalAggregationByValueButton},
  {UmlRealize, "realization", &realizeButton}
};

static const struct {
  UmlCode type;
  const char * lbl;
  QPixmap ** pix;
} UniRelTypes[] = {
  {UmlGeneralisation, "generalisation", &generalisationButton },
  {UmlDependency, "dependency", &dependencyButton},
  {UmlDirectionalAssociation, "unidirectional association", &directionalAssociationButton},
  {UmlDirectionalAggregation, "directional aggregation", &directionalAggregationButton},
  {UmlDirectionalAggregationByValue, "directional aggregation by value", &directionalAggregationByValueButton},
  {UmlRealize, "realization", &realizeButton}
};

static const struct {
  UmlCode type;
  const char * lbl;
  QPixmap ** pix;
} BiRelTypes[] = {
  {UmlAssociation, "association", &associationButton},
  {UmlAggregation, "aggregation", &aggregationButton},
  {UmlAggregationByValue, "aggregation by value", &aggregationByValueButton},
};

RelationDialog::RelationDialog(RelationData * r)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), rel(r) {
  setCaption("Relation dialog");

  a.visit = !r->start->is_writable();
  b.visit = (r->end) ? !r->end->is_writable()
		     : !r->end_removed_from->is_writable();
  
  if (!a.visit || !b.visit)
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }
  
  r->start->edit_start();
  
  UmlCode type = rel->get_type();
  
  current_type = type;
  
  //
  // general tab
  //
  
  QVBox * vtab = new QVBox(this);
  QHBox * htab;
  QGroupBox *  bg;

  umltab = vtab;
  vtab->setMargin(3);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  QLabel * lbl1 = new QLabel("name : ", htab);
  edname = new LineEdit(rel->get_name(), htab);
  edname->setReadOnly(a.visit);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  QLabel * lbl2 = new QLabel("type : ", htab);
  
  edtype = new QComboBox(FALSE, htab);
  
  int index;
  bool assoc = true;
    
  if (a.visit) {
    // relation kind cannot be changed
    for (index = 0; index != sizeof(RelTypes) / sizeof(*RelTypes); index += 1) {
      if (RelTypes[index].type == type) {
	edtype->insertItem(**RelTypes[index].pix, RelTypes[index].lbl);
	edtype->setCurrentItem(0);
	assoc = RelationData::isa_association(type);
	break;
      }
    }
  }
  else {
    if (b.visit) {
      // bi/uni dir cannot be changed
      if (r->end == 0) {
	// unidir
	for (index = 0; index != sizeof(UniRelTypes) / sizeof(*UniRelTypes); index += 1) {
	  edtype->insertItem(**UniRelTypes[index].pix, UniRelTypes[index].lbl);
	  if (UniRelTypes[index].type == type)
	    edtype->setCurrentItem(index);
	}
      }
      else {
	// bidir
	for (index = 0; index != sizeof(BiRelTypes) / sizeof(*BiRelTypes); index += 1) {
	  edtype->insertItem(**BiRelTypes[index].pix, BiRelTypes[index].lbl);
	  if (BiRelTypes[index].type == type)
	    edtype->setCurrentItem(index);
	}
      }
    }
    else {
      for (index = 0; index != sizeof(RelTypes) / sizeof(*RelTypes); index += 1) {
	edtype->insertItem(**RelTypes[index].pix, RelTypes[index].lbl);
	if (RelTypes[index].type == type)
	  edtype->setCurrentItem(index);
      }
    }
    
    connect(edtype, SIGNAL(activated(int)), this, SLOT(edTypeActivated(int)));
  }
  
  new QLabel("    stereotype : ", htab);
  edstereotype = new QComboBox(!a.visit, htab);
  edstereotype->insertItem(toUnicode(rel->get_stereotype()));
  if (!a.visit) {
    edstereotype->insertStringList(rel->get_start_class()
				   ->default_stereotypes(type,
							 rel->get_end_class()));
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  QLabel * lbl3 = new QLabel("association : ", htab);
    
  edassociation = new QComboBox(!a.visit, htab);
  edassociation->insertItem(rel->association.get_full_type());
  if (!a.visit) {
    BrowserClass::instances(nodes);
    nodes.full_names(list);
    
    edassociation->insertStringList(GenerationSettings::basic_types());
    edassociation->insertStringList(list);
    edassociation->setAutoCompletion(TRUE);
  }
  edassociation->setCurrentItem(0);
  edassociation->setSizePolicy(sp);
  
  same_width(lbl1, lbl2, lbl3);
    
  QSplitter * split = new QSplitter(Vertical, vtab);
  
  split->setOpaqueResize(TRUE);
    
  QString ina = "in " + rel->get_start_class()->full_name(TRUE);
  QString inb = "in " + rel->get_end_class()->full_name(TRUE);
  
  // role A
  bg = new QGroupBox(2, QGroupBox::Horizontal, ina, split);
  init_uml_role(a, rel->a, bg, rel->get_start_class());
  
  // role B
  bg = new QGroupBox(2, QGroupBox::Horizontal, inb, split);
  init_uml_role(b, rel->b, bg, rel->get_end_class());

  addTab(vtab, "Uml");
  
  //
  // C++
  //
  
  vtab = new QVBox(this);
  cpptab = vtab;
  vtab->setMargin(5);
  
  // A
  bg = new QGroupBox(2, QGroupBox::Horizontal, ina, vtab); 
  new QLabel(bg);
  a.cpp_virtual_inheritance_cb = new QCheckBox("virtual", bg);
  if (rel->a.cpp_virtual_inheritance)
    a.cpp_virtual_inheritance_cb->setChecked(TRUE);
  if (a.visit)
    a.cpp_virtual_inheritance_cb->setEnabled(FALSE);
  else
    connect(a.cpp_virtual_inheritance_cb, SIGNAL(toggled(bool)), this, SLOT(cpp_update_a()));
  init_cpp_role(a, rel->a, bg, SLOT(cpp_update_a()),
		SLOT(cpp_default_a()),SLOT(cpp_unmapped_a()),
		SLOT(cpp_include_in_header()));

  // B
  htab = new QHBox(vtab);	// to have a vertical margin
  htab->setMargin(5);
  
  cpp_b = new QGroupBox(2, QGroupBox::Horizontal, inb, vtab);  
  b.cpp_virtual_inheritance_cb = 0;
  init_cpp_role(b, rel->b, cpp_b, SLOT(cpp_update_b()),
		SLOT(cpp_default_b()), SLOT(cpp_unmapped_b()), 0);

  addTab(vtab, "C++");
  
  //
  // Java
  //
  
  vtab = new QVBox(this);
  javatab = vtab;
  vtab->setMargin(5);
  
  // A
  bg = new QGroupBox(2, QGroupBox::Horizontal, ina, vtab); 
  init_java_role(a, rel->a, bg, SLOT(java_update_a()), SLOT(java_default_a()),
		 SLOT(java_unmapped_a()), SLOT(java_edit_annotation_a()));
  
  // B
  htab = new QHBox(vtab);	// to have a vertical margin
  htab->setMargin(5);
  
  java_b = new QGroupBox(2, QGroupBox::Horizontal, inb, vtab);
  init_java_role(b, rel->b, java_b, SLOT(java_update_b()), SLOT(java_default_b()),
		 SLOT(java_unmapped_b()), SLOT(java_edit_annotation_b()));

  addTab(vtab, "Java");
  
  //
  // IDL
  //
  vtab = new QVBox(this);
  idltab = vtab;
  vtab->setMargin(5);
  
  // A
  bg = new QGroupBox(2, QGroupBox::Horizontal, ina, vtab); 
  
  ClassData * start_data = (ClassData *) rel->get_start_class()->get_data();
  ClassData * end_data = (ClassData *) rel->get_end_class()->get_data();
  
  if ((ClassDialog::idl_stereotype(start_data->get_stereotype())
       == "valuetype") &&
      (ClassDialog::idl_stereotype(end_data->get_stereotype())
       == "valuetype")) {
    new QLabel(bg);
    a.idl_truncatable_inheritance_cb = new QCheckBox("truncatable", bg);
    if (rel->a.idl_truncatable_inheritance)
      a.idl_truncatable_inheritance_cb->setChecked(TRUE);
    if (a.visit)
      a.idl_truncatable_inheritance_cb->setEnabled(FALSE);
    else
      connect(a.idl_truncatable_inheritance_cb, SIGNAL(toggled(bool)), this,
	      SLOT(idl_update_a()));
  }
  else
    a.idl_truncatable_inheritance_cb = 0;
  init_idl_role(a, rel->a, start_data, bg, SLOT(idl_update_a()),
		SLOT(idl_default_a()), SLOT(idl_unmapped_a()));

  // B
  htab = new QHBox(vtab);	// to have a vertical margin
  htab->setMargin(5);
  
  idl_b = new QGroupBox(2, QGroupBox::Horizontal, inb, vtab);    
  b.idl_truncatable_inheritance_cb = 0;
  init_idl_role(b, rel->b, end_data, idl_b, SLOT(idl_update_b()),
		SLOT(idl_default_b()), SLOT(idl_unmapped_b()));

  addTab(vtab, "IDL");
  
  //
  // USER : list key - value
  //
  
  vtab = new QVBox(this);
  bg = new QGroupBox(1, QGroupBox::Horizontal, ina, vtab); 
  a.kvtable = new KeyValuesTable(rel->get_start(), bg, a.visit);
  new QLabel(vtab);
  bg = new QGroupBox(1, QGroupBox::Horizontal, inb, vtab); 
  b.opt.append(bg);
  b.kvtable = new KeyValuesTable(rel->get_end(), bg, b.visit);
  addTab(vtab, "Properties");
  
  //
  
  edTypeActivated(edtype->currentItem());
  
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(update_all_tabs(QWidget *)));
}

RelationDialog::~RelationDialog() {
  rel->start->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void RelationDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void RelationDialog::init_uml_role(RoleDialog & role, const RoleData & rel,
				   QGroupBox * bg,
				   BrowserClass * cl1) {
  bool roleb = (&role == &b);
  QString stereotype = ((ClassData *) cl1->get_data())->get_stereotype();
  bool undef = (stereotype == "enum") || (stereotype == "typedef");
  QString st;
  QLabel * lbl;
  
  st = ClassDialog::cpp_stereotype(stereotype);
  role.cpp_undef = undef || (st == "enum") || (st == "typedef");
  
  st = ClassDialog::java_stereotype(stereotype);
  role.java_undef = undef || (st == "enum") || (st == "typedef");
  
  st = ClassDialog::idl_stereotype(stereotype);
  role.idl_undef = undef || (st == "enum") || (st == "typedef");
  role.idl_in_struct = (st == "struct");
  role.idl_in_union = (st == "union");
  role.idl_in_valuetype = (st == "valuetype");
  
  lbl = new QLabel("name : ", bg);
  role.opt.append(lbl);
  role.edrole = new LineEdit(rel.role, bg);
  role.edrole->setReadOnly(role.visit);
  role.opt.append(role.edrole);
  if (roleb) {
    groupb.append(role.edrole);
    groupb.append(lbl);
  }
  
  QHBox * htab;
  QSizePolicy sp;
  
  role.opt.append(new QLabel("multiplicity : ", bg));
  htab = new QHBox(bg);
  htab->setMargin(0);
  role.multiplicity = new QComboBox(!role.visit, htab);
  sp = role.multiplicity->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  role.multiplicity->setSizePolicy(sp);
  role.multiplicity->insertItem(rel.multiplicity);
  if (!role.visit) {
    role.multiplicity->insertItem("1");
    role.multiplicity->insertItem("0..1");
    role.multiplicity->insertItem("*");
    role.multiplicity->insertItem("1..*");
  }
  role.opt.append(role.multiplicity);  
  
  lbl = new QLabel("   initial value : ", htab);
  role.opt.append(lbl);
  role.edinit = new LineEdit(rel.init_value, htab);
  role.opt.append(role.edinit);  
  if (roleb) {
    groupb.append(role.edinit);
    groupb.append(lbl);
  }
  if (role.visit)
    role.edinit->setReadOnly(TRUE);
  else {
    SmallPushButton * bt = new SmallPushButton("Editor", htab);
    
    connect(bt, SIGNAL(clicked()),
	    this, (roleb) ? SLOT(edit_init_b()) : SLOT(edit_init_a()));
    if (roleb)
      groupb.append(bt);
  }
  
  (void) new QLabel(bg);
  htab = new QHBox(bg);
  htab->setMargin(0);
  QButtonGroup * bg2 = new QButtonGroup(3, QGroupBox::Horizontal, QString::null, htab);
  
  role.opt.append(bg2);
  if (roleb)
    groupb.append(bg2);
  role.classrelation_cb = new QCheckBox("class relation", bg2);
  if (rel.isa_class_relation)
    role.classrelation_cb->setChecked(TRUE);
  role.volatile_cb = new QCheckBox("volatile", bg2);
  if (rel.isa_volatile_relation)
    role.volatile_cb->setChecked(TRUE);
  
  role.constrelation_cb = new QCheckBox("read-only", bg2);
  if (rel.isa_const_relation)
    role.constrelation_cb->setChecked(TRUE);
  if (role.visit) {
    role.classrelation_cb->setEnabled(FALSE);
    role.volatile_cb->setEnabled(FALSE);
    role.constrelation_cb->setEnabled(FALSE);
  }
  
  QButtonGroup * vg = role.uml_visibility.init(htab, rel.uml_visibility, TRUE);
  
  if (role.visit)
    vg ->setEnabled(FALSE);
  else if (roleb)
    groupb.append(vg);

  lbl = new QLabel("description : ", bg);
  if (roleb)
    groupb.append(lbl);
  htab = new QHBox(bg);
  role.comment = new MultiLineEdit(htab);
  role.comment->setText(rel.comment);  
  QFont font = role.comment->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  role.comment->setFont(font);
  if (role.visit)
    role.comment->setReadOnly(TRUE);
  else {
    QVBox * vtab = new QVBox(htab);
    SmallPushButton * bt1 = new SmallPushButton("Editor", vtab);
    
    connect(bt1, SIGNAL(clicked()), this,
	    (roleb) ? SLOT(edit_description_b()) : SLOT(edit_description_a()));
    
    SmallPushButton * bt2 = new SmallPushButton("Default", vtab);
    
    connect(bt2, SIGNAL(clicked()), this,
	    (roleb) ? SLOT(default_description_b()) : SLOT(default_description_a()));
    
    if (roleb) {
      groupb.append(lbl);
      groupb.append(role.comment);
      groupb.append(bt1);
      groupb.append(bt2);
    }
  }

  lbl = new QLabel("constraint : ", bg);
  if (roleb)
    groupb.append(lbl);
  htab = new QHBox(bg);
  role.constraint = new MultiLineEdit(htab);
  role.constraint->setText(rel.constraint);  
  role.constraint->setFont(font);
  if (role.visit)
    role.constraint->setReadOnly(TRUE);
  else {
    QVBox * vtab = new QVBox(htab);
    SmallPushButton * bt1 = new SmallPushButton("Editor", vtab);
    
    connect(bt1, SIGNAL(clicked()), this,
	    (roleb) ? SLOT(edit_constraint_b()) : SLOT(edit_constraint_a()));
    
    if (roleb) {
      groupb.append(lbl);
      groupb.append(role.constraint);
    }
  }
}

void RelationDialog::default_description_a() {
  a.comment->setText(GenerationSettings::default_relation_description());
}

void RelationDialog::edit_description_a() {
  edit(a.comment->text(), a.edrole->text().stripWhiteSpace() + "_description",
       &rel->a, TxtEdit, this, (post_edit) post_edit_description_a, edits);
}

void RelationDialog::post_edit_description_a(RelationDialog * d, QString s)
{
  d->a.comment->setText(s);
}

void RelationDialog::default_description_b() {
  b.comment->setText(GenerationSettings::default_relation_description());
}

void RelationDialog::edit_description_b() {
  edit(b.comment->text(), a.edrole->text().stripWhiteSpace() + "_description",
       &rel->b, TxtEdit, this, (post_edit) post_edit_description_b, edits);
}

void RelationDialog::post_edit_description_b(RelationDialog * d, QString s)
{
  d->b.comment->setText(s);
}

void RelationDialog::edit_constraint_a() {
  edit(a.constraint->text(), a.edrole->text().stripWhiteSpace() + "_constraint",
       &rel->a, TxtEdit, this, (post_edit) post_edit_constraint_a, edits);
}

void RelationDialog::post_edit_constraint_a(RelationDialog * d, QString s)
{
  d->a.constraint->setText(s);
}

void RelationDialog::edit_constraint_b() {
  edit(b.constraint->text(), a.edrole->text().stripWhiteSpace() + "_constraint",
       &rel->b, TxtEdit, this, (post_edit) post_edit_constraint_b, edits);
}

void RelationDialog::post_edit_constraint_b(RelationDialog * d, QString s)
{
  d->b.constraint->setText(s);
}

void RelationDialog::init_cpp_role(RoleDialog & role, const RoleData & rel,
				   QGroupBox * bg,
				   const char * cpp_update_slot, 
				   const char * cpp_default_slot,
				   const char * cpp_unmapped_slot,
				   const char * cpp_include_in_header_slot) {
  QHBox * htab;
  
  new QLabel("Visibility : ", bg);
  htab = new QHBox(bg);
  role.cpp_visibility.init(htab, rel.cpp_visibility, FALSE, 0, "follow uml")
    ->setEnabled(!role.visit);

  new QLabel(" ", htab);
  
  role.mutable_cb = new QCheckBox("mutable", htab);
  role.opt.append(role.mutable_cb);
  if (rel.cpp_mutable)
    role.mutable_cb->setChecked(TRUE);
  if (role.visit)
    role.mutable_cb->setEnabled(FALSE);
  else
    connect(role.mutable_cb, SIGNAL(toggled(bool)), this, cpp_update_slot);
  
  role.opt.append(new QLabel("Declaration : ", bg));
  role.edcppdecl = new MultiLineEdit(bg);
  QFont font = role.edcppdecl->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  role.opt.append(role.edcppdecl);
  role.edcppdecl->setText(rel.cpp_decl);
  role.edcppdecl->setFont(font);
  if (role.visit)
    role.edcppdecl->setReadOnly(TRUE);
  else
    connect(role.edcppdecl, SIGNAL(textChanged()), this, cpp_update_slot);

  new QLabel("Result after\nsubstitution : ", bg);
  role.showcppdecl = new MultiLineEdit(bg);
  role.showcppdecl->setReadOnly(TRUE);
  role.showcppdecl->setFont(font);

  if (! role.visit) {
    new QLabel("", bg);
    htab = new QHBox(bg);
    htab->setMargin(5);
    
    role.cpp_default_decl_bt = new QPushButton("Default declaration", htab);
    connect(role.cpp_default_decl_bt, SIGNAL(pressed()),
	    this, cpp_default_slot);
    if (cpp_include_in_header_slot != 0) {
      role.cpp_include_in_header = new QPushButton("#include in header", htab);
      connect(role.cpp_include_in_header , SIGNAL(pressed()),
	      this, cpp_include_in_header_slot);
    }
    role.cpp_unmapped_decl_bt = new QPushButton("Not generated in C++", htab);
    connect(role.cpp_unmapped_decl_bt, SIGNAL(pressed()),
	    this, cpp_unmapped_slot);
  }
}

void RelationDialog::init_java_role(RoleDialog & role, const RoleData & rel,
				    QGroupBox * bg,
				    const char * java_update_slot, 
				    const char * java_default_slot,
				    const char * java_unmapped_slot,
				    const char * java_edit_annotation) {
  if (! role.java_undef) {
    new QLabel("", bg);
    role.transient_cb = new QCheckBox("transient", bg);
    role.opt.append(role.transient_cb);
    if (rel.java_transient)
      role.transient_cb->setChecked(TRUE);
    if (role.visit)
      role.transient_cb->setEnabled(FALSE);
    else
      connect(role.transient_cb, SIGNAL(toggled(bool)), this, java_update_slot);
  }
  
  role.opt.append(new QLabel("Declaration : ", bg));
  role.edjavadecl = new MultiLineEdit(bg);
  role.opt.append(role.edjavadecl);
  QFont font = role.edjavadecl->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  role.edjavadecl->setText(rel.java_decl);
  role.edjavadecl->setFont(font);
  if (role.visit)
    role.edjavadecl->setReadOnly(TRUE);
  else
    connect(role.edjavadecl, SIGNAL(textChanged()), this, java_update_slot);

  role.opt.append(new QLabel("Result after\nsubstitution : ", bg));
  role.showjavadecl = new MultiLineEdit(bg);
  role.opt.append(role.showjavadecl);
  role.showjavadecl->setReadOnly(TRUE);
  role.showjavadecl->setFont(font);

  new QLabel("", bg);
  QHBox * htab = new QHBox(bg);
  htab->setMargin(5);
  
  if (! role.visit) {
    role.java_default_decl_bt = new QPushButton("Default declaration", htab);
    connect(role.java_default_decl_bt, SIGNAL(pressed()),
	    this, java_default_slot);
    role.java_unmapped_decl_bt = new QPushButton("Not generated in Java", htab);
    connect(role.java_unmapped_decl_bt, SIGNAL(pressed()),
	    this, java_unmapped_slot);
  }
  
  role.javaannotation = (const char *) rel.java_annotation;
  role.editjavaannotation =
    new QPushButton((role.visit) ? "Show annotation" : "Edit annotation",
		    htab);
  connect(role.editjavaannotation, SIGNAL(clicked ()),
	  this, java_edit_annotation);
}

void RelationDialog::init_idl_role(RoleDialog & role, const RoleData & rel,
				   ClassData * cld, QGroupBox * bg,
				   const char * idl_update_slot, 
				   const char * idl_default_slot,
				   const char * idl_unmapped_slot) {
  if (role.idl_in_union) {
    role.opt.append(new QLabel("Case : ", bg));
    role.edcase = new QComboBox(!role.visit, bg);
    role.edcase->insertItem(RelationData::get_idlcase(rel));
    
    if (!role.visit) {
      role.edcase->setAutoCompletion(TRUE);
      
      AType switch_type = cld->get_switch_type();
      
      if (switch_type.type != 0) {
	switch_type.type->children(role.enums, UmlAttribute);
	role.enums.names(role.enum_names);
	role.edcase->insertStringList(role.enum_names);
      }
    }
    role.edcase->setCurrentItem(0);
    
    QSizePolicy sp = role.edcase->sizePolicy();
    
    sp.setHorData(QSizePolicy::Expanding);
    role.edcase->setSizePolicy(sp);
    if (!role.visit)
      connect(role.edcase, SIGNAL(activated(int)), this, idl_update_slot);      
  }
    
  role.opt.append(new QLabel("Declaration : ", bg));
  role.edidldecl = new MultiLineEdit(bg);
  role.opt.append(role.edidldecl);
  role.edidldecl->setText(rel.idl_decl);
  QFont font = role.edidldecl->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  role.edidldecl->setFont(font);
  if (role.visit)
    role.edidldecl->setReadOnly(TRUE);
  else
    connect(role.edidldecl, SIGNAL(textChanged()), this, idl_update_slot);

  role.opt.append(new QLabel("Result after\nsubstitution : ", bg));
  role.showidldecl = new MultiLineEdit(bg);
  role.opt.append(role.showidldecl);
  role.showidldecl->setReadOnly(TRUE);
  role.showidldecl->setFont(font);

  if (! role.visit) {
    new QLabel("", bg);
    QHBox * htab = new QHBox(bg);
    htab->setMargin(5);
    role.idl_default_decl_bt = new QPushButton("Default declaration", htab);
    connect(role.idl_default_decl_bt, SIGNAL(pressed()),
	    this, idl_default_slot);
    role.idl_unmapped_decl_bt = new QPushButton("Not generated in Idl", htab);
    connect(role.idl_unmapped_decl_bt, SIGNAL(pressed()),
	    this, idl_unmapped_slot);
  }
}

static void set_enabled(QList<QWidget> & l, bool y)
{
  QWidget * w;
  
  for (w = l.first(); w != 0; w = l.next())
    w->setEnabled(y);
}

void RelationDialog::set_inherit_or_dependency(UmlCode type)
{
  if (! a.visit) {
    switch (type) {
    case UmlRealize:
    case UmlGeneralisation:
      a.cpp_virtual_inheritance_cb->setEnabled(TRUE);
      if (! a.java_undef) a.transient_cb->setEnabled(FALSE);
      if (a.idl_truncatable_inheritance_cb != 0)
	a.idl_truncatable_inheritance_cb->setEnabled(TRUE);
      break;
    case UmlDependency:
      a.cpp_virtual_inheritance_cb->setEnabled(FALSE);
      if (! a.java_undef) a.transient_cb->setEnabled(FALSE);
      if (a.idl_truncatable_inheritance_cb != 0)
	a.idl_truncatable_inheritance_cb->setEnabled(FALSE);
      break;
    default:
      a.cpp_virtual_inheritance_cb->setEnabled(FALSE);
      if (! a.java_undef) a.transient_cb->setEnabled(TRUE);
      if (a.idl_truncatable_inheritance_cb != 0)
	a.idl_truncatable_inheritance_cb->setEnabled(FALSE);
    }
  }
}

void RelationDialog::edTypeActivated(int r)
{
  UmlCode type;
  
  if (a.visit)
    type = current_type;
  else if (b.visit)
    type = (rel->end == 0) ? UniRelTypes[r].type : BiRelTypes[r].type;
  else
    type = RelTypes[r].type;
  
  if ((edname->text() == RelationData::default_name(current_type)) ||
      !RelationData::isa_association(type))
    edname->setText(RelationData::default_name(type));
  
  if (!RelationData::isa_association(type)) {
    edname->setEnabled(FALSE);
    edassociation->setEnabled(FALSE);
    ::set_enabled(a.opt, type != UmlDependency);
    // note : management of dependency for buttons presence
    // and text done in update_all_tabs() to take into account
    // stereotype change
    
    ::set_enabled(groupb, FALSE);
    cpp_b->setEnabled(FALSE);
    java_b->setEnabled(FALSE);
    idl_b->setEnabled(FALSE);
  }
  else {
    if (! a.visit) {
      edname->setEnabled(TRUE);
      edassociation->setEnabled(TRUE);
      ::set_enabled(a.opt, TRUE);
    }
    
    if (RelationData::uni_directional(type)) {
      ::set_enabled(groupb, FALSE);
      cpp_b->setEnabled(FALSE);
      java_b->setEnabled(FALSE);
      idl_b->setEnabled(FALSE);
    }
    else if (! b.visit) {
      ::set_enabled(groupb, TRUE);
      cpp_b->setEnabled(TRUE);
      java_b->setEnabled(TRUE);
      idl_b->setEnabled(TRUE);
    }
  }
  
  if (current_type != type) {
    current_type = type;
    if (!a.visit) {
      edstereotype->clear();
      edstereotype->insertItem("");
      edstereotype->insertStringList(rel->get_start_class()
				     ->default_stereotypes(type,
							   rel->get_end_class()));
      edstereotype->setCurrentItem(0);
    
      if (!a.edcppdecl->text().isEmpty())
	cpp_default_a();
      if (!a.edjavadecl->text().isEmpty())
	java_default_a();
      if (!a.edidldecl->text().isEmpty())
	idl_default_a();
    }
    if (! b.visit) {    
      if (!b.edcppdecl->text().isEmpty())
	cpp_default_b();
      if (!b.edjavadecl->text().isEmpty())
	java_default_b();
      if (!b.edidldecl->text().isEmpty())
	idl_default_b();
    }
  }
  
  set_inherit_or_dependency(type);
}

void RelationDialog::update_all_tabs(QWidget * w) {  
  if (current_type == UmlDependency) {
    QString s = a.edcppdecl->text().stripWhiteSpace();
    
    if (GenerationSettings::cpp_relation_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()))
	!= "friend") {
      if (! a.visit) {
	a.cpp_include_in_header->show();
	a.cpp_default_decl_bt->setEnabled(TRUE);
	a.cpp_unmapped_decl_bt->setEnabled(TRUE);
	a.cpp_default_decl_bt->setText("#include in source");
      }
      if (!s.isEmpty() &&
	  (s != "#include in source") &&
	  (s != "#include in header"))
	s = "#include in source";
    }
    else {
      if (! a.visit) {
	a.cpp_include_in_header->hide();
	a.cpp_default_decl_bt->setText("Default declaration");
	a.cpp_default_decl_bt->setEnabled(FALSE);
	a.cpp_unmapped_decl_bt->setEnabled(FALSE);
      }
      s = "friend " +
	ClassDialog::cpp_stereotype(rel->get_end_class()->get_stereotype()) + 
	  " ${type};";
    }
    a.edcppdecl->setText(s);
  }
  else if (! a.visit) {
    a.cpp_include_in_header->hide();
    a.cpp_default_decl_bt->setText("Default declaration");
  }
  
  a.edrole->setText(a.edrole->text().stripWhiteSpace());
  b.edrole->setText(b.edrole->text().stripWhiteSpace());
  
  if (w == umltab) {
    if (! a.visit)
      a.edrole->setFocus();
  }
  else if (w == cpptab) {
    cpp_update_a();
    cpp_update_b();
    if (! a.visit)
      a.edcppdecl->setFocus();
  }
  else if (w == javatab) {
    java_update_a();
    java_update_b();
    if (! a.visit)
      a.edjavadecl->setFocus();
  }
  else if (w == idltab) {
    idl_update_a();
    idl_update_b();
    if (! a.visit)
      a.edidldecl->setFocus();
  }
}

void RelationDialog::edit_init_a() {
  edit(a.edinit->text(), a.edrole->text().stripWhiteSpace() + "_initialization",
       &rel->a, TxtEdit, this, (post_edit) post_edit_init_a, edits);
}

void RelationDialog::post_edit_init_a(RelationDialog * d, QString s)
{
  d->a.edinit->setText(s);
}

void RelationDialog::edit_init_b() {
  edit(b.edinit->text(), b.edrole->text().stripWhiteSpace() + "_initialization",
       &rel->b, TxtEdit, this, (post_edit) post_edit_init_b, edits);
}

void RelationDialog::post_edit_init_b(RelationDialog * d, QString s)
{
  d->b.edinit->setText(s);
}

// C ++ management

void RelationDialog::cpp_update(RoleDialog & role, BrowserClass * cl, BrowserNode * rl) {
  role.cpp_visibility.update_default(role.uml_visibility);
  
  QString s;
  
  switch (current_type) {
  case UmlRealize:
  case UmlGeneralisation:
    {
      // do NOT write
      //	const char * p = role.edcppdecl->text();
      // because the QString is immediatly destroyed !
      QString def = role.edcppdecl->text();
      const char * p = def;
      
      while (*p) {
	if (!strncmp(p, "${type}", 7)) {
	  if (role.cpp_virtual_inheritance_cb->isChecked())
	    s = "virtual ";
	  s += get_cpp_name(cl);
	  p += 7;
	}
	else
	  s += *p++;
      }
    }
    break;
  case UmlDependency:
    if (&role == &b) {
      s  = "";
      break;
    }
    // no break
  default:
    {
      // do NOT write
      //	const char * p = role.edcppdecl->text();
      // because the QString is immediatly destroyed !
      QString def = role.edcppdecl->text();
      const char * p = def;
      const char * pp = 0;
      QString indent = "";
      
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
	  manage_comment(role.comment->text(), p, pp,
			 GenerationSettings::cpp_javadoc_style());
	else if (!strncmp(p, "${description}", 14))
	  manage_description(role.comment->text(), p, pp);
	else if (!strncmp(p, "${static}", 9)) {
	  p += 9;
	  if (role.classrelation_cb->isChecked())
	    s += "static ";
	}
	else if (!strncmp(p, "${const}", 8)) {
	  p += 8;
	  if (role.constrelation_cb->isChecked())
	    s += "const ";
	}
	else if (!strncmp(p, "${mutable}", 10)) {
	  p += 10;
	  if (role.mutable_cb->isChecked())
	    s += "mutable ";
	}
	else if (!strncmp(p, "${volatile}", 11)) {
	  p += 11;
	  if (role.volatile_cb->isChecked())
	    s += "volatile ";
	}
	else if (!strncmp(p, "${type}", 7)) {
	  p += 7;
	  s += get_cpp_name(cl);
	}
	else if (!strncmp(p, "${name}", 7)) {
	  p += 7;
	  s += role.edrole->text();
	}
	else if (!strncmp(p, "${multiplicity}", 15)) {
	  p += 15;
	  
	  QString m = role.multiplicity->currentText().stripWhiteSpace();
	  
	  s += (*m == '[') ? m : QString("[") + m + "]";
	}
	else if (!strncmp(p, "${stereotype}", 13)) {
	  p += 13;
	  s += GenerationSettings::cpp_relation_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
	}
	else if (!strncmp(p, "${h_value}", 10)) {
	  p += 10;
	  if (!role.edinit->text().stripWhiteSpace().isEmpty() &&
	      role.classrelation_cb->isChecked()) {
	    if (role.edinit->text().at(0) == QChar('='))
	      s += ' ';
	    s += role.edinit->text();
	  }
	}
	else if (!strncmp(p, "${value}", 8))
	  p += 8;
	else if (!strncmp(p, "${association}", 14)) {
	  p += 14;
	  s += GenerationSettings::cpp_type(type(edassociation->currentText().stripWhiteSpace(),
						 list, nodes));
	}
	else if (*p == '\n') {
	  s += *p++;
	  if (*p && (*p != '#'))
	    s += indent;
	}
	else if (*p == '@')
	  manage_alias(rl, p, s, role.kvtable);
	else
	  s += *p++;
      }
    }
  }
  
  role.showcppdecl->setText(s);
}

void RelationDialog::cpp_update_a() {
  cpp_update(a, rel->get_end_class(), rel->start);
}

void RelationDialog::cpp_update_b() {
  if (! RelationData::uni_directional(current_type))
    cpp_update(b, rel->get_start_class(), rel->end);
}

void RelationDialog::cpp_default_a() {
  if (a.cpp_undef)
    a.edcppdecl->setText(QString::null);
  else if (RelationData::isa_association(current_type))
    a.edcppdecl->setText(GenerationSettings::cpp_default_rel_decl(current_type,
								  a.multiplicity->currentText().stripWhiteSpace()));
  else {
    if (current_type != UmlDependency)
      a.edcppdecl->setText("${type}");
    else if (GenerationSettings::cpp_relation_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()))
	     != "friend")
      a.edcppdecl->setText("#include in source");
    else
      a.edcppdecl->setText("friend " +
			   ClassDialog::cpp_stereotype(rel->get_end_class()->get_stereotype()) + 
			   " ${type};");
    
    if (! a.visit)
      a.cpp_unmapped_decl_bt->setOn(FALSE);
  }
  cpp_update_a();
}

void RelationDialog::cpp_include_in_header() {
  a.edcppdecl->setText("#include in header");
  if (! a.visit)
    a.cpp_unmapped_decl_bt->setOn(FALSE);
  cpp_update_a();
}

void RelationDialog::cpp_unmapped_a() {
  a.edcppdecl->setText(QString::null);
  a.showcppdecl->setText(QString::null);
  
  if (!RelationData::isa_association(current_type) && !a.visit)
    a.cpp_default_decl_bt->setOn(FALSE);
}

void RelationDialog::cpp_default_b() {
  if (b.cpp_undef)
    b.edcppdecl->setText(QString::null);
  else 
    b.edcppdecl->setText(GenerationSettings::cpp_default_rel_decl(UmlAssociation,
								  b.multiplicity->currentText().stripWhiteSpace()));
  cpp_update_b();
}

void RelationDialog::cpp_unmapped_b() {
  b.edcppdecl->setText(QString::null);
  b.showcppdecl->setText(QString::null);
}

// Java management

void RelationDialog::java_update(RoleDialog & role, BrowserClass * cl, BrowserNode * rl) {
  QString s;
  
  switch (current_type) {
  case UmlRealize:
  case UmlGeneralisation:
    role.editjavaannotation->setEnabled(FALSE);
    {
      // do NOT write
      //	const char * p = role.edjavadecl->text();
      // because the QString is immediatly destroyed !
      QString def = role.edjavadecl->text();
      const char * p = def;
      
      while (*p) {
	if (!strncmp(p, "${type}", 7)) {
	  s = get_java_name(cl);
	  p += 7;
	}
	else
	  s += *p++;
      }
    }
    break;
  case UmlDependency:
    role.editjavaannotation->setEnabled(FALSE);
    s = "";
    break;
  default:
    {
      // do NOT write
      //	const char * p = role.edjavadecl->text();
      // because the QString is immediatly destroyed !
      QString def = role.edjavadecl->text();
      const char * p = def;
      const char * pp = 0;
      QString indent = "";
      
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
	  manage_comment(role.comment->text(), p, pp,
			 GenerationSettings::java_javadoc_style());
	else if (!strncmp(p, "${description}", 14))
	  manage_description(role.comment->text(), p, pp);
	else if (!strncmp(p, "${visibility}", 13)) {
	  p += 13;
	  if (role.uml_visibility.value() != UmlPackageVisibility)
	    s += role.uml_visibility.state() + ' ';
	}
	else if (!strncmp(p, "${static}", 9)) {
	  p += 9;
	  if (role.classrelation_cb->isChecked())
	    s += "static ";
	}
	else if (!strncmp(p, "${transient}", 12)) {
	  p += 12;
	  if (!role.java_undef && role.transient_cb->isChecked())
	    s += "transient ";
	}
	else if (!strncmp(p, "${volatile}", 11)) {
	  p += 11;
	  if (role.volatile_cb->isChecked())
	    s += "volatile ";
	}
	else if (!strncmp(p, "${final}", 8)) {
	  p += 8;
	  if (role.constrelation_cb->isChecked())
	    s += "final ";
	}
	else if (!strncmp(p, "${type}", 7)) {
	  p += 7;
	  s += get_java_name(cl);
	}
	else if (!strncmp(p, "${name}", 7)) {
	  p += 7;
	  s += role.edrole->text();
	}
	else if (!strncmp(p, "${multiplicity}", 15)) {
	  p += 15;
	  
	  QString m = role.multiplicity->currentText().stripWhiteSpace();
	  
	  if (*m != '[')
	    s += "[]";
	  else {
	    for (unsigned index = 0; index != m.length(); index += 1) {
	      switch (m.at(index).latin1()) {
	      case '[':
		s += '[';
		break;
	      case ']':
		s += ']';
	      default:
		break;
	      }
	    }
	  }
	}
	else if (!strncmp(p, "${stereotype}", 13)) {
	  p += 13;
	  s += GenerationSettings::java_relation_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
	}
	else if (!strncmp(p, "${value}", 8)) {
	  p += 8;
	  if (!role.edinit->text().stripWhiteSpace().isEmpty()) {
	    if (role.edinit->text().stripWhiteSpace().at(0) == QChar('='))
	      s += ' ';
	    s += role.edinit->text();
	  }
	}
	else if (!strncmp(p, "${association}", 14)) {
	  p += 14;
	  s += GenerationSettings::java_type(type(edassociation->currentText().stripWhiteSpace(),
						  list, nodes));
	}
	else if (!strncmp(p, "${@}", 4)) {
	  p += 4;
	  if (pp != 0)
	    s += "${@}";
	  else if (!role.javaannotation.isEmpty()) {
	    pp =p;
	    p = role.javaannotation;
	  }
	}
	else if (*p == '\n') {
	  s += *p++;
	  if (*p)
	    s += indent;
	}
	else if (*p == '@')
	  manage_alias(rl, p, s, role.kvtable);
	else
	  s += *p++;
      }
      
      role.editjavaannotation->setEnabled(def.find("${@}") != -1);
    }
  }
  
  role.showjavadecl->setText(s);
}

void RelationDialog::java_update_a() {
  java_update(a, rel->get_end_class(), rel->start);
}

void RelationDialog::java_update_b() {
  if (! RelationData::uni_directional(current_type))
    java_update(b, rel->get_start_class(), rel->end);
}

void RelationDialog::java_default_a() {
  if (a.java_undef)
    a.edjavadecl->setText(QString::null);
  else if (RelationData::isa_association(current_type))
    a.edjavadecl->setText(GenerationSettings::java_default_rel_decl(a.multiplicity->currentText().stripWhiteSpace()));
  else {
    a.edjavadecl->setText("${type}");
    a.java_unmapped_decl_bt->setOn(FALSE);
  }
  java_update_a();
}

void RelationDialog::java_unmapped_a() {
  a.edjavadecl->setText(QString::null);
  a.showjavadecl->setText(QString::null);
  
  if (!RelationData::isa_association(current_type))
    a.java_default_decl_bt->setOn(FALSE);
}

void RelationDialog::java_default_b() {
  if (b.java_undef)
    b.edjavadecl->setText(QString::null);
  else 
    b.edjavadecl->setText(GenerationSettings::java_default_rel_decl(b.multiplicity->currentText().stripWhiteSpace()));
  java_update_b();
}

void RelationDialog::java_unmapped_b() {
  b.edjavadecl->setText(QString::null);
  b.showjavadecl->setText(QString::null);
}

void RelationDialog::java_edit_annotation_a() {
  AnnotationDialog dialog(a.javaannotation, !hasOkButton());
  
  if (dialog.exec() == QDialog::Accepted)
    java_update_a();
}

void RelationDialog::java_edit_annotation_b() {
  AnnotationDialog dialog(b.javaannotation, !hasOkButton());
  
  if (dialog.exec() == QDialog::Accepted)
    java_update_b();
}
  
// Idl management

void RelationDialog::idl_update(RoleDialog & role, BrowserClass * cl, BrowserNode * rl) {
  QString s;
  
  switch (current_type) {
  case UmlRealize:
  case UmlGeneralisation:
    {
      // do NOT write
      //	const char * p = role.edidldecl->text();
      // because the QString is immediatly destroyed !
      QString def = role.edidldecl->text();
      const char * p = def;
      
      while (*p) {
	if (!strncmp(p, "${type}", 7)) {
	  if ((role.idl_truncatable_inheritance_cb != 0) &&
	      role.idl_truncatable_inheritance_cb->isChecked())
	    s = "truncatable ";
	  s += get_idl_name(cl);
	  p += 7;
	}
	else
	  s += *p++;
      }
    }
    break;
  case UmlDependency:
    s = "";
    break;
  default:
    {  
      // do NOT write
      //	const char * p = role.edidldecl->text();
      // because the QString is immediatly destroyed !
      QString def = role.edidldecl->text();
      const char * p = def;
      const char * pp = 0;
      QString indent = "";
      
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
	  s += indent;
	}
	
	if (!strncmp(p, "${comment}", 10))
	  manage_comment(role.comment->text(), p, pp, FALSE);
	else if (!strncmp(p, "${description}", 14))
	  manage_description(role.comment->text(), p, pp);
	else if (!strncmp(p, "${readonly}", 11)) {
	  p += 11;
	  if (role.constrelation_cb->isChecked())
	    s += "readonly ";
	}
	else if (role.idl_in_union && !strncmp(p, "${case}", 7)) {
	  p += 7;
	  s += role.edcase->currentText().stripWhiteSpace();
	}
	else if (!strncmp(p, "${attribut}", 11)) {
	  // old version
	  p += 11;
	  if (!role.idl_in_struct && !role.idl_in_valuetype)
	    s += "attribute ";
	}
	else if (!strncmp(p, "${attribute}", 12)) {
	  p += 12;
	  if (!role.idl_in_struct && !role.idl_in_valuetype)
	    s += "attribute ";
	}
	else if (role.idl_in_valuetype && !strncmp(p, "${visibility}", 13)) {
	  p += 13;
	  switch (role.uml_visibility.value()) {
	  case UmlPublic:
	  case UmlPackage:
	    s += "public ";
	  default:
	    s += "private ";
	  }
	}
	else if (!strncmp(p, "${type}", 7)) {
	  p += 7;
	  s += get_idl_name(cl);
	}
	else if (!strncmp(p, "${name}", 7)) {
	  p += 7;
	  s += role.edrole->text();
	}
	else if (!strncmp(p, "${stereotype}", 13)) {
	  p += 13;
	  s += GenerationSettings::idl_relation_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
	}
	else if (!strncmp(p, "${multiplicity}", 15)) {
	  p += 15;
	  s += role.multiplicity->currentText().stripWhiteSpace();
	}
	else if (!strncmp(p, "${association}", 14)) {
	  p += 14;
	  s += GenerationSettings::idl_type(type(edassociation->currentText().stripWhiteSpace(),
						 list, nodes));
	}
	else if (*p == '\n') {
	  s += *p++;
	  if (*p && (*p != '#'))
	    s += indent;
	}
	else if (*p == '@')
	  manage_alias(rl, p, s, role.kvtable);
	else
	  s += *p++;
      }
    }
  }
  
  role.showidldecl->setText(s);
}

void RelationDialog::idl_update_a() {
  idl_update(a, rel->get_end_class(), rel->start);
}

void RelationDialog::idl_update_b() {
  if (! RelationData::uni_directional(current_type))
    idl_update(b, rel->get_start_class(), rel->end);
}

void RelationDialog::idl_default_a() {
  if (a.idl_undef)
    a.edidldecl->setText(QString::null);
  else if (RelationData::isa_association(current_type)) {
    QString mult = a.multiplicity->currentText().stripWhiteSpace();
    
    a.edidldecl->setText((a.idl_in_union)
			 ? GenerationSettings::idl_default_union_rel_decl(mult)
			 : ((a.idl_in_valuetype)
			    ? GenerationSettings::idl_default_valuetype_rel_decl(mult)
			    : GenerationSettings::idl_default_rel_decl(mult)));
  }
  else {
    a.edidldecl->setText("${type}");
    a.idl_unmapped_decl_bt->setOn(FALSE);
  }
  idl_update_a();
}

void RelationDialog::idl_unmapped_a() {
  a.edidldecl->setText(QString::null);
  a.showidldecl->setText(QString::null);
  
  if (!RelationData::isa_association(current_type))
    a.idl_default_decl_bt->setOn(FALSE);
}

void RelationDialog::idl_default_b() {
  if (b.idl_undef)
    b.edidldecl->setText(QString::null);
  else {
    QString mult = b.multiplicity->currentText().stripWhiteSpace();
    
    b.edidldecl->setText((b.idl_in_union)
			 ? GenerationSettings::idl_default_union_rel_decl(mult)
			 : ((b.idl_in_valuetype)
			    ? GenerationSettings::idl_default_valuetype_rel_decl(mult)
			    : GenerationSettings::idl_default_rel_decl(mult)));
  }
  idl_update_b();
}

void RelationDialog::idl_unmapped_b() {
  b.edidldecl->setText(QString::null);
  b.showidldecl->setText(QString::null);
}

//

static void accept_role(RoleDialog & role, RoleData & rel,
			bool assoc, RelationData * r)
{
  rel.uml_visibility = role.uml_visibility.value();
  
  if (!assoc) {
    rel.isa_class_relation = FALSE;
    rel.isa_volatile_relation = FALSE;
    rel.isa_const_relation = FALSE;
    rel.cpp_mutable = FALSE;
    rel.java_transient = FALSE;
  }
  else {
    rel.isa_class_relation = role.classrelation_cb->isChecked();
    rel.isa_volatile_relation = role.volatile_cb->isChecked();
    rel.isa_const_relation = role.constrelation_cb->isChecked();
    rel.cpp_mutable = !role.cpp_undef && role.mutable_cb->isChecked();
    rel.java_transient = !role.java_undef && role.transient_cb->isChecked();
  }
  
  rel.role = role.edrole->text().stripWhiteSpace();
  rel.multiplicity = role.multiplicity->currentText().stripWhiteSpace();
  rel.init_value = role.edinit->text();
  rel.comment = role.comment->text();
  rel.constraint = role.constraint->stripWhiteSpaceText();
  
  rel.cpp_visibility = role.cpp_visibility.value();
    
  if (role.idl_in_union) {
    int index;
    QString s = role.edcase->currentText().stripWhiteSpace();
    
    if (!s.isEmpty() && ((index = role.enum_names.findIndex(s)) != -1))
      r->set_idlcase(rel, ((BrowserAttribute *) role.enums.at(index)), "");
    else
      r->set_idlcase(rel, 0, s);
  }
  
  rel.cpp_decl = role.edcppdecl->text();
  rel.java_decl = role.edjavadecl->text();
  rel.java_annotation = role.javaannotation;
  rel.idl_decl = role.edidldecl->text();
}

void RelationDialog::accept() {
  if (!check_edits(edits))
    return;
    
  QString ra = a.edrole->text().stripWhiteSpace();
  QString rb = b.edrole->text().stripWhiteSpace();
  
  if (rel->wrong_role_a_name(ra))
    msg_critical("Error", ra + "\n\nillegal name or already used");
  else if (rel->wrong_role_b_name(rb))
    msg_critical("Error", rb + "\n\nillegal name or already used");
  else {
    rel->name = edname->text().stripWhiteSpace();

    int index = 0;
    QString s = edtype->currentText();
    
    while (RelTypes[index].lbl != s)
      index += 1;
    rel->type = RelTypes[index].type;
    
    rel->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    
    bool assoc = RelationData::isa_association(rel->get_type());
    
    if (assoc) {
      AType t;
      
      s = edassociation->currentText().stripWhiteSpace();
      if (!s.isEmpty()) {
	index = list.findIndex(s);
	
	if (index >= 0)
	  t.type = (BrowserClass *) nodes.at(index);
	else
	  t.explicit_type = s;
      }
      rel->set_association(t);
    }
    
    rel->a.cpp_virtual_inheritance = 
      ((!assoc) && a.cpp_virtual_inheritance_cb->isChecked());
    rel->a.idl_truncatable_inheritance = 
      ((!assoc) &&
       (a.idl_truncatable_inheritance_cb != 0) &&
       a.idl_truncatable_inheritance_cb->isChecked());
    
    accept_role(a, rel->a, assoc, rel);
    accept_role(b, rel->b, assoc, rel);
    
    a.kvtable->update(rel->start);    
    if (rel->end)
      rel->end->package_modified();
    if (rel->check_end_visibility()) {
      rel->end->modified();
      b.kvtable->update(rel->end);
    }
    rel->start->modified();
    rel->start->package_modified();
    rel->modified();
    
    QTabDialog::accept();
  }
}
