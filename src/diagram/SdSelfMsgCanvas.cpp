// *************************************************************************
//
// Copyleft 2004-2009 Bruno PAGES  .
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
#include <qcursor.h>
#include <qpopupmenu.h> 

#include "SdSelfMsgCanvas.h"
#include "SdDurationCanvas.h"
#include "OperationData.h"
#include "UmlCanvas.h"
#include "LabelCanvas.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "BrowserSeqDiagram.h"
#include "myio.h"
#include "MenuTitle.h"

#define SELF_MSG_WIDTH  20
#define SELF_MSG_HEIGHT 14

SdSelfMsgCanvas::SdSelfMsgCanvas(UmlCanvas * canvas, SdDurationCanvas * d,
				 UmlCode l, int v, int id)
    : SdMsgBaseCanvas(canvas, d, l, v + 1, id) {
  dest->add(this);
  update_hpos();
  setSize(SELF_MSG_WIDTH, SELF_MSG_HEIGHT);
  check_vpos(d->rect());
  set_center100();
  show();
}

SdSelfMsgCanvas::~SdSelfMsgCanvas() {
}

void SdSelfMsgCanvas::update_hpos() {
  LabelCanvas * lbl = label;
  int cy = center_y_scale100;

  if (the_canvas()->do_zoom())
    // the label is moved independently
    label = 0;
  
  DiagramCanvas::moveBy(dest->rect().right() + 1 - x(), 0);
  
  label = lbl;
  center_y_scale100 = cy;	// updated later
}

void SdSelfMsgCanvas::check_vpos(const QRect & r) {
  if (((BrowserSeqDiagram *) the_canvas()->browser_diagram())
      ->is_overlapping_bars()) {
    double v;
    
    switch (itsType) {
    case UmlSelfReturnMsg:
      v = r.bottom() - height() / 2;
      break;
    case UmlSyncSelfMsg:
      v = r.top();
      break;
    default:
      return;
    }
    
    if (y() != v)
      DiagramCanvas::moveBy(0, v - y());
  }
}

double SdSelfMsgCanvas::min_y() const {
  return dest->min_y();
}

void SdSelfMsgCanvas::draw(QPainter & p) {
  const QRect r = rect();
  int ah = (r.height() - 1 - 1 - 2 - 1 - 1)/2;
  int he = r.top() + 1 + 2 + ah + 1;
  FILE * fp = svg();
  
  if (itsType == UmlSelfReturnMsg)
    p.setPen(::Qt::DotLine);
  
  p.drawLine(r.left() + 1, r.top() + 1, r.right() - 1, r.top() + 1);



  p.lineTo(r.right() - 1, he);
  p.lineTo(r.left() + 1, he);

  if (fp != 0) {
    fputs("<g>\n\t<path fill=\"none\" stroke=\"black\" stroke-opacity=\"1\"", fp);
    if (itsType == UmlSelfReturnMsg)
      fputs(" stroke-dasharray=\"4,4\"", fp);
    fprintf(fp, " d=\"M %d %d L %d %d L %d %d L %d %d\" />\n",
	    r.left() + 1, r.top() + 1, r.right() - 1, r.top() + 1,
	    r.right() - 1, he,
	    r.left() + 1, he);
  }
  
  if (itsType == UmlSyncSelfMsg) {
    QPointArray poly(3);
    QBrush brsh = p.brush();
    
    p.setBrush(black);
    poly.setPoint(0, r.left() + 1, he);
    poly.setPoint(1, r.left() + 1 + ah, he + ah);
    poly.setPoint(2, r.left() + 1 + ah, he - ah);
    p.drawPolygon(poly/*, TRUE*/);
    p.setBrush(brsh);

    if (fp != 0) {
      draw_poly(fp, poly, UmlBlack, FALSE);
      fputs("</g>\n", fp);
    }
  }
  else {
    if (itsType == UmlSelfReturnMsg)
      p.setPen(::Qt::SolidLine);
    
    p.lineTo(r.left() + 1 + ah, he + ah);
    p.drawLine(r.left() + 1, he, r.left() + 1 + ah, he - ah);

    if (fp != 0)
      fprintf(fp, "\t<path fill=\"none\" stroke=\"black\" stroke-opacity=\"1\""
	      " d=\"M %d %d L %d %d L %d %d\" />\n"
	      "</g>\n",
	      r.left() + 1 + ah, he + ah,
	      r.left() + 1, he,
	      r.left() + 1 + ah, he - ah);
  }
  
  if (selected())
    show_mark(p, r);
}

void SdSelfMsgCanvas::update() {
  if (((BrowserSeqDiagram *) the_canvas()->browser_diagram())
      ->is_overlapping_bars()) {
    switch (itsType) {
    case UmlSelfReturnMsg:
    case UmlSyncSelfMsg:
      check_vpos(dest->rect());
      return;
    default:
      break;
    }
  }
  
  SdMsgBaseCanvas::update_after_move(dest);
}

void SdSelfMsgCanvas::change_duration(SdDurationCanvas *,
				      SdDurationCanvas * newone) {
  dest = newone;
}

int SdSelfMsgCanvas::overlap_dir(SdDurationCanvas *) const {
  switch (itsType) {
  case UmlSelfReturnMsg:
    return -1;
  case UmlSyncSelfMsg:
    return 1;
  default:
    // async
    return 2;
  }
}

void SdSelfMsgCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Message", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertItem("Go up", 13);
  m.insertItem("Go down", 14);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  m.insertItem("Edit drawing settings", 3);
  m.insertSeparator();
  if (msg != 0)
    m.insertItem("Select operation in browser", 8);
  m.insertItem("Select linked items", 4);
  if (label) {
    m.insertSeparator();
    m.insertItem("Select label", 5);
    m.insertItem("Label default position", 6);
  }
  if (((BrowserSeqDiagram *) the_canvas()->browser_diagram())
      ->is_overlapping_bars()) {
    m.insertSeparator();
    m.insertItem("Go to new overlapping bar", 9);
    if (dest->isOverlappingDuration())
      m.insertItem("Go to parent bar", 10);
  }
  m.insertSeparator();
  m.insertItem("Remove from view", 7);

  switch (m.exec(QCursor::pos())) {
  case 0:
    upper();
    // force son reaffichage
    hide();
    show();
    break;
  case 1:
    lower();
    // force son reaffichage
    hide();
    show();
    break;
  case 13:
    z_up();
    // force son reaffichage
    hide();
    show();
    break;
  case 14:
    z_down();
    // force son reaffichage
    hide();
    show();
    break;
  case 2:
    open();
    break;
  case 3:
    edit_drawing_settings();
    return;
  case 4:
    select_associated();
    break;
  case 5:
    the_canvas()->unselect_all();
    the_canvas()->select(label);
    break;
  case 6:
    default_label_position();
    break;
  case 7:
    delete_it();
    return;
  case 8:
    msg->get_browser_node()->select_in_browser();
    return;
  case 9:
    dest->go_up(this, TRUE);
    break;
  case 10:
    dest->go_down(this);
  default:
    return;
  }

  package_modified();  
  canvas()->update();
}

void SdSelfMsgCanvas::apply_shortcut(QString s) {
  if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Go up")
    z_up();
  else if (s == "Go down")
    z_down();
  else if (s == "Edit drawing settings") {
    edit_drawing_settings();
    return;
  }
  else if (s == "Edit")
    open();
  else 
    return;

  // force son reaffichage
  hide();
  show();
  package_modified();
  canvas()->update();
}

void SdSelfMsgCanvas::edit_drawing_settings() {
  QArray<StateSpec> st(2);
  
  st[0].set("operation drawing language", &drawing_language);
  st[1].set("show full operation definition", &show_full_oper);
  
  SettingsDialog dialog(&st, 0, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();
}

bool SdSelfMsgCanvas::has_drawing_settings() const {
  return TRUE;
}

void SdSelfMsgCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<StateSpec> st(2);
  DrawingLanguage drawing_language;
  Uml3States show_full_oper;
  
  st[0].set("operation drawing language", &drawing_language);
  st[1].set("show full operation definition", &show_full_oper);
  
  SettingsDialog dialog(&st, 0, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      if (st[0].name != 0)
	((SdSelfMsgCanvas *) it.current())->drawing_language =
	  drawing_language;
      if (st[1].name != 0)
	((SdSelfMsgCanvas *) it.current())->show_full_oper =
	  show_full_oper;
      ((SdSelfMsgCanvas *) it.current())->modified();	// call package_modified()
    }
  }  
}

void SdSelfMsgCanvas::select_associated() {
  the_canvas()->select(this);
  
  if (!dest->selected())
    dest->select_associated();
}

void SdSelfMsgCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << ((itsType == UmlSelfReturnMsg)
	   ? "selfreflexivemsg_ref "
	   : "reflexivemsg_ref ")
      << get_ident() << " // " << get_msg(FALSE);
  }
  else {
    nl_indent(st);
    switch (itsType) {
    case UmlSelfReturnMsg:
      st << "selfreflexivemsg " << get_ident();
      break;
    case UmlSyncSelfMsg:
    st << "reflexivemsg " << get_ident() << " synchronous";
      break;
    default:
      st << "reflexivemsg " << get_ident() << " asynchronous";
    }
    indent(+1);
    SdMsgBaseCanvas::save(st, warning);
    indent(-1);
  }
}

SdSelfMsgCanvas * SdSelfMsgCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "reflexivemsg_ref"))
    return ((SdSelfMsgCanvas *) dict_get(read_id(st), "reflexivemsg", canvas));
  if (!strcmp(k, "selfreflexivemsg_ref"))
    return ((SdSelfMsgCanvas *) dict_get(read_id(st), "selfreflexivemsg", canvas));
  else if (!strcmp(k, "reflexivemsg") || !strcmp(k, "selfreflexivemsg")) {
    int id = read_id(st);
    UmlCode c;
    
    if (*k == 's')
      c = UmlSelfReturnMsg;
    else {
      k = read_keyword(st);
      
      if (!strcmp(k, "synchronous"))
	c = UmlSyncSelfMsg;
      else if (!strcmp(k, "asynchronous"))
	c = UmlAsyncSelfMsg;
      else {
	wrong_keyword(k, "synchronous/asynchronous");
	return 0; 	// to avoid warning
      }
    }
    
    read_keyword(st, "to");

    SdDurationCanvas * d = SdDurationCanvas::read(st, canvas, TRUE);
    
    k = read_keyword(st);

    SdSelfMsgCanvas * result = 
      new SdSelfMsgCanvas(canvas, d, c, (int) read_double(st) - 1, id);

    if (!strcmp(k, "yz"))
      // new version
      result->setZ(read_double(st));
    else if (strcmp(k, "y"))
      wrong_keyword(k, "y/yz");
    
    result->SdMsgBaseCanvas::read(st);
    
    return result;
  }
  else
    return 0;
}
