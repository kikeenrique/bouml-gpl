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

#ifndef OBJECTLINKCANVAS_H
#define OBJECTLINKCANVAS_H

#include "ArrowCanvas.h"

class BrowserClass;
class RelationData;
class DiagramItem;

class ObjectLinkCanvas : public ArrowCanvas {
  Q_OBJECT
    
  protected:
    RelationData * data;
    LabelCanvas * role_a;
    LabelCanvas * role_b;
    
  public:
    ObjectLinkCanvas(UmlCanvas * canvas, DiagramItem * b, DiagramItem * e,
		     int id, RelationData * d = 0);
    virtual ~ObjectLinkCanvas();
    
    virtual void delete_it();
    
    virtual void history_save(QBuffer & b) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
   
    virtual void setVisible( bool yes);
    virtual void moveBy(double dx, double dy);
    virtual void set_z(double z);
    virtual void select_associated();
    
    virtual void update_pos();
    virtual ArrowPointCanvas * brk(const QPoint &);
    virtual ArrowCanvas * join(ArrowCanvas * other, ArrowPointCanvas * ap);
    
    virtual void save(QTextStream & st, bool ref, QString & warning) const;
    static ObjectLinkCanvas * read(char * & st, UmlCanvas * canvas, char *);
    
    virtual void open();
    virtual void menu(const QPoint&);
    
    void label_default_position() const;
    void role_a_default_position() const;
    void role_b_default_position() const;
    
    bool reflexive() const;
    void check();
    
  protected:
    void internal_set_relation(RelationData * d);
    void set_relation(RelationData * d);
    void update(bool updatepos);
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the relation is deleted
};

#endif
