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

#include <qcursor.h>
#include <qpainter.h>
#include <qpopupmenu.h> 

#include "DeploymentNodeCanvas.h"
#include "SimpleRelationCanvas.h"
#include "SimpleData.h"
#include "BrowserDeploymentNode.h"
#include "BrowserDiagram.h"
#include "UmlGlobal.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "InstanceDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"

DeploymentNodeCanvas::DeploymentNodeCanvas(BrowserNode * bn, UmlCanvas * canvas,
					   int x, int y, int id)
    : DiagramCanvas(0, canvas, x, y, DEPLOYMENTNODE_CANVAS_MIN_SIZE,
		    DEPLOYMENTNODE_CANVAS_MIN_SIZE, id),
      itscolor(UmlDefaultColor), used_color(UmlDefaultColor) {
  browser_node = bn;
  write_horizontally = UmlDefaultState;
  check_size();
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}

DeploymentNodeCanvas::~DeploymentNodeCanvas() {
}

void DeploymentNodeCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  DiagramCanvas::delete_it();
}

void DeploymentNodeCanvas::deleted() {
  delete_it();
  canvas()->update();
}

const QString & DeploymentNodeCanvas::get_name() const {
  return iname;
}

void DeploymentNodeCanvas::set_name(const QString & s) {
  iname = s;
}

BrowserNode * DeploymentNodeCanvas::get_type() const {
  return browser_node;
}

BrowserNodeList & DeploymentNodeCanvas::get_types(BrowserNodeList & r) const {
  return BrowserDeploymentNode::instances(r);
}

void DeploymentNodeCanvas::set_type(BrowserNode * t) {
  if (browser_node != t) {
    disconnect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
    disconnect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
    browser_node = t;
    connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
    connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  }
}

int DeploymentNodeCanvas::min_width() {
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  
  int wi;
  
  if (horiz)
    wi = fm.width(iname + ":" + browser_node->get_name());
  else {
    wi = fm.width(browser_node->get_name());
    
    int iw = fm.width(iname + ":");
    
    if (iw > wi)
      wi = iw;
  }
  
  const char * st = browser_node->get_data()->get_stereotype();
  
  if (st[0]) {
    int stw = fm.width(QString("<<") + toUnicode(st) + ">>");
    
    if (wi < stw)
      wi = stw;
  }
  
  wi += (int) ((DEPLOYMENTNODE_CANVAS_ADDED + 8) * the_canvas()->zoom());
  
  int min_w = (int) (DEPLOYMENTNODE_CANVAS_MIN_SIZE * the_canvas()->zoom());
  
  return (wi < min_w)
    ? min_w
    : wi;
}

int DeploymentNodeCanvas::min_height() {
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  int three = (int) (3 * the_canvas()->zoom());
  int h = fm.height() + three;
  const char * st = browser_node->get_data()->get_stereotype();
  int he = h;
  
  if (st[0]) 
    he += h;
	    
  if (! horiz)
    he += h;

  he += (int) ((DEPLOYMENTNODE_CANVAS_ADDED + 4) * the_canvas()->zoom());
  
  int min_h = (int) (DEPLOYMENTNODE_CANVAS_MIN_SIZE * the_canvas()->zoom());
  
  return (he < min_h)
    ? min_h
    : he;
}

void DeploymentNodeCanvas::check_size() {
  DeploymentDiagramSettings dflt;
  
  dflt.write_horizontally = write_horizontally;
  the_canvas()->browser_diagram()->get_deploymentdiagramsettings(dflt);
  horiz = (dflt.write_horizontally == UmlYes);
  
  int wi = min_width();
  int he = min_height();
  
  // warning : do NOT check if ((width() < wi) || (height() < he))
  // because te resize must be done to set data on scale change
  DiagramCanvas::resize((width() < wi) ? wi : width(),
			(height() < he) ? he : height());
}

void DeploymentNodeCanvas::change_scale() {
  double scale = the_canvas()->zoom();
  
  QCanvasRectangle::setVisible(FALSE);
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  check_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);  
}

void DeploymentNodeCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QBrush brsh = p.brush();
  QColor bckgrnd = p.backgroundColor();
  UmlColor c;
  
  if (itscolor != UmlDefaultColor)
    c = itscolor;
  else {
    if (used_color == UmlDefaultColor)
      used_color = the_canvas()->browser_diagram()->get_color(UmlDeploymentNode);
    c = used_color;
  }
  
  QColor co = color(c);
  const int added = (int) (DEPLOYMENTNODE_CANVAS_ADDED * the_canvas()->zoom());
  QRect r = rect();
  QPointArray a(7);
  
  r.setTop(r.top() + added);
  r.setRight(r.right() - added);
  
  a.setPoint(0, r.left(), r.top());
  a.setPoint(1, r.left() + added, r.top() - added);
  a.setPoint(2, r.right() + added, r.top() - added);
  a.setPoint(3, r.right() + added, r.bottom() - added);
  a.setPoint(4, r.right(), r.bottom());
  a.setPoint(5, r.right(), r.top());
  a.setPoint(6, r.left(), r.top());
  
  if (c == UmlTransparent) {
    p.setBackgroundMode(QObject::TransparentMode);
    p.setBackgroundColor(co);
    p.drawPolyline(a);
  }
  else {
    p.setBackgroundMode(QObject::OpaqueMode);
    p.fillRect(r, co);
    p.setBrush(co);
    p.drawPolygon(a, TRUE, 0, 6);
    p.setBrush(brsh);
    p.setBackgroundColor(co);
  }

  p.drawRect(r);
  p.drawLine(r.topRight(), a.point(2));

  const int three = (int) (3 * the_canvas()->zoom());  
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  const char * st = browser_node->get_data()->get_stereotype();
    
  r.setTop(r.top() + three);
  p.setFont(the_canvas()->get_font(UmlNormalFont));  
  
  if (st[0]) {
    p.drawText(r, QObject::AlignHCenter + QObject::AlignTop, 
	       QString("<<") + toUnicode(st) + ">>");
    r.setTop(r.top() + fm.height() + three);
  }
  
  if (horiz)
    p.drawText(r, QObject::AlignHCenter + QObject::AlignTop,
	       iname + ":" + browser_node->get_name());
  else {
    p.drawText(r, QObject::AlignHCenter + QObject::AlignTop,
	       iname + ":");
    r.setTop(r.top() + fm.height() + three);
    p.drawText(r, QObject::AlignHCenter + QObject::AlignTop,
	       browser_node->get_name());
  }
  
  p.setBackgroundColor(bckgrnd);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode DeploymentNodeCanvas::type() const {
  return UmlDeploymentNode;
}

void DeploymentNodeCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool DeploymentNodeCanvas::alignable() const {
  return TRUE;
}

bool DeploymentNodeCanvas::copyable() const {
  return selected();
}

void DeploymentNodeCanvas::open() {
  InstanceDialog d(this, "node : ", "Node instance dialog");
  
  if (d.exec() == QDialog::Accepted)
    modified();
}

void DeploymentNodeCanvas::modified() {
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlDeploymentNode)
    : itscolor;
  // force son reaffichage
  hide();
  check_size();
  show();
  force_self_rel_visible();
  canvas()->update();
  package_modified();
}

void DeploymentNodeCanvas::menu(const QPoint&) {
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
  m.insertItem("Select node in browser", 4);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  m.insertItem("Set node associated diagram", 6);
  m.insertSeparator();
  m.insertItem("Remove from view", 7);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, UmlDeploymentNode, 10))
    m.insertItem("Tool", &toolm);

  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    upper();
    modified();	// call package_modified()
    return;
  case 1:
    lower();
    modified();	// call package_modified()
    return;
  case 2:
    open();
    return;
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
    ((BrowserDeploymentNode *) browser_node)
      ->set_associated_diagram((BrowserDeploymentDiagram *)
			       the_canvas()->browser_diagram());
    return;
  case 7:
    // remove from view
    delete_it();
    break;
  default:
    if (rank >= 10)
      ToolCom::run(Tool::command(rank - 10), browser_node);
    return;
  }
  
  package_modified();
}

void DeploymentNodeCanvas::apply_shortcut(QString s) {
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
}

