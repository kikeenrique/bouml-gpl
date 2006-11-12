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

#include "ActivityCanvas.h"
#include "ActivityData.h"
#include "BrowserActivity.h"
#include "BrowserActivityDiagram.h"
#include "ArrowCanvas.h"
#include "FlowCanvas.h"
#include "SimpleRelationCanvas.h"
#include "BrowserParameter.h"
#include "ParameterCanvas.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"
#include "strutil.h"

ActivityCanvas::ActivityCanvas(BrowserNode * bn, UmlCanvas * canvas,
			       int x, int y)
    : ActivityContainerCanvas(0, canvas, x, y, ACTIVITY_CANVAS_MIN_SIZE * 10,
			      ACTIVITY_CANVAS_MIN_SIZE * 10, 0) {
  browser_node = bn;
  itscolor = UmlDefaultColor;

  compute_size();
  check_params();
  
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

ActivityCanvas::ActivityCanvas(UmlCanvas * canvas, int id)
    : ActivityContainerCanvas(canvas, id) {
  // for read operation
  browser_node = 0;
  itscolor = UmlDefaultColor;
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

ActivityCanvas::~ActivityCanvas() {
}

void ActivityCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  
  DiagramCanvas::delete_it();
  
  QValueList<ParameterCanvas *>::Iterator iter;
  
  for (iter = params.begin();  iter != params.end(); ++iter)
    // don't empty params to manage undo
    (*iter)->delete_it();
}

void ActivityCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void ActivityCanvas::deleted(ParameterCanvas * pa) {
  params.remove(pa);
}

void ActivityCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void ActivityCanvas::compute_size() {
  double zoom = the_canvas()->zoom();
  int margin = (int) (24 * zoom);
  QFontMetrics fm(the_canvas()->get_font(UmlNormalBoldFont));
  const ActivityData * data = (ActivityData *) browser_node->get_data();
  ActivityDrawingSettings st = settings;
  
  the_canvas()->browser_diagram()->get_activitydrawingsettings(st);
  min_width = fm.width(browser_node->get_name()) + 2*margin;
    
  int x0 = min_width;
  int y0 = margin;
  int addw = 0;
  int w;
  
  if (data->get_is_single_execution()) {
    singleexec_offset.setX(x0);
    singleexec_offset.setY(y0);
    addw = fm.width("<<singleExecution>>");
    y0 += fm.height() + 2;
  }
  else
    singleexec_offset.setX(0);
    
  if (data->get_is_read_only()) {
    readonly_offset.setX(x0);
    readonly_offset.setY(y0);
    if (addw == 0)
      addw = fm.width("<<readOnly>>");
    y0 += fm.height() + 2;
  }
  else
    readonly_offset.setX(0);

  if (st.show_infonote == UmlYes) {
    post_width = fm.width("<<postcondition>>##");
    
    pre = data->get_precond(st.drawing_language);
    if (! pre.isEmpty()) {
      pre = toUnicode(pre);
      pre_offset.setX(x0);
      pre_offset.setY(y0);

      QSize sz = fm.size(0, pre);

      w = post_width + sz.width();
      if (w > addw)
	addw = w;
      y0 += sz.height() + 2;
    }
    
    post = data->get_postcond(st.drawing_language);
    if (! post.isEmpty()) {
      post = toUnicode(post);
      post_offset.setX(x0);
      post_offset.setY(y0);

      QSize sz = fm.size(0, post);

      w = post_width + sz.width();
      if (w > addw)
	addw = w;
      y0 += sz.height() + 2;
    }
  }
  else
    pre = post = "";
  
  if (addw != 0)
    min_width += addw + margin;
  
  min_height = y0 + margin + fm.height();
    
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlActivity)
    : itscolor;

  DiagramCanvas::resize((width() > min_width) ? width() : min_width,
			(height() > min_height) ? height() : min_height);

  if (! the_canvas()->do_zoom()) {
    // update parameters position
    QValueList<ParameterCanvas *>::Iterator iter;
  
    for (iter = params.begin(); iter != params.end(); ++iter)
      (*iter)->check_position();
  }
}

void ActivityCanvas::moveBy(double dx, double dy) {
  DiagramCanvas::moveBy(dx, dy);
  
  // update parameters position
  QValueList<ParameterCanvas *>::Iterator iter;
  
  for (iter = params.begin(); iter != params.end(); ++iter)
    (*iter)->do_moveBy(dx, dy);
}

void ActivityCanvas::change_scale() {
  QCanvasRectangle::setVisible(FALSE);
  double scale = the_canvas()->zoom();
    
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  compute_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);
  
  // update parameters position
  QValueList<ParameterCanvas *>::Iterator iter;
  
  for (iter = params.begin(); iter != params.end(); ++iter)
    (*iter)->do_change_scale();
}

void ActivityCanvas::modified() {
  hide();
  hide_lines();
  compute_size();
  show();
  update_show_lines();
  check_params();
  canvas()->update();
  force_sub_inside();
  package_modified();
}

