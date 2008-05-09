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





#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "PseudoStateCanvas.h"
#include "TransitionCanvas.h"
#include "SimpleRelationCanvas.h"
#include "SimpleData.h"
#include "BrowserPseudoState.h"
#include "LabelCanvas.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"

PseudoStateCanvas::PseudoStateCanvas(BrowserNode * bn, UmlCanvas * canvas,
				     int x, int y)
    : DiagramCanvas(0, canvas, x, y, 16, 16, 0), horiz(FALSE) {
  browser_node = bn;
  set_xpm();
   
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));

  if (canvas->must_draw_all_relations()) {
    //draw_all_simple_relations();
    draw_all_transitions();
  }

  if (!bn->allow_empty()) {
    double zoom = canvas->zoom();
    QFontMetrics fm(canvas->get_font(UmlNormalFont));    

    label = new LabelCanvas(bn->get_name(), canvas,
			    x + (((int) (width() * zoom)) - fm.width(bn->get_name()))/2,
			    y + ((int) (height() * zoom)));
  }
}

PseudoStateCanvas::PseudoStateCanvas(UmlCanvas * canvas, int id)
    : DiagramCanvas(canvas, id), horiz(FALSE) {
  // for read operation
  browser_node = 0;
  xpm = 0;
}

PseudoStateCanvas::~PseudoStateCanvas() {
}

