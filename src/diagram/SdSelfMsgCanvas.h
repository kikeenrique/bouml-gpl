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

#ifndef SDSELFMSGCANVAS_H
#define SDSELFMSGCANVAS_H

#include "SdMsgBaseCanvas.h"

class SdSelfMsgCanvas : public SdMsgBaseCanvas {
  public:
    SdSelfMsgCanvas(UmlCanvas * canvas, SdDurationCanvas * d,
		    UmlCode l, int v, int id);
    virtual ~SdSelfMsgCanvas();
    
    virtual void draw(QPainter & p);
    
    virtual void update_hpos();
    virtual double min_y() const;
    virtual void update();
    virtual void change_duration(SdDurationCanvas * oldone,
				 SdDurationCanvas * newone);
    virtual void menu(const QPoint&);
    virtual void select_associated();
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static SdSelfMsgCanvas * read(char * &, UmlCanvas *, char *);
};

#endif

