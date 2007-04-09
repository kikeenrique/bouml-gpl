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

#ifndef BROWSERDIAGRAM_H
#define BROWSERDIAGRAM_H

#include "BrowserNode.h"
#include "DiagramItem.h"
#include "Labeled.h"

class QTextStream;

class BrowserDiagram : public BrowserNode, public Labeled<BrowserDiagram> {
  protected:
    static IdDict<BrowserDiagram> all;
  
    CanvasFormat canvas_size;
      
    BrowserDiagram(int id);
  
  public:
    BrowserDiagram(QString s, BrowserNode * parent, int id);
    virtual ~BrowserDiagram();
  
    virtual void package_modified();
    
    virtual bool get_auto_label_position(UmlCode who) const;
    virtual bool get_shadow() const = 0;
    virtual bool get_draw_all_relations() const = 0;
    virtual void dont_draw_all_relations() = 0;

    virtual void draw_svg() const = 0;
    
    CanvasFormat get_format() const { return canvas_size; }
    void set_format(CanvasFormat c) { canvas_size = c; }
    virtual void read_session(char * & st) = 0;
    
    void save();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(QTextStream &);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    static void import();
    static BrowserNode * read_diagram_ref(char * & st);
};

#endif
