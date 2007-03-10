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

#include <qfont.h>
#include <qpopupmenu.h> 

#include "DeploymentDiagramWindow.h"
#include "DeploymentDiagramView.h"
#include "ComponentCanvas.h"
#include "ArtifactCanvas.h"
#include "DeploymentNodeCanvas.h"
#include "BrowserDeploymentDiagram.h"
#include "BrowserDeploymentNode.h"
#include "BrowserComponent.h"
#include "BrowserArtifact.h"
#include "PackageCanvas.h"
#include "FragmentCanvas.h"
#include "IconCanvas.h"
#include "SimpleRelationCanvas.h"
#include "HubCanvas.h"
#include "AssocContainCanvas.h"
#include "NoteCanvas.h"
#include "TextCanvas.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "MenuTitle.h"
#include "myio.h"

DeploymentDiagramView::DeploymentDiagramView(QWidget * parent, UmlCanvas * canvas, int id)
    : DiagramView(parent, canvas, id) {
  load("Deployment");
}

void DeploymentDiagramView::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu formatm(0);
  
  m.insertItem(new MenuTitle("Deployment diagram menu", m.font()), -1);
 
  switch (default_menu(m, 20)) {
  case EDIT_DRAWING_SETTING_CMD:
    ((BrowserDeploymentDiagram *) the_canvas()->browser_diagram())->edit_settings();
    break;
  case RELOAD_CMD:
    // pure drawing modifications are lost
    // mark the diagram modified because the undid modifications
    // may be saved in the file are not saved in memory
    load("Deployment");
    window()->package_modified();
    break;
  }
}

void DeploymentDiagramView::contentsMousePressEvent(QMouseEvent * e) {
  if (!window()->frozen()) {
    UmlCode action = window()->buttonOn();
    
    switch (action) {
    case UmlDeploymentNode:
      {
	history_protected = TRUE;
	unselect_all();
	window()->selectOn();
	history_save();
	
	BrowserNode * b = 
	  BrowserDeploymentNode::get_deploymentnode(((BrowserNode *) window()->browser_diagram()->parent()));
	
	if (b != 0) {
	  DeploymentNodeCanvas * d = 
	    new DeploymentNodeCanvas(b, the_canvas(), e->x(), e->y(), 0);
	  
	  d->show();
	  d->upper();
	  window()->package_modified();
	}
      }
      break;
    case UmlComponent:
      {
	history_protected = TRUE;
	unselect_all();
	window()->selectOn();
	history_save();
	
	BrowserNode * b = 
	  BrowserComponent::get_component(((BrowserNode *) window()->browser_diagram()->parent()),
					  TRUE);
	
	if (b != 0) {
	  ComponentCanvas * c = 
	    new ComponentCanvas(b, the_canvas(), e->x(), e->y());
	  
	  c->show();
	  c->upper();
	  window()->package_modified();
	}
      }
      break;
    case UmlArtifact:
      {
	history_protected = TRUE;
	unselect_all();
	window()->selectOn();
	history_save();
	
	BrowserNode * b = 
	  BrowserArtifact::get_artifact(((BrowserNode *) window()->browser_diagram()->parent()),
					 TRUE);
	
	if (b != 0) {
	  ArtifactCanvas * c = 
	    new ArtifactCanvas(b, the_canvas(), e->x(), e->y());
	  
	  c->show();
	  c->upper();
	  window()->package_modified();
	}
      }
      break;
    case UmlHub:
      {
	history_protected = TRUE;
	unselect_all();
	window()->selectOn();
	history_save();
	
	HubCanvas * h = new HubCanvas(the_canvas(), e->x(), e->y(), 0);
	
	h->show();
	h->upper();
	window()->package_modified();
      }
      break;
    default:
      DiagramView::contentsMousePressEvent(e);
      return;
    }
    
    canvas()->update();
    history_protected = FALSE;
  }
}

void DeploymentDiagramView::dragEnterEvent(QDragEnterEvent * e) {
  abort_line_construction();
  
  if (!window()->frozen() &&
      (UmlDrag::canDecode(e, UmlDeploymentNode, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlComponent, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlArtifact, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlPackage, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlSimpleRelations, TRUE, TRUE) ||
       UmlDrag::canDecode(e, UmlClassDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlUseCaseDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlSeqDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlColDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlObjectDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlComponentDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlDeploymentDiagram, FALSE, TRUE)))
    e->accept();
  else
    e->ignore();
}

