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

#ifndef DEFAULTSDIALOG_H
#define DEFAULTSDIALOG_H

#include <qtabdialog.h>
#include <qarray.h> 
#include <qvector.h> 

#include "StateSpec.h"

class ComboStates;
class ComboColor;

class ColorSpec {
  public:
    const char * name;
    UmlColor * color;
    
    void set(const char * n, UmlColor * c) {
      name = n;
      color = c;
    };
};

class SettingsDialog : public QTabDialog {
  Q_OBJECT
 
  protected:
    QArray<StateSpec> * states;
    QArray<ColorSpec> * colors;
    QVector<ComboStates> * cbstates;
    QVector<ComboColor> * cbcolors;
    bool several;
 
    static QSize previous_size;
  
  public:
    SettingsDialog(QArray<StateSpec> * st, QArray<ColorSpec> * co,
		   bool nodefault, bool own, bool unchanged = false);
    virtual ~SettingsDialog();
    
  protected slots:
    virtual void polish();
    virtual void accept();
};

#endif
