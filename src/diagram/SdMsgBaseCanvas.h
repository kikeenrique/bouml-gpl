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

#ifndef SDMSGBASECANVAS_H
#define SDMSGBASECANVAS_H

#include "DiagramCanvas.h"
#include "mystr.h"

class SdDurationCanvas;
class OperationData;

class SdMsgBaseCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  friend class SdMsgDialog;
    
  protected:
    SdDurationCanvas * dest;
    MyStr explicit_msg;
    const OperationData * msg;
    MyStr args;
    UmlCode itsType;
    bool already_moved;
    Uml3States show_full_oper;
    DrawingLanguage drawing_language;
  
  protected:
    SdMsgBaseCanvas(UmlCanvas * canvas, SdDurationCanvas * d,
		    UmlCode l, int v, int id);
  
    void update_label();
    
  public:
    virtual ~SdMsgBaseCanvas();
  
    virtual void delete_it();
  
    SdDurationCanvas * get_dest()  const { return dest; };
    QString get_msg(bool with_args) const;
    QString get_args() const { return args; };
    void set_msg(const OperationData * m, const QString & e, const QString & a);
    bool is_synchronous() const;
    void set_synchronous(bool yes);
    void update_after_move(SdDurationCanvas * p);
    void default_label_position() const;
    virtual void update_hpos() = 0;
    virtual double min_y() const = 0;
    virtual void change_duration(SdDurationCanvas * oldone,
				 SdDurationCanvas * newone) = 0;

    virtual UmlCode type() const;
    virtual const char * may_start(UmlCode &) const;
    virtual const char * may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void delete_available(bool & in_model, bool & out_model) const;
    virtual void open();
    virtual void select_associated() = 0;
    virtual void change_scale();
    virtual bool copyable() const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    virtual void moveBy(double dx, double dy);
    
    void save(QTextStream & st, QString & warning) const;
    void read(char * &);
    
  protected slots:
    void modified();	// canvas must be updated
};

#endif

