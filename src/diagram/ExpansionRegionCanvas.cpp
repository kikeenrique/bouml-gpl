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

#include "ExpansionRegionCanvas.h"
#include "ExpansionRegionData.h"
#include "BrowserExpansionRegion.h"
#include "BrowserActivityDiagram.h"
#include "BrowserExpansionNode.h"
#include "ExpansionNodeCanvas.h"
#include "ArrowCanvas.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"
#include "strutil.h"

ExpansionRegionCanvas::ExpansionRegionCanvas(BrowserNode * bn, UmlCanvas * canvas,
					     int x, int y)
    : ActivityContainerCanvas(0, canvas, x, y, 
			      EXPANSION_REGION_CANVAS_MIN_SIZE * 4,
			      EXPANSION_REGION_CANVAS_MIN_SIZE * 4, 0) {
  browser_node = bn;
  itscolor = UmlDefaultColor;

  compute_size();
  check_nodes();
  
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

ExpansionRegionCanvas::ExpansionRegionCanvas(UmlCanvas * canvas, int id)
    : ActivityContainerCanvas(canvas, id) {
  // for read operation
  browser_node = 0;
  itscolor = UmlDefaultColor;
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

ExpansionRegionCanvas::~ExpansionRegionCanvas() {
}

void ExpansionRegionCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  
  DiagramCanvas::delete_it();
  
  QValueList<ExpansionNodeCanvas *>::Iterator iter;
  
  for (iter = nodes.begin(); iter != nodes.end(); ++iter)
    // don't empty nodes to manage undo
    (*iter)->delete_it();
}

void ExpansionRegionCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void ExpansionRegionCanvas::deleted(ExpansionNodeCanvas * n) {
  nodes.remove(n);
}

void ExpansionRegionCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void ExpansionRegionCanvas::compute_size() {
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlExpansionRegion)
    : itscolor;
  
  QString s;
  
  switch (((ExpansionRegionData *) 
	   ((BrowserExpansionRegion *) browser_node)->get_data())
	  ->get_mode()) {
  case UmlParallel:
    s = "<<parallel>>";
    break;
  case UmlIterative:
    s = "<<iterative>>";
    break;
  default:
    // UmlStream
    s = "<<stream>>";
    break;
  }
  
  double zoom = the_canvas()->zoom();
  int margin = (int) (12 * zoom);
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
				 
  min_width = fm.width(s) + margin;
  if (min_width < EXPANSION_REGION_CANVAS_MIN_SIZE * zoom)
    min_width = (int) (EXPANSION_REGION_CANVAS_MIN_SIZE * zoom);
      
  min_height = fm.height() + margin;
  if (min_height < EXPANSION_REGION_CANVAS_MIN_SIZE * zoom)
    min_height = (int) (EXPANSION_REGION_CANVAS_MIN_SIZE * zoom);
  
  DiagramCanvas::resize((width() > min_width) ? width() : min_width,
			(height() > min_height) ? height() : min_height);
 
  force_nodes_arround();
}

void ExpansionRegionCanvas::moveBy(double dx, double dy) {
  DiagramCanvas::moveBy(dx, dy);
  
  // update expansion nodes position
  QValueList<ExpansionNodeCanvas *>::Iterator iter;
  
  for (iter = nodes.begin(); iter != nodes.end(); ++iter)
    (*iter)->do_moveBy(dx, dy);
}

void ExpansionRegionCanvas::change_scale() {
  QCanvasRectangle::setVisible(FALSE);
  double scale = the_canvas()->zoom();
    
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  compute_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);
  
  // update expansion nodes position
  QValueList<ExpansionNodeCanvas *>::Iterator iter;
  
  for (iter = nodes.begin(); iter != nodes.end(); ++iter)
    (*iter)->do_change_scale();
}

void ExpansionRegionCanvas::modified() {
  hide();
  hide_lines();
  compute_size();
  show();
  update_show_lines();
  check_nodes();
  canvas()->update();
  force_sub_inside();
  package_modified();
}

aCorner ExpansionRegionCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void ExpansionRegionCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width, min_height);
  
  
  force_nodes_arround();
  force_sub_inside();
}

bool ExpansionRegionCanvas::move_with_its_package() const {
  return TRUE;
}

