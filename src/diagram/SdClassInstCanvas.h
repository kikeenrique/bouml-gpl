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

#ifndef SDCLASSINSTCANVAS_H
#define SDCLASSINSTCANVAS_H

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qobject.h>

#include "SdObjCanvas.h"
#include "ClassInstCanvas.h"

class BrowserNode;

class SdClassInstCanvas : public QObject, public SdObjCanvas, public ClassInstCanvas {
  Q_OBJECT
    
  protected:
    QString iname;	// useless if browser_node is a class instance rather than a class

  protected:
    ClassDrawingMode drawing_mode;
    ClassDrawingMode used_drawing_mode;
    
  public:
    SdClassInstCanvas(BrowserNode * t, UmlCanvas * canvas, int x, int id);
    virtual ~SdClassInstCanvas();
    
    virtual void delete_it();
        
    virtual void draw(QPainter & p);
    
    void compute_size();
    virtual UmlCode type() const;	// -> class or classinstance
    virtual QString get_name() const;	// all cases
    virtual void set_name(const QString & s);	// out of model case
    virtual BrowserNode * get_type() const;	// return class, all cases
    virtual void set_type(BrowserNode * t);	// out of model case
    virtual BrowserNodeList& get_types(BrowserNodeList&) const;
    virtual BrowserNode * container(UmlCode) const;
    virtual BrowserClass* get_class() const;
    virtual void delete_available(bool & in_model, bool & out_model) const;
    virtual void remove(bool from_model);
    virtual void open();
    virtual void menu(const QPoint&);
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(QList<DiagramItem> &);
    
    virtual void apply_shortcut(QString s);
    void edit_drawing_settings();
    
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static SdClassInstCanvas * read(char * &, UmlCanvas * canvas, char *);
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the class is deleted
};

#endif