void PseudoStateCanvas::delete_it() {
  disconnect(browser_node->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  
  DiagramCanvas::delete_it();
}

void PseudoStateCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void PseudoStateCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

void PseudoStateCanvas::set_xpm() {
  bool big = the_canvas()->zoom() >= 1.0;

  switch (browser_node->get_type()) {
  case InitialPS:
    xpm = (big) ? initialBigPixmap : initialPixmap;
    break;
  case EntryPointPS:
    xpm = (big) ? entryPointBigPixmap : entryPointPixmap;
    break;
  case FinalPS:
    xpm = (big) ? finalBigPixmap : finalPixmap;
    break;
  case TerminatePS:
    xpm = (big) ? terminateBigPixmap : terminatePixmap;
    break;
  case ExitPointPS:
    xpm = (big) ? exitPointBigPixmap : exitPointPixmap;
    break;
  case DeepHistoryPS:
    xpm = (big) ? deepHistoryBigPixmap : deepHistoryPixmap;
    break;
  case ShallowHistoryPS:
    xpm = (big) ? shallowHistoryBigPixmap : shallowHistoryPixmap;
    break;
  case JunctionPS:
    xpm = (big) ? junctionBigPixmap : junctionPixmap;
    break;
  case ChoicePS:
    xpm = (the_canvas()->shadow() == 0)
      ? ((big) ? choiceBigPixmap : choicePixmap)
      : ((big) ? choiceShadowBigPixmap : choiceShadowPixmap);
    break;
  case ForkPS:
  case JoinPS:
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

void PseudoStateCanvas::change_scale() {
  // defined to not change size
  QCanvasRectangle::setVisible(FALSE);
  set_xpm();
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void PseudoStateCanvas::modified() {
  hide();
  hide_lines();
  if (browser_node->get_type() == ChoicePS)
    set_xpm();
  show();
  update_show_lines();
  force_self_rel_visible();
  if (the_canvas()->must_draw_all_relations()) {
    //draw_all_simple_relations();
    draw_all_transitions();
  }
  canvas()->update();
  if (label != 0)
    label->set_name(browser_node->get_name());
  package_modified();
}

void PseudoStateCanvas::post_loaded() {
  force_self_rel_visible();
  if (the_canvas()->must_draw_all_relations()) {
    //draw_all_simple_relations();
    draw_all_transitions();
  }
}

void PseudoStateCanvas::connexion(UmlCode action, DiagramItem * dest,
				  const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (action == UmlTransition)
    a = new TransitionCanvas(the_canvas(), this, dest, 0, 0, -1.0, -1.0);
  else if (IsaSimpleRelation(action))
    a = new SimpleRelationCanvas(the_canvas(), this, dest, 0, action, 0, -1.0, -1.0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE, -1.0, -1.0);
  
  a->show();
  the_canvas()->select(a);
}

void PseudoStateCanvas::draw(QPainter & p) {
  if (!visible() || (xpm == 0)) return;

  QRect r = rect();
  
  p.setBackgroundMode(::Qt::OpaqueMode);
  
  p.drawPixmap(r.topLeft(), *xpm);
    
  if (selected())
    show_mark(p, r);
  
  FILE * fp = svg();
  
  if (fp != 0) {
    bool big = the_canvas()->zoom() >= 1.0;
    int px = (int) x();
    int py = (int) y();
    
    switch (browser_node->get_type()) {
    case InitialPS:
      if (big)
	fprintf(fp, "<ellipse fill=\"black\" cx=\"%d\" cy=\"%d\" rx=\"8.5\" ry=\"8.5\" />\n",
		px + 9, py + 9);
      else
	fprintf(fp, "<ellipse fill=\"black\" cx=\"%d\" cy=\"%d\" rx=\"5.5\" ry=\"5.5\" />\n",
		px + 7, py + 7);
      break;
    case EntryPointPS:
      if (big)
	fprintf(fp, "<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"8.5\" ry=\"8.5\" />\n",
		px + 9, py + 9);
      else
	fprintf(fp, "<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"5.5\" ry=\"5.5\" />\n",
		px + 7, py + 7);
      break;
    case FinalPS:
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
    case TerminatePS:
      if (big) {
	fprintf(fp, "<g>\n"
		"\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 1, py + 1, px + 19, py + 19);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 19, py + 1, px + 1, py + 19);
      }
      else{
	fprintf(fp, "<g>\n"
		"\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 2, py + 2, px + 12, py + 12);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 12, py + 2, px + 2, py + 12);
      }
      break;
    case ExitPointPS:
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
    case DeepHistoryPS:
      if (big) {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"12.5\" ry=\"12.5\" />\n",
		px + 12, py + 12);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 4, py + 8, px + 4, py + 16);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 10, py + 8, px + 10, py + 16);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 4, py + 12, px + 10, py + 12);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 12, py + 9, px + 20, py + 9);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 13, py + 6, px + 19, py + 12);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 13, py + 12, px + 19, py + 6);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 16, py + 5, px + 16, py + 13);
      }
      else {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"7.5\" ry=\"7.5\" />\n",
		px + 8, py + 8);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 4, py + 6, px + 4, py + 10);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 7, py + 6, px + 7, py + 10);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 4, py + 8, px + 7, py + 8);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 9, py + 7, px + 13, py + 7);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 9, py + 5, px + 13, py + 9);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 9, py + 9, px + 13, py + 5);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 11, py + 5, px + 11, py + 9);
      }
      break;
    case ShallowHistoryPS:
      if (big) {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"12.5\" ry=\"12.5\" />\n",
		px + 12, py + 12);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 8, py + 8, px + 8, py + 16);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 15, py + 8, px + 15, py + 16);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 8, py + 12, px + 15, py + 12);
      }
      else {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"7.5\" ry=\"7.5\" />\n",
		px + 8, py + 8);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 5, py + 5, px + 5, py + 11);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 11, py + 5, px + 11, py + 11);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 5, py + 8, px + 11, py + 8);
      }
      break;
    case JunctionPS:
      if (big)
	fprintf(fp, "<ellipse fill=\"black\" stroke=\"none\" cx=\"%d\" cy=\"%d\" rx=\"6.5\" ry=\"6.5\" />\n",
		px + 7, py + 7);
      else
	fprintf(fp, "<ellipse fill=\"black\" stroke=\"none\" cx=\"%d\" cy=\"%d\" rx=\"4.5\" ry=\"4.5\" />\n",
		px + 7, py + 7);
      break;
    case ChoicePS:
      // note : shadow not produced
      if (big)
	fprintf(fp, "<polygon fill=\"white\" stroke=\"black\" stroke-opacity=\"1\" points =\"%d,%d %d,%d %d,%d %d,%d\" />\n",
		px + 3, py + 18, px + 12, py + 1, px + 21, py + 18, px + 12, py + 35);
      else
	fprintf(fp, "<polygon fill=\"white\" stroke=\"black\" stroke-opacity=\"1\" points =\"%d,%d %d,%d %d,%d %d,%d\" />\n",
		px + 2, py + 12, px + 8, py + 1, px + 14, py + 12, px + 8, py + 23);
      break;
    case ForkPS:
    case JoinPS:
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

