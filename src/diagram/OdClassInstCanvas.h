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

#ifndef ODCLASSINSTCANVAS_H
#define ODCLASSINSTCANVAS_H

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qobject.h>

#include "DiagramCanvas.h"
#include "ClassInstCanvas.h"

class BasicData;
class AttributeData;

class OdClassInstCanvas : public QObject, public DiagramCanvas,
  			  public ClassInstCanvas {
  Q_OBJECT
    
    friend class ClassInstanceDialog;
  
  public:
    OdClassInstCanvas(BrowserClass * t, UmlCanvas * canvas,
		       int x, int y, int id);
    virtual ~OdClassInstCanvas();
    
    virtual void delete_it();
    
    virtual void draw(QPainter & p);
    virtual void change_scale();
    
    void compute_size();

    virtual UmlCode type() const;
    virtual BrowserClass * get_type();
    virtual void set_type(BrowserClass * t);
    virtual void open();
    virtual void menu(const QPoint&);
    virtual const char * may_start(UmlCode &) const;
    virtual const char * may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual bool move_with_its_package() const;
    virtual QString get_full_name() const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(QList<DiagramItem> &);

    virtual void apply_shortcut(QString s);
    void edit_drawing_settings();
  
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static OdClassInstCanvas * read(char * &, UmlCanvas * canvas, char *);

  protected:
    void check_attributes();
    void disconnect_attributes();

  protected:
    QList<AttributeData> attributes;
    QStringList values;	// unicode
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the class is deleted
};

#endif
