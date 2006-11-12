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

#include "BrowserFlow.h"
#include "FlowData.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "AType.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserFlow> BrowserFlow::all(1023);
QStringList BrowserFlow::its_default_stereotypes;	// unicode

BrowserFlow::BrowserFlow(BrowserNode * p, BrowserNode * end)
    : BrowserNode("<flow>", p), Labeled<BrowserFlow>(all, 0),
      def(new FlowData) {
  def->set_start_end(this, end);
}

BrowserFlow::BrowserFlow(BrowserNode * p, FlowData * d, int id)
    : BrowserNode("<flow>", p), Labeled<BrowserFlow>(all, id), def(d) {
  // for read
}

BrowserFlow::BrowserFlow(const BrowserFlow * model, BrowserNode * p)
    : BrowserNode(model->name, p),
      Labeled<BrowserFlow>(all, 0) {
  def = new FlowData(model, this);
  comment = model->comment;
}

BrowserFlow::BrowserFlow(int id)
  : BrowserNode(), Labeled<BrowserFlow>(all, id) {
  // was deleted but still referenced
  // create a pseudo definition
  def = new FlowData();
  
  def->set_start_end(this, this);
}

BrowserFlow::~BrowserFlow() {
  all.remove(get_ident());
  if (def)
    delete def;
}

BrowserNode * BrowserFlow::duplicate(BrowserNode * p, QString) {
  BrowserFlow * result = new BrowserFlow(this, p);

  result->def->get_start()->modified();
  result->def->get_start()->package_modified();
  result->update_stereotype();
  
  return result;
}

bool BrowserFlow::undelete(bool, QString & warning, QString & renamed) {
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

void BrowserFlow::compute_referenced_by(QList<BrowserNode> & l,
					BrowserNode * target)
{
  IdIterator<BrowserFlow> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp() &&
	(it.current()->def->get_end_node() == target))
      l.append(it.current());
    ++it;
  }
}

void BrowserFlow::clear(bool old)
{
  all.clear(old);
}

void BrowserFlow::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserFlow::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

void BrowserFlow::update_stereotype(bool) {
  BasicData * data = get_data();
  
  if (data != 0) {
    QString s = name;
    const char * stereotype = data->get_stereotype();
    
    if (s.isEmpty())
      s = "<flow>";
    
    setText(0,
	    (show_stereotypes && stereotype[0])
	    ? QString("<<") + stereotype + ">> " + s
	    : s);
  }
}

const QPixmap* BrowserFlow::pixmap(int) const {
  return (deletedp()) ? DeletedRelationIcon
		      : SimpleRelationIcon;
}

void BrowserFlow::menu() {
  QString s = name;
  
  if (s.isEmpty())
    s = "flow";
  
  QPopupMenu m(0, s);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(s, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!in_edition()) {
      m.setWhatsThis(m.insertItem("edit", 0),
		     "to edit the <em>flow</em>, \
a double click with the left mouse button does the same thing");
      if (!is_read_only && (edition_number == 0)) {
	m.setWhatsThis(m.insertItem("delete", 2),
		       "to delete the <em>flow</em>. \
Note that you can undelete it after");
      }
      m.insertSeparator();
    }
    
    s = def->get_end_node()->get_name();
    
    if (s.isEmpty())
      s = stringify(def->get_end_node()->get_type());
    
    m.setWhatsThis(m.insertItem("select " + s, 7),
		   "to select the destination");
    mark_menu(m, "flow", 90);
    if ((edition_number == 0) 
	&& Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("undelete", 3),
		   "undelete the <em>flow</em> \
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

bool BrowserFlow::in_edition() const {
  return (def->get_start()->is_edited);
}

void BrowserFlow::open(bool) {
  if (! in_edition())
    def->edit();
}

void BrowserFlow::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserFlow::get_type() const {
  return UmlFlow;
}

BasicData * BrowserFlow::get_data() const {
  return def;
}

QString BrowserFlow::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString s = (const char *) name;

  if (s.isEmpty())
    s = "<anonymous flow>";

  return (rev) ? s + "   [" + p + "]"
	       : p + "::" + s;
}

bool BrowserFlow::allow_empty() const {
  return TRUE;
}

bool BrowserFlow::same_name(const QString & s, UmlCode type) const {
  return ((get_type() == type) &&
	  (name == s) && 
	  (name != "<flow>"));
}

void BrowserFlow::init()
{
  its_default_stereotypes.clear();

  its_default_stereotypes.append("interrupt");
  its_default_stereotypes.append("multicast");
  its_default_stereotypes.append("multireceive");
}

// unicode
const QStringList & BrowserFlow::default_stereotypes()
{
  return its_default_stereotypes;
}

bool BrowserFlow::api_compatible(unsigned v) const {
  return (v > 24);
}

bool BrowserFlow::tool_cmd(ToolCom * com, const char * args) {
  return (def->tool_cmd(com, args, this, comment) ||
	  BrowserNode::tool_cmd(com, args));
}

void BrowserFlow::DragMoveEvent(QDragMoveEvent * e) {
  ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserFlow::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserFlow::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserFlow::drag_key() const {
  return QString::number(UmlFlow)
    + "#" + QString::number((unsigned long) parent());
}

QString BrowserFlow::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserFlow::drag_key(BrowserNode * p)
{
  return QString::number(UmlFlow)
    + "#" + QString::number((unsigned long) p);
}

void BrowserFlow::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "flow_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserFlow::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "flow_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
  else
    init();
}

void BrowserFlow::save(QTextStream & st, bool ref, QString & warning) {
  if (ref) {
    // for FlowCanvas
    st << "flow_ref " << get_ident() << " // " << get_name();
  }
  else {
    nl_indent(st);
    st << "flow " << get_ident() << ' ';
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

BrowserFlow * BrowserFlow::read_ref(char * & st)
{
  read_keyword(st, "flow_ref");
  
  int id = read_id(st);
  BrowserFlow * result = all[id];
  
  return (result == 0)
    ? new BrowserFlow(id)
    : result;  
}

BrowserFlow * BrowserFlow::read(char * & st, char * k, BrowserNode * parent)
{
  if (!strcmp(k, "flow")) {
    int id = read_id(st);    
    QString s = read_string(st);
    
    k = read_keyword(st);
    
    FlowData * d = FlowData::read(st, k);	// update k
    BrowserFlow * result;
    
    if ((result = all[id]) == 0)
      result = new BrowserFlow(parent, d, id);
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

BrowserNode * BrowserFlow::get_it(const char * k, int id)
{
  return (!strcmp(k, "flow_ref")) ? all[id] : 0;
}
