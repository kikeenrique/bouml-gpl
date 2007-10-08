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

#include <qcursor.h>
#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>
#include <qpopupmenu.h> 
#include <qpushbutton.h> 
#include <qfiledialog.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qradiobutton.h> 
#include <qbuttongroup.h>
#include <qsplitter.h> 

#include "GenerationSettingsDialog.h"
#include "BrowserView.h"
#include "MLinesItem.h"
#include "DialogUtil.h"
#include "UmlPixmap.h"
#include "UmlDesktop.h"
#include "strutil.h"

QSize GenerationSettingsDialog::previous_size;

GenerationSettingsDialog::GenerationSettingsDialog()
    : QTabDialog(0, "GenerationSettings dialog", TRUE, 0) {
  setCaption("GenerationSettings dialog");
  
  setCancelButton();
  init_types();
  init_stereotypes();
  init_cpp1();
  init_cpp2();
  init_cpp3();
  init_cpp4();
  init_cpp5();
  init_java1();
  init_java2();
  init_java3();
  init_java4();
  init_php1();
  init_php2();
  init_idl1();
  init_idl2();
  init_idl3();
  init_idl4();
  init_idl5();
  init_descriptions();
  init_dirs();
}

void GenerationSettingsDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::setsize_center(this, previous_size, 29.0/30, 19.0/29);
}

GenerationSettingsDialog::~GenerationSettingsDialog() {
  previous_size = size();
}

void GenerationSettingsDialog::init_types() {
  QGrid * grid = new QGrid(1, this);
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Types correspondence, and C++ operation argument default passing for them :", grid);

  types_table = new TypesTable(grid);
  
  addTab(grid, "Types");
}

void GenerationSettingsDialog::init_stereotypes() {
  QGrid * grid = new QGrid(2, this);
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Attributes and \nRelations\nstereotypes \ncorrespondence : ", grid);
  relation_stereotypes_table = 
    new StereotypesTable(grid, GenerationSettings::nrelattrstereotypes,
			 GenerationSettings::relattr_stereotypes, FALSE);
  
  //new QLabel(grid);
  //new QLabel(grid);
  
  new QLabel("Classes's \nstereotypes \ncorrespondence : ", grid);
  class_stereotypes_table =
    new StereotypesTable(grid, GenerationSettings::nclassstereotypes,
			 GenerationSettings::class_stereotypes, TRUE);

  addTab(grid, "Stereotypes");
}  

void GenerationSettingsDialog::init_cpp1() {
  QVBox * vtab = new QVBox(this);
  QGrid * grid = new QGrid(2, vtab);
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Header file\ndefault content :", grid);
  edcpp_h_content = new MultiLineEdit(grid);
  edcpp_h_content->setText(GenerationSettings::cpp_h_content);
  QFont font = edcpp_h_content->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  edcpp_h_content->setFont(font);
    
  new QLabel("Source file\ndefault content :", grid);
  edcpp_src_content = new MultiLineEdit(grid);
  edcpp_src_content->setText(GenerationSettings::cpp_src_content);
  edcpp_src_content->setFont(font);
  
  new QLabel(vtab);
  
  QHBox * htab = new QHBox(vtab);
  
  htab->setMargin(3);
  htab->setStretchFactor(new QLabel("generated/reversed \nheader file extension : ", htab), 0);
  edcpp_h_extension = new QComboBox(TRUE, htab);
  htab->setStretchFactor(edcpp_h_extension, 100);
  edcpp_h_extension->insertItem(GenerationSettings::cpp_h_extension);
  edcpp_h_extension->setCurrentItem(0);
  edcpp_h_extension->insertItem("h");
  edcpp_h_extension->insertItem("hh");
  
  htab->setStretchFactor(new QLabel("    generated/reversed \n    source file extension : ", htab), 0);
  edcpp_src_extension = new QComboBox(TRUE, htab);
  htab->setStretchFactor(edcpp_src_extension, 100);
  edcpp_src_extension->insertItem(GenerationSettings::cpp_src_extension);
  edcpp_src_extension->setCurrentItem(0);
  edcpp_src_extension->insertItem("cpp");
  edcpp_src_extension->insertItem("cc");

  htab->setStretchFactor(new QLabel("    #include : ", htab), 0);
  cpp_include_with_path_cb = new QComboBox(FALSE, htab);
  cpp_include_with_path_cb->insertItem("without path");
  cpp_include_with_path_cb->insertItem("with absolute path");
  cpp_include_with_path_cb->insertItem("with relative path");
  cpp_include_with_path_cb->insertItem("with root relative path");
  if (!GenerationSettings::cpp_include_with_path)
    cpp_include_with_path_cb->setCurrentItem(0);
  else if (GenerationSettings::cpp_relative_path)
    cpp_include_with_path_cb->setCurrentItem(2);
  else if (GenerationSettings::cpp_root_relative_path)
    cpp_include_with_path_cb->setCurrentItem(3);
  else
    cpp_include_with_path_cb->setCurrentItem(1);

  htab = new QHBox(vtab);
  htab->setMargin(3);
  
  htab->setStretchFactor(new QLabel("force namespace \nprefix generation : ", htab), 0);
  cpp_force_namespace_gen_cb = new QCheckBox(htab);
  cpp_force_namespace_gen_cb->setChecked(GenerationSettings::cpp_force_namespace_gen);
  
  htab->setStretchFactor(new QLabel("            generate Javadoc \n            style comment : ", htab), 0);
  cpp_javadoc_cb = new QCheckBox(htab);
  cpp_javadoc_cb->setChecked(GenerationSettings::cpp_javadoc_comment);
  
  htab->setStretchFactor(new QLabel(htab), 1000);
  
  addTab(vtab, "C++[1]");
  
  if (!GenerationSettings::cpp_get_default_defs())
    removePage(vtab);
}

void GenerationSettingsDialog::init_cpp2() {
  QGrid * grid = new QGrid(2, this);
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Class default \ndeclaration :", grid);
  edcpp_class_decl = new MultiLineEdit(grid);
  edcpp_class_decl->setText(GenerationSettings::cpp_class_decl);
  QFont font = edcpp_class_decl->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  edcpp_class_decl->setFont(font);

  new QLabel("Struct default \ndeclaration :", grid);
  edcpp_struct_decl = new MultiLineEdit(grid);
  edcpp_struct_decl->setText(GenerationSettings::cpp_struct_decl);
  edcpp_struct_decl->setFont(font);

  new QLabel("Union default \ndeclaration :", grid);
  edcpp_union_decl = new MultiLineEdit(grid);
  edcpp_union_decl->setText(GenerationSettings::cpp_union_decl);
  edcpp_union_decl->setFont(font);

  new QLabel("Enum default \ndeclaration :", grid);
  edcpp_enum_decl = new MultiLineEdit(grid);
  edcpp_enum_decl->setText(GenerationSettings::cpp_enum_decl);
  edcpp_enum_decl->setFont(font);

  new QLabel("Typedef default \ndeclaration :", grid);
  edcpp_typedef_decl = new MultiLineEdit(grid);
  edcpp_typedef_decl->setText(GenerationSettings::cpp_typedef_decl);
  edcpp_typedef_decl->setFont(font);

  addTab(grid, "C++[2]");
  
  if (!GenerationSettings::cpp_get_default_defs())
    removePage(grid);
}

void GenerationSettingsDialog::init_cpp3() {
  QGrid * grid = new QGrid(2, this);
  QGrid * grid2;
  QHBox * htab;
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Attribute \ndefault \ndeclaration :", grid);

  grid2 = new QGrid(2, grid);
  (new QLabel("Multiplicity", grid2))->setAlignment(Qt::AlignCenter);
  new QLabel(grid2);
  new QLabel("unspecified,\n1 or 1", grid2);
  edcpp_attr_decl[0] = new MultiLineEdit(grid2);
  new QLabel("* or a..b", grid2);
  edcpp_attr_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (means [X])\nor [...]...[...]", grid2);
  edcpp_attr_decl[2] = new MultiLineEdit(grid2);

  QFont font = edcpp_attr_decl[0]->font();
  int i, j;

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);

  for (i = 0; i != 3; i += 1) {
    edcpp_attr_decl[i]->setText(GenerationSettings::cpp_attr_decl[i]);
    edcpp_attr_decl[i]->setFont(font);
  }
  
  new QLabel(grid);
  new QLabel(grid);

  new QLabel("Association\nand\naggregation\ndefault\ndeclaration :", grid);
  grid2 = new QGrid(2, grid);
  (new QLabel("Multiplicity", grid2))->setAlignment(Qt::AlignCenter);
  htab = new QHBox(grid2);
  htab->setStretchFactor(new QLabel(htab), 1000);
  (new QLabel("Association ", htab))->setAlignment(Qt::AlignCenter);
  (new QLabel(htab))->setPixmap(*associationButton);
  (new QLabel("  and aggregation ", htab))->setAlignment(Qt::AlignCenter);
  (new QLabel(htab))->setPixmap(*aggregationButton);
  htab->setStretchFactor(new QLabel(htab), 1000);
  new QLabel("unspecified,\n1 or 0..1", grid2);
  edcpp_rel_decl[0][0] = new MultiLineEdit(grid2);
  new QLabel("* or a..b", grid2);
  edcpp_rel_decl[0][1] = new MultiLineEdit(grid2);
  new QLabel("X (means [X])\nor [...]...[...]", grid2);
  edcpp_rel_decl[0][2] = new MultiLineEdit(grid2);
  
  new QLabel(grid);
  new QLabel(grid);

  new QLabel("Aggregation\nby value\ndefault\ndeclaration :", grid);
  grid2 = new QGrid(2, grid);
  (new QLabel("Multiplicity", grid2))->setAlignment(Qt::AlignCenter);
  htab = new QHBox(grid2);
  htab->setStretchFactor(new QLabel(htab), 1000);
  (new QLabel("Aggregation by value ", htab))->setAlignment(Qt::AlignCenter);
  (new QLabel(htab))->setPixmap(*aggregationByValueButton);
  htab->setStretchFactor(new QLabel(htab), 1000);
  new QLabel("unspecified\nor 1", grid2);
  edcpp_rel_decl[1][0] = new MultiLineEdit(grid2);
  new QLabel("* or a..b", grid2);
  edcpp_rel_decl[1][1] = new MultiLineEdit(grid2);
  new QLabel("X (means [X])\nor [...]...[...]", grid2);
  edcpp_rel_decl[1][2] = new MultiLineEdit(grid2);

  for (i = 0; i != 2; i += 1) {
    for (j = 0; j != 3; j += 1) {
      edcpp_rel_decl[i][j]->setText(GenerationSettings::cpp_rel_decl[i][j]);
      edcpp_rel_decl[i][j]->setFont(font);
    }
  }
  
  addTab(grid, "C++[3]");
  
  if (!GenerationSettings::cpp_get_default_defs())
    removePage(grid);
}

