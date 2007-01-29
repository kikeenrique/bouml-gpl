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

#include "ActivityContainerCanvas.h"
#include "BrowserNode.h"
#include "ArrowCanvas.h"
#include "PinCanvas.h"
#include "ParameterCanvas.h"
#include "ExpansionNodeCanvas.h"
#include "ArrowPointCanvas.h"

void ActivityContainerCanvas::force_inside(DiagramItem * di, QCanvasItem * ci) {
  QRect r = rect();
  QRect sr = di->rect();
  int dx = 0;
  int dy = 0;
  
  if (sr.left() < r.left()) {
    if (sr.right() <= r.right())
      dx = r.left() - sr.left();
  }
  else if (sr.right() > r.right())
    dx = r.right() - sr.right();
  
  if (sr.top() < r.top()) {
    if (sr.bottom() <= r.bottom())
      dy = r.top() - sr.top();
  }
  else if (sr.bottom() > r.bottom())
    dy = r.bottom() - sr.bottom();
  
  if ((dx != 0) || (dy != 0)) {
    ci->moveBy(dx, dy);
    if ((ci->z() - z()) < 2)
      ci->setZ(z() + 2);
  }
}

// only for pin & parameter & expansion node
static bool indirectly_selected(DiagramItem * di)
{
  switch (di->type()) {
  case UmlActivityPin:
    return ((PinCanvas *) di)->action_selected();
  case UmlParameter:
    return ((ParameterCanvas *) di)->activity_selected();
  case UmlExpansionNode:
    return ((ExpansionNodeCanvas *) di)->region_selected();
  default:
    return FALSE;
  }
}

void ActivityContainerCanvas::prepare_for_move(bool on_resize) {
  if (! on_resize) {
    // select all sub nodes
    QCanvasItemList all = canvas()->allItems();
    QCanvasItemList::Iterator cit;
    UmlCanvas * canvas = the_canvas();
    
    for (cit = all.begin(); cit != all.end(); ++cit) {
      if ((*cit)->visible() && !(*cit)->selected()) {
	DiagramItem * di = QCanvasItemToDiagramItem(*cit);
	
	if ((di != 0) &&
	    (di->get_bn() != 0) &&
	    (((BrowserNode *) di->get_bn()->parent()) == browser_node)) {
	  // must look at the type because some canvas items have browser_node
	  // attr set to the diagram and the parent of the diagram is the activity
	  switch (di->type()) {
	  case UmlActivityObject:
	  case UmlActivityAction:
	  case UmlPartition:
	  case InitialAN:
	  case FlowFinalAN:
	  case ActivityFinalAN:
	  case DecisionAN:
	  case MergeAN:
	  case ForkAN:
	  case JoinAN:
	  case UmlExpansionRegion:
	  case UmlInterruptibleActivityRegion:
	    canvas->select(*cit);
	    ((DiagramCanvas *) di)->prepare_for_move(on_resize);
	    break;
	  default:
	    break;
	  }
	}
      }
    }

    // select points on lines having the two extremities selected or
    // connecting pin/parameter/expansion node of element selected
    for (cit = all.begin(); cit != all.end(); ++cit) {
      if ((*cit)->visible() && !(*cit)->selected() && isa_arrow(*cit)) {
	ArrowCanvas * ar = (ArrowCanvas *) *cit;
	DiagramItem * b;
	DiagramItem * e;
	
	ar->extremities(b, e);

	if (!b->isSelected() || !e->isSelected()) {
	  DiagramItem * start = ar->get_start();
	  DiagramItem * dest = ar->get_end();

	  if ((start->isSelected() || indirectly_selected(start)) &&
	      (dest->isSelected() || indirectly_selected(dest))) {
	    while (b->type() == UmlArrowPoint) {
	      canvas->select((ArrowPointCanvas *) b);
	      ar = ((ArrowPointCanvas *) b)->get_other(ar);
	      ar->extremities(b, e);
	    }

	    ar = (ArrowCanvas *) *cit;
	    ar->extremities(b, e);

	    while (e->type() == UmlArrowPoint) {
	      canvas->select((ArrowPointCanvas *) e);
	      ar = ((ArrowPointCanvas *) e)->get_other(ar);
	      ar->extremities(b, e);
	    }
	  }
	}
      }
    }
  }
}
