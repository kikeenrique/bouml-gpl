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

#ifndef ACTIVITYDIALOG_H
#define ACTIVITYDIALOG_H

#include <qwidgetlist.h>
#include <qstringlist.h>
#include <qtabdialog.h>

#include "UmlEnum.h"

class QPushButton;
class QComboBox;
class QCheckBox;

class LineEdit;
class MultiLineEdit;
class ActivityData;
class KeyValuesTable;
struct InfoData;
class BodyDialog;

struct CondDialog {
  MultiLineEdit * edpre;
  MultiLineEdit * edpost;
  
  void accept(InfoData &);
};

class ActivityDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    bool visit;
    ActivityData * activity;
    LineEdit * edname;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    QList<BodyDialog> edits;
    QCheckBox * readonly_cb;
    QCheckBox * singlexec_cb;
    CondDialog uml;
    CondDialog cpp;
    CondDialog java;
        
    // User
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
  
    static void post_edit_description(ActivityDialog * d, QString s);
  
  public:
    ActivityDialog(ActivityData * r);
    virtual ~ActivityDialog();
  
  protected:
    void init_tab(CondDialog &, InfoData & cond, const char * lbl);
  
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void edit_description();

    void change_tabs(QWidget *);
};

#endif
