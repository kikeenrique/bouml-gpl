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
#include <qmessagebox.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qptrdict.h>

#include "BrowserTransition.h"
#include "TransitionData.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "AType.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserTransition> BrowserTransition::all(1023);

BrowserTransition::BrowserTransition(BrowserNode * p, BrowserNode * end)
    : BrowserNode("<transition>", p), Labeled<BrowserTransition>(all, 0),
      def(new TransitionData) {
  def->set_start_end(this, end);
}

BrowserTransition::BrowserTransition(BrowserNode * p, TransitionData * d, int id)
    : BrowserNode("<transition>", p), Labeled<BrowserTransition>(all, id), def(d) {
}

BrowserTransition::BrowserTransition(const BrowserTransition * model,
				     BrowserNode * p)
    : BrowserNode(model->name, p),
      Labeled<BrowserTransition>(all, 0) {
  def = new TransitionData(model, this);
  comment = model->comment;
}

BrowserTransition::BrowserTransition(int id)
  : BrowserNode(), Labeled<BrowserTransition>(all, id) {
  // was deleted but still referenced
  // create a pseudo definition
  def = new TransitionData();
  
  def->set_start_end(this, this);
}

BrowserTransition::~BrowserTransition() {
  all.remove(get_ident());
  if (def)
    delete def;
}

BrowserNode * BrowserTransition::duplicate(BrowserNode * p, QString) {
  BrowserTransition * result = new BrowserTransition(this, p);

  result->def->get_start()->modified();
  result->def->get_start()->package_modified();
  result->update_stereotype();
  
  return result;
}

bool BrowserTransition::undelete(bool, QString & warning, QString & renamed) {
  if (! deletedp())
    return FALSE;
  
  if (def->get_start_node()->deletedp() ||
      def->get_end_node()->deletedp()) {
    warning += QString("<li><b>") + quote(name) + "</b> from <b>" +
      def->get_start_node()->full_name() +
	"</b> to <b>" + def->get_end_node()->full_name() + "</b>\n";
    return FALSE;
  }

  is_deleted = FALSE;
  def->undelete(warning, renamed);

  package_modified();
  repaint();
  
  return TRUE;
}

void BrowserTransition::compute_referenced_by(QList<BrowserNode> & l,
					      BrowserNode * target)
{
  IdIterator<BrowserTransition> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp() &&
	(it.current()->def->get_end_node() == target))
      l.append(it.current());
    ++it;
  }
}

void BrowserTransition::clear(bool old)
{
  all.clear(old);
}

void BrowserTransition::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserTransition::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

void BrowserTransition::update_stereotype(bool) {
  BasicData * data = get_data();
  
  if (data != 0) {
    QString s = name;
    const char * stereotype = data->get_stereotype();
    
    if (s.isEmpty())
      s = "<transition>";
    
    setText(0,
	    (show_stereotypes && stereotype[0])
	    ? QString("<<") + stereotype + ">> " + s
	    : s);
  }
}

const QPixmap* BrowserTransition::pixmap(int) const {
  return (deletedp()) ? DeletedRelationIcon
		      : SimpleRelationIcon;
}

QString BrowserTransition::str(bool horiz, DrawingLanguage lg) const {
  return def->str(horiz, lg);
}

void BrowserTransition::menu() {
  QString s = name;
  
  if (s.isEmpty())
    s = "transition";
  
  QPopupMenu m(0, s);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(s, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!in_edition()) {
      m.setWhatsThis(m.insertItem("Edit", 0),
		     "to edit the <em>transition</em>, \
a double click with the left mouse button does the same thing");
      if (!is_read_only && (edition_number == 0)) {
	m.setWhatsThis(m.insertItem("Delete", 2),
		       "to delete the <em>transition</em>. \
Note that you can undelete it after");
      }
      m.insertSeparator();
    }
    
    s = def->get_end_node()->get_name();
    
    if (s.isEmpty())
      s = stringify(def->get_end_node()->get_type());
    
    m.setWhatsThis(m.insertItem("Select " + s, 7),
		   "to select the destination");
    mark_menu(m, "transition", 90);
    if ((edition_number == 0) 
	&& Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("Undelete", 3),
		   "undelete the <em>transition</em> \
(except if the other side is also deleted)");
  
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserTransition::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    open(FALSE);
    break;
  case 2:
    delete_it();
    break;
  case 3:
    BrowserNode::undelete(FALSE);
    break;
  case 7:
    def->get_end_node()->select_in_browser();
    return;
  default:
    if (rank >= 100)
      ToolCom::run(Tool::command(rank - 100), this);
    else
      mark_management(rank - 90);
    return;
  }
  ((BrowserNode *) parent())->modified();
  package_modified();
}

