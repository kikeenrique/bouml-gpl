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

#include <qpainter.h>
#include <qcursor.h>
#include <qpopupmenu.h> 
#include <qapplication.h>
#include <qclipboard.h>
#include <qpaintdevicemetrics.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qbuffer.h>
#ifdef WIN32
#include <qtimer.h>
#endif

#include "DiagramView.h"
#include "DiagramWindow.h"
#include "DiagramCanvas.h"
#include "RelationCanvas.h"
#include "SimpleRelationCanvas.h"
#include "TransitionCanvas.h"
#include "FlowCanvas.h"
#include "CodLinkCanvas.h"
#include "ObjectLinkCanvas.h"
#include "AssocContainCanvas.h"
#include "ArrowPointCanvas.h"
#include "LabelCanvas.h"
#include "NoteCanvas.h"
#include "TextCanvas.h"
#include "BrowserPackage.h"
#include "PackageCanvas.h"
#include "FragmentCanvas.h"
#include "SelectAreaCanvas.h"
#include "BrowserDiagram.h"
#include "UmlWindow.h"
#include "Shortcut.h"
#include "MenuTitle.h"
#include "myio.h"

#include "align_top.xpm"
#include "align_bottom.xpm"
#include "align_left.xpm"
#include "align_right.xpm"
#include "align_center.xpm"
#include "align_vcenter.xpm"
#include "align_hcenter.xpm"

QCString DiagramView::clipboard;
UmlCode DiagramView::copied_from;

DiagramView::DiagramView(QWidget * parent, UmlCanvas * canvas, int i)
    : QCanvasView(canvas, parent), id(i), selectArea(0), start(0),
      line(0), draw_line(FALSE), arrowBeginning(0), do_resize(NoCorner),
      preferred_zoom(0), history_protected(FALSE), history_frozen(FALSE),
      first_move(FALSE), history_index(~0u) {
  // enableClipper(TRUE); => probleme d'affichage
  setStaticBackground(TRUE);
  setAcceptDrops(TRUE);
  setDragAutoScroll(TRUE);
  setVScrollBarMode(Auto);
  setHScrollBarMode(Auto);
  setFocusPolicy(QWidget::StrongFocus);
  //setFocusPolicy(QWidget::WheelFocus);
  canvas->setBackgroundColor(QObject::white);
  canvas->setDoubleBuffering(TRUE);
  
  canvas->set_view(this);
  
  history.setAutoDelete(TRUE);
}

void DiagramView::init()
{
  clipboard = 0;
}

UmlCanvas * DiagramView::the_canvas() const {
  return (UmlCanvas *) canvas();
}

void DiagramView::contentsMouseDoubleClickEvent(QMouseEvent * e) {
  setFocus();
  unselect_all();
  
  if (draw_line)
    abort_line_construction();
  else {
    QCanvasItem * ci = the_canvas()->collision(e->pos());
    
    if (ci != 0) {
      DiagramItem * di = QCanvasItemToDiagramItem(ci);
      
      if (di != 0)
	di->open();
    }
  }
}

void DiagramView::contentsMousePressEvent(QMouseEvent * e) {
  first_move = TRUE;
  setFocus();
  if (!window()->frozen()) {
    QCanvasItem * ci = the_canvas()->collision(e->pos());
    
    if (e->button() == QObject::RightButton) {
      // menu on several objects (excluding labels)
  
      if (! draw_line) {
	const QCanvasItemList selected = selection();
	QCanvasItemList::ConstIterator it;
	int n_targets = 0;
	bool in_model = FALSE;
	bool out_model = FALSE;
	bool alignable = FALSE;
	UmlCode k = UmlCodeSup;
	QList<DiagramItem> l;
	
	for (it = selected.begin(); it != selected.end(); ++it) {
	  if (! isa_label(*it)) {
	    n_targets += 1;
	    
	    DiagramItem * item = QCanvasItemToDiagramItem(*it);
	    
	    item->delete_available(in_model, out_model);
	    alignable |= item->alignable();
	    
	    if (item->has_drawing_settings()) {
	      // note : relations doesn't have drawing setting, transition and flow have
	      switch (k) {
	      case UmlCodeSup:
		// first case
		k = item->type();
		l.append(item);
		break;
	      case UmlArrowPoint:
		// mark for several types
		break;
	      default:
		if (item->type() == k)
		  l.append(item);
		else {
		  // several types
		  l.clear();
		  k = UmlArrowPoint;	// mark for several types
		}
	      }
	    }
	  }	  
	}
	
	if (n_targets > 1) {
	  multiple_selection_menu(in_model, out_model, alignable, l);
	  return;
	}
      }
      
      unselect_all();
      
      if (draw_line)
	abort_line_construction();
      else if (! BrowserNode::popupMenuActive()) {	// Qt bug
	BrowserNode::setPopupMenuActive(TRUE);
	
	if (ci != 0) {
	  DiagramItem * di = QCanvasItemToDiagramItem(ci);
	  
	  if (di != 0)
	    di->menu(e->pos());
	  BrowserNode::setPopupMenuActive(FALSE);
	}
	else  {
	  menu(e->pos());
	  BrowserNode::setPopupMenuActive(FALSE);
	  // here the window & view may be deleted
	  return;
	}
      }
    }
    else if (draw_line)
      history_protected = TRUE;
    else {
      history_protected = TRUE;
      
      UmlCode & action = window()->buttonOn();
      
      switch (action) {
      case UmlSelect:
	mousePressPos = e->pos();
	
	if (ci == 0) {
	  // ne designe pas d objet, vide la selection
	  unselect_all();
	}
	else {
	  const QCanvasItemList selected = selection();
	  QCanvasItemList::ConstIterator it = selected.find(ci);
	  
	  if ((e->state() & ControlButton) == 0) {
	    if (it == selected.end()) {
	      // ne designe pas un objet selectionne, vide la selection
	      unselect_all();
	      
	      // l objet est selectionne
	      select(ci);
	    }
	    // sinon on ne fait rien, l utilisateur veut probablement
	    // deplacer les objets selectionnes
	  }
	  else if (it != selected.end())
	    // l objet etait selectionne
	    unselect(ci);
	  else
	    // l objet n etait pas selectionne
	    select(ci);

	  DiagramItem * di = QCanvasItemToDiagramItem(ci);
	  
	  if (di != 0)
	    do_resize = di->on_resize_point(e->pos());
	}
	break;
      case UmlNote:
	{
	  unselect_all();
	  window()->selectOn();
	  history_save();
	  
	  NoteCanvas * note = 
	    new NoteCanvas(the_canvas(), e->x(), e->y(), 0);
	  
	  note->show();
	  note->upper();
	  window()->package_modified();
	}
	break;
      case UmlText:
	{
	  unselect_all();
	  window()->selectOn();
	  history_save();
	  
	  TextCanvas * text = 
	    new TextCanvas(the_canvas(), e->x(), e->y(), 0);
	  
	  text->show();
	  text->upper();
	  text->open();
	  window()->package_modified();
	}
	break;
      case UmlPackage:
	unselect_all();
	window()->selectOn();
	{
	  BrowserPackage * p = BrowserPackage::get_package();
	  
	  if (p != 0) {
	    history_save();
	    
	    PackageCanvas * pk =
	      new PackageCanvas(p, the_canvas(), e->x(), e->y(), 0);
	    
	    pk->show();
	    pk->upper();
	    window()->package_modified();
	  }
	}
	break;
      case UmlFragment:
	{
	  unselect_all();
	  window()->selectOn();
	  history_save();
	  
	  FragmentCanvas * f = 
	    new FragmentCanvas(the_canvas(), e->x(), e->y(), 0);
	  
	  f->show();
	  f->upper();
	  window()->package_modified();
	}
	break;
      default:
	// lines
	unselect_all();
	
	if ((ci != 0) && isa_label(ci))
	  ci = the_canvas()->collision(e->pos(), RTTI_LABEL);
	
	if (ci != 0) {
	  DiagramItem * i = QCanvasItemToDiagramItem(ci);
	  
	  if (i != 0) {
	    const char * err = i->may_start(action);
	    
	    if (err != 0) {
	      QMessageBox::critical(0, "Bouml" , err);
	      window()->selectOn();
	    }
	    else {
	      history_save();
	      setCursor(pointingHandCursor);
	      arrowBeginning = start = i;
	      draw_line = TRUE;
	      mousePressPos = e->pos();
	    }
	  }
	  else
	    window()->selectOn();
	}
	else
	  window()->selectOn();
      }
    }
      
    canvas()->update();
    history_protected = FALSE;
  }
}