UmlCode PseudoStateCanvas::type() const {
  return browser_node->get_type();
}

void PseudoStateCanvas::delete_available(bool & in_model, bool & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool PseudoStateCanvas::alignable() const {
  return TRUE;
}

bool PseudoStateCanvas::copyable() const {
  return selected();
}

void PseudoStateCanvas::open() {
  browser_node->open(TRUE);
}

void PseudoStateCanvas::menu(const QPoint&) {
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
  m.insertItem("Go up", 13);
  m.insertItem("Go down", 14);
  m.insertSeparator();
  switch (browser_node->get_type()) {
  case ForkPS:
  case JoinPS:
    m.insertItem((horiz) ? "draw vertically" : "draw horizontally", 2);
    m.insertSeparator();
    break;
  default:
    break;
  }
  /*m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();*/
  m.insertItem("Edit pseudo state", 3);
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
  if (Tool::menu_insert(&toolm, browser_node->get_type(), 20))
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
  case 13:
    z_up();
    modified();	// call package_modified()
    return;
  case 14:
    z_down();
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
    if (index >= 20)
      ToolCom::run(Tool::command(index - 20), browser_node);
    return;
  }
  
  package_modified();
}

void PseudoStateCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser") {
    browser_node->select_in_browser();
    return;
  }
  else if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Go up")
    z_up();
  else if (s == "Go down")
    z_down();
  else {
    browser_node->apply_shortcut(s);
    return;
  }

  modified();
  package_modified();
}

const char * PseudoStateCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlAnchor:
    return 0;
  case UmlTransition:
    return ((BrowserPseudoState *) browser_node)->may_start();
  default:
    return "illegal";
  }
}

const char * PseudoStateCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  if (l == UmlAnchor)
    return dest->may_start(l);
  
  switch (dest->type()) {
  case UmlState:
  case UmlStateAction:
  case FinalPS:
  case TerminatePS:      
  case ExitPointPS:
  case DeepHistoryPS:
  case ShallowHistoryPS:
  case JunctionPS:
  case ChoicePS:
  case ForkPS:
  case JoinPS:
  case EntryPointPS:
    return (l == UmlTransition)
      ? ((BrowserPseudoState *) browser_node)->may_connect(dest->get_bn())
      : "illegal";
  default:
    return "illegal";
  }
}

void PseudoStateCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "pseudostatecanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "pseudostatecanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    nl_indent(st);
    if (horiz)
      st << "horizontal ";
    save_xyz(st, this, "xyz");
    if (label != 0)
      save_xy(st, label, " label_xy");
    indent(-1);
  }
}

PseudoStateCanvas * PseudoStateCanvas::read(char * & st, UmlCanvas * canvas,
					char * k)
{
  if (!strcmp(k, "pseudostatecanvas_ref"))
    return ((PseudoStateCanvas *) dict_get(read_id(st), "PseudoStateCanvas", canvas));
  else if (!strcmp(k, "pseudostatecanvas")) {
    int id = read_id(st);
    BrowserPseudoState * ps = BrowserPseudoState::read_ref(st);
    PseudoStateCanvas * result = new PseudoStateCanvas(canvas, id);
    
    result->browser_node = ps;
    connect(ps->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(ps->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));
    if (ps->get_type() == ChoicePS)
      connect(DrawingSettings::instance(), SIGNAL(changed()), result, SLOT(modified()));

    k = read_keyword(st);
    
    if (!strcmp(k, "horizontal")) {
      result->horiz = TRUE;
      k = read_keyword(st);
    }
    
    if (strcmp(k, "xyz"))
      wrong_keyword(k, "xyz");
    
    read_xyz(st, result);
    
    if (!ps->allow_empty()) {
      result->label = new LabelCanvas(ps->get_name(), canvas, 0, 0);
      read_keyword(st, "label_xy");
      read_xy(st, result->label);
      result->label->setZ(result->z());
      result->label->set_center100();
    }
    
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

void PseudoStateCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(browser_node->get_data(), 0, this, 0);
}

void PseudoStateCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}
