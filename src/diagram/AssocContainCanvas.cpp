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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qcursor.h>
#include <qpopupmenu.h> 

#include "AssocContainCanvas.h"
#include "ArrowPointCanvas.h"
#include "ArtifactCanvas.h"
#include "UmlCanvas.h"
#include "LabelCanvas.h"
#include "StereotypeDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"

AssocContainCanvas::AssocContainCanvas(UmlCanvas * canvas, DiagramItem * b,
				   DiagramItem * e, int id)
    : ArrowCanvas(canvas, b, e, UmlContain, id, TRUE) {
  // note : can't be a self relation
}

AssocContainCanvas::~AssocContainCanvas() {
}

void AssocContainCanvas::open() {
  ArrowCanvas * plabel;
  ArrowCanvas * pstereotype;
  QStringList defaults;
  
  search_supports(plabel, pstereotype);
  
  if (edit(defaults, plabel, pstereotype)) {
    canvas()->update();
    package_modified();
  }
}

void AssocContainCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu geo(0);
  ArrowCanvas * aplabel;
  ArrowCanvas * apstereotype;
    
  search_supports(aplabel, apstereotype);
  
  AssocContainCanvas * plabel = (AssocContainCanvas *) aplabel;
  AssocContainCanvas * pstereotype = (AssocContainCanvas *) apstereotype;
  
  m.insertItem(new MenuTitle("Association", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Edit",1);
  
  if (pstereotype || plabel) {
    m.insertSeparator();
    m.insertItem("Select stereotype and label", 2);
    m.insertItem("Default stereotype and label position", 3);
    if (plabel && (label == 0))
      m.insertItem("Attach label to this segment", 4);
    if (pstereotype && (stereotype == 0))
      m.insertItem("Attach stereotype to this segment", 5);
  }
  
  if (get_start() != get_end()) {
    m.insertSeparator();
    init_geometry_menu(geo, 10);
    m.insertItem("Geometry (Ctrl+l)", &geo);
  }
  
  m.insertSeparator();
  m.insertItem("Delete from model",6);

  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 1:
    open();
    return;
  case 2:
    the_canvas()->unselect_all();
    the_canvas()->select(plabel->label);
    the_canvas()->select(pstereotype->stereotype);
    return;
  case 3:
    plabel->default_label_position();
    pstereotype->default_stereotype_position();
    break;
  case 4:
    label = plabel->label;
    plabel->label = 0;
    default_label_position();
    break;
  case 5:
    stereotype = pstereotype->stereotype;
    pstereotype->stereotype = 0;
    default_stereotype_position();
    break;
  case 6:
    get_start()->unassociate(get_end());	// line will be deleted
    break;
  default:
    if (rank >= 10) {
      rank -= 10;
      if (rank != (int) geometry)
	set_geometry((LineGeometry) rank, TRUE);
      else
	return;      
    }
    else
      return;
  }
  
  package_modified();
}

void AssocContainCanvas::remove(bool from_model) {
  if (from_model)
    get_start()->unassociate(get_end());	// line will be deleted
}

void AssocContainCanvas::delete_available(bool & in_model, bool &) const {
  in_model |= TRUE;
}

ArrowPointCanvas * AssocContainCanvas::brk(const QPoint & p) {
  ArrowPointCanvas * ap =
    new ArrowPointCanvas(the_canvas(), p.x(), p.y());
  
  ap->setZ(z() + 1);
  
  AssocContainCanvas * other =
    new AssocContainCanvas(the_canvas(), ap, end, 0);

  ap->add_line(this);
  end->remove_line(this);
  end = ap;
  
  //update_pos();
  
  ap->show();
  other->show();
  
  modified();
  other->modified();
  
  return ap;
}

void AssocContainCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "containcanvas_ref " << get_ident();
  else if (begin->type() != UmlArrowPoint) {
    nl_indent(st);
    st << "containcanvas " << get_ident();
    if (geometry != NoGeometry) {
      st << " geometry " << stringify(geometry);
      if (!fixed_geometry)
	st << " unfixed";
    }
    indent(+1);
    save_lines(st, TRUE, TRUE, warning);
    indent(-1);
  }
}

static ArrowCanvas * make(UmlCanvas * canvas, DiagramItem * b,
			  DiagramItem * e, UmlCode, int id)
{
  return new AssocContainCanvas(canvas, b, e, id);
}

AssocContainCanvas * AssocContainCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "containcanvas_ref"))
    return ((AssocContainCanvas *) dict_get(read_id(st), "contain canvas", canvas));
  else if (!strcmp(k, "containcanvas")) {
    int id = read_id(st);    
    LineGeometry geo;
    bool fixed;
    
    k = read_keyword(st);	// may read id
    if (!strcmp(k, "geometry")) {
      geo = line_geometry(read_keyword(st));
      k = read_keyword(st);
      if (! strcmp(k, "unfixed"))
	fixed = FALSE;
      else {
	fixed = TRUE;
	unread_keyword(k, st);
      }
    }
    else {
      unread_keyword(k, st);
      geo = NoGeometry;
      fixed = FALSE;
    }
    
    AssocContainCanvas * r = (AssocContainCanvas *)
      read_list(st, canvas, UmlContain, geo, fixed, id, &make);
    
    // remove association between components available in the
    // 2.0 deployment diagrams
    if (r->begin->type() == UmlComponent)
      Undefined.append(r);
    
    return r;
  }
  else
    return 0;
}

