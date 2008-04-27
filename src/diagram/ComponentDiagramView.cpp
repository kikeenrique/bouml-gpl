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





#include <qfont.h>
#include <qpopupmenu.h> 

#include "ComponentDiagramWindow.h"
#include "ComponentDiagramView.h"
#include "ComponentCanvas.h"
#include "BrowserComponentDiagram.h"
#include "PackageCanvas.h"
#include "FragmentCanvas.h"
#include "SimpleRelationCanvas.h"
#include "IconCanvas.h"
#include "NoteCanvas.h"
#include "TextCanvas.h"
#include "ArrowJunctionCanvas.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "BrowserComponent.h"
#include "MenuTitle.h"
#include "myio.h"

ComponentDiagramView::ComponentDiagramView(QWidget * parent, UmlCanvas * canvas, int id)
    : DiagramView(parent, canvas, id) {
  load("Component");
}

void ComponentDiagramView::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Component diagram menu", m.font()), -1);
 
  switch (default_menu(m, 20)) {
  case EDIT_DRAWING_SETTING_CMD:
    ((BrowserComponentDiagram *) the_canvas()->browser_diagram())->edit_settings();
    break;
  case RELOAD_CMD:
    // pure drawing modifications are lost
    // mark the diagram modified because the undid modifications
    // may be saved in the file are not saved in memory
    load("Component");
    window()->package_modified();
    break;
  }
}

void ComponentDiagramView::contentsMousePressEvent(QMouseEvent * e) {
  if (!window()->frozen()) {
    UmlCode action = window()->buttonOn();
    
    switch (action) {
    case UmlComponent:
      {
	history_protected = TRUE;
	unselect_all();
	window()->selectOn();
	history_save();
	
	BrowserNode * b = 
	  BrowserComponent::get_component(((BrowserNode *) window()->browser_diagram()->parent()));
	
	if (b != 0) {
	  ComponentCanvas * c = 
	    new ComponentCanvas(b, the_canvas(), e->x(), e->y());
	  
	  c->show();
	  c->upper();
	  window()->package_modified();
	}
	canvas()->update();
	history_protected = FALSE;
      }
      break;
    default:
      DiagramView::contentsMousePressEvent(e);
      break;
    }
  }
  else
    DiagramView::contentsMousePressEvent(e);
}

void ComponentDiagramView::dragEnterEvent(QDragEnterEvent * e) {
  if (!window()->frozen() &&
      (UmlDrag::canDecode(e, UmlComponent, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlPackage, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlSimpleRelations, TRUE, TRUE) ||
       UmlDrag::canDecode(e, UmlClassDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlUseCaseDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlSeqDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlColDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlObjectDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlComponentDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlDeploymentDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlActivityDiagram, TRUE, TRUE) ||
       UmlDrag::canDecode(e, UmlStateDiagram, TRUE, TRUE)))
    e->accept();
  else
    e->ignore();
}

void ComponentDiagramView::dropEvent(QDropEvent * e) {
  BrowserNode * bn;
  QPoint p = viewportToContents(e->pos());
  
  if ((bn = UmlDrag::decode(e, UmlComponent)) != 0) {
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
	   ((bn = UmlDrag::decode(e, UmlDeploymentDiagram)) != 0) ||
	   ((bn = UmlDrag::decode(e, UmlStateDiagram, TRUE)) != 0) ||
	   ((bn = UmlDrag::decode(e, UmlActivityDiagram, TRUE)) != 0)) {
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

void ComponentDiagramView::save(QTextStream & st, QString & warning,
				bool copy) const {
  DiagramItemList items(canvas()->allItems());
  DiagramItem * di;
  
  if (!copy)
    // sort is useless for a copy
    items.sort();
  
  st << "format " << FILEFORMAT << "\n";
  
  // save first component packages fragment notes junctions and icons
  
  for (di = items.first(); di != 0; di = items.next()) {
    switch (di->type()) {
    case UmlComponent:
    case UmlNote:
    case UmlText:
    case UmlPackage:
    case UmlFragment:
    case UmlArrowJunction:
    case UmlIcon:
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
    case UmlRequired:
    case UmlProvided:
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

void ComponentDiagramView::read(char * st, char * k) {
  UmlCanvas * canvas = the_canvas();
  
  // reads first component package icons notes junctions and text
  while (ComponentCanvas::read(st, canvas, k) ||
	 NoteCanvas::read(st, canvas, k) ||
	 TextCanvas::read(st, canvas, k) ||
	 IconCanvas::read(st, canvas, k) ||
	 PackageCanvas::read(st, canvas, k) ||
	 FragmentCanvas::read(st, canvas, k) ||
	 ArrowJunctionCanvas::read(st, canvas, k))
    k = read_keyword(st);
  
  // then reads relations and anchors
  ArrowCanvas * a;
  
  while (((a = ArrowCanvas::read(st, canvas, k)) != 0) ||
	 ((a = SimpleRelationCanvas::read(st, canvas, k)) != 0)) {
    a->get_start()->check_line(a);
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "preferred_whz") || !strcmp(k, "prefered_whz")) {
    preferred_size.setWidth(read_unsigned(st));
    preferred_size.setHeight(read_unsigned(st));
    preferred_zoom = read_double(st);
    k = read_keyword(st);
  }
  
  if (strcmp(k, "end"))
    wrong_keyword(k, "end");
}


