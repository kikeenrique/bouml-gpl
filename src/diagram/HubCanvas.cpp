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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "HubCanvas.h"
#include "ArrowCanvas.h"
#include "UmlCanvas.h"
#include "myio.h"
#include "BrowserDiagram.h"
#include "MenuTitle.h"

HubCanvas::HubCanvas(UmlCanvas * canvas, int x, int y, int id) 
    : DiagramCanvas(0, canvas, x, y, -HUB_SIZE, -HUB_SIZE, id) {
  browser_node = canvas->browser_diagram();
}

HubCanvas::~HubCanvas() {
}

void HubCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

bool HubCanvas::alignable() const {
  return TRUE;
}

bool HubCanvas::copyable() const {
  return selected();
}

void HubCanvas::change_scale() {
  // the size is not modified
  QCanvasRectangle::setVisible(FALSE);
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void HubCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  p.drawRect(rect());
  
  if (selected())
    show_mark(p, rect());
}

UmlCode HubCanvas::type() const {
  return UmlHub;
}

void HubCanvas::open() {
}

void HubCanvas::menu(const QPoint&) {
  QPopupMenu m;
  
  m.insertItem(new MenuTitle("Network connexion", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Remove from view", 0);
  
  switch (m.exec(QCursor::pos())) {
  case 0:
    delete_it();
    break;
  default:
    return;
  }
  
  package_modified();
}

const char * HubCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlAssociation:
  case UmlAnchor:
    return 0;
  default:
    return "illegal";
  }
  return (l == UmlAssociation) ? 0 : "illegal";
}

const char * HubCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlDeploymentNode:
  case UmlHub:
    return (l == UmlAssociation) ? 0 : "illegal";
  case UmlNote:
  case UmlIcon:
    return (l == UmlAnchor) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

void HubCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref)
    st << "hubcanvas_ref " << get_ident();
  else {
    nl_indent(st);
    st << "hubcanvas " << get_ident();
    save_xyz(st, this, " xyz");
  }
}

HubCanvas * HubCanvas::read(char * & st, UmlCanvas * canvas,
			    char * k)
{
  if (!strcmp(k, "hubcanvas_ref"))
    return ((HubCanvas *) dict_get(read_id(st), "hubcanvas", canvas));
  if (strcmp(k, "hubcanvas"))
    return 0;
  
  int id = read_id(st);
  
  read_keyword(st, "xyz");
  
  int x = (int) read_double(st);
  
  HubCanvas * result =
    new HubCanvas(canvas, x, (int) read_double(st), id);
  
  result->setZ(read_double(st));
  result->show();
  
  return result;
}

