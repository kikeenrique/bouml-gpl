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

#ifndef ARROWCANVAS_H
#define ARROWCANVAS_H

#include "qcanvas.h"

#include "DiagramItem.h"

#define ARROW_LENGTH 10

//#define ARROW_Z 1000
#define OLD_ARROW_Z 0

#define isa_arrow(x) ((x)->rtti() == RTTI_ARROW)

class DiagramCanvas;
class ArrowPointCanvas;
class LabelCanvas;
class UmlCanvas;
class QPopupMenu;

class ArrowCanvas : public QObject, public QCanvasPolygon, public DiagramItem {
  Q_OBJECT
    
  protected:
    DiagramItem * begin;
    DiagramItem * end;
    UmlCode itstype;
    // HV : start and point aligned horiz, true end and point aligned vert
    // HVr : true end and point aligned horiz, start and point aligned vert
    // HVH = start and point1 aligned horiz,
    //	     point1 and point2 aligned vert
    //	     point2 and true end aligned horiz
    // etc ...
    // X and Xr seem to be X for the user when he choose the geometry
    LineGeometry geometry;
    bool fixed_geometry;
    bool auto_pos;
    LabelCanvas * label;
    LabelCanvas * stereotype;
    QPoint beginp;
    QPoint endp;
    QPointArray boundings;
    QPoint arrow[3];	// les 2 extremites et le point milieu pour inherit
    QPointArray poly;   // aggregations
    
    // to remove redondant relation made by release 2.22
    static QList<ArrowCanvas> RelsToCheck;
    
  public:
    ArrowCanvas(UmlCanvas * canvas, DiagramItem * b, DiagramItem * e,
		UmlCode t, int id, bool own_brk);
    virtual ~ArrowCanvas();
    
    virtual void delete_it();
    
    virtual void setVisible( bool yes);
    virtual void moveBy(double dx, double dy);
    virtual void select_associated();
    virtual bool isSelected() const;
    
    virtual void update_pos();
    void move_outside(QRect r);
    virtual void default_stereotype_position() const;
    virtual void default_label_position() const;
    bool cut_on_move(ArrowPointCanvas *&) const;
    void cut_self();
    virtual ArrowPointCanvas * brk(const QPoint &);
    bool may_join() const;
    virtual ArrowCanvas * join(ArrowCanvas * other, ArrowPointCanvas * ap);
    DiagramItem * get_start() const;  
    DiagramItem * get_end() const;
    void extremities(DiagramItem *& b, DiagramItem *& e) const;
    virtual BasicData * get_data() const;
    
    virtual UmlCode type() const;
    virtual void delete_available(bool & in_model, bool & out_model) const;
    virtual int rtti() const;
    virtual QPoint center() const;
    virtual QRect rect() const;
    virtual bool contains(int, int) const;
    QPoint get_point(int i) { return arrow[i]; }
    virtual void change_scale();
    virtual void open();
    bool edit(const QStringList &defaults, ArrowCanvas * plabel,
	      ArrowCanvas * pstereotype);
    virtual void menu(const QPoint&);
    virtual const char * may_start(UmlCode &) const;
    virtual const char * may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual double get_z() const;
    virtual void set_z(double z);	// only called by upper() & lower()
    virtual bool copyable() const;
    virtual UmlCanvas * the_canvas() const;
    ArrowCanvas * next_geometry();
    void update_geometry();
    
    void package_modified() const;
    
    virtual void save(QTextStream  & st, bool ref, QString & warning) const;
    const ArrowCanvas * save_lines(QTextStream  & st, bool with_label, bool with_stereotype, QString & warning) const;
    static ArrowCanvas * read(char * & st, UmlCanvas * canvas, char * k);
    static ArrowCanvas * read_list(char * & st, UmlCanvas * canvas,
				   UmlCode t, LineGeometry geo,
				   bool fixed, int id,
				   ArrowCanvas * (*pf)(UmlCanvas * canvas, DiagramItem * b,
						       DiagramItem * e, UmlCode t, int id));
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();

    static void remove_redondant_rels();
    
  protected:
    void search_supports(ArrowCanvas *& plabel, 
			 ArrowCanvas *& pstereotype) const;
    void propag_geometry(LineGeometry geo, bool fixed);
    ArrowCanvas * set_geometry(LineGeometry geo, bool fixed);
    void init_geometry_menu(QPopupMenu & m, int first);
    
    virtual void drawShape(QPainter & p);
    
  protected slots:
    void modified();
    void drawing_settings_modified();
};

#endif
