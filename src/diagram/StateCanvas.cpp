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

#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "StateCanvas.h"
#include "TransitionCanvas.h"
#include "SimpleRelationCanvas.h"
#include "StateData.h"
#include "BrowserState.h"
#include "BrowserStateDiagram.h"
#include "BrowserRegion.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"
#include "strutil.h"

StateCanvas::StateCanvas(BrowserNode * bn, UmlCanvas * canvas,
			 int x, int y)
    : DiagramCanvas(0, canvas, x, y, STATE_CANVAS_MIN_SIZE, 1, 0) {
  browser_node = bn;
  itscolor = UmlDefaultColor;
  
  compute_size();
  
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

StateCanvas::StateCanvas(UmlCanvas * canvas, int id)
    : DiagramCanvas(canvas, id) {
  // for read operation
  browser_node = 0;
  itscolor = UmlDefaultColor;
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

StateCanvas::~StateCanvas() {
}

void StateCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  
  DiagramCanvas::delete_it();
}

void StateCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void StateCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void StateCanvas::compute_size() {
  double zoom = the_canvas()->zoom();
  QFontMetrics fm(the_canvas()->get_font(UmlNormalBoldFont));
  const BasicData * data = browser_node->get_data();
  StateDrawingSettings st = settings;
  
  the_canvas()->browser_diagram()
    ->get_statedrawingsettings(st);
  region_horizontally = st.region_horizontally == UmlYes;
  
  min_height = 2*fm.height();
  min_width = fm.width(browser_node->get_name());
    
  if (data->get_stereotype()[0] != 0) {
    int w = fm.width(QString("<<") + toUnicode(data->get_stereotype()) + ">>");
    
    if (min_width < w)
      min_width = w;
    
    min_height += fm.height();
  }
    
  activities = QString::null;
  if (st.show_activities == UmlYes) {
    const StateBehavior & behavior = 
      ((StateData *) browser_node->get_data())
	->get_behavior(st.drawing_language);
    QString sep;
    
    if (! behavior.on_entry.isEmpty()) {
      activities = "entry / " + behavior.on_entry;
      sep = "\n";
    }
    
    if (! behavior.on_exit.isEmpty()) {
      activities += sep + "exit / " + behavior.on_exit;
      sep = "\n";
    }
    
    if (! behavior.do_activity.isEmpty())
      activities += sep + "do / " + behavior.do_activity;
    
    if (!activities.isEmpty()) {
      activities = toUnicode(activities);
      
      QSize sz = fm.size(0, activities);
      
      min_height += fm.height() + sz.height();
      
      int w = sz.width() + (int) (16 * zoom);
      
      if (min_width < w)
	min_width = w;
    }
  }
    
  min_width += (int) (16 * zoom);
  
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlState)
    : itscolor;
  
  if (used_color != UmlTransparent) {
    const int shadow = the_canvas()->shadow();
    
    min_width += shadow;
    min_height += shadow;
  }

  // force odd width and height for line alignment
  min_width |= 1;
  min_height |= 1;
    
  DiagramCanvas::resize((width() > min_width) ? width() : min_width,
			(height() > min_height) ? height() : min_height);
}

void StateCanvas::prepare_for_move(bool on_resize) {
  if (! on_resize) {
    DiagramCanvas::prepare_for_move(on_resize);
    
    // select sub states, even through regions
    QCanvasItemList all = canvas()->allItems();
    QCanvasItemList::Iterator cit;
    
    for (cit = all.begin(); cit != all.end(); ++cit) {
      if ((*cit)->visible() && !(*cit)->selected()) {
	DiagramItem * di = QCanvasItemToDiagramItem(*cit);
	
	if ((di != 0) &&
	    (di->type() == UmlState) &&
	    ((BrowserState *) di->get_bn())->sub_state_of((BrowserState*) browser_node))
	  the_canvas()->select(*cit);
      }
    }
  }
}

