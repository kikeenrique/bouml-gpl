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

#include "UcUseCaseCanvas.h"
#include "SimpleRelationCanvas.h"
#include "BasicData.h"
#include "BrowserUseCase.h"
#include "BasicData.h"
#include "LabelCanvas.h"
#include "BrowserDiagram.h"
#include "UmlGlobal.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"

UcUseCaseCanvas::UcUseCaseCanvas(BrowserNode * bn, UmlCanvas * canvas,
				 int x, int y, int id)
    : DiagramCanvas(bn, canvas, x, y, USECASE_CANVAS_MIN_WIDTH,
		    USECASE_CANVAS_MIN_HEIGHT, id),
      itscolor(UmlDefaultColor), used_color(UmlDefaultColor) {
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}

UcUseCaseCanvas::~UcUseCaseCanvas() {
}

void UcUseCaseCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  DiagramCanvas::delete_it();
}

void UcUseCaseCanvas::deleted() {
  delete_it();
}

void UcUseCaseCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void UcUseCaseCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  UmlColor co;
  
  if (itscolor != UmlDefaultColor)
    co = itscolor;
  else {
    if (used_color == UmlDefaultColor)
      used_color = the_canvas()->browser_diagram()->get_color(UmlUseCase);
    co = used_color;
  }
  
  QBrush brsh = p.brush();
  
  if (co != UmlTransparent) {
    const int shadow = the_canvas()->shadow() - 1;
    
    if (shadow != -1) {
      r.setRight(r.right() - shadow);
      r.setBottom(r.bottom() - shadow);
      p.setPen(QObject::NoPen);
      p.setBrush(QObject::darkGray);
      p.drawEllipse(r.left() + shadow, r.top() + shadow, r.width(), r.height());
    }
  }
  
  bool realizationp =
    !strcmp(browser_node->get_data()->get_stereotype(), "realization");
  
  p.setBackgroundMode((co == UmlTransparent) ? QObject::TransparentMode : QObject::OpaqueMode);
  p.setBrush(color(co));
  if (realizationp)
    p.setPen(QObject::DotLine);
  else
    p.setPen(QObject::SolidLine);
  p.drawEllipse(r.left(), r.top(), r.width(), r.height());
  if (realizationp)
    p.setPen(QObject::SolidLine);
  p.setBrush(brsh);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode UcUseCaseCanvas::type() const {
  return UmlUseCase;
}

void UcUseCaseCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool UcUseCaseCanvas::alignable() const {
  return TRUE;
}

bool UcUseCaseCanvas::copyable() const {
  return selected();
}

void UcUseCaseCanvas::open() {
  browser_node->open(FALSE);
}

void UcUseCaseCanvas::modified() {
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlUseCase)
    : itscolor;
  
  label->set_name(browser_node->get_name());
  
  // force son reaffichage
  hide();
  show();
  force_self_rel_visible();
  canvas()->update();
  package_modified();
}

void UcUseCaseCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 3);
  m.insertSeparator();
  m.insertItem("Select in browser",4);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  if (browser_node->is_writable())
    m.insertItem("Set associated diagram",6);
  m.insertSeparator();
  m.insertItem("Remove from view",7);
  if (browser_node->is_writable())
    m.insertItem("Delete from model", 8);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, UmlUseCase, 10))
    m.insertItem("Tool", &toolm);

  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    upper();
    modified();
    return;
  case 1:
    lower();
    modified();
    return;
  case 2:
    browser_node->open(TRUE);
    break;
  case 3:
    edit_drawing_settings();
    return;
  case 4:
    browser_node->select_in_browser();
    return;
  case 5:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 6:
    ((BrowserUseCase *) browser_node)->set_associated_diagram((BrowserUseCaseDiagram *)
							      the_canvas()->browser_diagram());
    break;
  case 7:
    // remove from view
    delete_it();
    break;
  case 8:
    // delete from model
    browser_node->delete_it();	// will remove canvas
    break;
  default:
    if (rank >= 10)
      ToolCom::run(Tool::command(rank - 10), browser_node);
    return;
  }
  
  package_modified();
}

