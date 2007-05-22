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

#ifndef INTERRUPTIBLEACTIVITYREGIONCANVAS_H
#define INTERRUPTIBLEACTIVITYREGIONCANVAS_H

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qobject.h>
#include <qvaluelist.h>

#include "ActivityContainerCanvas.h"

#define INTERRUPTIBLE_ACTIVITY_REGION_CANVAS_MIN_SIZE 40

class InterruptibleActivityRegionCanvas : public QObject, public ActivityContainerCanvas {
  Q_OBJECT
    
  protected:
    UmlColor itscolor;
    UmlColor used_color;
    
  protected:
    InterruptibleActivityRegionCanvas(UmlCanvas * canvas, int id);
  
  public:
    InterruptibleActivityRegionCanvas(BrowserNode * bn, UmlCanvas * canvas, int x, int y);
    virtual ~InterruptibleActivityRegionCanvas();
    
    void force_sub_inside();
    void force_inside();
    
    virtual void delete_it();
    
    void update();
    
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
    virtual aCorner on_resize_point(const QPoint & p);
    virtual void resize(aCorner c, int dx, int dy);
    virtual bool move_with_its_package() const;
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(QList<DiagramItem> &);
    
    virtual void apply_shortcut(QString s);
    void edit_drawing_settings();
  
    virtual void save(QTextStream  & st, bool ref, QString & warning) const;
    static InterruptibleActivityRegionCanvas * read(char * &, UmlCanvas *, char *);
    
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();
};

#endif
