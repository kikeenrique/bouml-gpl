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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qpainter.h>
#include <qcursor.h>
#include <qpopupmenu.h> 

#include "SdLifeLineCanvas.h"
#include "SdObjCanvas.h"
#include "SdDurationCanvas.h"
#include "MenuTitle.h"
#include "ToolCom.h"
#include "myio.h"

#define LIFE_LINE_WIDTH 7
#define LIFE_LINE_HEIGHT 10000

SdLifeLineCanvas::SdLifeLineCanvas(UmlCanvas * canvas, SdObjCanvas * o)
    : DiagramCanvas(0, canvas, 0, 0, LIFE_LINE_WIDTH, LIFE_LINE_HEIGHT, -1),
      obj(o), end(LIFE_LINE_HEIGHT) {
  update_pos();
  set_center100();
  setZ(DIAGRAMCANVAS_Z);
  
  if (obj->is_mortal())
    update_instance_dead();	// call show
  else
    show();
  
  if (canvas->paste())
    // must not be selected else the move done after if wrong
    canvas->unselect(this);
}

SdLifeLineCanvas::~SdLifeLineCanvas() {
}

void SdLifeLineCanvas::delete_it() {
  while (!durations.isEmpty())
    durations.first()->delete_it();	// will remove the msgs
    
  DiagramCanvas::delete_it();
}

void SdLifeLineCanvas::update_pos() {
  QRect r = obj->rect();
  
  move((r.left() + r.right() - width())/2,
       r.top() + LIFE_LINE_TOPOFFSET * the_canvas()->zoom() + 100000);
}

bool SdLifeLineCanvas::is_decenter(const QPoint &, bool &) const {
  return FALSE;
}

void SdLifeLineCanvas::change_scale() {
  double scale = the_canvas()->zoom();
  int w = (int) (width_scale100*scale);
  // force odd width
  setSize(((w & 1) == 0) ? w + 1 : w, LIFE_LINE_HEIGHT);
  // don't move, the position is updated with its obj
}

void SdLifeLineCanvas::drawShape(QPainter & p) {
  p.setBackgroundMode(::Qt::OpaqueMode);
  
  p.setPen(::Qt::DashLine);
  
  int m = (int) (x()+width()/2);
  FILE * fp = svg();

  p.drawLine(m, (int) y(), m, end);

  p.setPen(::Qt::SolidLine);

  if (end != LIFE_LINE_HEIGHT) {
    int b = end + (int) width();
    
    p.drawLine((int) x(), end,
	       (int) (x() + width()) - 1, b);
    p.drawLine((int) (x() + width() - 1), end,
	       (int) x(), b);

    if (fp != 0) {
      fprintf(fp, "<g>\n"
	      "\t<line stroke=\"black\" stroke-dasharray=\"18,6\"  stroke-opacity=\"1\""
	      " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
	      "</g>\n",
	      m, (int) y(), m, end);

      fprintf(fp, "<g>\n"
	      "\t<line stroke=\"black\" stroke-opacity=\"1\""
	      " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
	      (int) x(), end, (int) (x() + width()) - 1, b);
      fprintf(fp, "\t<line stroke=\"black\" stroke-opacity=\"1\""
	      " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
	      "</g>\n",
	      (int) (x() + width() - 1), end, (int) x(), b);
    }
  }
  else if (fp != 0)
    fprintf(fp, "<g>\n"
	    "\t<line stroke=\"black\" stroke-dasharray=\"18,6\"  stroke-opacity=\"1\""
	    " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
	    "</g>\n",
	    m, (int) y(), m, svg_height());
}

void SdLifeLineCanvas::moveBy(double dx, double dy) {
  DiagramCanvas::moveBy(dx, (dy > 80000) ? dy - 100000 : /*dy*/0);
  
  if ((dy < 80000) && !obj->selected())
    // not called by update_pos() which is only called by obj->moveBy()
    obj->moveBy(dx, 0);
  
  QListIterator<SdDurationCanvas> it(durations);
  
  for ( ; it.current(); ++it )
    it.current()->update_hpos();
}

UmlCode SdLifeLineCanvas::type() const {
  return UmlLifeLine;
}

double SdLifeLineCanvas::instance_max_y() const {
  QListIterator<SdDurationCanvas> it(durations);
  double miny = 100000;
    
  for ( ; it.current(); ++it )
    if (it.current()->y() < miny)
      miny = it.current()->y();
  
  return miny - LIFE_LINE_TOPOFFSET * the_canvas()->zoom() - 5;
}

DiagramItem::LineDirection SdLifeLineCanvas::allowed_direction(UmlCode) {
  return DiagramItem::Vertical;
}

const char * SdLifeLineCanvas::may_start(UmlCode & l) const {
  return (l != UmlAnchor) ? 0 : "illegal";
}

const char * SdLifeLineCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlActivityDuration:
  case UmlLifeLine:
    return (l != UmlAnchor) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

