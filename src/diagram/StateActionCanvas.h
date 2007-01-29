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

#ifndef STATEACTIONCANVAS_H
#define STATEACTIONCANVAS_H

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qobject.h>

#include "DiagramCanvas.h"
#include "Settings.h"

#define STATE_ACTION_CANVAS_MIN_SIZE 30

class BrowserRegion;

class StateActionCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  protected:
    DrawingLanguage language;
    UmlColor itscolor;
    UmlColor used_color;
    QString s;
    int min_width;
    int min_height;
    
  protected:
    StateActionCanvas(UmlCanvas * canvas, int id);
  
  public:
    StateActionCanvas(BrowserNode * bn, UmlCanvas * canvas, int x, int y);
    virtual ~StateActionCanvas();
    
    virtual void delete_it();
    
    void compute_size();
    
    virtual void draw(QPainter & p);
    virtual void change_scale();
    
    virtual UmlCode type() const;
    virtual void delete_available(bool & in_model, bool & out_model) const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual void remove(bool from_model);
    virtual void open();
    virtual void menu(const QPoint&);
    virtual const char * may_start(UmlCode &) const;
    virtual const char * may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual aCorner on_resize_point(const QPoint & p);
    virtual void resize(aCorner c, int dx, int dy);
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(QList<DiagramItem> &);
    
    virtual void apply_shortcut(QString s);
    void edit_drawing_settings();
  
    virtual void save(QTextStream  & st, bool ref, QString & warning) const;
    static StateActionCanvas * read(char * &, UmlCanvas *, char *);
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();
};

#endif
