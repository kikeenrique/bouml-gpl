// *************************************************************************
//
// Copyleft 2004-2008 Bruno PAGES  .
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

#ifndef PINCANVAS_H
#define PINCANVAS_H

#include "DiagramCanvas.h"

class ActivityActionCanvas;
class LabelCanvas;

#define PIN_SIZE 11

class PinCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  protected:
    ActivityActionCanvas * act;
    UmlColor itscolor;
    UmlColor used_color;
        
  public:
    PinCanvas(BrowserNode * bn, UmlCanvas * canvas, int x, int y,
	      int id, ActivityActionCanvas * a);
    virtual ~PinCanvas();
    
    virtual void delete_it();
    virtual void delete_available(bool & in_model, bool & out_model) const;
    
    void update();
    void check_position();

    virtual void draw(QPainter & p);
    
    virtual UmlCode type() const;
    virtual bool copyable() const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual const char * may_start(UmlCode &) const;
    virtual const char * may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual void change_scale();
    virtual void moveBy(double dx, double dy);
    virtual bool primaryItem() const;
    void do_moveBy(double dx, double dy);
    void do_change_scale();

    bool action_selected() const;
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(QList<DiagramItem> &);
    void edit_drawing_settings();
    
    virtual void apply_shortcut(QString s);
  
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static PinCanvas * read(char * &, UmlCanvas *, char *, ActivityActionCanvas *);
    virtual void post_loaded();
    
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();
};

#endif