void BrowserTransition::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (!in_edition()) {
      if (s == "Edit")
	choice = 0;
      if (!is_read_only && (edition_number == 0)) {
	if (s == "Delete")
	  choice = 2;
      }
    }
    if (s == "Select target")
      choice = 7;
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0))
    if (s == "Undelete")
    choice = 3;
  
  exec_menu_choice(choice);
}

bool BrowserTransition::in_edition() const {
  return (def->get_start()->is_edited);
}

void BrowserTransition::open(bool) {
  if (! in_edition())
    def->edit();
}

void BrowserTransition::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserTransition::get_type() const {
  return UmlTransition;
}

BasicData * BrowserTransition::get_data() const {
  return def;
}

QString BrowserTransition::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString s = (const char *) name;

  if (s.isEmpty())
    s = "<anonymous transition>";

  return (rev) ? s + "   [" + p + "]"
	       : p + "::" + s;
}

bool BrowserTransition::allow_empty() const {
  return TRUE;
}

bool BrowserTransition::same_name(const QString & s, UmlCode type) const {
  return ((get_type() == type) &&
	  (name == s) && 
	  (name != "<transition>"));
}

bool BrowserTransition::tool_cmd(ToolCom * com, const char * args) {
  return (def->tool_cmd(com, args, this, comment) ||
	  BrowserNode::tool_cmd(com, args));
}

void BrowserTransition::DragMoveEvent(QDragMoveEvent * e) {
  ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserTransition::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserTransition::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserTransition::drag_key() const {
  return QString::number(UmlTransition)
    + "#" + QString::number((unsigned long) parent());
}

QString BrowserTransition::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserTransition::drag_key(BrowserNode * p)
{
  return QString::number(UmlTransition)
    + "#" + QString::number((unsigned long) p);
}

void BrowserTransition::save(QTextStream & st, bool ref,
			     QString & warning) {
  if (ref) {
    // for TransitionCanvas
    st << "transition_ref " << get_ident() << " // " << get_name();
  }
  else {
    nl_indent(st);
    st << "transition " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
    BrowserNode::save(st);
    indent(-1);
    nl_indent(st);
    st << "end";
    
    // for saveAs
    if (! is_api_base())
      is_read_only = FALSE;
  }
}

BrowserTransition * BrowserTransition::read_ref(char * & st)
{
  read_keyword(st, "transition_ref");
  
  int id = read_id(st);
  BrowserTransition * result = all[id];
  
  return (result == 0)
    ? new BrowserTransition(id)
    : result;  
}

BrowserTransition *
  BrowserTransition::read(char * & st, char * k,
			  BrowserNode * parent)
{
  if (!strcmp(k, "transition")) {
    int id = read_id(st);    
    QString s = read_string(st);
    
    k = read_keyword(st);
    
    TransitionData * d = TransitionData::read(st, k);	// update k
    BrowserTransition * result;
    
    if ((result = all[id]) == 0)
      result = new BrowserTransition(parent, d, id);
    else {
      if (result->def != 0)
	// re-load
	delete result->def;
      result->def = d;
      result->set_parent(parent);
    }
    
    result->set_name(s);
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    result->BrowserNode::read(st, k);
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    d->set_browser_node(result);	// call update_stereotype();
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserTransition::get_it(const char * k, int id)
{
  return (!strcmp(k, "transition_ref")) ? all[id] : 0;
}