void GenerationSettingsDialog::init_cpp4() {
  QGrid * grid = new QGrid(2, this);
  QVBox * vtab;
  QHBox * htab;
  QButtonGroup * bg;
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Get operation\ndefault definition : ", grid);
  
  vtab = new QVBox(grid);
  
  htab = new QHBox(vtab);
  cpp_get_visibility.init(htab, GenerationSettings::cpp_get_visibility,
			       FALSE, "Visibility");

  bg = new QButtonGroup(3, QGroupBox::Horizontal, "Modifiers", htab);
  bg->setExclusive(FALSE);
  cpp_get_inline_cb = new QCheckBox("inline", bg);
  cpp_get_value_const_cb = new QCheckBox("const value", bg);
  cpp_get_const_cb = new QCheckBox("const", bg);
  cpp_get_inline_cb->setChecked(GenerationSettings::cpp_get_inline);
  cpp_get_value_const_cb->setChecked(GenerationSettings::cpp_get_value_const);
  cpp_get_const_cb->setChecked(GenerationSettings::cpp_get_const);
  
  new QLabel("  name : ", htab);
  edcpp_get_name = new LineEdit(htab);
  edcpp_get_name->setText(GenerationSettings::cpp_get_name);
  QFont font = edcpp_get_name->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  edcpp_get_name->setFont(font);
  
  new QLabel("  ", htab);
  uml_follow_cpp_get_name = new QCheckBox("also in uml", htab);
  if (GenerationSettings::uml_get_name == CppView)
    uml_follow_cpp_get_name->setChecked(TRUE);
  connect(uml_follow_cpp_get_name, SIGNAL(toggled(bool)),
	  this, SLOT(follow_cpp_get_name()));
  
  new QLabel("Set operation\ndefault definition : ", grid);
  
  vtab = new QVBox(grid);
  
  htab = new QHBox(vtab);
  cpp_set_visibility.init(htab, GenerationSettings::cpp_get_visibility,
			  FALSE, "Visibility");
  
  bg = new QButtonGroup(4, QGroupBox::Horizontal, "Modifiers", htab);
  bg->setExclusive(FALSE);
  cpp_set_inline_cb = new QCheckBox("inline", bg);
  cpp_set_param_const_cb = new QCheckBox("const param", bg);
  cpp_set_param_ref_cb = new QCheckBox("by ref", bg);
  cpp_set_inline_cb->setChecked(GenerationSettings::cpp_set_inline);
  cpp_set_param_const_cb->setChecked(GenerationSettings::cpp_set_param_const);
  cpp_set_param_ref_cb->setChecked(GenerationSettings::cpp_set_param_ref);
  
  new QLabel("  name : ", htab);
  edcpp_set_name = new LineEdit(htab);
  edcpp_set_name->setText(GenerationSettings::cpp_set_name);
  edcpp_set_name->setFont(font);
  
  new QLabel("  ", htab);
  uml_follow_cpp_set_name = new QCheckBox("also in uml", htab);
  if (GenerationSettings::uml_set_name == CppView)
    uml_follow_cpp_set_name->setChecked(TRUE);
  connect(uml_follow_cpp_set_name, SIGNAL(toggled(bool)),
	  this, SLOT(follow_cpp_set_name()));
  
  //new QLabel(grid);
  //new QLabel(grid);
  
  new QLabel("Enumeration item \ndefault definition :", grid);
  edcpp_enum_item_decl = new MultiLineEdit(grid);
  edcpp_enum_item_decl->setText(GenerationSettings::cpp_enum_item_decl);
  edcpp_enum_item_decl->setFont(font);
  
  //new QLabel(grid);
  //new QLabel(grid);

  new QLabel("Default type forms\nfor the enums :", grid);
  htab = new QHBox(grid);
  htab->setMargin(3);
  new QLabel("input\nparameter : ", htab);
  cpp_enum_in = new LineEdit(htab);
  cpp_enum_in->setText(GenerationSettings::cpp_enum_in);
  cpp_enum_in->setFont(font);
  new QLabel("   output\n   parameter : ", htab);
  cpp_enum_out = new LineEdit(htab);
  cpp_enum_out->setText(GenerationSettings::cpp_enum_out);
  cpp_enum_out->setFont(font);
  new QLabel("   input output \n   parameter : ", htab);
  cpp_enum_inout = new LineEdit(htab);
  cpp_enum_inout->setText(GenerationSettings::cpp_enum_inout);
  cpp_enum_inout->setFont(font);
  new QLabel("   operation \n   return : ", htab);
  cpp_enum_return = new LineEdit(htab);
  cpp_enum_return->setText(GenerationSettings::cpp_enum_return);
  cpp_enum_return->setFont(font);
  
  new QLabel("Default type forms for\nthe types not given\nin the first tab:", grid);
  htab = new QHBox(grid);
  htab->setMargin(3);
  new QLabel("input\nparameter : ", htab);
  cpp_in = new LineEdit(htab);
  cpp_in->setText(GenerationSettings::cpp_in);
  cpp_in->setFont(font);
  new QLabel("   output\n   parameter : ", htab);
  cpp_out = new LineEdit(htab);
  cpp_out->setText(GenerationSettings::cpp_out);
  cpp_out->setFont(font);
  new QLabel("   input output \n   parameter : ", htab);
  cpp_inout = new LineEdit(htab);
  cpp_inout->setText(GenerationSettings::cpp_inout);
  cpp_inout->setFont(font);
  new QLabel("   operation \n   return : ", htab);
  cpp_return = new LineEdit(htab);
  cpp_return->setText(GenerationSettings::cpp_return);
  cpp_return->setFont(font);
  
  //new QLabel(grid);
  //new QLabel(grid);
  
  new QLabel("Operation default \ndeclaration :", grid);
  edcpp_oper_decl = new MultiLineEdit(grid);
  edcpp_oper_decl->setText(GenerationSettings::cpp_oper_decl);
  edcpp_oper_decl->setFont(font);

  new QLabel("Operation default \ndefinition :", grid);
  htab = new QHBox(grid);
  edcpp_oper_def = new MultiLineEdit(htab);
  edcpp_oper_def->setText(GenerationSettings::cpp_oper_def);
  edcpp_oper_def->setFont(font);
  new QLabel("  ", htab);
  cpp_force_throw_cb = new QCheckBox("throw()", htab);
  cpp_force_throw_cb->setChecked(GenerationSettings::cpp_force_throw);


  addTab(grid, "C++[4]");
  
  if (!GenerationSettings::cpp_get_default_defs())
    removePage(grid);
}

void GenerationSettingsDialog::init_cpp5() {
  QSplitter * split = new QSplitter(Vertical, this);
  
  split->setOpaqueResize(TRUE);
  
  QHBox * htab;
  
  htab = new QHBox(split);
  htab->setMargin(3);
  QLabel * lbl1 = 
    new QLabel("External classes : \nname making\n#include, using", htab);
  edcpp_external_class_decl = new MultiLineEdit(htab);
  edcpp_external_class_decl->setText(GenerationSettings::cpp_external_class_decl);
  htab = new QHBox(split);
  htab->setMargin(3);
  QLabel * lbl2 = 
    new QLabel("External types :\n#include form(s),\nusing, etc...", htab);
  cpp_include_table =
    new IncludeTable(htab, GenerationSettings::cpp_includes,
		     "Include etc...", "#include <>");

  same_width(lbl1, lbl2);
  
  addTab(split, "C++[5]");
  
  if (!GenerationSettings::cpp_get_default_defs())
    removePage(split);
}

