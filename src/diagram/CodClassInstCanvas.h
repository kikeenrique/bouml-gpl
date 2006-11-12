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

#ifndef CODCLASSINSTCANVAS_H
#define CODCLASSINSTCANVAS_H

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qobject.h>

#include "CodObjCanvas.h"
#include "ClassInstCanvas.h"

class QStringList;

class CodClassInstCanvas : public QObject, public CodObjCanvas, public ClassInstCanvas {
  Q_OBJECT
    
  public:
    CodClassInstCanvas(BrowserClass * t, UmlCanvas * canvas,
		       int x, int y, int id);
    virtual ~CodClassInstCanvas();
    
    virtual void delete_it();
    
    virtual void draw(QPainter & p);
    
    void compute_size();
    virtual UmlCode type() const;
    virtual BrowserClass * get_type();
    virtual void set_type(BrowserClass * t);
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString get_full_name() const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();

    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static CodClassInstCanvas * read(char * &, UmlCanvas * canvas, char *);
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the class is deleted
};

#endif
