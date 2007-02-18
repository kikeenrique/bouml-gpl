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

#ifndef RELATIONDIALOG_H
#define  RELATIONDIALOG_H

#include <qwidgetlist.h>
#include <qstringlist.h>
#include <qtabdialog.h>

#include "BrowserNode.h"
#include "VisibilityGroup.h"

class QWidget;
class QComboBox;
class QCheckBox;
class QPushButton;
class QGroupBox;
class QButtonGroup;
class QVBox;
class QRadioButton;

class LineEdit;
class MultiLineEdit;
class RelationData;
class BrowserNode;
class KeyValuesTable;
class BrowserClass;
class ClassData;
struct RoleData;
class BodyDialog;

struct RoleDialog {
  // Uml
  bool visit;
  LineEdit * edrole;
  QComboBox * multiplicity;
  LineEdit * edinit;
  VisibilityGroup uml_visibility;
  QCheckBox * classrelation_cb;
  QCheckBox * constrelation_cb;
  MultiLineEdit * comment;
  QWidgetList opt;
  
  // C++
  bool cpp_undef;
  QCheckBox * cpp_virtual_inheritance_cb;
  VisibilityGroup cpp_visibility;
  QCheckBox * mutable_cb;
  QCheckBox * volatile_cb;
  MultiLineEdit * edcppdecl;
  MultiLineEdit * showcppdecl;
  QPushButton * cpp_default_decl_bt;
  QPushButton * cpp_include_in_header; // role a only
  QPushButton * cpp_unmapped_decl_bt;
  
  // Java
  bool java_undef;
  QCheckBox * transient_cb;
  MultiLineEdit * edjavadecl;
  MultiLineEdit * showjavadecl;
  QPushButton * java_default_decl_bt;
  QPushButton * java_unmapped_decl_bt;
  QPushButton * editjavaannotation;
  QString javaannotation;
  
  // Idl
  bool idl_undef;
  bool idl_in_struct;
  bool idl_in_union;
  bool idl_in_valuetype;
  QCheckBox * idl_truncatable_inheritance_cb;
  QComboBox * edcase;
  BrowserNodeList enums;
  QStringList enum_names;
  MultiLineEdit * edidldecl;
  MultiLineEdit * showidldecl;
  QPushButton * idl_default_decl_bt;
  QPushButton * idl_unmapped_decl_bt;
  
  // User
  KeyValuesTable * kvtable;
};

class RelationDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    // Uml
    RelationData * rel;
    LineEdit * edname;
    QComboBox * edtype;
    QComboBox * edstereotype;
    RoleDialog a;
    RoleDialog b;
    UmlCode current_type;
    QStringList list;
    BrowserNodeList nodes;
    QComboBox * edassociation;
    QGroupBox * uml_bgroup_b;
    QGroupBox * cpp_b;
    QGroupBox * java_b;
    QGroupBox * idl_b;
    QList<BodyDialog> edits;
        
    static QSize previous_size;
  
  protected:
    void init_uml_role(RoleDialog & role, const RoleData & rel,
		       QGroupBox * bg, BrowserClass * cl1, BrowserClass * cl2);
    void init_cpp_role(RoleDialog & role, const RoleData & rel,
		       QGroupBox * bg, const char * cpp_update_slot, 
		       const char * cpp_default_slot,
		       const char * cpp_unmapped_slot,
		       const char * cpp_include_in_header_slot);
    void init_java_role(RoleDialog & role, const RoleData & rel,
			QGroupBox * bg, const char * java_update_slot, 
			const char * java_default_slot,
			const char * java_unmapped_slot,
			const char * java_edit_annotation);
    void init_idl_role(RoleDialog & role, const RoleData & rel, ClassData * cld,
		       QGroupBox * bg, const char * idl_update_slot, 
		       const char * idl_default_slot, const char * idl_unmapped_slot);
    void cpp_update(RoleDialog & role, BrowserClass * cl, BrowserNode * rl);
    void java_update(RoleDialog & role, BrowserClass * cl, BrowserNode * rl);
    void idl_update(RoleDialog & role, BrowserClass * cl, BrowserNode * rl);
    void set_inherit_or_dependency(UmlCode type);
    
    static void post_edit_description_a(RelationDialog * d, QString s);
    static void post_edit_description_b(RelationDialog * d, QString s);
    static void post_edit_init_a(RelationDialog * d, QString s);
    static void post_edit_init_b(RelationDialog * d, QString s);
  
  public:
    RelationDialog(RelationData * r);
    virtual ~RelationDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void default_description_a();
    void default_description_b();
    void edit_description_a();
    void edit_description_b();
    void edit_init_a();
    void edit_init_b();
    void edTypeActivated(int);
    void cpp_update_a();
    void cpp_update_b();
    void cpp_default_a();
    void cpp_include_in_header();
    void cpp_unmapped_a();
    void cpp_default_b();
    void cpp_unmapped_b();
    void java_update_a();
    void java_update_b();
    void java_default_a();
    void java_unmapped_a();
    void java_default_b();
    void java_unmapped_b();
    void java_edit_annotation_a();
    void java_edit_annotation_b();
    void idl_update_a();
    void idl_update_b();
    void idl_default_a();
    void idl_unmapped_a();
    void idl_default_b();
    void idl_unmapped_b();
    void update_all_tabs();
};

#endif
