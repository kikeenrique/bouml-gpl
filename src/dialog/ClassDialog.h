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

#ifndef CLASSDIALOG_H
#define  CLASSDIALOG_H

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qtabdialog.h>

#include "MyTable.h"
#include "BrowserNode.h"
#include "VisibilityGroup.h"

class QVBox;
class QLabel;
class QComboBox;
class QCheckBox;
class QGroupBox;

class LineEdit;
class MultiLineEdit;
class ClassData;
class BrowserNode;
class FormalParamsTable;
class ActualParamsTable;
class KeyValuesTable;
class BodyDialog;

class ClassDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    ClassData * cl;
    LineEdit * edname;
    QComboBox * edstereotype;
    VisibilityGroup uml_visibility;
    QLabel * basetypelbl;
    QComboBox * edbasetype;
    QCheckBox * abstract_cb;
    QGroupBox * opt_bg;
    QComboBox * artifact;
    MultiLineEdit * comment;
    FormalParamsTable * formals_table;
    QVBox * parametrized_vtab;
    BrowserNodeList nodes;
    QStringList node_names;
    BrowserNodeList artifacts;
    QList<BodyDialog> edits;
    
    ActualParamsTable * actuals_table;
    QVBox * instantiate_vtab;
    
    // C++
    QString current_cpp_stereotype;
    VisibilityGroup cpp_visibility;
    QCheckBox * cpp_external_cb;
    MultiLineEdit * edcppdecl;
    MultiLineEdit * showcppdecl;
    
    // Java
    QString current_java_stereotype;
    QCheckBox * java_external_cb;
    QCheckBox * java_final_cb;
    QCheckBox * java_public_cb;
    MultiLineEdit * edjavadecl;
    MultiLineEdit * showjavadecl;
    QPushButton * editjavaannotation;
    QString javaannotation;
    
    // Idl
    QString current_idl_stereotype;
    QCheckBox * idl_external_cb;
    MultiLineEdit * edidldecl;
    MultiLineEdit * showidldecl;
    QGroupBox * switch_bg;
    QComboBox * edswitch_type;
    QCheckBox * idl_local_cb;
    QCheckBox * idl_custom_cb;
    
    // user
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
  
  public:
    ClassDialog(ClassData * c);
    virtual ~ClassDialog();
  
    static void cpp_generate_decl(QString & s, ClassData * cl,
				  QString def, QString name,
				  QString stereotype,
				  QString basetype, QString comment,
				  ActualParamsTable * actuals,
				  FormalParamsTable * formals,
				  BrowserNodeList & nodes,
				  QStringList & node_names,
				  KeyValuesTable * kvt);
    
    static void java_generate_decl(QString & s, ClassData * cl, QString def,
				   QString annotation, QString name,
				   QString stereotype,
				   QString comment, bool is_public,
				   bool is_final, bool is_abstract,
				   ActualParamsTable * actuals,
				   FormalParamsTable * formals,
				   BrowserNodeList & nodes,
				   QStringList & node_names,
				   KeyValuesTable * kvt);
  
    static void idl_generate_decl(QString & s, ClassData * cl, QString def,
				  QString name, QString stereotype,
				  QString basetype, QString comment,
				  QString switch_type, bool is_local,
				  bool is_custom, bool is_abstract,
				  KeyValuesTable * kvt);
    
    static void cpp_generate_members_def(const BrowserNode * cl, QString & s);
    
    static void post_edit_description(ClassDialog * d, QString s);
    
    static QString cpp_stereotype(const QString & stereotype);
    static QString java_stereotype(const QString & stereotype);
    static QString idl_stereotype(const QString & stereotype);

  protected slots:
    virtual void polish();
    void accept();
    void default_description();
    void edit_description();
    void edStereotypeActivated(const QString &);
    void update_all_tabs();
    void cpp_update_decl();
    void cpp_default_decl();
    void cpp_unmapped_decl();
    void java_update_decl();
    void java_default_decl();
    void java_unmapped_decl();
    void java_edit_annotation();
    void idl_update_decl();
    void idl_default_decl();
    void idl_unmapped_decl();
};

class FormalParamsTable : public MyTable {
  Q_OBJECT

  protected:
    const QStringList & types;

    static QString type_copy;
    static QString name_copy;		// copy/cut/paste
    static QString default_value_copy;
    static QString extends_copy;
  
  public:
    FormalParamsTable(ClassData * a, QWidget * parent,
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
    void update(ClassData * c, BrowserNodeList & nodes);
    void generate_cpp(QString & s, BrowserNodeList & nodes,
		      QStringList & node_names, const QString & cl_name);
    void generate_java(QString & s, BrowserNodeList & nodes,
		       QStringList & node_names);
  
  protected slots:
    void button_pressed(int row, int col, int button, const QPoint & mousePos);
    void value_changed(int row, int col);
};

class ActualParamsTable : public MyTable {
  protected:
    const QStringList & types;
  
  public:
    ActualParamsTable(ClassData * a, QWidget * parent,
		      const QStringList & list, bool visit);
    void generate(QString & s, ClassData * cl, BrowserNode * parent, bool cpp,
		  BrowserNodeList & nodes, QStringList & node_names);
  
  protected:
    //virtual void activateNextCell();
    
  public:
    void update(ClassData * c, BrowserNodeList & nodes);
};

#endif
