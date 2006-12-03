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

#include <math.h>
#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "ArrowCanvas.h"
#include "ArrowPointCanvas.h"
#include "ArrowJunctionCanvas.h"
#include "UmlCanvas.h"
#include "LabelCanvas.h"
#include "CdClassCanvas.h"
#include "BrowserDiagram.h"
#include "StereotypeDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "DiagramView.h"

#include "geometry_hv.xpm"
#include "geometry_vh.xpm"
#include "geometry_hvh.xpm"
#include "geometry_vhv.xpm"

ArrowCanvas::ArrowCanvas(UmlCanvas * canvas, DiagramItem * b,
			 DiagramItem * e, UmlCode t, int id,
			 bool own_brk)
      : QCanvasPolygon(canvas), DiagramItem(id, canvas), begin(b), end(e),
        itstype(t), geometry(NoGeometry), fixed_geometry(FALSE),
  	label(0), stereotype(0) {
  double bz = begin->get_z();
  double ez = end->get_z();
  
  setZ(((bz < ez) ? bz : ez) - 1);
  b->add_line(this);
  e->add_line(this);
  
  // manages the case start == end
  if ((b == e) && !own_brk)
    cut_self();
  
  // the first time the canvas must not have a label nor stereotype  
  auto_pos = canvas->browser_diagram()
    ->get_auto_label_position(canvas->browser_diagram()->get_type());
  update_pos();
  
  connect(DrawingSettings::instance(), SIGNAL(changed()),
	  this, SLOT(drawing_settings_modified()));
}

ArrowCanvas::~ArrowCanvas() {
}

void ArrowCanvas::cut_self() {
  // cut the line adding two points
  QRect r = ((DiagramCanvas *) begin)->rect();
  QPoint pt(r.right() + ARROW_LENGTH*3, r.top());
  
  ArrowPointCanvas * ap = brk(pt);
  
  pt.setY(pt.y() + ARROW_LENGTH*3);
  ap->get_other(this)->brk(pt)->update_show_lines();
  ap->update_show_lines();
}

void ArrowCanvas::delete_it() {
  if (begin != 0) {
    begin->remove_line(this);
    if (begin->type() == UmlArrowPoint)
      begin->delete_it();
  }
  
  if (end != 0) {
    end->remove_line(this);
    if (end->type() == UmlArrowPoint)
      end->delete_it();
  }
  
  if (label != 0)
    the_canvas()->del(label);
  
  if (stereotype != 0)
    the_canvas()->del(stereotype);
  
  while (!lines.isEmpty())
    lines.first()->delete_it();	// will remove the line
  
  the_canvas()->del(this);
  
  disconnect();
}

UmlCanvas * ArrowCanvas::the_canvas() const {
  return ((UmlCanvas *) canvas());
}

