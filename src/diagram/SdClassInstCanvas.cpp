// *************************************************************************
//
// Copyleft 2004-2007 Bruno PAGES  .
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
#include <qpopupmenu.h> 
#include <qpainter.h> 

#include "SdClassInstCanvas.h"
#include "BrowserDiagram.h"
#include "BrowserClass.h"
#include "BrowserClassInstance.h"
#include "ClassInstanceData.h"
#include "UmlCanvas.h"
#include "ClassData.h"
#include "InstanceDialog.h"
#include "SettingsDialog.h"
#include "Settings.h"
#include "myio.h"
#include "MenuTitle.h"

SdClassInstCanvas::SdClassInstCanvas(BrowserNode * bn, UmlCanvas * canvas,
				     int x, int id)
    : SdObjCanvas(0, canvas, x, CLASSINST_CANVAS_MIN_SIZE, 1, id),
      ClassInstCanvas() {
  // bn may a class instance or a class
  browser_node = bn;
  
  itscolor = UmlDefaultColor;
  drawing_mode = DefaultClassDrawingMode;
  
  if (id == 0)
    // not on read
    compute_size();
  
  BasicData * d = bn->get_data();
  
  connect(d, SIGNAL(changed()), this, SLOT(modified()));
  connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

SdClassInstCanvas::~SdClassInstCanvas() {
}

void SdClassInstCanvas::delete_it() {
  BasicData * d = browser_node->get_data();

  disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
  disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  
  SdObjCanvas::delete_it();
}

void SdClassInstCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void SdClassInstCanvas::remove(bool from_model) {
  if (!from_model || (browser_node->get_type() == UmlClass))
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void SdClassInstCanvas::compute_size() {
  int w, h;
  SequenceDiagramSettings dflt;
  
  dflt.instances_drawing_mode = drawing_mode;
  the_canvas()->browser_diagram()->get_sequencediagramsettings(dflt);
  
  used_drawing_mode = dflt.instances_drawing_mode;
    
  if (used_drawing_mode == Natural) {
    const char * st = get_class()->get_stereotype();
    
    if (!strcmp(st, "control"))
      used_drawing_mode = asControl;
    else if (!strcmp(st, "entity"))
      used_drawing_mode = asEntity;
    else if (!strcmp(st, "boundary"))
      used_drawing_mode = asBoundary;
    else if (!strcmp(st, "actor"))
      used_drawing_mode = asActor;
    else
      used_drawing_mode = asClass;
  }

  if (used_drawing_mode == asClass)
    ClassInstCanvas::compute_size(w, h, the_canvas());
  else {
    used_color = (itscolor == UmlDefaultColor)
      ? the_canvas()->browser_diagram()->get_color(UmlClass)
      : itscolor;
  
    int minw;
    QFontMetrics fm(the_canvas()->get_font(UmlNormalUnderlinedFont));
    double zoom = the_canvas()->zoom();
    int delta = (int) (4 * zoom);
    
    h = fm.height() + delta;
    
    switch (used_drawing_mode) {
    case asEntity:
      minw = (int) (ENTITY_SIZE * zoom);
      h += minw;
      break;
    case asControl:
      minw = (int) (CONTROL_WIDTH * zoom);
      h += (int) (CONTROL_HEIGHT * zoom);
      break;
    case asBoundary:
      minw = (int) (BOUNDARY_WIDTH * zoom);
      h += (int) (BOUNDARY_HEIGHT * zoom);
      break;
    default: // asActor
      {
	// divide original size by two to not be on life line
	int actor_size = (int) (ACTOR_SIZE / 2.0 * zoom);
	
	h += actor_size;
	minw = actor_size;
      }
    }
    
    w = fm.width(full_name());
    
    if (w < minw)
      w = minw;
    
    w += delta;
  }
  
  resize(w, h);
}

void SdClassInstCanvas::modified() {
  if (visible()) {
    int w = width();
    
    hide();
    compute_size();
    DiagramCanvas::moveBy((w-width())/2, 0);
    show();
    canvas()->update();
    package_modified();
  }
}

void SdClassInstCanvas::draw(QPainter & p) {
  if (visible()) {
    QRect r = rect();
    
    if (used_drawing_mode == asClass)
      ClassInstCanvas::draw(p, the_canvas(), r);
    else {
      FILE * fp = svg();
      
      if (fp != 0)
	fputs("<g>\n", fp);
  
      double zoom = the_canvas()->zoom();
      
      switch (used_drawing_mode) {
      case asEntity:
	DiagramCanvas::draw_entity_icon(p, r, used_color, zoom);
	r.setTop(r.top() + (int) (ENTITY_SIZE * zoom));
	break;
      case asControl:
	DiagramCanvas::draw_control_icon(p, r, used_color, zoom);
	r.setTop(r.top() + (int) (CONTROL_HEIGHT * zoom));
	break;
      case asBoundary:
	DiagramCanvas::draw_boundary_icon(p, r, used_color, zoom);
	r.setTop(r.top() + (int) (BOUNDARY_HEIGHT * zoom));
	break;
      default: // asActor
	{
	  // divide original size by two to not be on life line
	  double actor_size = ACTOR_SIZE / 2.0 * zoom;
	  
	  r.setHeight((int) actor_size);
	  r.setLeft(r.left() + 
		    (int) ((r.width() - actor_size)/2));
	  r.setWidth(r.height());
	  draw_actor(&p, r);
	  r = rect();
	  r.setTop(r.top() + (int) actor_size);
	}
      }
      
      QColor bckgrnd = p.backgroundColor();
      
      p.setBackgroundMode(QObject::TransparentMode);
      p.setFont(the_canvas()->get_font(UmlNormalFont));
      p.drawText(r, QObject::AlignCenter, full_name());
      
      if (fp != 0)
	draw_text(r, QObject::AlignCenter, full_name(),
		  p.font(), fp);
      p.setBackgroundColor(bckgrnd);

      if (fp != 0)
	fputs("</g>\n", fp);
    }

    if (selected())
      show_mark(p, rect());
  }
}

// all cases
QString SdClassInstCanvas::get_name() const {
  return (browser_node->get_type() == UmlClass)
    ? iname
    : QString(browser_node->get_name());
}

// out of model case
void SdClassInstCanvas::set_name(const QString & s) {
  iname = s;
}

// UmlClass or UmlClassInstance
UmlCode SdClassInstCanvas::type() const {
  return browser_node->get_type();
}

// return class, all cases
BrowserNode * SdClassInstCanvas::get_type() const {
  return (browser_node->get_type() == UmlClass)
    ? browser_node
    : ((BrowserNode *)
       ((ClassInstanceData *) browser_node->get_data())->get_class());
}

// ut of model case
void SdClassInstCanvas::set_type(BrowserNode * t) {
  if (t != browser_node) {
    disconnect(browser_node->get_data(), 0, this, 0);
    browser_node = t;
    connect(t->get_data(), SIGNAL(changed()), this, SLOT(modified()));
    connect(t->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  }
}

BrowserNodeList& SdClassInstCanvas::get_types(BrowserNodeList& r) const {
  return BrowserClass::instances(r);
}

BrowserNode * SdClassInstCanvas::container(UmlCode c) const {
  return the_canvas()->browser_diagram()->container(c);
}

BrowserClass* SdClassInstCanvas::get_class() const {
  return (BrowserClass*) get_type();
}

void SdClassInstCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  if (browser_node->get_type() == UmlClass)
    in_model |= browser_node->is_writable();
}

void SdClassInstCanvas::open() {
  if (browser_node->get_type() == UmlClass) {
    InstanceDialog d(this, "class", UmlClass);
    
    d.raise();
    if (d.exec() == QDialog::Accepted)
      modified();
  }
  else
    browser_node->open(FALSE);
}

void SdClassInstCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  bool modelized = (browser_node->get_type() == UmlClassInstance);
  
  m.insertItem(new MenuTitle(full_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  if (is_mortal())
    m.insertItem("Become immortal", 3);
  else
    m.insertItem("Become mortal", 4);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 5);
  m.insertSeparator();
  if (browser_node->get_type() == UmlClassInstance)
    m.insertItem("Select in browser", 6);
  m.insertItem("Select class in browser", 7);
  m.insertSeparator();
  if (modelized)
    m.insertItem("Exit from model", 10);
  else {
    if (container(UmlClass)->is_writable())
      m.insertItem("Insert in model", 11);
    m.insertItem("Replace it", 12);
  }
  m.insertSeparator();
  m.insertItem("Remove from view", 8);
  if ((browser_node->get_type() == UmlClassInstance) &&
      browser_node->is_writable())
    m.insertItem("Delete from model", 9);
  
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
    open();	// call modified()
    return;
  case 3:
    set_mortal(FALSE);
    break;
  case 4:
    set_mortal(TRUE);
    break;
  case 5:
    edit_drawing_settings();
    return;
  case 6:
    browser_node->select_in_browser();
    return;
  case 7:
    get_type()->select_in_browser();
    return;
  case 8:
    delete_it();
    break;
  case 9:
    //delete from model
    browser_node->delete_it();	// will delete the canvas
    break;
  case 10:
    {
      BasicData * d = browser_node->get_data();
      
      disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
      disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
      
      iname = browser_node->get_name();
      browser_node = ((ClassInstanceData *) d)->get_class();
      d = browser_node->get_data();
      connect(d, SIGNAL(changed()), this, SLOT(modified()));
      connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
    }
    break;
  case 11:
    {
      BasicData * d = browser_node->get_data();
      
      disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
      disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
      
      browser_node =
	new BrowserClassInstance(iname, (BrowserClass *) browser_node,
				 container(UmlClass));
      d = browser_node->get_data();
      connect(d, SIGNAL(changed()), this, SLOT(modified()));
      connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
    }
    break;
  case 12:
    {
      BrowserNode * bn = 
	BrowserClassInstance::get_classinstance((BrowserClass *) browser_node);
      
      if (bn == 0)
	return;
      
      BasicData * d = browser_node->get_data();
      
      disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
      disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
      
      browser_node = bn;
      d = browser_node->get_data();
      connect(d, SIGNAL(changed()), this, SLOT(modified()));
      connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
      modified();	// call package_modified
      return;
    }
  default:
    return;
  }
  
  package_modified();
}

void SdClassInstCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser") { 
    browser_node->select_in_browser();
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

void SdClassInstCanvas::edit_drawing_settings() {
  QArray<StateSpec> st(2);
  QArray<ColorSpec> co(1);
  
  st[0].set("drawing mode", &drawing_mode);
  st[1].set("write name:type \nhorizontally", &write_horizontally);
  co[0].set("class instance color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();
}

bool SdClassInstCanvas::has_drawing_settings() const {
  return TRUE;
}

void SdClassInstCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
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
	((SdClassInstCanvas *) it.current())->write_horizontally =
	  write_horizontally;
      if (co[0].name != 0)
	((SdClassInstCanvas *) it.current())->itscolor = itscolor;
      ((SdClassInstCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

void SdClassInstCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "classinstance_ref " << get_ident() << " // "
      << full_name();
  else if (browser_node->get_type() == UmlClass) {
    nl_indent(st);
    st << "classinstance " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    nl_indent(st);
    ClassInstCanvas::save(st);
    if (drawing_mode != DefaultClassDrawingMode)
      st << "  drawing_mode " << stringify(drawing_mode);
    st << "  name ";
    save_string(iname, st);
    st << ' ';
    SdObjCanvas::save(st);
  }
  else {
    nl_indent(st);
    st << "classinstancecanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    nl_indent(st);
    ClassInstCanvas::save(st);
    if (drawing_mode != DefaultClassDrawingMode)
      st << "  drawing_mode " << stringify(drawing_mode);
    SdObjCanvas::save(st);
  }
}

SdClassInstCanvas * SdClassInstCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "classinstance_ref"))
    return ((SdClassInstCanvas *) dict_get(read_id(st), "classinstance", canvas));
  else if (!strcmp(k, "classinstance")) {
    // old release and graphic instance
    int id = read_id(st);
    BrowserClass * cl = BrowserClass::read_ref(st);
    SdClassInstCanvas * result =
      new SdClassInstCanvas(cl, canvas, 0, id);
   
    result->ClassInstCanvas::read(st, k);

    if (!strcmp(k, "drawing_mode")) {
      result->drawing_mode = ::drawing_mode(read_keyword(st));
      k = read_keyword(st);
    }
    else
      result->drawing_mode = DefaultClassDrawingMode;
    
    if (strcmp(k, "name"))
      wrong_keyword(k, "name");
    
    result->iname = read_string(st);
    result->compute_size();
    result->SdObjCanvas::read(st, read_keyword(st));
    result->show();
    
    return result;
  }
  else if (!strcmp(k, "classinstancecanvas")) {
    int id = read_id(st);
    BrowserClassInstance * icl = BrowserClassInstance::read_ref(st);
    SdClassInstCanvas * result =
      new SdClassInstCanvas(icl, canvas, 0, id);
   
    result->ClassInstCanvas::read(st, k);

    if (!strcmp(k, "drawing_mode")) {
      result->drawing_mode = ::drawing_mode(read_keyword(st));
      k = read_keyword(st);
    }

    if (result->get_type() != 0) {
      // not a deleted instance
      result->compute_size();
      result->SdObjCanvas::read(st, k);
      result->show();
    }
    else
      result->SdObjCanvas::read(st, k);
    
    return result;
  }
  else
    return 0;
}

void SdClassInstCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  
  BasicData * d = browser_node->get_data();

  disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
  disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
}

void SdClassInstCanvas::history_load(QBuffer & b) {
  SdObjCanvas::history_load(b);
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  
  BasicData * d = browser_node->get_data();

  disconnect(d, SIGNAL(changed()), this, SLOT(modified()));
  disconnect(d, SIGNAL(deleted()), this, SLOT(deleted()));
}
