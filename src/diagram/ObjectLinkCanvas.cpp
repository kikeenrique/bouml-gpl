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

#include <qcursor.h>
#include <math.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>

#include "ObjectLinkCanvas.h"
#include "ArrowPointCanvas.h"
#include "OdClassInstCanvas.h"
#include "UmlCanvas.h"
#include "LabelCanvas.h"
#include "BrowserDiagram.h"
#include "DiagramView.h"
#include "BrowserRelation.h"
#include "BrowserClass.h"
#include "RelationData.h"
#include "ObjectLinkDialog.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"

ObjectLinkCanvas::ObjectLinkCanvas(UmlCanvas * canvas, DiagramItem * b,
				   DiagramItem * e, int id,
				   RelationData * d)
      : ArrowCanvas(canvas, b, e, UmlObjectLink, id, TRUE),
        data(d), role_a(0), role_b(0) {
  if (d != 0) {
    itstype = d->get_type();
    
    // note : not connected to all the inheritances used
    // to access to the relation
    connect(d, SIGNAL(changed()), this, SLOT(modified()));
    connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  }
  
  // manages the case start == end
  if (b == e)
    cut_self();
  
  if (d != 0)
    update(TRUE);
}

ObjectLinkCanvas::~ObjectLinkCanvas() {
}

void ObjectLinkCanvas::delete_it() {
  if (data != 0)
    disconnect(data, 0, this, 0);
  
  ArrowCanvas::delete_it();
}

void ObjectLinkCanvas::deleted() {
  delete_it();
  canvas()->update();
  package_modified();
}

void ObjectLinkCanvas::internal_set_relation(RelationData * d) {
  if (data != 0)
    disconnect(data, 0, this, 0);
  
  if (d != 0) {
    itstype = d->get_type();
    connect(d, SIGNAL(changed()), this, SLOT(modified()));
    connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  }
  else {
    itstype = UmlObjectLink;
    
    if (role_a != 0) {
      the_canvas()->del(role_a);
      role_a = 0;
    }
    if (role_b != 0) {
      the_canvas()->del(role_b);
      role_b = 0;
    }
  }
  
  data = d;
}

void ObjectLinkCanvas::set_relation(RelationData * d) {
  if (data != d) {
    // Updates data in all segment
    internal_set_relation(d);
    
    ObjectLinkCanvas * a = this;
    
    while (a->begin->type() == UmlArrowPoint) {
      a = (ObjectLinkCanvas *) ((ArrowPointCanvas *) a->begin)->get_other(a);
      a->internal_set_relation(d);
    }
    
    ObjectLinkCanvas * z = this;
    
    while (z->end->type() == UmlArrowPoint) {
      z = (ObjectLinkCanvas *) ((ArrowPointCanvas *) z->end)->get_other(z);
      z->internal_set_relation(d);
    }
    
    if ((d != 0) && 
	(d->get_start_class() != ((OdClassInstCanvas *) a->begin)->get_type())) {
      // relation in the wrong direction
      
      // remove labels
      if (a->role_b != 0) {
	the_canvas()->del(a->role_b);
	a->role_b = 0;
      }
      if (z->role_a != 0) {
	the_canvas()->del(z->role_a);
	z->role_a = 0;
      }

      // reverse direction updating begin/end & beginp/endp
      for (;;) {
	DiagramItem * di = a->begin;
	
	a->begin = a->end;
	a->end = di;
	
	QPoint p = a->beginp;
	
	a->beginp = a->endp;
	a->endp = p;
	
	if (a == z)
	  break;
	
	a = (ObjectLinkCanvas *) ((ArrowPointCanvas *) a->begin)->get_other(a);
      }
    }
  }
  
  update(FALSE);
  setVisible(TRUE);
}

void ObjectLinkCanvas::update_pos() {
  ArrowCanvas::update_pos();

  if (auto_pos) {
    if ((role_a != 0) && !role_a->selected())
      role_a_default_position();
    
    if ((role_b != 0) && !role_b->selected())
      role_b_default_position();
  }
  
  update_show_lines();
}

