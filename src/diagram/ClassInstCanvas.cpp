// *************************************************************************
//
// Copyleft 2004-2008 Bruno PAGES  .
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





#include <qpainter.h>

#include "ClassInstCanvas.h"
#include "BrowserClass.h"
#include "BrowserClassInstance.h"
#include "UmlCanvas.h"
#include "ClassData.h"
#include "ClassInstanceData.h"
#include "DiagramCanvas.h"
#include "UmlGlobal.h"
#include "BrowserDiagram.h"
#include "myio.h"

ClassInstCanvas::ClassInstCanvas() {
  itscolor = UmlDefaultColor;
  write_horizontally = UmlDefaultState;
}

ClassInstCanvas::~ClassInstCanvas() {
}

QString ClassInstCanvas::full_name() const {
  BrowserNode * cl = get_type();
  QString iname = get_name();
  
  if (iname.isEmpty())
    return cl->get_name();
  else
    return iname + ":" + cl->get_name();
}

BrowserNode * ClassInstCanvas::new_type() {
  BrowserNode * bn = container(UmlClass);
  
  return (bn->is_writable()) ? BrowserClass::add_class(FALSE, bn) : 0;
}

bool ClassInstCanvas::new_type_available() {
  return container(UmlClass)->is_writable();
}

void ClassInstCanvas::compute_size(int & w, int & h, UmlCanvas * canvas) {
  BrowserNode * cl = get_type();
  QString iname = get_name();
  
  used_color = (itscolor == UmlDefaultColor)
    ? canvas->browser_diagram()->get_color(UmlClass)
    : itscolor;
    
  QFontMetrics fm(canvas->get_font(UmlNormalUnderlinedFont));
  double zoom = canvas->zoom();
  int minw = (int) (zoom * CLASSINST_CANVAS_MIN_SIZE);
  int delta = (int) (8 * zoom);

  h = fm.height() + delta;
  horiz = TRUE;
  
  if (!iname.isEmpty()) {
    switch (write_horizontally) {
    case UmlYes:
      horiz = TRUE;
      break;
    case UmlNo:
      horiz = FALSE;
      break;
    default:
      // right get_classinstwritehorizontally arg set by the diagram itself
      horiz = canvas->browser_diagram()->get_classinstwritehorizontally(UmlCodeSup);
    }
  }
  
  if (used_color != UmlTransparent) {
    const int shadow = canvas->shadow();
    
    delta += shadow;
    h += shadow;
  }
  
  if (horiz)
    w = fm.width(full_name());
  else {
    h += fm.height();
    
    int wi = fm.width(iname + ":");
    
    w = fm.width(cl->get_name());
    if (wi > w)
      w = wi;
  }
  
  w += delta;
  if (w < minw) 
    w = minw;
}

void ClassInstCanvas::draw(QPainter & p, UmlCanvas * canvas, QRect r) {
  BrowserNode * cl = get_type();
  QString iname = get_name();
  
  QColor bckgrnd = p.backgroundColor();

  p.setBackgroundMode((used_color == UmlTransparent)
		      ? ::Qt::TransparentMode :
		      ::Qt::OpaqueMode);

  FILE * fp = svg();

  if (fp != 0)
    fputs("<g>\n", fp);
  
  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  
  if (used_color != UmlTransparent) {
    const int shadow = canvas->shadow();
    
    if (shadow != 0) {
      r.setRight(r.right() - shadow);
      r.setBottom(r.bottom() - shadow);
      
      p.fillRect (r.right(), r.top() + shadow,
		  shadow, r.height() - 1,
		  ::Qt::darkGray);
      p.fillRect (r.left() + shadow, r.bottom(),
		  r.width() - 1, shadow,
		  ::Qt::darkGray);
      
      p.fillRect(r, co);
      
      if (fp != 0) {
	fprintf(fp, "\t<rect fill=\"#%06x\" stroke=\"none\" stroke-opacity=\"1\""
		" x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
		::Qt::darkGray.rgb()&0xffffff,
		r.right(), r.top() + shadow, shadow - 1, r.height() - 1 - 1);
	
	fprintf(fp, "\t<rect fill=\"#%06x\" stroke=\"none\" stroke-opacity=\"1\""
		" x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
		::Qt::darkGray.rgb()&0xffffff,
		r.left() + shadow, r.bottom(), r.width() - 1 - 1, shadow - 1);
	
	fprintf(fp, "\t<rect fill=\"%s\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\""
		" x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
		svg_color(used_color), 
		r.x(), r.y(), r.width() - 1, r.height() - 1);
      }
    }
  }
  else if (fp != 0)
    fprintf(fp, "\t<rect fill=\"none\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\""
	    " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
	    r.x(), r.y(), r.width() - 1, r.height() - 1);
  
  p.drawRect(r);
  
  p.setBackgroundMode(::Qt::TransparentMode);
  p.setFont(canvas->get_font(UmlNormalUnderlinedFont));
  if (horiz) {
    p.drawText(r, ::Qt::AlignCenter, full_name());
    if (fp != 0)
      draw_text(r, ::Qt::AlignCenter, full_name(),
		p.font(), fp);
  }
  else {
    QRect r1 = r;
    
    r1.setHeight(r.height()/2);
    p.drawText(r1, ::Qt::AlignCenter, iname + ":");
    if (fp != 0)
      draw_text(r1, ::Qt::AlignCenter, iname + ":",
		p.font(), fp);
    r1.moveBy(0, r.height()/2);
    p.drawText(r1, ::Qt::AlignCenter, cl->get_name());
    if (fp != 0)
      draw_text(r1, ::Qt::AlignCenter, cl->get_name(),
		p.font(), fp);
  }
  p.setFont(canvas->get_font(UmlNormalFont));
  p.setBackgroundColor(bckgrnd);

  if (fp != 0)
    fputs("</g>\n", fp);
}

void ClassInstCanvas::save(QTextStream & st) const {
  if (itscolor != UmlDefaultColor)
    st << " color " << stringify(itscolor);
  if (write_horizontally != UmlDefaultState)
    st << " write_horizontally " << stringify(write_horizontally);
}

void ClassInstCanvas::read(char *& st, char *& k) {
  itscolor = UmlDefaultColor;
  
  k = read_keyword(st);
  read_color(st, "color", itscolor, k);	// updates k
  
  if (!strcmp(k, "write_horizontally")) {
    write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  else
    write_horizontally = UmlDefaultState;
}

