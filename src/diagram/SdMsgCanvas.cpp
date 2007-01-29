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

#include "SdMsgCanvas.h"
#include "SdDurationCanvas.h"
#include "OperationData.h"
#include "UmlCanvas.h"
#include "LabelCanvas.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "BrowserDiagram.h"
#include "myio.h"
#include "MenuTitle.h"

#define MSG_HEIGHT 11

SdMsgCanvas::SdMsgCanvas(UmlCanvas * canvas, SdDurationCanvas * s,
			 SdDurationCanvas * d, UmlCode l, int v, int id)
    : SdMsgBaseCanvas(canvas, d, l, v, id), start(s) {
  start->add(this);
  dest->add(this);
  
  update_hpos();
  set_center100();
  show();
}

SdMsgCanvas::~SdMsgCanvas() {
}

void SdMsgCanvas::delete_it() {
  start->remove(this);
  SdMsgBaseCanvas::delete_it();
}

void SdMsgCanvas::update_hpos() {
  const QRect sr = start->rect();
  const QRect dr = dest->rect();
  LabelCanvas * lbl = label;
  int cy = center_y_scale100;

  if (the_canvas()->do_zoom())
    // the label is moved independently
    label = 0;
  
  if (sr.left() < dr.left()) {
    DiagramCanvas::moveBy(sr.right() + 1 - x(), 0);
    setSize(dr.left() - sr.right(), MSG_HEIGHT);
  }
  else {
    DiagramCanvas::moveBy(dr.right() + 1 - x(), 0);
    setSize(sr.left() - dr.right(), MSG_HEIGHT);
  }
  center_y_scale100 = cy;	// updated later
  
  label = lbl;
}

double SdMsgCanvas::min_y() const {
  return (start->min_y() < dest->min_y())
    ? start->min_y()
    : dest->min_y();
}

void SdMsgCanvas::draw(QPainter & p) {
  const QRect r = rect();
  const int v = r.center().y();
  QPointArray poly(3);
  
  if (itsType == UmlReturnMsg)
    p.setPen(QObject::DotLine);
  
  p.drawLine(r.left(), v, r.right(), v);
  
  if (start->rect().left() < dest->rect().left()) {
    poly.setPoint(0, r.right(), v);
    poly.setPoint(1, r.right() - MSG_HEIGHT/2 + 1, v - MSG_HEIGHT/2 + 1);
    poly.setPoint(2, r.right() - MSG_HEIGHT/2 + 1, v + MSG_HEIGHT/2 - 1);
  }
  else {
    poly.setPoint(0, r.left(), v);
    poly.setPoint(1, r.left() + MSG_HEIGHT/2 - 1, v - MSG_HEIGHT/2 + 1);
    poly.setPoint(2, r.left() + MSG_HEIGHT/2 - 1, v + MSG_HEIGHT/2 - 1);
  }
    
  switch (itsType) {
  case UmlSyncMsg:
    {
      QBrush brsh = p.brush();
      
      p.setBrush(black);
      p.drawPolygon(poly/*, TRUE*/);
      p.setBrush(brsh);
    }
    break;
  case UmlReturnMsg:
    p.setPen(QObject::SolidLine);
    // no break !
  default:
    p.drawLine(poly.point(0), poly.point(1));
    p.drawLine(poly.point(0), poly.point(2));
  }
  
  if (selected())
    show_mark(p, r);
}

void SdMsgCanvas::update() {
  SdMsgBaseCanvas::update_after_move(start);
  SdMsgBaseCanvas::update_after_move(dest);
}

void SdMsgCanvas::change_duration(SdDurationCanvas * oldone,
				  SdDurationCanvas * newone) {
  if (oldone == start)
    start = newone;
  else
    dest = newone;
}

void SdMsgCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle("Message", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  m.insertSeparator();
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
  case 2:
    open();
    break;
  case 3:
    edit_drawing_settings();
    return;
  case 4:
    select_associated();
    return;
  case 5:
    the_canvas()->unselect_all();
    the_canvas()->select(label);
    return;
  case 6:
    default_label_position();
    break;
  case 7:
    delete_it();
    return;
  case 8:
    msg->get_browser_node()->select_in_browser();
    return;
  default:
    return;
  }

  package_modified();
  canvas()->update();
}

void SdMsgCanvas::apply_shortcut(QString s) {
  if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
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

void SdMsgCanvas::edit_drawing_settings() {
  QArray<StateSpec> st(2);
  
  st[0].set("operation drawing language", &drawing_language);
  st[1].set("show full operation definition", &show_full_oper);
  
  SettingsDialog dialog(&st, 0, FALSE, TRUE);
  
  dialog.raise();
  if (dialog.exec() == QDialog::Accepted)
    modified();
}

bool SdMsgCanvas::has_drawing_settings() const {
  return TRUE;
}

void SdMsgCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
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
	((SdMsgCanvas *) it.current())->drawing_language =
	  drawing_language;
      if (st[1].name != 0)
	((SdMsgCanvas *) it.current())->show_full_oper =
	  show_full_oper;
      ((SdMsgCanvas *) it.current())->modified();	// call package_modified()
    }
  }  
}

void SdMsgCanvas::select_associated() {
  the_canvas()->select(this);
  
  if (!start->selected())
    start->select_associated();
  
  if (!dest->selected())
    dest->select_associated();
}

bool SdMsgCanvas::copyable() const {
  // must not call start->copyable() else infinite loop
  return start->selected() && SdMsgBaseCanvas::copyable();
}

void SdMsgCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "msg_ref " << get_ident()
      << " // " << get_msg(FALSE);
  }
  else {
    nl_indent(st);
    st << "msg " << get_ident();
    switch (itsType) {
      case UmlSyncMsg:
	st << " synchronous";
	break;
      case UmlAsyncMsg:
	st << " asynchronous";
	break;
    default:
	st << " return";
    }
    indent(+1);
    nl_indent(st);
    st << "from ";
    start->save(st, TRUE, warning);
    SdMsgBaseCanvas::save(st, warning);
    indent(-1);
  }
}

SdMsgCanvas * SdMsgCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "msg_ref"))
    return ((SdMsgCanvas *) dict_get(read_id(st), "msg", canvas));
  else if (!strcmp(k, "msg")) {
    int id = read_id(st);
    UmlCode c;
    
    k = read_keyword(st);
    
    if (!strcmp(k, "synchronous"))
      c = UmlSyncMsg;
    else if (!strcmp(k, "asynchronous"))
      c = UmlAsyncMsg;
    else if (!strcmp(k, "return"))
      c = UmlReturnMsg;
    else {
      wrong_keyword(k, "synchronous/asynchronous/return");
      return 0; 	// to avoid warning
    }
    
    read_keyword(st, "from");

    SdDurationCanvas * start = SdDurationCanvas::read(st, canvas, TRUE);
    
    read_keyword(st, "to");

    SdDurationCanvas * dest = SdDurationCanvas::read(st, canvas, TRUE);
    
    k = read_keyword(st);

    SdMsgCanvas * result = 
      new SdMsgCanvas(canvas, start, dest, c, (int) read_double(st), id);

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
