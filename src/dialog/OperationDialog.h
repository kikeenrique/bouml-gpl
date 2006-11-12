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

#ifndef OPERATIONDIALOG_H
#define  OPERATIONDIALOG_H

#include <qtabdialog.h>

#include "MyTable.h"
#include "BrowserNode.h"
#include "VisibilityGroup.h"

class QComboBox;
class QCheckBox;
class QRadioButton;
class QPushButton;

class LineEdit;
class MultiLineEdit;
class BrowserOperation;
class OperationData;
class ClassData;
class AttributeData;
class RelationData;
class BrowserNode;
class ParamsTable;
class CppParamsTable;
class ExceptionsTable;
class KeyValuesTable;
class BodyDialog;

class OperationDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    bool visit;
    OperationData * oper;
    ClassData * cl;
    QStringList list;
    BrowserNodeList nodes;
    QList<BodyDialog> edits;
    AttributeData * get_of_attr;
    AttributeData * set_of_attr;
    RelationData * get_of_rel;
    RelationData * set_of_rel;
    bool is_rel_a;
    
    // uml tab
    LineEdit * edname;
    QComboBox * edreturn_type;
    int edreturn_type_offset;
    QComboBox * edstereotype;
    VisibilityGroup uml_visibility;
    QCheckBox * classoperation_cb;
    QCheckBox * abstract_cb;
    ParamsTable * table;
    ExceptionsTable * etable;
    MultiLineEdit * comment;
    
    // c++ tab
    bool cpp_undef;
    VisibilityGroup cpp_visibility;
    QCheckBox * friend_cb;
    QCheckBox * const_cb;
    QCheckBox * volatile_cb;
    QCheckBox * virtual_cb;
    QCheckBox * inline_cb;
    LineEdit * edcppnamespec;	// get/set
    MultiLineEdit * edcppdecl;
    CppParamsTable * paramcppdecltable;
    MultiLineEdit * showcppdecl;
    MultiLineEdit * edcppdef;
    CppParamsTable * paramcppdeftable;
    MultiLineEdit * showcppdef;
    QPushButton * editcppbody;
    QString cppbody;
    QString oldcppbody;
    QString templates;
    QString cl_names;
    QString templates_tmplop;
    QString cl_names_tmplop;
        
    // java tab
    bool java_undef;
    QCheckBox * final_cb;
    QCheckBox * synchronized_cb;
    LineEdit * edjavanamespec;	// get/set
    MultiLineEdit * edjavadef;
    MultiLineEdit * showjavadef;
    QPushButton * editjavabody;
    QPushButton * editjavaannotation;
    QString javaannotation;
    QString javabody;
    QString oldjavabody;
        
    // idl tab
    bool idl_undef;
    QCheckBox * oneway_cb;
    LineEdit * edidlnamespec;	// get/set
    MultiLineEdit * edidldecl;
    MultiLineEdit * showidldecl;
    
    // user
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
  
  protected:
    QString compute_name(LineEdit * spec);
    void manage_cpp_type(unsigned rank, QString & s);
    void manage_cpp_exceptions(QString & s);
    void manage_var(unsigned rank, QString & s);
    void manage_java_type(unsigned rank, QString & s);
    void manage_java_exceptions(QString & s);
    void manage_idl_type(unsigned rank, QString & s);
    void manage_dir(unsigned rank, QString & s);
    void manage_idl_exceptions(QString & s);
  
    static void post_edit_description(OperationDialog * d, QString s);
    static void post_cpp_edit_body(OperationDialog * d, QString s);
    static void post_java_edit_body(OperationDialog * d, QString s);
  
  public:
    OperationDialog(OperationData * a, DrawingLanguage l);
    virtual ~OperationDialog();
  
    static QString cpp_decl(const BrowserOperation * op);
    static QString java_decl(const BrowserOperation * op);
    static QString idl_decl(const BrowserOperation * op);
  
  protected slots:
    virtual void polish();
    virtual void accept();
    void default_description();
    void edit_description();
    void classoper_toggled(bool on);
    void abstract_toggled(bool on);
    void cpp_default_decl();
    void cpp_decl_from_def();
    void cpp_unmapped_decl();
    void cpp_update_decl();
    void cpp_edit_param_decl();
    void cpp_def_from_decl();
    void cpp_default_def();
    void cpp_unmapped_def();
    void cpp_update_def();
    void cpp_edit_param_def();
    void cpp_edit_body();
    void const_volatile_toggled(bool on);
    void friend_toggled(bool on);
    void virtual_toggled(bool on);
    void inline_toggled(bool on);
    void java_default_def();
    void java_unmapped_def();
    void java_update_def();
    void java_edit_body();
    void java_edit_annotation();
    void finalsynchronized_toggled(bool on);
    void idl_default_def();
    void idl_unmapped_def();
    void idl_update_decl();
    void oneway_toggled(bool on);
    void update_all_tabs();
    void menu_returntype();
};

