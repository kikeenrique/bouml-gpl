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

#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "ActivityNodeCanvas.h"
#include "ActivityContainerCanvas.h"
#include "FlowCanvas.h"
#include "SimpleData.h"
#include "BrowserActivityNode.h"
#include "LabelCanvas.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"

ActivityNodeCanvas::ActivityNodeCanvas(BrowserNode * bn, UmlCanvas * canvas,
				       int x, int y)
    : DiagramCanvas(0, canvas, x, y, 16, 16, 0), horiz(FALSE) {
  browser_node = bn;
  set_xpm();
   
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));

  if (canvas->must_draw_all_relations())
    draw_all_flows();
}

ActivityNodeCanvas::ActivityNodeCanvas(UmlCanvas * canvas, int id)
    : DiagramCanvas(canvas, id), horiz(FALSE) {
  // for read operation
  browser_node = 0;
  xpm = 0;
}

ActivityNodeCanvas::~ActivityNodeCanvas() {
}

void ActivityNodeCanvas::delete_it() {
  disconnect(browser_node->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  
  DiagramCanvas::delete_it();
}

void ActivityNodeCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void ActivityNodeCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void ActivityNodeCanvas::force_inside() {
  // if its activity is present, force inside it
  
  QCanvasItemList all = the_canvas()->allItems();
  QCanvasItemList::Iterator cit;
  BrowserNode * parent = (BrowserNode *) browser_node->parent();

  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*cit);
      
      if ((di != 0) &&
	  IsaActivityContainer(di->type()) &&
	  (((ActivityContainerCanvas *) di)->get_bn() == parent)) {
	((ActivityContainerCanvas *) di)->force_inside(this, this);
	break;
      }
    }
  }
}

void ActivityNodeCanvas::set_xpm() {
  bool big = the_canvas()->zoom() >= 1.0;

  switch (browser_node->get_type()) {
  case InitialAN:
    xpm = (big) ? initialBigPixmap : initialPixmap;
    break;
  case ActivityFinalAN:
    xpm = (big) ? finalBigPixmap : finalPixmap;
    break;
  case FlowFinalAN:
    xpm = (big) ? exitPointBigPixmap : exitPointPixmap;
    break;
  case DecisionAN:
  case MergeAN:
    xpm = (the_canvas()->shadow() == 0)
      ? ((big) ? choiceBigPixmap : choicePixmap)
      : ((big) ? choiceShadowBigPixmap : choiceShadowPixmap);
    break;
  case ForkAN:
  case JoinAN:
    xpm = (horiz)
      ? ((big) ? joinForkHorizBigPixmap : joinForkHorizPixmap)
      : ((big) ? joinForkBigPixmap : joinForkPixmap);
    break;
  default:
    xpm = 0;
    return;
  }

  setSize(xpm->width(), xpm->height());
}

void ActivityNodeCanvas::change_scale() {
  // defined to not change size
  QCanvasRectangle::setVisible(FALSE);
  set_xpm();
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void ActivityNodeCanvas::modified() {
  hide();
  hide_lines();
  switch (browser_node->get_type()) {
  case DecisionAN:
  case MergeAN:
    set_xpm();
    break;
  default:
    break;
  }
  show();
  update_show_lines();
  force_self_rel_visible();
  if (the_canvas()->must_draw_all_relations())
    draw_all_flows();
  canvas()->update();
  if (label != 0)
    label->set_name(browser_node->get_name());
  package_modified();
}

void ActivityNodeCanvas::post_loaded() {
  force_self_rel_visible();
  if (the_canvas()->must_draw_all_relations())
    draw_all_flows();
}

void ActivityNodeCanvas::draw(QPainter & p) {
  if (!visible() || (xpm == 0)) return;

  QRect r = rect();
  
  p.setBackgroundMode(QObject::OpaqueMode);
  
  p.drawPixmap(r.topLeft(), *xpm);
    
  if (selected())
    show_mark(p, r);

  FILE * fp = svg();

  if (fp != 0) {
    bool big = the_canvas()->zoom() >= 1.0;
    int px = (int) x();
    int py = (int) y();

    switch (browser_node->get_type()) {
    case InitialAN:
      if (big)
	fprintf(fp, "\t<ellipse fill=\"black\" stroke=\"none\" cx=\"%d\" cy=\"%d\" rx=\"8.5\" ry=\"8.5\" />\n",
		px + 9, py + 9);
      else
	fprintf(fp, "\t<ellipse fill=\"black\" stroke=\"none\" cx=\"%d\" cy=\"%d\" rx=\"5.5\" ry=\"5.5\" />\n",
		px + 7, py + 7);
      break;
    case ActivityFinalAN:
      if (big) {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"11.5\" ry=\"11.5\" />\n",
		px + 12, py + 12);
	fprintf(fp, "\t<ellipse fill=\"black\" cx=\"%d\" cy=\"%d\" rx=\"8.5\" ry=\"8.5\" />\n"
		"</g>\n",
		px + 12, py + 12);
      }
      else {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"7.5\" ry=\"7.5\" />\n",
		px + 8, py + 8);
	fprintf(fp, "\t<ellipse fill=\"black\" cx=\"%d\" cy=\"%d\" rx=\"4.5\" ry=\"4.5\" />\n"
		"</g>\n",
		px + 8, py + 8);
      }
      break;
    case FlowFinalAN:
      if (big) {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"8.5\" ry=\"8.5\" />\n",
		px + 9, py + 9);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 4, py + 4, px + 14, py + 14);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 14, py + 4, px + 4, py + 14);
      }
      else {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"5.5\" ry=\"5.5\" />\n",
		px + 7, py + 7);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 4, py + 4, px + 10, py + 10);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 10, py + 4, px + 4, py + 10);
      }
      break;
    case DecisionAN:
    case MergeAN:
      // note : shadow not produced
      if (big)
	fprintf(fp, "<polygon fill=\"white\" stroke=\"black\" stroke-opacity=\"1\" points =\"%d,%d %d,%d %d,%d %d,%d\" />\n",
		px + 3, py + 18, px + 12, py + 1, px + 21, py + 18, px + 12, py + 35);
      else
	fprintf(fp, "<polygon fill=\"white\" stroke=\"black\" stroke-opacity=\"1\" points =\"%d,%d %d,%d %d,%d %d,%d\" />\n",
		px + 2, py + 12, px + 8, py + 1, px + 14, py + 12, px + 8, py + 23);
      break;
    case ForkAN:
    case JoinAN:
      if (horiz) {
	if (big)
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  px + 1, py + 7, px + 23, py + 7);
	else
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  px + 1, py + 7, px + 16, py + 7);
      }
      else {
	if (big)
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  px + 6, py + 1, px + 6, py + 24);
	else
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  px + 6, py + 1, px + 6, py + 17);
      }
      break;
    default:
      break;
    }
  }
}

