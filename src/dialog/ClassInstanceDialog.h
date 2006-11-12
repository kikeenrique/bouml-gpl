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

#ifndef CLASSINSTANCEDIALOG_H
#define CLASSINSTANCEDIALOG_H

#include <qdialog.h>
#include <qstringlist.h>

#include "BrowserNode.h"

class QComboBox;
class LineEdit;
class MyTable;
class OdClassInstCanvas;

class ClassInstanceDialog : public QDialog {
  Q_OBJECT
    
  protected:
    void init_table();
  
    OdClassInstCanvas * inst;
    QStringList list;
    BrowserNodeList nodes;
    LineEdit * edname;
    QComboBox * edtype;
    BrowserNodeList attributes;
    MyTable * tbl;
    
    static QSize previous_size;
  
  public:
    ClassInstanceDialog(OdClassInstCanvas * i);
    virtual ~ClassInstanceDialog();
  
  protected slots:
    virtual void polish();
    void type_changed(int);
    void accept();
    void menu_class();
};

#endif
