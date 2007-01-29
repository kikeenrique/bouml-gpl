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

#include <qcursor.h>
#include <qpainter.h>
#include <qpopupmenu.h> 

#include "PackageCanvas.h"
#include "SimpleRelationCanvas.h"
#include "PackageData.h"
#include "BrowserPackage.h"
#include "BrowserDiagram.h"
#include "UmlGlobal.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"

PackageCanvas::PackageCanvas(BrowserNode * bn, UmlCanvas * canvas,
			     int x, int y, int id)
    : DiagramCanvas(0, canvas, x, y, PACKAGE_CANVAS_MIN_WIDTH,
		    PACKAGE_CANVAS_MIN_HEIGHT, id) {
  browser_node = bn;
  show_context_mode = DefaultShowContextMode;
  name_in_tab = UmlDefaultState;
  itscolor = UmlDefaultColor;
  check_size();
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}

PackageCanvas::~PackageCanvas() {
}

void PackageCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  DiagramCanvas::delete_it();
}

void PackageCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void PackageCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

int PackageCanvas::min_width() {
  bool intab;
  ShowContextMode showcontext;
  
  the_canvas()->browser_diagram()->package_settings(intab, showcontext);
  
  in_tab = (name_in_tab != UmlDefaultState) ? (name_in_tab == UmlYes)
					    : intab;
  
  const PackageData * data = (const PackageData *) browser_node->get_data();

  switch ((show_context_mode != DefaultShowContextMode)
	  ? show_context_mode : showcontext) {
  case noContext:
    full_name = browser_node->get_name();
    break;
  case umlContext:
    full_name = browser_node->full_name();
    break;
  case namespaceContext:
    if ((full_name = ((const char *) data->get_cpp_namespace())).isEmpty())
      full_name = browser_node->get_name();
    break;
  case packageContext:
    if ((full_name = ((const char *) data->get_java_package())).isEmpty())
      full_name = browser_node->get_name();
    break;
  case moduleContext:
    if ((full_name = ((const char *) data->get_idl_module())).isEmpty())
      full_name = browser_node->get_name();
  case DefaultShowContextMode:
    // not possible
    break;
  }
  
  int eight = (int) (8 * the_canvas()->zoom());
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  
  int wi = (in_tab)
    ? (fm.width(browser_node->get_name()) + eight) * 5 / 2
    : fm.width(full_name) + eight;
  
  if (full_name != browser_node->get_name()) {
    int cntxw = fm.width(full_name) + eight;
    
    if (wi < cntxw)
      wi = cntxw;
  }
  
  if (data->get_stereotype()[0]) {
    int stw = fm.width(QString("<<") + toUnicode(data->get_stereotype()) + ">>") + eight;
    
    if (wi < stw)
      wi = stw;
  }
  
  int min_w = (int) (PACKAGE_CANVAS_MIN_WIDTH * the_canvas()->zoom());
  
  // note : used_color already updated
  if (used_color != UmlTransparent)
    min_w += the_canvas()->shadow();
  
  return (wi < min_w) ? min_w : wi;
}

int PackageCanvas::min_height() {
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  
  int he = fm.height() * 5;
  int min_h = (int) (PACKAGE_CANVAS_MIN_HEIGHT * the_canvas()->zoom());
  
  // note : used_color already updated
  if (used_color != UmlTransparent)
    min_h += the_canvas()->shadow();
  
  return (he < min_h) ? min_h : he;
  
}

void PackageCanvas::check_size() {
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlPackage)
    : itscolor;
  
  int wi = min_width();
  int he = min_height();
  
  // warning : do NOT check if ((width() < wi) || (height() < he))
  // because te resize must be done to set data on scale change
  // Force odd width and height for line alignment
  DiagramCanvas::resize((width() > wi) ? width() : wi | 1,
			(height() > he) ? height() : he | 1);
}
				    
void PackageCanvas::change_scale() {
  double scale = the_canvas()->zoom();
  
  QCanvasRectangle::setVisible(FALSE);
  setSize((int) (width_scale100*scale), (int) (height_scale100*scale));
  check_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);  
}

void PackageCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QColor bckgrnd = p.backgroundColor();

  p.setBackgroundMode((used_color == UmlTransparent) ? QObject::TransparentMode : QObject::OpaqueMode);

  QColor co = color(used_color);
  
  p.setBackgroundColor(co);
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  const int four = (int) (4 * the_canvas()->zoom());
  const int he = fm.height();
  QRect r = rect();
  const int shadow = the_canvas()->shadow();

  if ((used_color != UmlTransparent) && (shadow != 0)) {
    r.setRight(r.right() - shadow);
    r.setBottom(r.bottom() - shadow);
  }
  
  r.setWidth(r.width() * 2 / 5);
  r.setHeight(he + four);
  if (used_color != UmlTransparent) p.fillRect(r, co);
  p.drawRect(r);
  
  if ((used_color != UmlTransparent) && (shadow != 0))
    p.fillRect (r.right(), r.top() + shadow,
		shadow, r.height() - 1 - shadow,
		QObject::darkGray);

  const char * name = browser_node->get_name();
  
  if (in_tab)
    p.drawText(r, QObject::AlignCenter, name);
  
  r = rect();
  if ((used_color != UmlTransparent) && (shadow != 0)) {
    r.setRight(r.right() - shadow);
    r.setBottom(r.bottom() - shadow);
  }
  
  r.setTop(r.top() + he + four - 1);
  if (used_color != UmlTransparent) p.fillRect(r, co);
  p.drawRect(r);
  
  if ((used_color != UmlTransparent) && (shadow != 0)) {
    p.fillRect (r.right(), r.top() + shadow,
		shadow, r.height() - 1,
		QObject::darkGray);
    p.fillRect (r.left() + shadow, r.bottom(),
		r.width() - 1, shadow,
		QObject::darkGray);
  }
  
  const int three = (int) (3 * the_canvas()->zoom());
  
  r.setTop(r.top() + three);
  
  if (! in_tab) {
    p.drawText(r, QObject::AlignHCenter + QObject::AlignTop, name);
    r.setTop(r.top() + he + three);
  }
  
  const BasicData * data = browser_node->get_data();
  
  if (data->get_stereotype()[0]) {
    p.drawText(r, QObject::AlignHCenter + QObject::AlignTop, 
	       QString("<<") + toUnicode(data->get_stereotype()) + ">>");
    r.setTop(r.top() + he + three);
  }
  
  if (full_name != name) {
    p.setFont(the_canvas()->get_font(UmlNormalItalicFont));
    p.drawText(r, QObject::AlignHCenter + QObject::AlignTop, full_name);
    p.setFont(the_canvas()->get_font(UmlNormalFont));
  }
  
  p.setBackgroundColor(bckgrnd);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode PackageCanvas::type() const {
  return UmlPackage;
}

void PackageCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool PackageCanvas::alignable() const {
  return TRUE;
}

bool PackageCanvas::copyable() const {
  return selected();
}

void PackageCanvas::open() {
  browser_node->open(FALSE);
}

void PackageCanvas::modified() {
  // force son reaffichage
  hide();
  check_size();
  show();
  force_self_rel_visible();
  canvas()->update();
  package_modified();
}

void PackageCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 3);
  m.insertSeparator();
  m.insertItem("Select in browser", 4);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  if (browser_node->is_writable())
    m.insertItem("Set associated diagram", 6);
  m.insertSeparator();
  m.insertItem("Remove from view", 7);
  if (browser_node->is_writable())
    m.insertItem("Delete from model", 8);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, UmlPackage, 10))
    m.insertItem("Tool", &toolm);

  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    upper();
    modified();	// call package_modified()
    return;
  case 1:
    lower();
    modified();	// call package_modified()
    return;
  case 2:
    browser_node->open(TRUE);
    return;
  case 3:
    edit_drawing_settings();
    return;
  case 4:
    browser_node->select_in_browser();
    return;
  case 5:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 6:
    ((BrowserPackage *) browser_node)
      ->set_associated_diagram((BrowserNode *)
			       the_canvas()->browser_diagram());
    return;
  case 7:
    // remove from view
    delete_it();
    break;
  case 8:
    // delete from model
    browser_node->delete_it();	// will remove canvas
    break;
  default:
    if (rank >= 10)
      ToolCom::run(Tool::command(rank - 10), browser_node);
    return;
  }
  
  package_modified();
}

void PackageCanvas::apply_shortcut(QString s) {
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
  else {
    browser_node->apply_shortcut(s);
    return;
  }

  modified();
  package_modified();
}

