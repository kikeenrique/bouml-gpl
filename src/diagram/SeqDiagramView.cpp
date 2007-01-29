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
#include <qmessagebox.h>

#include "SeqDiagramWindow.h"
#include "SeqDiagramView.h"
#include "BrowserSeqDiagram.h"
#include "BrowserClass.h"
#include "DiagramCanvas.h"
#include "SdClassInstCanvas.h"
#include "SdDurationCanvas.h"
#include "SdMsgCanvas.h"
#include "SdSelfMsgCanvas.h"
#include "ArrowCanvas.h"
#include "IconCanvas.h"
#include "FragmentCanvas.h"
#include "SdContinuationCanvas.h"
#include "NoteCanvas.h"
#include "TextCanvas.h"
#include "UmlPixmap.h"
#include "UmlDrag.h"
#include "myio.h"
#include "MenuTitle.h"

SeqDiagramView::SeqDiagramView(QWidget * parent, UmlCanvas * canvas, int id)
    : DiagramView(parent, canvas, id) {
  load("Sequence");
}

void SeqDiagramView::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu formatm(0);
  
  m.insertItem(new MenuTitle("Sequence diagram menu", m.font()), -1);
 
  switch (default_menu(m, 20)) {
  case EDIT_DRAWING_SETTING_CMD:
    ((BrowserSeqDiagram *) the_canvas()->browser_diagram())->edit_settings();
    return;
  case RELOAD_CMD:
    // pure drawing modifications are lost
    // mark the diagram modified because the undid modifications
    // may be saved in the file are not saved in memory
    load("Sequence");
    window()->package_modified();
    break;
  }
}

