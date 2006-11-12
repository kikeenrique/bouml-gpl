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

#include "BrowserRegion.h"
#include "BrowserState.h"
#include "SimpleData.h"
#include "UmlDrag.h"
#include "UmlPixmap.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserRegion> BrowserRegion::all;
QStringList BrowserRegion::its_default_stereotypes;	// unicode

BrowserRegion::BrowserRegion(QString s, BrowserNode * p, BasicData * d, int id)
    : BrowserNode(s, p), Labeled<BrowserRegion>(all, id), def(d) {
  def->set_browser_node(this);
}

BrowserRegion::BrowserRegion(const BrowserRegion * model,
			     BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserRegion>(all, 0) {
  def = new SimpleData(model->def);
  def->set_browser_node(this);
  comment = model->comment;
}

BrowserRegion::BrowserRegion(int id)
    : BrowserNode(), Labeled<BrowserRegion>(all, id),
      def(new SimpleData) {
  // not yet read
  def->set_browser_node(this);
}

BrowserRegion::~BrowserRegion() {
  all.remove(get_ident());
  delete def;
}

void BrowserRegion::delete_it() {
  // to update state's region drawing
  ((BrowserNode *) parent())->modified();
  
  BrowserNode::delete_it();
}

BrowserNode * BrowserRegion::duplicate(BrowserNode * p, QString name) {
  BrowserRegion * result = new BrowserRegion(this, p);
  
  result->set_name((name.isEmpty()) ? get_name() : (const char *) name);
  result->update_stereotype();
  
  return result;
}

void BrowserRegion::clear(bool old)
{
  all.clear(old);
}

void BrowserRegion::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserRegion::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

const QPixmap* BrowserRegion::pixmap(int) const {
  return (deletedp()) ? DeletedRegionIcon : RegionIcon;
}

BrowserRegion * BrowserRegion::add_region(BrowserNode * future_parent,
					  const char * s)
{
  SimpleData * st = new SimpleData();
  BrowserRegion * r = new BrowserRegion(s, future_parent, st);
  
  future_parent->setOpen(TRUE);
  future_parent->package_modified();
  // to update state's region drawing
  future_parent->modified();
  
  return r;
}

void BrowserRegion::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(def->definition(FALSE), m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_read_only) {
      m.setWhatsThis(m.insertItem("add state", 1),
		     "to add a <em>state</em> to the <em>region</em>");
    }
    m.insertSeparator();
    m.setWhatsThis(m.insertItem("edit", 3),
		   "to edit the <em>region</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only && (edition_number == 0)) {
      m.insertSeparator();
      m.setWhatsThis(m.insertItem("delete", 7),
		     "to delete the <em>region</em>. \
Note that you can undelete it after");
    }
    mark_menu(m, "region", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0)) {
    m.setWhatsThis(m.insertItem("undelete", 8),
		   "to undelete the <em>region</em>");
 
    QListViewItem * child;
  
    for (child = firstChild(); child != 0; child = child->nextSibling()) {
      if (((BrowserNode *) child)->deletedp()) {
	m.setWhatsThis(m.insertItem("undelete recursively", 9),
		       "undelete the region and its children");
	break;
      }
    }
  }
  
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 1:
    BrowserState::add_state(this, (bool) FALSE);
    return;
  case 3:
    open(FALSE);
    return;
  case 7:
    delete_it();
    break;
  case 8:
    BrowserNode::undelete(FALSE);
    break;
  case 9:
    BrowserNode::undelete(TRUE);
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

void BrowserRegion::open(bool) {
  if (!is_edited)
    edit("Region", its_default_stereotypes);
}

void BrowserRegion::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserRegion::get_type() const {
  return UmlRegion;
}

BasicData * BrowserRegion::get_data() const {
  return def;
}

QString BrowserRegion::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString s = name;

  if (s.isEmpty())
    s = "<anonymous region>";

  return (rev) ? s + "   [" + p + "]"
	       : p + "::" + s;
}

bool BrowserRegion::allow_empty() const {
  return TRUE;
}

bool BrowserRegion::allow_spaces() const {
  return TRUE;
}

bool BrowserRegion::same_name(const QString &, UmlCode) const {
  return FALSE;
}

void BrowserRegion::init()
{
  its_default_stereotypes.clear();
}