void ObjectLinkCanvas::setVisible(bool yes) {
  ArrowCanvas::setVisible(yes);

  if (role_a != 0)
    role_a->setVisible(yes);
  if (role_b != 0)
    role_b->setVisible(yes);
}

void ObjectLinkCanvas::moveBy(double dx, double dy) {
  ArrowCanvas::moveBy(dx, dy);
  
  if ((role_a != 0) && !role_a->selected())
    role_a->moveBy(dx, dy);
  if ((role_b != 0) && !role_b->selected())
    role_b->moveBy(dx, dy);
}

void ObjectLinkCanvas::set_z(double z) {
  ArrowCanvas::set_z(z);
  
  if (role_a != 0)
    role_a->setZ(z);
  if (role_b != 0)
    role_b->setZ(z);
}

void ObjectLinkCanvas::select_associated() {
  // special case to move a class with its self relations goes
  // throw all the relation (even the already selected sections)
  // and does not check that this is or not already selected
  
  ObjectLinkCanvas * rel = this;
  
  // goes to the beginning of the (broken) line
  while (rel->begin->type() == UmlArrowPoint)
    rel = ((ObjectLinkCanvas *) ((ArrowPointCanvas *) rel->begin)->get_other(rel));
  
  rel->begin->select_associated();
  
  for (;;) {
    // select labels
    if ((rel->role_a != 0) && !rel->role_a->selected())
      the_canvas()->select(rel->role_a);
    if ((rel->role_b != 0) && !rel->role_b->selected())
      the_canvas()->select(rel->role_b);
    
    if (! rel->selected())
      the_canvas()->select(rel);
    
    // goes forward
    if (rel->end->type() != UmlArrowPoint)
      break;
    
    if (!((ArrowPointCanvas *) rel->end)->selected())
      the_canvas()->select((ArrowPointCanvas *) rel->end);
    rel = (ObjectLinkCanvas *) ((ArrowPointCanvas *) rel->end)->get_other(rel);
  }
  
  rel->end->select_associated();
}

bool ObjectLinkCanvas::reflexive() const {
  return (get_start() == get_end());
}

bool compatible(UmlCode k)
{
  switch (k) {
  case UmlAssociation:
  case UmlDirectionalAssociation:
  case UmlAggregation:
  case UmlAggregationByValue:
  case UmlDirectionalAggregation:
  case UmlDirectionalAggregationByValue:
    return TRUE;
  default:
    return FALSE;
  }
}

// cherche les relations des classes de 'from' dont
// la cible est dans 'to'

static void get_assocs(QList<BrowserClass> & from,
		       QList<BrowserClass> & to,
		       QList<RelationData> & rels)
{
  QListIterator<BrowserClass> it(from);
  
  for (; it.current(); ++it) {
    QListViewItem * child;
    
    for (child = it.current()->firstChild(); child; child = child->nextSibling()) {
      if (!((BrowserNode *) child)->deletedp() &&
	  compatible(((BrowserNode *) child)->get_type())) {
	RelationData * rd = (RelationData *)
	  ((BrowserRelation *) child)->get_data();
	
	if ((rels.findRef(rd) == -1) &&
	    (to.findRef(rd->get_end_class()) != -1))
	  rels.append(rd);
      }
    }
  }
}