aCorner ActivityCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void ActivityCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width, min_height);
  
  force_sub_inside();
}

bool ActivityCanvas::move_with_its_package() const {
  return TRUE;
}

void ActivityCanvas::set_z(double z) {
  setZ(z);
  
  z += 1;
  
  QValueList<ParameterCanvas *>::Iterator iter;
  
  for (iter = params.begin(); iter != params.end(); ++iter)
    (*iter)->set_z(z);
}

void ActivityCanvas::force_sub_inside() {
  // update sub nodes position to be inside of the activity
  // except the parameters whose are in the border
  // and the diagram icon
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator cit;
  
  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible() && !(*cit)->selected()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*cit);
      
      if ((di != 0) &&
	  (di->get_bn() != 0) &&
	  (((BrowserNode *) di->get_bn())->parent() == browser_node)) {
	// must look at the type because some canvas items have browser_node
	// attr equals to the diagram and the parent of the diagram is the activity
	switch (di->type()) {
	case UmlParameter:
	  ((ParameterCanvas *) di)->check_position();
	  break;
	case UmlActivityObject:
	case UmlActivityAction:
	case UmlPartition:
	case UmlExpansionRegion:
	case UmlInterruptibleActivityRegion:
	case InitialAN:
	case FlowFinalAN:
	case ActivityFinalAN:
	case DecisionAN:
	case MergeAN:
	case ForkAN:
	case JoinAN:
	  force_inside(di, *cit);
	default:
	  break;
	}
      }
    }
  }
}

void ActivityCanvas::check_params() {
  // add missing params
  const QValueList<BrowserParameter *> brparams =
    ((BrowserActivity *) browser_node)->get_params();
  QValueList<BrowserParameter *>::ConstIterator iter;
  int dy = (int) (the_canvas()->zoom() * PARAMETER_CANVAS_MIN_HEIGHT);
  int rank;
  
  for (iter = brparams.begin(), rank = 0; iter != brparams.end(); ++iter, rank += 1) {
    QValueList<ParameterCanvas *>::ConstIterator itershown;
    
    for (itershown = params.begin(); itershown != params.end(); ++itershown)
      if ((*itershown)->get_bn() == *iter)
	break;
    
    if (itershown == params.end()) {
      // add it
      int px;
      int py;
      
      switch (rank % 8) {
      case 0:
	px = (int) x();
	py = (int) y() + dy;
	break;
      case 1:
	px = (int) x();
	py = center().y();
	break;
      case 2:
	px = (int) x();
	py = (int) rect().bottom() - dy - dy;
	break;
      case 3:
	px = center().x();
	py = (int) y() + dy;
	break;
      case 4:
	px = rect().right();
	py = center().y();
	break;
      case 5:
	px = rect().right();
	py = (int) rect().bottom() - dy - dy;
	break;
      case 6:
	px = rect().right();
	py = (int) y() + dy;
	break;
      default:
	px = center().x();
	py = center().y() + dy;
	break;
      }
      
      ParameterCanvas * pc =
	new ParameterCanvas(*iter, the_canvas(), px, py, 0, this);
      
      params.append(pc);
      pc->show();
    }
  }
}

void ActivityCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  QBrush brsh = p.brush();
  QColor bckgrnd = p.backgroundColor();
  int margin = (int) (24 * the_canvas()->zoom());
  
  p.setBackgroundMode((used_color == UmlTransparent)
		      ? QObject::TransparentMode
		      : QObject::OpaqueMode);

  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  
  if (used_color != UmlTransparent) 
    p.setBrush(co);
  
  p.setPen(QObject::SolidLine);
  p.drawRoundRect(r, 8, 8);
  
  p.setFont(the_canvas()->get_font(UmlNormalBoldFont));
  p.drawText(r.x() + margin, r.y() + margin,
	     r.width(), r.height(), QObject::AlignLeft,
	     browser_node->get_name());  
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  
  if (singleexec_offset.x() != 0)
    p.drawText(r.x() + singleexec_offset.x(),
	       r.y() + singleexec_offset.y(),
	       r.width(), r.height(), QObject::AlignLeft,
	       "<<singleExecution>>");
  
  if (readonly_offset.x() != 0)
    p.drawText(r.x() + readonly_offset.x(),
	       r.y() + readonly_offset.y(),
	       r.width(), r.height(), QObject::AlignLeft,
	       "<<readOnly>>");
  
  if (! pre.isEmpty()) {
    p.drawText(r.x() + pre_offset.x(),
	       r.y() + pre_offset.y(),
	       r.width(), r.height(), QObject::AlignLeft,
	       "<<precondition>>");
    p.drawText(r.x() + pre_offset.x() + post_width,
	       r.y() + pre_offset.y(),
	       r.width(), r.height(),
	       QObject::AlignLeft, pre);
  }
  
  if (! post.isEmpty()) {
    p.drawText(r.x() + post_offset.x(),
	       r.y() + post_offset.y(),
	       r.width(), r.height(), QObject::AlignLeft,
	       "<<postcondition>>");
    p.drawText(r.x() + post_offset.x() + post_width,
	       r.y() + post_offset.y(),
	       r.width(), r.height(),
	       QObject::AlignLeft, post);
  }
      
  p.setBackgroundColor(bckgrnd);
  p.setBrush(brsh);
  
  if (selected())
    show_mark(p, r);
}

