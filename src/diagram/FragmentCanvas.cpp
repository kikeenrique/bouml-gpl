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





#include <qcursor.h>
#include <qpainter.h>
#include <qpopupmenu.h> 

#include "FragmentCanvas.h"
#include "FragmentDialog.h"
#include "FragmentSeparatorCanvas.h"
#include "UmlGlobal.h"
#include "UmlCanvas.h"
#include "BrowserDiagram.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "DialogUtil.h"
#include "ToolCom.h"
#include "strutil.h"

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
  while (! separators.isEmpty())
    separators.first()->delete_it();	// call remove_it
  
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  DiagramCanvas::delete_it();
}

void FragmentCanvas::remove_it(FragmentSeparatorCanvas * sp) {
  separators.remove(sp);
}

void FragmentCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();
  QColor bckgrnd = p.backgroundColor();

  p.setBackgroundMode((used_color == UmlTransparent) ? ::Qt::TransparentMode : ::Qt::OpaqueMode);

  QColor co = color(used_color);
  FILE * fp = svg();

  if (fp != 0)
    fputs("<g>\n", fp);
  
  p.setBackgroundColor(co);
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  
  if (used_color != UmlTransparent)
    p.fillRect(r, co);

  if (fp != 0)
    fprintf(fp, "\t<rect fill=\"%s\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\""
	    " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
	    svg_color(used_color), 
	    r.x(), r.y(), r.width() - 1, r.height() - 1);
  
  p.drawRect(r);
  
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  int w = fm.width((name.isEmpty()) ? QString("X") : name);
  int h = fm.height() / 2;
  
  r.setWidth(w + h);
  r.setHeight(fm.height() + h);
  if (!name.isEmpty())
    p.drawText(r, ::Qt::AlignCenter, name);
  if (fp != 0)
    draw_text(r, ::Qt::AlignCenter, name,
	      p.font(), fp);
  
  h = (fm.height() + h)/2;
  p.drawLine(r.left(), r.bottom(), r.right(), r.bottom());
  p.drawLine(r.right(), r.bottom(), r.right() + h, r.bottom() - h);
  p.drawLine(r.right() + h, r.bottom() - h, r.right() + h, r.top());

  if (fp != 0) {
    fprintf(fp, "\t<line stroke=\"black\" stroke-opacity=\"1\""
	    " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
	    r.left(), r.bottom(), r.right(), r.bottom());
    fprintf(fp, "\t<line stroke=\"black\" stroke-opacity=\"1\""
	    " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
	    r.right(), r.bottom(), r.right() + h, r.bottom() - h);
    fprintf(fp, "\t<line stroke=\"black\" stroke-opacity=\"1\""
	    " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
	    r.right() + h, r.bottom() - h, r.right() + h, r.top());
    fputs("</g>\n", fp);
  }
	       
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

void FragmentCanvas::moveBy(double dx, double dy) {
  DiagramCanvas::moveBy(dx, dy);
  
  QListIterator<FragmentSeparatorCanvas> it(separators);
  
  for (; it.current(); ++it)
    it.current()->update();
}

void FragmentCanvas::set_z(double z) {
  setZ(z);
  
  QListIterator<FragmentSeparatorCanvas> it(separators);
  
  for (; it.current(); ++it)
    it.current()->update();
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
  
  QListIterator<FragmentSeparatorCanvas> it(separators);
  
  for (; it.current(); ++it)
    it.current()->update();
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
  m.insertItem("Add separator", 6);
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
    edit_drawing_settings();
    return;
  case 4:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 5:
    delete_it();
    break;
  case 6:
    {
      FragmentSeparatorCanvas * sp =
	new FragmentSeparatorCanvas(the_canvas(), this);
      
      separators.append(sp);
      the_canvas()->unselect_all();
      the_canvas()->select(sp);
    }
    break;
  default:
    return;
  }
  
  package_modified();
}

void FragmentCanvas::apply_shortcut(QString s) {
  if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Edit drawing settings") {
    edit_drawing_settings();
    return;
  }
  else if (s == "Edit") {
    open();  // call modified then package_modified
    return;
  }
  else 
    return;

  modified();
  package_modified();
}

