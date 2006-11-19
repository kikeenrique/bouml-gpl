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

#include "InterruptibleActivityRegionCanvas.h"
#include "SimpleData.h"
#include "BrowserInterruptibleActivityRegion.h"
#include "BrowserActivityDiagram.h"
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

InterruptibleActivityRegionCanvas::InterruptibleActivityRegionCanvas(BrowserNode * bn, UmlCanvas * canvas,
								     int x, int y)
    : ActivityContainerCanvas(0, canvas, x, y, 
			      INTERRUPTIBLE_ACTIVITY_REGION_CANVAS_MIN_SIZE,
			      INTERRUPTIBLE_ACTIVITY_REGION_CANVAS_MIN_SIZE, 0) {
  browser_node = bn;
  itscolor = UmlDefaultColor;

  update();
  
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

InterruptibleActivityRegionCanvas::InterruptibleActivityRegionCanvas(UmlCanvas * canvas, int id)
    : ActivityContainerCanvas(canvas, id) {
  // for read operation
  browser_node = 0;
  itscolor = UmlDefaultColor;
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

InterruptibleActivityRegionCanvas::~InterruptibleActivityRegionCanvas() {
}

void InterruptibleActivityRegionCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  
  DiagramCanvas::delete_it();
}

void InterruptibleActivityRegionCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void InterruptibleActivityRegionCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void InterruptibleActivityRegionCanvas::update() {
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlInterruptibleActivityRegion)
    : itscolor;
}

void InterruptibleActivityRegionCanvas::modified() {
  hide();
  hide_lines();
  update();
  show();
  update_show_lines();
  canvas()->update();
  force_sub_inside();
  package_modified();
}

aCorner InterruptibleActivityRegionCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void InterruptibleActivityRegionCanvas::resize(aCorner c, int dx, int dy) {
  double zoom = the_canvas()->zoom();
  
  DiagramCanvas::resize(c, dx, dy, 
			(int) (INTERRUPTIBLE_ACTIVITY_REGION_CANVAS_MIN_SIZE * zoom),
			(int) (INTERRUPTIBLE_ACTIVITY_REGION_CANVAS_MIN_SIZE * zoom));
  
  force_sub_inside();
}

bool InterruptibleActivityRegionCanvas::move_with_its_package() const {
  return TRUE;
}

void InterruptibleActivityRegionCanvas::force_sub_inside() {
  // update sub nodes position to be inside of the activity region
  QCanvasItemList all = canvas()->allItems();
  QCanvasItemList::Iterator cit;
  
  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible() && !(*cit)->selected()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*cit);
      
      if ((di != 0) &&
	  (di->get_bn() != 0) &&
	  (((BrowserNode *) di->get_bn())->parent() == browser_node))
	ActivityContainerCanvas::force_inside(di, *cit);
    }
  }
}

void InterruptibleActivityRegionCanvas::force_inside() {
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

void InterruptibleActivityRegionCanvas::draw(QPainter & p) {
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
  
  p.setPen(QObject::SolidLine);      
  p.setBackgroundColor(bckgrnd);
  p.setBrush(brsh);
  
  if (selected())
    show_mark(p, r);
}

UmlCode InterruptibleActivityRegionCanvas::type() const {
  return UmlInterruptibleActivityRegion;
}

void InterruptibleActivityRegionCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool InterruptibleActivityRegionCanvas::alignable() const {
  return TRUE;
}

bool InterruptibleActivityRegionCanvas::copyable() const {
  return selected();
}

void InterruptibleActivityRegionCanvas::open() {
  browser_node->open(FALSE);
}

void InterruptibleActivityRegionCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  int index;
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("Edit interruptible activity region", 3);
  m.insertSeparator();
  m.insertItem("Select in browser", 4);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  if (browser_node->is_writable())
    m.insertItem("Set associated diagram",6);
  m.insertSeparator();
  m.insertItem("Remove from view", 7);
  if (browser_node->is_writable())
    m.insertItem("Delete from model", 8);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, UmlInterruptibleActivityRegion, 10))
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
    {
      QArray<ColorSpec> co(1);
      
      co[0].set("interruptible activity region color", &itscolor);

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
    ((BrowserInterruptibleActivityRegion *) browser_node)
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
  default:
    if (index >= 10)
      ToolCom::run(Tool::command(index - 10), browser_node);
    return;
  }
  
  package_modified();
}

bool InterruptibleActivityRegionCanvas::has_drawing_settings() const {
  return TRUE;
}

void InterruptibleActivityRegionCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  
  co[0].set("interruptible activity region color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if ((dialog.exec() == QDialog::Accepted) && (co[0].name != 0)) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      ((InterruptibleActivityRegionCanvas *) it.current())->itscolor = itscolor;
      ((InterruptibleActivityRegionCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

const char * InterruptibleActivityRegionCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : "illegal";
}

const char * InterruptibleActivityRegionCanvas::may_connect(UmlCode &, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlNote:
  case UmlIcon:
    return 0;
  default:
    return "illegal";
  }
}

void InterruptibleActivityRegionCanvas::connexion(UmlCode action, DiagramItem * dest,
						  const QPoint &, const QPoint &) {
  ArrowCanvas * a =
    new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

void InterruptibleActivityRegionCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "interruptibleactivityregioncanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "interruptibleactivityregioncanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    if (itscolor != UmlDefaultColor) {
      nl_indent(st);
      st << "color " << stringify(itscolor);
    }
    nl_indent(st);
    save_xyzwh(st, this, "xyzwh");

    indent(-1);
    nl_indent(st);
    st << "end";
  }
}

InterruptibleActivityRegionCanvas * 
  InterruptibleActivityRegionCanvas::read(char * & st, UmlCanvas * canvas,
					  char * k)
{
  if (!strcmp(k, "interruptibleactivityregioncanvas_ref"))
    return ((InterruptibleActivityRegionCanvas *) dict_get(read_id(st), "interruptibleactivityregioncanvas", canvas));
  else if (!strcmp(k, "interruptibleactivityregioncanvas")) {
    int id = read_id(st);
    BrowserInterruptibleActivityRegion * br =
      BrowserInterruptibleActivityRegion::read_ref(st);
    InterruptibleActivityRegionCanvas * result =
      new InterruptibleActivityRegionCanvas(canvas, id);
    
    result->browser_node = br;
    connect(br->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(br->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));

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
    
    read_keyword(st, "end");
    
    // result->force_sub_inside() useless
    
    return result;
  }
  else 
    return 0;
}

void InterruptibleActivityRegionCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void InterruptibleActivityRegionCanvas::history_load(QBuffer & b) {
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

void InterruptibleActivityRegionCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);

  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
}