void ArrowCanvas::update_pos() {
  hide_lines();
  
  QPoint old_beginp = beginp;
  QPoint old_endp = endp;
  
  // calcul beginp & endp pour ne plus etre dans b_rct & e_rct
  if (begin->type() == UmlArrowPoint)
    beginp = begin->center();
  else
    begin->shift(beginp, end->center());
  
  switch (end->type()) {
  case UmlArrowPoint:
  case UmlArrowJunction:
    endp = end->center();
    break;
  default:
    end->shift(endp, beginp);
  }
  
  // calcul rectangle englobant
  
  const int dx = endp.x() - beginp.x();
  const int dy = beginp.y() - endp.y();
  
  if ((dx == 0) && (dy == 0))
    return;
  
  const double m = ARROW_LENGTH/sqrt(dx*dx + dy*dy);
  
  double deltax = dy*m;
  double deltay = dx*m;
  
  // modif de delta?/1 pour placer une marge de l/1 a chaque extremite
  boundings.resize(4);
  boundings.setPoint(0, (int) (beginp.x() - deltax - deltay/1),
		     (int) (beginp.y() - deltay + deltax/1));
  boundings.setPoint(1, (int) (beginp.x() + deltax - deltay/1),
		     (int) (beginp.y() + deltay + deltax/1));
  boundings.setPoint(2, (int) (endp.x() + deltax + deltay/1),
		     (int) (endp.y() + deltay - deltax/1));
  boundings.setPoint(3, (int) (endp.x() - deltax + deltay/1),
		     (int) (endp.y() - deltay - deltax/1));
  
  setPoints(boundings);
  
  // points de la fleche
  // *3/5 pour diminuer la fleche et ne pas risquer de sortir de boundings
  deltax *= 3.0/5.0;
  deltay *= 3.0/5.0;
    
  switch (end->type()) {
  case UmlArrowPoint:
    break;
  case UmlArrowJunction:
    // note : the circle or arc is drawn by the arrow junction
    if (itstype == UmlRequired) {
      // end of line
      arrow[0].setX((int) (endp.x() - deltay*(5.0/3.0/ARROW_LENGTH*ARROW_JUNCTION_SIZE/2)));
      arrow[0].setY((int) (endp.y() + deltax*(5.0/3.0/ARROW_LENGTH*ARROW_JUNCTION_SIZE/2)));
      // start angle*16
      arrow[1].setX((dx == 0)
		    ? ((dy > 0) ? 180*16 : 0*16 )
		    : ((((int) (atan(float(dy)/dx)*(180/3.1415927*16)))+
			((dx > 0) ? 90*16 : 270*16))
		       % (360*16)));
      // arc len*16
      arrow[1].setY(180*16);
    }
    else {
      // end of line
      arrow[0].setX((int) (endp.x() - deltay*(5.0/3.0/ARROW_LENGTH*PROVIDED_RADIUS)));
      arrow[0].setY((int) (endp.y() + deltax*(5.0/3.0/ARROW_LENGTH*PROVIDED_RADIUS)));
    }
    break;
  default:
    arrow[0].setX((int) (endp.x() - deltax - deltay));
    arrow[0].setY((int) (endp.y() - deltay + deltax));
    arrow[1].setX((int) (endp.x() + deltax - deltay));
    arrow[1].setY((int) (endp.y() + deltay + deltax));
    arrow[2].setX((int) (endp.x() - deltay));	// pour generalize
    arrow[2].setY((int) (endp.y() + deltax));
  }
  
  if (begin->type() != UmlArrowPoint) {  
    // aggregation's polygone
    switch (itstype) {
    case UmlAggregation:
    case UmlDirectionalAggregation:
    case UmlAggregationByValue:
    case UmlDirectionalAggregationByValue:
      {
	double d_x = (deltax + deltay); // *2.0/3.0;
	double d_y = (deltay - deltax); // *2.0/3.0;
	
	poly.resize(4);
	poly.setPoint(0, beginp.x(), beginp.y());
	poly.setPoint(1, (int) (beginp.x() + d_y), (int) (beginp.y() - d_x));
	poly.setPoint(2, (int) (beginp.x() + d_y + d_x), (int) (beginp.y() - d_x + d_y));
	poly.setPoint(3, (int) (beginp.x() + d_x), (int) (beginp.y() + d_y));
      }
      break;
    default:	// to avoid compiler warning
      break;
    }
  }
  
  if (auto_pos) {
    // moves label dependent on old position
    if ((label != 0) && !label->selected())
      label->moveBy((beginp.x() - old_beginp.x() + endp.x() - old_endp.x())/2.0,
		    (beginp.y() - old_beginp.y() + endp.y() - old_endp.y())/2.0);
    
    // moves stereotype dependent on old position
    if ((stereotype != 0) && !stereotype->selected())
      stereotype->moveBy((beginp.x() - old_beginp.x() + endp.x() - old_endp.x())/2.0,
			 (beginp.y() - old_beginp.y() + endp.y() - old_endp.y())/2.0);
  }
  
  update_show_lines();
}

// force all the arrow points to be ouside r
void ArrowCanvas::move_outside(QRect r) {
  QPoint c = r.center();
  ArrowCanvas * ar = this;
  
  while (ar->end->type() == UmlArrowPoint) {
    ArrowPointCanvas * p = (ArrowPointCanvas *) ar->end;
    double px = p->x();
    double py = p->y();
    
    double dx = (px < c.x()) ? -10.0 : 10.0;
    double dy = (py < c.y()) ? -10.0 : 10.0;
    
    while (r.contains((int) px, (int) py)) {
      px += dx;
      py += dy;
    }
    
    p->move(px, py);
    
    ar = p->get_other(ar);
  }
}

void ArrowCanvas::change_scale() {
  // all the extremities and the label&stereotype are already moved
  LabelCanvas * lab = label;
  LabelCanvas * ste = stereotype;
  
  label = 0;
  stereotype = 0;
  QCanvasPolygon::setVisible(FALSE);
  update_pos();
  QCanvasPolygon::setVisible(TRUE);
  label = lab;
  stereotype = ste;
}

void ArrowCanvas::setVisible(bool yes) {
  QCanvasPolygon::setVisible(yes);
  if (label != 0)
    label->setVisible(yes);
  if (stereotype != 0)
    stereotype->setVisible(yes);
}

void ArrowCanvas::moveBy(double dx, double dy) {
  if ((label != 0) && !label->selected())
    label->moveBy(dx, dy);
  if ((stereotype != 0) && !stereotype->selected())
    stereotype->moveBy(dx, dy);
}

double ArrowCanvas::get_z() const {
  return z();
}

void ArrowCanvas::set_z(double z) {
  setZ(z);
  if (stereotype != 0)
    stereotype->setZ(z);
}

bool ArrowCanvas::copyable() const {
  return selected() && get_start()->copyable() && get_end()->copyable();    
}

