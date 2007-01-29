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

#ifndef DIAGRAM_CANVAS
#define DIAGRAM_CANVAS

#include "UmlCanvas.h"
#include "UmlEnum.h"
#include "DiagramItem.h"

#define DIAGRAMCANVAS_Z 2000

class BrowserNode;
class LabelCanvas;

class DiagramCanvas : public QCanvasRectangle, public DiagramItem {
  protected:
    BrowserNode * browser_node;
    LabelCanvas * label;
    int width_scale100;
    int height_scale100;
    int center_x_scale100;
    int center_y_scale100;
    
  protected:
    DiagramCanvas(UmlCanvas * canvas, int id);
  
  public:
    DiagramCanvas(BrowserNode * bn, UmlCanvas * canvas,
		  int x, int y, int w, int h, int id);
    virtual ~DiagramCanvas();
    
    virtual void delete_it();
    
    virtual BrowserNode * get_bn() const;
  
    virtual void upper();
    virtual void lower();
    virtual double get_z() const;
    virtual void set_z(double z);	// only called by upper() & lower()
    
    virtual void setVisible(bool yes);
    virtual void moveBy(double dx, double dy);
    virtual void setSelected(bool yes);
    virtual void select_associated();
    virtual bool isSelected() const;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual void prepare_for_move(bool on_resize);
    void force_self_rel_visible();
    
    virtual void change_scale();
    virtual QPoint center() const;
    virtual QRect rect() const;
    void recenter();
    void set_center100();
    virtual bool contains(int, int) const;
    
    void resize(aCorner c, int dx, int dy, int min_width, int min_height,
		bool odd = FALSE, bool stay_centered = FALSE);
    void resize(int w, int h);
    virtual UmlCanvas * the_canvas() const;
    void package_modified() const;
    
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    static void draw_actor(QPainter * p, QRect r);
    static void draw_entity_icon(QPainter & p, QRect & r,
				 const QColor & co, double zoom);
    static void draw_boundary_icon(QPainter & p, QRect & r,
				   const QColor & co, double zoom);
    static void draw_control_icon(QPainter & p, QRect & r,
				  const QColor & co, double zoom);
    static double compute_angle(double delta_x, double delta_y);
};

#define CLASS_ICON_SIZE 20
#define CONTROL_WIDTH CLASS_ICON_SIZE
#define CONTROL_HEIGHT (CLASS_ICON_SIZE + 5)
#define ENTITY_SIZE CLASS_ICON_SIZE
#define BOUNDARY_WIDTH (2*CLASS_ICON_SIZE)
#define BOUNDARY_HEIGHT CLASS_ICON_SIZE
#define ACTOR_SIZE 40

#endif