void UcUseCaseCanvas::apply_shortcut(QString s) {
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

void UcUseCaseCanvas::edit_drawing_settings() {
  QArray<ColorSpec> co(1);
  
  co[0].set("use case color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();
}

bool UcUseCaseCanvas::has_drawing_settings() const {
  return TRUE;
}

void UcUseCaseCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  
  co[0].set("use case color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if ((dialog.exec() == QDialog::Accepted) && (co[0].name != 0)) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      ((UcUseCaseCanvas *) it.current())->itscolor = itscolor;
      ((UcUseCaseCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

const char * UcUseCaseCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlDependency:
    l = UmlDependOn;
    return (browser_node->is_writable()) ? 0 : "read only";
  case UmlGeneralisation:
    l = UmlInherit;
    return (browser_node->is_writable()) ? 0 : "read only";
  case UmlAnchor:
  case UmlAssociation:
  case UmlDirectionalAssociation:
    return 0;
  default:
    return "illegal";
  }
}

const char * UcUseCaseCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlUseCase:
    switch (l) {
    case UmlInherit:
      return browser_node->check_inherit(dest->get_bn());
    case UmlDependOn:
    case UmlAssociation:
    case UmlDirectionalAssociation:
      return 0;
    default:
      return "illegal";
    }
  case UmlClass:
    return ((l == UmlAssociation) || (l == UmlDirectionalAssociation))
      ? 0 : "illegal";
  case UmlNote:
  case UmlIcon:
    return (l == UmlAnchor) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

void UcUseCaseCanvas::connexion(UmlCode action, DiagramItem * dest,
				const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (IsaSimpleRelation(action))
    a = new SimpleRelationCanvas(the_canvas(), this, dest, 0, action, 0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

aCorner UcUseCaseCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void UcUseCaseCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy,
			(int) (USECASE_CANVAS_MIN_WIDTH * the_canvas()->zoom()),
			(int) (USECASE_CANVAS_MIN_HEIGHT * the_canvas()->zoom()));
}

void UcUseCaseCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "usecasecanvas_ref " << get_ident() << " // " << browser_node->get_name();
  else {
    nl_indent(st);
    st << "usecasecanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    if (itscolor != UmlDefaultColor) {
      nl_indent(st);
      st << "  color " << stringify(itscolor);
    }
    nl_indent(st);
    save_xyzwh(st, this, "  xyzwh");
    save_xy(st, label, " label_xy");
  }
}

bool UcUseCaseCanvas::move_with_its_package() const {
  return TRUE;
}

UcUseCaseCanvas * UcUseCaseCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "usecasecanvas_ref"))
    return ((UcUseCaseCanvas *) dict_get(read_id(st), "usecasecanvas", canvas));
  else if (!strcmp(k, "usecasecanvas")) {
    int id = read_id(st);
    BrowserNode * br = BrowserUseCase::read_ref(st);
    UmlColor color = UmlDefaultColor;
    
    k = read_keyword(st);
    read_color(st, "color", color, k);
    
    if (strcmp(k, "xyzwh"))
      wrong_keyword(k, "xyzwh");
    
    int x = (int) read_double(st);
    UcUseCaseCanvas * result =
      new UcUseCaseCanvas(br, canvas, x, (int) read_double(st), id);
    
    result->itscolor = color;
    read_zwh(st, result);
    result->width_scale100 = result->width();
    result->height_scale100 = result->height();

    k = read_keyword(st);
    read_xy(st, result->label);
    result->label->setZ(result->z() + 0.5);
    result->label->set_center100();
    
    if (!strcmp(k, "label_xyz"))
      // old version
      read_double(st);
    else if (strcmp(k, "label_xy"))
      wrong_keyword(k, "label_xy/label_xyz");
    
    result->show();
    return result;
  }
  else
    return 0;
}

void UcUseCaseCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
}

void UcUseCaseCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void UcUseCaseCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
  
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}
