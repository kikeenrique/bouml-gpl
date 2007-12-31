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

#include <qcursor.h>
#include <qinputdialog.h> 

#include "SdMsgBaseCanvas.h"
#include "SdDurationCanvas.h"
#include "SdObjCanvas.h"
#include "OperationData.h"
#include "UmlCanvas.h"
#include "SdLifeLineCanvas.h"
#include "SdMsgDialog.h"
#include "LabelCanvas.h"
#include "Settings.h"
#include "BrowserClass.h"
#include "BrowserOperation.h"
#include "BrowserDiagram.h"
#include "myio.h"
#include "ToolCom.h"
#include "../Tools/aMessageKind.h"

SdMsgBaseCanvas::SdMsgBaseCanvas(UmlCanvas * canvas, SdDurationCanvas * d,
				 UmlCode l, int v, int id)
    : DiagramCanvas(0, canvas, 0, v, 0, 0, id),
      dest(d), msg(0), itsType(l), already_moved(FALSE),
      show_full_oper(UmlDefaultState),
      drawing_language(DefaultDrawingLanguage) {
  browser_node = canvas->browser_diagram();
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
}

SdMsgBaseCanvas::~SdMsgBaseCanvas() {
}

void SdMsgBaseCanvas::delete_it() {
  dest->remove(this);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  if (msg != 0)
    disconnect(msg, 0, this, 0);
  DiagramCanvas::delete_it();
}

UmlCode SdMsgBaseCanvas::type() const {
  return itsType;
}

void SdMsgBaseCanvas::delete_available(bool &, bool & out_model) const {
  out_model |= TRUE;
}

QString SdMsgBaseCanvas::get_msg(bool with_args) const {
  QString m;

  if (msg == 0)
    m = (const char *) explicit_msg;
  else {
    SequenceDiagramSettings dflt;
      
    dflt.show_full_operations_definition = show_full_oper;
    dflt.drawing_language = drawing_language;
    the_canvas()->browser_diagram()->get_sequencediagramsettings(dflt);
    
    m = msg->definition(dflt.show_full_operations_definition == UmlYes,
			dflt.drawing_language, TRUE, TRUE);
  }
  
  if (!with_args || args.isEmpty())
    return m;
  
  QString a = (const char *) args;
  int index1 = m.find('(');
  int index2 = m.findRev(')');
  
  if ((index1 == -1) || (index2 == -1) || (index2 < index1))
    return m;
  
  if (a.find('\n') == -1)
    return m.left(index1 + 1) + a + m.mid(index2);
  
  // multi lines args, compute indent
  
  QString indent = "  ";
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  int w = fm.width(m.left(index1 + 1));
  
  while (fm.width(indent) < w)
    indent += ' ';
  
  QString s = m.left(index1 + 1);
  
  index1 = 0;
  
  int index3;
  
  while ((index3 = a.find('\n', index1)) != -1) {
    s += a.mid(index1, index3 - index1 + 1) + indent;
    index1 = index3 + 1;
  }
  
  return s + a.mid(index1) + m.mid(index2);
}

void SdMsgBaseCanvas::set_msg(const OperationData * m, const QString & e,
			      const QString & a) {
  if (m != msg) {
    if (msg != 0)
      disconnect(msg, 0, this, 0);
    if ((msg = m) != 0) {
      connect(msg, SIGNAL(changed()), this, SLOT(modified()));
      connect(msg, SIGNAL(deleted()), this, SLOT(modified()));
    }
  }
  
  explicit_msg = e;
  args = a;
}

bool SdMsgBaseCanvas::is_synchronous() const {
  switch (itsType) {
  case UmlAsyncMsg:
  case UmlAsyncSelfMsg:
    return FALSE;
  default:
    return TRUE;
  }
}

void SdMsgBaseCanvas::set_synchronous(bool yes) {
  switch (itsType) {
  case UmlSyncMsg:
    if (! yes)
      itsType = UmlAsyncMsg;
    break;
  case UmlAsyncMsg:
    if (yes)
      itsType = UmlSyncMsg;
    break;
  case UmlSyncSelfMsg:
    if (! yes)
      itsType = UmlAsyncSelfMsg;
    break;
  case UmlAsyncSelfMsg:
    if (yes)
      itsType = UmlSyncSelfMsg;
    break;
  default:
    // to avoid compiler warning
    break;
  }
}