void DeploymentDiagramView::dropEvent(QDropEvent * e) {
  BrowserNode * bn;
  QPoint p = viewportToContents(e->pos());
  
  if ((bn = UmlDrag::decode(e, UmlDeploymentNode)) != 0) {
    history_save();
    
    DeploymentNodeCanvas * d = 
      new DeploymentNodeCanvas(bn, the_canvas(), p.x(), p.y(), 0);
    
    history_protected = TRUE;
    d->show();
    d->upper();
    canvas()->update();
    history_protected = FALSE;
    window()->package_modified();
  }
  else if ((bn = UmlDrag::decode(e, UmlComponent)) != 0) {
    history_save();
    
    ComponentCanvas * c = 
      new ComponentCanvas(bn, the_canvas(), p.x(), p.y());
    
    history_protected = TRUE;
    c->show();
    c->upper();
    
    canvas()->update();
    history_protected = FALSE;
    window()->package_modified();
  }
  else if ((bn = UmlDrag::decode(e, UmlArtifact)) != 0) {
    history_save();
    
    ArtifactCanvas * c = 
      new ArtifactCanvas(bn, the_canvas(), p.x(), p.y());
    
    history_protected = TRUE;
    c->show();
    c->upper();
    
    canvas()->update();
    history_protected = FALSE;
    window()->package_modified();
  }
  else if ((bn = UmlDrag::decode(e, UmlPackage)) != 0) {
    history_save();
    
    PackageCanvas * pk = 
      new PackageCanvas(bn, the_canvas(), p.x(), p.y(), 0);
    
    history_protected = TRUE;
    pk->show();
    pk->upper();
    canvas()->update();
    window()->package_modified();
  }
  else if ((bn = UmlDrag::decode(e, UmlSimpleRelations, TRUE)) != 0) {
    history_save();
    
    history_protected = TRUE;
    SimpleRelationCanvas::drop(bn, the_canvas());
    canvas()->update();
    history_protected = FALSE;
  }
  else if (((bn = UmlDrag::decode(e, UmlClassDiagram)) != 0) ||
	   ((bn = UmlDrag::decode(e, UmlUseCaseDiagram)) != 0) ||
	   ((bn = UmlDrag::decode(e, UmlSeqDiagram)) != 0) ||
	   ((bn = UmlDrag::decode(e, UmlColDiagram)) != 0) ||
	   ((bn = UmlDrag::decode(e, UmlObjectDiagram)) != 0) ||
	   ((bn = UmlDrag::decode(e, UmlComponentDiagram)) != 0) ||
	   ((bn = UmlDrag::decode(e, UmlDeploymentDiagram)) != 0)) {
    history_save();
    
    IconCanvas * ic = new IconCanvas(bn, the_canvas(), p.x(), p.y(), 0);
    
    history_protected = TRUE;
    ic->show();
    ic->upper();
    canvas()->update();
    history_protected = FALSE;
    window()->package_modified();
  }
}

void DeploymentDiagramView::save(QTextStream & st, QString & warning,
				 bool copy) const {
  DiagramItemList items(canvas()->allItems());
  DiagramItem * di;
  
  if (!copy)
    // sort is useless for a copy
    items.sort();
  
  st << "format " << FILEFORMAT << "\n";
  
  // save first package fragment deploymentnode component hub notes text and icons
  
  for (di = items.first(); di != 0; di = items.next()) {
    switch (di->type()) {
    case UmlDeploymentNode:
    case UmlArtifact:
    case UmlComponent:
    case UmlPackage:
    case UmlFragment:
    case UmlNote:
    case UmlText:
    case UmlIcon:
    case UmlHub:
      if (!copy || di->copyable())
	di->save(st, FALSE, warning);
      // no break
    default:
      break;
    }
  }

  // then saves relations
  
  for (di = items.first(); di != 0; di = items.next()) {
    switch (di->type()) {
    case UmlInherit:
    case UmlDependency:
    case UmlDependOn:
    case UmlContain:
    case UmlAssociation:
      if (!copy || di->copyable())
	di->save(st, FALSE, warning);
    default:	// to avoid compiler warning
      break;
    }
  }
  
  // then saves anchors
  
  for (di = items.first(); di != 0; di = items.next())
    if ((!copy || di->copyable()) && (di->type() == UmlAnchor))
      di->save(st, FALSE, warning);
  
  if (!copy && (preferred_zoom != 0)) {
    nl_indent(st);
    st << "preferred_whz " << preferred_size.width() << ' '
      << preferred_size.height() << ' ' << preferred_zoom;
  }
  
  nl_indent(st);
  st << "end\n";
}

void DeploymentDiagramView::read(char * st, char * k) {
  UmlCanvas * canvas = the_canvas();
  
  // reads first deploymentnode component icons text and notes
  while (DeploymentNodeCanvas::read(st, canvas, k) ||
	 // reads artifact before component to manage component -> artifact
	 ArtifactCanvas::read(st, canvas, k) ||
	 ComponentCanvas::read(st, canvas, k) ||
	 HubCanvas::read(st, canvas, k) ||
	 NoteCanvas::read(st, canvas, k) ||
	 TextCanvas::read(st, canvas, k) ||
	 IconCanvas::read(st, canvas, k) ||
	 PackageCanvas::read(st, canvas, k) ||
	 FragmentCanvas::read(st, canvas, k))
    k = read_keyword(st);
  
  // then reads relations and anchors
  while (ArrowCanvas::read(st, canvas, k) ||
	 AssocContainCanvas::read(st, canvas, k) ||
	 SimpleRelationCanvas::read(st, canvas, k))
    k = read_keyword(st);
  
  if (!strcmp(k, "preferred_whz") || !strcmp(k, "prefered_whz")) {
    preferred_size.setWidth(read_unsigned(st));
    preferred_size.setHeight(read_unsigned(st));
    preferred_zoom = read_double(st);
    k = read_keyword(st);
  }
  
  if (strcmp(k, "end"))
    wrong_keyword(k, "end");
}