UmlCode ActivityNodeCanvas::type() const {
  return browser_node->get_type();
}

void ActivityNodeCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool ActivityNodeCanvas::alignable() const {
  return TRUE;
}

bool ActivityNodeCanvas::copyable() const {
  return selected();
}

void ActivityNodeCanvas::open() {
  browser_node->open(TRUE);
}

void ActivityNodeCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  int index;
  QString s = browser_node->get_name();
  
  if (s.isEmpty()) {
    s = stringify(browser_node->get_type());
    int index = s.find("_");
    
    if (index != -1)
      s.replace(index, 1, " ");
  }
  
  m.insertItem(new MenuTitle(s, m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  switch (browser_node->get_type()) {
  case ForkAN:
  case JoinAN:
    m.insertItem((horiz) ? "draw vertically" : "draw horizontally", 2);
    m.insertSeparator();
    break;
  default:
    break;
  }
  /*m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();*/
  m.insertItem("Edit activity node", 3);
  m.insertSeparator();
  m.insertItem("Select in browser", 4);
  if (linked())
    m.insertItem("Select linked items", 5);
  m.insertSeparator();
  /*if (browser_node->is_writable())
    m.insertItem("Set associated diagram",6);
  m.insertSeparator();*/
  m.insertItem("Remove from view", 7);
  if (browser_node->is_writable())
    m.insertItem("Delete from model", 8);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, browser_node->get_type(), 10))
    m.insertItem("Tool", &toolm);
  
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
    horiz ^= TRUE;
    set_xpm();
    modified();	// call package_modified()
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
  case 7:
    //remove from view
    delete_it();
    break;
  case 8:
    //delete from model
    browser_node->delete_it();	// will delete the canvas
    break;
  default:
    if (index >= 10)
      ToolCom::run(Tool::command(index - 10), browser_node);
    return;
  }
  
  package_modified();
}

void ActivityNodeCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser") {
    browser_node->select_in_browser();
    return;
  }
  else if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else {
    browser_node->apply_shortcut(s);
    return;
  }

  modified();
  package_modified();
}

const char * ActivityNodeCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlFlow:
    return ((BrowserActivityNode *) browser_node)->may_start();
  case UmlDependOn:
    return "illegal";
  default: // anchor
    return 0;
  }
}

const char * ActivityNodeCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  if (l == UmlAnchor)
    return dest->may_start(l);
  else if(dest->get_bn() == 0)
    return "illegal";
  else
    return ((BrowserActivityNode *) browser_node)->may_connect(dest->get_bn());
}

void ActivityNodeCanvas::connexion(UmlCode action, DiagramItem * dest,
				   const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (action == UmlFlow)
    a = new FlowCanvas(the_canvas(), this, dest, 0, 0, -1.0, -1.0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE, -1.0, -1.0);
  
  a->show();
  the_canvas()->select(a);
}

void ActivityNodeCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "activitynodecanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "activitynodecanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    nl_indent(st);
    if (horiz)
      st << "horizontal ";
    save_xyz(st, this, "xyz");
    indent(-1);
  }
}

ActivityNodeCanvas * ActivityNodeCanvas::read(char * & st, UmlCanvas * canvas,
					      char * k)
{
  if (!strcmp(k, "activitynodecanvas_ref"))
    return ((ActivityNodeCanvas *) dict_get(read_id(st), "ActivityNodeCanvas", canvas));
  else if (!strcmp(k, "activitynodecanvas")) {
    int id = read_id(st);
    BrowserActivityNode * ps = BrowserActivityNode::read_ref(st);
    ActivityNodeCanvas * result = new ActivityNodeCanvas(canvas, id);
    
    result->browser_node = ps;
    connect(ps->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(ps->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));

    switch (result->browser_node->get_type()) {
    case DecisionAN:
    case MergeAN:
      connect(DrawingSettings::instance(), SIGNAL(changed()), result, SLOT(modified()));
      break;
    default:
      break;
    }

    k = read_keyword(st);
    
    if (!strcmp(k, "horizontal")) {
      result->horiz = TRUE;
      k = read_keyword(st);
    }
    
    if (strcmp(k, "xyz"))
      wrong_keyword(k, "xyz");
    
    read_xyz(st, result);
    result->set_xpm();
    result->set_center100();
    result->show();
    
    if (canvas->paste())
      result->remove_if_already_present();
    
    return result;
  }
  else 
    return 0;
}

void ActivityNodeCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(browser_node->get_data(), 0, this, 0);
}

void ActivityNodeCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}
