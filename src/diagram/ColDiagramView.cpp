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

#include <qfont.h>
#include <qpopupmenu.h> 
#include <qmessagebox.h>

#include "ColDiagramWindow.h"
#include "ColDiagramView.h"
#include "BrowserColDiagram.h"
#include "BrowserClass.h"
#include "DiagramCanvas.h"
#include "CodClassInstCanvas.h"
#include "CodDirsCanvas.h"
#include "CodLinkCanvas.h"
#include "CodSelfLinkCanvas.h"
#include "ArrowCanvas.h"
#include "IconCanvas.h"
#include "NoteCanvas.h"
#include "PackageCanvas.h"
#include "FragmentCanvas.h"
#include "TextCanvas.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "myio.h"
#include "MenuTitle.h"
#include "CodEditMsgDialog.h"

ColDiagramView::ColDiagramView(QWidget * parent, UmlCanvas * canvas, int id)
    : DiagramView(parent, canvas, id) {
  load("Collaboration");
}

void ColDiagramView::update_msgs() {
  ColMsg::update_ranks(get_msgs());
  update_msg_supports();
}

void ColDiagramView::update_msg_supports() {
  QCanvasItemList l = canvas()->allItems();
  QCanvasItemList::Iterator it;
    
  for (it = l.begin(); it != l.end(); ++it) {
    if ((*it)->visible()) { // at least not deleted
      DiagramItem * di = QCanvasItemToDiagramItem(*it);
      
      if (di != 0) {
	switch (di->type()) {
	case UmlSelfLink:
	  ((CodSelfLinkCanvas *) di)->update_msgs();
	  break;
	case UmlLinkDirs:
	  ((CodDirsCanvas *) di)->update_msgs();
	  break;
	default:	// to avoid compiler warning
	  break;
	}
      }
    }
  }
}

void ColDiagramView::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Collaboration diagram menu", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Edit all the messages", 19);
 
  switch (default_menu(m, 20)) {
  case EDIT_DRAWING_SETTING_CMD:
    ((BrowserColDiagram *) the_canvas()->browser_diagram())->edit_settings();
    break;
  case RELOAD_CMD:
    // pure drawing modifications are lost
    // mark the diagram modified because the undid modifications
    // may be saved in the file are not saved in memory
    load("Collaboration");
    window()->package_modified();
    break;
  case 19:
    CodEditMsgDialog::exec(this, msgs);
    break;
  }
}
  
void ColDiagramView::contentsMousePressEvent(QMouseEvent * e) {
  if (!window()->frozen()) {
    if (e->button() == QObject::RightButton)
      DiagramView::contentsMousePressEvent(e);
    else {
      UmlCode c = window()->buttonOn();
      
      switch (c) {
      case UmlClass:
	{
	  history_protected = TRUE;
	  window()->selectOn();
	  history_save();
	  
	  BrowserNode * parent =
	    ((BrowserNode *) window()->browser_diagram()->parent());
	  BrowserClass * b = BrowserClass::get_class(parent);
	  
	  if (b != 0) {
	    CodClassInstCanvas * cl = 
	      new CodClassInstCanvas(((BrowserClass *) b), the_canvas(),
				     e->x(), e->y(), 0);
	    
	    cl->show();
	    cl->upper();
	    window()->package_modified();
	  }
	}
	canvas()->update();
	history_protected = FALSE;
	break;
      case UmlSelfLink:
	{
	  history_protected = TRUE;
	  window()->selectOn();
	  history_save();
	  
	  QCanvasItem * ci = the_canvas()->collision(e->pos());
	  
	  if (ci != 0) {
	    DiagramItem * i = QCanvasItemToDiagramItem(ci);
	    
	    if (i != 0) {
	      const char * err = i->may_start(c);
	      
	      if (err != 0)
		QMessageBox::critical(0, "Bouml", err);
	      else {
		i->connexion(c, i, e->pos(), e->pos());
		window()->package_modified();
	      }
	    }
	  }
	}
	canvas()->update();
	history_protected = FALSE;
	break;
      default:
	DiagramView::contentsMousePressEvent(e);
	break;
      }
    }
  }
}

