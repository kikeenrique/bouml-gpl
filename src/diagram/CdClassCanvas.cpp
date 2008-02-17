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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "CdClassCanvas.h"
#include "TemplateCanvas.h"
#include "RelationCanvas.h"
#include "SimpleRelationCanvas.h"
#include "ArrowPointCanvas.h"
#include "ConstraintCanvas.h"
#include "BrowserClass.h"
#include "BrowserOperation.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "BrowserAttribute.h"
#include "AttributeData.h"
#include "OperationData.h"
#include "ClassData.h"
#include "RelationData.h"
#include "PackageData.h"
#include "DiagramView.h"
#include "SettingsDialog.h"
#include "HideShowDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "MenuItalic.h"
#include "OperationListDialog.h"
#include "BrowserClassDiagram.h"
#include "strutil.h"
#include "GenerationSettings.h"

CdClassCanvas::CdClassCanvas(BrowserNode * bn, UmlCanvas * canvas,
			     int x, int y)
    : DiagramCanvas(0, canvas, x, y, CLASS_CANVAS_MIN_SIZE,
		    CLASS_CANVAS_MIN_SIZE, 0) {
  browser_node = bn;
  templ = 0;	// may be updated by compute_size()
  itscolor = UmlDefaultColor;
  indicate_visible_attr = FALSE;
  indicate_visible_oper = FALSE;
  constraint = 0;
  
  compute_size();	// update used_settings
  check_constraint();
  
  subscribe(bn->get_data());	// = TRUE
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  
  if (canvas->must_draw_all_relations()) {
    draw_all_relations();
    draw_all_class_assoc();
    draw_all_simple_relations();
  }
}

CdClassCanvas::CdClassCanvas(UmlCanvas * canvas, int id)
    : DiagramCanvas(canvas, id) {
  // for read operation
  browser_node = 0;
  templ = 0;	// may be updated by compute_size()
  itscolor = UmlDefaultColor;
  indicate_visible_attr = FALSE;
  indicate_visible_oper = FALSE;
  constraint = 0;
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

CdClassCanvas::~CdClassCanvas() {
}

void CdClassCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  
  MultipleDependencyIterator<BasicData> it(this);
  
  while (it.current()) {
    disconnect(it.current(), 0, this, 0);
    ++it;
  }
  
  unsubscribe_all();
  
  if (templ)
    templ->delete_it();
  
  if (constraint)
    constraint->delete_it();
  
  DiagramCanvas::delete_it();
}

void CdClassCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void CdClassCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void CdClassCanvas::compute_size() {
  // does not unsubscribe & disconnect signals because compute_size may
  // be called during a signal management, and the signal connection list
  // cannot be modified in this case
  used_settings = settings;
  the_canvas()->browser_diagram()->get_classdiagramsettings(used_settings);
    
  full_name = browser_node->get_name();
  
  const MyStr & (PackageData::*f)() const = 0;
  const char * sep = 0;	// to avoid warning
  
  switch (used_settings.show_context_mode) {
  case umlContext:
    full_name = browser_node->full_name();
    break;
  case namespaceContext:
    f = &PackageData::get_cpp_namespace;
    sep = "::";
    break;
  case javaPackageContext:
    f = &PackageData::get_java_package;
    sep = ".";
    break;
  case pythonPackageContext:
    f = &PackageData::get_python_package;
    sep = ".";
    break;
  case moduleContext:
    f = &PackageData::get_idl_module;
    sep = "::";
    break;
  default:
    break;
  }
  
  if (f != 0) {
    BrowserClass * cl = (BrowserClass *) browser_node;
    
    while (cl->nestedp())
      cl = (BrowserClass *) cl->parent();
    
    if (((BrowserNode *) cl->parent())->get_type() == UmlClassView) {
      // not under a use case
      BrowserArtifact * cp = cl->get_associated_artifact();
      
      QString context =
	(((PackageData *)
	  ((BrowserNode *)
	   (((cp == 0) ? (BrowserNode *) cl : (BrowserNode *) cp)
	    ->parent()->parent()))->get_data())->*f)();
      
      if (!context.isEmpty())
	full_name = context + sep + full_name;
    }
  }
  
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  QFontMetrics fbm(the_canvas()->get_font(UmlNormalBoldFont));
  QFontMetrics fim(the_canvas()->get_font(UmlNormalItalicFont));
  QFontMetrics fbim(the_canvas()->get_font(UmlNormalBoldItalicFont));
  const ClassData * data = ((ClassData *) browser_node->get_data());
  int wi =
    (data->get_is_abstract()) ? fbim.width(full_name) : fbm.width(full_name);
  QListViewItem * child;
  int natt = 0;
  int noper = 0;
  bool full_members = (used_settings.show_full_members_definition == UmlYes);
  bool show_visibility = (used_settings.show_members_visibility == UmlYes);
  bool show_stereotype = (used_settings.show_members_stereotype == UmlYes);
  bool show_multiplicity = (used_settings.show_members_multiplicity == UmlYes);
  bool show_initialization = (used_settings.show_members_initialization == UmlYes);
  bool show_dir = (used_settings.show_parameter_dir == UmlYes);
  bool show_name = (used_settings.show_parameter_name == UmlYes);
  bool hide_attrs = (used_settings.hide_attributes == UmlYes);
  bool hide_opers = (used_settings.hide_operations == UmlYes);
  int max_member_width = used_settings.member_max_width;
  
  if (max_member_width == UmlUnlimitedMemberWidth)
    max_member_width = 1000000;

  if (used_settings.class_drawing_mode == Natural) {
    const char * st = data->get_stereotype();
    
    if (!strcmp(st, "control"))
      used_view_mode = asControl;
    else if (!strcmp(st, "entity"))
      used_view_mode = asEntity;
    else if (!strcmp(st, "boundary"))
      used_view_mode = asBoundary;
    else if (!strcmp(st, "actor"))
      used_view_mode = asActor;
    else
      used_view_mode = asClass;
  }
  else
    used_view_mode = used_settings.class_drawing_mode;

  if (!hide_attrs || !hide_opers) {
    QString s;
    
    for (child = browser_node->firstChild(); child; child = child->nextSibling()) {
      if (! ((BrowserNode *) child)->deletedp()) {
	BasicData * child_data = ((BrowserNode *) child)->get_data();
	int wa;
	
	switch (((BrowserNode *) child)->get_type()) {
	case UmlAttribute:
	  
	  if (hide_attrs ||
	      ((indicate_visible_attr)
	       ? (hidden_visible_attributes.findIndex((BrowserNode *) child) == -1)
	       : (hidden_visible_attributes.findIndex((BrowserNode *) child) != -1)))
	    continue;
	  
	  s = ((AttributeData *) child_data)
	    ->definition(full_members, show_multiplicity,
			 show_initialization, used_settings.drawing_language);
	  
	  if (s.isEmpty())
	    continue;
	  
	  if ((int) s.length() >= max_member_width)
	    s = s.left(max_member_width) + "...";
	  
	  natt += 1;
	  wa = fm.width(s);
	  
	  if (full_members) {
	    BrowserClass * t = ((AttributeData *) child_data)->get_type().type;
	    
	    if (t && subscribe(t->get_data()))
	      connect(t->get_data(), SIGNAL(changed()),
		      this, SLOT(modified()));
	  }
	  break;
	case UmlOperation:
	  if (hide_opers ||
	      ((indicate_visible_oper)
	       ? (hidden_visible_operations.findIndex((BrowserNode *) child) == -1)
	       : (hidden_visible_operations.findIndex((BrowserNode *) child) != -1)))
	    continue;
	  
	  s = ((OperationData *) child_data)
	    ->definition(full_members, used_settings.drawing_language,
			 show_dir, show_name);

	  if (s.isEmpty())
	    continue;
	  
	  if ((int) s.length() >= max_member_width)
	    s = s.left(max_member_width) + "...";
	  
	  noper += 1;
	  if (((OperationData *) child_data)->get_is_abstract())
	    wa = fim.width(s);
	  else
	    wa = fm.width(s);
	  
	  if (full_members) {
	    BrowserClass * t =
	      ((OperationData *) child_data)->get_return_type().type;
	    
	    if ((t != 0) && subscribe(t->get_data()))
	      connect(t->get_data(), SIGNAL(changed()),
		      this, SLOT(modified()));
	    
	    int n = (int) ((OperationData *) child_data)->get_n_params();
	    
	    for (int i = 0; i != n; i += 1)
	      if (((t = ((OperationData *) child_data)->get_param_type(i).type) != 0) &&
		  subscribe(t->get_data()))
		connect(t->get_data(), SIGNAL(changed()),
			this, SLOT(modified()));
	  }
	  break;
	default:
	  continue;
	}
	
	if (show_visibility) {
	  wa += fbim.width("#_");
	  if (subscribe(child_data))
	    connect(child_data, SIGNAL(changed()), this, SLOT(modified()));
	}
	
	if (show_stereotype) {
	  QString st = child_data->get_stereotype();
	  
	  if (! st.isEmpty()) 
	    wa += fm.width("<<" + st + ">>_");
	}
	
	if ((full_members || show_visibility || show_stereotype) &&
	    subscribe(child_data))
	  connect(child_data, SIGNAL(changed()), this, SLOT(modified()));
	
	
	if (wa > wi)
	  wi = wa;
      }
    }
  }

  double zoom = the_canvas()->zoom();
  const int two = (int) (2 * zoom);
  const int four = (int) (4 * zoom);
  const int six = (int) (6 * zoom);
  const int eight = (int) (8 * zoom);

  int he = (fbm.height() + two) * (natt + noper + 1) + six;
  
  bool tmpl = data->get_n_formalparams() != 0;
  
  if (tmpl)
    he += fbm.height();
  if (natt == 0)
    he += six;
  if (noper == 0)
    he += six;
  
  if ((used_view_mode == asClass) && data->get_stereotype()[0]) {
    int stw = fm.width(QString("<<") + toUnicode(data->get_stereotype()) + ">>");
    
    if (wi < stw)
      wi = stw;
    
    he += fbm.height() + four;
  }
  
  wi += eight;
  
  int min_w;
  
  switch (used_view_mode) {
  case asControl:
    min_w = (int) (CONTROL_WIDTH * zoom);
    he += (int) (CONTROL_HEIGHT * zoom);
    break;
  case asBoundary:
    min_w = (int) (BOUNDARY_WIDTH * zoom);
    he += (int) (BOUNDARY_HEIGHT * zoom);
    break;
  case asEntity:
    min_w = (int) (ENTITY_SIZE * zoom);
    he += min_w;
    break;
  case asActor:
    min_w = (int) (ACTOR_SIZE * zoom);
    he += (int) (ACTOR_SIZE * zoom);
    break;
  default:	// class
    min_w = (int) (CLASS_CANVAS_MIN_SIZE * zoom);
    break;
  }
  
  if (wi < min_w)
    wi = min_w;
  
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlClass)
    : itscolor;
  
  if ((used_view_mode == asClass) && (used_color != UmlTransparent)) {
    const int shadow = the_canvas()->shadow();
    
    wi += shadow;
    he += shadow;
  }
  
  // force odd width and height for line alignment
  DiagramCanvas::resize(wi | 1, he | 1);
    
  if (tmpl) {
    if (templ == 0) {
      templ = new TemplateCanvas(this);
      templ->show();
    }
    else
      templ->update();
  }
  else if (templ != 0) {
    templ->delete_it();
    templ = 0;
  }
}