void SeqDiagramView::contentsMousePressEvent(QMouseEvent * e) {
  if (!window()->frozen()) {
    if (e->button() == QObject::RightButton)
      DiagramView::contentsMousePressEvent(e);
    else {
      UmlCode c = window()->buttonOn();
      
      switch (c) {
      case UmlClass:
	{
	  history_protected = TRUE;
	  unselect_all();
	  window()->selectOn();
	  history_save();
	  
	  BrowserNode * parent =
	    ((BrowserNode *) window()->browser_diagram()->parent());
	  BrowserClass * b = BrowserClass::get_class(parent);
	  
	  if (b != 0) {
	    SdClassInstCanvas * cli = 
	      new SdClassInstCanvas(((BrowserClass *) b), the_canvas(), e->x(), 0);
	    
	    cli->show();
	    cli->moveBy(e->x() - cli->center().x(), 0);
	    cli->upper();
	    window()->package_modified();
	  }
	}
	break;
      case UmlSyncSelfMsg:
      case UmlAsyncSelfMsg:
	{
	  history_protected = TRUE;
	  unselect_all();
	  window()->selectOn();
	  history_save();
	  
	  QCanvasItem * ci = the_canvas()->collision(e->pos());
	  
	  if (ci != 0) {
	    DiagramItem * i = QCanvasItemToDiagramItem(ci);
	    
	    if (i != 0) {
	      const char * err = i->may_start(c);
	      
	      if (err != 0)
		QMessageBox::critical(0, "Bouml" , err);
	      else {
		i->connexion(c, i, e->pos(), e->pos());
		window()->package_modified();
	      }
	    }
	  }
	}
	break;
      case UmlContinuation:
	{
	  history_protected = TRUE;
	  unselect_all();
	  window()->selectOn();
	  history_save();
	  
	  SdContinuationCanvas * cont = 
	    new SdContinuationCanvas(the_canvas(), e->x(), e->y(), 0);
	  
	  cont->show();
	  cont->upper();
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
}

void SeqDiagramView::contentsMouseMoveEvent(QMouseEvent * e) {
  if (!window()->frozen()) {
    DiagramView::contentsMouseMoveEvent(e);
    
    QCanvasItemList all = canvas()->allItems();
    QCanvasItemList::Iterator cit;
    
    for (cit = all.begin(); cit != all.end(); ++cit) {
      DiagramItem * it = QCanvasItemToDiagramItem(*cit);
      
      if ((it != 0) && // an uml canvas item
	  (*cit)->visible())
	it->update();
    }
  }
}

void SeqDiagramView::keyPressEvent(QKeyEvent * e) {
  if (!window()->frozen()) {
    DiagramView::keyPressEvent(e);
    
    if (e->state() != QObject::ControlButton) {
      switch (e->key()) {
      case QObject::Key_Left:
      case QObject::Key_Up:
      case QObject::Key_Right:
      case QObject::Key_Down:
	{
	  QCanvasItemList all = canvas()->allItems();
	  QCanvasItemList::Iterator cit;
    
	  for (cit = all.begin(); cit != all.end(); ++cit) {
	    DiagramItem * it = QCanvasItemToDiagramItem(*cit);
      
	    if ((it != 0) && // an uml canvas item
		(*cit)->visible())
	      it->update();
	  }
	}
	break;
      default:
	break;
      }
    }
  }
}

void SeqDiagramView::dragEnterEvent(QDragEnterEvent * e) {
  if (!window()->frozen() &&
      (UmlDrag::canDecode(e, UmlClass, TRUE, TRUE) ||
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

void SeqDiagramView::dropEvent(QDropEvent * e) {
  BrowserNode * bn;
  QPoint p = viewportToContents(e->pos());
  
  if ((bn = UmlDrag::decode(e, UmlClass)) != 0) {
    history_save();
    
    SdClassInstCanvas * cli = 
      new SdClassInstCanvas(((BrowserClass *) bn), the_canvas(), p.x(), 0);
    
    history_protected = TRUE;
    cli->show();
    cli->moveBy(p.x() - cli->center().x(), 0);
    canvas()->update();
    history_protected = FALSE;
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

void SeqDiagramView::save(QTextStream & st, QString & warning,
			  bool copy) const {
  DiagramItemList items(canvas()->allItems());
  DiagramItem * di;
  
  if (!copy)
    // sort is useless for a copy
    items.sort();
  
  st << "format " << FILEFORMAT << "\n";
  
  // save first the fragments, continuation, actors, classes instances, notes, icons and text
  
  for (di = items.first(); di != 0; di = items.next()) {
    switch (di->type()) {
    case UmlFragment:
    case UmlContinuation:
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
  
  // then save durations
  
  for (di = items.first(); di != 0; di = items.next())
    if ((!copy || di->copyable()) &&
	(di->type() == UmlActivityDuration))
      di->save(st, FALSE, warning);
  
  // then save messages
  
  for (di = items.first(); di != 0; di = items.next()) {
    switch (di->type()) {
    case UmlSyncMsg:
    case UmlAsyncMsg:
    case UmlSyncSelfMsg:
    case UmlAsyncSelfMsg:
    case UmlReturnMsg:
      if (!copy || di->copyable())
	di->save(st, FALSE, warning);
    default:
      break;
    }
  }
  
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

void SeqDiagramView::read(char * st, char * k) {
  UmlCanvas * canvas = the_canvas();
  
  // reads first the actors, classes instances, notes, icons and text
  while (SdClassInstCanvas::read(st, canvas, k) ||
	 NoteCanvas::read(st, canvas, k) ||
	 TextCanvas::read(st, canvas, k) ||
	 IconCanvas::read(st, canvas, k) ||
	 FragmentCanvas::read(st, canvas, k) ||
	 SdContinuationCanvas::read(st, canvas, k))
    k = read_keyword(st);
  
  // then reads durations
  while (SdDurationCanvas::read(st, canvas, k))
    k = read_keyword(st);
  
  // then reads msgs
  while (SdMsgCanvas::read(st, canvas, k) ||
	 SdSelfMsgCanvas::read(st, canvas, k))
    k = read_keyword(st);
  
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