void DiagramView::contentsMouseReleaseEvent(QMouseEvent * e) {
  if (!window()->frozen()) {
    if (do_resize != NoCorner)
      do_resize = NoCorner;
    else if (line != 0) {
      history_protected = TRUE;
      the_canvas()->del(line);
      line = 0;
      
      QCanvasItem * ci = the_canvas()->collision(e->pos(), RTTI_LABEL);
      UmlCode action = window()->buttonOn();
      
      if (ci != 0) {
	DiagramItem * i = QCanvasItemToDiagramItem(ci);
	
	if (i != 0) {
	  UmlCode theo = action;
	  const char * err = arrowBeginning->may_connect(action, i);
	  
	  if (err == 0) {
	    if ((theo != action) && (start != arrowBeginning))
	      relation_to_simplerelation(action);
	    start->connexion(action, i, mousePressPos, e->pos());
	    window()->package_modified();
	    temp.clear();
	    draw_line = FALSE;
	    unsetCursor();
	    arrowBeginning->post_connexion(action, i);
	  }
	  else if (strcmp(err, "illegal")) {
	    QMessageBox::critical(0, "Bouml", err);
	    abort_line_construction();
	  }
	  else if (arrowBeginning->allowed_direction(action) == DiagramItem::All) {
	    add_point(e);
	    return;
	  }
	  else
	    abort_line_construction();
	}
	else
	  abort_line_construction();
      }
      else if (start->may_connect(action)) {
	// component required/provided interface
	if (start->connexion(action, mousePressPos, e->pos())) {
	  window()->package_modified();
	  temp.clear();
	  draw_line = FALSE;
	  unsetCursor();
	}
	else
	  abort_line_construction();
      }
      else if (arrowBeginning->allowed_direction(action) == DiagramItem::All) {
	add_point(e);
	return;
      }
      else
	abort_line_construction();
      
      history_protected = TRUE;
      canvas()->update();
      history_protected = FALSE;
      window()->selectOn();
    }
    else if ((e->button() != QObject::RightButton) && (selectArea != 0)) {
      // selectionne les objets dans la zone de selection
      QRect r = selectArea->boundingRect();
      
      the_canvas()->del(selectArea);
      selectArea = 0;
      
      QCanvasItemList l = canvas()->collisions(r);
      QCanvasItemList::Iterator it;
      
      for (it = l.begin(); it != l.end(); ++it)
	if (r.contains((*it)->boundingRect(), TRUE) && // completement inclus
	    !isa_alien(*it) &&		// DiagramItem
	    (*it)->visible() &&		// pas en attente de destruction
	    !(*it)->selected())   	// pas deja selectionne
	  select(*it);
      
      history_protected = TRUE;
      canvas()->update();
      history_protected = FALSE;
    }
  }
}

void DiagramView::add_point(QMouseEvent * e) {
  // adds an ArrowPoint and the line to it
  history_protected = TRUE;
  
  ArrowPointCanvas * ap =
    new ArrowPointCanvas(the_canvas(), e->x(), e->y());
  UmlCode action = window()->buttonOn();
  
  ap->upper();
  
  UmlCode t = window()->browser_diagram()->get_type();
  ArrowCanvas * a;
  
  if ((t == UmlClassDiagram) && IsaRelation(action))
    a = new RelationCanvas(the_canvas(), start, ap, 0, action, 0);
  else if ((t == UmlColDiagram) && (action == UmlLink))
    a = new CodLinkCanvas(the_canvas(), start, ap, 0);
  else if ((t == UmlObjectDiagram) && (action == UmlObjectLink))
    a = new ObjectLinkCanvas(the_canvas(), start, ap, 0);
  else if (((t == UmlComponentDiagram) || (t == UmlDeploymentDiagram))
	   && (action == UmlContain))
    a = new AssocContainCanvas(the_canvas(), start, ap, 0);
  else if ((t == UmlStateDiagram) && (action == UmlTransition))
    a = new TransitionCanvas(the_canvas(), start, ap, 0, 0);
  else if ((t == UmlActivityDiagram) && (action == UmlFlow))
    a = new FlowCanvas(the_canvas(), start, ap, 0, 0);
  else if (IsaSimpleRelation(action))
    a = new SimpleRelationCanvas(the_canvas(), start, ap, 0, action, 0);
  else
    a = new ArrowCanvas(the_canvas(), start, ap, action, 0, FALSE);
  
  temp.append(a);		// before the point, see abort_line_construction()
  temp.append(ap);
  
  ap->show();
  a->show();
  
  // re create immediatly the line
  draw_line = TRUE;
  mousePressPos = e->pos();
  start = ap;
  line = new QCanvasLine(canvas());
  line->setZ(TOP_Z);
  line->setPoints(e->pos().x(), e->pos().y(), 
		  e->pos().x(), e->pos().y());
  line->setPen(QObject::DotLine);
  line->show();
  line->setPen(QObject::SolidLine);
  
  canvas()->update();
  history_protected = FALSE;
  window()->package_modified();
  
  // remark : mouse tracking does not work (?)
  setCursor(pointingHandCursor);
}