void GenerationSettingsDialog::init_java1() {
  QGrid * grid = new QGrid(2, this);
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("file default \ncontent :", grid);
  
  QHBox * htab = new QHBox(grid);

  htab->setMargin(3);
  
  edjava_src_content = new MultiLineEdit(htab);
  edjava_src_content->setText(GenerationSettings::java_src_content);
  QFont font = edjava_src_content->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  edjava_src_content->setFont(font);

  QVBox * vtab = new QVBox(htab);
  QHBox * htab2 = new QHBox(vtab);

  vtab->setMargin(3);
  htab2->setMargin(3);

  new QLabel("    generated / reversed \n    file extension : ", htab2);
  edjava_extension = new QComboBox(TRUE, htab2);
  edjava_extension->insertItem(GenerationSettings::java_extension);
  edjava_extension->setCurrentItem(0);
  edjava_extension->insertItem("java");

  htab2 = new QHBox(vtab);
  htab2->setMargin(3);
  new QLabel("    generate Javadoc \n    style comment : ", htab2);
  java_javadoc_cb = new QCheckBox(htab2);
  java_javadoc_cb->setChecked(GenerationSettings::java_javadoc_comment);
  
  new QLabel("Class default \ndeclaration :", grid);
  edjava_class_decl = new MultiLineEdit(grid);
  edjava_class_decl->setText(GenerationSettings::java_class_decl);
  edjava_class_decl->setFont(font);

  new QLabel("Interface default \ndeclaration :", grid);
  edjava_interface_decl = new MultiLineEdit(grid);
  edjava_interface_decl->setText(GenerationSettings::java_interface_decl);
  edjava_interface_decl->setFont(font);

  new QLabel("Enum default \ndeclaration :", grid);
  edjava_enum_decl = new MultiLineEdit(grid);
  edjava_enum_decl->setText(GenerationSettings::java_enum_decl);
  edjava_enum_decl->setFont(font);

  new QLabel("Enum pattern \ndefault declaration :", grid);
  edjava_enum_pattern_decl = new MultiLineEdit(grid);
  edjava_enum_pattern_decl->setText(GenerationSettings::java_enum_pattern_decl);
  edjava_enum_pattern_decl->setFont(font);

  addTab(grid, "Java[1]");
  
  if (!GenerationSettings::java_get_default_defs())
    removePage(grid);
}

void GenerationSettingsDialog::init_java2() {
  QGrid * grid = new QGrid(2, this);
  QGrid * grid2;
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Attribute \ndefault \ndeclaration :", grid);

  grid2 = new QGrid(2, grid);
  new QLabel("multiplicity '1'\nor unspecified", grid2);
  edjava_attr_decl[0] = new MultiLineEdit(grid2);
  new QLabel("multiplicity '*'\nor 'a..b'", grid2);
  edjava_attr_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (probably a\nnumber)", grid2);
  edjava_attr_decl[2] = new MultiLineEdit(grid2);

  QFont font = edjava_attr_decl[0]->font();
  int i;

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);

  for (i = 0; i != 3; i += 1) {
    edjava_attr_decl[i]->setText(GenerationSettings::java_attr_decl[i]);
    edjava_attr_decl[i]->setFont(font);
  }

  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel("Association and\naggregation\ndefault\ndeclaration :", grid);
  grid2 = new QGrid(2, grid);
  new QLabel("multiplicity '1'\nor unspecified", grid2);
  edjava_rel_decl[0] = new MultiLineEdit(grid2);
  new QLabel("multiplicity '*'\nor 'a..b'", grid2);
  edjava_rel_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (probably a\nnumber)", grid2);
  edjava_rel_decl[2] = new MultiLineEdit(grid2);
  for (i = 0; i != 3; i += 1) {
    edjava_rel_decl[i]->setText(GenerationSettings::java_rel_decl[i]);
    edjava_rel_decl[i]->setFont(font);
  }

  addTab(grid, "Java[2]");
  
  if (!GenerationSettings::java_get_default_defs())
    removePage(grid);
}  

void GenerationSettingsDialog::init_java3() {
  QGrid * grid = new QGrid(2, this);
  QHBox * htab;
  QButtonGroup * bg;
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Enumeration item \ndefault definition :", grid);
  edjava_enum_item_decl = new MultiLineEdit(grid);
  edjava_enum_item_decl->setText(GenerationSettings::java_enum_item_decl);
  
  QFont font = edjava_enum_item_decl->font();

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  edjava_enum_item_decl->setFont(font);

  new QLabel("Enum pattern item \ndefault definition :", grid);
  edjava_enum_pattern_item_decl = new MultiLineEdit(grid);
  edjava_enum_pattern_item_decl->setText(GenerationSettings::java_enum_pattern_item_decl);
  edjava_enum_pattern_item_decl->setFont(font);

  new QLabel("Enum pattern item \n'case' in 'from_int' :", grid);
  edjava_enum_pattern_item_case = new MultiLineEdit(grid);
  edjava_enum_pattern_item_case->setText(GenerationSettings::java_enum_pattern_item_case);
  edjava_enum_pattern_item_case->setFont(font);

  new QLabel("Get operation\ndefault definition : ", grid);
  htab = new QHBox(grid);
  htab->setMargin(3);
  java_get_visibility.init(htab, GenerationSettings::javaphp_get_visibility,
			   TRUE, "Visibility (shared with Php)");
  java_get_visibility.connect(SIGNAL(clicked (int)), this, SLOT(java_get_visi_changed(int)));
  
  bg = new QButtonGroup(1, QGroupBox::Horizontal, "Modifiers", htab);
  bg->setExclusive(FALSE);
  java_get_final_cb = new QCheckBox("final", bg);
  java_get_final_cb->setChecked(GenerationSettings::java_get_final);
  
  new QLabel("  name : ", htab);
  edjava_get_name = new LineEdit(htab);
  edjava_get_name->setText(GenerationSettings::java_get_name);
  edjava_get_name->setFont(font);
  
  new QLabel("  ", htab);
  uml_follow_java_get_name = new QCheckBox("also in uml", htab);
  if (GenerationSettings::uml_get_name == JavaView)
    uml_follow_java_get_name->setChecked(TRUE);
  connect(uml_follow_java_get_name, SIGNAL(toggled(bool)),
	  this, SLOT(follow_java_get_name()));
  
  new QLabel("Set operation\ndefault definition : ", grid);
  htab = new QHBox(grid);
  htab->setMargin(3);
  java_set_visibility.init(htab, GenerationSettings::javaphp_set_visibility,
			   TRUE, "Visibility (shared with Php)");
  java_set_visibility.connect(SIGNAL(clicked (int)), this, SLOT(java_set_visi_changed(int)));
  
  bg = new QButtonGroup(2, QGroupBox::Horizontal, "Modifiers", htab);
  bg->setExclusive(FALSE);
  java_set_final_cb = new QCheckBox("final", bg);
  java_set_param_final_cb = new QCheckBox("final parameter", bg);
  java_set_final_cb->setChecked(GenerationSettings::java_set_final);
  java_set_param_final_cb->setChecked(GenerationSettings::java_set_param_final);
  
  new QLabel("  name : ", htab);
  edjava_set_name = new LineEdit(htab);
  edjava_set_name->setText(GenerationSettings::java_set_name);
  edjava_set_name->setFont(font);
  
  new QLabel("  ", htab);
  uml_follow_java_set_name = new QCheckBox("also in uml", htab);
  if (GenerationSettings::uml_set_name == JavaView)
    uml_follow_java_set_name->setChecked(TRUE);
  connect(uml_follow_java_set_name, SIGNAL(toggled(bool)),
	  this, SLOT(follow_java_set_name()));
  
  //new QLabel(grid);
  //new QLabel(grid);
  
  new QLabel("Operation\ndefault definition :", grid);
  edjava_oper_def = new MultiLineEdit(grid);
  edjava_oper_def->setText(GenerationSettings::java_oper_def);
  edjava_oper_def->setFont(font);

  addTab(grid, "Java[3]");
  
  if (!GenerationSettings::java_get_default_defs())
    removePage(grid);
}  

void GenerationSettingsDialog::init_java4() {
  QSplitter * split = new QSplitter(Vertical, this);
  
  split->setOpaqueResize(TRUE);
  
  QHBox * htab;
  
  htab = new QHBox(split);
  htab->setMargin(3);
  QLabel * lbl1 = 
    new QLabel("External classes : \nname making", htab);
  edjava_external_class_decl = new LineEdit(htab);
  edjava_external_class_decl->setText(GenerationSettings::java_external_class_decl);

  htab = new QHBox(split);
  htab->setMargin(3);
  QLabel * lbl2 = 
    new QLabel("External types :\nimport form(s) etc...", htab);
  java_import_table =
    new IncludeTable(htab, GenerationSettings::java_imports,
		     "Import etc...", "import ");

  same_width(lbl1, lbl2);
  addTab(split, "Java[4]");
  
  if (!GenerationSettings::java_get_default_defs())
    removePage(split);
}

void GenerationSettingsDialog::init_php1() {
  QGrid * grid = new QGrid(2, this);
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("file default \ncontent :", grid);
  
  QHBox * htab = new QHBox(grid);

  htab->setMargin(3);
  
  edphp_src_content = new MultiLineEdit(htab);
  edphp_src_content->setText(GenerationSettings::php_src_content);
  QFont font = edphp_src_content->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  edphp_src_content->setFont(font);

  new QLabel("    generated / reversed \n    file extension : ", htab);
  edphp_extension = new QComboBox(TRUE, htab);
  edphp_extension->insertItem(GenerationSettings::php_extension);
  edphp_extension->setCurrentItem(0);
  edphp_extension->insertItem("php");

  new QLabel("Class default \ndeclaration :", grid);
  edphp_class_decl = new MultiLineEdit(grid);
  edphp_class_decl->setText(GenerationSettings::php_class_decl);
  edphp_class_decl->setFont(font);

  new QLabel("Interface default \ndeclaration :", grid);
  edphp_interface_decl = new MultiLineEdit(grid);
  edphp_interface_decl->setText(GenerationSettings::php_interface_decl);
  edphp_interface_decl->setFont(font);

  new QLabel("Enum default \ndeclaration :", grid);
  edphp_enum_decl = new MultiLineEdit(grid);
  edphp_enum_decl->setText(GenerationSettings::php_enum_decl);
  edphp_enum_decl->setFont(font);

  addTab(grid, "Php[1]");
  
  if (!GenerationSettings::php_get_default_defs())
    removePage(grid);
}