void ExpansionRegionCanvas::set_z(double z) {
  setZ(z);
  
  z += 1;
  
  QValueList<ExpansionNodeCanvas *>::Iterator iter;
  
  for (iter = nodes.begin(); iter != nodes.end(); ++iter)
    (*iter)->set_z(z);
}

void ExpansionRegionCanvas::force_sub_inside() {
  // update sub nodes position to be inside of the region
  // except the expansion nodes whose are in the border
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator cit;
  
  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible() && !(*cit)->selected()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*cit);
      
      if ((di != 0) &&
	  (di->get_bn() != 0) &&
	  (((BrowserNode *) di->get_bn())->parent() == browser_node) &&
	  (di->type() != UmlExpansionNode))
	ActivityContainerCanvas::force_inside(di, *cit);
    }
  }
}

void ExpansionRegionCanvas::force_inside() {
  // if its parent is present, force inside it
  
  QCanvasItemList all = the_canvas()->allItems();
  QCanvasItemList::Iterator cit;
  BrowserNode * parent = (BrowserNode *) browser_node->parent();

  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*cit);
      
      if ((di != 0) &&
	  IsaActivityContainer(di->type()) &&
	  (((ActivityContainerCanvas *) di)->get_bn() == parent)) {
	((ActivityContainerCanvas *) di)->force_inside(this, this);
	break;
      }
    }
  }
}

void ExpansionRegionCanvas::check_nodes() {
  // add missing nodes
  const QValueList<BrowserExpansionNode *> brnodes =
    ((BrowserExpansionRegion *) browser_node)->get_nodes();
  QValueList<BrowserExpansionNode *>::ConstIterator iter;
  double zoom = the_canvas()->zoom();
  QRect r = rect();
  int cx = (int) (x() + (width() - zoom * EXPANSIONNODE_WIDTH) / 2);
  int rx = r.right() - (int) (zoom * EXPANSIONNODE_WIDTH);
  int ty = (int) (y() - zoom * EXPANSIONNODE_HEIGHT / 2);
  int by = r.bottom() - (int) (zoom * EXPANSIONNODE_HEIGHT / 2);
  int rank;

  // thanks to the min size of an expansion region cx and rx 
  // are valid, ie not ouside
  
  for (iter = brnodes.begin(), rank = 0; iter != brnodes.end(); ++iter, rank += 1) {
    QValueList<ExpansionNodeCanvas *>::ConstIterator itershown;
    
    for (itershown = nodes.begin(); itershown != nodes.end(); ++itershown)
      if ((*itershown)->get_bn() == *iter)
	break;
    
    if (itershown == nodes.end()) {
      // add it
      int px;
      int py;
      
      switch (rank % 6) {
      case 0:
	px = (int) x();
	py = ty;
	break;
      case 1:
	px = (int) x();
	py = by;
	break;
      case 2:
	px = cx;
	py = ty;
	break;
      case 3:
	px = cx;
	py = by;
	break;
      case 4:
	px = rx;
	py = ty;
	break;
      default:
	px = rx;
	py = by;
      }
      
      ExpansionNodeCanvas * pc =
	new ExpansionNodeCanvas(*iter, the_canvas(), px, py, 0, this);
      
      nodes.append(pc);
      pc->show();
    }
  }
}

void ExpansionRegionCanvas::force_nodes_arround() {
  QValueList<ExpansionNodeCanvas *>::Iterator iter;
  
  for (iter = nodes.begin(); iter != nodes.end(); ++iter)
    (*iter)->check_position();
}

void ExpansionRegionCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  QBrush brsh = p.brush();
  QColor bckgrnd = p.backgroundColor();
  
  p.setBackgroundMode((used_color == UmlTransparent)
		      ? QObject::TransparentMode
		      : QObject::OpaqueMode);

  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  
  if (used_color != UmlTransparent) 
    p.setBrush(co);
  
  p.setPen(QObject::DotLine);
  p.drawRoundRect(r, 8, 8);

  int margin = (int) (6 * the_canvas()->zoom());
  QString s;
  
  switch (((ExpansionRegionData *) 
	   ((BrowserExpansionRegion *) browser_node)->get_data())
	  ->get_mode()) {
  case UmlParallel:
    s = "<<parallel>>";
    break;
  case UmlIterative:
    s = "<<iterative>>";
    break;
  default:
    // UmlStream
    s = "<<stream>>";
    break;
  }
  
  r.setTop(r.top() + margin);
  r.setLeft(r.left() + margin);
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  p.drawText(r, QObject::AlignLeft, s);
  
  p.setPen(QObject::SolidLine);      
  p.setBackgroundColor(bckgrnd);
  p.setBrush(brsh);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode ExpansionRegionCanvas::type() const {
  return UmlExpansionRegion;
}

void ExpansionRegionCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool ExpansionRegionCanvas::alignable() const {
  return TRUE;
}

bool ExpansionRegionCanvas::copyable() const {
  return selected();
}

void ExpansionRegionCanvas::open() {
  browser_node->open(FALSE);
}

void ExpansionRegionCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  int index;
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  if (browser_node->is_writable()) {
    m.insertItem("add expansion node", 9);
    m.insertSeparator();
  }
  m.insertItem("upper", 0);
  m.insertItem("lower", 1);
  m.insertSeparator();
  m.insertItem("edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("edit expansion region", 3);
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
  if (Tool::menu_insert(&toolm, UmlExpansionRegion, 10))
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
      QArray<ColorSpec> co(1);
      
      co[0].set("expansion region color", &itscolor);

      SettingsDialog dialog(0, &co, FALSE, TRUE);
      
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
    ((BrowserExpansionRegion *) browser_node)
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
    if (((BrowserExpansionRegion *) browser_node)->add_expansionnode(0, 0) != 0)
      modified();
    break;
  default:
    if (index >= 10)
      ToolCom::run(Tool::command(index - 10), browser_node);
    return;
  }
  
  package_modified();
}

const char * ExpansionRegionCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : "illegal";
}

const char * ExpansionRegionCanvas::may_connect(UmlCode &, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlNote:
  case UmlIcon:
    return 0;
  default:
    return "illegal";
  }
}

void ExpansionRegionCanvas::connexion(UmlCode action, DiagramItem * dest,
						  const QPoint &, const QPoint &) {
  ArrowCanvas * a =
    new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

void ExpansionRegionCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "expansionregioncanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "expansionregioncanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    if (itscolor != UmlDefaultColor) {
      nl_indent(st);
      st << "color " << stringify(itscolor);
    }
    nl_indent(st);
    save_xyzwh(st, this, "xyzwh");
    
    if (! nodes.isEmpty()) {
      nl_indent(st);
      st << "nodes";
      indent(+1);
    
      QValueList<ExpansionNodeCanvas *>::ConstIterator iter;
  
      for (iter = nodes.begin(); iter != nodes.end(); ++iter)
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

ExpansionRegionCanvas * 
  ExpansionRegionCanvas::read(char * & st, UmlCanvas * canvas,
					  char * k)
{
  if (!strcmp(k, "expansionregioncanvas_ref"))
    return ((ExpansionRegionCanvas *) dict_get(read_id(st), "expansionregioncanvas", canvas));
  else if (!strcmp(k, "expansionregioncanvas")) {
    int id = read_id(st);
    BrowserExpansionRegion * br = BrowserExpansionRegion::read_ref(st);
    ExpansionRegionCanvas * result = new ExpansionRegionCanvas(canvas, id);
    
    result->browser_node = br;
    connect(br->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(br->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));

    k = read_keyword(st);
    
    read_color(st, "color", result->itscolor, k);	// updates k
    
    if (!strcmp(k, "xyzwh"))
      read_xyzwh(st, result);
    else
      wrong_keyword(k, "xyzwh");
    result->compute_size();
    result->set_center100();
    result->show();
    
    k = read_keyword(st);

    if (! strcmp(k, "nodes")) {
      while (strcmp(k = read_keyword(st), "end") != 0)
	result->nodes.append(ExpansionNodeCanvas::read(st, canvas, k, result));
      k = read_keyword(st);
    }

    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    result->check_nodes();
    
    // result->force_sub_inside() useless
    
    return result;
  }
  else 
    return 0;
}

void ExpansionRegionCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(min_width, b);
  ::save(min_height, b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void ExpansionRegionCanvas::history_load(QBuffer & b) {
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

void ExpansionRegionCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);

  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
}