void DiagramView::contentsMouseMoveEvent(QMouseEvent * e) {
  if (!window()->frozen()) {
    if (e->button() != QObject::RightButton) {
      history_protected = TRUE;
      
      int dx = e->pos().x() - mousePressPos.x();
      int dy = e->pos().y() - mousePressPos.y();
      QCanvasItemList selected = selection();
      
      if (! selected.isEmpty()) {
	// deplace/redimentionne les objets selectionnes
	if ((selected.count() == 1) && isa_arrow(selected.first())) {
	  ArrowCanvas * a = ((ArrowCanvas *) selection().first());
	  ArrowPointCanvas * ap;
	  
	  if (a->cut_on_move(ap)) {
	    // cuts the line in two adding an ArrowPoint which
	    // becomes the alone selected item allowing to move it
	    history_save();
	    unselect(a);
	    select(a->brk(e->pos()));
	    window()->package_modified();
	    first_move = FALSE;
	  }
	  else if (ap != 0) {
	    // replace the selection by 'ap' to move it
	    unselect(a);
	    select(ap);
	    if (first_move) {
	      history_save();
	      first_move = FALSE;
	      moveSelected(dx, dy, TRUE);
	    }
	    else
	      moveSelected(dx, dy, FALSE);
	  }
	}
	else {
	  if (first_move) {
	    history_save();
	    first_move = FALSE;
	    moveSelected(dx, dy, TRUE);
	  }
	  else
	    moveSelected(dx, dy, FALSE);
	}
	
	mousePressPos = e->pos();
      }
      else if (draw_line) {
	if (line == 0) {
	  // premier deplacement : cree la ligne
	  line = new QCanvasLine(canvas());
	  line->setZ(TOP_Z);
	  switch (arrowBeginning->allowed_direction(window()->buttonOn())) {
	  case DiagramItem::Horizontal:
	    // horizontal line
	    line->setPoints(mousePressPos.x(), mousePressPos.y(), 
			    e->pos().x(), mousePressPos.y());
	    break;
	  default:
	    line->setPoints(mousePressPos.x(), mousePressPos.y(), 
			    e->pos().x(), e->pos().y());
	  }
	  line->setPen(QObject::DotLine);
	  line->show();
	  line->setPen(QObject::SolidLine);
	}
	else {
	  QPoint st = line->startPoint();
	  
	  switch (arrowBeginning->allowed_direction(window()->buttonOn())) {
	  case DiagramItem::Horizontal:
	    // horizontal line
	    line->setPoints(st.x(), st.y(), e->pos().x(), st.y());
	    break;
	  default:
	    line->setPoints(st.x(), st.y(), e->pos().x(), e->pos().y());
	  }
	}
      }
      else {
	// la selection est vide, veut probablement la faire en faisant
	// une zone via la souris, montre cette zone
	if (selectArea == 0) {
	  // premier deplacement : cree la zone
	  selectArea = 
	    new SelectAreaCanvas(mousePressPos.x(), mousePressPos.y(), dx, dy, canvas());
	  
	  selectArea->setZ(TOP_Z);
	  selectArea->show();
	}
	else {
	  // redimensionne la zone
	  selectArea->setSize(dx, dy);
	}
      }    
      
      canvas()->update();
      history_protected = FALSE;
    }
  }
}

void DiagramView::abort_line_construction() {
  if (draw_line) {
    draw_line = FALSE;
    history_protected = TRUE;
    unsetCursor();
        
    if (line != 0) {
      the_canvas()->del(line);
      line = 0;
    }
    
    if (! temp.isEmpty()) {
      QCanvasItemList::Iterator it = temp.begin();
      
      // remove the line in the start item
      arrowBeginning->remove_line(((ArrowCanvas *) *it));
      
      do {
	the_canvas()->del(*it);
	it = temp.remove(it);
      } while (it != temp.end());
    }
      
    canvas()->update();
    history_protected = FALSE;
  }
}

void DiagramView::relation_to_simplerelation(UmlCode k) {  
  QCanvasItemList::Iterator it = temp.begin();

  // remove the line in the start item
  arrowBeginning->remove_line(((ArrowCanvas *) *it));
  
  start = arrowBeginning;
  
  do {
    // *it = arrow, (*it+1) = arrowpoint
    the_canvas()->del(*it);
    it = temp.remove(it);
    
    ArrowPointCanvas * ap =
      new ArrowPointCanvas(the_canvas(), (int) (*it)->x(), (int) (*it)->y());
    
    ap->upper();
  
    ArrowCanvas * a = 
      new SimpleRelationCanvas(the_canvas(), start, ap, 0, k, 0);
    
    ap->show();
    a->show();
    start = ap;
    
    the_canvas()->del(*it);
    it = temp.remove(it);
  } while (it != temp.end());
}


#ifdef WIN32
#pragma optimize("", off)
// generated code is wrong if optimize speed
#endif
void DiagramView::delete_them(bool in_model) {
  if (!in_model)
    history_save();
  
  for (;;) {
    const QCanvasItemList & selected = selection();

    if (selected.isEmpty())
      break;

    QCanvasItemList::ConstIterator it = selected.begin();
    DiagramItem * item = QCanvasItemToDiagramItem(*it);
    bool in = FALSE;
    bool out = FALSE;

    item->delete_available(in, out);
    if (in || out)
      item->remove(in_model & in);		// modify selection()
    else
      the_canvas()->unselect(*it);
  }
  
  window()->package_modified();
}
#ifdef WIN32
#pragma optimize("", on)
#endif

