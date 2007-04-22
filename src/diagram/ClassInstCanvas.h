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

#ifndef CLASSINSTCANVAS_H
#define CLASSINSTCANVAS_H

#define CLASSINST_CANVAS_MIN_SIZE 50

#include <qstring.h>
#include <qrect.h>

#include "UmlEnum.h"
#include "Instance.h"

class BrowserClass;
class UmlCanvas;
class QPainter;

class ClassInstCanvas : public Instance {
  friend class ClassInstDialog;
  
  protected:
    QString iname;
    BrowserClass * cl;
    UmlColor itscolor;
    UmlColor used_color;
    Uml3States write_horizontally;
    bool horiz;
  
  public:
    ClassInstCanvas(BrowserClass * t);
    virtual ~ClassInstCanvas();
    
    QString full_name() const;
    virtual const QString & get_name() const;
    virtual void set_name(const QString & s);
    
    virtual BrowserNode * get_type() const;
    virtual BrowserNodeList & get_types(BrowserNodeList &) const;
    virtual void set_type(BrowserNode * t);
    virtual BrowserNode * new_type();
    virtual bool new_type_available();
    virtual void set_type(BrowserClass * t) = 0;
    virtual BrowserNode * the_diagram() const = 0;
    void compute_size(int & w, int & h, UmlCanvas *, bool as_class = FALSE);
    void draw(QPainter & p, UmlCanvas * canvas, QRect r, bool as_class = FALSE);
};

#endif