void ArrowCanvas::drawShape(QPainter & p) {
  if (! visible()) return;
  
  p.setBackgroundMode(QObject::OpaqueMode);
  
  switch (itstype) {
  case UmlDirectionalAggregation:
  case UmlDirectionalAggregationByValue:
  case UmlDirectionalAssociation:
  case UmlContain:
  case UmlTransition:
  case UmlFlow:
    if (end->type() != UmlArrowPoint) {
      p.drawLine(endp, arrow[0]);
      p.drawLine(endp, arrow[1]);
    }
    // no break
  case UmlAssociation:
  case UmlAggregation:
  case UmlAggregationByValue:
  case UmlLink:
  case UmlObjectLink:
    p.drawLine(beginp, endp);
    if (begin->type() != UmlArrowPoint) {
      switch (itstype) {
      case UmlAggregation:
      case UmlDirectionalAggregation:
	{
	  QBrush brsh = p.brush();
	  
	  p.setBrush(white);
	  p.drawPolygon(poly/*, TRUE*/);
	  p.setBrush(brsh);
	}
	break;
      case UmlAggregationByValue:
      case UmlDirectionalAggregationByValue:
	{
	  QBrush brsh = p.brush();
	  
	  p.setBrush(black);
	  p.drawPolygon(poly/*, TRUE*/);
	  p.setBrush(brsh);
	}      
	break;
      default:	// to avoid compiler warning
	break;
      }
    }
    break;
  case UmlDependency:
  case UmlDependOn:
    if (end->type() != UmlArrowPoint) {
      p.drawLine(endp, arrow[0]);
      p.drawLine(endp, arrow[1]);
    }
    p.setPen(QObject::DotLine);
    p.drawLine(beginp, endp);
    p.setPen(QObject::SolidLine);
    break;
  case UmlRealize:
    p.setPen(QObject::DotLine);
    // no break
  case UmlGeneralisation:
  case UmlInherit:
    if (end->type() != UmlArrowPoint) {
      p.drawLine(beginp, arrow[2]);
      p.setPen(QObject::SolidLine);
      p.drawLine(endp, arrow[0]);
      p.drawLine(endp, arrow[1]);
      p.drawLine(arrow[0], arrow[1]);
    }
    else {
      p.drawLine(beginp, endp);
      p.setPen(QObject::SolidLine);
    }
    break;
  case UmlAnchor:
    p.setPen(QObject::DotLine);
    p.drawLine(beginp, endp);
    p.setPen(QObject::SolidLine);
    break;
  case UmlRequired:
  case UmlProvided:
    if (end->type() != UmlArrowPoint)
      p.drawLine(beginp, arrow[0]);
    else
      p.drawLine(beginp, endp);
    break;
  default:	// to avoid compiler warning
    break;
  }
  
  if (selected()) {
    p.fillRect(beginp.x() - SELECT_SQUARE_SIZE/2,
	       beginp.y() - SELECT_SQUARE_SIZE/2,
	       SELECT_SQUARE_SIZE, SELECT_SQUARE_SIZE, black);
    p.fillRect(endp.x() - SELECT_SQUARE_SIZE/2,
	       endp.y() - SELECT_SQUARE_SIZE/2,
	       SELECT_SQUARE_SIZE, SELECT_SQUARE_SIZE, black);
  }
}

UmlCode ArrowCanvas::type() const {
  return itstype;
}

int ArrowCanvas::rtti() const {
  return RTTI_ARROW;
}

BasicData * ArrowCanvas::get_data() const {
  // must not be used at this level
  return 0;
}

void ArrowCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

QRect ArrowCanvas::rect() const {
  QRect r(beginp, endp);
  
  return r.normalize();
}

QPoint ArrowCanvas::center() const {
  return QPoint((beginp.x() + endp.x())/2,
		(beginp.y() + endp.y())/2);
}

bool ArrowCanvas::isSelected() const {
  return selected();
}

bool ArrowCanvas::contains(int, int) const {
  return FALSE;
}

const char * ArrowCanvas::may_start(UmlCode & l) const {
  if (itstype == UmlAnchor)
    return "can't add an anchor to an anchor";
  else if (l != UmlAnchor)
    return "a relation can't have a relation";
  else
    return 0;
}

const char * ArrowCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlClass:
  case UmlNote:
  case UmlInfo:
  case UmlIcon:
    return (l == UmlAnchor) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

void ArrowCanvas::connexion(UmlCode action, DiagramItem * dest,
			    const QPoint &, const QPoint &) {
  ArrowCanvas * a =
    new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
  a->modified();
}

bool ArrowCanvas::edit(const QStringList & defaults, 
		       ArrowCanvas * plabel, ArrowCanvas * pstereotype) {
  QString la;
  QString st;
  
  if (plabel)
    la = plabel->label->get_name();
  if (pstereotype)
    st = pstereotype->stereotype->get_name();
  
  StereotypeDialog d(defaults, st, la);
  
  d.raise();
  if (d.exec() == QDialog::Accepted) {
    bool result = FALSE;
    
    if (la.isEmpty()) {
      if (plabel != 0) {
	// removes it
	the_canvas()->del(plabel->label);
	plabel->label = 0;
	result = TRUE;
      }
    }
    else {
      if (plabel == 0) {
	// adds relation's label
	label = new LabelCanvas(la, the_canvas(), 0, 0);
	default_label_position();	    
	label->show();
	result = TRUE;
      }
      else if ((plabel != 0) &&
	       (plabel->label->get_name() != la)) {
	// update name
	plabel->label->set_name(la);
	plabel->default_label_position();
	result = TRUE;
      }
    }
    
    if (st.isEmpty()) {
      if (pstereotype != 0) {
	// removes it
	the_canvas()->del(pstereotype->stereotype);
	pstereotype->stereotype = 0;
	result = TRUE;
      }
    }
    else {
      if (pstereotype == 0) {
	// adds relation's stereotype
	stereotype = new LabelCanvas(st, the_canvas(), 0, 0);
	default_stereotype_position();	    
	stereotype->show();
	result = TRUE;
      }
      else if ((pstereotype != 0) &&
	       (pstereotype->stereotype->get_name() != st)) {
	// update name
	pstereotype->stereotype->set_name(st);
	pstereotype->default_stereotype_position();
	result = TRUE;
      }
    }
    
    return result;
  }
  else
    return FALSE;
}

