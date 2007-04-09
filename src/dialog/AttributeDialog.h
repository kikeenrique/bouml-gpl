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

#ifndef ATTRIBUTEDIALOG_H
#define  ATTRIBUTEDIALOG_H

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qstringlist.h>
#include <qtabdialog.h>

#include "BrowserNode.h"
#include "VisibilityGroup.h"

class QComboBox;
class QCheckBox;
class QRadioButton;

class BrowserAttribute;
class AttributeData;
class BrowserNode;
class LineEdit;
class MultiLineEdit;
class KeyValuesTable;
class BodyDialog;

class AttributeDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    bool visit;
    AttributeData * att;
    bool in_enum;
    QStringList list;
    BrowserNodeList nodes;
    QList<BodyDialog> edits;
    int offset;
    
    // uml tab
    QWidget * umltab;
    LineEdit * edname;
    LineEdit * edinit;
    QComboBox * edtype;
    QComboBox * edstereotype;
    int attribute_st_rank;
    int empty_st_rank;
    VisibilityGroup uml_visibility;
    QCheckBox * classattribute_cb;
    QCheckBox * constattribute_cb;
    MultiLineEdit * comment;
    
    // C++ tab
    QWidget * cpptab;
    bool cpp_in_enum;
    bool cpp_in_typedef;
    VisibilityGroup cpp_visibility;
    QCheckBox * mutable_cb;
    QCheckBox * volatile_cb;
    MultiLineEdit * edcppdecl;
    MultiLineEdit * showcppdecl;
    
    // java tab
    QWidget * javatab;
    bool java_in_enum;
    bool java_in_enum_pattern;
    bool java_in_typedef;
    QCheckBox * transient_cb;
    MultiLineEdit * edjavadecl;
    MultiLineEdit * showjavadecl;
    QPushButton * editjavaannotation;
    QString javaannotation;
    
    // IDL tab
    QWidget * idltab;
    bool idl_in_enum;
    bool idl_in_struct;
    bool idl_in_union;
    bool idl_in_typedef;
    QComboBox * edcase;
    BrowserNodeList enums;
    QStringList enum_names;
    MultiLineEdit * edidldecl;
    MultiLineEdit * showidldecl;
    
    // User tab
    KeyValuesTable * kvtable;

    static QSize previous_size;
  
    static void post_edit_init(AttributeDialog *, QString);
    static void post_edit_description(AttributeDialog *, QString);
    
  public:
    AttributeDialog(AttributeData * a);
    virtual ~AttributeDialog();
  
    static QString cpp_decl(const BrowserAttribute * at);
    static QString java_decl(const BrowserAttribute * at);
    static QString idl_decl(const BrowserAttribute * at);
  
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void edit_init();
    void default_description();
    void edit_description();
    void menu_type();
    void cpp_default();
    void cpp_unmapped();
    void cpp_update();
    void java_default();
    void java_default_item();
    void java_unmapped();
    void java_update();
    void java_edit_annotation();
    void idl_default();
    void idl_default_state();
    void idl_unmapped();
    void idl_update();
    void update_all_tabs(QWidget *);
};

#endif
