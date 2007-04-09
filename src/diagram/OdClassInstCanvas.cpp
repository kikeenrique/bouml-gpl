// *************************************************************************
//
// Copyright (C) 2004-2007 Bruno PAGES  All rights reserved.
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

#include "OdClassInstCanvas.h"
#include "ObjectLinkCanvas.h"
#include "BrowserClass.h"
#include "BrowserAttribute.h"
#include "AttributeData.h"
#include "BrowserDiagram.h"
#include "UmlCanvas.h"
#include "ClassData.h"
#include "ClassInstanceDialog.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "ObjectDiagramView.h"
#include "Settings.h"
#include "UmlGlobal.h"
#include "strutil.h"

OdClassInstCanvas::OdClassInstCanvas(BrowserClass * t, UmlCanvas * canvas,
				     int x, int y, int id)
    : DiagramCanvas(0, canvas, x, y, CLASSINST_CANVAS_MIN_SIZE, 1, id),
      ClassInstCanvas(t) {
  browser_node = canvas->browser_diagram();
  itscolor = UmlDefaultColor;
  compute_size();	// update used_settings
  set_center100();
  
  connect(cl->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(cl->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

OdClassInstCanvas::~OdClassInstCanvas() {
  disconnect(cl->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect_attributes();
}

void OdClassInstCanvas::disconnect_attributes() {
  AttributeData * d;
  
  for (d = attributes.first(); d!= 0; d = attributes.next())
    disconnect(d, 0, this, 0);
}

void OdClassInstCanvas::delete_it() {
  disconnect(cl->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  disconnect_attributes();
  DiagramCanvas::delete_it();
}

void OdClassInstCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void OdClassInstCanvas::compute_size() {
  UmlCanvas * canvas = the_canvas();
  
  used_color = (itscolor == UmlDefaultColor)
    ? canvas->browser_diagram()->get_color(UmlClass)
    : itscolor;
  
  QFontMetrics fm(canvas->get_font(UmlNormalUnderlinedFont));
  double zoom = canvas->zoom();
  const int two = (int) (2 * zoom);
  int wi, he;

  horiz = TRUE;
  
  if (!get_name().isEmpty()) {
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
      
  he = fm.height() + two + two;
  
  if (horiz)
    wi = fm.width(full_name());
  else {
    he += fm.height();
    
    int w = fm.width(get_name() + ":");
    
    wi = fm.width(cl->get_name());
    if (w > wi)
      wi = w;
  }
  
  if (! attributes.isEmpty()) {
    QListIterator<AttributeData> it_at(attributes);
    QStringList::Iterator it_val = values.begin();
    QString egal = " = ";

    do {
      QString s =
	it_at.current()->get_browser_node()->get_name() + egal + *it_val;      
      int w = fm.width(s);
      
      if (w > wi)
	wi = w;
      
      ++it_at;
      ++it_val;
    } while (it_val != values.end());
    
    he += (fm.height() + two) * values.count() + two + two;
  }
  
  if (used_color != UmlTransparent) {
    const int shadow = canvas->shadow();
	
    wi += shadow;
    he += shadow;
  }
  
  wi += (int) (8 * zoom);
  
  int minw = (int) (zoom * CLASSINST_CANVAS_MIN_SIZE);
  
  if (wi < minw) 
    wi = minw;
  
  // force odd width and height for line alignment
  setSize(wi | 1, he | 1);
}

void OdClassInstCanvas::modified() {
  if (visible()) {
    hide();
    hide_lines();
    check_attributes();
    compute_size();
    show();
    update_show_lines();
    
    QListIterator<ArrowCanvas> it(lines);
    
    for (; it.current(); ++it)
      if (IsaRelation(it.current()->type()))
	// useless to check UmlObjectLink ie unset link
	((ObjectLinkCanvas *) it.current())->check();
    
    canvas()->update();
    package_modified();
  }
}

void OdClassInstCanvas::check_attributes() {
  disconnect_attributes();

  if (! attributes.isEmpty()) {
    QList<BrowserClass> l;
    
    cl->get_all_parents(l);
    l.append(cl);
    
    AttributeData * d = attributes.first();
    QStringList::Iterator it_val = values.begin();

    do {
      BrowserAttribute * att = (BrowserAttribute *) d->get_browser_node();
      
      if (att->deletedp() ||
	  (l.findRef((BrowserClass *) att->parent())  == -1)) {
	// must be removed
	attributes.remove();
	d = attributes.current();
	it_val = values.remove(it_val);
      }
      else {
	connect(d, SIGNAL(changed()), this, SLOT(modified()));
	connect(d, SIGNAL(deleted()), this, SLOT(modified()));
	
	d = attributes.next();
	++it_val;
      }
    } while(it_val != values.end());
  }
}

void OdClassInstCanvas::change_scale() {
  QCanvasRectangle::setVisible(FALSE);
  compute_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void OdClassInstCanvas::draw(QPainter & p) {
  if (visible()) {
    QRect r = rect();
    QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
    QColor bckgrnd = p.backgroundColor();
    double zoom = the_canvas()->zoom();

    p.setBackgroundMode((used_color == UmlTransparent) ? QObject::TransparentMode : QObject::OpaqueMode);
    
    QColor co = color(used_color);
    FILE * fp = svg();

    if (fp != 0)
      fputs("<g>\n", fp);
    
    if (used_color != UmlTransparent) {
      const int shadow = the_canvas()->shadow();
      
      if (shadow != 0) {
	r.setRight(r.right() - shadow);
	r.setBottom(r.bottom() - shadow);
	
	p.fillRect (r.right(), r.top() + shadow,
		    shadow, r.height() - 1,
		    QObject::darkGray);
	p.fillRect (r.left() + shadow, r.bottom(),
		    r.width() - 1, shadow,
		    QObject::darkGray);

	if (fp != 0) {
	  fprintf(fp, "\t<rect fill=\"#%06x\" stroke=\"none\" stroke-opacity=\"1\""
		  " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
		  QObject::darkGray.rgb()&0xffffff,
		  r.right(), r.top() + shadow, shadow - 1, r.height() - 1 - 1);

	  fprintf(fp, "\t<rect fill=\"#%06x\" stroke=\"none\" stroke-opacity=\"1\""
		  " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
		  QObject::darkGray.rgb()&0xffffff,
		  r.left() + shadow, r.bottom(), r.width() - 1 - 1, shadow - 1);
	}
      }
    }
    
    p.setBackgroundColor(co);
    
    if (used_color != UmlTransparent) {
      p.fillRect(r, co);

      if (fp != 0)
	fprintf(fp, "\t<rect fill=\"#%06x\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\""
		" x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
		co.rgb()&0xffffff, 
		r.x(), r.y(), r.width() - 1, r.height() - 1);
    }
    else if (fp != 0)
      fprintf(fp, "\t<rect fill=\"none\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\""
	      " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
	      r.x(), r.y(), r.width() - 1, r.height() - 1);

    p.drawRect(r);
    
    const int two = (int) (2 * zoom);
    int he = fm.height() + two;
    
    p.setFont(the_canvas()->get_font(UmlNormalFont));

    r.setTop(r.top() + two);
    if (horiz) {
      p.drawText(r, QObject::AlignHCenter + QObject::AlignTop,
		 full_name());
      if (fp != 0)
	draw_text(r, QObject::AlignHCenter + QObject::AlignTop,
		  full_name(),
		  p.font(), fp);
    }
    else {
      p.drawText(r, QObject::AlignHCenter + QObject::AlignTop,
		 get_name() + ":");
      if (fp != 0)
	draw_text(r, QObject::AlignHCenter + QObject::AlignTop,
		  get_name() + ":",
		  p.font(), fp);
      r.setTop(r.top() + fm.height());
      p.drawText(r, QObject::AlignHCenter + QObject::AlignTop,
		 cl->get_name());
      if (fp != 0)
	draw_text(r, QObject::AlignHCenter + QObject::AlignTop,
		  cl->get_name(),
		  p.font(), fp);
    }
    
    if (!attributes.isEmpty()) {
      r.setTop(r.top() + he + two);
      p.drawLine(r.topLeft(), r.topRight());
      if (fp != 0)
	fprintf(fp, "\t<line stroke=\"black\" stroke-opacity=\"1\""
		" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		r.left(), r.top(), r.right(), r.top());

      r.setTop(r.top() + two);
      r.setLeft(r.left() + (int) (4 * zoom));
      
      QListIterator<AttributeData> it_at(attributes);
      QStringList::Iterator it_val = values.begin();
      QString egal = " = ";

      do {
	QString s =
	  it_at.current()->get_browser_node()->get_name() + egal + *it_val;      

	p.drawText(r, QObject::AlignTop, s);
	if (fp != 0)
	  draw_text(r, QObject::AlignTop, s,
		    p.font(), fp);
	r.setTop(r.top() + he);
	++it_at;
	++it_val;
      } while (it_val != values.end());
    }

    if (fp != 0)
      fputs("</g>\n", fp);
        
    if (selected())
      show_mark(p, rect());
  }
}

UmlCode OdClassInstCanvas::type() const {
  return UmlClass;
}

BrowserClass * OdClassInstCanvas::get_type() {
  return cl;
}

QString OdClassInstCanvas::get_full_name() const {
  return full_name();
}

void OdClassInstCanvas::open() {
  ClassInstanceDialog d(this);
  
  if (d.exec() == QDialog::Accepted)
    modified();		// calls check_attributes
}

void OdClassInstCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle(full_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("Edit", 3);
  m.insertSeparator();
  m.insertItem("Select class in browser", 4);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  m.insertItem("Remove from view", 6);
  
  switch (m.exec(QCursor::pos())) {
  case 0:
    upper();
    modified();	// call package_modified
    return;
  case 1:
    lower();
    modified();	// call package_modified
    return;
  case 2:
    edit_drawing_settings();
    return;
  case 3:
    open();	// call package_modified
    return;
  case 4:
    cl->select_in_browser();
    return;
  case 5:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 6:
    delete_it();
    package_modified();
  default:
    return;
  }
}

void OdClassInstCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser") { 
    cl->select_in_browser();
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
  else if (s == "Edit") {
    open();  // call modified then package_modified
    return;
  }
  else 
    return;

  modified();
}

void OdClassInstCanvas::edit_drawing_settings() {
  QArray<StateSpec> st(1);
  QArray<ColorSpec> co(1);
  
  st[0].set("write name:type \nhorizontally", &write_horizontally);
  co[0].set("class instance color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() != QDialog::Accepted)
    return;
  modified();	// call package_modified
}

bool OdClassInstCanvas::has_drawing_settings() const {
  return TRUE;
}

void OdClassInstCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<StateSpec> st(1);
  QArray<ColorSpec> co(1);
  Uml3States write_horizontally;
  UmlColor itscolor;
  
  st[0].set("write name:type \nhorizontally", &write_horizontally);
  co[0].set("class instance color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      if (st[0].name != 0)
	((OdClassInstCanvas *) it.current())->write_horizontally =
	  write_horizontally;
      if (co[0].name != 0)
	((OdClassInstCanvas *) it.current())->itscolor = itscolor;
      ((OdClassInstCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

void OdClassInstCanvas::set_type(BrowserClass * t) {
  if (t != cl) {
    disconnect(cl->get_data(), 0, this, 0);
    cl = t;
    connect(cl->get_data(), SIGNAL(changed()), this, SLOT(modified()));
    connect(cl->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  }
}

bool OdClassInstCanvas::alignable() const {
  return TRUE;
}

bool OdClassInstCanvas::copyable() const {
  return selected();
}

const char * OdClassInstCanvas::may_start(UmlCode &) const {
  // all (object link & anchor) allowed
  return 0;
}

const char * OdClassInstCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  return ((dest->type() == UmlClass)
	  ? ((l == UmlObjectLink) || IsaRelation(l))
	  : (l == UmlAnchor))
    ? 0 : "illegal";
}

void OdClassInstCanvas::connexion(UmlCode t, DiagramItem * dest, const QPoint &, const QPoint &) {
  ArrowCanvas * l;
  
  if (t == UmlObjectLink)
    l = new ObjectLinkCanvas(the_canvas(), this, dest, 0, 0);
  else
    l = new ArrowCanvas(the_canvas(), this, dest, t, 0, FALSE);
  
  l->show();
  the_canvas()->select(l);
}

bool OdClassInstCanvas::move_with_its_package() const {
  return TRUE;
}

void OdClassInstCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "classinstance_ref " << get_ident() << " // "
      << full_name();
  else {
    nl_indent(st);
    st << "classinstance " << get_ident() << ' ';
    cl->save(st, TRUE, warning);
    nl_indent(st);
    if (itscolor != UmlDefaultColor)
      st << "  color " << stringify(itscolor) << ' ';
    if (write_horizontally != UmlDefaultState)
      st << "  write_horizontally " << stringify(write_horizontally) << ' ';
    save_xyz(st, this, "  xyz");
    st << " name ";
    save_string(get_name(), st);
    indent(+1);
    if (! attributes.isEmpty()) {
      nl_indent(st);
      st << "values";
      indent(+1);
      
      QListIterator<AttributeData> it_at(attributes);
      QStringList::ConstIterator it_val = values.begin();
      QString dummy;

      do {
	nl_indent(st);
	(*it_at)->get_browser_node()->save(st, TRUE, dummy);
	nl_indent(st);
	save_string(fromUnicode(*it_val), st);
	++it_at;
	++it_val;
      } while (it_val != values.end());
      
      indent(-1);
    }
    
    indent(-1);
    nl_indent(st);
    st << "end";
  }
}

OdClassInstCanvas * OdClassInstCanvas::read(char * & st, UmlCanvas * canvas,
					      char * k)
{
  if (!strcmp(k, "classinstance_ref"))
    return ((OdClassInstCanvas *) dict_get(read_id(st), "classinstance", canvas));
  else if (!strcmp(k, "classinstance")) {
    int id = read_id(st);
    BrowserClass * cl = BrowserClass::read_ref(st);
    
    k = read_keyword(st);
    
    UmlColor co = UmlDefaultColor;
    
    read_color(st, "color", co, k);	// updates k
    
    Uml3States ho;
    
    if (!strcmp(k, "write_horizontally") || 
	!strcmp(k, "write_horizontaly")) {
      ho = state(read_keyword(st));
      k = read_keyword(st);
    }
    else
      ho = UmlDefaultState;
    
    if (strcmp(k, "xyz"))
      wrong_keyword(k, "xyz");
    
    int x = (int) read_double(st);
    OdClassInstCanvas * result =
      new OdClassInstCanvas(cl, canvas, x, (int) read_double(st), id);

    result->setZ(read_double(st));
    read_keyword(st, "name");
    result->set_name(read_string(st));
    result->itscolor = co;
    result->write_horizontally = ho;
    
    k = read_keyword(st);
    
    QList<AttributeData> attributes;
    QStringList values;
    
    if (!strcmp(k, "values")) {
      while (strcmp(k = read_keyword(st), "end") &&
	     strcmp(k, "xyz")) {	// old version
	BrowserAttribute * a = BrowserAttribute::read(st, k, 0);
	QString s = read_string(st);
	
	if (a != 0) {
	  attributes.append((AttributeData *) a->get_data());
	  values.append(toUnicode(s));
	}
      }
    }
    else if (strcmp(k, "end") && 
	     strcmp(k, "xyz"))
      wrong_keyword(k, "end or xyz");
    
    if (*k == 'x')
      read_xyz(st, result);
    result->attributes = attributes;
    result->values = values;
    result->check_attributes();
    result->compute_size();
    result->set_center100();
    result->show();
    return result;
  }
  else
    return 0;
}

void OdClassInstCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(browser_node->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

void OdClassInstCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}