void ArrowCanvas::open() {
  if (IsaRelation(itstype)) {
    ArrowCanvas * plabel;
    ArrowCanvas * pstereotype;
    
    search_supports(plabel, pstereotype);
    
    BrowserNode * bn = get_start()->get_bn();

    if ((bn != 0) &&
	edit(bn->default_stereotypes(itstype), plabel, pstereotype)) {
      canvas()->update();
      package_modified();
    }
  }
}

void ArrowCanvas::default_label_position() const {
  QPoint c = center();
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  
  label->move(c.x() - fm.width(label->get_name())/2, c.y() - fm.height());
}

void ArrowCanvas::default_stereotype_position() const {
  QPoint c = center();
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  
  stereotype->move(c.x() - fm.width(stereotype->get_name())/2, c.y());
}

void ArrowCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu geo(0);
  ArrowCanvas * plabel;
  ArrowCanvas * pstereotype;
    
  search_supports(plabel, pstereotype);
    
  m.insertItem(new MenuTitle(((plabel == 0) ||
			      plabel->label->get_name().isEmpty())
			     ? QString("line") : plabel->label->get_name(),
			     m.font()), -1);
  if (IsaRelation(itstype)) {
    m.insertSeparator();
    m.insertItem("Edit",1);
  }
  
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
  m.insertItem("Remove from view", 6);
  
  int choice = m.exec(QCursor::pos());
  
  switch (choice) {
  case 1:
    open();
    return;
  case 2:
    the_canvas()->unselect_all();
    if (plabel != 0)
      the_canvas()->select(plabel->label);
    if (pstereotype)
      the_canvas()->select(pstereotype->stereotype);
    return;
  case 3:
    if (plabel != 0)
      plabel->default_label_position();
    if (pstereotype)
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
    delete_it();
    break;
  default:
    if (choice >= 10) {
      choice -= 10;
      if (choice != (int) geometry)
	set_geometry((LineGeometry) choice, TRUE);
      else
	return;
    }
    else
      return;
    break;
  }
  
  package_modified();
}

void ArrowCanvas::init_geometry_menu(QPopupMenu & m, int first) {
  QPixmap hv((const char **) geometry_hv);
  QPixmap vh((const char **) geometry_vh);
  QPixmap hvh((const char **) geometry_hvh);
  QPixmap vhv((const char **) geometry_vhv);

  m.setCheckable(TRUE);
  
  m.insertItem("None", first);
  m.insertItem(hv, first + HVGeometry);
  m.insertItem(vh, first + VHGeometry);
  m.insertItem(hvh, first + HVHGeometry);
  m.insertItem(vhv, first + VHVGeometry);
  
  switch (geometry) {
  case NoGeometry:
    m.setItemChecked(first, TRUE);
    break;
  case HVGeometry:
  case HVrGeometry:
    m.setItemChecked(first + HVGeometry, TRUE);
    break;
  case VHGeometry:
  case VHrGeometry:
    m.setItemChecked(first + VHGeometry, TRUE);
    break;
  case HVHGeometry:
    m.setItemChecked(first + HVHGeometry, TRUE);
    break;
  default:
    // VHVGeometry
    m.setItemChecked(first + VHVGeometry, TRUE);
  }
}

DiagramItem * ArrowCanvas::get_start() const {
  const ArrowCanvas * a = this;
  
  while (a->begin->type() == UmlArrowPoint)
    a = ((ArrowPointCanvas *) a->begin)->get_other(a);
  
  return a->begin;
}

DiagramItem * ArrowCanvas::get_end() const {
  const ArrowCanvas * a = this;
  
  while (a->end->type() == UmlArrowPoint)
    a = ((ArrowPointCanvas *) a->end)->get_other(a);
  
  return a->end;
}