// unicode
const QStringList & BrowserRegion::default_stereotypes()
{
  return its_default_stereotypes;
}

bool BrowserRegion::api_compatible(unsigned v) const {
  return (v > 14);
}

bool BrowserRegion::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case createCmd: 
    {
      bool ok = TRUE;
      
      if (is_read_only && !root_permission())
	ok = FALSE;
      else {
	UmlCode k = com->get_kind(args);
	
	switch (k) {
	case UmlState:
	  if (wrong_child_name(args, UmlState, TRUE, FALSE))
	    ok = FALSE;
	  else
	    (BrowserState::add_state(this, args))->write_id(com);
	  break;
	default:
	  ok = FALSE;
	  break;
	}
      }
      
      if (! ok)
	com->write_id(0);
      else
	package_modified();
      
      return TRUE;
    }
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

bool BrowserRegion::may_contains_them(const QList<BrowserNode> & l,
				     bool & duplicable) const {
  QListIterator<BrowserNode> it(l);
  
  for (; it.current(); ++it) {
    if (it.current()->get_type() != UmlState) {
      duplicable = FALSE;
      return FALSE;
    }
    
    if (! may_contains(it.current(), FALSE))
      return FALSE;
    
    duplicable = may_contains_it(it.current());
  }
  
  return TRUE;
}

void BrowserRegion::DragMoveEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserState::drag_key(this)))
    e->accept();
  else
    ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserRegion::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserRegion::DragMoveInsideEvent(QDragMoveEvent * e) {
  if (UmlDrag::canDecode(e, BrowserState::drag_key(this)))
    e->accept();
  else
    e->ignore();
}

void BrowserRegion::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  BrowserNode * bn;
  
  if (((bn = UmlDrag::decode(e, BrowserState::drag_key(this))) != 0) &&
      (bn != after) && (bn != this)) {
    if (may_contains(bn, FALSE)) 
      move(bn, after);
    else {
      QMessageBox::critical(0, "Error", "Forbiden");
      e->ignore();
    }
  }
  else if ((bn == 0) && (after == 0))
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserRegion::drag_key() const {
  return QString::number(UmlRegion)
    + "#" + QString::number((unsigned long) parent());
}

QString BrowserRegion::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserRegion::drag_key(BrowserNode * p)
{
  return QString::number(UmlRegion)
    + "#" + QString::number((unsigned long) p);
}

void BrowserRegion::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "state_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserRegion::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "state_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserRegion::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "region_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "region " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);

    BrowserNode::save(st);
    
    // saves the sub elts
      
    QListViewItem * child = firstChild();
    
    if (child != 0) {
      for (;;) {
	if (! ((BrowserNode *) child)->deletedp()) {
	  ((BrowserNode *) child)->save(st, FALSE, warning);
	  
	  child = child->nextSibling();
	  if (child != 0)
	    st << '\n';
	  else
	    break;
	}
	else if ((child = child->nextSibling()) == 0)
	  break;
      }
    }
    
    indent(-1);
    nl_indent(st);
    st << "end";
    
    // for saveAs
    if (! is_api_base())
      is_read_only = FALSE;
  }
}

BrowserRegion * BrowserRegion::read_ref(char * & st)
{
  read_keyword(st, "region_ref");
  
  int id = read_id(st);
  BrowserRegion * result = all[id];
  
  return (result == 0)
    ? new BrowserRegion(id)
    : result;
}

BrowserRegion * BrowserRegion::read(char * & st, char * k,
				    BrowserNode * parent)
{
  BrowserRegion * result;
  int id;
  
  if (!strcmp(k, "region_ref")) {
    id = read_id(st);
    result = all[id];
    
    return (result == 0)
      ? new BrowserRegion(id)
      : result;
  }
  else if (!strcmp(k, "region")) {
    id = read_id(st);
    result = all[id];
    
    if (result == 0)
      result = new BrowserRegion(read_string(st), parent, new SimpleData, id);
    else {
      result->set_parent(parent);
      result->set_name(read_string(st));
    }
    
    k = read_keyword(st);
    result->def->read(st, k);

    result->BrowserNode::read(st, k);
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    if (strcmp(k, "end")) {
      while (BrowserState::read(st, k, result))
	k = read_keyword(st);
      
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserRegion::get_it(const char * k, int id)
{
  return (!strcmp(k, "region_ref")) ? all[id] : 0;
}
