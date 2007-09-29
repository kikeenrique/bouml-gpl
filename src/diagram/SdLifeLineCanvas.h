// *************************************************************************
//
// Copyleft 2004-2007 Bruno PAGES  .
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

#ifndef SDLIFELINECANVAS_H
#define SDLIFELINECANVAS_H

#include "DiagramCanvas.h"

#define LIFE_LINE_TOPOFFSET 41

class SdDead;
class SdObjCanvas;
class SdDurationCanvas;

class SdLifeLineCanvas : public DiagramCanvas {
  protected:
    SdObjCanvas * obj;
    int end;
    QList<SdDurationCanvas> durations;
  
  public:
    SdLifeLineCanvas(UmlCanvas * canvas, SdObjCanvas * o);
    virtual ~SdLifeLineCanvas();
    
    virtual void delete_it();

    SdObjCanvas * get_obj() const { return obj; };
    void add(SdDurationCanvas *);
    void remove(SdDurationCanvas *);
    double instance_max_y() const;
    void update_pos();
    void update_instance_dead();
    
    virtual bool is_decenter(const QPoint &, bool &) const;
    
    virtual void drawShape(QPainter & p);
    virtual void moveBy(double dx, double dy);
    
    virtual UmlCode type() const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual void change_scale();
    virtual const char * may_start(UmlCode &) const;
    virtual const char * may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual LineDirection allowed_direction(UmlCode);
    virtual bool copyable() const;
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
};

#endif