void GenerationSettingsDialog::init_php2() {
  QGrid * grid = new QGrid(2, this);
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Attribute default \ndeclaration :", grid);
  edphp_attr_decl = new MultiLineEdit(grid);

  QFont font = edphp_attr_decl->font();

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);

  edphp_attr_decl->setText(GenerationSettings::php_attr_decl);
  edphp_attr_decl->setFont(font);

  new QLabel("Association and\naggregation\ndefault\ndeclaration :", grid);
  edphp_rel_decl = new MultiLineEdit(grid);
  edphp_rel_decl->setText(GenerationSettings::php_rel_decl);
  edphp_rel_decl->setFont(font);

  QHBox * htab;
  QButtonGroup * bg;
  
  new QLabel("Enumeration item \ndefault definition :", grid);
  edphp_enum_item_decl = new MultiLineEdit(grid);
  edphp_enum_item_decl->setText(GenerationSettings::php_enum_item_decl);
  
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  edphp_enum_item_decl->setFont(font);

  new QLabel("Get operation\ndefault definition : ", grid);
  htab = new QHBox(grid);
  htab->setMargin(3);
  php_get_visibility.init(htab, GenerationSettings::javaphp_get_visibility,
			   FALSE, "Visibility (shared with Java)");
  php_get_visibility.connect(SIGNAL(clicked (int)), this, SLOT(php_get_visi_changed(int)));
  
  bg = new QButtonGroup(1, QGroupBox::Horizontal, "Modifiers", htab);
  bg->setExclusive(FALSE);
  php_get_final_cb = new QCheckBox("final", bg);
  php_get_final_cb->setChecked(GenerationSettings::php_get_final);
  
  new QLabel("  name : ", htab);
  edphp_get_name = new LineEdit(htab);
  edphp_get_name->setText(GenerationSettings::php_get_name);
  edphp_get_name->setFont(font);
  
  new QLabel("  ", htab);
  uml_follow_php_get_name = new QCheckBox("also in uml", htab);
  if (GenerationSettings::uml_get_name == PhpView)
    uml_follow_php_get_name->setChecked(TRUE);
  connect(uml_follow_php_get_name, SIGNAL(toggled(bool)),
	  this, SLOT(follow_php_get_name()));
  
  new QLabel("Set operation\ndefault definition : ", grid);
  htab = new QHBox(grid);
  htab->setMargin(3);
  php_set_visibility.init(htab, GenerationSettings::javaphp_set_visibility,
			   FALSE, "Visibility (shared with Java)");
  php_set_visibility.connect(SIGNAL(clicked (int)), this, SLOT(php_set_visi_changed(int)));
  
  bg = new QButtonGroup(2, QGroupBox::Horizontal, "Modifiers", htab);
  bg->setExclusive(FALSE);
  php_set_final_cb = new QCheckBox("final", bg);
  php_set_final_cb->setChecked(GenerationSettings::php_set_final);
  
  new QLabel("  name : ", htab);
  edphp_set_name = new LineEdit(htab);
  edphp_set_name->setText(GenerationSettings::php_set_name);
  edphp_set_name->setFont(font);
  
  new QLabel("  ", htab);
  uml_follow_php_set_name = new QCheckBox("also in uml", htab);
  if (GenerationSettings::uml_set_name == PhpView)
    uml_follow_php_set_name->setChecked(TRUE);
  connect(uml_follow_php_set_name, SIGNAL(toggled(bool)),
	  this, SLOT(follow_php_set_name()));
  
  //new QLabel(grid);
  //new QLabel(grid);
  
  new QLabel("Operation\ndefault definition :", grid);
  edphp_oper_def = new MultiLineEdit(grid);
  edphp_oper_def->setText(GenerationSettings::php_oper_def);
  edphp_oper_def->setFont(font);

  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel("External classes : \nname making", grid);
  edphp_external_class_decl = new LineEdit(grid);
  edphp_external_class_decl->setText(GenerationSettings::php_external_class_decl);
  
  addTab(grid, "Php[2]");
  
  if (!GenerationSettings::php_get_default_defs())
    removePage(grid);
}  

void GenerationSettingsDialog::init_idl1() {
  QSplitter * split = new QSplitter(Vertical, this);
  
  split->setOpaqueResize(TRUE);
  
  QHBox * htab;
  
  htab = new QHBox(split);
  htab->setMargin(3);
  QLabel * lbl1 = 
    new QLabel("file default \ncontent :", htab);
  edidl_src_content = new MultiLineEdit(htab);
  edidl_src_content->setText(GenerationSettings::idl_src_content);
  QFont font = edidl_src_content->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  edidl_src_content->setFont(font);

  new QLabel("    file extension : ", htab);
  edidl_extension = new QComboBox(TRUE, htab);
  edidl_extension->insertItem(GenerationSettings::idl_extension);
  edidl_extension->setCurrentItem(0);
  edidl_extension->insertItem("Idl");

  QGrid * grid = new QGrid(2, split);
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Interface default \ndeclaration :", grid);
  edidl_interface_decl = new MultiLineEdit(grid);
  edidl_interface_decl->setText(GenerationSettings::idl_interface_decl);
  edidl_interface_decl->setFont(font);

  QLabel * lbl2 = 
    new QLabel("Valuetype default \ndeclaration :", grid);
  same_width(lbl1, lbl2);
  edidl_valuetype_decl = new MultiLineEdit(grid);
  edidl_valuetype_decl->setText(GenerationSettings::idl_valuetype_decl);
  edidl_valuetype_decl->setFont(font);

  new QLabel("Struct default \ndeclaration :", grid);
  edidl_struct_decl = new MultiLineEdit(grid);
  edidl_struct_decl->setText(GenerationSettings::idl_struct_decl);
  edidl_struct_decl->setFont(font);

  new QLabel("Union default \ndeclaration :", grid);
  edidl_union_decl = new MultiLineEdit(grid);
  edidl_union_decl->setText(GenerationSettings::idl_union_decl);
  edidl_union_decl->setFont(font);

  new QLabel("Enum default \ndeclaration :", grid);
  edidl_enum_decl = new MultiLineEdit(grid);
  edidl_enum_decl->setText(GenerationSettings::idl_enum_decl);
  edidl_enum_decl->setFont(font);

  new QLabel("Typedef default \ndeclaration :", grid);
  edidl_typedef_decl = new MultiLineEdit(grid);
  edidl_typedef_decl->setText(GenerationSettings::idl_typedef_decl);
  edidl_typedef_decl->setFont(font);

  new QLabel("Exception default \ndeclaration :", grid);
  edidl_exception_decl = new MultiLineEdit(grid);
  edidl_exception_decl->setText(GenerationSettings::idl_exception_decl);
  edidl_exception_decl->setFont(font);

  addTab(split, "Idl[1]");
  
  if (!GenerationSettings::idl_get_default_defs())
    removePage(split);
}

void GenerationSettingsDialog::init_idl2() {
  QGrid * grid = new QGrid(2, this);
  QGrid * grid2;
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Attribute default \ndeclaration :", grid);
  grid2 = new QGrid(2, grid);
  new QLabel("multiplicity '1'\nor unspecified", grid2);
  edidl_attr_decl[0] = new MultiLineEdit(grid2);
  new QLabel("multiplicity '*'\nor 'a..b'", grid2);
  edidl_attr_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (probably a\nnumber)", grid2);
  edidl_attr_decl[2] = new MultiLineEdit(grid2);

  QFont font = edjava_attr_decl[0]->font();
  int i;

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);

  for (i = 0; i != 3; i += 1) {
    edidl_attr_decl[i]->setText(GenerationSettings::idl_attr_decl[i]);
    edidl_attr_decl[i]->setFont(font);
  }

  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel("Attribute default \ndeclaration in\nvaluetype :", grid);
  grid2 = new QGrid(2, grid);
  new QLabel("multiplicity '1'\nor unspecified", grid2);
  edidl_valuetype_attr_decl[0] = new MultiLineEdit(grid2);
  new QLabel("multiplicity '*'\nor 'a..b'", grid2);
  edidl_valuetype_attr_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (probably a\nnumber)", grid2);
  edidl_valuetype_attr_decl[2] = new MultiLineEdit(grid2);
  for (i = 0; i != 3; i += 1) {
    edidl_valuetype_attr_decl[i]->setText(GenerationSettings::idl_valuetype_attr_decl[i]);
    edidl_valuetype_attr_decl[i]->setFont(font);
  }

  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel("Constant default \ndeclaration :", grid);
  grid2 = new QGrid(2, grid);
  new QLabel("multiplicity '1'\nor unspecified", grid2);
  edidl_const_decl[0] = new MultiLineEdit(grid2);
  new QLabel("multiplicity '*'\nor 'a..b'", grid2);
  edidl_const_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (probably a\nnumber)", grid2);
  edidl_const_decl[2] = new MultiLineEdit(grid2);
  for (i = 0; i != 3; i += 1) {
    edidl_const_decl[i]->setText(GenerationSettings::idl_const_decl[i]);
    edidl_const_decl[i]->setFont(font);
  }

  addTab(grid, "Idl[2]");
  
  if (!GenerationSettings::idl_get_default_defs())
    removePage(grid);
}

