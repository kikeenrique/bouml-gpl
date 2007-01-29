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

#ifndef TEMPLATECANVAS_H
#define TEMPLATECANVAS_H

#include "DiagramCanvas.h"

class CdClassCanvas;

#define TEMPLATE_CANVAS_MIN_SIZE 1

class TemplateCanvas : public DiagramCanvas {
  protected:
    CdClassCanvas * cl;
    QString text;
  
  public:
    TemplateCanvas(CdClassCanvas * c);
    virtual ~TemplateCanvas();

    virtual void draw(QPainter & p);
    
    virtual UmlCode type() const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual const char * may_start(UmlCode &) const;
    virtual const char * may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void moveBy(double dx, double dy);
    virtual void change_scale();
    virtual void upper();
    virtual void lower();
    virtual void set_z(double z);	// only called by upper() & lower()
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    
    void update();
};

#endif