void DiagramView::alignLeft() {
  const QCanvasItemList selected = selection();
  QCanvasItemList::ConstIterator it = selected.begin();
  double ref = (*it)->x();
  
  while ((++it)  != selected.end())
    (*it)->moveBy(ref - (*it)->x(), 0);
}

void DiagramView::alignRight() {
  const QCanvasItemList selected = selection();
  QCanvasItemList::ConstIterator it = selected.begin();
  double ref = (*it)->boundingRectAdvanced().right();
  
  while ((++it)  != selected.end())
    (*it)->moveBy(ref - (*it)->boundingRectAdvanced().right(), 0);
  
  window()->package_modified();
}

void DiagramView::alignTop() {
  const QCanvasItemList selected = selection();
  QCanvasItemList::ConstIterator it = selected.begin();
  double ref = (*it)->y();
  
  while ((++it)  != selected.end())
    (*it)->moveBy(0, ref - (*it)->y());
}

void DiagramView::alignBottom() {
  const QCanvasItemList selected = selection();
  QCanvasItemList::ConstIterator it = selected.begin();
  double ref = (*it)->boundingRectAdvanced().bottom();
  
  while ((++it)  != selected.end())
    (*it)->moveBy(0, ref - (*it)->boundingRectAdvanced().bottom());
  
  window()->package_modified();
}

void DiagramView::alignHorizontaly() {
  const QCanvasItemList selected = selection();
  QCanvasItemList::ConstIterator it = selected.begin();
  QRect r = (*it)->boundingRectAdvanced();
  double ref = (r.top() + r.bottom())/2;
  
  while ((++it)  != selected.end()) {
    r = (*it)->boundingRectAdvanced();
    
    (*it)->moveBy(0, ref - (r.top() + r.bottom())/2);
  }
  
  window()->package_modified();
}

void DiagramView::alignVerticaly() {
  const QCanvasItemList selected = selection();
  QCanvasItemList::ConstIterator it = selected.begin();
  QRect r = (*it)->boundingRectAdvanced();
  double ref = (r.left() + r.right())/2;
  
  while ((++it)  != selected.end()) {
    r = (*it)->boundingRectAdvanced();
    
    (*it)->moveBy(ref - (r.left() + r.right())/2, 0);
  }
  
  window()->package_modified();
}

void DiagramView::alignCenter() {
  const QCanvasItemList selected = selection();
  QCanvasItemList::ConstIterator it = selected.begin();
  QRect r = (*it)->boundingRectAdvanced();
  double refx = (r.left() + r.right())/2;
  double refy = (r.top() + r.bottom())/2;
  
  while ((++it)  != selected.end()) {
    r = (*it)->boundingRectAdvanced();
    
    (*it)->moveBy(refx - (r.left() + r.right())/2,
		  refy - (r.top() + r.bottom())/2);
  }
  
  window()->package_modified();
}


void DiagramView::multiple_selection_menu(bool in_model, bool out_model,
					  bool alignable,
					  QList<DiagramItem> & l) {
  const QCanvasItemList selected = selection();
  QCanvasItemList::ConstIterator it;
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Multiple selection menu", m.font()), -1);
  m.insertSeparator();
  for (it = selected.begin(); it != selected.end(); ++it) {
    if (QCanvasItemToDiagramItem(*it)->linked()) {
      m.insertItem("Select linked items", 1);
      m.insertSeparator();
      break;
    }
  }
  
  QPopupMenu al(0);
  
  if (alignable) {
    QPixmap top((const char **) align_top);
    QPixmap bottom((const char **) align_bottom);
    QPixmap left((const char **) align_left);
    QPixmap right((const char **) align_right);
    QPixmap center((const char **) align_center);
    QPixmap vcenter((const char **) align_vcenter);
    QPixmap hcenter((const char **) align_hcenter);
    
    al.insertItem(top, "align top", 4);
    al.insertItem(bottom, "align bottom", 5);
    al.insertItem(left, "align left", 6);
    al.insertItem(right, "align right", 7);
    al.insertItem(center, "align center", 8);
    al.insertItem(vcenter, "align center verticaly", 9);
    al.insertItem(hcenter, "align center horizontaly", 10);

    m.insertItem("Align", &al);
    m.insertSeparator();
  }
  
  m.insertItem("Copy selected (Ctrl+c)", 11);
  
  if (out_model) {
    m.insertItem("Cut selected (Ctrl+x, remove from view)", 12);
    m.insertItem("Remove selected from view (Suppr)", 2);
  }
  if (in_model)
    m.insertItem("Delete selected (Control+d)", 3);

  if (l.count() > 1) {
    m.insertSeparator();
    m.insertItem("Edit drawing settings", 13);
  }
  
  history_protected = TRUE;
  
  switch (m.exec(QCursor::pos())) {
  case 1:
    unselect_all();
    for (it = selected.begin(); it != selected.end(); ++it)
      QCanvasItemToDiagramItem(*it)->select_associated();
    break;
  case 12:
    clipboard = window()->copy_selected();
    copied_from = window()->browser_diagram()->get_type();
    // no break !
  case 2:
    delete_them(FALSE);
    break;
  case 3:
    delete_them(TRUE);
    break;
  case 4:
    history_save();
    alignTop();
    break;
  case 5:
    history_save();
    alignBottom();
    break;
  case 6:
    history_save();
    alignLeft();
    break;
  case 7:
    history_save();
    alignRight();
    break;
  case 8:
    history_save();
    alignCenter();
    break;
  case 9:
    history_save();
    alignVerticaly();
    break;
  case 10:
    history_save();
    alignHorizontaly();
    break;
  case 11:
    clipboard = window()->copy_selected();
    copied_from = window()->browser_diagram()->get_type();
    return;
  case 13:
    history_protected = FALSE;
    l.first()->edit_drawing_settings(l);
    break;
  default:
    return;
  }
  
  canvas()->update();
  history_protected = FALSE;
}

void DiagramView::moveSelected(int dx, int dy, bool first) {
  QCanvasItemList selected = selection();
  QCanvasItemList::ConstIterator it;
  
  if (first) {
    for (it = selected.begin(); it != selected.end(); ++it)
      QCanvasItemToDiagramItem(*it)
	->prepare_for_move(do_resize != NoCorner);
    
    selected = selection();    
  }
  
  for (it = selected.begin(); it != selected.end(); ++it) {
    DiagramItem * di = QCanvasItemToDiagramItem(*it);
    
    if (do_resize)
      di->resize(do_resize, dx, dy);
    else
      (*it)->moveBy(dx, dy);
  }
  
  window()->package_modified();
}

