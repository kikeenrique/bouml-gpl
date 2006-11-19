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

#include "FragmentCanvas.h"
#include "FragmentDialog.h"
#include "MLEDialog.h"
#include "UmlGlobal.h"
#include "UmlCanvas.h"
#include "BrowserDiagram.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "DialogUtil.h"

FragmentCanvas::FragmentCanvas(UmlCanvas * canvas, int x, int y, int id)
    : DiagramCanvas(0, canvas, x, y, FRAGMENT_CANVAS_MIN_SIZE,
		    FRAGMENT_CANVAS_MIN_SIZE, id) {
  browser_node = canvas->browser_diagram();
  itscolor = UmlDefaultColor;
  check_size();
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

FragmentCanvas::~FragmentCanvas() {
}

void FragmentCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  DiagramCanvas::delete_it();
}


void FragmentCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  QColor bckgrnd = p.backgroundColor();

  p.setBackgroundMode((used_color == UmlTransparent) ? QObject::TransparentMode : QObject::OpaqueMode);

  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  
  if (used_color != UmlTransparent) p.fillRect(r, co);
  p.drawRect(r);
  
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  int w = fm.width((name.isEmpty()) ? QString("X") : name);
  int h = fm.height() / 2;
  
  r.setWidth(w + h);
  r.setHeight(fm.height() + h);
  if (!name.isEmpty())
    p.drawText(r, QObject::AlignCenter, name);
  
  h = (fm.height() + h)/2;
  p.drawLine(r.left(), r.bottom(),
	     r.right(), r.bottom());
  p.drawLine(r.right(), r.bottom(),
	     r.right() + h, r.bottom() - h);
  p.drawLine(r.right() + h, r.bottom() - h,
	     r.right() + h, r.top());
	     
  p.setBackgroundColor(bckgrnd);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode FragmentCanvas::type() const {
  return UmlFragment;
}

void FragmentCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

bool FragmentCanvas::alignable() const {
  return TRUE;
}

bool FragmentCanvas::copyable() const {
  return selected();
}

void FragmentCanvas::open() {
  static QStringList dflt;
  
  if (dflt.isEmpty()) {
    dflt.append("alt");
    dflt.append("assert");
    dflt.append("break");
    dflt.append("consider");
    dflt.append("critical");
    dflt.append("else");
    dflt.append("ignore");
    dflt.append("loop");
    dflt.append("neg");
    dflt.append("opt");
    dflt.append("par");
    dflt.append("seq");
    dflt.append("strict");
  }
  
  FragmentDialog d(dflt, name);
  
  d.exec();
  modified();
}

void FragmentCanvas::check_size() {
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  
  min_width = fm.width((name.isEmpty()) ? QString("X") : name);
  min_height = fm.height() * 2;
  
  min_width += min_height;
  min_height += fm.height();
  
  int min = (int) (FRAGMENT_CANVAS_MIN_SIZE * the_canvas()->zoom());
  
  if (min_width < min)
    min_width = min;
  if (min_height < min)
    min_height = min;
  
  // force odd width and height for line alignment
  min_width |= 1;
  min_height |= 1;
    
  // warning : do NOT check if ((width() < min_width) || (height() < min_height))
  // because te resize must be done to set data on scale change
  DiagramCanvas::resize((width() > min_width) ? width() : min_width,
			(height() > min_height) ? height() : min_height);

  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlFragment)
    : itscolor;
}

void FragmentCanvas::change_scale() {
  double scale = the_canvas()->zoom();
  
  QCanvasRectangle::setVisible(FALSE);
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  check_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);  
}

void FragmentCanvas::modified() {
  // force son reaffichage
  hide();
  check_size();
  show();
  canvas()->update();
  package_modified();
}

void FragmentCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Fragment", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 3);
  if (linked()) {
    m.insertSeparator();
    m.insertItem("Select linked items", 4);
  }
  m.insertSeparator();
  m.insertItem("Remove from view",5);

  int index = m.exec(QCursor::pos());
  
  switch (index) {
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
    // modified then package_modified already called
    return;
  case 3:
    {
      QArray<ColorSpec> co(1);
      
      co[0].set("fragment color", &itscolor);

      SettingsDialog dialog(0, &co, FALSE, TRUE);
      
      dialog.raise();
      if (dialog.exec() == QDialog::Accepted)
	modified();
      return;
    }
    break;
  case 4:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 5:
    delete_it();
    break;
  default:
    return;
  }
  
  package_modified();
}

bool FragmentCanvas::has_drawing_settings() const {
  return TRUE;
}

void FragmentCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  
  co[0].set("fragment color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if ((dialog.exec() == QDialog::Accepted) && (co[0].name != 0)) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      ((FragmentCanvas *) it.current())->itscolor = itscolor;
      ((FragmentCanvas *) it.current())->modified();	// call package_modified()    
    }
  }
}

const char * FragmentCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : "illegal";
}

const char * FragmentCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlAnchor:
  case UmlLabel:
  case UmlArrowPoint:
  case UmlLifeLine:
    return "illegal";
  default:
    return (l == UmlAnchor) ? 0 : "illegal";
  }
}

aCorner FragmentCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void FragmentCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width, min_height);
}

void FragmentCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref) {
    st << "fragment_ref " << get_ident();
  }
  else {
    nl_indent(st);
    
    st << "fragment " << get_ident() << ' ';
    save_string(name, st);
    nl_indent(st);
    if (itscolor != UmlDefaultColor)
      st << "  color " << stringify(itscolor);
    save_xyzwh(st, this, "  xyzwh");
  }
}

FragmentCanvas * FragmentCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "fragment_ref"))
    return (FragmentCanvas *) dict_get(read_id(st), "fragment", canvas);
  else if (!strcmp(k, "fragment")) {
    int id = read_id(st);
    FragmentCanvas * result = new FragmentCanvas(canvas, 0, 0, id);
    
    result->name = read_string(st);
    
    k = read_keyword(st);
    read_color(st, "color", result->itscolor, k);
    
    if (strcmp(k, "xyzwh"))
      wrong_keyword(k, "xyzwh");
    
    read_xyzwh(st, result);
    result->check_size();
    result->width_scale100 = result->width();
    result->height_scale100 = result->height();
    result->set_center100();
    result->show();
    return result;
  }
  else
    return 0;
}


void FragmentCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(min_width, b);
  ::save(min_height, b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void FragmentCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(min_width, b);
  ::load(min_height, b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
  
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

void FragmentCanvas::history_hide() {
  DiagramCanvas::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

