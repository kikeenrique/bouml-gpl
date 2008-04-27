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





#include <qapplication.h>
#include <qfont.h>
#include <qpopupmenu.h> 

#include "ClassDiagramWindow.h"
#include "ClassDiagramView.h"
#include "BrowserClassDiagram.h"
#include "BrowserClass.h"
#include "DiagramCanvas.h"
#include "CdClassCanvas.h"
#include "PackageCanvas.h"
#include "FragmentCanvas.h"
#include "RelationCanvas.h"
#include "SimpleRelationCanvas.h"
#include "IconCanvas.h"
#include "NoteCanvas.h"
#include "TextCanvas.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "myio.h"
#include "MenuTitle.h"
#include "BrowserView.h"

ClassDiagramView::ClassDiagramView(QWidget * parent, UmlCanvas * canvas, int id)
    : DiagramView(parent, canvas, id) {
  load("Class");
}

void ClassDiagramView::menu(const QPoint& p) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Class diagram menu", m.font()),  -1);

  if ((((UmlCanvas *) canvas())->browser_diagram())->is_writable()) {
    BrowserNode * bn = BrowserView::selected_item();
    DiagramItemList items(canvas()->allItems());
    QPtrDict<DiagramItem> drawn;
    
    if ((bn != 0) && (bn->get_type() == UmlClassView)) {
      // memoryze drawn classes
      DiagramItem * di;
      
      for (di = items.first(); di != 0; di = items.next())
	if (di->type() == UmlClass)
	  drawn.insert(di->get_bn(), di);
      
      // class view contains class not already drawn ?
      QListViewItem * child;
      
      for (child = bn->firstChild(); child != 0; child = child->nextSibling()) {
	if (!((BrowserNode *) child)->deletedp() &&
	    (((BrowserNode *) child)->get_type() == UmlClass) &&
	    (drawn[(BrowserNode *) child] == 0)) {
	  // not already shown
	  m.insertItem("Add classes of the selected class view", 29);
	  m.insertSeparator();
	  break;
	}
      }
    }
    
    switch (default_menu(m, 30)) {
    case EDIT_DRAWING_SETTING_CMD:
      ((BrowserClassDiagram *) ((UmlCanvas *) canvas())->browser_diagram())->edit_settings();
      break;
    case RELOAD_CMD:
      // pure drawing modifications are lost
      // mark the diagram modified because the undid modifications
      // may be saved in the file are not saved in memory
      load("Class");
      window()->package_modified();
      break;
    case 29:
      add_classview_classes(bn, p, drawn);
      break;
    }
  }
  else
    (void) default_menu(m, 30);
}

void ClassDiagramView::add_classview_classes(BrowserNode * cv, const QPoint& p,
					     QPtrDict<DiagramItem> & drawn) {
  history_save();
  history_protected = TRUE;
  
  DiagramItemList items(canvas()->allItems());
  QListViewItem * child;
  const int margin = 20;
  int x = p.x();
  int y = p.y();
  int future_y = margin;
  int xmax = canvas()->width() - margin;
  int ymax = canvas()->height() - margin;
  
  for (child = cv->firstChild(); child != 0; child = child->nextSibling()) {
    if (!((BrowserNode *) child)->deletedp() &&
	(((BrowserNode *) child)->get_type() == UmlClass) &&
	(drawn[(BrowserNode *) child] == 0)) {
      CdClassCanvas * cl = 
	new CdClassCanvas((BrowserNode *) child, the_canvas(), x, y);

      if ((x + cl->width()) > xmax)
	cl->move(x = margin, y = future_y);
      
      if (y + cl->height() > ymax) {
	cl->move(x = margin, y = margin);
	future_y = y + cl->height() + margin;
      }
      else {
	int bot = y + cl->height() + margin;
      
	if (bot > future_y)
	  future_y = bot;
      }
	      
      x = x + cl->width() + margin;
      
      cl->show();
      cl->upper();
    }
  }
  
  canvas()->update();
  history_protected = FALSE;
  window()->package_modified();
}