void DiagramView::keyPressEvent(QKeyEvent * e) {
  if (draw_line) {
    abort_line_construction();
  }
  else if (!window()->frozen()) {
    QString s = Shortcut::shortcut(e->key(), e->state());
  
    if (!s.isEmpty()) {
      if (s == "Move left") {
	history_protected = TRUE;
	if (first_move) {
	  history_save();
	  first_move = FALSE;
	  moveSelected(-1, 0, TRUE);
	}
	else
	  moveSelected(-1, 0, FALSE);
      }
      else if (s == "Move right") {
	history_protected = TRUE;
	if (first_move) {
	  history_save();
	  first_move = FALSE;
	  moveSelected(1, 0, TRUE);
	}
	else
	  moveSelected(1, 0, FALSE);
      }
      else if (s == "Move up") {
	history_protected = TRUE;
	if (first_move) {
	  history_save();
	  first_move = FALSE;
	  moveSelected(0, -1, TRUE);
	}
	else
	  moveSelected(0, -1, FALSE);
      }
      else if (s == "Move down") {
	history_protected = TRUE;
	if (first_move) {
	  history_save();
	  first_move = FALSE;
	  moveSelected(0, 1, TRUE);
	}
	else
	  moveSelected(0, 1, FALSE);
      }
      else if (s == "Delete") {
	history_protected = FALSE;
	delete_them(TRUE);
      }
      else if (s == "Remove from view") {
	history_protected = TRUE;
	delete_them(FALSE);
      }
      else if (s == "Select all") {
	history_protected = TRUE;
	select_all();	// clear history_protected
	e->ignore();
	return;
      }
      else if (s == "Copy") {
	history_protected = TRUE;
	clipboard = window()->copy_selected();
	copied_from = window()->browser_diagram()->get_type();
      }
      else if (s == "Paste") {
	history_protected = TRUE;
	if (!clipboard.isEmpty() &&
	    (copied_from == window()->browser_diagram()->get_type()))
	  paste();	
      }
      else if (s == "Cut") {
	history_protected = TRUE;
	clipboard = window()->copy_selected();
	copied_from = window()->browser_diagram()->get_type();
	delete_them(FALSE);
      }
      else if (s == "Undo") {
	history_protected = TRUE;
	if (available_undo())
	  undo();
	else
	  QApplication::beep();
	e->ignore();
	return;
      }
      else if (s == "Redo") {
	history_protected = TRUE;
	if (available_redo())
	  redo();
	else
	  QApplication::beep();
	e->ignore();
	return;
      }
      else if (s == "Save") {
	history_protected = TRUE;
	UmlWindow::save_it();
      }
      else if (s == "Arrow geometry") {
	history_protected = TRUE;

	const QCanvasItemList selected = selection();
	QCanvasItemList::ConstIterator it;
	
	for (it = selected.begin(); it != selected.end(); ++it) {
	  if (isa_arrow(*it)) {
	    unselect_all();
	    // warning : the selected arrow may disapear =>
	    // select the returned arrow still present
	    // this allows to do several control-L
	    select(((ArrowCanvas *) *it)->next_geometry());
	    break;
	  }
	}
      }
      else {
	const QCanvasItemList selected = selection();

	if (selected.count() > 1) {
	  if (s == "Select linked items") {
	    history_protected = TRUE;
	    unselect_all();
	    
	    QCanvasItemList::ConstIterator it;
	    
	    for (it = selected.begin(); it != selected.end(); ++it)
	      QCanvasItemToDiagramItem(*it)->select_associated();
	  }
	  else if (s == "Edit drawing settings") {
	    QCanvasItemList::ConstIterator it;
	    UmlCode k = UmlCodeSup;
	    QList<DiagramItem> l;
	
	    for (it = selected.begin(); it != selected.end(); ++it) {
	      if (! isa_label(*it)) {
		DiagramItem * item = QCanvasItemToDiagramItem(*it);
		
		if (item->has_drawing_settings()) {
		  // note : relations doesn't have drawing setting, transition and flow have
		  switch (k) {
		  case UmlCodeSup:
		    // first case
		    k = item->type();
		    l.append(item);
		    break;
		  case UmlArrowPoint:
		    // mark for several types
		    break;
		  default:
		    if (item->type() == k)
		      l.append(item);
		    else {
		      // several types
		      l.clear();
		      k = UmlArrowPoint;	// mark for several types
		    }
		  }
		}
	      }
	    }
	    
	    switch (l.count()) {
	    case 0:
	      break;
	    case 1:
	      history_protected = FALSE;
	      l.first()->apply_shortcut(s);
	      break;
	    default:
	      history_protected = FALSE;
	      l.first()->edit_drawing_settings(l);
	    }
	  }
	  else if (s == "Align bottom") {
	    history_protected = TRUE;
	    history_save();
	    alignBottom();
	  }
	  else if (s == "Align center") {
	    history_protected = TRUE;
	    history_save();
	    alignCenter();
	  }
	  else if (s == "Align center horizontaly") {
	    history_protected = TRUE;
	    history_save();
	    alignHorizontaly();
	  }
	  else if (s == "Align center verticaly") {
	    history_protected = TRUE;
	    history_save();
	    alignVerticaly();
	  }
	  else if (s == "Align left") {
	    history_protected = TRUE;
	    history_save();
	    alignLeft();
	  }
	  else if (s == "Align right") {
	    history_protected = TRUE;
	    history_save();
	    alignRight();
	  }
	  else if (s == "Align top") {
	    history_protected = TRUE;
	    history_save();
	    alignTop();
	  }
	}
	else if (selected .count() == 1) {
	  DiagramItem * item = 
	    QCanvasItemToDiagramItem(selected.first());
	  
	  if (item != 0) {
	    if (s == "Select linked items") {
	      history_protected = TRUE;
	      unselect_all();
	      item->select_associated();
	    }
	    else {
	      history_protected = FALSE;
	      item->apply_shortcut(s);
	    }
	  }
	}
      }
      canvas()->update();
      history_protected = FALSE;
    }
  }  
  e->ignore();
}

void DiagramView::keyReleaseEvent(QKeyEvent *) {
  first_move = FALSE;
}