const char * SdMsgBaseCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : "illegal";
}

const char * SdMsgBaseCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  return (l == UmlAnchor) ? dest->may_start(l) : "illegal";
}

bool SdMsgBaseCanvas::is_decenter(const QPoint &, bool &) const {
  return FALSE;
}

void SdMsgBaseCanvas::open() {
  switch (itsType) {
  case UmlReturnMsg:
  case UmlSelfReturnMsg:
    {
      bool ok = FALSE;
      QString s =
	QInputDialog::getText("Return", "value :", QLineEdit::Normal,
			      (const char *) explicit_msg, &ok);
      
      s = s.stripWhiteSpace();
      
      if (ok && (s != (const char *) explicit_msg)) {
	explicit_msg = s;
	modified();
      }
    }
    break;
  default:
    {
      SdMsgDialog d(this);
      
      if (d.exec() == QDialog::Accepted)
	modified();
    }
    break;
  }
}

void SdMsgBaseCanvas::modified() {
  if (visible()) {
    hide();
    
    QString s = get_msg(TRUE);
    
    if (s.isEmpty()) {
      // remove label if needed
      if (label != 0) {
	the_canvas()->del(label);
	label = 0;
      }
    }
    else if (label == 0) {
      // adds label
      label = new LabelCanvas(s, the_canvas(), 0, 0, FALSE, FALSE, FALSE, FALSE);
      default_label_position();
      label->show();
    }
    else if (label->get_name() != s)
      // update label
      label->set_name(s);
    else
      label->set_strikeout((msg != 0) && (msg->deletedp()));
    
    package_modified();
  
    show();
    canvas()->update();
  }
}

void SdMsgBaseCanvas::default_label_position() const {
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  QSize sz = fm.size(0, label->get_name());
      
  label->move(rect().center().x() - sz.width()/2,
	      y() - fm.height());
}

void SdMsgBaseCanvas::moveBy(double dx, double dy) {
  if (dy > 80000)
    // horizontal moving due to the life line moving
    DiagramCanvas::moveBy(dx, 0);
  else if (!already_moved && ((y() + dy) > min_y())) {
    // vertical moving due to the duration moving or user moving
    already_moved = TRUE;
    DiagramCanvas::moveBy(0, dy);
  }
}

void SdMsgBaseCanvas::update_after_move(SdDurationCanvas * p) {
  already_moved = FALSE;

  p->update_v_to_contain(rect());
}

void SdMsgBaseCanvas::change_scale() {
  // Just update the vertical position, the horizontal size
  // and position are update by the durations.
  // The label is also moved independently
  QCanvasRectangle::setVisible(FALSE);
  
  QCanvasRectangle::moveBy(0,
			   ((int) (center_y_scale100 * the_canvas()->zoom() + 0.5))
			   - center().y());
  QCanvasRectangle::setVisible(TRUE);
}

bool SdMsgBaseCanvas::copyable() const {
  // must not call dest->copyable() else infinite loop
  return selected() && dest->selected();
}

void SdMsgBaseCanvas::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  st << "to ";
  dest->save(st, TRUE, warning);
  nl_indent(st);
  st << "yz " << (int) y() << ' ' << (int) z();
  if (msg != 0) {
    if (msg->deletedp()) {
      warning += QString("<b>") + the_canvas()->browser_diagram()->full_name() +
	"</b> reference the class <b>" +
	  dest->get_line()->get_obj()->get_class()->full_name() +
	    "</b> deleted operation <b>" + msg->definition(TRUE) + "</b><br>\n<br>\n";
      if (warning[0] == '!') {
	st << " msg ";
	msg->save(st, TRUE, warning);
      }
      else {
	st << " explicitmsg ";
	save_string(msg->get_browser_node()->get_name(), st);
      }
    }
    else {
      st << " msg ";
      msg->save(st, TRUE, warning);
    }
  }
  else if (! explicit_msg.isEmpty()) {
    st << " explicitmsg ";
    save_string(explicit_msg, st);
  }
  else
    st << " unspecifiedmsg";
  
  nl_indent(st);
  st << "show_full_operations_definition " << stringify(show_full_oper)
    << " drawing_language " << stringify(drawing_language);
  
  if (label != 0) {
    if (! args.isEmpty()) {
      nl_indent(st);
      st << "args ";
      save_string(args, st);
    }
    nl_indent(st);
    save_xy(st, label, "label_xy");
  }
}

