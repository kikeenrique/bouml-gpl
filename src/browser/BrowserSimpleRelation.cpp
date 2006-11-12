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

#include "BrowserSimpleRelation.h"
#include "SimpleRelationData.h"
#include "BrowserUseCase.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "AType.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserSimpleRelation> BrowserSimpleRelation::all(257);

BrowserSimpleRelation::BrowserSimpleRelation(BrowserNode * p, SimpleRelationData * d, int id)
    : BrowserNode(d->definition(FALSE), p), Labeled<BrowserSimpleRelation>(all, id),
      def(d) {
}

BrowserSimpleRelation::BrowserSimpleRelation(const BrowserSimpleRelation * model,
					     BrowserNode * p)
    : BrowserNode(model->def->definition(FALSE), p),
      Labeled<BrowserSimpleRelation>(all, 0) {
  def = new SimpleRelationData(model, this);
  comment = model->comment;
}

BrowserSimpleRelation::BrowserSimpleRelation(int id)
  : BrowserNode(), Labeled<BrowserSimpleRelation>(all, id) {
  // was deleted but still referenced
  // create a pseudo definition
  def = new SimpleRelationData(UmlDependOn);
  
  def->set_start_end(this, this);
}

BrowserSimpleRelation::~BrowserSimpleRelation() {
  all.remove(get_ident());
  if (def)
    delete def;
}

BrowserNode * BrowserSimpleRelation::duplicate(BrowserNode * p, QString) {
  BrowserSimpleRelation * result = new BrowserSimpleRelation(this, p);

  result->def->get_start()->modified();
  result->def->get_start()->package_modified();
  result->update_stereotype();
  
  return result;
}

bool BrowserSimpleRelation::undelete(bool, QString & warning, QString & renamed) {
  if (! deletedp())
    return FALSE;
  
  if (def->get_start_node()->deletedp() ||
      def->get_end_node()->deletedp()) {
    warning += QString("<li><b>") + quote(def->definition(FALSE)) + "</b> from <b>" +
      def->get_start_node()->full_name() +
	"</b> to <b>" + def->get_end_node()->full_name() + "</b>\n";
    return FALSE;
  }
  else {
    switch (get_type()) {
    case UmlInherit:
      // use case
      if (def->get_start_node()->check_inherit(def->get_end_node()) != 0) {
	warning += QString("<li><b>") + quote(def->definition(FALSE)) +
	  "</b> because <b>" + def->get_start_node()->full_name() +
	    "</b> cannot (or already) inherit on <b>" +
	      def->get_end_node()->full_name() + "</b>\n";
	return FALSE;
      }
      break;
    default:
      break;
    }
  }

  is_deleted = FALSE;
  def->undelete(warning, renamed);

  package_modified();
  repaint();
  
  return TRUE;
}

void BrowserSimpleRelation::compute_referenced_by(QList<BrowserNode> & l,
						  BrowserNode * target)
{
  IdIterator<BrowserSimpleRelation> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp() &&
	(it.current()->def->get_end_node() == target))
      l.append(it.current());
    ++it;
  }
}

void BrowserSimpleRelation::clear(bool old)
{
  all.clear(old);
}

void BrowserSimpleRelation::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserSimpleRelation::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

void BrowserSimpleRelation::update_stereotype(bool) {
  BasicData * data = get_data();
  
  if (data != 0) {
    QString s = def->definition(FALSE) + " " +
      def->get_end_node()->get_name();
    const char * stereotype = data->get_stereotype();
    
    setText(0,
	    (show_stereotypes && stereotype[0])
	    ? QString("<<") + stereotype + ">> " + s
	    : s);
  }
}

const QPixmap* BrowserSimpleRelation::pixmap(int) const {
  return (deletedp()) ? DeletedRelationIcon
		      : SimpleRelationIcon;
}

void BrowserSimpleRelation::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!in_edition()) {
      m.setWhatsThis(m.insertItem("edit", 0),
		     "to edit the <em>relation</em>, \
a double click with the left mouse button does the same thing");
      if (!is_read_only && (edition_number == 0)) {
	m.setWhatsThis(m.insertItem("delete", 2),
		       "to delete the <em>relation</em>. \
Note that you can undelete it after");
      }
      m.insertSeparator();
    }
    m.setWhatsThis(m.insertItem(QString("select ") + def->get_end_node()->get_name(),
				7),
		   "to select the destination");
    mark_menu(m, "relation", 90);
    if ((edition_number == 0) 
	&& Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("undelete", 3),
		   "undelete the <em>relation</em> \
(except if the other side is also deleted)");
  
  int rank = m.exec(QCursor::pos());
  
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

bool BrowserSimpleRelation::in_edition() const {
  return (def->get_start()->is_edited);
}

void BrowserSimpleRelation::open(bool) {
  if (! in_edition())
    def->edit();
}

void BrowserSimpleRelation::modified() {
  set_name(def->definition(FALSE));
  update_stereotype(FALSE);
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserSimpleRelation::get_type() const {
  return def->get_type();
}

BasicData * BrowserSimpleRelation::get_data() const {
  return def;
}

QString BrowserSimpleRelation::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString n = (const char *) name;

  return (rev) ? n + "   [" + p + "]"
	       : p + "::" + n;
}

void BrowserSimpleRelation::write_id(ToolCom * com) {
  com->write_id(this, UmlSimpleRelations - UmlRelations, name);
}

bool BrowserSimpleRelation::tool_cmd(ToolCom * com, const char * args) {
  return (def->tool_cmd(com, args, this, comment) ||
	  BrowserNode::tool_cmd(com, args));
}

void BrowserSimpleRelation::DragMoveEvent(QDragMoveEvent * e) {
  ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserSimpleRelation::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserSimpleRelation::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserSimpleRelation::drag_key() const {
  return QString::number(UmlSimpleRelations)
    + "#" + QString::number((unsigned long) parent());
}

QString BrowserSimpleRelation::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserSimpleRelation::drag_key(BrowserNode * p)
{
  return QString::number(UmlSimpleRelations)
    + "#" + QString::number((unsigned long) p);
}

void BrowserSimpleRelation::save(QTextStream & st, bool ref,
				 QString & warning) {
  if (ref) {
    // for SimpleRelationCanvas
    st << "simplerelation_ref " << get_ident();
  }
  else {
    nl_indent(st);
    st << "simplerelation " << get_ident();
    indent(+1);
    nl_indent(st);
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

BrowserSimpleRelation * BrowserSimpleRelation::read_ref(char * & st)
{
  read_keyword(st, "simplerelation_ref");
  
  int id = read_id(st);
  BrowserSimpleRelation * result = all[id];
  
  return (result == 0)
    ? new BrowserSimpleRelation(id)
    : result;  
}

BrowserSimpleRelation *
  BrowserSimpleRelation::read(char * & st, char * k,
			      BrowserNode * parent)
{
  if (!strcmp(k, "simplerelation")) {
    int id = read_id(st);
    
    SimpleRelationData * d = SimpleRelationData::read(st);
    BrowserSimpleRelation * result;
    
    if ((result = all[id]) == 0)
      result = new BrowserSimpleRelation(parent, d, id);
    else {
      if (result->def != 0)
	// re-load
	delete result->def;
      result->def = d;
      result->set_parent(parent);
      result->set_name(d->definition(FALSE));
    }
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    k = read_keyword(st);
    
    result->BrowserNode::read(st, k);
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    d->set_browser_node(result);	// call update_stereotype();
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserSimpleRelation::get_it(const char * k, int id)
{
  return (!strcmp(k, "simplerelation_ref")) ? all[id] : 0;
}