UmlCode ActivityCanvas::type() const {
  return UmlActivity;
}

void ActivityCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool ActivityCanvas::alignable() const {
  return TRUE;
}

bool ActivityCanvas::copyable() const {
  return selected();
}

void ActivityCanvas::open() {
  browser_node->open(FALSE);
}

void ActivityCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  int index;
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  if (browser_node->is_writable()) {
    m.insertItem("add parameter", 9);
    m.insertSeparator();
  }
  m.insertItem("upper", 0);
  m.insertItem("lower", 1);
  m.insertSeparator();
  m.insertItem("edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("edit activity", 3);
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
  if (Tool::menu_insert(&toolm, UmlActivity, 10))
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
      QArray<StateSpec> st(2);
      QArray<ColorSpec> co(1);
      
      st[0].set("show conditions", &settings.show_infonote);
      st[1].set("drawing language", &settings.drawing_language);
      
      co[0].set("activity color", &itscolor);

      SettingsDialog dialog(&st, &co, FALSE, TRUE);
      
      dialog.raise();
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
    ((BrowserActivity *) browser_node)
      ->set_associated_diagram((BrowserActivityDiagram *)
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
  case 9:
    ((BrowserActivity *) browser_node)->add_parameter(0, 0);
    modified();
  default:
    if (index >= 10)
      ToolCom::run(Tool::command(index - 10), browser_node);
    return;
  }
  
  package_modified();
}

const char * ActivityCanvas::may_start(UmlCode & l) const {
  return (l == UmlFlow) ? "illegal" : 0;
}

const char * ActivityCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  if (l == UmlAnchor) {
    switch (dest->type()) {
    case UmlNote:
    case UmlIcon:
      return 0;
    default:
      return "illegal";
    }
  }
  else if(dest->get_bn() == 0)
    return "illegal";
  else
    // dependency
    return ((BrowserActivity *) browser_node)->may_connect(dest->get_bn());
}

void ActivityCanvas::connexion(UmlCode action, DiagramItem * dest,
			       const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (IsaSimpleRelation(action))
    a = new SimpleRelationCanvas(the_canvas(), this, dest, 0, action, 0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

void ActivityCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "activitycanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "activitycanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    settings.save(st);
    if (itscolor != UmlDefaultColor) {
      nl_indent(st);
      st << "color " << stringify(itscolor);
    }
    nl_indent(st);
    save_xyzwh(st, this, "xyzwh");
    
    if (! params.isEmpty()) {
      nl_indent(st);
      st << "params";
      indent(+1);
    
      QValueList<ParameterCanvas *>::ConstIterator iter;
  
      for (iter = params.begin(); iter != params.end(); ++iter)
	(*iter)->save(st, FALSE, warning);

      indent(-1);
      nl_indent(st);
      st << "end";
    }

    indent(-1);
    nl_indent(st);
    st << "end";
  }
}

ActivityCanvas * ActivityCanvas::read(char * & st, UmlCanvas * canvas,
				      char * k)
{
  if (!strcmp(k, "activitycanvas_ref"))
    return ((ActivityCanvas *) dict_get(read_id(st), "activitycanvas", canvas));
  else if (!strcmp(k, "activitycanvas")) {
    int id = read_id(st);
    BrowserActivity * br = BrowserActivity::read_ref(st);
    ActivityCanvas * result = new ActivityCanvas(canvas, id);
    
    result->browser_node = br;
    connect(br->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(br->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));

    k = read_keyword(st);
    
    result->settings.read(st, k);	// updates k
    read_color(st, "color", result->itscolor, k);	// updates k
    
    if (!strcmp(k, "xyzwh"))
      read_xyzwh(st, result);
    else
      wrong_keyword(k, "xyzwh");
    result->compute_size();
    result->set_center100();
    result->show();
    
    k = read_keyword(st);

    if (! strcmp(k, "params")) {
      while (strcmp(k = read_keyword(st), "end") != 0)
	result->params.append(ParameterCanvas::read(st, canvas, k, result));
      k = read_keyword(st);
    }

    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    result->check_params();
    
    // result->force_sub_inside() useless : may only grow
    
    return result;
  }
  else 
    return 0;
}

void ActivityCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(min_width, b);
  ::save(min_height, b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void ActivityCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(min_width, b);
  ::load(min_height, b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
  
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));  
}

void ActivityCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);

  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
}

