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

#include <qpainter.h>
#include <qpopupmenu.h> 
#include <qcursor.h>

#include "ParameterCanvas.h"
#include "BrowserParameter.h"
#include "ActivityCanvas.h"
#include "ParameterData.h"
#include "LabelCanvas.h"
#include "FlowCanvas.h"
#include "UmlCanvas.h"
#include "BrowserDiagram.h"
#include "BrowserClass.h"
#include "UmlGlobal.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"
#include "strutil.h"

ParameterCanvas::ParameterCanvas(BrowserNode * bn, UmlCanvas * canvas,
				 int x, int y, int id, ActivityCanvas * a)
    : DiagramCanvas(0, canvas, x, y, PARAMETER_CANVAS_MIN_WIDTH,
		    PARAMETER_CANVAS_MIN_HEIGHT, id),
      act(a) {
  browser_node = bn;
  itscolor = UmlDefaultColor;
  
  if (id == 0) {
    // not on read
    update();
    setZ(a->z() + 2);	// 2 to have lines upper activity's z
  }

  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

ParameterCanvas::~ParameterCanvas() {
}

void ParameterCanvas::delete_it() {
  // don't remove it from activity's params to manage undo
  disconnect(browser_node->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  
  DiagramCanvas::delete_it();
}

void ParameterCanvas::deleted() {
  delete_it();
  act->deleted(this);
  canvas()->update();
}

void ParameterCanvas::update() {
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlParameter)
    : itscolor;
  
  ParameterData * d = (ParameterData *) browser_node->get_data();
  
  if (d->get_is_stream()) {
    if (label == 0) {
      QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));

      label = new LabelCanvas("{stream}", the_canvas(), 
			      (int) x(), (int) y() - fm.height() - 4);
    }
    else
      label->set_name("{stream}");
  }
  else if (d->get_is_exception()) {
    if (label == 0) {
      QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));

      label = new LabelCanvas(LabelCanvas::Triangle, the_canvas(), 
			      (int) x(), (int) y() - fm.height() - 4);
    }
    else
      label->set_name(LabelCanvas::Triangle);
  }
  else if (label != 0) {
    label->delete_it();
    label = 0;
  }

  check_position();
}

// check position, note : size is odd

void ParameterCanvas::check_position() {
  QRect act_rect = act->rect();
  QPoint ce = center();
  
  double dxl = fabs(act_rect.left() - ce.x());
  double dxr = fabs(act_rect.right() - ce.x());
  double dyt = fabs(act_rect.top() - ce.y());
  double dyb = fabs(act_rect.bottom() - ce.y());
  
  if ((dxr == 0) || (dxl == 0)) {
    if (ce.y() < act_rect.top())
      DiagramCanvas::moveBy(0, act_rect.top() - ce.y());
    else if (ce.y() > act_rect.bottom())
      DiagramCanvas::moveBy(0, act_rect.bottom() - ce.y());
  }
  else if ((dyt == 0) || (dyb == 0)) {
    if (ce.x() < act_rect.left())
      DiagramCanvas::moveBy(act_rect.left() - ce.x(), 0);
    else if (ce.x() > act_rect.right())
      DiagramCanvas::moveBy(act_rect.right() - ce.x(), 0);
  }
  else {
    double dx = (dxl < dxr) ? dxl : dxr;
    double dy = (dyt < dyb) ? dyt : dyb;
    
    if (dx < dy) {
      dx = (dxl < dxr) ? act_rect.left() - ce.x()
		       : act_rect.right() - ce.x();
      
      if (ce.y() < act_rect.top())
	DiagramCanvas::moveBy(dx, act_rect.top() - ce.y());
      else if (ce.y() > act_rect.bottom())
	DiagramCanvas::moveBy(dx, act_rect.bottom() - ce.y());
      else
	DiagramCanvas::moveBy(dx, 0);
    }
    else {
      dy = (dyt < dyb) ? act_rect.top() - ce.y()
		       : act_rect.bottom() - ce.y();
      
      if (ce.x() < act_rect.left())
	DiagramCanvas::moveBy(act_rect.left() - ce.x(), dy);
      else if (ce.x() > act_rect.right())
	DiagramCanvas::moveBy(act_rect.right() - ce.x(), dy);
      else
	DiagramCanvas::moveBy(0, dy);
    }
  }
}

aCorner ParameterCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void ParameterCanvas::resize(aCorner c, int dx, int dy) {
  double zoom = the_canvas()->zoom();
  
  DiagramCanvas::resize(c, dx, dy,
			(int) (PARAMETER_CANVAS_MIN_WIDTH * zoom),
			(int) (PARAMETER_CANVAS_MIN_HEIGHT * zoom),
			TRUE, TRUE);
  
  check_position();	// theorically useless
}

void ParameterCanvas::change_scale() {
  // does nothing, made by do_change_scale called by 
  // ActivityCanvas::change_scale()
}

void ParameterCanvas::do_change_scale() {
  QCanvasRectangle::setVisible(FALSE);
  double scale = the_canvas()->zoom();
    
  setSize((int) (width_scale100*scale) | 1,
	  (int) (height_scale100*scale) | 1);
  recenter();
  // activity already in position, can check
  check_position();
  QCanvasRectangle::setVisible(TRUE);
}

void ParameterCanvas::moveBy(double dx, double dy) {
  if (!act->selected() || the_canvas()->paste())
    do_moveBy(dx, dy);
  // else do_moveBy called by ActivityCanvas::moveBy()
}

void ParameterCanvas::do_moveBy(double dx, double dy) {
  DiagramCanvas::moveBy(dx, dy);
  
  // activity already in position, can check
  check_position();
  
  if (!the_canvas()->do_zoom())
    set_center100();
}

void ParameterCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QBrush brsh = p.brush();
  QColor bckgrnd = p.backgroundColor();
  
  p.setBackgroundMode((used_color == UmlTransparent)
		      ? QObject::TransparentMode
		      : QObject::OpaqueMode);

  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  
  if (used_color != UmlTransparent) 
    p.setBrush(co);
  
  QRect r = rect();
  
  p.drawRect(r);
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  p.drawText(r.left() + 2, r.top() + 2,
	     r.width() - 4, r.height() - 4,
	     QObject::AlignCenter + QObject::WordBreak,
	     browser_node->get_name());
      
  p.setBackgroundColor(bckgrnd);
  p.setBrush(brsh);
  
  if (selected())
    show_mark(p, r);
}

UmlCode ParameterCanvas::type() const {
  return UmlParameter;
}

bool ParameterCanvas::copyable() const {
  return act->selected();
}

bool ParameterCanvas::activity_selected() const {
  return act->selected();
}

void ParameterCanvas::open() {
  browser_node->open(FALSE);
}

void ParameterCanvas::menu(const QPoint &) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  int index;
  QValueList<ParameterCanvas *> params = act->get_params();
  BrowserClass * cl = ((ParameterData *) browser_node->get_data())->type.type;
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 2);
  if (params.count() > 1)
    m.insertItem("Resize other like it", 6);
  m.insertSeparator();
  m.insertItem("Edit parameter", 3);
  m.insertSeparator();
  m.insertItem("Select in browser", 4);
  if (cl != 0)
    m.insertItem("Select class in browser", 9);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  if (browser_node->is_writable())
    m.insertItem("Delete from model", 8);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, UmlParameter, 10))
    m.insertItem("Tool", &toolm);
  
  switch (index = m.exec(QCursor::pos())) {
  case 0:
    act->upper();
    modified();	// call package_modified()
    return;
  case 1:
    act->lower();
    modified();	// call package_modified()
    return;
  case 2:
    {
      QArray<ColorSpec> co(1);
      
      co[0].set("parameter node color", &itscolor);

      SettingsDialog dialog(0, &co, FALSE, TRUE);
      
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
    {
      int w = width();
      int h = height();
      QValueList<ParameterCanvas *>::Iterator iter;
  
      for (iter = params.begin(); iter != params.end(); ++iter) {
	if (*iter != this) {
	  (*iter)->setSize(w, h);
	  (*iter)->recenter();
	  (*iter)->width_scale100 = width_scale100;
	  (*iter)->height_scale100 = height_scale100;
	}
      }
    }
    break;
  case 8:
    //delete from model
    browser_node->delete_it();	// will delete the canvas
    break;
  case 9:
    cl->select_in_browser();
    return;
  default:
    if (index >= 10)
      ToolCom::run(Tool::command(index - 10), browser_node);
    return;
  }
  
  package_modified();
}

bool ParameterCanvas::has_drawing_settings() const {
  return TRUE;
}

void ParameterCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  
  co[0].set("parameter node color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if ((dialog.exec() == QDialog::Accepted) && (co[0].name != 0)) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      ((ParameterCanvas *) it.current())->itscolor = itscolor;
      ((ParameterCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

const char * ParameterCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlFlow:
    return ((BrowserParameter *) browser_node)->may_start();
  case UmlDependOn:
    return "illegal";
  default: // anchor
    return 0;
  }
}

const char * ParameterCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
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
    return ((BrowserParameter *) browser_node)->may_connect(dest->get_bn());
}

void ParameterCanvas::connexion(UmlCode action, DiagramItem * dest,
				const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (action == UmlFlow)
    a = new FlowCanvas(the_canvas(), this, dest, 0, 0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

void ParameterCanvas::modified() {
  hide();
  hide_lines();
  update();
  show();
  update_show_lines();
  canvas()->update();
  package_modified();
}

void ParameterCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "parametercanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "parametercanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    nl_indent(st);
    if (itscolor != UmlDefaultColor)
      st << "  color " << stringify(itscolor);
    save_xyzwh(st, this, "  xyzwh");
    if (label != 0)
      save_xy(st, label, " label_xy");
    indent(-1);
    nl_indent(st);
    st << "end";
  }
}

ParameterCanvas * ParameterCanvas::read(char * & st, UmlCanvas * canvas,
					char * k, ActivityCanvas * a) {
  if (!strcmp(k, "parametercanvas_ref"))
    return ((ParameterCanvas *) dict_get(read_id(st), "parametercanvas", canvas));
  else if (!strcmp(k, "parametercanvas")) {
    int id = read_id(st);
    BrowserParameter * br = BrowserParameter::read_ref(st);
    ParameterCanvas * result =
      new ParameterCanvas(br, canvas, 0, 0, id, a);
    
    k = read_keyword(st);
    
    read_color(st, "color", result->itscolor, k);	// updates k
    
    if (!strcmp(k, "xyzwh"))
      read_xyzwh(st, result);
    else
      wrong_keyword(k, "xyzwh");
    
    result->width_scale100 = result->width();
    result->height_scale100 = result->height();
    result->set_center100();
    result->update();
    result->show();
    
    k = read_keyword(st);
    if (!strcmp(k, "label_xy")) {
      if (result->label != 0) {
	read_xy(st, result->label);
        result->label->set_center100();
      }
      else
	bypass_xy(st);
      
      k = read_keyword(st);
    }

    if (strcmp(k, "end"))
      wrong_keyword(k, "end");

    return result;
  }
  else 
    return 0;
}

void ParameterCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  disconnect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

void ParameterCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void ParameterCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
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
