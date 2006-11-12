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

#ifndef SDDURATIONCANVAS_H
#define SDDURATIONCANVAS_H

#include "DiagramCanvas.h"

#define DURATION_WIDTH 11

class SdObjCanvas;
class SdLifeLineCanvas;
class SdMsgBaseCanvas;

class SdDurationCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  protected:
    SdLifeLineCanvas * line;
    QList<SdMsgBaseCanvas> msgs;
    UmlColor itscolor;
  
    SdDurationCanvas(UmlCanvas * canvas, SdLifeLineCanvas * ll,
		     int x, int y, int w, int h, int id);
  public:
    SdDurationCanvas(UmlCanvas * canvas, SdLifeLineCanvas * ll,
		     int v, bool isdest);
    virtual ~SdDurationCanvas();
    
    virtual void delete_it();

    double min_y() const;
    void update_hpos();
    void update_v_to_contain(const QRect re);
    void add(SdMsgBaseCanvas *);
    void remove(SdMsgBaseCanvas *);
    SdLifeLineCanvas * get_line() { return line; };
    void cut(const QPoint & p);
    void merge(QList<SdDurationCanvas> &);
    
    virtual void draw(QPainter & p);
    virtual void moveBy(double dx, double dy);
    
    virtual UmlCode type() const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual const char * may_start(UmlCode &) const;
    virtual const char * may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual aCorner on_resize_point(const QPoint &);
    virtual void resize(aCorner c, int dx, int dy);
    virtual void change_scale();
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual void delete_available(bool & in_model, bool & out_model) const;
    virtual LineDirection allowed_direction(UmlCode);
    virtual void select_associated();
    virtual bool copyable() const;
    virtual void history_save(QBuffer & b) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
  
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static SdDurationCanvas * read(char * & st, UmlCanvas * canvas, char *);
    static SdDurationCanvas * read(char * & st, UmlCanvas * canvas, bool ref);
    
  private slots:
    void modified();
};

#endif