void ArrowCanvas::extremities(DiagramItem *& b, DiagramItem *& e) const {
  b = begin;
  e = end;
}
// search label & stereotype supports
void ArrowCanvas::search_supports(ArrowCanvas *& plabel, 
				  ArrowCanvas *& pstereotype) const {
  plabel = 0;
  pstereotype = 0;
  
  const ArrowCanvas * p;
  
  p = this;
  while (p->end->type() == UmlArrowPoint) {
    if (p->label != 0) plabel = (ArrowCanvas *) p;
    if (p->stereotype != 0) pstereotype = (ArrowCanvas *) p;
    p = ((ArrowPointCanvas *) p->end)->get_other(p);
  }

  if (p->label != 0) plabel = (ArrowCanvas *) p;
  if (p->stereotype != 0) pstereotype = (ArrowCanvas *) p;

  p = this;
  while (p->begin->type() == UmlArrowPoint) {
    if (p->label != 0) plabel = (ArrowCanvas *) p;
    if (p->stereotype != 0) pstereotype = (ArrowCanvas *) p;
    p = ((ArrowPointCanvas *) p->begin)->get_other(p);
  }
  
  if (p->label != 0) plabel = (ArrowCanvas *) p;
  if (p->stereotype != 0) pstereotype = (ArrowCanvas *) p;
}

ArrowPointCanvas * ArrowCanvas::brk(const QPoint & p) {
  ArrowPointCanvas * ap =
    new ArrowPointCanvas(the_canvas(), p.x(), p.y());
  
  ap->setZ(z() + 1);	// + 1 else point can't be selected
  
  ArrowCanvas * other =
    new ArrowCanvas(the_canvas(), ap, end, itstype, 0, FALSE);

  ap->add_line(this);
  end->remove_line(this);
  end = ap;
  
  if ((p - beginp).manhattanLength() < (p - endp).manhattanLength()) {
    if (label != 0) {
      other->label = label;
      label = 0;
    }
    
    if (stereotype != 0) {
      other->stereotype = stereotype;
      stereotype = 0;
    }
  }
  
  ap->show();
  other->show();
  
  modified();
  other->modified();
  
  // no geometry
  propag_geometry(NoGeometry, FALSE);
  
  return ap;
}

// return false if a point can't removed
bool ArrowCanvas::may_join() const {
  if (get_start() == get_end()) {
    // self relation
    int npoint = 0;
    const ArrowCanvas * a = this;
    
    a = this;
    while (a->begin->type() == UmlArrowPoint) {
      npoint += 1;
      a = ((ArrowCanvas *) ((ArrowPointCanvas *) a->begin)->get_other(a));
    }
    
    a = this;
    while (a->end->type() == UmlArrowPoint) {
      npoint += 1;
      a = ((ArrowCanvas *) ((ArrowPointCanvas *) a->end)->get_other(a));
    }
    
    return (npoint > 2);
  }
  else
    return TRUE;
}

ArrowCanvas * ArrowCanvas::join(ArrowCanvas * other, ArrowPointCanvas * ap) {
  // has already check is join is possible (self relation must have two points)
  // ap is the removed arrow point
  if (end == ap) {
    end = other->end;
    end->add_line(this);	// add before remove in case end is
    end->remove_line(other);	// an arrow junction and not del it
    
    QList<ArrowCanvas> olines = other->lines;
    LabelCanvas * olabel = other->label;
    LabelCanvas * ostereotype = other->stereotype;
    
    other->lines.clear();
    other->begin = other->end = 0;
    other->label = 0;
    other->stereotype = 0;
    ap->remove_line(this);
    ap->remove_line(other);
    the_canvas()->del(other);
    the_canvas()->del(ap);
    hide();
    update_pos();
    show();
    
    //gets other's lines (anchor to note)
    while (! olines.isEmpty()) {
      ArrowCanvas * l = olines.take();
      
      l->hide();
      if (l->begin == other)
	l->begin = this;
      else
	l->end = this;
      add_line(l);
      l->update_pos();
      l->show();
    }
    
    //gets label and stereotype
    QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
    int h = fm.height();
    QPoint c = center();

    if (olabel) {
      ///tant que gere pas label sur n'importe qu el segment
      if (label != 0)
	label->hide();
      //tq
	
      label = olabel;
      label->move(c.x() - fm.width(label->get_name())/2, c.y() - h);
    }
    if (ostereotype) {
      stereotype = ostereotype;
      stereotype->move(c.x() - fm.width(stereotype->get_name())/2, c.y());
    }
    
    // no geometry
    propag_geometry(NoGeometry, FALSE);
    
    return this;
  }
  else 
    return other->ArrowCanvas::join(this, ap);
}

// the arrow is teh alone selected element and there is
// a mouse move event, what is must be done ?

bool ArrowCanvas::cut_on_move(ArrowPointCanvas *& ap) const {
  ap = 0;
  
  switch (geometry) {
  case NoGeometry:
    // cut the line
    return TRUE;
  case HVGeometry:
  case VHrGeometry:
  case HVrGeometry:
  case VHGeometry:
    // nothing must be done (ap == 0)
    return FALSE;
  default:
    // get any point, this will be moved
    ap =  (ArrowPointCanvas *)
      ((begin->type() == UmlArrowPoint) ? begin : end);
    return FALSE;
  }
}
  
