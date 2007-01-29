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

#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "StateActionCanvas.h"
#include "TransitionCanvas.h"
#include "SimpleRelationCanvas.h"
#include "StateActionData.h"
#include "BrowserStateAction.h"
#include "BrowserStateDiagram.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"
#include "strutil.h"

StateActionCanvas::StateActionCanvas(BrowserNode * bn, UmlCanvas * canvas,
			 int x, int y)
    : DiagramCanvas(0, canvas, x, y, STATE_ACTION_CANVAS_MIN_SIZE, 1, 0) {
  browser_node = bn;
  itscolor = UmlDefaultColor;
  language = DefaultDrawingLanguage;
  
  compute_size();
  
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

StateActionCanvas::StateActionCanvas(UmlCanvas * canvas, int id)
    : DiagramCanvas(canvas, id) {
  // for read operation
  browser_node = 0;
  itscolor = UmlDefaultColor;
  language = DefaultDrawingLanguage;
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

StateActionCanvas::~StateActionCanvas() {
}

void StateActionCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  
  DiagramCanvas::delete_it();
}

void StateActionCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void StateActionCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void StateActionCanvas::compute_size() {
  double zoom = the_canvas()->zoom();
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  const StateActionData * data =
    (const StateActionData *) browser_node->get_data();
  
  s = data->get_action((language == DefaultDrawingLanguage)
		       ? the_canvas()->browser_diagram()->get_language(UmlStateAction)
		       : language);
  s = toUnicode(s);
   
  QSize sz = fm.size(0, s);
  
  min_height = fm.height() + sz.height();
  min_width = sz.width();
    
  const char * st = data->get_stereotype();
  int mn = (int) (STATE_ACTION_CANVAS_MIN_SIZE*zoom);
  
  if (!strcmp(st, "receive-signal") ||
      !strcmp(st, "send-signal")) {
    if (min_height < mn)
      min_height = mn;
    min_width += ((height() > min_height) ? height() : min_height) / 2;
  }
  else if (st[0] != 0) {
    int w = fm.width(QString("<<") + toUnicode(data->get_stereotype()) + ">>");
    
    if (min_width < w)
      min_width = w;
    
    min_height += 2*fm.height();
    
    if (min_height < mn)
    min_height = mn;
  }
    
  min_width += (int) (8 * zoom);
  
  if (min_width < mn)
    min_width = mn;
  
  min_width += 2;

  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlStateAction)
    : itscolor;

  if (used_color != UmlTransparent) {
    const char * st = data->get_stereotype();
    
    if (strcmp(st, "send-signal") && strcmp(st, "receive-signal")) {
      const int shadow = the_canvas()->shadow();
    
      min_width += shadow;
      min_height += shadow;
    }
  }

  DiagramCanvas::resize((width() > min_width) ? width() : min_width,
			(height() > min_height) ? height() : min_height);
}

void StateActionCanvas::change_scale() {
  QCanvasRectangle::setVisible(FALSE);
  double scale = the_canvas()->zoom();
    
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  compute_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void StateActionCanvas::modified() {
  hide();
  hide_lines();
  compute_size();
  show();
  update_show_lines();
  force_self_rel_visible();
  canvas()->update();
  package_modified();
}

void StateActionCanvas::connexion(UmlCode action, DiagramItem * dest,
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

aCorner StateActionCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void StateActionCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width, min_height);
  compute_size();
}

void StateActionCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  int mw = min_width;
  QColor bckgrnd = p.backgroundColor();
  QBrush brsh = p.brush();
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  const BasicData * data = browser_node->get_data();
  const char * st = data->get_stereotype();
  const int shadow = the_canvas()->shadow();
      
  p.setBackgroundMode((used_color == UmlTransparent)
		      ? QObject::TransparentMode
		      : QObject::OpaqueMode);

  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  
  if (used_color != UmlTransparent)
    p.setBackgroundMode(QObject::OpaqueMode);
  else
    p.setBackgroundMode(QObject::TransparentMode);
  
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  
  if (!strcmp(st, "send-signal")) {
    const int hh = r.height()/2;
    QPointArray a(6);
  
    if ((used_color != UmlTransparent) && (shadow != 0)) {
      r.setRight(r.right() - shadow);
      r.setBottom(r.bottom() - shadow);
    }
  
    a.setPoint(0, r.left(), r.top());
    a.setPoint(1, r.right() - hh, r.top());
    a.setPoint(2, r.right(), r.top() + hh);
    a.setPoint(3, r.right() - hh, r.bottom());
    a.setPoint(4, r.left(), r.bottom());
    a.setPoint(5, r.left(), r.top());
    
    if (used_color == UmlTransparent)
      p.drawPolyline(a);
    else {
      if (shadow != 0) {
	QPointArray b(6);
	
	b.setPoint(0, r.left() + shadow, r.top() + shadow);
	b.setPoint(1, r.right() - hh + shadow, r.top() + shadow);
	b.setPoint(2, r.right() + shadow, r.top() + hh + shadow);
	b.setPoint(3, r.right() - hh + shadow, r.bottom() + shadow);
	b.setPoint(4, r.left() + shadow, r.bottom() + shadow);
	b.setPoint(5, r.left() + shadow, r.top() + shadow);
	p.setBrush(QObject::darkGray);
	p.setPen(QObject::NoPen);
	p.drawPolygon(b, TRUE, 0, 5);
	p.setPen(QObject::SolidLine);
      }
      
      p.setBrush(co);
      p.drawPolygon(a, TRUE, 0, 5);
    }

    r.setRight(r.right() - hh);
    mw -= hh;
  }
  else if (!strcmp(st, "receive-signal")) {
    const int hh = r.height()/2;
    QPointArray a(6);
  
    if ((used_color != UmlTransparent) && (shadow != 0)) {
      r.setRight(r.right() - shadow);
      r.setBottom(r.bottom() - shadow);
    }
  
    a.setPoint(0, r.left(), r.top());
    a.setPoint(1, r.right(), r.top());
    a.setPoint(2, r.right() - hh, r.top() + hh);
    a.setPoint(3, r.right(), r.bottom());
    a.setPoint(4, r.left(), r.bottom());
    a.setPoint(5, r.left(), r.top());
    
    if (used_color == UmlTransparent)
      p.drawPolyline(a);
    else {
      if (shadow != 0) {
	QPointArray b(6);
	
	b.setPoint(0, r.left() + shadow, r.top() + shadow);
	b.setPoint(1, r.right() + shadow, r.top() + shadow);
	b.setPoint(2, r.right() - hh + shadow, r.top() + hh + shadow);
	b.setPoint(3, r.right() + shadow, r.bottom() + shadow);
	b.setPoint(4, r.left() + shadow, r.bottom() + shadow);
	b.setPoint(5, r.left() + shadow, r.top() + shadow);
	p.setBrush(QObject::darkGray);
	p.setPen(QObject::NoPen);
	p.drawPolygon(b, TRUE, 0, 5);
	p.setPen(QObject::SolidLine);
      }
      
      p.setBrush(co);
      p.drawPolygon(a, TRUE, 0, 5);
    }

    r.setRight(r.right() - hh);
    mw -= hh;
  }
  else {
    if (shadow != 0) {
      r.setRight(r.right() - shadow);
      r.setBottom(r.bottom() - shadow);
      
      p.fillRect (r.right(), r.top() + shadow,
		  shadow, r.height() - 1,
		  QObject::darkGray);
      p.fillRect (r.left() + shadow, r.bottom(),
		  r.width() - 1, shadow,
		  QObject::darkGray);
    }
    
    p.setBrush(co);
    p.drawRect(r);
    
    if (st[0] != 0) {
      r.setTop(r.top() + fm.height() / 2);
      p.drawText(r, QObject::AlignHCenter, QString("<<") + toUnicode(st) + ">>");
      r.setTop(r.top() + 3*fm.height()/2);
    }
  }
  
  r.setLeft(r.left() + (r.width()
			+ (int) (8 * the_canvas()->zoom())
			- mw)/2 + 1);
  p.drawText(r, QObject::AlignVCenter, s);
  
  p.setBackgroundColor(bckgrnd);
  p.setBrush(brsh);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode StateActionCanvas::type() const {
  return UmlStateAction;
}

void StateActionCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool StateActionCanvas::alignable() const {
  return TRUE;
}

bool StateActionCanvas::copyable() const {
  return selected();
}

void StateActionCanvas::open() {
  browser_node->open(FALSE);
}

void StateActionCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  int index;
  const char * s = browser_node->get_name();
  
  if (*s == 0) {
    const BasicData * data = browser_node->get_data();
    const char * st = data->get_stereotype();
    
    s = (*st != 0) ? st : "action";
  }
  
  m.insertItem(new MenuTitle(s, m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("Edit state action", 3);
  m.insertSeparator();
  m.insertItem("Select in browser", 4);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  m.insertItem("Remove from view", 7);
  if (browser_node->is_writable())
    m.insertItem("Delete from model", 8);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, UmlStateAction, 10))
    m.insertItem("Tool", &toolm);
  
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
    edit_drawing_settings();
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

void StateActionCanvas::apply_shortcut(QString s) {
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

void StateActionCanvas::edit_drawing_settings() {
  QArray<StateSpec> st(1);
  QArray<ColorSpec> co(1);
  
  st[0].set("drawing language", &language);
  
  co[0].set("state action color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();	// call package_modified()
}

bool StateActionCanvas::has_drawing_settings() const {
  return TRUE;
}

void StateActionCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<StateSpec> st(1);
  QArray<ColorSpec> co(1);
  DrawingLanguage language;
  UmlColor itscolor;
  
  st[0].set("drawing language", &language);
  
  co[0].set("state action color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      if (st[0].name != 0)
	((StateActionCanvas *) it.current())->language = language;
      if (co[0].name != 0)
	((StateActionCanvas *) it.current())->itscolor = itscolor;
      ((StateActionCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

const char * StateActionCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlAnchor:
  case UmlTransition:
    return 0;
  default:
    return "illegal";
  }
}

const char * StateActionCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
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

void StateActionCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "stateactioncanvas_ref " << get_ident();
  }
  else {
    nl_indent(st);
    st << "stateactioncanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    if (language != DefaultDrawingLanguage) {
      nl_indent(st);
      st << "language " << stringify(language);
    }
    if (itscolor != UmlDefaultColor) {
      nl_indent(st);
      st << "color " << stringify(itscolor);
    }
    nl_indent(st);
    save_xyzwh(st, this, "xyzwh");
    indent(-1);
  }
}

StateActionCanvas * StateActionCanvas::read(char * & st, UmlCanvas * canvas,
					char * k)
{
  if (!strcmp(k, "stateactioncanvas_ref"))
    return ((StateActionCanvas *) dict_get(read_id(st), "stateactioncanvas", canvas));
  else if (!strcmp(k, "stateactioncanvas")) {
    int id = read_id(st);
    BrowserStateAction * br = BrowserStateAction::read_ref(st);
    StateActionCanvas * result = new StateActionCanvas(canvas, id);
    
    result->browser_node = br;
    connect(br->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(br->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));

    k = read_keyword(st);
    
    if (!strcmp(k, "language")) {
      result->language = drawing_language(read_keyword(st));
      k = read_keyword(st);
    }
    read_color(st, "color", result->itscolor, k);	// updates k
    
    if (!strcmp(k, "xyzwh"))
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

void StateActionCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(min_width, b);
  ::save(min_height, b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void StateActionCanvas::history_load(QBuffer & b) {
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

void StateActionCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
}