void CdClassCanvas::modified() {
  if (visible()) {
    hide();
    
    hide_lines();
    compute_size();
    recenter();
    show();
    update_show_lines();
    force_self_rel_visible();
    check_inner();
    if (the_canvas()->must_draw_all_relations()) {
      draw_all_relations();    
      draw_all_simple_relations();
    }
    check_constraint();
    canvas()->update();
    package_modified();
  }
}

void CdClassCanvas::post_loaded() {
  force_self_rel_visible();
  check_inner();
  if (the_canvas()->must_draw_all_relations()) {
    draw_all_relations();    
    draw_all_simple_relations();
  }
  check_constraint();
}

void CdClassCanvas::check_inner() {
  QListIterator<ArrowCanvas> it(lines);
	
  while (it.current()) {
    if (it.current()->type() == UmlInner) {
      DiagramItem * b = ((ArrowCanvas *) it.current())->get_start();
      DiagramItem * e = ((ArrowCanvas *) it.current())->get_end();
      
      if (((BrowserNode *) b->get_bn()->parent()) != e->get_bn())
	it.current()->delete_it();
      else
	++it;
    }
    else
      ++it;
  }  
}

bool CdClassCanvas::has_relation(BasicData * def) const {
  QListIterator<ArrowCanvas> it(lines);
	
  while (it.current()) {
    if (IsaRelation(it.current()->type()) &&
	(((RelationCanvas *) it.current())->get_data() == def))
      return TRUE;
    ++it;
  }
  
  return FALSE;
}

bool CdClassCanvas::has_inner(DiagramItem * end) const {
  QListIterator<ArrowCanvas> it(lines);
	
  while (it.current()) {
    if ((it.current()->type() == UmlInner) &&
	(((ArrowCanvas *) it.current())->get_end() == end))
      return TRUE;
    ++it;
  }
  
  return FALSE;
}

void CdClassCanvas::draw_all_relations(CdClassCanvas * end) {
  QListViewItem * child;
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator cit;
  
  for (child = browser_node->firstChild(); child; child = child->nextSibling()) {
    if (!((BrowserNode *) child)->deletedp()) {
      UmlCode k = ((BrowserNode *) child)->get_type();
      
      if (IsaRelation(k)) {
	RelationData * def =
	  ((RelationData *) ((BrowserNode *) child)->get_data());
	
	if ((def->get_start_class() == browser_node) && 	// rel begins by this
	    ((end == 0) || (def->get_end_class() == end->browser_node)) &&
	    !has_relation(def)) {
	  // adds it in case the other class is drawed
	  BrowserClass * end_class = 
	    ((BrowserClass *) def->get_end_class());
	  DiagramItem * di;
	  
	  if (end_class == browser_node)
	    di = this;
	  else {	
	    di = 0;
	    for (cit = all.begin(); cit != all.end(); ++cit) {
	      DiagramItem * adi = QCanvasItemToDiagramItem(*cit);
	      
	      if ((adi != 0) &&		// an uml canvas item
		  (adi->type() == UmlClass) &&
		  (((CdClassCanvas *) adi)->browser_node == end_class) &&
		  ((((CdClassCanvas *) adi) == end) || (*cit)->visible())) {
		// other class canvas find
		di = adi;
		break;
	      }
	    }
	  }
	  
	  if (di != 0)
	    (new RelationCanvas(the_canvas(), this, di,
				((BrowserClass *) browser_node), 
				def->get_type(), 0, -1.0, -1.0, def))->show();
	}
      }
      else if (k == UmlClass) {
	for (cit = all.begin(); cit != all.end(); ++cit) {
	  DiagramItem * adi = QCanvasItemToDiagramItem(*cit);
	  
	  if ((adi != 0) &&		// an uml canvas item
	      (adi->type() == UmlClass) &&
	      (((CdClassCanvas *) adi)->browser_node == child) &&
	      ((((CdClassCanvas *) adi) == end) || (*cit)->visible())) {
	    if (! ((CdClassCanvas *) adi)->has_inner(this))
	      (new ArrowCanvas(the_canvas(), adi, this, UmlInner, 0, FALSE, -1.0, -1.0))
		->show();
	    break;
	  }
	}
      }
    }
  }
  
  if ((end == 0) &&
      !DrawingSettings::just_modified() &&
      !on_load_diagram()) {
    for (cit = all.begin(); cit != all.end(); ++cit) {
      DiagramItem * di = QCanvasItemToDiagramItem(*cit);
      
      if ((di != 0) &&	// an uml canvas item
	  (di->type() == UmlClass) &&
	  (((CdClassCanvas *) di) != this) &&
	  !((CdClassCanvas *) di)->browser_node->deletedp() &&
	  ((CdClassCanvas *) di)->visible()) {
	((CdClassCanvas *) di)->draw_all_relations(this);
      }
    }
  }
}

// draw class association being 'this' or from
// a drawn relation to/from 'this'

void CdClassCanvas::draw_all_class_assoc() {
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator cit;

  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible() &&
	isa_arrow(*cit) && 
	IsaRelation(((ArrowCanvas *) *cit)->type())) {
      RelationCanvas * r = (RelationCanvas *) *cit;
      BrowserNode * assoc = 
	((RelationData *) r->get_data())->get_association().type;
      
      if (assoc == browser_node)
	r->show_assoc_class(this);
      else {
	DiagramItem * b;
	DiagramItem * e;
	
	r->extremities(b, e);
	if ((b == this) || (e == this))
	  r->show_assoc_class(0);
      }
    }
  }
}