void ColDiagramView::dragEnterEvent(QDragEnterEvent * e) {
  if (!window()->frozen() &&
      (UmlDrag::canDecode(e, UmlClass, TRUE, TRUE) ||
       UmlDrag::canDecode(e, UmlPackage, FALSE, TRUE) ||
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

void ColDiagramView::dropEvent(QDropEvent * e) {
  BrowserNode * bn;
  QPoint p = viewportToContents(e->pos());
  
  if ((bn = UmlDrag::decode(e, UmlClass)) != 0) {
    history_save();
    
    CodClassInstCanvas * i = 
      new CodClassInstCanvas(((BrowserClass *) bn),
			     the_canvas(), p.x(), p.y(), 0);
    
    history_protected = TRUE;
    i->show();
    i->upper();
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

void ColDiagramView::save(QTextStream & st, QString & warning,
			  bool copy) const {
  DiagramItemList items(canvas()->allItems());
  DiagramItem * di;
  
  if (!copy)
    // sort is useless for a copy
    items.sort();
  
  st << "format " << FILEFORMAT << "\n";
  
  // save first the packages fragment, classes instances, notes, icons and text
  
  for (di = items.first(); di != 0; di = items.next()) {
    switch (di->type()) {
    case UmlPackage:
    case UmlFragment:
    case UmlClass:
    case UmlNote:
    case UmlText:
    case UmlIcon:
      if (!copy || di->copyable())
	di->save(st, FALSE, warning);
      // no break
    default:
      break;
    }
  }
  
  // then save links selflink and dirs (without messages)

  for (di = items.first(); di != 0; di = items.next()) {
    switch (di->type()) {
    case UmlLink:	// saves associated dirs
    case UmlSelfLink:
      if (!copy || di->copyable())
	di->save(st, FALSE, warning);
    default:	// to avoid warning
      break;
    }
  }
  
  // then save messages
  ColMsg::save(st, msgs, copy, warning,
	       window()->browser_diagram()->full_name());
  
  // then save anchors
  
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

void ColDiagramView::read(char * st, char * k) {
  UmlCanvas * canvas = the_canvas();
  
  // reads first the instances, package, fragment notes, icons and text
  while (CodClassInstCanvas::read(st, canvas, k) ||
	 NoteCanvas::read(st, canvas, k) ||
	 TextCanvas::read(st, canvas, k) ||
	 IconCanvas::read(st, canvas, k) ||
	 PackageCanvas::read(st, canvas, k) ||
	 FragmentCanvas::read(st, canvas, k))
    k = read_keyword(st);
  
  // then reads links selflink and dirs (without messages)
  while (CodLinkCanvas::read(st, canvas, k) ||	// updates k
	 CodSelfLinkCanvas::read(st, canvas, k) ||	// updates k
	 CodDirsCanvas::read(st, canvas, k))		// updates k
    ;
  
  // then read messages
  if (!strcmp(k, "msgs"))
    ColMsg::read(st, msgs, canvas);
  else if (strcmp(k, "no_msg"))
    wrong_keyword(k, "msgs/no_msg");
  
  k = read_keyword(st);
  
  // then show/update msgs
  QCanvasItemList items = canvas->allItems();  
  QCanvasItemList::Iterator it;
  
  for (it = items.begin(); it != items.end(); ++it) {
    DiagramItem * di = QCanvasItemToDiagramItem(*it);
    
    if (di != 0) {
      switch (di->type()) {
      case UmlSelfLink:
	((CodSelfLinkCanvas *) di)->update_msgs();
	break;
      case UmlLinkDirs:
	((CodDirsCanvas *) di)->update_msgs();
	break;
      default:	// to avoid warning
	break;
      }
    }
  }
  
  // then reads anchors
  while (ArrowCanvas::read(st, canvas, k))
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


