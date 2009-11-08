// *************************************************************************
//
// Copyright 2004-2009 Bruno PAGES  .
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
    QString name;
    UmlColor * color;
    
    void set(QString n, UmlColor * c) { name = n; color = c; }
};

class ColorSpecVector {
  public:
    ColorSpecVector(int s) : _size(s) { _v = new ColorSpec[s]; };
    ~ColorSpecVector() { if (_v) delete [] _v; }
    int size() const { return _size; }
    void resize(int s);
    ColorSpec & operator[] (int i) const { return _v[i]; }
    ColorSpec & at (int i) const { return _v[i]; }
    
  private:
    int _size;
    ColorSpec * _v;
};

class SettingsDialog : public QTabDialog {
  Q_OBJECT
 
  protected:
    StateSpecVector * states;
  ColorSpecVector * colors;
    QVector<ComboStates> * cbstates;
    QVector<ComboColor> * cbcolors;
    bool several;
 
    static QSize previous_size;
  
  public:
    SettingsDialog(StateSpecVector * st, ColorSpecVector * co,
		   bool nodefault, bool unchanged = FALSE,
		   QString title = QString());
    virtual ~SettingsDialog();
    
  protected slots:
    virtual void polish();
    virtual void accept();
};

#endif