void ArrowCanvas::propag_geometry(LineGeometry geo, bool fixed) {
  geometry = geo;
  fixed_geometry = fixed;
  
  ArrowCanvas * a = this;
  
  while (a->begin->type() == UmlArrowPoint) {
    a = ((ArrowPointCanvas *) a->begin)->get_other(a);
    a->geometry = geo;
    a->fixed_geometry = fixed;
  }
  
  a = this;
  
  while (a->end->type() == UmlArrowPoint) {
    a = ((ArrowPointCanvas *) a->end)->get_other(a);
    a->geometry = geo;
    a->fixed_geometry = fixed;
  }
}

// (get_start() != get_end()) check before

ArrowCanvas * ArrowCanvas::set_geometry(LineGeometry geo, bool fixed) {
  DiagramView * view = the_canvas()->get_view();
  
  view->history_save();
  view->freeze_history(TRUE);
  
  // first remove arrow points
  // warning : after join() 'this' can't be used
  ArrowCanvas * ar = this;
  
  while (ar->begin->type() == UmlArrowPoint)
    ar = ar->join(((ArrowPointCanvas *) ar->begin)->get_other(ar),
		  (ArrowPointCanvas *) ar->begin);
  
  while (ar->end->type() == UmlArrowPoint)
    ar = ar->join(((ArrowPointCanvas *) ar->end)->get_other(ar),
		  (ArrowPointCanvas *) ar->end);
  
  DiagramItem * a = ar->get_start();
  DiagramItem * b = ar->get_end();
  QPoint ca = a->center();
  QPoint cb = b->center();
    
  switch(geo) {
  case NoGeometry:
    // note : new geometry already propaged
    view->freeze_history(FALSE);
    view->protect_history(TRUE);
    return ar;
  case HVGeometry:
  case VHGeometry:
    if (ca.x() > cb.x())
      geo = (LineGeometry) (geo + 1);	// Xr
    break;
  default:
    break;
  }
  
  switch (geo) {
  case HVGeometry:
  case VHrGeometry:
    {
      QPoint p(cb.x() - ARROW_POINT_SIZE/2,
	       ca.y() - ARROW_POINT_SIZE/2);
      
      ar->brk(p);
    }
    break;
  case HVrGeometry:
  case VHGeometry:
    {
      QPoint p(ca.x() - ARROW_POINT_SIZE/2,
	       cb.y() - ARROW_POINT_SIZE/2);
      
      ar->brk(p);
    }
    break;
  case HVHGeometry:
    {
      QRect arect = a->rect();
      QRect brect = b->rect();
      QPoint p(((ca.x() < cb.x()) ? (arect.right() + brect.left())
				  : (arect.left() + brect.right()))/2
	       - ARROW_POINT_SIZE/2,
	       ca.y() - ARROW_POINT_SIZE/2);
      ArrowPointCanvas * ap = ar->brk(p);
      
      p.setY(cb.y() -  ARROW_POINT_SIZE/2);
      ap->get_other(ar)->brk(p);
    }
    break;
  default:
    // VHVGeometry
    {
      QRect arect = a->rect();
      QRect brect = b->rect();
      QPoint p(ca.x() - ARROW_POINT_SIZE/2,
	       ((ca.y() < cb.y()) ? (arect.bottom() + brect.top())
				  : (arect.top() + brect.bottom()))/2
	       - ARROW_POINT_SIZE/2);
      ArrowPointCanvas * ap = ar->brk(p);
      
      p.setX(cb.x() -  ARROW_POINT_SIZE/2);
      ap->get_other(ar)->brk(p);
    }
    break;
  }
  
  ar->propag_geometry(geo, fixed);
  view->freeze_history(FALSE);
  view->protect_history(TRUE);
  return ar;
}
// To move te arrow point to nx,ny but 'may be not exactly'
// to not go in an infinite loop between ArrowCanvas::update_geometry()
// and DiagramItem::update_show_lines() because when a extremity
// is moved this imply an update of the line attached to this point
// then the update of the geometry of the line, => a float computation
// problem may be dramatic

static void move_to(ArrowPointCanvas * ap, double nx, double ny) {
  nx = nx - ap->x();
  ny = ny - ap->y();

  if ((fabs(nx) > 0.01) || (fabs(ny) > 0.01))
    ap->moveBy(nx, ny);
}