void DiagramView::select_all() {
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator cit;

  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((QCanvasItemToDiagramItem(*cit) != 0) && // an uml canvas item
	!(*cit)->selected() &&
	(*cit)->visible())
      select(*cit);
  }
  
  history_protected = TRUE;
  canvas()->update();
  history_protected = FALSE;
}

void DiagramView::set_zoom(double zoom) {
  ((UmlCanvas *) canvas())->set_zoom(zoom);
  
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator cit;

  // hack to freeze arrow's labels position : select all
  QCanvasItemList selected = selection();
  
  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((QCanvasItemToDiagramItem(*cit) != 0) && // an uml canvas item
	!(*cit)->selected() &&
	(*cit)->visible())
      select(*cit);
  }
  
  DiagramItem * di;
  
  for (cit = all.begin(); cit != all.end(); ++cit)
    if ((*cit)->visible() && 
	!isa_arrow(*cit) &&
	((di = QCanvasItemToDiagramItem(*cit)) != 0))
      di->change_scale();
  
  for (cit = all.begin(); cit != all.end(); ++cit)
    if ((*cit)->visible() && isa_arrow(*cit))
      ((ArrowCanvas *) *cit)->change_scale();
  
  // hack end
  unselect_all();
  for (cit = selected.begin(); cit != selected.end(); ++cit)
    select(*cit);
  
  ((UmlCanvas *) canvas())->zoom_end();
  
  canvas()->update();
}

void DiagramView::needed_width_height(int & maxx, int & maxy) const {
  // search the max used x and y
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator cit;
  
  maxx = 0;
  maxy = 0;
  
  for (cit = all.begin(); cit != all.end(); ++cit) {
    DiagramItem * di;
    
    if ((*cit)->visible() && 
	((di = QCanvasItemToDiagramItem(*cit)) != 0)) {
      if (di->type() != UmlLifeLine) {
	QRect r = (*cit)->boundingRect();
	
	if (r.right() > maxx)
	  maxx = r.right();
	if (r.bottom() > maxy)
	  maxy = r.bottom();
      }
    }
  }
}

void DiagramView::fit_scale() {
  int maxx;
  int maxy;
  
  needed_width_height(maxx, maxy);
  
  double scale = the_canvas()->zoom() * visibleWidth() / maxx;
  double scale_y = the_canvas()->zoom() * visibleHeight() / maxy;
  
  if (scale > scale_y)
    scale = scale_y;
  
  scale -= 0.02;
  
  set_zoom((scale < SCALE_MIN/100.0) 
	   ? SCALE_MIN/100.0
	   : ((scale > SCALE_MAX/100.0) ? SCALE_MAX/100.0 : scale));
}

void DiagramView::do_optimal_window_size() {
  history_protected = TRUE;
  optimal_window_size();
  canvas()->update();
  // do it an other time because of the scroll bars
#ifdef WIN32
  QTimer::singleShot(1, this, SLOT(optimal_window_size()));
#else
  optimal_window_size();
#endif
}

void DiagramView::optimal_window_size() {
  int maxx;
  int maxy;
  
  needed_width_height(maxx, maxy);
  
  window()->resize(maxx + 10 + window()->width() - visibleWidth(),
		   maxy + 10 + window()->height() - visibleHeight());
}

void DiagramView::preferred_size_zoom() {
  if (preferred_zoom != 0) {
    window()->resize(preferred_size);
    window()->new_scale((int) (preferred_zoom * 100));
  }
}

void DiagramView::set_format(int f) {
  if (f != (int) window()->browser_diagram()->get_format()) {
    window()->browser_diagram()->set_format((CanvasFormat) f);
    the_canvas()->resize((CanvasFormat) f);
  }
}

int DiagramView::default_menu(QPopupMenu & m, int f) {
  QPopupMenu formatm(0);
  
  m.insertItem("Edit drawing settings", EDIT_DRAWING_SETTING_CMD);
  m.insertSeparator();
  m.insertItem("Select diagram in browser", 1);
  m.insertItem("Select all (Ctrl+a)", 2);
  m.insertSeparator();
  m.insertItem("Optimal scale", 7);
  m.insertItem("Optimal window size", 8);
  m.insertSeparator();
  m.insertItem("Copy optimal picture part", 13);
  m.insertItem("Copy visible picture part", 3);
  m.insertItem("Save optimal picture part", 14);
  m.insertItem("Save visible picture part", 10);
  if (!clipboard.isEmpty() &&
      (copied_from == window()->browser_diagram()->get_type()))
    m.insertItem("Paste copied items (Ctrl+v)", 9);
  m.insertSeparator();
  m.insertItem("Set preferred size and scale", 4);
  if (preferred_zoom != 0)
    m.insertItem("Restore preferred size and scale", 5);
  init_format_menu(formatm, f);
  m.insertItem("Format", &formatm);
  m.insertSeparator();
  m.insertItem("Undo all changes", RELOAD_CMD);
  if (available_undo())
    m.insertItem("Undo (Ctrl+z or Ctrl+u)", 11);
  if (available_redo())
    m.insertItem("Redo (Ctrl+y or Ctrl+r)", 12);

  int choice = m.exec(QCursor::pos());
  
  switch (choice) {
  case 1:
    history_protected = TRUE;
    the_canvas()->browser_diagram()->select_in_browser();
    break;
  case 2:
    select_all();
    return choice;	// clear history_protected
  case 3:
    history_protected = TRUE;
    copy_in_clipboard(FALSE, FALSE);
    break;
  case 13:
    history_protected = TRUE;
    copy_in_clipboard(TRUE, FALSE);
    break;
  case 4:
    history_protected = TRUE;
    preferred_zoom = the_canvas()->zoom();
    preferred_size.setWidth(window()->width());
    preferred_size.setHeight(window()->height());
    window()->package_modified();
    break;
  case 5:
    preferred_size_zoom();
    break;
  case 7:
    window()->fit_scale();
    break;
  case 8:
    do_optimal_window_size();
    break;
  case 9:
    history_protected = TRUE;
    history_save();
    paste();
    break;
  case 10:
    history_protected = TRUE;
    save_picture(FALSE);
    break;
  case 14:
    history_protected = TRUE;
    save_picture(TRUE);
    break;
  case 11:
    undo();
    break;
  case 12:
    redo();
    break;
  default:
    if (choice >= f) {
      set_format(choice - f);
      window()->package_modified();
    }
  }
  
  return choice;
}