class ParamsTable : public MyTable {
  Q_OBJECT

  protected:
    const QStringList & types;
    QStringList alltypes;
  
    static QString dir_copy;		// copy/cut/paste
    static QString name_copy;
    static QString type_copy;
    static QString default_value_copy;
  
  public:
    ParamsTable(OperationData * a, QWidget * parent,
		const QStringList & list, bool visit);
  
  protected:
    virtual void activateNextCell();
  
    void insert_row_before(int row);
    void insert_row_after(int row);
    void delete_row(int row);
    void copy_row(int row);
    void cut_row(int row);
    void paste_row(int row);
    void move_row(int from, int to);
    
  public:
    void update(OperationData * oper, BrowserNodeList & nodes);
    unsigned nparams() const;
    QString name(unsigned rank) const;
    QString type(unsigned rank) const;
    UmlParamDirection dir(unsigned rank) const;
    QString value(unsigned rank) const;
  
  protected slots:
    void button_pressed(int row, int col, int button, const QPoint & mousePos);
    void value_changed(int row, int col);
};

class ExceptionsTable : public MyTable {
  Q_OBJECT

  protected:
    const QStringList & types;
  
    static QString type_copy;		// copy/cut/paste
  
  public:
    ExceptionsTable(OperationData * a, QWidget * parent,
		    const QStringList & list, bool visit);
  
  protected:
    virtual void activateNextCell();
  
    void insert_row_before(int row);
    void insert_row_after(int row);
    void delete_row(int row);
    void copy_row(int row);
    void cut_row(int row);
    void paste_row(int row);
    void move_row(int from, int to);
    
  public:
    void update(OperationData * oper, BrowserNodeList & nodes);
    unsigned nexceptions() const;
    QString type(unsigned rank) const;
  
  protected slots:
    void button_pressed(int row, int col, int button, const QPoint & mousePos);
    void value_changed(int row, int col);
};

class CppParamsTable : public MyTable {
  Q_OBJECT

  protected:
    ParamsTable * params;
    MultiLineEdit * edform;
  
    static QString copied[7];		// copy/cut/paste
  
  public:
    CppParamsTable(ParamsTable * p, MultiLineEdit * f, QWidget * parent);
    void update_edform();
  
  protected:
    bool extract(int tblindex, int & strindex, QString s);

    void insert_row_before(int row);
    void insert_row_after(int row);
    void delete_row(int row);
    void copy_row(int row);
    void cut_row(int row);
    void paste_row(int row);
    void move_row(int from, int to);
    void init_row(int row);
    void update_name(int row);
    void update_names();
    
    virtual void setItem(int row, int col, QTableItem * item);
    
  protected slots:
    void button_pressed(int row, int col, int button, const QPoint & mousePos);
    virtual void setCurrentCell(int row, int col);
};

class CppParamsDialog : public QDialog {
  Q_OBJECT
    
  protected:
    CppParamsTable * tbl;
    
    static QSize previous_size;
  
  public:
    CppParamsDialog(ParamsTable * params, MultiLineEdit * form);
    virtual ~CppParamsDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
};

#endif