void CdClassCanvas::check_constraint() {
  // update must be called before
  if (used_settings.show_infonote == UmlYes) {
    ConstraintCanvas * c = 
      ConstraintCanvas::compute(the_canvas(), this, constraint);
    
    if (constraint == 0) {
      constraint = c;
      constraint->upper();
      
      constraint->move((int) (x() + width() + the_canvas()->zoom() * 20),
		       (int) y() + height());
      constraint->show();
      (new ArrowCanvas(the_canvas(), this, constraint, UmlAnchor, 0, FALSE, -1.0, -1.0))->show();
    }
  }
  else if (constraint != 0) {
    constraint->delete_it();
    constraint = 0;
  }
}

void CdClassCanvas::change_scale() {
  QCanvasRectangle::setVisible(FALSE);
  compute_size();
  recenter();
  if (templ != 0)
    templ->update();
  QCanvasRectangle::setVisible(TRUE);
}

void CdClassCanvas::moveBy(double dx, double dy) {
  DiagramCanvas::moveBy(dx, dy);
  if (templ != 0)
    templ->update();
  if ((constraint != 0) && !constraint->selected())
    constraint->moveBy(dx, dy);
}

bool CdClassCanvas::move_with_its_package() const {
  return TRUE;
}

void CdClassCanvas::set_z(double z) {
  setZ(z);
  if (templ)
    templ->update();
}

void CdClassCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  QFontMetrics fbm(the_canvas()->get_font(UmlNormalBoldFont));
  QFontMetrics fbim(the_canvas()->get_font(UmlNormalBoldItalicFont));
  QColor bckgrnd = p.backgroundColor();
  double zoom = the_canvas()->zoom();
  FILE * fp = svg();

  if (fp != 0)
    fputs("<g>\n", fp);

  p.setBackgroundMode((used_color == UmlTransparent) ? ::Qt::TransparentMode : ::Qt::OpaqueMode);

  QColor co = color(used_color);
  
  if (used_view_mode == asClass) {
    if (used_color != UmlTransparent) {
      const int shadow = the_canvas()->shadow();

      if (shadow != 0) {
	r.setRight(r.right() - shadow);
	r.setBottom(r.bottom() - shadow);
	
	p.fillRect (r.right(), r.top() + shadow,
		    shadow, r.height() - 1,
		    ::Qt::darkGray);
	p.fillRect (r.left() + shadow, r.bottom(),
		    r.width() - 1, shadow,
		    ::Qt::darkGray);

	if (fp != 0) {
	  fprintf(fp, "\t<rect fill=\"#%06x\" stroke=\"none\" stroke-opacity=\"1\""
		  " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
		  ::Qt::darkGray.rgb()&0xffffff,
		  r.right(), r.top() + shadow, shadow - 1, r.height() - 1 - 1);

	  fprintf(fp, "\t<rect fill=\"#%06x\" stroke=\"none\" stroke-opacity=\"1\""
		  " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
		  ::Qt::darkGray.rgb()&0xffffff,
		  r.left() + shadow, r.bottom(), r.width() - 1 - 1, shadow - 1);
	}
      }
    }
    
    p.setBackgroundColor(co);
  
    if (used_color != UmlTransparent) {
      p.fillRect(r, co);

      if (fp != 0)
	fprintf(fp, "\t<rect fill=\"%s\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\""
		" x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
		svg_color(used_color), 
		r.x(), r.y(), r.width() - 1, r.height() - 1);
    }
    else if (fp != 0)
      fprintf(fp, "\t<rect fill=\"none\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\""
	      " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
	      r.x(), r.y(), r.width() - 1, r.height() - 1);

    p.drawRect(r);
  }
  
  const ClassData * data = ((ClassData *) browser_node->get_data());
  const int two = (int) (2 * zoom);
  int he = fbm.height() + two;
  
  if (data->get_n_formalparams() != 0)
    r.setTop(r.top() + fbm.height());
  
  switch (used_view_mode) {
  case asControl:
    draw_control_icon(p, r, used_color, zoom);
    r.setTop(r.top() + (int) (CONTROL_HEIGHT * zoom) + two);
    break;
  case asBoundary:
    draw_boundary_icon(p, r, used_color, zoom);
    r.setTop(r.top() + (int) (BOUNDARY_HEIGHT * zoom) + two);
    break;
  case asEntity:
    draw_entity_icon(p, r, used_color, zoom);
    r.setTop(r.top() + (int) (ENTITY_SIZE * zoom) + two);
    break;
  case asActor:
    {
      QRect ra = r;
      
      ra.setHeight((int) (ACTOR_SIZE * zoom));
      ra.setLeft(ra.left() + 
		 (int) ((ra.width() - ACTOR_SIZE * zoom)/2));
      ra.setWidth(ra.height());
      draw_actor(&p, ra);
    }
    r.setTop(r.top() + (int) (ACTOR_SIZE * zoom) + two);
    break;
  default:	// class
    r.setTop(r.top() + two);
    if (data->get_stereotype()[0]) {
      p.setFont(the_canvas()->get_font(UmlNormalFont));
      p.drawText(r, ::Qt::AlignHCenter + ::Qt::AlignTop, 
		 QString("<<") + toUnicode(data->get_stereotype()) + ">>");
      if (fp != 0)
	draw_text(r, ::Qt::AlignHCenter + ::Qt::AlignTop, 
		  QString("<<") + toUnicode(data->get_stereotype()) + ">>",
		  p.font(), fp);
      r.setTop(r.top() + he + two);
    }
  }
  
  p.setBackgroundMode(::Qt::TransparentMode);
  p.setFont((data->get_is_abstract())
	    ? the_canvas()->get_font(UmlNormalBoldItalicFont)
	    : the_canvas()->get_font(UmlNormalBoldFont));
  p.drawText(r, ::Qt::AlignHCenter + ::Qt::AlignTop, full_name);
  if (fp != 0)
    draw_text(r, ::Qt::AlignHCenter + ::Qt::AlignTop, full_name, p.font(), fp);

  p.setFont(the_canvas()->get_font(UmlNormalFont));
  
  r.setTop(r.top() + he);
  if (used_settings.hide_attributes != UmlYes) {
    p.drawLine(r.topLeft(), r.topRight());

    if (fp != 0)
      fprintf(fp, "\t<line stroke=\"black\" stroke-opacity=\"1\""
	      " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
	      r.left(), r.top(), r.right(), r.top());
  }
  
  static const QString v[] = { "+", "#", "-", "~" };
  
  bool have = FALSE;
  int left0 = r.left();
  int left1 = left0 + (int) (4 * zoom);
  int left2 = left1 + fbim.width("#_");
  int space = fm.width("_");
  QListViewItem * child;
  bool full_members = (used_settings.show_full_members_definition == UmlYes);
  bool show_visibility = (used_settings.show_members_visibility == UmlYes);
  bool show_stereotype = (used_settings.show_members_stereotype == UmlYes);
  bool show_multiplicity = (used_settings.show_members_multiplicity == UmlYes);
  bool show_initialization = (used_settings.show_members_initialization == UmlYes);
  bool show_dir = (used_settings.show_parameter_dir == UmlYes);
  bool show_name = (used_settings.show_parameter_name == UmlYes);
  int max_member_width = used_settings.member_max_width;
  
  if (max_member_width == UmlUnlimitedMemberWidth)
    max_member_width = 1000000;
  
  r.setTop(r.top() + two);
  
  if (used_settings.hide_attributes != UmlYes) {
    r.setLeft(left1);
    for (child = browser_node->firstChild(); child; child = child->nextSibling()) {
      if (!((BrowserNode *) child)->deletedp() &&
	  (((BrowserNode *) child)->get_type() == UmlAttribute) &&
	  ((indicate_visible_attr)
	   ? (hidden_visible_attributes.findIndex((BrowserNode *) child) != -1)
	   : (hidden_visible_attributes.findIndex((BrowserNode *) child) == -1))) {
	AttributeData * data =
	  ((AttributeData *) ((BrowserNode *) child)->get_data());
	QString s = data->definition(full_members, show_multiplicity,
				     show_initialization, used_settings.drawing_language);
	
	if (s.isEmpty())
	  continue;
	
	r.setLeft(left1);
	if (show_visibility) {
	  int vi = ((used_settings.drawing_language == CppView) &&
		    (data->get_cpp_visibility() != UmlDefaultVisibility))
	    ? data->get_cpp_visibility() : data->get_uml_visibility();
	  
	  if ((vi == UmlPackageVisibility) &&
	      ((used_settings.drawing_language == CppView) || (used_settings.drawing_language == IdlView)))
	    vi = UmlPublic;
	  
	  p.setFont(the_canvas()->get_font(UmlNormalBoldFont));
	  p.drawText(r, ::Qt::AlignLeft + ::Qt::AlignTop, v[vi]);
	  if (fp != 0)
	    draw_text(r, ::Qt::AlignLeft + ::Qt::AlignTop, v[vi], p.font(), fp);
	  r.setLeft(left2);
	}
	if (show_stereotype) {
	  QString st = data->get_stereotype();
	  
	  if (! st.isEmpty()) {
	    st = "<<" + st + ">>";
	    p.setFont(the_canvas()->get_font(UmlNormalFont));
	    p.drawText(r, ::Qt::AlignLeft + ::Qt::AlignTop, st);
	    if (fp != 0)
	      draw_text(r, ::Qt::AlignLeft + ::Qt::AlignTop, st, p.font(), fp);
	    r.setLeft(r.left() + fm.width(st) + space);
	  }
	}
	p.setFont((data->get_isa_class_attribute()) ? the_canvas()->get_font(UmlNormalUnderlinedFont)
						    : the_canvas()->get_font(UmlNormalFont));  
	if ((int) s.length() >= max_member_width)
	  s = s.left(max_member_width) + "...";
	p.drawText(r, ::Qt::AlignLeft + ::Qt::AlignTop, s);
	if (fp != 0)
	  draw_text(r, ::Qt::AlignLeft + ::Qt::AlignTop, s, p.font(), fp);
	r.setTop(r.top() + he);
	have = TRUE;
      }
    }
    r.setLeft(left0);
  }
  
  if (! have)
    r.setTop(r.top() + (int) (6 * zoom));
  
  if (used_settings.hide_operations != UmlYes) {
    p.drawLine(r.topLeft(), r.topRight());

    if (fp != 0)
      fprintf(fp, "\t<line stroke=\"black\" stroke-opacity=\"1\""
	      " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
	      r.left(), r.top(), r.right(), r.top());
  }
  
  r.setLeft(left1);
  r.setTop(r.top() + two);
  
  if (used_settings.hide_operations != UmlYes) {
    for (child = browser_node->firstChild(); child; child = child->nextSibling()) {
      if (!((BrowserNode *) child)->deletedp() &&
	  (((BrowserNode *) child)->get_type() == UmlOperation) &&
	  ((indicate_visible_oper)
	   ? (hidden_visible_operations.findIndex((BrowserNode *) child) != -1)
	   : (hidden_visible_operations.findIndex((BrowserNode *) child) == -1))) {
	OperationData * data =
	  ((OperationData *) ((BrowserNode *) child)->get_data());
	QString s = data->definition(full_members, used_settings.drawing_language,
				     show_dir, show_name);
	
	if (!s.isEmpty()) {
	  r.setLeft(left1);
	  if (show_visibility) {
	    int vi = ((used_settings.drawing_language == CppView) &&
		      (data->get_cpp_visibility() != UmlDefaultVisibility))
	      ? data->get_cpp_visibility() : data->get_uml_visibility();
	    
	    if ((vi == UmlPackageVisibility) &&
		((used_settings.drawing_language == CppView) || (used_settings.drawing_language == IdlView)))
	      vi = UmlPublic;
	    
	    p.setFont(the_canvas()->get_font(UmlNormalBoldFont));
	    p.drawText(r, ::Qt::AlignLeft + ::Qt::AlignTop, v[vi]);
	    if (fp != 0)
	      draw_text(r, ::Qt::AlignLeft + ::Qt::AlignTop, v[vi], p.font(), fp);
	    r.setLeft(left2);
	  }
	  if (show_stereotype) {
	    QString st = data->get_stereotype();
	    
	    if (! st.isEmpty()) {
	      st = "<<" + st + ">>";
	      p.setFont(the_canvas()->get_font(UmlNormalFont));
	      p.drawText(r, ::Qt::AlignLeft + ::Qt::AlignTop, st);
	      if (fp != 0)
		draw_text(r, ::Qt::AlignLeft + ::Qt::AlignTop, st, p.font(), fp);
	      r.setLeft(r.left() + fm.width(st) + space);
	    }
	  }
	  if (data->get_isa_class_operation())
	    p.setFont(the_canvas()->get_font(UmlNormalUnderlinedFont));
	  else if (data->get_is_abstract())
	    p.setFont(the_canvas()->get_font(UmlNormalItalicFont));
	  else
	    p.setFont(the_canvas()->get_font(UmlNormalFont));
	  if ((int) s.length() >= max_member_width)
	    s = s.left(max_member_width) + "...";
	  p.drawText(r, ::Qt::AlignLeft + ::Qt::AlignTop, s);
	  if (fp != 0)
	    draw_text(r, ::Qt::AlignLeft + ::Qt::AlignTop, s, p.font(), fp);
	  r.setTop(r.top() + he);
	}
      }
    }
  }
      
  if (fp != 0)
    fputs("</g>\n", fp);
    
  p.setBackgroundColor(bckgrnd);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode CdClassCanvas::type() const {
  return UmlClass;
}

void CdClassCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool CdClassCanvas::alignable() const {
  return TRUE;
}

bool CdClassCanvas::copyable() const {
  return selected();
}

void CdClassCanvas::open() {
  browser_node->open(FALSE);
}

void CdClassCanvas::menu(const QPoint&) {
  QList<BrowserOperation> l = 
    ((BrowserClass *) browser_node)->inherited_operations(21);
  QPopupMenu m(0);
  QPopupMenu gensubm(0);
  QPopupMenu attrsubm(0);
  QPopupMenu opersubm(0);
  QPopupMenu inhopersubm(0);
  QPopupMenu toolm(0);
  const char * stereotype = browser_node->get_data()->get_stereotype();
  BrowserNodeList attributes;
  BrowserNodeList operations;
  QStringList attributes_def;
  QStringList operations_def;
  int index;
  
  browser_node->children(attributes, UmlAttribute);
  browser_node->children(operations, UmlOperation);
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 2);
  if (attributes.count() != 0)
    m.insertItem("Individual attribute visibility", 3);
  if (operations.count() != 0)
    m.insertItem("Individual operation visibility", 4);
  m.insertSeparator();
  m.insertItem("Edit class", 6);
  if (!strcmp(stereotype, "enum")) {
    if (browser_node->is_writable()) {
      m.insertItem("Add item", 7);
      m.insertItem("Add attribute", 9);
    }
    if (attributes.count() != 0)
      m.insertItem("Edit item or attribute", &attrsubm);
    if (browser_node->is_writable())
      m.insertItem("Add operation", 8);
  }
  else if (!strcmp(stereotype, "enum_pattern")) {
    if (browser_node->is_writable()) {
      m.insertItem("Add item", 7);
    }
    if (attributes.count() != 0)
      m.insertItem("Edit item", &attrsubm);
  }
  else if (strcmp(stereotype, "typedef")) {
    if (browser_node->is_writable())
      m.insertItem("Add attribute", 9);
    if (attributes.count() != 0)
      m.insertItem("Edit attribute", &attrsubm);
    if (browser_node->is_writable())
      m.insertItem("Add operation", 8);
    if (browser_node->is_writable() &&
	strcmp(stereotype, "union") &&
	(l.count() != 0)) {
      if (l.count() > 20)
	m.insertItem("Add inherited operation", 2999);
      else {
	BrowserOperation * oper;
	
	inhopersubm.insertItem(new MenuTitle("Choose operation", m.font()), -1);
	inhopersubm.insertSeparator();
	
	for (oper = l.first(), index = 3000;
	     oper;
	     oper = l.next(), index += 1) {
	  if (((OperationData *) oper->get_data())->get_is_abstract())
	    inhopersubm.insertItem(new MenuItalic(((BrowserNode *) oper->parent())->get_name() +
						  QString("::") + oper->get_data()->definition(TRUE),
						  inhopersubm.font()),
				   index);
	  else
	    inhopersubm.insertItem(((BrowserNode *) oper->parent())->get_name() +
				   QString("::") + oper->get_data()->definition(TRUE),
				   index);
	}
	
	m.insertItem("Add inherited operation", &inhopersubm);
      }
    }
    
    if (operations.count() != 0) {
      if (operations.count() <= 20)
	m.insertItem("Edit operation", &opersubm);
      else
	m.insertItem("Edit operation", 1999);
    }
  }
  m.insertSeparator();
  m.insertItem("Select in browser", 10);
  if (linked())
    m.insertItem("Select linked items",17);
  m.insertSeparator();
  if (browser_node->is_writable()) {
    m.insertItem("Set associated diagram",11);
    
    if (browser_node->get_associated())
      m.insertItem("Remove diagram association",18);
  }
  m.insertSeparator();
  m.insertItem("Remove from view",12);
  if (browser_node->is_writable())
    m.insertItem("Delete from model", 13);
  m.insertSeparator();
  bool cpp = GenerationSettings::cpp_get_default_defs();
  bool java = GenerationSettings::java_get_default_defs();
  bool php = GenerationSettings::php_get_default_defs();
  bool python = GenerationSettings::python_get_default_defs();
  bool idl = GenerationSettings::idl_get_default_defs();

  if (cpp || java || php || python || idl)
    m.insertItem("Generate", &gensubm);
  
  if (Tool::menu_insert(&toolm, UmlClass, 30))
    m.insertItem("Tool", &toolm);
  
  if (cpp)
    gensubm.insertItem("C++", 14);
  if (java)
    gensubm.insertItem("Java", 15);
  if (php)
    gensubm.insertItem("Php", 19);
  if (python)
    gensubm.insertItem("Python", 20);
  if (idl)
    gensubm.insertItem("Idl", 16);
  
  QStringList::Iterator it;
  
  if (attributes.count() != 0) {
    attributes.full_defs(attributes_def);
    index = 1000;
    
    for (it = attributes_def.begin(); it != attributes_def.end(); ++it)
      attrsubm.insertItem(*it, index++);
  }
  
  if ((operations.count() != 0) && (operations.count() <= 20)) {
    operations.full_defs(operations_def);
    index = 2000;
    
    for (it = operations_def.begin(); it != operations_def.end(); ++it)
      opersubm.insertItem(*it, index++);
  }
  
  switch (index = m.exec(QCursor::pos())) {
  case 0:
    upper();
    break;
  case 1:
    lower();
    break;
  case 2:
    edit_drawing_settings();
    return;
  case 3:
    {
      bool visible = indicate_visible_attr;
      HideShowDialog dialog(attributes, hidden_visible_attributes, visible);
      
      dialog.raise();
      if (dialog.exec() != QDialog::Accepted)
	return;
      indicate_visible_attr = visible;
    }
    break;
  case 4:
    {
      bool visible = indicate_visible_oper;
      HideShowDialog dialog(operations, hidden_visible_operations, visible);
      
      dialog.raise();
      if (dialog.exec() != QDialog::Accepted)
	return;
      indicate_visible_oper = visible;
    }
    break;
  case 6:
    browser_node->open(TRUE);
    return;
  case 7:
    browser_node->apply_shortcut("New item");
    return;
  case 9:
    browser_node->apply_shortcut("New attribute");
    return;
  case 8:
    browser_node->apply_shortcut("New operation");
    return;
  case 10:
    browser_node->select_in_browser();
    return;
  case 11:
    ((BrowserClass *) browser_node)->set_associated_diagram((BrowserClassDiagram *)
							    the_canvas()->browser_diagram());
    return;
  case 18:
    ((BrowserClass *) browser_node)
      ->set_associated_diagram(0);
    return;
  case 12:
    //remove from view
    delete_it();
    break;
  case 13:
    //delete from model
    browser_node->delete_it();	// will delete the canvas
    break;
  case 14:
    browser_node->apply_shortcut("Generate C++");
    return;
  case 15:
    browser_node->apply_shortcut("Generate Java");
    return;
  case 16:
    browser_node->apply_shortcut("Generate Idl");
    return;
  case 17:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 19:
    browser_node->apply_shortcut("Generate Php");
    return;
  case 20:
    browser_node->apply_shortcut("Generate Python");
    return;
  case 1999:
    {
      OperationListDialog dialog("Choose operation to edit", 
				 (QList<BrowserOperation> &) operations);
      
      dialog.raise();
      if (dialog.exec() == QDialog::Accepted)
	operations.at(dialog.choosen())->open(FALSE);
    }
    return;
  case 2999:
    {
      l = ((BrowserClass *) browser_node)->inherited_operations(~0u);
      
      OperationListDialog dialog("Choose inherited operation to add it", l);
      
      dialog.raise();
      if (dialog.exec() == QDialog::Accepted)
	((BrowserClass *) browser_node)->add_inherited_operation(l.at(dialog.choosen()));
    }
    return;
  default:
    if (index >= 3000)
      // inherited operation
      ((BrowserClass *) browser_node)->add_inherited_operation(l.at(index - 3000));
    else if (index >= 2000)
      // operation
      operations.at(index - 2000)->open(FALSE);
    else if (index >= 1000)
      // attribute
      attributes.at(index - 1000)->open(FALSE);
    else if (index >= 30)
      ToolCom::run(Tool::command(index - 30), browser_node);
    return;
  }
  
  modified();
  package_modified();
}

void CdClassCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser") {
    browser_node->select_in_browser();
    return;
  }
  else if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Edit drawing settings") {
    edit_drawing_settings();
    return;
  }
  else {
    browser_node->apply_shortcut(s);
    return;
  }

  modified();
  package_modified();
}

void CdClassCanvas::edit_drawing_settings()  {
  QArray<StateSpec> st;
  QArray<ColorSpec> co(1);
  
  settings.complete(st, UmlClass);
  
  co[0].set("class color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    modified();
    package_modified();
  }
}

bool CdClassCanvas::has_drawing_settings() const {
  return TRUE;
}

void CdClassCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<StateSpec> st;
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  ClassDiagramSettings settings;
  
  settings.complete(st, UmlClass);
  
  co[0].set("class color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      if (co[0].name != 0)
	((CdClassCanvas *) it.current())->itscolor = itscolor;
      ((CdClassCanvas *) it.current())->settings.set(st, 0);
      ((CdClassCanvas *) it.current())->modified();
      ((CdClassCanvas *) it.current())->package_modified();
    }
  }  
}

const char * CdClassCanvas::may_start(UmlCode & l) const {
  return ((BrowserClass *) browser_node)->may_start(l);
}

const char * CdClassCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  if (l == UmlAnchor)
    return (IsaRelation(dest->type()))
      ? ((RelationCanvas *) dest)->may_connect(l, this)
      : dest->may_start(l);
  
  switch (dest->type()) {
  case UmlClass:
    return ((BrowserClass *) browser_node)
      ->may_connect(l, (BrowserClass *) dest->get_bn());
  case UmlArrowPoint:
    return "illegal";
  case UmlPackage:
    if (l != UmlDependency)
      return "illegal";
    l = UmlDependOn;
    return 0;
  default:
    return "illegal";
  }
}