void DiagramView::init_format_menu(QPopupMenu & m, int f) const {
  m.setCheckable(TRUE);
  
  for (int i = 0; i != CanvasFormatSup; i += 1)
    m.insertItem(stringify((CanvasFormat) i), f + i);
  
  m.setItemChecked(f + (int) window()->browser_diagram()->get_format(),
		   TRUE);
}

void DiagramView::load(const char * pfix) {
  // for a re-load :
  unselect_all();
  preferred_zoom = 0;
  
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator it;
  DiagramItem * di;
  
  for (it = all.begin(); it != all.end(); ++it)
    if (((di = QCanvasItemToDiagramItem(*it)) != 0) && // an uml canvas item
	(*it)->visible())
      di->delete_it();
  
  ((UmlCanvas *) canvas())->set_zoom(1);
  ((UmlCanvas *) canvas())->zoom_end();
  
  // load
  BrowserNode::pre_load();
  
  try {
    QString t = pfix;
    
    t += " diagram " + ((QWidget *) parent())->caption();
    
    read_in(t);
    read();
  }
  catch (int) {
    ;
  }
  
  BrowserNode::post_load();
  DiagramItem::post_load();
  
  // to managed deleted items present in the browser
  all = canvas()->allItems();  
  
  for (it = all.begin(); it != all.end(); ++it) {
    if ((*it)->visible() &&
	((di = QCanvasItemToDiagramItem(*it)) != 0) && 
	(di->get_bn() != 0) && // do not manage relations here
	di->get_bn()->deletedp())
      di->delete_it();
  }
  
  if (window()->get_view() != 0)
    // re-load
    preferred_size_zoom();
  canvas()->update();
}

void DiagramView::read() {
  char * s;
  
  if ((s = read_definition(id, "diagram")) != 0) {
    unsigned format = read_file_format();
    bool error = FALSE;
    
    try {
      char * st = s;
      char * k;
      
      k = read_keyword(st);
      if (!strcmp(k, "format")) {
	set_read_file_format(read_unsigned(st));
	k = read_keyword(st);
      }
      else
	// format saved in diagram since format 4
	set_read_file_format(3);
      
      the_canvas()->read_old_diagram(read_file_format() < 9);
      
      read(st, k);
    }
    catch (...) {
      error = TRUE;
    }
    
    delete [] s;
    the_canvas()->read_old_diagram(FALSE);
    set_read_file_format(format);
    the_canvas()->clear_old_ids();
    
    if (error)
      throw 0;
  }
}

void DiagramView::paste() {
  history_protected = TRUE;
  
  double old_zoom = ((UmlCanvas *) canvas())->zoom();
  char * s = new char[clipboard.length() + 1];
  
  memcpy(s, (const char *) clipboard, clipboard.length() + 1);
  
  unselect_all();
  set_zoom(1);
  the_canvas()->set_paste(TRUE);
  read_in("clipboard");
  
  try {
    char * st = s;
    char * k;
      
    k = read_keyword(st);
    if (!strcmp(k, "format")) {
      set_read_file_format(read_unsigned(st));
      k = read_keyword(st);
    }
    else
      // format saved in diagram since format 4
      set_read_file_format(3);
      
    read(st, k);
  }
  catch (int) {
    ;
  }
  
  delete [] s;
  
  the_canvas()->clear_old_ids();
  the_canvas()->set_paste(FALSE);
  DiagramItem::post_load();
  
  // to managed deleted items present in the browser
  // compute rect containing all the paste items
  QCanvasItemList l = selection();
  QCanvasItemList::Iterator it;
  DiagramItem * di;
  double minx = 1e10;
  double maxx = 0;
  double miny = 1e10;
  double maxy = 0;
  
  for (it = l.begin(); it != l.end(); ++it) {
    if ((*it)->visible() &&
	((di = QCanvasItemToDiagramItem(*it)) != 0) && 
	(di->get_bn() != 0) && // do not manage relations here
	di->get_bn()->deletedp())
      di->delete_it();
    else {
      QRect r = (*it)->boundingRect();
      
      if (r.x() < minx)
	minx = r.x();
      if (r.y() < miny)
	miny = r.y();
      if (r.x() > maxx)
	maxx = r.right();
      if (r.y() > maxy)
	maxy = r.bottom();
    }
  }

  // place all paste items in the center of the viewport
  double dx = contentsX() + width()/2 - (minx + maxx)/2;
  double dy = contentsY() + height()/2 - (miny + maxy)/2;
  
  l = selection();
  
  for (it = l.begin(); it != l.end(); ++it)
    QCanvasItemToDiagramItem(*it)->prepare_for_move(FALSE);
  
  l = selection();
  
  for (it = l.begin(); it != l.end(); ++it)
    (*it)->moveBy(dx, dy);
  
  set_zoom(old_zoom);
  canvas()->update();
  history_protected = FALSE;
  window()->package_modified();
}

bool DiagramView::save_in(const char * f, bool optimal, bool temporary) {
  if (the_canvas()->selection().count() != 0) {
    // unselect element and redraw them to remove selection mark
    history_protected = TRUE;
    unselect_all();
    the_canvas()->setAllChanged();
    canvas()->update();
    history_protected = TRUE;
  }
  
  the_canvas()->show_limits(FALSE);
  
  bool r;
  
  if (optimal) {
    int maxx;
    int maxy;
    
    needed_width_height(maxx, maxy);
    maxx += 10;
    maxy += 10;
    
    // add a large margin to be sure to see all in one shot contrarilly
    // to the optimal_window_size whose must have the pretty size
    
    if ((visibleWidth() >= (maxx + 90)) && (visibleHeight() >= (maxy + 90)))
      r = QPixmap::grabWidget(viewport(), 0, 0, maxx, maxy).save(f, "PNG");
    else {
#ifndef WIN32
      int saved_w;
      int saved_h;
#endif
      saved_w = window()->width();
      saved_h = window()->height();
      
      window()->resize(maxx + 90, maxy + 90);
      history_protected = TRUE;
      r = QPixmap::grabWidget(viewport(), 0, 0, maxx, maxy).save(f, "PNG");

      if (! temporary) {
#ifdef WIN32
	QTimer::singleShot(1, this, SLOT(restore_window_size()));
#else
	window()->resize(saved_w, saved_h);
#endif
      }
    }
  }
  else
    r = QPixmap::grabWidget(viewport()).save(f, "PNG");
  
  the_canvas()->show_limits(TRUE);
  return r;
}