void ArrowCanvas::update_geometry() {
  if (geometry == NoGeometry)
    return;
  
  DiagramItem * a = get_start();
  DiagramItem * b = get_end();
  
  if (a == b)
    return;
  
  const int half_arrow_size = ARROW_POINT_SIZE/2;

  QPoint ca = a->center();
  QPoint cb = b->center();
    
  switch (geometry) {
  case HVGeometry:
  case VHrGeometry:
    {
      ArrowPointCanvas * ap = (ArrowPointCanvas *)
	((begin == a) ? end : begin);
      
      move_to(ap, cb.x() - half_arrow_size, ca.y() - half_arrow_size);
    }
    break;
  case HVrGeometry:
  case VHGeometry:
    {
      ArrowPointCanvas * ap = (ArrowPointCanvas *)
	((begin == a) ? end : begin);

      move_to(ap, ca.x() - half_arrow_size, cb.y() - half_arrow_size);
    }
    break;
  case HVHGeometry:
    {
      const ArrowCanvas * ar = this;
  
      while (ar->begin->type() == UmlArrowPoint)
	ar = ((ArrowPointCanvas *) ar->begin)->get_other(ar);
  
      ArrowPointCanvas * ap1 = (ArrowPointCanvas *) ar->end;
      ArrowPointCanvas * ap2 =
	(ArrowPointCanvas *) ap1->get_other(ar)->end;
      
      if (fixed_geometry &&
	  !a->isSelected() &&
	  !b->isSelected() &&
	  (ap1->selected() || ap2->selected()))
	// user moves just the middle line for the first time
	propag_geometry(geometry, FALSE);

      QRect arect = a->rect();
      QRect brect = b->rect();
      double nx;

      if (fixed_geometry)
	nx = ((ca.x() < cb.x()) ? (arect.right() + brect.left())
				: (arect.left() + brect.right()))/2
	 - half_arrow_size;
      else if (ap1->selected())
	// may be ap1 moved
	nx = ap1->x();
      else
	// may be ap2 moved
	nx = ap2->x();
      
      move_to(ap1, nx, ca.y() - half_arrow_size);
           
      move_to(ap2, ap1->x(), cb.y() - half_arrow_size);
    }
    break;
  default:
    // VHVGeometry
    {
      const ArrowCanvas * ar = this;
  
      while (ar->begin->type() == UmlArrowPoint)
	ar = ((ArrowPointCanvas *) ar->begin)->get_other(ar);
  
      ArrowPointCanvas * ap1 = (ArrowPointCanvas *) ar->end;
      ArrowPointCanvas * ap2 = 
	(ArrowPointCanvas *) ap1->get_other(ar)->end;
      
      if (fixed_geometry &&
	  !a->isSelected() &&
	  !b->isSelected() &&
	  (ap1->selected() || ap2->selected()))
	// user moves just the middle line for the first time
	propag_geometry(geometry, FALSE);

      QRect arect = a->rect();
      QRect brect = b->rect();
      double ny;

      if (fixed_geometry)
	ny = ((ca.y() < cb.y()) ? (arect.bottom() + brect.top())
				: (arect.top() + brect.bottom()))/2
	  - half_arrow_size;
      else if (ap1->selected())
	// may be ap1 moved
	ny =  ap1->y();
      else
	// may be ap2 moved
	ny = ap2->y();
      
      move_to(ap1, ca.x() - half_arrow_size, ny);
      
      move_to(ap2, cb.x() - half_arrow_size, ap1->y());
    }
    break;
  }  
}

ArrowCanvas * ArrowCanvas::next_geometry() {
  if (get_start() != get_end()) {
    int geo = geometry;
    
    for (;;) {
      geo = (geo + 1) % GeometrySup;
      
      switch (geo) {
      case HVrGeometry:
      case VHrGeometry:
	break;
      default:
	return set_geometry((LineGeometry) geo, TRUE);
      }
    }
  }
  else
    return this;
}
  
void ArrowCanvas::select_associated() {
  if (! selected()) {
    the_canvas()->select(this);
    if ((label != 0) && !label->selected())
      the_canvas()->select(label);
    if ((stereotype != 0) && !stereotype->selected())
      the_canvas()->select(stereotype);
    DiagramItem::select_associated();
    begin->select_associated();
    end->select_associated();
  }
}

void ArrowCanvas::modified() {
  if (visible()) {
    hide();
    update_pos();
    show();
    canvas()->update();
    package_modified();
  }
}

void ArrowCanvas::drawing_settings_modified() {
  auto_pos = the_canvas()->browser_diagram()
    ->get_auto_label_position(the_canvas()->browser_diagram()->get_type());
}

void ArrowCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "line_ref " << get_ident();
  else if (begin->type() != UmlArrowPoint) {
    nl_indent(st);
    st << "line " << get_ident() << ' ' << stringify(itstype);
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

const ArrowCanvas * ArrowCanvas::save_lines(QTextStream & st, bool with_label,
					    bool with_stereotype,
					    QString & warning) const {
  nl_indent(st);
  st << "from ref " << begin->get_ident();
  
  const ArrowCanvas * ar = this;
  
  while (ar->end->type() == UmlArrowPoint) {
    st << " z " << z();
    if (with_label && ar->label) {
      st << ' ';
      ar->label->save(st, FALSE, warning);
    }
    if (with_stereotype && ar->stereotype) {
      st << " stereotype ";
      save_string(ar->stereotype->get_name(), st);
      save_xyz(st, ar->stereotype, " xyz");
    }
    st << " to ";
    ar->end->save(st, FALSE, warning);
    ar = ((ArrowPointCanvas *) ar->end)->get_other(ar);
    nl_indent(st);
    st << "line " << ar->get_ident();
  }
  
  st << " z " << z();
  if (with_label && ar->label) {
    st << ' ';
    ar->label->save(st, FALSE, warning);
  }
  if (with_stereotype && ar->stereotype) {
    st << " stereotype ";
    save_string(ar->stereotype->get_name(), st);
    save_xyz(st, ar->stereotype, " xyz");
  }
  st << " to ref " << ar->end->get_ident();
  
  return ar;
}

static ArrowCanvas * make(UmlCanvas * canvas, DiagramItem * b,
			  DiagramItem * e, UmlCode t, int id)
{
  return new ArrowCanvas(canvas, b, e, t, id, FALSE);
}

ArrowCanvas * ArrowCanvas::read_list(char * & st, UmlCanvas * canvas,
				     UmlCode t, LineGeometry geo,
				     bool fixed, int id,
				     ArrowCanvas * (*pf)(UmlCanvas * canvas, DiagramItem * b,
							 DiagramItem * e, UmlCode t, int id))
{
  char * k;
    
  read_keyword(st, "from");
  read_keyword(st, "ref");
  
  DiagramItem * bi = dict_get(read_id(st), "", canvas);
  ArrowCanvas * result;
  LabelCanvas * label;
  LabelCanvas * stereotype;
  
  for (;;) {
    double z;
    
    if (read_file_format() < 5)
      z = OLD_ARROW_Z;
    else {
      read_keyword(st, "z");
      z = read_double(st);
    }
      
    k = read_keyword(st);
    
    if ((label = LabelCanvas::read(st, canvas, k)) != 0) {
      label->setZ(z);
      k = read_keyword(st);
    }
    if (!strcmp(k, "stereotype")) {
      k = read_string(st);
      if (read_file_format() < 5)
	read_keyword(st, "xy");
      else
	read_keyword(st, "xyz");
      
      int x = (int) read_double(st);
      
      stereotype =
	new LabelCanvas(k, canvas, x, (int) read_double(st));
	
      stereotype->setZ((read_file_format() < 5) ? OLD_ARROW_Z 
						: read_double(st));
      k = read_keyword(st);
    }
    else
      stereotype = 0;
    
    if (strcmp(k, "to"))
      wrong_keyword(k, "to");
    
    DiagramItem * di;
    
    if (!strcmp(k = read_keyword(st), "ref"))
      di = dict_get(read_id(st), "", canvas);
    else {
      di = ArrowPointCanvas::read(st, canvas, k);
      
      if (di == 0)
	unknown_keyword(k);
    }
    
    result = pf(canvas, bi, di, t, id);
    result->geometry = geo;
    result->fixed_geometry = fixed;
    result->setZ(z);
    result->show();
    if (label != 0)
      (result->label = label)->show();
    if (stereotype!= 0)
      (result->stereotype = stereotype)->show();
    
    if (di->type() != UmlArrowPoint)
      return result;
    
    bi = di;
    
    read_keyword(st, "line");
    id = read_id(st);
  }
}

ArrowCanvas * ArrowCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "line_ref"))
    return ((ArrowCanvas *) dict_get(read_id(st), "arrow canvas", canvas));
  else if (!strcmp(k, "line")) {
    int id = read_id(st);
    UmlCode t = arrow_type(read_keyword(st));
    LineGeometry geo;
    bool fixed;
    
    k = read_keyword(st);
    if (! strcmp(k, "geometry")) {
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
    
    return read_list(st, canvas, t, geo, fixed, id, &make);
  }
  else
    return 0;
}

void ArrowCanvas::package_modified() const {
  the_canvas()->browser_diagram()->package_modified();
}

void ArrowCanvas::history_save(QBuffer & b) const {
  ::save(this, b);
  // must save begin etc .. because they may change
  // when an arrow point is added/removed
  ::save(begin, b);
  ::save(end, b);
  
  int v = geometry;
  
  ::save((fixed_geometry) ? v : -v, b);
  ::save(label, b);
  ::save(stereotype, b);
  ::save(beginp, b);
  ::save(endp, b);
  ::save(boundings, b);
  ::save(arrow[0], b);
  ::save(arrow[1], b);
  ::save(arrow[2], b);
  ::save(poly, b);
  ::save(z(), b);
}

void ArrowCanvas::history_load(QBuffer & b) {
  begin = load_item(b);
  end = load_item(b);
  
  int geo;
  
  ::load(geo, b);
  if (geo < 0) {
    fixed_geometry = FALSE;
    geometry = (LineGeometry) -geo;
  }
  else {
    fixed_geometry = TRUE;
    geometry = (LineGeometry) geo;
  }
  
  label = (LabelCanvas *) load_item(b);
  stereotype = (LabelCanvas *) load_item(b);
  ::load(beginp, b);
  ::load(endp, b);
  ::load(boundings, b);
  ::load(arrow[0], b);
  ::load(arrow[1], b);
  ::load(arrow[2], b);
  ::load(poly, b);
  setPoints(boundings);
  QCanvasPolygon::setZ(load_double(b));
  QCanvasItem::setSelected(FALSE);
  QCanvasItem::setVisible(TRUE);

  begin->add_line(this);
  end->add_line(this);
  connect(DrawingSettings::instance(), SIGNAL(changed()),
	  this, SLOT(drawing_settings_modified()));
}

void ArrowCanvas::history_hide() {
  QCanvasPolygon::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(drawing_settings_modified()));
}
