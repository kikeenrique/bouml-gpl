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

#ifndef PACKAGECANVAS_H
#define PACKAGECANVAS_H

#include "DiagramCanvas.h"

#define PACKAGE_CANVAS_MIN_WIDTH 64
#define PACKAGE_CANVAS_MIN_HEIGHT 32

class PackageCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  protected:
    bool in_tab;
    Uml3States name_in_tab;
    ShowContextMode show_context_mode;
    UmlColor itscolor;
    UmlColor used_color;
    QString full_name;
  
  public:
    PackageCanvas(BrowserNode * bn, UmlCanvas * canvas, int x, int y, int id);
    virtual ~PackageCanvas();
    
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
    virtual aCorner on_resize_point(const QPoint & p);
    virtual void resize(aCorner c, int dx, int dy);
    virtual void prepare_for_move(bool on_resize);
    virtual bool move_with_its_package() const;
    virtual void change_scale();
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();

    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static PackageCanvas * read(char * &, UmlCanvas * canvas, char *);
  
    void check_size();
    int min_width();
    int min_height();
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the package is deleted
};

#endif