void CdClassCanvas::post_connexion(UmlCode l, DiagramItem * dest) {
  if ((l == UmlAnchor) && IsaRelation(dest->type()))
    ((RelationCanvas *) dest)->post_connexion(l, this);
}

void CdClassCanvas::connexion(UmlCode action, DiagramItem * dest,
			      const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (IsaRelation(action))
    a = new RelationCanvas(the_canvas(), this, dest, 0, action, 0, -1.0, -1.0);
  else if (IsaSimpleRelation(action))
    a = new SimpleRelationCanvas(the_canvas(), this, dest, 0, action, 0, -1.0, -1.0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE, -1.0, -1.0);
  
  a->show();
  the_canvas()->select(a);
}

//

static void save_hidden_list(BrowserNode * bn, UmlCode c, QTextStream & st,
			     const char * s,
			     const QValueList<BrowserNode *> & hidden_visible)
{
  BrowserNodeList l;
  
  bn->children(l, c);
  
  QListIterator<BrowserNode> it(l);
  
  while (it.current() != 0) {
    QString dummy;
  
    if (hidden_visible.findIndex(it.current()) != -1) {
      if (s != 0) {
	nl_indent(st);
	st << s;
	s = 0;
      }
      nl_indent(st);
      st << "  ";
      it.current()->save(st, TRUE, dummy);
    }
    
    ++it;
  }
}

void CdClassCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "classcanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "classcanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    settings.save(st);
    if (itscolor != UmlDefaultColor) {
      nl_indent(st);
      st << "color " << stringify(itscolor);
    }
    save_hidden_list(browser_node, UmlAttribute, st,
		     (indicate_visible_attr) ? "visible_attributes"
					     : "hidden_attributes",
		     hidden_visible_attributes);
    save_hidden_list(browser_node, UmlOperation, st,
		     (indicate_visible_oper) ? "visible_operations"
					     : "hidden_operations",
		     hidden_visible_operations);
    nl_indent(st);
    save_xyz(st, this, "xyz");
    
    if (constraint != 0)
      constraint->save(st, FALSE, warning);

    nl_indent(st);
    st << "end";
    
    indent(-1);
  }
}

CdClassCanvas * CdClassCanvas::read(char * & st, UmlCanvas * canvas,
				    char * k)
{
  if (!strcmp(k, "classcanvas_ref"))
    return ((CdClassCanvas *) dict_get(read_id(st), "classcanvas", canvas));
  else if (!strcmp(k, "classcanvas")) {
    int id = read_id(st);
    BrowserClass * br = BrowserClass::read_ref(st);
    CdClassCanvas * result = new CdClassCanvas(canvas, id);
    
    result->browser_node = br;
    result->subscribe(br->get_data());	// = TRUE
    connect(br->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(br->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));

    k = read_keyword(st);
    
    result->settings.read(st, k);	// updates k
    if (result->settings.draw_all_relations == UmlNo)
      // old release
      canvas->dont_draw_all_relations();
    result->settings.draw_all_relations = UmlDefaultState;

    read_color(st, "color", result->itscolor, k);	// updates k
    
    if (!strcmp(k, "hidden_attributes") || !strcmp(k, "visible_attributes")) {
      result->indicate_visible_attr = (*k == 'v');
	
      BrowserNodeList l;
    
      br->children(l, UmlAttribute);
      
      while ((strcmp(k = read_keyword(st), "hidden_operations")) &&
	     (strcmp(k, "visible_operations")) &&
	     (strcmp(k, "xyz"))) {
	BrowserNode * b = BrowserAttribute::read(st, k, 0, FALSE);
	
	if ((b != 0) && (l.find(b) != -1))
	  result->hidden_visible_attributes.append(b);
      }
    }
    
    if (!strcmp(k, "hidden_operations") || !strcmp(k, "visible_operations")) {
      result->indicate_visible_oper = (*k == 'v');
      
      BrowserNodeList l;
    
      br->children(l, UmlOperation);
      
      while (strcmp(k = read_keyword(st), "xyz")) {
	BrowserNode * b = BrowserOperation::read(st, k, 0, FALSE);
	
	if ((b != 0) && (l.find(b) != -1))
	  result->hidden_visible_operations.append(b);
      }
    }
    
    if (strcmp(k, "xyz"))
      wrong_keyword(k, "xyz");
    read_xyz(st, result);
    
    if (read_file_format() >= 37) {
      k = read_keyword(st);
      if (! strcmp(k, "constraint")) {
	result->constraint = ConstraintCanvas::read(st, canvas, k, result);
	k = read_keyword(st);
      }
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    result->compute_size();
    result->set_center100();
    result->show();
    result->check_constraint();
    return result;
  }
  else 
    return 0;
}

void CdClassCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  compute_size();
}

void CdClassCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  
  MultipleDependencyIterator<BasicData> it(this);
  
  while (it.current()) {
    disconnect(it.current(), 0, this, 0);
    ++it;
  }
  
  unsubscribe_all();
}