void PackageCanvas::edit_drawing_settings() {
  QArray<StateSpec> st(2);
  QArray<ColorSpec> co(1);
  
  st[0].set("name in tab", &name_in_tab);
  st[1].set("show context", &show_context_mode);
  co[0].set("Package color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();
}

bool PackageCanvas::has_drawing_settings() const {
  return TRUE;
}

void PackageCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<StateSpec> st(2);
  QArray<ColorSpec> co(1);
  Uml3States name_in_tab;
  ShowContextMode show_context_mode;
  UmlColor itscolor;
  
  st[0].set("name in tab", &name_in_tab);
  st[1].set("show context", &show_context_mode);
  co[0].set("Package color", &itscolor);
  
  SettingsDialog dialog(&st, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      if (st[0].name != 0)
	((PackageCanvas *) it.current())->name_in_tab = name_in_tab;
      if (st[1].name != 0)
	((PackageCanvas *) it.current())->show_context_mode = show_context_mode;
      if (co[0].name != 0)
	((PackageCanvas *) it.current())->itscolor = itscolor;
      ((PackageCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

const char * PackageCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlDependency:
    l = UmlDependOn;
  case UmlDependOn:
    return (browser_node->is_writable()) ? 0 : "read only";
  case UmlGeneralisation:
    l = UmlInherit;
    return (browser_node->is_writable()) ? 0 : "read only";
  case UmlAnchor:
    return 0;
  default:
    return "illegal";
  }
}

const char * PackageCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  if (l == UmlAnchor) {
    switch (dest->type()) {
    case UmlNote:
    case UmlIcon:
    case UmlInfo:
      return 0;
    default:
      return "illegal";
    }
  }
  else if(dest->get_bn() == 0)
    return "illegal";
  else
    return ((BrowserPackage *) browser_node)->may_connect(l, dest->get_bn());
}

void PackageCanvas::connexion(UmlCode action, DiagramItem * dest,
			      const QPoint &, const QPoint &) {
  ArrowCanvas * r;
  
  if (IsaSimpleRelation(action))
    r = new SimpleRelationCanvas(the_canvas(), this, dest, 0, action, 0);
  else
    r = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  r->show();
  the_canvas()->select(r);
}

aCorner PackageCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void PackageCanvas::resize(aCorner c, int dx, int dy) {
  DiagramCanvas::resize(c, dx, dy, min_width(), min_height());
}

void PackageCanvas::prepare_for_move(bool on_resize) {
  if (! on_resize) {
    DiagramCanvas::prepare_for_move(on_resize);
    
    QCanvasItemList l = collisions(TRUE);
    QCanvasItemList::ConstIterator it;
    QCanvasItemList::ConstIterator end = l.end();
    DiagramItem * di;
    BrowserNode * p = get_bn();
  
    for (it = l.begin(); it != end; ++it) {
      if ((*it)->visible() && // at least not deleted
	  !(*it)->selected() &&
	  ((di = QCanvasItemToDiagramItem(*it)) != 0) &&
	  di->move_with_its_package()) {
	BrowserNode * bn = di->get_bn();
	
	do
	  bn = (BrowserNode *) bn->parent();
	while (bn->get_type() != UmlPackage);
	
	if (bn == p) {
	  the_canvas()->select(*it);
	  di->prepare_for_move(FALSE);
	}
      }
    }
  }
}

bool PackageCanvas::move_with_its_package() const {
  return TRUE;
}

void PackageCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "packagecanvas_ref " << get_ident() << " // " << browser_node->get_name();
  else {
    nl_indent(st);
    st << "packagecanvas " << get_ident() << ' ';
    indent(+1);
    browser_node->save(st, TRUE, warning);
    nl_indent(st);
    if (name_in_tab != UmlDefaultState)
      st << "name_in_tab " << stringify(name_in_tab);
    if (show_context_mode != DefaultShowContextMode)
      st << " show_context_mode " << stringify(show_context_mode);
    if (itscolor != UmlDefaultColor)
      st << " color " << stringify(itscolor);
    save_xyzwh(st, this, "  xyzwh");
    indent(-1);
  }
}

PackageCanvas * PackageCanvas::read(char * & st, UmlCanvas * canvas, char * k) {
  if (!strcmp(k, "packagecanvas_ref"))
    return ((PackageCanvas *) dict_get(read_id(st), "packagecanvas", canvas));
  else if (!strcmp(k, "packagecanvas")) {
    int id = read_id(st);
    BrowserNode * br = BrowserPackage::read_ref(st);
    UmlColor color = UmlDefaultColor;
    Uml3States in_tab = UmlDefaultState;
    ShowContextMode context = DefaultShowContextMode;
    
    k = read_keyword(st);
    
    if (!strcmp(k, "name_in_tab")) {
      in_tab = state(read_keyword(st));
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "show_context_mode")) {
      context = context_mode(read_keyword(st));
      k = read_keyword(st);
    }

    read_color(st, "color", color, k);
    
    if (strcmp(k, "xyzwh"))
      wrong_keyword(k, "xyzwh");
    
    int x = (int) read_double(st);
    PackageCanvas * result =
      new PackageCanvas(br, canvas, x, (int) read_double(st), id);
    
    result->name_in_tab = in_tab;
    result->show_context_mode = context;
    result->itscolor = color;
    read_zwh(st, result);
    result->check_size();
    result->set_center100();

    result->show();
    return result;
  }
  else
    return 0;
}

void PackageCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
}

void PackageCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void PackageCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
  
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}