void StateCanvas::change_scale() {
  QCanvasRectangle::setVisible(FALSE);
  double scale = the_canvas()->zoom();
    
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  compute_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void StateCanvas::modified() {
  hide();
  hide_lines();
  compute_size();
  show();
  update_show_lines();
  force_self_rel_visible();
  canvas()->update();
  force_sub_states_inside();
  package_modified();
}

void StateCanvas::connexion(UmlCode action, DiagramItem * dest,
			    const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (action == UmlTransition)
    a = new TransitionCanvas(the_canvas(), this, dest, 0, 0);
  else if (IsaSimpleRelation(action))
    a = new SimpleRelationCanvas(the_canvas(), this, dest, 0, action, 0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

aCorner StateCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void StateCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width, min_height);
  
  force_sub_states_inside();
}

void StateCanvas::force_sub_states_inside() {
  // update sub states (even through region) position to be
  // inside of the state / region
  
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator cit;
  
  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible() && !(*cit)->selected()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*cit);
      
      if ((di != 0) && (di->type() == UmlState)) {
	StateCanvas * sst = (StateCanvas *) di;	
	BrowserNode * parent =
	  (BrowserNode *) sst->browser_node->parent();
	QRect r;
	
	if (parent == browser_node)
	  r = regions_rect[0];
	else if ((parent->get_type() == UmlRegion) &&
		 (parent->parent() == browser_node)) {
	  if (regions.size() == 0)
	    continue;
	  r = region_rect((BrowserRegion *) parent);
	}
	else
	  continue;
	
	QRect ssr = sst->rect();
	int dx = 0;
	int dy = 0;
	
	if (ssr.left() < r.left()) {
	  if (ssr.right() <= r.right())
	    dx = r.left() - ssr.left();
	}
	else if (ssr.right() > r.right())
	  dx = r.right() - ssr.right();
	
	if (ssr.top() < r.top()) {
	  if (ssr.bottom() <= r.bottom())
	    dy = r.top() - ssr.top();
	}
	else if (ssr.bottom() > r.bottom())
	  dy = r.bottom() - ssr.bottom();
	
	if ((dx != 0) || (dy != 0)) {
	  (*cit)->moveBy(dx, dy);
	  sst->force_sub_states_inside();
	}
      }
    }
  }
}

void StateCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  QRect re = r;    
  QBrush brsh = p.brush();
  
  if (used_color != UmlTransparent) {
    const int shadow = the_canvas()->shadow();
    
    if (shadow != 0) {
      r.setRight(r.right() - shadow);
      r.setBottom(r.bottom() - shadow);
      p.setPen(QObject::NoPen);
      p.setBrush(QObject::darkGray);
      p.drawRoundRect(r.left() + shadow, r.top() + shadow, r.width(), r.height());
      p.setPen(QObject::SolidLine);
    }
  }
  
  QColor bckgrnd = p.backgroundColor();
  QFontMetrics fm(the_canvas()->get_font(UmlNormalBoldFont));
  const BasicData * data = browser_node->get_data();
  
  p.setBackgroundMode((used_color == UmlTransparent)
		      ? QObject::TransparentMode
		      : QObject::OpaqueMode);

  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  
  if (used_color != UmlTransparent) 
    p.setBrush(co);
  
  p.drawRoundRect(r);
  
  p.setFont(the_canvas()->get_font(UmlNormalBoldFont));
  r.setTop(r.top() + fm.height() / 2);
  p.drawText(r, QObject::AlignHCenter, browser_node->get_name());  
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  r.setTop(r.top() + fm.height());
  
  if (data->get_stereotype()[0] != 0) {
    p.drawText(r, QObject::AlignHCenter,
	       QString("<<") + toUnicode(data->get_stereotype()) + ">>");
    r.setTop(r.top() + fm.height());
  }
  
  int sixteen = (int) (16 * the_canvas()->zoom());
  
  r.setLeft(r.left() + sixteen);
  r.setRight(r.right() - sixteen);
  
  if (! activities.isEmpty()) {
    r.setTop(r.top() + fm.height()/2);
    p.drawLine(r.topLeft(), r.topRight());
    r.setTop(r.top() + fm.height()/2);
    p.drawText(r, 0, activities);
    
    QSize sz = fm.size(0, activities);
    
    r.setTop(r.top() + sz.height());
  }
  
  r.setTop(r.top() + fm.height() / 2);  
  
  int nregion = 0;
  QListViewItem * child = browser_node->firstChild();
  
  while (child != 0) {
    if (!((BrowserNode *) child)->deletedp() &&
	(((BrowserNode *) child)->get_type() == UmlRegion))
      nregion += 1;
    child = child->nextSibling();
  }
  
  if (nregion != 0) {
    int szreg = ((region_horizontally) ? r.height() : r.width())
      / nregion;
    
    if (szreg > fm.height()) {
      regions.resize(nregion);
      regions_rect.resize(nregion);
      p.drawLine(r.topLeft(), r.topRight());
      p.setPen(QObject::DashLine);
      
      child = browser_node->firstChild();
      
      for (;;) {
	while (((BrowserNode *) child)->deletedp() ||
	       (((BrowserNode *) child)->get_type() != UmlRegion))
	  child = child->nextSibling();
	
	// child
	regions[--nregion] = (BrowserRegion *) child;
	
	QRect & rr = regions_rect[nregion];
	  
	if (region_horizontally) {
	  rr.setTop(r.top());
	  r.setTop(r.top() + szreg);
	  rr.setBottom(r.top());
	  rr.setLeft(re.left());
	  rr.setRight(re.right());
	}
	else {
	  rr.setTop(r.top());
	  rr.setBottom(re.bottom());
	  rr.setLeft(r.left());
	  r.setLeft(r.left() + szreg);
	  rr.setRight(r.left());
	}
	
	if (nregion == 0)
	  break;
	
	if (region_horizontally)
	  p.drawLine(r.topLeft(), r.topRight());
	else
	  p.drawLine(r.topLeft(), r.bottomLeft());
	
	child = child->nextSibling();
      }
      
      p.setPen(QObject::SolidLine);
    }
    else {
      regions.resize(0);
      regions_rect.resize(1);
      regions_rect[0] = r;
    }
  }
  else {
    regions.resize(0);
    regions_rect.resize(1);
    regions_rect[0] = r;
  }
    
  p.setBackgroundColor(bckgrnd);
  p.setBrush(brsh);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode StateCanvas::type() const {
  return UmlState;
}

BrowserRegion * StateCanvas::pointed_region(const QPoint & p) const {
  // warning : without region, region_rect.size() == 1
  for (unsigned i = 0; i != regions.size(); i += 1)
    if (regions_rect.at(i).contains(p))
      return regions.at(i);

  return 0;
}						     

QRect StateCanvas::region_rect(BrowserRegion * r) {
  // warning : without region, region_rect.size() == 1
  for (unsigned i = 0; i != regions.size(); i += 1)
    if (regions.at(i) == r)
      return regions_rect.at(i);
  
  QRect re;
  
  return re;
}

void StateCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool StateCanvas::alignable() const {
  return TRUE;
}

bool StateCanvas::copyable() const {
  return selected();
}

void StateCanvas::open() {
  browser_node->open(FALSE);
}

void StateCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  int index;
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("upper", 0);
  m.insertItem("lower", 1);
  m.insertSeparator();
  m.insertItem("edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("edit state", 3);
  m.insertSeparator();
  m.insertItem("select in browser", 4);
  if (linked())
    m.insertItem("select linked items", 5);
  m.insertSeparator();
  if (browser_node->is_writable())
    m.insertItem("set associated diagram",6);
  m.insertSeparator();
  m.insertItem("remove from view", 7);
  if (browser_node->is_writable())
    m.insertItem("delete from model", 8);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, UmlState, 10))
    m.insertItem("tool", &toolm);
  
  switch (index = m.exec(QCursor::pos())) {
  case 0:
    upper();
    modified();	// call package_modified()
    return;
  case 1:
    lower();
    modified();	// call package_modified()
    return;
  case 2:
    {
      QArray<StateSpec> st(3);
      QArray<ColorSpec> co(1);
      
      st[0].set("show activities", &settings.show_activities);
      st[1].set("draw regions horizontally", &settings.region_horizontally);
      st[2].set("drawing language", &settings.drawing_language);
      
      co[0].set("state color", &itscolor);

      SettingsDialog dialog(&st, &co, FALSE, TRUE);
      
      if (dialog.exec() == QDialog::Accepted)
	modified();	// call package_modified()
    }
    return;
  case 3:
    browser_node->open(TRUE);
    return;
  case 4:
    browser_node->select_in_browser();
    return;
  case 5:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 6:
    ((BrowserState *) browser_node)
      ->set_associated_diagram((BrowserStateDiagram *)
			       the_canvas()->browser_diagram());
    return;
  case 7:
    //remove from view
    delete_it();
    break;
  case 8:
    //delete from model
    browser_node->delete_it();	// will delete the canvas
    break;
  default:
    if (index >= 10)
      ToolCom::run(Tool::command(index - 10), browser_node);
    return;
  }
  
  package_modified();
}

const char * StateCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlAnchor:
  case UmlTransition:
    return 0;
  default:
    return "illegal";
  }
}

const char * StateCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlState:
  case UmlStateAction:
  case EntryPointPS:
  case FinalPS:
  case TerminatePS:      
  case ExitPointPS:
  case DeepHistoryPS:
  case ShallowHistoryPS:
  case JunctionPS:
  case ChoicePS:
  case ForkPS:
  case JoinPS:
    return (l == UmlTransition) ? 0 : "illegal";
  case UmlNote:
  case UmlIcon:
    return (l == UmlAnchor) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

void StateCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "statecanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "statecanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    settings.save(st);
    if (itscolor != UmlDefaultColor) {
      nl_indent(st);
      st << "color " << stringify(itscolor);
    }
    nl_indent(st);
    save_xyzwh(st, this, "xyzwh");
    indent(-1);
  }
}

StateCanvas * StateCanvas::read(char * & st, UmlCanvas * canvas,
					char * k)
{
  if (!strcmp(k, "statecanvas_ref"))
    return ((StateCanvas *) dict_get(read_id(st), "statecanvas", canvas));
  else if (!strcmp(k, "statecanvas")) {
    int id = read_id(st);
    BrowserState * br = BrowserState::read_ref(st);
    StateCanvas * result = new StateCanvas(canvas, id);
    
    result->browser_node = br;
    connect(br->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(br->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));

    k = read_keyword(st);
    
    result->settings.read(st, k);	// updates k
    read_color(st, "color", result->itscolor, k);	// updates k
    
    if (!strcmp(k, "xyz"))
      read_xyz(st, result);
    else if (!strcmp(k, "xyzwh"))
      read_xyzwh(st, result);
    else
      wrong_keyword(k, "xyzwh");
    result->compute_size();
    result->set_center100();
    result->show();
    return result;
  }
  else 
    return 0;
}

void StateCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(min_width, b);
  ::save(min_height, b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
  
  QRect * r = regions_rect.data();
  
  if (r != 0) {
    for (unsigned i = 0; i != regions_rect.size(); i++) {
      ::save(r->topLeft(), b);
      ::save(r->bottomRight(), b);
      r += 1;
    }
  }  
}

void StateCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(min_width, b);
  ::load(min_height, b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
  
  QRect * r = regions_rect.data();
  
  if (r != 0) {
    for (unsigned i = 0; i != regions_rect.size(); i++) {
      QPoint p;
      
      ::load(p, b);
      r->moveTopLeft(p);
      ::load(p, b);
      r->moveBottomRight(p);
      r += 1;
    }
  }
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));  
}

void StateCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
}