void SdMsgBaseCanvas::read(char * & st) {
  if (((itsType != UmlReturnMsg) && (itsType != UmlSelfReturnMsg)) ||
      (read_file_format() >= 4)) {
    char * k = read_keyword(st);
    const OperationData * method = 0;
    QString expl;
    QString args;
    bool unspec = FALSE;
    
    if (!strcmp(k, "msg")) {
      if ((method = OperationData::read_ref(st)) == 0)
	expl = "<deleted operation>";
    }
    else if (!strcmp(k, "explicitmsg"))
      expl = read_string(st);
    else if (strcmp(k, "unspecifiedmsg"))
      wrong_keyword(k, "msg/explicitmsg/unspecifiedmsg");
    else
      unspec = TRUE;
    
    k = read_keyword(st);
    
    if (!strcmp(k, "show_full_operations_definition")) {
      show_full_oper = state(read_keyword(st));
      k = read_keyword(st);
    }
    if (!strcmp(k, "drawing_language")) {
      drawing_language = ::drawing_language(read_keyword(st));
      k = read_keyword(st);
    }
    
    if (unspec) {
      unread_keyword(k, st);
      return;
    }
    
    if (!strcmp(k, "args")) {
      args = read_string(st);
      k = read_keyword(st);
    }
    
    set_msg(method, expl, args);
    
    if (strcmp(k, "label_xy"))
      wrong_keyword(k, "label_xy");
    
    int lx = (int) read_double(st);
    
    label =
      new LabelCanvas(get_msg(TRUE), the_canvas(),
		      lx, (int) read_double(st),
		      FALSE, FALSE, FALSE, FALSE);
    label->show();
  }
}

void SdMsgBaseCanvas::history_hide() {
  DiagramCanvas::setVisible(FALSE);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  if (msg != 0)
    disconnect(msg, 0, this, 0);
}

void SdMsgBaseCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  if (msg != 0) {
    connect(msg, SIGNAL(changed()), this, SLOT(modified()));
    connect(msg, SIGNAL(deleted()), this, SLOT(modified()));
  }
}

// for plug out

void SdMsgBaseCanvas::send_implicit_return(ToolCom * com, int fromid,
					   unsigned x, unsigned y)
{
  com->write_id(0);
  com->write_string("");
  com->write_unsigned((unsigned) fromid);
  com->write_unsigned((unsigned) fromid);
  
  com->write_char(anImplicitReturn);
  com->write_string("");
  com->write_unsigned(x);
  com->write_unsigned(y);
  com->write_unsigned(y);
}

void SdMsgBaseCanvas::send(ToolCom * com, int fromid) const {
  if (msg == 0) {
    com->write_id(0);
    com->write_string(explicit_msg);
  }
  else if (msg->deletedp()) {
    com->write_id(0);
    com->write_string(msg->get_browser_node()->get_name());
  }
  else 
    msg->get_browser_node()->write_id(com);
  com->write_unsigned((unsigned) fromid);
  com->write_unsigned((unsigned) dest->get_line()->get_obj()->get_ident());
  
  switch (itsType) {
  case UmlSyncMsg:
  case UmlSyncSelfMsg:
    com->write_char(aSynchronousCall);
    break;
  case UmlAsyncMsg:
  case UmlAsyncSelfMsg:
    com->write_char(anAsynchronousCall);
    break;
  default:
    com->write_char(anExplicitReturn);
    break;
  }

  com->write_string((const char *) args);
  com->write_unsigned((unsigned) x());
  
  unsigned v = (unsigned) y() + height()/2;
  
  com->write_unsigned(v);
  com->write_unsigned(v);
}
