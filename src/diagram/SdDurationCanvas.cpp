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

#include <qpainter.h>
#include <qcursor.h>
#include <qpopupmenu.h> 

#include "SdDurationCanvas.h"
#include "SdLifeLineCanvas.h"
#include "SdClassInstCanvas.h"
#include "SdSelfMsgCanvas.h"
#include "SdMsgCanvas.h"
#include "UmlCanvas.h"
#include "BrowserDiagram.h"
#include "UmlGlobal.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "MenuTitle.h"

#define DURATION_MIN_HEIGHT 25
#define DURATION_START_HEIGHT 40

#define DURATION_TOP 70

SdDurationCanvas::SdDurationCanvas(UmlCanvas * canvas, SdLifeLineCanvas * ll,
				   int v, bool isdest)
    : DiagramCanvas(0, canvas, 0, v, DURATION_WIDTH,
		    (isdest) ? DURATION_MIN_HEIGHT : DURATION_START_HEIGHT,
		    0),
      line(ll), itscolor(UmlDefaultColor) {
  browser_node = canvas->browser_diagram();
  update_hpos();
  setZ(DIAGRAMCANVAS_Z + 10);	// == LifeLine.z() + 10
  show();
  
  line->add(this);

  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

SdDurationCanvas::SdDurationCanvas(UmlCanvas * canvas, SdLifeLineCanvas * ll,
				   int x, int y, int wi, int he, int id)
    : DiagramCanvas(0, canvas, x, y, wi, he, id),
      line(ll), itscolor(UmlDefaultColor) {
  browser_node = canvas->browser_diagram();
  line->add(this);
}

SdDurationCanvas::~SdDurationCanvas() {
}

void SdDurationCanvas::delete_it() {
  while (!msgs.isEmpty())
    msgs.first()->delete_it();	// will update msgs
  
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  line->remove(this);
  DiagramCanvas::delete_it();
}

void SdDurationCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

void SdDurationCanvas::change_scale() {
  DiagramCanvas::change_scale();
  
  hide();
  if ((width() & 1) == 0)
    // force odd width
    setSize(width() + 1, height());
  update_hpos();
  show();
}

void SdDurationCanvas::update_hpos() {
  QRect r = line->rect();
  
  move((r.left() + r.right() - width())/2 + 1, 100000);
}

void SdDurationCanvas::draw(QPainter & p) {
  const QRect r = rect();
  
  p.setBackgroundMode(QObject::OpaqueMode);
  p.fillRect(r, color((itscolor != UmlDefaultColor)
		      ? itscolor
		      : browser_node->get_color(UmlActivityDuration)));
  p.drawRect(r);
  
  if (selected())
    show_mark(p, r);
}

void SdDurationCanvas::moveBy(double dx, double dy) {
  if (dy > 80000) {
    // from update_hpos
    DiagramCanvas::moveBy(dx, 0);
    
    // update messages position (even selected, this case is managed by the messages)
    QListIterator<SdMsgBaseCanvas> it(msgs);
    
    for ( ; it.current(); ++it )
      it.current()->update_hpos();
  }
  else {
    if ((y() + dy) < min_y())
      dy = min_y() - y();
    
    // accept only vertical move
    DiagramCanvas::moveBy(0, dy);
    
    // deplacer les SdDurationCanvas qui ne sont pas selectionnees et qu il recouvre

    // move messages (even selected, this case is managed by the messages)
    QListIterator<SdMsgBaseCanvas> it(msgs);
    
    for ( ; it.current(); ++it )
      it.current()->moveBy(100000, dy);
  }
  
  line->update_instance_dead();
}

double SdDurationCanvas::min_y() const {
  return line->y() + 5;
}

UmlCode SdDurationCanvas::type() const {
  return UmlActivityDuration;
}

DiagramItem::LineDirection SdDurationCanvas::allowed_direction(UmlCode c) {
  return (c == UmlAnchor) ? DiagramItem::All : DiagramItem::Horizontal;
}

void SdDurationCanvas::update_v_to_contain(const QRect re) {
  const QRect r = rect();
  
  if (re.top() < r.top()) {
    int dy = re.top() - r.top();
    
    QCanvasItem::moveBy(0, dy);
    DiagramCanvas::resize(r.width(), r.height() - dy);
  
    line->update_instance_dead();
  }
  else if (re.bottom() > r.bottom()) {
    DiagramCanvas::resize(r.width(), re.bottom() - r.top() + 1);
  
    line->update_instance_dead();
  }
}

const char * SdDurationCanvas::may_start(UmlCode &) const {
  return 0;
}

const char * SdDurationCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlNote:
  case UmlIcon:
    return (l == UmlAnchor) ? 0 : "illegal";
  case UmlActivityDuration:
  case UmlLifeLine:
    return (l != UmlAnchor) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

void SdDurationCanvas::connexion(UmlCode l, DiagramItem * dest,
				 const QPoint & s, const QPoint & e) {
  switch (l) {
  case UmlSyncSelfMsg:
  case UmlAsyncSelfMsg:
    new SdSelfMsgCanvas(the_canvas(), this, l, s.y(), 0);
    break;
  case UmlAnchor:
    DiagramCanvas::connexion(l, dest, s, e);
    break;
  default:
    if (dest != this) {
      if (dest->type() == UmlLifeLine)
	// insert an activity duration canvas
	dest = new SdDurationCanvas(the_canvas(), ((SdLifeLineCanvas *) dest), s.y(), TRUE);
      
      new SdMsgCanvas(the_canvas(), this, ((SdDurationCanvas *) dest), l, s.y(), 0);
    }
  }
}

void SdDurationCanvas::add(SdMsgBaseCanvas * m) {
  msgs.append(m);
}

void SdDurationCanvas::remove(SdMsgBaseCanvas * m) {
  msgs.removeRef(m);
  
  if (msgs.isEmpty())
    // now useless, remove it
    delete_it();
}

void SdDurationCanvas::cut(const QPoint & p) {
  int py = p.y();
  QRect r = rect();
  DiagramCanvas::resize(width(), py - r.top());
  QList<SdMsgBaseCanvas> ms(msgs);
  QListIterator<SdMsgBaseCanvas> it(ms);
  SdDurationCanvas * newone = 0;
  
  for ( ; it.current(); ++it ) {
    if (it.current()->rect().center().y() > py) {
      // on the new duration
      if (newone == 0) { 
	// width and height to 0 on creation to not a round error
	// because of the zoom
	newone = new SdDurationCanvas(the_canvas(), line, r.x(), py, 0, 0, 0);
	newone->DiagramCanvas::resize(width(), r.bottom() - py);
	newone->itscolor = itscolor;
	newone->setZ(z());
	newone->show();
      }
      remove(it.current());
      newone->add(it.current());
      it.current()->change_duration(this, newone);
    }
  }
  
  package_modified();
}

void SdDurationCanvas::merge(QList<SdDurationCanvas> & l) {
  l.removeRef(this);
  
  QRect r = rect();
  int vmin = r.top();
  int vmax = r.bottom();
  SdDurationCanvas * d;
  
  for (d = l.first(); d != 0; d = l.next()) {
    QRect dr = d->rect();
    
    if (dr.top() < vmin)
      vmin = dr.top();
    if (dr.bottom() > vmax)
      vmax = dr.bottom();
    
    QList<SdMsgBaseCanvas> & ms = d->msgs;
    
    while (! ms.isEmpty()) {
      SdMsgBaseCanvas * m = ms.first();
      
      msgs.append(m);
      m->change_duration(d, this);
      ms.removeRef(m);
    }
    
    d->delete_it();
  }
  
  if (vmin < r.top())
    QCanvasItem::moveBy(0, vmin - r.top());

  DiagramCanvas::resize(r.width(), vmax - vmin + 1);  
}

void SdDurationCanvas::open() {
  // package_modified();
}

void SdDurationCanvas::modified() {
  // force son reaffichage
  hide();
  show();
  canvas()->update();
  package_modified();
}

void SdDurationCanvas::menu(const QPoint & p) {
  QPopupMenu m(0);
  QCanvasItemList items = collisions(TRUE);
  QList<SdDurationCanvas> l;
  QCanvasItemList::ConstIterator it;
  QCanvasItemList::ConstIterator end = items.end();
  
  for (it = items.begin(); it != end; ++it) {
    if ((*it)->visible()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*it);
      
      if ((di != 0) &&
	  (di->type() == UmlActivityDuration) &&
	  (((SdDurationCanvas *) di)->line == line))
	l.append((SdDurationCanvas *) di);
    }
  }
  
  m.insertItem(new MenuTitle("Activity bar", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("Select linked items", 3);
  m.insertSeparator();
  m.insertItem("Remove from view", 4);
  m.insertSeparator();
  m.insertItem("Cut here", 5);
  if (!l.isEmpty())
    m.insertItem("Merge juxtaposed activity bars", 6);

  switch (m.exec(QCursor::pos())) {
  case 0:
    upper();
    modified();
    return;
  case 1:
    lower();
    modified();
    return;
  case 2:
    edit_drawing_settings();
    return;
  case 3:
    select_associated();
    break;
  case 4:
    delete_it();
    package_modified();
    break;
  case 5:
    cut(p);
    package_modified();
    break;
  case 6:
    merge(l);
    package_modified();
    break;
  default:
    return;
  }
  
  canvas()->update();
}

void SdDurationCanvas::apply_shortcut(QString s) {
  if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Edit drawing settings") {
    edit_drawing_settings();
    return;
  }
  else 
    return;

  modified();
}

void SdDurationCanvas::edit_drawing_settings() {
  QArray<ColorSpec> co(1);
  
  co[0].set("duration color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();
}

bool SdDurationCanvas::has_drawing_settings() const {
  return TRUE;
}

void SdDurationCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  
  co[0].set("duration color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if ((dialog.exec() == QDialog::Accepted) && (co[0].name != 0)) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      ((SdDurationCanvas *) it.current())->itscolor = itscolor;
      ((SdDurationCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

aCorner SdDurationCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void SdDurationCanvas::resize(aCorner c, int, int dy) {
  double zoom = the_canvas()->zoom();
  
  DiagramCanvas::resize(c, 0, dy,
			(int) (DURATION_WIDTH * zoom),
			(int) (DURATION_MIN_HEIGHT * zoom));
  
  line->update_instance_dead();
}

void SdDurationCanvas::select_associated() {
  the_canvas()->select(this);
  
  QListIterator<SdMsgBaseCanvas> it(msgs);
  
  for ( ; it.current(); ++it )
    if (! it.current()->selected())
      it.current()->select_associated();
}

bool SdDurationCanvas::copyable() const {
  if (! selected())
    return FALSE;
  
  QListIterator<SdMsgBaseCanvas> it(msgs);
  
  for ( ; it.current(); ++it )
    if (it.current()->copyable())
      return TRUE;
  
  return FALSE;
}

void SdDurationCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "durationcanvas_ref " << get_ident() << " // "
      << line->get_obj()->get_ident();
  else {
    nl_indent(st);
    st << "durationcanvas " << get_ident() << ' ';
    ((DiagramCanvas *) line->get_obj())->save(st, TRUE, warning);
    if (itscolor != UmlDefaultColor) {
      nl_indent(st);
      st << "color " << stringify(itscolor);
    }
    nl_indent(st);
    save_xyzwh(st, this, "  xyzwh");
  }
}

SdDurationCanvas * SdDurationCanvas::read(char * & st, UmlCanvas * canvas,
					  bool ref)
{
  return read(st, canvas,
	      read_keyword(st, (ref) ? "durationcanvas_ref"
				     : "durationcanvas"));
}

SdDurationCanvas * SdDurationCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "durationcanvas_ref"))
    return ((SdDurationCanvas *) dict_get(read_id(st), "durationcanvas", canvas));
  else if (!strcmp(k, "durationcanvas")) {
    int id = read_id(st);
    SdObjCanvas * o;
    
    k = read_keyword(st);
    
    if ((o = SdClassInstCanvas::read(st, canvas, k)) == 0)
      wrong_keyword(st, k);
    
    UmlColor color = UmlDefaultColor;
    
    k = read_keyword(st);
    
    read_color(st, "color", color, k);
    
    if (strcmp(k, "xyzwh"))
      wrong_keyword(k, "xyzwh");
    
    int x = (int) read_double(st);
    int y = (int) read_double(st);
    double z = read_double(st);
    int w = (int) read_double(st);
    SdDurationCanvas * result =
      new SdDurationCanvas(canvas, o->get_life_line(),
			   x, y, w, (int) read_double(st), id);
    
    result->itscolor = color;
    result->setZ(z);
    result->update_hpos();	// in case the current font is not the original one
    result->set_center100();
    result->show();
    
    return result;
  }
  else
    return 0;
}

void SdDurationCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

void SdDurationCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
  ::save(line, b);
  ::save((int) msgs.count(), b);
  
  QListIterator<SdMsgBaseCanvas> it(msgs);
  
  for (; it.current(); ++it)
    ::save(it.current(), b);
}

void SdDurationCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
  
  line = (SdLifeLineCanvas *) ::load_item(b);
  
  int n;
  
  ::load(n, b);
  msgs.clear();
  
  while (n--)
    msgs.append((SdMsgBaseCanvas *) ::load_item(b));
  
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));  
}
