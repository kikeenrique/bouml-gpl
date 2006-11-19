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

#include "NoteCanvas.h"
#include "MLEDialog.h"
#include "UmlGlobal.h"
#include "UmlCanvas.h"
#include "BrowserDiagram.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "DialogUtil.h"

NoteCanvas::NoteCanvas(UmlCanvas * canvas, int x, int y, int id)
    : DiagramCanvas(0, canvas, x, y, NOTE_CANVAS_MIN_SIZE,
		    NOTE_CANVAS_MIN_SIZE, id),
      itsfont(UmlNormalFont), itscolor(UmlDefaultColor),
      used_color(UmlDefaultColor) {
  browser_node = canvas->browser_diagram();
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

NoteCanvas::~NoteCanvas() {
}

void NoteCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  DiagramCanvas::delete_it();
}


void NoteCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  QBrush brsh = p.brush();
  QColor bckgrnd = p.backgroundColor();
  QPointArray a(7);
  UmlColor c;
  
  if (itscolor != UmlDefaultColor)
    c = itscolor;
  else {
    if (used_color == UmlDefaultColor)
      used_color = the_canvas()->browser_diagram()->get_color(UmlNote);
    c = used_color;
  }
  
  QColor co = color(c);
  
  const int corner_size = (int) (NOTE_MARGIN * the_canvas()->zoom());
  
  a.setPoint(0, r.left(), r.top());
  a.setPoint(1, r.right() - corner_size, r.top());
  a.setPoint(2, r.right() - corner_size, r.top() + corner_size);
  a.setPoint(3, r.right(), r.top() + corner_size);
  a.setPoint(4, r.right(), r.bottom());
  a.setPoint(5, r.left(), r.bottom());
  a.setPoint(6, r.left(), r.top());
  
  if (c == UmlTransparent) {
    p.setBackgroundMode(QObject::TransparentMode);
    p.setBackgroundColor(co);
    p.drawPolyline(a);
  }
  else {
    p.setBackgroundMode(QObject::OpaqueMode);
    p.setBrush(co);
    p.drawPolygon(a, TRUE, 0, 6);
    p.setBrush(brsh);
    p.setBackgroundColor(co);
  }
  
  p.moveTo(r.right() - corner_size, r.top());
  p.lineTo(r.right(), r.top() + corner_size);
  
  p.setFont(the_canvas()->get_font(itsfont));
  p.drawText (r.left() + corner_size, r.top() + corner_size,
	      r.width() - 2*corner_size, r.height() - 2*corner_size, 
	      QObject::AlignLeft + QObject::AlignTop + QObject::WordBreak,
	      note);
  p.setFont(the_canvas()->get_font(UmlNormalFont));
    
  p.setBackgroundColor(bckgrnd);
  
  if (selected())
    show_mark(p, r);
}

UmlCode NoteCanvas::type() const {
  return UmlNote;
}

void NoteCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

bool NoteCanvas::alignable() const {
  return TRUE;
}

bool NoteCanvas::copyable() const {
  return selected();
}

void NoteCanvas::open() {
  // warning : 'note' is already unicode
  QString s = fromUnicode(note);
  
  MLEDialog::get(s, QCursor::pos());
  note = toUnicode(s);
  
  modified();
}

void NoteCanvas::modified() {
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlNote)
    : itscolor;
  // force son reaffichage
  hide();
  show();
  canvas()->update();
  package_modified();
}

void NoteCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu fontsubm(0);
  
  m.insertItem(new MenuTitle("Note", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  m.insertSeparator();
  m.insertItem("Font", &fontsubm);  
  init_font_menu(fontsubm, the_canvas(), 10);
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
      
      co[0].set("note color", &itscolor);

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
    if (index >= 10) {
      itsfont = (UmlFont) (index - 10);
      modified();
    }
    return;
  }
  
  package_modified();
}

bool NoteCanvas::has_drawing_settings() const {
  return TRUE;
}

void NoteCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  
  co[0].set("note color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if ((dialog.exec() == QDialog::Accepted) && (co[0].name != 0)) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      ((NoteCanvas *) it.current())->itscolor = itscolor;
      ((NoteCanvas *) it.current())->modified();	// call package_modified()
    }    
  }
}

const char * NoteCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : "illegal";
}

const char * NoteCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
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

aCorner NoteCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void NoteCanvas::resize(aCorner c, int dx, int dy) {
  int min = (int) (NOTE_CANVAS_MIN_SIZE * the_canvas()->zoom());
  
  DiagramCanvas::resize(c, dx, dy, min, min);
}

void NoteCanvas::save_internal(QTextStream & st) const {
  save_string(fromUnicode(note), st);
  nl_indent(st);
  if (itscolor != UmlDefaultColor)
    st << "  color " << stringify(itscolor);
  if (itsfont != UmlNormalFont)
    st << "  font " << stringify(itsfont);
  save_xyzwh(st, this, "  xyzwh");
}

void NoteCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref) {
    st << "note_ref " << get_ident();
  }
  else {
    nl_indent(st);
    
    st << "note " << get_ident() << ' ';
    save_internal(st);
  }
}

void NoteCanvas::read_internal(char * & st) {
  note = toUnicode(read_string(st));
  
  char * k = read_keyword(st);
  read_color(st, "color", itscolor, k);
  read_font(st, "font", itsfont, k);
  
  if (strcmp(k, "xyzwh"))
    wrong_keyword(k, "xyzwh");
  
  read_xyzwh(st, this);
  width_scale100 = width();
  height_scale100 = height();
  set_center100();
  show();
}

NoteCanvas * NoteCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "note_ref"))
    return (NoteCanvas *) dict_get(read_id(st), "note", canvas);
  else if (!strcmp(k, "note")) {
    int id = read_id(st);
    NoteCanvas * result = new NoteCanvas(canvas, 0, 0, id);
    
    result->read_internal(st);
    return result;
  }
  else
    return 0;
}

void NoteCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

void NoteCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void NoteCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
  
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}