void SdLifeLineCanvas::connexion(UmlCode l, DiagramItem * dest,
				 const QPoint & s, const QPoint & e) {
  switch (l) {
  default:
    if (dest == this)
      break;
    // no break
  case UmlSyncSelfMsg:
  case UmlAsyncSelfMsg:
  case UmlSelfReturnMsg:
    (new SdDurationCanvas(the_canvas(), this, s.y(), FALSE))->connexion(l, dest, s, e);
  }
}

void SdLifeLineCanvas::add(SdDurationCanvas * d) {
  durations.append(d);
  update_instance_dead();
}

void SdLifeLineCanvas::remove(SdDurationCanvas * d) {
  durations.remove(d);
  update_instance_dead();
}

void SdLifeLineCanvas::toFlat() {
  QListIterator<SdDurationCanvas> it(durations);

  for (; it.current(); ++it)
    it.current()->toFlat();
}

void SdLifeLineCanvas::toOverlapping() {
  QListIterator<SdDurationCanvas> it(durations);

  for (; it.current(); ++it)
    it.current()->toOverlapping();
}

void SdLifeLineCanvas::update_instance_dead() {
  if (obj->is_mortal() && !durations.isEmpty()) {
    QListIterator<SdDurationCanvas> it(durations);
    int new_end = 0;
    
    for (; it.current(); ++it) {
      int b = (int) (it.current()->y() + it.current()->height());
      
      if (b > new_end)
	new_end = b;
    }
  
    if (new_end != end) {
      end = new_end;
      hide();
      show();
    }
  }
  else if (end != LIFE_LINE_HEIGHT) {
    end = LIFE_LINE_HEIGHT;
    hide();
    show();
  }
}

void SdLifeLineCanvas::update_v_to_contain(SdDurationCanvas *, bool) {
  update_instance_dead();
}

int SdLifeLineCanvas::sub_x(int sub_w) const {
  QRect r = rect();
  
  return (r.left() + r.right() - sub_w)/2 + 1;
}

double SdLifeLineCanvas::min_y() const {
  return y() + 6 * the_canvas()->zoom();
}

SdLifeLineCanvas * SdLifeLineCanvas::get_line() const {
  return (SdLifeLineCanvas *) this;
}

bool SdLifeLineCanvas::isaDuration() const {
  return false;
}

double SdLifeLineCanvas::getZ() const {
  return z();
}

void SdLifeLineCanvas::open() {
}

void SdLifeLineCanvas::menu(const QPoint&) {
  // delete must call SdObjCanvas->delete_it() NOT the own delete_it !
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Message", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);

  int index = m.exec(QCursor::pos());
  
  switch (index) {
  case 0:
    upper();
    break;
  case 1:
    lower();
    break;
  default:
    return;
  }
  
  // force son reaffichage
  hide();
  show();
  canvas()->update();
  package_modified();
}

bool SdLifeLineCanvas::copyable() const {
  QListIterator<SdDurationCanvas> it(durations);
    
  for ( ; it.current(); ++it )
    if (it.current()->copyable())
      return TRUE;
  
  return FALSE;
}

void SdLifeLineCanvas::save(QTextStream & st, bool, QString &) const {
  st << "\nERROR SdLifeLineCanvas::save must not be called\n";
}

void SdLifeLineCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(end, b);
  ::save((int) durations.count(), b);
  
  QListIterator<SdDurationCanvas> it(durations);
  
  for (; it.current(); ++it)
    ::save(it.current(), b);
}

void SdLifeLineCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(end, b);
  
  int n;
  
  ::load(n, b);
  durations.clear();
  
  while (n--)
    durations.append((SdDurationCanvas *) ::load_item(b));
}

void SdLifeLineCanvas::send(ToolCom * com, const QCanvasItemList & l)
{
  QCanvasItemList::ConstIterator cit;
  unsigned n = 0;
  
  for (cit = l.begin(); cit != l.end(); ++cit) {
    DiagramItem * it = QCanvasItemToDiagramItem(*cit);
      
    if ((it != 0) && // an uml canvas item
	(*cit)->visible() &&
	(it->type() == UmlLifeLine)) {
      QListIterator<SdDurationCanvas> iter(((SdLifeLineCanvas *) it)->durations);
      
      for (; iter.current(); ++iter)
	n += iter.current()->count_msg();
    }
  }
  
  com->write_unsigned(n);
  
  for (cit = l.begin(); cit != l.end(); ++cit) {
    DiagramItem * it = QCanvasItemToDiagramItem(*cit);
      
    if ((it != 0) && // an uml canvas item
	(*cit)->visible() &&
	(it->type() == UmlLifeLine)) {
      int id = ((SdLifeLineCanvas *) it)->obj->get_ident();
      QListIterator<SdDurationCanvas> iter(((SdLifeLineCanvas *) it)->durations);
      
      for (; iter.current(); ++iter)
	iter.current()->send(com, id);
    }
  }
}
