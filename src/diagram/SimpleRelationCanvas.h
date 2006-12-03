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

#ifndef SIMPLERELATIONCANVAS_H
#define SIMPLERELATIONCANVAS_H

#include "ArrowCanvas.h"

class BrowserNode;
class SimpleRelationData;
class DiagramItem;

class SimpleRelationCanvas : public ArrowCanvas {
  Q_OBJECT
    
  protected:
    BrowserNode * br_begin;
    SimpleRelationData * data;
    
  public:
    SimpleRelationCanvas(UmlCanvas * canvas, DiagramItem * b,
			 DiagramItem * e, BrowserNode * bb, UmlCode t,
			 int id, SimpleRelationData * d = 0);
    virtual ~SimpleRelationCanvas();
    
    virtual void delete_it();
    virtual void delete_available(bool & in_model, bool & out_model) const;
    virtual void remove(bool from_model);
    
    virtual ArrowPointCanvas * brk(const QPoint &);
    virtual ArrowCanvas * join(ArrowCanvas * other, ArrowPointCanvas * ap);
    
    virtual void setSelected(bool yes);
    
    void stereotype_default_position() const;
    
    virtual void history_load(QBuffer &);
    virtual void history_hide();

    virtual void apply_shortcut(QString s);
    
    virtual void save(QTextStream & st, bool ref, QString & warning) const;
    static SimpleRelationCanvas * read(char * & st, UmlCanvas * canvas, char *);
    
    virtual BasicData * get_data() const;
    
    virtual void open();
    virtual void menu(const QPoint&);
    
    static void drop(BrowserNode *, UmlCanvas *);
    
  protected:
    BrowserNode * update_begin(DiagramItem * cnend);
    void update(bool updatepos);
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the relation is deleted
};

#endif