void GenerationSettingsDialog::init_idl3() {
  QGrid * grid = new QGrid(2, this);
  QGrid * grid2;
  int i;
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Association and\naggregation\ndefault\ndeclaration :", grid);
  grid2 = new QGrid(2, grid);
  new QLabel("multiplicity '1'\nor unspecified", grid2);
  edidl_rel_decl[0] = new MultiLineEdit(grid2);
  new QLabel("multiplicity '*'\nor 'a..b'", grid2);
  edidl_rel_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (probably a\nnumber)", grid2);
  edidl_rel_decl[2] = new MultiLineEdit(grid2);
  QFont font = edidl_rel_decl[0]->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  for (i = 0; i != 3; i += 1) {
    edidl_rel_decl[i]->setText(GenerationSettings::idl_rel_decl[i]);
    edidl_rel_decl[i]->setFont(font);
  }
  
  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel("Association and\naggregation\ndefault\ndeclaration in\nvaluetype :", grid);
  grid2 = new QGrid(2, grid);
  new QLabel("multiplicity '1'\nor unspecified", grid2);
  edidl_valuetype_rel_decl[0] = new MultiLineEdit(grid2);
  new QLabel("multiplicity '*'\nor 'a..b'", grid2);
  edidl_valuetype_rel_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (probably a\nnumber)", grid2);
  edidl_valuetype_rel_decl[2] = new MultiLineEdit(grid2);
  for (i = 0; i != 3; i += 1) {
    edidl_valuetype_rel_decl[i]->setText(GenerationSettings::idl_valuetype_rel_decl[i]);
    edidl_valuetype_rel_decl[i]->setFont(font);
  }
  
  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel("Association and\naggregation\ndefault\ndeclaration in\nunion:", grid);
  grid2 = new QGrid(2, grid);
  new QLabel("multiplicity '1'\nor unspecified", grid2);
  edidl_union_rel_decl[0] = new MultiLineEdit(grid2);
  new QLabel("multiplicity '*'\nor 'a..b'", grid2);
  edidl_union_rel_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (probably a\nnumber)", grid2);
  edidl_union_rel_decl[2] = new MultiLineEdit(grid2);
  for (i = 0; i != 3; i += 1) {
    edidl_union_rel_decl[i]->setText(GenerationSettings::idl_union_rel_decl[i]);
    edidl_union_rel_decl[i]->setFont(font);
  }
  
  addTab(grid, "Idl[3]");
  
  if (!GenerationSettings::idl_get_default_defs())
    removePage(grid);
}

void GenerationSettingsDialog::init_idl4() {
  QGrid * grid = new QGrid(2, this);
  QGrid * grid2;
  QHBox * htab;
  
  grid->setMargin(3);
  grid->setSpacing(3);

  new QLabel("Union item \ndefault \ndeclaration :", grid);
  grid2 = new QGrid(2, grid);
  new QLabel("multiplicity '1'\nor unspecified", grid2);
  edidl_union_item_decl[0] = new MultiLineEdit(grid2);
  new QLabel("multiplicity '*'\nor 'a..b'", grid2);
  edidl_union_item_decl[1] = new MultiLineEdit(grid2);
  new QLabel("X (probably a\nnumber)", grid2);
  edidl_union_item_decl[2] = new MultiLineEdit(grid2);
  
  QFont font = edidl_union_item_decl[0]->font();
  int i;

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  
  for (i = 0; i != 3; i += 1) {
    edidl_union_item_decl[i]->setText(GenerationSettings::idl_union_item_decl[i]);
    edidl_union_item_decl[i]->setFont(font);
  }

  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel("Enumeration item \ndefault declaration :", grid);
  edidl_enum_item_decl = new MultiLineEdit(grid);
  edidl_enum_item_decl->setText(GenerationSettings::idl_enum_item_decl);
  edidl_enum_item_decl->setFont(font);

  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel("Get operation\ndefault definition : ", grid);  
  htab = new QHBox(grid);
  new QLabel("name : ", htab);
  edidl_get_name = new LineEdit(htab);
  edidl_get_name->setText(GenerationSettings::idl_get_name);
  edidl_get_name->setFont(font);
  
  new QLabel("  ", htab);
  uml_follow_idl_get_name = new QCheckBox("also in uml", htab);
  if (GenerationSettings::uml_get_name == IdlView)
    uml_follow_idl_get_name->setChecked(TRUE);
  connect(uml_follow_idl_get_name, SIGNAL(toggled(bool)),
	  this, SLOT(follow_idl_get_name()));
  
  new QLabel("Set operation\ndefault definition : ", grid);
  htab = new QHBox(grid);
  idl_set_oneway_cb = new QCheckBox("oneway", htab);
  idl_set_oneway_cb->setChecked(GenerationSettings::idl_set_oneway);
  
  new QLabel("  name : ", htab);
  edidl_set_name = new LineEdit(htab);
  edidl_set_name->setText(GenerationSettings::idl_set_name);
  edidl_set_name->setFont(font);
  
  new QLabel("  ", htab);
  uml_follow_idl_set_name = new QCheckBox("also in uml", htab);
  if (GenerationSettings::uml_set_name == IdlView)
    uml_follow_idl_set_name->setChecked(TRUE);
  connect(uml_follow_idl_set_name, SIGNAL(toggled(bool)),
	  this, SLOT(follow_idl_set_name()));
  
  new QLabel(grid);
  new QLabel(grid);
  
  new QLabel("Operation default \ndeclaration : ", grid);
  edidl_oper_decl = new MultiLineEdit(grid);
  edidl_oper_decl->setText(GenerationSettings::idl_oper_decl);
  edidl_oper_decl->setFont(font);

  addTab(grid, "Idl[4]");
  
  if (!GenerationSettings::idl_get_default_defs())
    removePage(grid);
}

void GenerationSettingsDialog::init_idl5() {
  QSplitter * split = new QSplitter(Vertical, this);
  
  split->setOpaqueResize(TRUE);
  
  QHBox * htab;
  
  htab = new QHBox(split);
  htab->setMargin(3);
  QLabel * lbl1 = 
    new QLabel("External classes : \nname making\n#include", htab);
  edidl_external_class_decl = new MultiLineEdit(htab);
  edidl_external_class_decl->setText(GenerationSettings::idl_external_class_decl);
  htab = new QHBox(split);
  htab->setMargin(3);
  QLabel * lbl2 = 
    new QLabel("External types :\n#include form(s)\netc...", htab);
  idl_include_table =
    new IncludeTable(htab, GenerationSettings::idl_includes,
		     "Include etc...", "#include \"\"");

  same_width(lbl1, lbl2);
  
  addTab(split, "Idl[5]");
  
  if (!GenerationSettings::idl_get_default_defs())
    removePage(split);
}

void GenerationSettingsDialog::init_descriptions() {
  QSplitter * split = new QSplitter(Vertical, this);
  
  split->setOpaqueResize(TRUE);
  
  QHBox * htab;
  
  htab = new QHBox(split);
  htab->setMargin(3);
  new QLabel("Artifact\ndefault\ndescription : ", htab);
  edartifact_default_description = new MultiLineEdit(htab);
  edartifact_default_description->setText(GenerationSettings::artifact_default_description);
  
  htab = new QHBox(split);
  htab->setMargin(3);
  new QLabel("Class\ndefault\ndescription : ", htab);
  edclass_default_description = new MultiLineEdit(htab);
  edclass_default_description->setText(GenerationSettings::class_default_description);
  
  htab = new QHBox(split);
  htab->setMargin(3);
  new QLabel("Operation\ndefault\ndescription : ", htab);
  edoperation_default_description = new MultiLineEdit(htab);
  edoperation_default_description->setText(GenerationSettings::operation_default_description);
  
  htab = new QHBox(split);
  htab->setMargin(3);
  new QLabel("Attribute\ndefault\ndescription : ", htab);
  edattribute_default_description = new MultiLineEdit(htab);
  edattribute_default_description->setText(GenerationSettings::attribute_default_description);
  
  htab = new QHBox(split);
  htab->setMargin(3);
  new QLabel("Relation\ndefault\ndescription : ", htab);
  edrelation_default_description = new MultiLineEdit(htab);
  edrelation_default_description->setText(GenerationSettings::relation_default_description);
  
  addTab(split, "Description");
}

static const char * Relative = "Set it relative";
static const char * Absolute = "Set it absolute";