void DiagramView::copy_in_clipboard(bool optimal, bool temporary) {
  the_canvas()->show_limits(FALSE);
  
  if (optimal) {
    int maxx;
    int maxy;
    
    needed_width_height(maxx, maxy);
    maxx += 10;
    maxy += 10;
    
    // add a large margin to be sure to see all in one shot contrarilly
    // to the optimal_window_size whose must have the pretty size
    
    if ((visibleWidth() >= (maxx + 90)) && (visibleHeight() >= (maxy + 90)))
      QApplication::clipboard()
	->setPixmap(QPixmap::grabWidget(viewport(), 0, 0, maxx, maxy));
    else {
#ifndef WIN32
      int saved_w;
      int saved_h;
#endif
      saved_w = window()->width();
      saved_h = window()->height();
      
      window()->resize(maxx + 90, maxy + 90);
      history_protected = TRUE;
      canvas()->update();
      QApplication::clipboard()
	->setPixmap(QPixmap::grabWidget(viewport(), 0, 0, maxx, maxy));
      if (! temporary) {
#ifdef WIN32
	QTimer::singleShot(1, this, SLOT(restore_window_size()));
#else
	window()->resize(saved_w, saved_h);
#endif
      }
    }
  }
  else
    QApplication::clipboard()->setPixmap(QPixmap::grabWidget(viewport()));
  
  the_canvas()->show_limits(TRUE);
}

// not used out of windows, but must be defined because
// this is a 'slot'
void DiagramView::restore_window_size() {
#ifdef WIN32
  window()->resize(saved_w, saved_h);
#endif
}

void DiagramView::save_picture(bool optimal) {
  QString filename =
    QFileDialog::getSaveFileName(QString::null, "PNG file (*.png)", this);

  if (!filename.isNull()) {
    if (filename.right(4).lower() != ".png")
      filename += ".png";
  
    save_in(filename, optimal, FALSE);
  }
}

void DiagramView::print(QPainter * p) {
  if (the_canvas()->selection().count() != 0)
    unselect_all();

  double old_zoom = ((UmlCanvas *) canvas())->zoom();
  
  if (old_zoom < 1)
    set_zoom(1);
  
  QPaintDeviceMetrics m(p->device());
  int devh = m.height();
  int devw = m.width();
  int h = contentsHeight();
  int w = contentsWidth();
  double zoom = ((double) devh)/h;

  if (((double) devw)/w < zoom)
    zoom = ((double) devw)/w;

  zoom  *= 0.95;
  p->scale(zoom, zoom);
  ((UmlCanvas *) canvas())->show_limits(FALSE);
  drawContents(p, 0, 0, w, h);
  ((UmlCanvas *) canvas())->show_limits(TRUE);
  
  if (old_zoom < 1)
    set_zoom(old_zoom);
}

void DiagramView::renumber(int ident) {
  id = ident;
}

void DiagramView::save_session(QTextStream & st) {
  st << (int) (((UmlCanvas *) canvas())->zoom() * 100)
     << ' ' << (int) window()->browser_diagram()->get_format()
     << ' ' << verticalScrollBar()->value()
     << ' ' << horizontalScrollBar()->value() << '\n';
}

void DiagramView::read_session(char * & st) {
  // zoom managed by DiagramWindow
  canvas()->update();
  set_format((int) read_unsigned(st));
  {
    extern QApplication * theApp;
    theApp->processEvents(/*500*/);
  }
  verticalScrollBar()->setValue(read_unsigned(st));
  horizontalScrollBar()->setValue(read_unsigned(st));
  canvas()->update();
  {
    extern QApplication * theApp;
    theApp->processEvents(/*500*/);
  }
}

//history
//
// A new history element is add at the beginning of an action undoable
// A history element is not added if it is equel to the current one
// History may be empty or may heve onlt one element, in these cases
// undo & redo are not available
// history_index indicate the current state, or ~0 if the stack is empty

// save after the current state

void DiagramView::history_save(bool on_undo) {
  if (history_frozen)
    return;
  
  // get current state
  QByteArray * ba = new QByteArray();
  QBuffer b(*ba);
  
  b.open(IO_WriteOnly);

  /*
  double z = the_canvas()->zoom();
  
  b.writeBlock((char *) &z, sizeof(z));
  */
  
  DiagramItemList items(canvas()->allItems());
  DiagramItem * di;
  
  for (di = items.first(); di != 0; di = items.next())
    di->history_save(b);
  
  b.close();
  
  // update history if something changed
  if (history.isEmpty()) {
    history.append(ba);
    history_index = 0;
  }
  else {
    QByteArray * current = history.at(history_index);
    
    if (! on_undo) {
      // remove redo
      while (history.getLast() != current)
	history.removeLast();
    }
    
    if (*ba == *current) {
       // nothing new
      delete ba;
    }
    else {
      history.append(ba);
      
      // limit history depth to 20
      if (history_index == 19)
	history.removeFirst();
      else
	history_index += 1;
    }
  }
}

// load the state indicated by history_index
void DiagramView::history_load() {
  history_protected = TRUE;
  
  // unselect and hide all
  unselect_all();
  
  DiagramItemList items(canvas()->allItems());
  DiagramItem * di;
  
  for (di = items.first(); di != 0; di = items.next()) {
    di->history_hide();
    di->post_history_hide();
  }
  
  // load history
  QBuffer b(*(history.at(history_index)));
  
  b.open(IO_ReadOnly);
  
  while (!b.atEnd())
    load_item(b)->history_load(b);
  
  b.close();
  canvas()->update();
  history_protected = FALSE;
}

// clear history if not undoable
void DiagramView::update_history() {
  if (!history_protected && !history_frozen) {
    history.clear();
    history_index = ~0u;
  }
}

// want to be the state preceding the state indicated by history_index
void DiagramView::undo() {
  history_index -= 1; 
  history_load();
}

// want to be the state following the state indicated by history_index
void DiagramView::redo() {
  history_index += 1;
  history_load();
}

bool DiagramView::available_undo() {
  if (history_index == (history.count() - 1))
    history_save(TRUE);
  
  return (history_index != 0);
}

bool DiagramView::available_redo() {
  return (history_index != (history.count() - 1));
}