void DeploymentNodeCanvas::edit_drawing_settings() {
  QArray<StateSpec> st(1);
  QArray<ColorSpec> co(1);
  
  st[0].set("write node instance \nhorizontally", &write_horizontally);
  co[0].set("Node color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();
}

bool DeploymentNodeCanvas::has_drawing_settings() const {
  return TRUE;
}

void DeploymentNodeCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<StateSpec> st(1);
  QArray<ColorSpec> co(1);
  Uml3States write_horizontally;
  UmlColor itscolor;
  
  st[0].set("write node instance \nhorizontally", &write_horizontally);
  co[0].set("Node color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      if (st[0].name != 0)
	((DeploymentNodeCanvas *) it.current())->write_horizontally =
	  write_horizontally;
      if (co[0].name != 0)
	((DeploymentNodeCanvas *) it.current())->itscolor = itscolor;
      ((DeploymentNodeCanvas *) it.current())->modified();	// call package_modified()
    }
  } 
}

const char * DeploymentNodeCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlDependency:
    l = UmlDependOn;
    return (browser_node->is_writable()) ? 0 : "read only";
  case UmlAssociation:
  case UmlAnchor:
    return 0;
  default:
    return "illegal";
  }
}

const char * DeploymentNodeCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlDeploymentNode:
    return (l != UmlAnchor) ? 0 : "can't have an anchor between nodes";
  case UmlNote:
  case UmlIcon:
    return (l == UmlAnchor) ? 0 : "illegal";
  case UmlHub:
    return (l == UmlAssociation) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

void DeploymentNodeCanvas::connexion(UmlCode action, DiagramItem * dest,
				     const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (action == UmlDependOn)
    a = new SimpleRelationCanvas(the_canvas(), this, dest, 0, action, 0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

aCorner DeploymentNodeCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void DeploymentNodeCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width(), min_height());
}

bool DeploymentNodeCanvas::move_with_its_package() const {
  return TRUE;
}

void DeploymentNodeCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "deploymentnodecanvas_ref " << get_ident() << " // " << browser_node->get_name();
  else {
    nl_indent(st);
    st << "deploymentnodecanvas " << get_ident() << ' ';
    indent(+1);
    browser_node->save(st, TRUE, warning);
    if (!iname.isEmpty()) {
      nl_indent(st);
      st << "name ";
      save_string(iname, st);  
    }
    nl_indent(st);
    if (write_horizontally != UmlDefaultState)
      st << "write_horizontally " << stringify(write_horizontally) << ' ';
    if (itscolor != UmlDefaultColor)
      st << "color " << stringify(itscolor) << ' ';
    save_xyzwh(st, this, "xyzwh");
    indent(-1);
  }
}

DeploymentNodeCanvas * DeploymentNodeCanvas::read(char * & st, UmlCanvas * canvas, char * k) {
  if (!strcmp(k, "deploymentnodecanvas_ref"))
    return ((DeploymentNodeCanvas *) dict_get(read_id(st), "deploymentnodecanvas", canvas));
  else if (!strcmp(k, "deploymentnodecanvas")) {
    int id = read_id(st);
    BrowserNode * br = BrowserDeploymentNode::read_ref(st);
    QString iname;
    Uml3States write_horizontally = UmlDefaultState;
    
    k = read_keyword(st);
    
    if (!strcmp(k, "name")) {
      iname = read_string(st);
      k = read_keyword(st);
    }
    if (!strcmp(k, "write_horizontally") ||
	!strcmp(k, "write_horizontaly")) {
      write_horizontally = state(read_keyword(st));
      k = read_keyword(st);
    }
    
    UmlColor color = UmlDefaultColor;
    
    read_color(st, "color", color, k);
    
    if (strcmp(k, "xyzwh"))
      wrong_keyword(k, "xyzwh");
    
    int x = (int) read_double(st);
    DeploymentNodeCanvas * result =
      new DeploymentNodeCanvas(br, canvas, x, (int) read_double(st), id);
    
    result->iname = iname;
    result->write_horizontally = write_horizontally;
    result->itscolor = color;
    read_zwh(st, result);
    result->check_size();
    result->set_center100();

    result->show();
    return result;
  }
  else
    return 0;
}

void DeploymentNodeCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
}

void DeploymentNodeCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void DeploymentNodeCanvas::history_load(QBuffer & b) {
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
