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
#include <qpainter.h>
#include <qcursor.h>

#include "BrowserParameterSet.h"
#include "ParameterSetData.h"
#include "BrowserActivityAction.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserParameterSet> BrowserParameterSet::all(257);
QStringList BrowserParameterSet::its_default_stereotypes;	// unicode

BrowserParameterSet::BrowserParameterSet(QString s, BrowserNode * p, ParameterSetData * d, int id)
    : BrowserNode(s, p), Labeled<BrowserParameterSet>(all, id), def(d) {
}

BrowserParameterSet::BrowserParameterSet(int id)
    : BrowserNode(), Labeled<BrowserParameterSet>(all, id),
      def(new ParameterSetData) {
  // not yet read
  def->set_browser_node(this);
}

BrowserParameterSet::BrowserParameterSet(const BrowserParameterSet * model, BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserParameterSet>(all, 0) {
  def = new ParameterSetData(model->def, this);
  comment = model->comment;
}

BrowserParameterSet * BrowserParameterSet::new_one(const char * s,
						   BrowserNode * p)
{
  ParameterSetData * d = new ParameterSetData();
  BrowserParameterSet * result = new BrowserParameterSet(s, p, d);
  
  d->set_browser_node(result);
  
  return result;
}

BrowserNode * BrowserParameterSet::duplicate(BrowserNode * p, QString) {
  BrowserNode * result = new BrowserParameterSet(this, p);
  
  result->set_name("");
  result->update_stereotype();
  
  return result;
}

BrowserParameterSet::~BrowserParameterSet() {
  all.remove(get_ident());
  delete def;
}

void BrowserParameterSet::clear(bool old)
{
  all.clear(old);
}

void BrowserParameterSet::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserParameterSet::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserParameterSet::pixmap(int) const {
  if (deletedp())
    return DeletedParameterSetIcon;
  else if (is_marked)
    return ParameterSetMarkedIcon;
  else
    return ParameterSetIcon;
}

void BrowserParameterSet::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  QString s = name;
  
  if (s.isEmpty())
    s = "Parameter Set";
  
  m.insertItem(new MenuTitle(s, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_edited)
    m.setWhatsThis(m.insertItem("edit", 0),
		   "to edit the <em>parameter set</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only && (edition_number == 0)) {
      m.setWhatsThis(m.insertItem("duplicate", 1),
		     "to copy the <em>parameter set</em> in a new one");
      m.insertSeparator();
      m.setWhatsThis(m.insertItem("delete", 2),
		     "to delete the <em>parameter set</em>. \
Note that you can undelete it after");
    }
    mark_menu(m, "parameter set", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("undelete", 3),
		   "to undelete the <em>parameter set</em>");
  
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    open(FALSE);
    return;
  case 1:
    ((BrowserActivityAction *) parent())->add_parameter_set(this, 0);
    return;
  case 2:
    delete_it();
    break;
  case 3:
    undelete(FALSE);
    break;
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

void BrowserParameterSet::open(bool) {
  if (!is_edited)
    def->edit();
}

void BrowserParameterSet::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserParameterSet::get_type() const {
  return UmlParameterSet;
}

BasicData * BrowserParameterSet::get_data() const {
  return def;
}

bool BrowserParameterSet::allow_empty() const {
  return TRUE;
}

QString BrowserParameterSet::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString s = name;

  if (s.isEmpty())
    s = "<anonymous parameter set>";

  return (rev) ? s + "   [" + p + "]"
	       : p + "::" + s;
}

bool BrowserParameterSet::tool_cmd(ToolCom * com, const char * args) {
  return (def->tool_cmd(com, args, this, comment) ||
	  BrowserNode::tool_cmd(com, args));
}

void BrowserParameterSet::DragMoveEvent(QDragMoveEvent * e) {
  ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserParameterSet::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserParameterSet::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserParameterSet::drag_key() const {
  return QString::number(UmlParameterSet)
      + "#" + QString::number((unsigned long) parent());
}

QString BrowserParameterSet::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserParameterSet::drag_key(BrowserNode * p)
{
  return QString::number(UmlParameterSet)
    + "#" + QString::number((unsigned long) p);
}

void BrowserParameterSet::init()
{
  its_default_stereotypes.clear();
}

// unicode
  const QStringList & BrowserParameterSet::default_stereotypes()
{
  return its_default_stereotypes;
}

void BrowserParameterSet::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "parameterset_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserParameterSet::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "parameterset_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserParameterSet::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "parameterset_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "parameterset " << get_ident() << ' ';
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

BrowserParameterSet * BrowserParameterSet::read_ref(char * & st)
{
  read_keyword(st, "parameterset_ref");
  
  int id = read_id(st);
  BrowserParameterSet * result = all[id];
  
  return (result == 0)
    ? new BrowserParameterSet(id)
    : result;
}

BrowserParameterSet * BrowserParameterSet::read(char * & st, char * k,
						BrowserNode * parent)
{
  BrowserParameterSet * result;
  int id;
  
  if (!strcmp(k, "parameterset_ref")) {
    if (((result = all[id = read_id(st)]) == 0) && (parent != 0))
      result = new BrowserParameterSet(id);
    return result;
  }
  else if (!strcmp(k, "parameterset")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    k = read_keyword(st);
    
    if ((result = all[id]) == 0) {
      result = new BrowserParameterSet(s, parent, new ParameterSetData, id);
      result->def->read(st, k);	// updates k
    }
    else {
      result->def->read(st, k);	// updates k
      result->set_parent(parent);
      result->set_name(s);
    }
    
    result->is_read_only = (!in_import() && read_only_file()) || 
      (user_id() != 0) && result->is_api_base();
    result->def->set_browser_node(result);
    
    result->BrowserNode::read(st, k);
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserParameterSet::get_it(const char * k, int id)
{
  return (!strcmp(k, "parameterset_ref")) ? all[id] : 0;
}
