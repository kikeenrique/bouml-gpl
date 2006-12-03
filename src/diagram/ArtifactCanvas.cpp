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

#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "ArtifactCanvas.h"
#include "AssocContainCanvas.h"
#include "SimpleRelationCanvas.h"
#include "ArtifactData.h"
#include "BrowserArtifact.h"
#include "BrowserDeploymentDiagram.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"
#include "strutil.h"

ArtifactCanvas::ArtifactCanvas(BrowserNode * bn, UmlCanvas * canvas,
				 int x, int y)
    : DiagramCanvas(0, canvas, x, y, ARTIFACT_CANVAS_MIN_SIZE, 1, 0) {
  browser_node = bn;
  itscolor = UmlDefaultColor;
  
  compute_size();
  
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

ArtifactCanvas::ArtifactCanvas(UmlCanvas * canvas, int id)
    : DiagramCanvas(canvas, id) {
  // for read operation
  browser_node = 0;
  itscolor = UmlDefaultColor;
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

ArtifactCanvas::~ArtifactCanvas() {
}

void ArtifactCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
  DiagramCanvas::delete_it();
}

void ArtifactCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void ArtifactCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void ArtifactCanvas::compute_size() {
  // <<artifact>>/stereotype on 2*font_height with the icon on the right
  // the icon height = 2*font_height
  // the icon width = 3*height/4
  // name on font_height+4 points
  const int four = (int) (4 * the_canvas()->zoom());
  QFontMetrics fm(the_canvas()->get_font(UmlNormalBoldFont));
  int he = 3*fm.height() + four;
  int wi = fm.width(browser_node->get_name());
  const BasicData * data = browser_node->get_data();
  int stw = fm.width((data->get_stereotype()[0])
		     ? (QString("<<") + toUnicode(data->get_stereotype()) + ">>")
		     : QString("<<artifact>>"))
    + 3*fm.height();
  
  if (wi < stw)
    wi = stw;
  
  wi += four*2;
  
  int min_w = (int) (ARTIFACT_CANVAS_MIN_SIZE * the_canvas()->zoom());
  
  if (wi <min_w) 
    wi = min_w;
  
  used_color = (itscolor == UmlDefaultColor)
    ? the_canvas()->browser_diagram()->get_color(UmlArtifact)
    : itscolor;
  
  if (used_color != UmlTransparent) {
    const int shadow = the_canvas()->shadow();
    
    wi += shadow;
    he += shadow;
  }
  
  // force odd width and height for line alignment
  setSize(wi | 1, he | 1);
}

void ArtifactCanvas::change_scale() {  
  QCanvasRectangle::setVisible(FALSE);
  compute_size();
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void ArtifactCanvas::modified() {
  hide();
  hide_lines();
  compute_size();
  show();
  update_show_lines();
  draw_all_relations();
  canvas()->update();
  package_modified();
}

void ArtifactCanvas::connexion(UmlCode action, DiagramItem * dest,
				const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (action == UmlContain)
    a = new AssocContainCanvas(the_canvas(), this, dest, 0);
  else if (IsaSimpleRelation(action))
    a = new SimpleRelationCanvas(the_canvas(), this, dest, 0, action, 0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE);
  
  a->show();
  the_canvas()->select(a);
}

void ArtifactCanvas::post_connexion(UmlCode action, DiagramItem * dest) {
  if (action == UmlContain)
    ((ArtifactData *) browser_node->get_data())
      ->associate((BrowserArtifact *)
		  ((ArtifactCanvas *) dest)->browser_node);
}

void ArtifactCanvas::unassociate(DiagramItem * other) {
  ((ArtifactData *) browser_node->get_data())
    ->unassociate((BrowserArtifact *) 
		  ((ArtifactCanvas *) other)->browser_node);
}

void ArtifactCanvas::draw_all_relations() {
  if (!strcmp(browser_node->get_stereotype(), "source")) {
    // remove all association starting from 'this'
    QListIterator<ArrowCanvas> it(lines);
    
    while (it.current()) {
      if ((it.current()->type() == UmlContain) &&
	  (((AssocContainCanvas *) it.current())->get_start() == this))
	it.current()->delete_it();
      else
	++it;
    }
    
    // update non source artifact vis a vis 'this'
    QCanvasItemList all = canvas()->allItems();
    QCanvasItemList::Iterator cit;
    
    for (cit = all.begin(); cit != all.end(); ++cit) {
      if ((*cit)->visible()) {
	DiagramItem * adi = QCanvasItemToDiagramItem(*cit);
	
	if ((adi != 0) &&		// an uml canvas item
	    (adi->type() == UmlArtifact) &&
	    strcmp(((ArtifactCanvas *) adi)->browser_node->get_stereotype(), "source"))
	  ((ArtifactCanvas *) adi)->update_relations(this);
      }
    }
  }
  else
    update_relations();
}

void ArtifactCanvas::update_relations(ArtifactCanvas * other) {
  // 'this' is a non source, check association with 'other'
  const QPtrDict<BrowserArtifact> * associated = 
    ((ArtifactData *) browser_node->get_data())->get_associated();
  bool association_must_exist = 
    ((associated != 0) &&
     (associated->find((BrowserArtifact *) other->browser_node) != 0));
  QListIterator<ArrowCanvas> it(lines);
    
  while (it.current()) {
    if ((it.current()->type() == UmlContain) &&
	(((AssocContainCanvas *) it.current())->get_end() == other)) {
      if (! association_must_exist)
	it.current()->delete_it();
      return;
    }
    ++it;
  }

  // association not yet exist
  
  if (association_must_exist)
    (new AssocContainCanvas(the_canvas(), this, other, 0))->show();
}

void ArtifactCanvas::update_relations() {
  // 'this' is a non source, check its associations
  const QPtrDict<BrowserArtifact> * associated = 
    ((ArtifactData *) browser_node->get_data())->get_associated();
  QPtrDict<BrowserArtifact> associations;
  QListIterator<ArrowCanvas> it(lines);
    
  while (it.current()) {
    if ((it.current()->type() == UmlContain) &&
	(((AssocContainCanvas *) it.current())->get_start() == this)) {
      DiagramItem * adi = ((AssocContainCanvas *) it.current())->get_end();
      
      if ((adi->type() == UmlArtifact) &&
	  (associated != 0) &&
	  (associated->find((BrowserArtifact *) 
			    ((ArtifactCanvas *) adi)->browser_node) != 0)) {
	// association must exist
	BrowserArtifact * c = (BrowserArtifact *) 
	  ((ArtifactCanvas *) adi)->browser_node;
	
	associations.insert(c, c);
	++it;
      }
      else
	// association must not exist
	it.current()->delete_it();
    }
    else
      ++it;
  }
  
  if (associated != 0) {
    QPtrDictIterator<BrowserArtifact> it(*associated);
    
    while (it.current()) {
      if (associations.find(it.current()) == 0) {
	// the association to 'it.current()' is not yet drawed
	
	// search 'it.current()' cancas
	QCanvasItemList all = canvas()->allItems();
	QCanvasItemList::Iterator cit;
	
	for (cit = all.begin(); cit != all.end(); ++cit) {
	  if ((*cit)->visible()) {
	    DiagramItem * adi = QCanvasItemToDiagramItem(*cit);
	    
	    if ((adi != 0) &&		// an uml canvas item
		(adi->type() == UmlArtifact) &&
		(((ArtifactCanvas *) adi)->browser_node == it.current())) {
	      // find
	      (new AssocContainCanvas(the_canvas(), this, adi, 0))->show();
	      break;
	    }
	  }
	}
      }
      ++it;
    }
  }
}

void ArtifactCanvas::draw_all_the_relations(UmlCanvas * canvas) {
  QCanvasItemList all = canvas->allItems();
  QCanvasItemList::Iterator cit;
  
  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible()) {
      DiagramItem * adi = QCanvasItemToDiagramItem(*cit);
      
      if ((adi != 0) &&		// an uml canvas item
	  (adi->type() == UmlArtifact))
	if (!strcmp(((ArtifactCanvas *) adi)->browser_node->get_stereotype(),
		    "source")) {
	// remove all association starting from adi
	QListIterator<ArrowCanvas> it(((ArtifactCanvas *) adi)->lines);
	
	while (it.current()) {
	  if ((it.current()->type() == UmlContain) &&
	      (((AssocContainCanvas *) it.current())->get_start() == adi))
	    it.current()->delete_it();
	  else
	    ++it;
	}
      }
      else
	((ArtifactCanvas *) adi)->update_relations();
    }
  }
}

void ArtifactCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QColor bckgrnd = p.backgroundColor();

  p.setBackgroundMode((used_color == UmlTransparent) ? QObject::TransparentMode : QObject::OpaqueMode);

  QColor co = color(used_color);

  p.setBackgroundColor(co);
  
  // <<artifact>>/stereotype on 2*font_height with the icon on the right
  // the icon height = 2*font_height
  // the icon width = 3*height/4
  // name on font_height+4 points
  const int four = (int) (4 * the_canvas()->zoom());
  QFontMetrics fm(the_canvas()->get_font(UmlNormalBoldFont));
  const int he = fm.height();
  QRect r = rect();
  const BasicData * data = browser_node->get_data();
  
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
		  QObject::darkGray);}
  }
  
  QRect re = r;
  
  if (used_color != UmlTransparent) p.fillRect(r, co);
  p.drawRect(r);

  r.setHeight(he*2);
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  if (data->get_stereotype()[0])
    p.drawText(r, QObject::AlignCenter,
	       QString("<<") + toUnicode(data->get_stereotype()) + ">>");
  else
    p.drawText(r, QObject::AlignCenter, "<<artifact>>");
  
  r.moveBy(0, r.height());
  r.setHeight(he+four);
  p.setFont(the_canvas()->get_font(UmlNormalBoldFont));
  p.drawText(r, QObject::AlignCenter, browser_node->get_name());
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  
  // draw icon
  re.setLeft(re.right() - 6*he/4 + four);
  re.setRight(re.right() - four);
  re.setTop(re.top() + four);
  re.setHeight(2*(he - four));
  
  QPointArray a(7);
  const int corner_size = re.width()/3;
  
  a.setPoint(0, re.left(), re.top());
  a.setPoint(1, re.right() - corner_size, re.top());
  a.setPoint(2, re.right() - corner_size, re.top() + corner_size);
  a.setPoint(3, re.right(), re.top() + corner_size);
  a.setPoint(4, re.right(), re.bottom());
  a.setPoint(5, re.left(), re.bottom());
  a.setPoint(6, re.left(), re.top());
  p.drawPolyline(a);
  p.moveTo(re.right() - corner_size, re.top());
  p.lineTo(re.right(), re.top() + corner_size);
  
  p.setBackgroundColor(bckgrnd);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode ArtifactCanvas::type() const {
  return UmlArtifact;
}

void ArtifactCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool ArtifactCanvas::alignable() const {
  return TRUE;
}

bool ArtifactCanvas::copyable() const {
  return selected();
}

void ArtifactCanvas::open() {
  browser_node->open(FALSE);
}

void ArtifactCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu gensubm(0);
  QPopupMenu toolm(0);
  int index;
  
  m.insertItem(new MenuTitle(browser_node->get_name(), m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();
  m.insertItem("Edit artifact", 3);
  m.insertSeparator();
  m.insertItem("Select in browser", 4);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  if (browser_node->is_writable())
    m.insertItem("Set associated diagram",6);
  m.insertSeparator();
  m.insertItem("Remove from view", 7);
  if (browser_node->is_writable())
    m.insertItem("Delete from model", 8);
  m.insertSeparator();
  m.insertItem("Generate", &gensubm);
  if (Tool::menu_insert(&toolm, UmlArtifact, 20))
    m.insertItem("Tool", &toolm);
  
  gensubm.insertItem("C++", 9);
  gensubm.insertItem("Java", 10);
  gensubm.insertItem("Idl", 11);
  
  switch (index = m.exec(QCursor::pos())) {
  case 0:
    upper();
    modified();	// call package_modified()
    return;
  case 1:
    lower();
    modified();	// call package_modified()
    return;
  case 2:
    edit_drawing_settings();
    return;
  case 3:
    browser_node->open(TRUE);
    return;
  case 4:
    browser_node->select_in_browser();
    return;
  case 5:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 6:
    ((BrowserArtifact *) browser_node)
      ->set_associated_diagram((BrowserDeploymentDiagram *)
			       the_canvas()->browser_diagram());
    return;
  case 7:
    //remove from view
    delete_it();
    break;
  case 8:
    //delete from model
    browser_node->delete_it();	// will delete the canvas
    break;
  case 9:
    browser_node->apply_shortcut("Generate C++");
    return;
  case 10:
    browser_node->apply_shortcut("Generate Java");
    return;
  case 11:
    browser_node->apply_shortcut("Generate Idl");
    return;
  default:
    if (index >= 20)
      ToolCom::run(Tool::command(index - 20), browser_node);
    return;
  }
  
  package_modified();
}

void ArtifactCanvas::apply_shortcut(QString s) {
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

void ArtifactCanvas::edit_drawing_settings() {
  QArray<StateSpec> st;
  QArray<ColorSpec> co(1);
  
  co[0].set("artifact color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();	// call package_modified()
}

bool ArtifactCanvas::has_drawing_settings() const {
  return TRUE;
}

void ArtifactCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  
  co[0].set("artifact color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if ((dialog.exec() == QDialog::Accepted) && (co[0].name != 0)) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      ((ArtifactCanvas *) it.current())->itscolor = itscolor;
      ((ArtifactCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

const char * ArtifactCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlContain:
    if (!browser_node->is_writable())
      return "read only";
    else if (strcmp(browser_node->get_stereotype(), "source"))
      return "illlegal for a non source artifact";
    else
      return 0;
  case UmlDependency:
    l = UmlDependOn;
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

const char * ArtifactCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  switch (dest->type()) {
  case UmlArtifact:
    switch (l) {
    case UmlContain:
    case UmlDependOn:
      return (dest == this) ? "illegal" : 0;
    case UmlInherit:
      return browser_node->check_inherit(dest->get_bn());;
    default:
      return "illegal";
    }
  case UmlPackage:
  case UmlComponent:
  case UmlDeploymentNode:
    return (l == UmlDependOn) ? 0 : "illegal";
  case UmlNote:
  case UmlIcon:
    return (l == UmlAnchor) ? 0 : "illegal";
  default:
    return "illegal";
  }
}

bool ArtifactCanvas::move_with_its_package() const {
  return TRUE;
}

void ArtifactCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "artifactcanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "artifactcanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    if (itscolor != UmlDefaultColor) {
      nl_indent(st);
      st << "color " << stringify(itscolor);
    }
    nl_indent(st);
    save_xyz(st, this, "xyz");
    indent(-1);
  }
}

ArtifactCanvas * ArtifactCanvas::read(char * & st, UmlCanvas * canvas,
					char * k)
{
  if (!strcmp(k, "artifactcanvas_ref") || 
      ((read_file_format() < 20) && !strcmp(k, "componentcanvas_ref")))
    return ((ArtifactCanvas *) dict_get(read_id(st), "artifactcanvas", canvas));
  else if (!strcmp(k, "artifactcanvas") || 
	   ((read_file_format() < 20) && !strcmp(k, "componentcanvas"))) {
    int id = read_id(st);
    BrowserArtifact * br = BrowserArtifact::read_ref(st, read_keyword(st));
    ArtifactCanvas * result = new ArtifactCanvas(canvas, id);
    
    result->browser_node = br;
    connect(br->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(br->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));

    k = read_keyword(st);
    
    read_color(st, "color", result->itscolor, k);	// updates k
    
    if (strcmp(k, "xyz"))
      wrong_keyword(k, "xyz");
    read_xyz(st, result);
    
    result->compute_size();
    result->set_center100();
    result->show();
    return result;
  }
  else 
    return 0;
}

void ArtifactCanvas::history_hide() {
  DiagramCanvas::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  disconnect(browser_node->get_data(), 0, this, 0);
}

void ArtifactCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}