void GenerationSettingsDialog::init_dirs() {

  QPushButton * button;
  QVBox * vtab = new QVBox(this);
  QHBox * htab;
  
  vtab->setMargin(3);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  new QLabel("Defining a project root directory allows to specify \
packages's generation directory relative to the root directory rather \
than absolute.\n"
	     "A root directory may itself be relative to the project path",
	     htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  QLabel * lbl1 = new QLabel("C++ root dir : ", htab);
  edcpproot = new LineEdit(GenerationSettings::cpp_root_dir, htab);
  new QLabel(" ", htab);
  button = new QPushButton("Browse", htab);
  connect(button, SIGNAL(clicked ()), this, SLOT(cpproot_browse()));
  new QLabel("", htab);
  cpprelbutton = new QPushButton((GenerationSettings::cpp_root_dir.isEmpty() || 
				  QDir::isRelativePath(GenerationSettings::cpp_root_dir))
				 ? Absolute : Relative, htab);
  connect(cpprelbutton, SIGNAL(clicked ()), this, SLOT(cpp_relative()));
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  QLabel * lbl2 = new QLabel("Java root dir : ", htab);
  edjavaroot = new LineEdit(GenerationSettings::java_root_dir, htab);
  new QLabel(" ", htab);
  button = new QPushButton("Browse", htab);
  connect(button, SIGNAL(clicked ()), this, SLOT(javaroot_browse()));
  new QLabel("", htab);
  javarelbutton = new QPushButton((GenerationSettings::java_root_dir.isEmpty() || 
				   QDir::isRelativePath(GenerationSettings::java_root_dir))
				  ? Absolute : Relative, htab);
  connect(javarelbutton, SIGNAL(clicked ()), this, SLOT(java_relative()));
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  QLabel * lbl3 = new QLabel("Php root dir : ", htab);
  edphproot = new LineEdit(GenerationSettings::php_root_dir, htab);
  new QLabel(" ", htab);
  button = new QPushButton("Browse", htab);
  connect(button, SIGNAL(clicked ()), this, SLOT(phproot_browse()));
  new QLabel("", htab);
  phprelbutton = new QPushButton((GenerationSettings::php_root_dir.isEmpty() || 
				   QDir::isRelativePath(GenerationSettings::php_root_dir))
				  ? Absolute : Relative, htab);
  connect(phprelbutton, SIGNAL(clicked ()), this, SLOT(php_relative()));
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  new QLabel("", htab);
  
  htab = new QHBox(vtab);
  htab->setMargin(3);
  QLabel * lbl4 = new QLabel("Idl root dir : ", htab);
  edidlroot = new LineEdit(GenerationSettings::idl_root_dir, htab);
  new QLabel(" ", htab);
  button = new QPushButton("Browse", htab);
  connect(button, SIGNAL(clicked ()), this, SLOT(idlroot_browse()));
  new QLabel("", htab);
  idlrelbutton = new QPushButton((GenerationSettings::idl_root_dir.isEmpty() || 
				  QDir::isRelativePath(GenerationSettings::idl_root_dir))
				 ? Absolute : Relative, htab);
  connect(idlrelbutton, SIGNAL(clicked ()), this, SLOT(idl_relative()));
  new QLabel("", htab);
  
  same_width(lbl1, lbl2, lbl3, lbl4);
  
  vtab->setStretchFactor(new QHBox(vtab), 1000);
  
  addTab(vtab, "Directory");
}

static QString add_last_slash(QString s)
{
  if (!s.isEmpty()) {
    int i = s.length() - 1;
    
    switch (s.at(i).latin1()) {
    case '/':
    case '\\':
      break;
    default:
      return QDir::convertSeparators(s + "/");
    }
  }
  
  return s;
}

void GenerationSettingsDialog::follow_cpp_get_name() {
  if (uml_follow_cpp_get_name->isChecked()) {
    uml_follow_java_get_name->setChecked(FALSE);
    uml_follow_php_get_name->setChecked(FALSE);
    uml_follow_idl_get_name->setChecked(FALSE);
  }
}

void GenerationSettingsDialog::follow_cpp_set_name() {
  if (uml_follow_cpp_set_name->isChecked()) {
    uml_follow_java_set_name->setChecked(FALSE);
    uml_follow_php_set_name->setChecked(FALSE);
    uml_follow_idl_set_name->setChecked(FALSE);
  }
}

void GenerationSettingsDialog::follow_java_get_name() {
  if (uml_follow_java_get_name->isChecked()) {
    uml_follow_cpp_get_name->setChecked(FALSE);
    uml_follow_php_get_name->setChecked(FALSE);
    uml_follow_idl_get_name->setChecked(FALSE);
  }
}

void GenerationSettingsDialog::follow_java_set_name() {
  if (uml_follow_java_set_name->isChecked()) {
    uml_follow_cpp_set_name->setChecked(FALSE);
    uml_follow_php_set_name->setChecked(FALSE);
    uml_follow_idl_set_name->setChecked(FALSE);
  }
}

void GenerationSettingsDialog::follow_php_get_name() {
  if (uml_follow_php_get_name->isChecked()) {
    uml_follow_cpp_get_name->setChecked(FALSE);
    uml_follow_java_get_name->setChecked(FALSE);
    uml_follow_idl_get_name->setChecked(FALSE);
  }
}

void GenerationSettingsDialog::follow_php_set_name() {
  if (uml_follow_php_set_name->isChecked()) {
    uml_follow_cpp_set_name->setChecked(FALSE);
    uml_follow_java_set_name->setChecked(FALSE);
    uml_follow_idl_set_name->setChecked(FALSE);
  }
}

void GenerationSettingsDialog::follow_idl_get_name() {
  if (uml_follow_idl_get_name->isChecked()) {
    uml_follow_cpp_get_name->setChecked(FALSE);
    uml_follow_java_get_name->setChecked(FALSE);
    uml_follow_php_get_name->setChecked(FALSE);
  }
}

void GenerationSettingsDialog::follow_idl_set_name() {
  if (uml_follow_idl_set_name->isChecked()) {
    uml_follow_cpp_set_name->setChecked(FALSE);
    uml_follow_java_set_name->setChecked(FALSE);
    uml_follow_php_set_name->setChecked(FALSE);
  }
}

void GenerationSettingsDialog::accept() {
  if (types_table->check()) {
    QString enum_in = cpp_enum_in->text().stripWhiteSpace();
    QString enum_out = cpp_enum_out->text().stripWhiteSpace();
    QString enum_inout = cpp_enum_inout->text().stripWhiteSpace();
    QString enum_return = cpp_enum_return->text().stripWhiteSpace();
    
    if (enum_in.find("${type}") == -1) {
      msg_critical("Error", "C++ 'in' enum argument default passing must contains ${type}");
      return;
    }
    if (enum_out.find("${type}") == -1) {
      msg_critical("Error", "C++ 'out' enum argument default passing must contains ${type}");
      return;
    }
    if (enum_inout.find("${type}") == -1) {
      msg_critical("Error", "C++ 'inout' enum argument default passing must contains ${type}");
      return;
    }
    if (enum_return.find("${type}") == -1) {
      msg_critical("Error", "C++ 'return' enum argument default passing must contains ${type}");
      return;
    }

    QString in = cpp_in->text().stripWhiteSpace();
    QString out = cpp_out->text().stripWhiteSpace();
    QString inout = cpp_inout->text().stripWhiteSpace();
    QString opreturn = cpp_return->text().stripWhiteSpace();
    
    if (in.find("${type}") == -1) {
      msg_critical("Error", "C++ 'in' argument default passing must contains ${type}");
      return;
    }
    if (out.find("${type}") == -1) {
      msg_critical("Error", "C++ 'out' argument default passing must contains ${type}");
      return;
    }
    if (inout.find("${type}") == -1) {
      msg_critical("Error", "C++ 'inout' argument default passing must contains ${type}");
      return;
    }
    if (opreturn.find("${type}") == -1) {
      msg_critical("Error", "C++ 'return' argument default passing must contains ${type}");
      return;
    }

    types_table->update();
    relation_stereotypes_table->update(GenerationSettings::nrelattrstereotypes,
				       GenerationSettings::relattr_stereotypes);
    class_stereotypes_table->update(GenerationSettings::nclassstereotypes,
				    GenerationSettings::class_stereotypes);
    cpp_include_table->update();
    java_import_table->update();
    idl_include_table->update();
    
    GenerationSettings::cpp_enum_in = enum_in;
    GenerationSettings::cpp_enum_out = enum_out;
    GenerationSettings::cpp_enum_inout = enum_inout;
    GenerationSettings::cpp_enum_return = enum_return;

    GenerationSettings::cpp_in = in;
    GenerationSettings::cpp_out = out;
    GenerationSettings::cpp_inout = inout;
    GenerationSettings::cpp_return = opreturn;

    GenerationSettings::cpp_h_extension = edcpp_h_extension->currentText();
    GenerationSettings::cpp_src_extension = edcpp_src_extension->currentText();
    GenerationSettings::java_extension = edjava_extension->currentText();
    GenerationSettings::php_extension = edphp_extension->currentText();
    GenerationSettings::idl_extension = edidl_extension->currentText();

    GenerationSettings::cpp_h_content = edcpp_h_content->text();
    GenerationSettings::cpp_src_content = edcpp_src_content->text();
    GenerationSettings::java_src_content = edjava_src_content->text();
    GenerationSettings::php_src_content = edphp_src_content->text();
    GenerationSettings::idl_src_content = edidl_src_content->text();
    
    switch (cpp_include_with_path_cb->currentItem()) {
    case 0:
      GenerationSettings::cpp_include_with_path = FALSE;
      GenerationSettings::cpp_relative_path = FALSE;
      GenerationSettings::cpp_root_relative_path = FALSE;
      break;
    case 1:
      GenerationSettings::cpp_include_with_path = TRUE;
      GenerationSettings::cpp_relative_path = FALSE;
      GenerationSettings::cpp_root_relative_path = FALSE;
      break;
    case 2:
      GenerationSettings::cpp_include_with_path = TRUE;
      GenerationSettings::cpp_relative_path = TRUE;
      GenerationSettings::cpp_root_relative_path = FALSE;
      break;
    default:
      GenerationSettings::cpp_include_with_path = TRUE;
      GenerationSettings::cpp_relative_path = FALSE;
      GenerationSettings::cpp_root_relative_path = TRUE;
    }
    
    GenerationSettings::cpp_force_namespace_gen = 
      cpp_force_namespace_gen_cb->isChecked();

    GenerationSettings::cpp_javadoc_comment = cpp_javadoc_cb->isChecked();

    int i;
    
    GenerationSettings::cpp_class_decl = edcpp_class_decl->text();
    GenerationSettings::cpp_external_class_decl = edcpp_external_class_decl->text();
    GenerationSettings::cpp_struct_decl = edcpp_struct_decl->text();
    GenerationSettings::cpp_typedef_decl = edcpp_typedef_decl->text();
    GenerationSettings::cpp_union_decl = edcpp_union_decl->text();
    GenerationSettings::cpp_enum_decl = edcpp_enum_decl->text();
    for (i = 0; i != 3; i += 1)
      GenerationSettings::cpp_attr_decl[i] = edcpp_attr_decl[i]->text();
    GenerationSettings::cpp_enum_item_decl = edcpp_enum_item_decl->text();
    GenerationSettings::cpp_oper_decl = edcpp_oper_decl->text();
    GenerationSettings::cpp_oper_def = edcpp_oper_def->text();
    GenerationSettings::cpp_force_throw = cpp_force_throw_cb->isChecked();
    
    GenerationSettings::java_class_decl = edjava_class_decl->text();
    GenerationSettings::java_external_class_decl = edjava_external_class_decl->text();
    GenerationSettings::java_interface_decl = edjava_interface_decl->text();
    GenerationSettings::java_enum_decl = edjava_enum_decl->text();
    GenerationSettings::java_enum_pattern_decl = edjava_enum_pattern_decl->text();
    GenerationSettings::java_enum_item_decl = edjava_enum_item_decl->text();
    GenerationSettings::java_enum_pattern_item_decl = edjava_enum_pattern_item_decl->text();
    GenerationSettings::java_enum_pattern_item_case = edjava_enum_pattern_item_case->text();
    for (i = 0; i != 3; i += 1)
      GenerationSettings::java_attr_decl[i] = edjava_attr_decl[i]->text();
    GenerationSettings::java_oper_def = edjava_oper_def->text();
    GenerationSettings::java_javadoc_comment = java_javadoc_cb->isChecked();
    
    GenerationSettings::php_class_decl = edphp_class_decl->text();
    GenerationSettings::php_external_class_decl = edphp_external_class_decl->text();
    GenerationSettings::php_interface_decl = edphp_interface_decl->text();
    GenerationSettings::php_enum_decl = edphp_enum_decl->text();
    GenerationSettings::php_enum_item_decl = edphp_enum_item_decl->text();
    GenerationSettings::php_attr_decl = edphp_attr_decl->text();
    GenerationSettings::php_oper_def = edphp_oper_def->text();
    
    GenerationSettings::idl_interface_decl = edidl_interface_decl->text();
    GenerationSettings::idl_valuetype_decl = edidl_valuetype_decl->text();
    GenerationSettings::idl_struct_decl = edidl_struct_decl->text();
    GenerationSettings::idl_union_decl = edidl_union_decl->text();
    GenerationSettings::idl_enum_decl = edidl_enum_decl->text();
    GenerationSettings::idl_exception_decl = edidl_exception_decl->text();
    GenerationSettings::idl_typedef_decl = edidl_typedef_decl->text();
    GenerationSettings::idl_external_class_decl = edidl_external_class_decl->text();
    for (i = 0; i != 3; i += 1) {
      GenerationSettings::idl_attr_decl[i] = edidl_attr_decl[i]->text();
      GenerationSettings::idl_valuetype_attr_decl[i] = edidl_valuetype_attr_decl[i]->text();
      GenerationSettings::idl_const_decl[i] = edidl_const_decl[i]->text();
      GenerationSettings::idl_union_item_decl[i] = edidl_union_item_decl[i]->text();
    }
    GenerationSettings::idl_enum_item_decl = edidl_enum_item_decl->text();
    GenerationSettings::idl_oper_decl = edidl_oper_decl->text();

    int j;
    
    for (i = 0; i != 2; i += 1)
      for (j = 0; j != 3; j += 1)
	GenerationSettings::cpp_rel_decl[i][j] = 
	  edcpp_rel_decl[i][j]->text();
    
    for (i = 0; i != 3; i += 1)
      GenerationSettings::java_rel_decl[i] = 
	edjava_rel_decl[i]->text();
    
    GenerationSettings::php_rel_decl = edphp_rel_decl->text();
    
    for (i = 0; i != 3; i += 1) {
      GenerationSettings::idl_rel_decl[i] = 
	edidl_rel_decl[i]->text();
      GenerationSettings::idl_valuetype_rel_decl[i] = 
	edidl_valuetype_rel_decl[i]->text();
      GenerationSettings::idl_union_rel_decl[i] = 
	edidl_union_rel_decl[i]->text();
    }
    
    //
    
    GenerationSettings::cpp_get_visibility = cpp_get_visibility.value();
    GenerationSettings::cpp_get_name = edcpp_get_name->text();
    GenerationSettings::cpp_get_inline = cpp_get_inline_cb->isChecked();
    GenerationSettings::cpp_get_const = cpp_get_const_cb->isChecked();
    GenerationSettings::cpp_get_value_const = cpp_get_value_const_cb->isChecked();
    GenerationSettings::cpp_set_visibility = cpp_set_visibility.value();
    GenerationSettings::cpp_set_name = edcpp_set_name->text();
    GenerationSettings::cpp_set_inline = cpp_set_inline_cb->isChecked();
    GenerationSettings::cpp_set_param_const = cpp_set_param_const_cb->isChecked();
    GenerationSettings::cpp_set_param_ref = cpp_set_param_ref_cb->isChecked();
    
    GenerationSettings::javaphp_get_visibility = java_get_visibility.value();
    GenerationSettings::java_get_name = edjava_get_name->text();
    GenerationSettings::java_get_final = java_get_final_cb->isChecked();
    GenerationSettings::javaphp_set_visibility = java_set_visibility.value();
    GenerationSettings::java_set_name = edjava_set_name->text();
    GenerationSettings::java_set_final = java_set_final_cb->isChecked();
    GenerationSettings::java_set_param_final = java_set_param_final_cb->isChecked();

    GenerationSettings::php_get_name = edphp_get_name->text();
    GenerationSettings::php_get_final = php_get_final_cb->isChecked();
    GenerationSettings::php_set_name = edphp_set_name->text();
    GenerationSettings::php_set_final = php_set_final_cb->isChecked();

    GenerationSettings::idl_get_name = edidl_get_name->text();
    GenerationSettings::idl_set_name = edidl_set_name->text();
    GenerationSettings::idl_set_oneway = idl_set_oneway_cb->isChecked();

    //
    
    GenerationSettings::umltypes.clear();
    
    for (i = 0; i != GenerationSettings::nbuiltins; i += 1)
      GenerationSettings::umltypes.append(GenerationSettings::builtins[i].uml);

    //
    
    GenerationSettings::artifact_default_description = edartifact_default_description->text();
    GenerationSettings::class_default_description = edclass_default_description->text();
    GenerationSettings::operation_default_description = edoperation_default_description->text();
    GenerationSettings::attribute_default_description = edattribute_default_description->text();
    GenerationSettings::relation_default_description = edrelation_default_description->text();
    
    //
    
    if (uml_follow_cpp_get_name->isChecked())
      GenerationSettings::uml_get_name = CppView;
    else if (uml_follow_java_get_name->isChecked())
      GenerationSettings::uml_get_name = JavaView;
    else if (uml_follow_php_get_name->isChecked())
      GenerationSettings::uml_get_name = PhpView;
    else if (uml_follow_idl_get_name->isChecked())
      GenerationSettings::uml_get_name = IdlView;
    else
      GenerationSettings::uml_get_name = UmlView;
    
    if (uml_follow_cpp_set_name->isChecked())
      GenerationSettings::uml_set_name = CppView;
    else if (uml_follow_java_set_name->isChecked())
      GenerationSettings::uml_set_name = JavaView;
    else if (uml_follow_php_set_name->isChecked())
      GenerationSettings::uml_set_name = PhpView;
    else if (uml_follow_idl_set_name->isChecked())
      GenerationSettings::uml_set_name = IdlView;
    else
      GenerationSettings::uml_set_name = UmlView;

    //
    
    GenerationSettings::cpp_root_dir = add_last_slash(edcpproot->text());
    GenerationSettings::java_root_dir = add_last_slash(edjavaroot->text());
    GenerationSettings::php_root_dir = add_last_slash(edphproot->text());
    GenerationSettings::idl_root_dir = add_last_slash(edidlroot->text());
    
    //
    
    QDialog::accept();
  }
}

void GenerationSettingsDialog::cpproot_browse() {
  QString dir =
    QFileDialog::getExistingDirectory(edcpproot->text(), this, 0,
				      "C++ root directory");
  
  if (! dir.isNull())
    edcpproot->setText(dir);
}

void GenerationSettingsDialog::javaroot_browse() {
  QString dir =
    QFileDialog::getExistingDirectory(edjavaroot->text(), this, 0,
				      "Java root directory");
  
  if (! dir.isNull())
    edjavaroot->setText(dir);
}

void GenerationSettingsDialog::phproot_browse() {
  QString dir =
    QFileDialog::getExistingDirectory(edphproot->text(), this, 0,
				      "Php root directory");
  
  if (! dir.isNull())
    edphproot->setText(dir);
}

void GenerationSettingsDialog::idlroot_browse() {
  QString dir =
    QFileDialog::getExistingDirectory(edidlroot->text(), this, 0,
				      "Idl root directory");
  
  if (! dir.isNull())
    edidlroot->setText(dir);
}

void GenerationSettingsDialog::relative(LineEdit * ed, QPushButton * button) {
  QString root = BrowserView::get_dir().absPath();
  const QString s = ed->text();

  if (root.at(root.length() - 1) != QChar('/'))
    root += '/';
    
  if (button->text() == Relative) {
    unsigned len = root.length();
      
    if (
#ifdef WIN32
	(s.lower().find(root.lower()) == 0) &&
#else
	(s.find(root) == 0) &&
#endif
	(s.length() >= len)) {
      ed->setText(s.mid(len));
      button->setText(Absolute);
    }
  }
  else {
    ed->setText(root + s);
    button->setText(Relative);
  }
}

void GenerationSettingsDialog::cpp_relative() {
  relative(edcpproot, cpprelbutton);
}

void GenerationSettingsDialog::java_relative() {
  relative(edjavaroot, javarelbutton);
}

void GenerationSettingsDialog::php_relative() {
  relative(edphproot, phprelbutton);
}

void GenerationSettingsDialog::idl_relative() {
  relative(edidlroot, idlrelbutton);
}

//

void GenerationSettingsDialog::java_get_visi_changed(int) {
  php_get_visibility.follow(java_get_visibility);
}

void GenerationSettingsDialog::java_set_visi_changed(int) {
  php_set_visibility.follow(java_set_visibility);
}

void GenerationSettingsDialog::php_get_visi_changed(int) {
  java_get_visibility.follow(php_get_visibility);
}

void GenerationSettingsDialog::php_set_visi_changed(int) {
  java_get_visibility.follow(php_set_visibility);
}

// TypesTable

TypesTable::TypesTable(QWidget * parent)
    : StringTable(GenerationSettings::nbuiltins + 1, 9, parent, FALSE) {
  horizontalHeader()->setLabel(0, "Uml");
  horizontalHeader()->setLabel(1, "C++");
  horizontalHeader()->setLabel(2, "Java");
  horizontalHeader()->setLabel(3, "Idl");
  horizontalHeader()->setLabel(4, "C++ in");
  horizontalHeader()->setLabel(5, "C++ out");
  horizontalHeader()->setLabel(6, "C++ in out");
  horizontalHeader()->setLabel(7, "C++ return");
  horizontalHeader()->setLabel(8, "do");
  
  int index;
  
  for (index = 0; index < GenerationSettings::nbuiltins; index += 1){
    Builtin & b = GenerationSettings::builtins[index];
    
    setText(index, 0, b.uml);
    setText(index, 1, b.cpp);
    setText(index, 2, b.java);
    setText(index, 3, b.idl);
    setText(index, 4, b.cpp_in);
    setText(index, 5, b.cpp_out);
    setText(index, 6, b.cpp_inout);
    setText(index, 7, b.cpp_return);
    setText(index, 8, QString::null);
  }
  
  init_row(index);
  
  for (index = 4; index < 8; index += 1)
    setColumnStretchable (index, TRUE);
  adjustColumn(8);
  setColumnStretchable (8, FALSE);
}

void TypesTable::init_row(int index) {
  setText(index, 0, QString::null);
  setText(index, 1, QString::null);
  setText(index, 2, QString::null);
  setText(index, 3, QString::null);
  setText(index, 4, "${type}");
  setText(index, 5, "${type} &");
  setText(index, 6, "${type} &");
  setText(index, 7, "${type}");
}

void TypesTable::update() {
  forceUpdateCells();
  
  int n = numRows();
  int index;
  
  if (text(n - 1, 0).isEmpty())
    n -= 1;
  
  delete [] GenerationSettings::builtins;
  GenerationSettings::nbuiltins = n;
  GenerationSettings::builtins = new Builtin[n];
  
  for (index = 0; index != n; index += 1) {
    Builtin & b = GenerationSettings::builtins[index];
    
    b.uml = text(index, 0).stripWhiteSpace();
    b.cpp = text(index, 1).stripWhiteSpace();
    b.java = text(index, 2).stripWhiteSpace();
    b.idl = text(index, 3).stripWhiteSpace();
    b.cpp_in = text(index, 4).stripWhiteSpace();
    b.cpp_out = text(index, 5).stripWhiteSpace();
    b.cpp_inout = text(index, 6).stripWhiteSpace();
    b.cpp_return = text(index, 7).stripWhiteSpace();
  }
}

bool TypesTable::check() {
  int n = numRows();
  int index;
  
  if (text(n - 1, 0).isEmpty())
    n -= 1;
  
  for (index = 0; index != n; index += 1) {
    int col;
    
    for (col = 0; col != 4; col += 1) {
      if (text(index, col).stripWhiteSpace().isEmpty()) {
	QString err;

	err.sprintf("row %d : %s specification is mandatory", 
		    index + 1, (const char *) horizontalHeader()->label(col));
	msg_critical("Error", err);
	return FALSE;
      }
    }
    
    for (col = 4; col != 8; col += 1) {
      if (text(index, col).find("${type}") == -1) {
	QString err;

	err.sprintf("row %d : %s '%s' argument default passing does not contains ${type}", 
		    index + 1, (const char *) text(index, 0),
		    (const char *) horizontalHeader()->label(col));
	msg_critical("Error", err);

	return FALSE;
      }
    }
  }
  
  return TRUE;
}

// StereotypesTable

StereotypesTable::StereotypesTable(QWidget * parent, int nst,
				   Stereotype * st, bool php)
    : StringTable(nst + 1, (php) ? 6 : 5, parent, FALSE), with_php(php) {
  horizontalHeader()->setLabel(0, "Uml");
  horizontalHeader()->setLabel(1, "C++");
  horizontalHeader()->setLabel(2, "Java");
  if (with_php) {
    horizontalHeader()->setLabel(3, "Php");
    horizontalHeader()->setLabel(4, "Idl");
    horizontalHeader()->setLabel(5, "do");
    
    int index;
    
    for (index = 0; index < nst; index += 1){
      Stereotype & s = st[index];
      
      setText(index, 0, s.uml);
      setText(index, 1, s.cpp);
      setText(index, 2, s.java);
      setText(index, 3, s.php);
      setText(index, 4, s.idl);
      setText(index, 5, QString::null);
    }
    
    init_row(index);
    
    for (index = 0; index != 5; index += 1)
      setColumnStretchable (index, TRUE);
    adjustColumn(5);
    setColumnStretchable (5, FALSE);
  }
  else {
    horizontalHeader()->setLabel(3, "Idl");
    horizontalHeader()->setLabel(4, "do");
    
    int index;
    
    for (index = 0; index < nst; index += 1){
      Stereotype & s = st[index];
      
      setText(index, 0, s.uml);
      setText(index, 1, s.cpp);
      setText(index, 2, s.java);
      setText(index, 3, s.idl);
      setText(index, 4, QString::null);
    }
    
    init_row(index);
    
    for (index = 0; index != 4; index += 1)
      setColumnStretchable (index, TRUE);
    adjustColumn(4);
    setColumnStretchable (4, FALSE);
  }
}

void StereotypesTable::init_row(int index) {
  setText(index, 0, QString::null);
  setText(index, 1, QString::null);
  setText(index, 2, QString::null);
  setText(index, 3, QString::null);
  if (with_php)
    setText(index, 3, QString::null);
}

void StereotypesTable::update(int & nst, Stereotype *& st) {
  forceUpdateCells();
  
  int n = numRows();
  int index;
  
  if (text(n - 1, 0).isEmpty())
    n -= 1;
  
  delete [] st;
  nst = n;
  st = new Stereotype[n];
  
  if (with_php) {
    for (index = 0; index != n; index += 1) {
      Stereotype & s = st[index];
      
      s.uml = text(index, 0).stripWhiteSpace();
      s.cpp = text(index, 1).stripWhiteSpace();
      s.java = text(index, 2).stripWhiteSpace();
      s.php = text(index, 3).stripWhiteSpace();
      s.idl = text(index, 4).stripWhiteSpace();
    }
  }
  else {
    for (index = 0; index != n; index += 1) {
      Stereotype & s = st[index];
      
      s.uml = text(index, 0).stripWhiteSpace();
      s.cpp = text(index, 1).stripWhiteSpace();
      s.java = text(index, 2).stripWhiteSpace();
      s.idl = text(index, 3).stripWhiteSpace();
    }
  }
}

// IncludeTable

IncludeTable::IncludeTable(QWidget * parent, IncludesSpec & spc,
			   const char * title, const char * df)
    : StringTable(spc.types.count() + 1, 3, parent, FALSE),
      spec(spc), dflt(df) {
  horizontalHeader()->setLabel(0, "External type");
  horizontalHeader()->setLabel(1, title);
  horizontalHeader()->setLabel(2, "do");
  
  int index;
  int sup = spc.types.count();
  QStringList::Iterator it_type = spc.types.begin();
  QStringList::Iterator it_incl = spc.includes.begin();
  
  for (index = 0; index < sup; index += 1, it_type++, it_incl++) {
    setText(index, 0, *it_type);
    setItem(index, 1, new MLinesItem(this, *it_incl));
    setText(index, 2, QString::null);
    setRowStretchable(index, TRUE);
    adjustRow(index);
  }
  
  setText(index, 0, QString::null);
  setItem(index, 1, new MLinesItem(this, dflt));
  setText(index, 2, QString::null);
  setRowStretchable(index, TRUE);
  
  adjustColumn(0);
  setColumnStretchable(1, TRUE);
  adjustColumn(2);
  setColumnStretchable(2, FALSE);
			
  connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	  this, SLOT(button_pressed(int, int, int, const QPoint &)));
  connect(this, SIGNAL(valueChanged(int, int)),
	  this, SLOT(value_changed(int, int)));
}

void IncludeTable::init_row(int index) {
  setText(index, 0, QString::null);
  setItem(index, 1, new MLinesItem(this, QString::null));
  setText(index, 2, QString::null);
  setRowStretchable(index, TRUE);
}

void IncludeTable::update() {
  forceUpdateCells();
  
  int n = numRows();
  int index;
  
  spec.types.clear();
  spec.includes.clear();
  
  for (index = 0; index != n; index += 1) {
    QString t = text(index, 0).stripWhiteSpace();
    
    if (! t.isEmpty()) {
      spec.types.append(t);
      spec.includes.append(text(index, 1));
    }
  }
}