void ObjectLinkCanvas::open() {
  // get first & last segments
  ObjectLinkCanvas * first = this;
  ObjectLinkCanvas * last = this;
  
  while (first->begin->type() == UmlArrowPoint)
    first = ((ObjectLinkCanvas *) ((ArrowPointCanvas *) first->begin)->get_other(first));
  
  while (last->end->type() == UmlArrowPoint)
    last = (ObjectLinkCanvas *) ((ArrowPointCanvas *) last->end)->get_other(last);
  
  // compute all compatible relations
  BrowserClass * a = ((OdClassInstCanvas *) first->begin)->get_type();
  BrowserClass * b = ((OdClassInstCanvas *) last->end)->get_type();
  QList<BrowserClass> la;
  QList<BrowserClass> lb;
  QList<RelationData> l;
  
  a->get_all_parents(la);  
  b->get_all_parents(lb);
  la.append(a);
  lb.append(b);
  get_assocs(la, lb, l);
  get_assocs(lb, la, l);
  
  // dialog
  ObjectLinkDialog dialog(l, data);
  
  if ((dialog.exec() == QDialog::Accepted) && (dialog.rel() != data)) {
    set_relation(dialog.rel());
    first->modified();	// to update role label
    last->modified();	// to update role label
    package_modified();
  }
}

