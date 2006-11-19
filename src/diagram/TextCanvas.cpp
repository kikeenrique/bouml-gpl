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

#include "TextCanvas.h"
#include "MLEDialog.h"
#include "UmlCanvas.h"
#include "myio.h"
#include "MenuTitle.h"
#include "BrowserDiagram.h"
#include "strutil.h"
#include "DialogUtil.h"

TextCanvas::TextCanvas(UmlCanvas * canvas, int x, int y, int id)
    : DiagramCanvas(0, canvas, x, y, TEXT_CANVAS_MIN_SIZE,
		    TEXT_CANVAS_MIN_SIZE, id),
      itsfont(UmlNormalFont) {
  browser_node = canvas->browser_diagram();
}

TextCanvas::~TextCanvas() {
}

void TextCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  p.setBackgroundMode(QObject::TransparentMode);
  
  QRect r = rect();
  
  p.setFont(the_canvas()->get_font(itsfont));
  p.drawText (r.left(), r.top(), r.width(), r.height(), 
	      QObject::AlignLeft + QObject::AlignTop + QObject::WordBreak, text);
  p.setFont(the_canvas()->get_font(UmlNormalFont));
    
  if (selected())
    show_mark(p, r);
}

UmlCode TextCanvas::type() const {
  return UmlText;
}

void TextCanvas::open() {
  // warning : 'note' is already unicode
  QString s = fromUnicode(text);
  bool created = s.isEmpty();
  
  MLEDialog::get(s, QCursor::pos());
  
  if (s.isEmpty())
    delete_it();
  else {
    text = toUnicode(s);
    
    if (created) {  
      QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
      QSize sz = fm.size(0, text);
      
      DiagramCanvas::resize(sz.width(), sz.height());
    }
  
    // force son reaffichage
    hide();
    show();
  }
  canvas()->update();
  
  package_modified();
}

void TextCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

void TextCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu fontsubm(0);
  
  m.insertItem(new MenuTitle("Text", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  m.insertSeparator();
  m.insertItem("Font", &fontsubm);  
  init_font_menu(fontsubm, the_canvas(), 10);
  if (linked()) {
    m.insertSeparator();
    m.insertItem("Select linked items", 3);
  }
  m.insertSeparator();
  m.insertItem("Remove from view",4);

  int index = m.exec(QCursor::pos());
  
  switch (index) {
  case 0:
    upper();
    // force son reaffichage
    hide();
    show();
    canvas()->update();
    break;
  case 1:
    lower();
    // force son reaffichage
    hide();
    show();
    canvas()->update();
    break;
  case 2:
    open();
    return;	// call package_modified
  case 3:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 4:
    // delete
    the_canvas()->del(this);
    break;
  default:
    if (index >= 10) {
      itsfont = (UmlFont) (index - 10);
      // force son reaffichage
      hide();
      show();
      canvas()->update();
      break;
    }
    return;
  }
  
  package_modified();
}

const char * TextCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : "illegal";
}

const char * TextCanvas::may_connect(UmlCode & l, const DiagramItem * o) const {
  switch (o->type()) {
  case UmlNote:
  case UmlIcon:
    return (l == UmlAnchor) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

bool TextCanvas::alignable() const {
  return TRUE;
}

bool TextCanvas::copyable() const {
  return selected();
}

aCorner TextCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void TextCanvas::resize(aCorner c, int dx, int dy) {
  int min = (int) (TEXT_CANVAS_MIN_SIZE * the_canvas()->zoom());
  
  DiagramCanvas::resize(c, dx, dy, min, min);
}

void TextCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref)
    st << "textcanvas_ref " << get_ident();
  else {
    nl_indent(st);
    st << "textcanvas " << get_ident() << ' ';
    save_string(fromUnicode(text), st);
    nl_indent(st);
    if (itsfont != UmlNormalFont)
      st << "  font " << stringify(itsfont);
    save_xyzwh(st, this, "  xyzwh");
  }
}

TextCanvas * TextCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "textcanvas_ref"))
    return ((TextCanvas *) dict_get(read_id(st), "textcanvas", canvas));
  else if (!strcmp(k, "textcanvas")) {
    int id = read_id(st);
    QString text = toUnicode(read_string(st));
    UmlFont font;

    k = read_keyword(st);
    read_font(st, "font", font, k);
    
    if (strcmp(k, "xyzwh"))
      wrong_keyword(k, "xyzwh");
    
    int x = (int) read_double(st);
    
    TextCanvas * result =
      new TextCanvas(canvas, x, (int) read_double(st), id);
    
    read_zwh(st, result);
    result->width_scale100 = result->width();
    result->height_scale100 = result->height();
    result->text = text;
    result->itsfont = font;
    result->show();
    
    return result;
  }
  else
    return 0;
}

void TextCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void TextCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
}
