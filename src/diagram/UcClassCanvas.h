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

#ifndef UCCLASSCANVAS_H
#define UCCLASSCANVAS_H

#include "DiagramCanvas.h"

#define ACTOR_CANVAS_SIZE 40

class UcClassCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  public:
    UcClassCanvas(BrowserNode * bn, UmlCanvas * canvas, int x, int y, int id);
    virtual ~UcClassCanvas();
    
    virtual void delete_it();

    virtual void draw(QPainter & p);
    
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
    virtual bool has_relation(BasicData *) const; 
    virtual bool move_with_its_package() const;
    virtual void apply_shortcut(QString s);
    
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static UcClassCanvas * read(char * &, UmlCanvas * canvas, char *);
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the class is deleted
};

#endif
