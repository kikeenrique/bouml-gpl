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

#ifndef STATEDIALOG_H
#define STATEDIALOG_H

#include <qwidgetlist.h>
#include <qstringlist.h>
#include <qtabdialog.h>

#include "BrowserNode.h"

class QPushButton;
class QComboBox;

class LineEdit;
class MultiLineEdit;
class StateData;
class BrowserNode;
class KeyValuesTable;
struct StateBehavior;
class BodyDialog;

struct StDialog {
  MultiLineEdit * edentry;
  MultiLineEdit * edexit;
  MultiLineEdit * edactivity;
  
  void accept(StateBehavior &);
};

class StateDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    QWidget * umltab;
    bool visit;
    StateData * state;
    LineEdit * edname;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    QList<BodyDialog> edits;
    
    QWidget * ocltab;
    QWidget * cpptab;
    QWidget * javatab;
    StDialog uml;
    StDialog cpp;
    StDialog java;
        
    // User
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
  
    static void post_edit_description(StateDialog * d, QString s);
    static void post_edit_uml_entry(StateDialog * d, QString s);
    static void post_edit_uml_exit(StateDialog * d, QString s);
    static void post_edit_uml_activity(StateDialog * d, QString s);
    static void post_edit_cpp_entry(StateDialog * d, QString s);
    static void post_edit_cpp_exit(StateDialog * d, QString s);
    static void post_edit_cpp_activity(StateDialog * d, QString s);
    static void post_edit_java_entry(StateDialog * d, QString s);
    static void post_edit_java_exit(StateDialog * d, QString s);
    static void post_edit_java_activity(StateDialog * d, QString s);
  
  public:
    StateDialog(StateData * r);
    virtual ~StateDialog();
  
  protected:
    void init_tab(QWidget *&, StDialog &, StateBehavior & td, const char * lbl,
		  const char * sl_enbeh, const char * sl_exbeh, const char * sl_beh);
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void edit_description();
    void edit_uml_entry();
    void edit_uml_exit();
    void edit_uml_activity();
    void edit_cpp_entry();
    void edit_cpp_exit();
    void edit_cpp_activity();
    void edit_java_entry();
    void edit_java_exit();
    void edit_java_activity();

    void change_tabs(QWidget *);
};

#endif