void ClassDiagramView::contentsMousePressEvent(QMouseEvent * e) {
  if (!window()->frozen()) {
    switch (window()->buttonOn()) {
    case UmlClass:
      {
	history_protected = TRUE;
	unselect_all();
	window()->selectOn();
	history_save();
	
	BrowserNode * parent = 
	  ((BrowserNode *) window()->browser_diagram()->parent());
	BrowserNode * b = BrowserClass::get_class(parent);
	
	if (b != 0) {
	  CdClassCanvas * cl = 
	    new CdClassCanvas(b, the_canvas(), e->x(), e->y());
	  
	  cl->show();
	  cl->upper();
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

void ClassDiagramView::dragEnterEvent(QDragEnterEvent * e) {
  if (!window()->frozen() &&
      (UmlDrag::canDecode(e, UmlClass, TRUE, TRUE) ||
       UmlDrag::canDecode(e, UmlRelations, TRUE, TRUE) ||
       UmlDrag::canDecode(e, UmlSimpleRelations, TRUE, TRUE) ||
       UmlDrag::canDecode(e, UmlPackage, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlClassDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlUseCaseDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlSeqDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlColDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlObjectDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlComponentDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlDeploymentDiagram, FALSE, TRUE) ||
       UmlDrag::canDecode(e, UmlStateDiagram, TRUE, TRUE) ||
       UmlDrag::canDecode(e, UmlActivityDiagram, TRUE, TRUE)))
    e->accept();
  else
    e->ignore();
}

void ClassDiagramView::dropEvent(QDropEvent * e) {
  BrowserNode * bn;
  QPoint p = viewportToContents(e->pos());
  
  if ((bn = UmlDrag::decode(e, UmlClass)) != 0) {
    history_save();
    
    CdClassCanvas * cl = 
      new CdClassCanvas(bn, the_canvas(), p.x(), p.y());
    
    history_protected = TRUE;
    cl->show();
    cl->upper();
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
    history_protected = FALSE;
    window()->package_modified();
  }
  else if ((bn = UmlDrag::decode(e, UmlRelations, TRUE)) != 0) {
    history_save();
    
    history_protected = TRUE;
    RelationCanvas::drop(bn, the_canvas());
    canvas()->update();
    history_protected = FALSE;
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

void ClassDiagramView::save(QTextStream & st, QString & warning,
			    bool copy) const {
  DiagramItemList items(canvas()->allItems());
  DiagramItem * di;
  
  if (!copy)
    // sort is useless for a copy
    items.sort();
    
  st << "format " << FILEFORMAT << "\n";
  
  // save first the classes packages fragment notes and icons
  
  for (di = items.first(); di != 0; di = items.next()) {
    switch (di->type()) {
    case UmlClass:
    case UmlNote:
    case UmlText:
    case UmlPackage:
    case UmlFragment:
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
    if (!copy || di->copyable()) {
      UmlCode k = di->type();
      
      if (IsaRelation(k) || IsaSimpleRelation(k) || (k == UmlInner))
	di->save(st, FALSE, warning);
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

void ClassDiagramView::read(char * st, char * k) {
  UmlCanvas * canvas = the_canvas();
  
  // reads first the classes package icons text and notes
  while (CdClassCanvas::read(st, canvas, k) ||
	 NoteCanvas::read(st, canvas, k) ||
	 TextCanvas::read(st, canvas, k) ||
	 IconCanvas::read(st, canvas, k) ||
	 PackageCanvas::read(st, canvas, k) ||
	 FragmentCanvas::read(st, canvas, k))
    k = read_keyword(st);
  
  // then reads relations and anchors
  while (ArrowCanvas::read(st, canvas, k) ||
	 RelationCanvas::read(st, canvas, k) ||
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