void ObjectLinkCanvas::menu(const QPoint & lpos) {
  ObjectLinkCanvas * plabel;
  ObjectLinkCanvas * pstereotype;
    
  {
    ArrowCanvas * aplabel;
    ArrowCanvas * apstereotype;
    
    search_supports(aplabel, apstereotype);
    plabel = (ObjectLinkCanvas *) aplabel;
    pstereotype = (ObjectLinkCanvas *) apstereotype;
  }
    
  const ObjectLinkCanvas * first = this;
  const ObjectLinkCanvas * last = this;
  
  while (first->begin->type() == UmlArrowPoint)
    first = ((ObjectLinkCanvas *) ((ArrowPointCanvas *) first->begin)->get_other(first));
  
  while (last->end->type() == UmlArrowPoint)
    last = (ObjectLinkCanvas *) ((ArrowPointCanvas *) last->end)->get_other(last);
  
  bool prefer_start = 
    (lpos - first->beginp).manhattanLength() >
      (lpos - last->endp).manhattanLength();
  QPopupMenu m(0);
  QPopupMenu geo(0);
  //QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle("Object link", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Edit", 0);
  m.insertSeparator();
  
  if (data != 0)
    m.insertItem("Select relation in browser", 2);
  if (plabel || pstereotype || first->role_b || last->role_a) {
    m.insertSeparator();
    m.insertItem("Select labels", 3);
    m.insertItem("Labels default position", 4);
    if (plabel && (label == 0))
      m.insertItem("Attach relation's name to this segment", 5);
    if (pstereotype && (stereotype == 0))
      m.insertItem("Attach relation's stereotype to this segment", 6);
  }
  
  if (get_start() != get_end()) {
    m.insertSeparator();
    init_geometry_menu(geo, 10);
    m.insertItem("Geometry (Ctrl+l)", &geo);
  }
  
  m.insertSeparator();
  m.insertItem("Remove from view",7);
  
  m.insertSeparator();
  /*
  if (Tool::menu_insert(&toolm, itstype, 20))
    m.insertItem("Tool", &toolm);
  */
  
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    open();	// call package_modified
    return;
  case 2:
    data->select_in_browser(prefer_start);
    return;
  case 3:
    the_canvas()->unselect_all();
    if (plabel)
      the_canvas()->select(plabel->label);
    if (pstereotype)
      the_canvas()->select(pstereotype->stereotype);
    if (last->role_a)
      the_canvas()->select(last->role_a);
    if (first->role_b)
      the_canvas()->select(first->role_b);
    return;
  case 4:
    if (plabel)
      plabel->default_label_position();
    if (pstereotype)
      pstereotype->default_stereotype_position();
    if (last->role_a != 0)
      last->role_a_default_position();
    if (first->role_b != 0)
      first->role_b_default_position();
    break;
  case 5:
    label = plabel->label;
    plabel->label = 0;
    default_label_position();
    break;
  case 6:
    stereotype = pstereotype->stereotype;
    pstereotype->stereotype = 0;
    default_stereotype_position();
    break;
  case 7:
    // not removed from the browser : just hide it
    delete_it();
    break;
  default:
    /*
    if (rank >= 20) {
      ToolCom::run(Tool::command(rank - 20),
		   ((prefer_start || RelationData::uni_directional(itstype))
		    ? data->get_start()
		    : data->get_end()));
      return;
    }
    else*/ if (rank >= 10) {
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


ArrowPointCanvas * ObjectLinkCanvas::brk(const QPoint & p) {
  ArrowPointCanvas * ap =
    new ArrowPointCanvas(the_canvas(), p.x(), p.y());
  
  ap->setZ(z() + 1);	// + 1 else point can't be selected
  
  ObjectLinkCanvas * other =
    // do not give data to not call update()
    new ObjectLinkCanvas(the_canvas(), ap, end, itstype, 0);
  
  if (data != 0) {
    other->data = data;
    other->itstype = itstype;
    connect(data, SIGNAL(changed()), other, SLOT(modified()));
    connect(data, SIGNAL(deleted()), other, SLOT(deleted()));
  }

  ap->add_line(this);
  end->remove_line(this);
  end = ap;
  ap->show();
  other->show();
  
  modified();		// to set/unset role_b
  other->modified();	// to set/unset role_a
  
  return ap;
}


ArrowCanvas * ObjectLinkCanvas::join(ArrowCanvas * other, ArrowPointCanvas * ap) {
  // has already check is join is possible (self relation must have two points)
  ArrowCanvas * result = ArrowCanvas::join(other, ap);

  ((ObjectLinkCanvas *) result)->modified();
  
  return result;
}

void ObjectLinkCanvas::modified() {
  if (visible()) {
    hide();
    update(TRUE);
    show();
    canvas()->update();
    package_modified();
  }
}

void ObjectLinkCanvas::update(bool updatepos) {
  if (data != 0) {
    QString s;
    
    // manages role_a
    
    s = data->get_role_a();
    
    if (s.isEmpty() || (end->type() == UmlArrowPoint)) {
      // relation does not have role_a name or it must be hidden
      if (role_a != 0) {
	the_canvas()->del(role_a);
	role_a = 0;
      }
    }
    else if (role_a == 0) {
      // adds role_a
      role_a = new LabelCanvas(s, the_canvas(), 0, 0);
      role_a_default_position();
    }
    else if (role_a->get_name() != s) {
      role_a->set_name(s);
      role_a_default_position();
    }
    
    // manages role_b
    
    s = data->get_role_b();
    
    if (s.isEmpty() || RelationData::uni_directional(itstype)) {
      // relation does not have role_b name
      if (role_b != 0) {
	the_canvas()->del(role_b);
	role_b = 0;
      }
    }
    else if ((role_b == 0) && (begin->type() != UmlArrowPoint)) {
      // adds role_b
      role_b = new LabelCanvas(s, the_canvas(), 0, 0);
      role_b_default_position();
    }
    else if ((role_b != 0) && (role_b->get_name() != s)) {
      role_b->set_name(s);
      role_b_default_position();
    }
    
    // manages relation's name
    
    s = data->get_name();
    
    QPoint c = center();
    bool unamed = !RelationData::isa_association(itstype);
    QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
    int h = fm.height();
    ObjectLinkCanvas * plabel;
    ObjectLinkCanvas * pstereotype;
    
    {
      ArrowCanvas * aplabel;
      ArrowCanvas * apstereotype;
      
      search_supports(aplabel, apstereotype);
      plabel = (ObjectLinkCanvas *) aplabel;
      pstereotype = (ObjectLinkCanvas *) apstereotype;
    }
    
    if (unamed || s.isEmpty() || (s == RelationData::default_name(itstype))) {
      // relation does not have name or 'true' name
      if (plabel != 0) {
	// removes it
	the_canvas()->del(plabel->label);
	plabel->label = 0;
      }
    }
    else if ((plabel == 0) && (begin->type() != UmlArrowPoint)) {
      // adds relation's name
      label =
	new LabelCanvas(s, the_canvas(), c.x() - fm.width(s)/2,
			c.y() - h, FALSE, TRUE, FALSE);
      default_label_position();
    }
    else if ((plabel != 0) && (plabel->label->get_name() != s)) {
      // update name
      plabel->label->set_name(s);
      plabel->default_label_position();
    }
    
    // manages relation's stereotype
    
    s = data->get_stereotype();
    
    if (s.isEmpty()) {
      // relation does not have stereotype
      if (pstereotype != 0) {
	// removes it
	the_canvas()->del(pstereotype->stereotype);
	pstereotype->stereotype = 0;
      }
    }
    else {
      s = toUnicode(s);

      if (s[0] != '{')
	s = QString("<<") + s + ">>";
      
      if ((pstereotype == 0) && (begin->type() != UmlArrowPoint)) {
	// adds relation's stereotype
	stereotype = new LabelCanvas(s, the_canvas(), 0, 0);
	default_stereotype_position();
      }
      else if ((pstereotype != 0) && 
	       (pstereotype->stereotype->get_name() != s)) {
	// update name
	pstereotype->stereotype->set_name(s);
	pstereotype->default_stereotype_position();
      }
    }    
  }
  else {
    ArrowCanvas * aplabel;
    ArrowCanvas * apstereotype;
    
    search_supports(aplabel, apstereotype);
    
    if (aplabel != 0) {
      // removes it
      the_canvas()->del(((ObjectLinkCanvas *) aplabel)->label);
      ((ObjectLinkCanvas *) aplabel)->label = 0;
    }
    if (apstereotype != 0) {
      // removes it
      the_canvas()->del(((ObjectLinkCanvas *) apstereotype)->stereotype);
      ((ObjectLinkCanvas *) apstereotype)->stereotype = 0;
    }
  }
  
  if (updatepos)
    update_pos();
}

// check if the relation still exist between the extremities
// even through inheritance

void ObjectLinkCanvas::check() {
  if (data == 0)
    return;
  
  if (compatible(data->get_type())) {
    BrowserClass * a = ((OdClassInstCanvas *) get_start())->get_type();
    BrowserClass * z = ((OdClassInstCanvas *) get_end())->get_type();
    QList<BrowserClass> la;
    QList<BrowserClass> lz;
    
    a->get_all_parents(la);  
    z->get_all_parents(lz);
    la.append(a);
    lz.append(z);
    
    if ((la.findRef(data->get_start_class()) != -1) &&
	(lz.findRef(data->get_end_class()) != -1))
      return;
  }
  
  // here the relation doesn't exist
  set_relation(0);
  package_modified();
}

void ObjectLinkCanvas::role_a_default_position() const {
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  int h = fm.height();
  int dx = endp.x() - beginp.x();
  int dy = endp.y() - beginp.y();
  
  if (fabs(dx) >= fabs(dy)) {
    // horizontal line
    if (dx == 0) dx = 1;
  
    if (dx > 0) {
      int w = fm.width(data->get_role_a());
      
      role_a->move(endp.x() - w - ARROW_LENGTH,
		   endp.y() - 4*h/3 + (dy * ARROW_LENGTH) / dx);
    }
    else
      role_a->move(endp.x() + ARROW_LENGTH,
		   endp.y() - 4*h/3 + (dy * ARROW_LENGTH) / dx);
  }
  else {
    // vertical line
    if (dy == 0) dy = 1;
    
    if (dy > 0)
      role_a->move(endp.x() + ARROW_LENGTH - (dx * h/2) / dy,
		   endp.y() - 4*h/3);
    else
      role_a->move(endp.x() + ARROW_LENGTH + (dx * h/2) / dy,
		   endp.y() + h/2);
  }
  package_modified();
}

void ObjectLinkCanvas::role_b_default_position() const {
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  int h = fm.height();
  int dx = endp.x() - beginp.x();
  int dy = endp.y() - beginp.y();
  
  if (fabs(dx) >= fabs(dy)) {
    // horizontal line
    if (dx == 0) dx = 1;
  
    if (dx > 0)
      role_b->move(beginp.x() + ARROW_LENGTH,
		   beginp.y() - 4*h/3 + (dy * ARROW_LENGTH) / dx);
    else {
      int w = fm.width(data->get_role_b());
      
      role_b->move(beginp.x() - w - ARROW_LENGTH,
		   beginp.y() - 4*h/3 + (dy * ARROW_LENGTH) / dx);
    }
  }
  else {
    // vertical line
    if (dy == 0) dy = 1;
    
    if (dy > 0)
      role_b->move(beginp.x() + ARROW_LENGTH + (dx * h/2) / dy,
		   beginp.y() + h/2);
    else
      role_b->move(beginp.x() + ARROW_LENGTH - (dx * h/2) / dy,
		   beginp.y() - 4*h/3);
  }
  package_modified();
}

void ObjectLinkCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "objectlinkcanvas_ref " << get_ident()
       << " // " << data->get_name();
  else if (begin->type() != UmlArrowPoint) {
    // relation canvas start
    nl_indent(st);
    st << "objectlinkcanvas " << get_ident();
    if (data == 0)
      st << " norel";
    else {
      st << " rel ";
      data->save(st, TRUE, warning);
    }
    indent(+1);
    
    if (geometry != NoGeometry) {
      nl_indent(st);
      st << "geometry " << stringify(geometry);
      if (!fixed_geometry)
	st << " unfixed";
    }
    
    const ObjectLinkCanvas * last = 
      (const ObjectLinkCanvas *) ArrowCanvas::save_lines(st, TRUE, TRUE, warning);
    
    nl_indent(st);
    if (last->role_a)
      save_xyz(st, last->role_a, "role_a_pos");
    else
      st << "no_role_a";
    if (role_b)
      save_xyz(st, role_b, " role_b_pos");
    else
      st << " no_role_b";
    indent(-1);
  }
}

ObjectLinkCanvas * ObjectLinkCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "objectlinkcanvas_ref"))
    return ((ObjectLinkCanvas *) dict_get(read_id(st), "objectlinkcanvas", canvas));
  else if (!strcmp(k, "objectlinkcanvas")) {    
    int id = read_id(st);
    RelationData * rd;
    
    k = read_keyword(st);
    if (! strcmp(k, "norel"))
      rd = 0;
    else if (! strcmp(k, "rel"))
      rd = RelationData::read_ref(st, TRUE);
    else
      wrong_keyword(k, "norel' or 'rel");
    
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

    read_keyword(st, "from");
    read_keyword(st, "ref");
    
    DiagramItem * bi = dict_get(read_id(st), "classinstance", canvas);
    UmlCode t;
    bool unamed = TRUE;
    QString s;
    ObjectLinkCanvas * first = 0;
    ObjectLinkCanvas * result;
    LabelCanvas * label;
    LabelCanvas * stereotype;
    double z;
              
    if (rd != 0) {
      t = rd->get_type();
      unamed = !RelationData::isa_association(t);
      s = rd->get_name();
    }
    
    for (;;) {
      read_keyword(st, "z");
      z = read_double(st);
      
      k = read_keyword(st);
      
      if ((label = LabelCanvas::read(st, canvas, k)) != 0) {
	if ((rd == 0) || unamed || s.isEmpty() || (s == RelationData::default_name(t))) {
	  // relation does not have name or 'true' name
	  canvas->del(label);
	  label = 0;
	}
	else
	  label->setZ(z);
	
	k = read_keyword(st);
      }
      
      if (!strcmp(k, "stereotype")) {
	(void) read_string(st);
	read_keyword(st, "xyz");
	
	int x = (int) read_double(st);
	int y = (int) read_double(st);
	
	if ((rd == 0) || (*(rd->get_stereotype()) == 0)) {
	  stereotype = 0;
	  read_double(st);	// z
	}
	else {
	  QString s = toUnicode(rd->get_stereotype());
	  
	  if (s[0] != '{')
	    s = QString("<<") + s + ">>";

	  stereotype = new LabelCanvas(s, canvas, x, y);
	  stereotype->setZ(read_double(st));
	}
	k = read_keyword(st);
      }
      else
	stereotype = 0;

      if (strcmp(k, "to"))
	wrong_keyword(k, "to");

      DiagramItem * di;
      
      if (strcmp(k = read_keyword(st), "ref")) {
	di = ArrowPointCanvas::read(st, canvas, k);
	
	if (di == 0)
	  unknown_keyword(k);
      }
      else
	di = dict_get(read_id(st), "classinstance", canvas);

      // do not give rd to not call update()
      result = new ObjectLinkCanvas(canvas, bi, di, id);
      result->geometry = geo;
      result->fixed_geometry = fixed;
      result->setZ(z);
      if (rd != 0) {
	result->data = rd;
	result->itstype = rd->get_type();
	connect(rd, SIGNAL(changed()), result, SLOT(modified()));
	connect(rd, SIGNAL(deleted()), result, SLOT(deleted()));
      }

      if (first == 0)
	first = result;
      if (label != 0)
	result->label = label;
      if (stereotype != 0)
	(result->stereotype = stereotype)->show();
      result->show();
      
      if (di->type() != UmlArrowPoint)
	break;
      
      bi = di;
      
      read_keyword(st, "line");
      id = read_id(st);
    }
    
    // roles
    
    int x, y;
    
    k = read_keyword(st);
    if (!strcmp(k, "role_a_pos")) {
      x = (int) read_double(st);
      y = (int) read_double(st);
      z = (read_file_format() < 5) ? /*OLD_*/LABEL_Z : read_double(st);
      
      if (rd != 0) {
	s = rd->get_role_a();
	
	if (!unamed && !s.isEmpty()) {
	  result->role_a = new LabelCanvas(s, canvas, x, y);
	  result->role_a->setZ(z);
	  result->role_a->show();
	}
      }
    }
    else if (strcmp(k, "no_role_a"))
      wrong_keyword(k, "no_role_a");
    
    k = read_keyword(st);
    if (!strcmp(k, "role_b_pos")) {
      x = (int) read_double(st);
      y = (int) read_double(st);
      z = (read_file_format() < 5) ? /*OLD_*/LABEL_Z : read_double(st);
      
      if (rd != 0) {
	s = rd->get_role_b();
	
	if (!unamed && !s.isEmpty()) {
	  first->role_b = new LabelCanvas(s, canvas, x, y);
	  first->role_b->setZ(z);
	  first->role_b->show();
	}
      }
    }
    else if (strcmp(k, "no_role_b"))
      wrong_keyword(k, "no_role_b");
    
    // to add /remove roles is needed
    
    first->update(FALSE);
    if (first != result)
      result->update(FALSE);
    
    if (rd != 0) {
      if (rd->get_start()->deletedp())
	result->set_relation(0);
      else
	first->check();
    }
    
    return result;
  }
  else
    return 0;
}

void ObjectLinkCanvas::history_save(QBuffer & b) const {
  ArrowCanvas::history_save(b);
  // save labels because they may change
  // when an arrow point is added/removed
  ::save(role_a, b);
  ::save(role_b, b);
}

void ObjectLinkCanvas::history_load(QBuffer & b) {
  ArrowCanvas::history_load(b);
  role_a = (LabelCanvas *) load_item(b);
  role_b = (LabelCanvas *) load_item(b);
  if (data != 0) {
    connect(data, SIGNAL(changed()), this, SLOT(modified()));
    connect(data, SIGNAL(deleted()), this, SLOT(deleted()));
  }
}

void ObjectLinkCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  if (data != 0)
    disconnect(data, 0, this, 0);
}