void FragmentCanvas::edit_drawing_settings() {
  QArray<ColorSpec> co(1);
  
  co[0].set("fragment color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();
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
  return (l == UmlAnchor) ? dest->may_start(l) : "illegal";
}

aCorner FragmentCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void FragmentCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width, min_height);
  
  QListIterator<FragmentSeparatorCanvas> it(separators);
  
  for (; it.current(); ++it)
    it.current()->update();
}

void FragmentCanvas::prepare_for_move(bool on_resize) {
  if (!on_resize) {
    DiagramCanvas::prepare_for_move(on_resize);
    
    QRect r = rect();
    QCanvasItemList l = collisions(TRUE);
    QCanvasItemList::ConstIterator it;
    QCanvasItemList::ConstIterator end = l.end();
    DiagramItem * di;
  
    for (it = l.begin(); it != end; ++it) {
      if ((*it)->visible() && // at least not deleted
	  !(*it)->selected() &&
	  ((di = QCanvasItemToDiagramItem(*it)) != 0) &&
	  r.contains(di->rect(), TRUE) &&
	  di->move_with(UmlFragment)) {
	the_canvas()->select(*it);
	di->prepare_for_move(FALSE);
      }
    }
  }
}

void FragmentCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "fragment_ref " << get_ident();
  }
  else {
    nl_indent(st);
    
    st << "fragment " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    nl_indent(st);
    if (itscolor != UmlDefaultColor)
      st << "color " << stringify(itscolor) << ' ';
    save_xyzwh(st, this, "xyzwh");
    
    QListIterator<FragmentSeparatorCanvas> it(separators);
    
    for (; it.current(); ++it)
      it.current()->save(st, FALSE, warning);
    
    indent(-1);
    nl_indent(st);
    st << "end";
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
    
    if (read_file_format() >= 28) {
      while (strcmp(k = read_keyword(st), "end"))
	result->separators.append(FragmentSeparatorCanvas::read(st, canvas, k, result));
    }
    
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

  ::save((int) separators.count(), b);
  
  QListIterator<FragmentSeparatorCanvas> it(separators);
  
  for (; it.current(); ++it)
    ::save(it.current(), b);
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
  
  int l;
  
  ::load(l, b);
  
  separators.clear();
  while (l--)
    separators.append((FragmentSeparatorCanvas *) ::load_item(b));
  
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

void FragmentCanvas::history_hide() {
  DiagramCanvas::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

// for plug outs

void FragmentCanvas::send(ToolCom * com, QCanvasItemList & all)
{
  QList<FragmentCanvas> fragments;
  QCanvasItemList::Iterator cit;

  for (cit = all.begin(); cit != all.end(); ++cit) {
    DiagramItem *di = QCanvasItemToDiagramItem(*cit);
    
    if ((di != 0) && (*cit)->visible() && (di->type() == UmlFragment))
      fragments.append((FragmentCanvas *) di);
  }
  
  com->write_unsigned(fragments.count());
  
  FragmentCanvas * f;
  
  for (f = fragments.first(); f != 0; f = fragments.next()) {
    QCString s = fromUnicode(f->name);
    
    com->write_string((const char *) s);
    com->write(f->rect());
    
    // few separators, use bubble sort
    unsigned sz = f->separators.count();
    
    if (sz == 0)
      com->write_unsigned(1);
    else {
      FragmentSeparatorCanvas ** v = new FragmentSeparatorCanvas *[sz];
      unsigned index;      
      QListIterator<FragmentSeparatorCanvas> it(f->separators);
      
      for (index = 0; it.current(); ++it, index += 1)
	v[index] = it.current();
      
      bool modified;
      
      do {
	modified = FALSE;
	
	for (index = 1; index < sz; index += 1) {
	  if (v[index - 1]->y() > v[index]->y()) {
	    modified = TRUE;
	    
	    FragmentSeparatorCanvas * fs = v[index - 1];
	    
	    v[index - 1] = v[index];
	    v[index] = fs;
	  }
	}
      } while (modified);
      
      com->write_unsigned(sz + 1);
      
      for (index = 0; index != sz; index += 1)
	com->write_unsigned((unsigned) v[index]->y());
      
      delete [] v;
    }
      
    com->write_unsigned((unsigned) (f->y() + f->height() - 1));
  }
}
