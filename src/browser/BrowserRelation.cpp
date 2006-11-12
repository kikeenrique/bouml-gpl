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

#include "BrowserRelation.h"
#include "BrowserOperation.h"
#include "BrowserClass.h"
#include "RelationData.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "AType.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserRelation> BrowserRelation::all(1023);

BrowserRelation::BrowserRelation(BrowserNode * p, RelationData * d, int id)
    : BrowserNode(d->get_name(), p), Labeled<BrowserRelation>(all, id),
      def(d), get_oper(0), set_oper(0) {
}

BrowserRelation::BrowserRelation(const BrowserRelation * model,
				 BrowserNode * p)
    : BrowserNode(model->def->get_name(), p),
      Labeled<BrowserRelation>(all, 0),
      get_oper(0), set_oper(0) {
  def = new RelationData(model, this);
  comment = model->comment;
}

BrowserRelation::BrowserRelation(int id)
  : BrowserNode(), Labeled<BrowserRelation>(all, id), def(0), get_oper(0), set_oper(0) {
  // for reading
}

BrowserRelation::~BrowserRelation() {
  all.remove(get_ident());
  if (def)
    def->garbage(this);
}

BrowserNode * BrowserRelation::duplicate(BrowserNode * p, QString) {
  BrowserRelation * result = new BrowserRelation(this, p);

  result->def->get_start()->modified();
  result->def->get_start()->package_modified();
  if (result->def->get_end() != 0) {
    result->def->get_end()->modified();
    result->def->get_end()->package_modified();
  }
  
  result->update_stereotype();
  
  return result;
}

void BrowserRelation::delete_it() {
  if (get_oper != 0)
    get_oper->delete_it();
  if (set_oper != 0)
    set_oper->delete_it();
  
  BrowserNode::delete_it();
}

bool BrowserRelation::undelete(bool, QString & warning, QString & renamed) {
  if (! deletedp())
    return FALSE;
  
  if (def->get_start_class()->deletedp() ||
      def->get_end_class()->deletedp()) {
    if (def->is_a(this))
      warning += QString("<li><b>") + quote(def->get_name(this)) + "</b> from <b>" +
	def->get_start_class()->full_name() +
	  "</b> to <b>" + def->get_end_class()->full_name() + "</b>\n";
    else
      warning += QString("<li><b>") + def->get_name(this) + "</b> from <b>" +
	def->get_end_class()->full_name() +
	  "</b> to <b>" + quote(def->get_start_class()->full_name()) + "</b>\n";
    return FALSE;
  }
  else {
    switch (get_type()) {
    case UmlGeneralisation:
    case UmlRealize:
      if (def->get_start_class()->check_inherit(def->get_end_class()) != 0) {
	warning += QString("<li><b>") + quote(def->get_name(this)) +
	  "</b> because <b>" + def->get_start_class()->full_name() +
	  "</b> cannot (or already) inherit on <b>" + def->get_end_class()->full_name()
	    + "</b>\n";
	return FALSE;
      }
      break;
    default:
      break;
    }
  }

  if (def->undelete(warning, renamed, this, is_deleted)) {
    renamed += QString("<li><b>") + full_name() + "</b>\n";
    modified();
  }

  package_modified();
  repaint();
  
  return TRUE;
}

const char * BrowserRelation::get_comment() const {
  return (def->is_a(this))
    ? def->get_comment_a() 
    : def->get_comment_b();
}

void BrowserRelation::set_comment(const char * c) {
  if (def->is_a(this))
    def->set_comment_a(c);
  else
    def->set_comment_b(c);
}

void BrowserRelation::compute_referenced_by(QList<BrowserNode> & l,
					    BrowserClass * target)
{
  IdIterator<BrowserRelation> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp() &&
	((it.current()->def->is_a(it.current()))
	 ? (it.current()->def->get_end_class() == target)
	 : (it.current()->def->get_start_class() == target)))
      l.append(it.current());
    ++it;
  }
}

BrowserRelation * BrowserRelation::temporary(RelationData * d) {
  BrowserRelation * result = new BrowserRelation(0);
  
  result->def = d;
  return result;
}

void BrowserRelation::clear(bool old)
{
  all.clear(old);
  RelationData::clear(old);
}

void BrowserRelation::update_idmax_for_root()
{
  all.update_idmax_for_root();
  RelationData::update_idmax_for_root();
}
    
void BrowserRelation::renumber(int phase) {
  if (phase != -1) {
    new_ident(phase, all);
    def->renumber(phase, this);
  }
}

void BrowserRelation::set_get_oper(BrowserOperation * op) {
  if (get_oper == 0) {
    get_oper = op;
    op->set_get_of(this);
  }
}

void BrowserRelation::set_set_oper(BrowserOperation * op) {
  if (set_oper == 0) {
    set_oper = op;
    op->set_set_of(this);
  }
}

void BrowserRelation::update_get_oper() {
  AType t;
  
  if (def->is_a(this)) {
    t.type = def->get_end_class();
    get_oper->update_get_of(def->get_role_a(), def->get_cppdecl_a(),
			    def->get_javadecl_a(), def->get_idldecl_a(), 
			    def->get_isa_const_relation_a(),
			    def->get_isa_class_relation_a(),
			    t, def->get_multiplicity_a(),
			    def->get_stereotype());
  }
  else {
    t.type = def->get_start_class();
    get_oper->update_get_of(def->get_role_b(), def->get_cppdecl_b(),
			    def->get_javadecl_b(), def->get_idldecl_b(), 
			    def->get_isa_const_relation_b(),
			    def->get_isa_class_relation_b(),
			    t, def->get_multiplicity_b(),
			    def->get_stereotype());
  }
}

void BrowserRelation::update_set_oper() {
  AType t;
  
  if (def->is_a(this)) {
    t.type = def->get_end_class();
    set_oper->update_set_of(def->get_role_a(), def->get_cppdecl_a(),
			    def->get_javadecl_a(), def->get_idldecl_a(), 
			    def->get_isa_const_relation_a(),
			    def->get_isa_class_relation_a(),
			    t, def->get_multiplicity_a(),
			    def->get_stereotype());
  }
  else {
    t.type = def->get_start_class();
    set_oper->update_set_of(def->get_role_b(), def->get_cppdecl_b(),
			    def->get_javadecl_b(), def->get_idldecl_b(), 
			    def->get_isa_const_relation_b(),
			    def->get_isa_class_relation_b(),
			    t, def->get_multiplicity_b(),
			    def->get_stereotype());
  }
}

void BrowserRelation::add_get_oper() {
  set_get_oper(BrowserOperation::new_one(QString::null, (BrowserNode *) parent()));
  update_get_oper();
  ((BrowserNode *) parent())->modified();
}

void BrowserRelation::add_set_oper() {
  set_set_oper(BrowserOperation::new_one(QString::null, (BrowserNode *) parent()));
  update_set_oper();
  ((BrowserNode *) parent())->modified();
}

void BrowserRelation::update_stereotype(bool) {
  BasicData * data = get_data();
  
  if (data != 0) {
    QString s;
    
    switch (get_type()) {
    case UmlGeneralisation:
    case UmlDependency:
    case UmlRealize:
      s = def->get_name(this) + " " +
	((def->is_a(this)) ? def->get_end_class()
			   : def->get_start_class())->get_name();
      break;
    default:
      s = (const char *) name;
    }
    
    const char * stereotype = data->get_stereotype();
    
    setText(0,
	    (show_stereotypes && stereotype[0])
	    ? QString("<<") + stereotype + ">> " + s
	    : s);
  }
}

AType BrowserRelation::class_association() const {
  return def->get_association();
}

const QPixmap* BrowserRelation::pixmap(int) const {
  if (deletedp())
    return DeletedRelationIcon;
  else {
    switch (def->is_a(this) ? def->get_uml_visibility_a()
			    : def->get_uml_visibility_b()) {
    case UmlPublic:
      return PublicRelationIcon;
    case UmlProtected:
      return ProtectedRelationIcon;
    case UmlPrivate:
      return PrivateRelationIcon;
    default:
      return PackageRelationIcon;
    }
  }
}

void BrowserRelation::paintCell(QPainter * p, const QColorGroup & cg, int column,
				int width, int alignment) {
  bool class_relation = def->is_a(this)
    ? def->get_isa_class_relation_a() : def->get_isa_class_relation_b();
  
  const QColor & bg = p->backgroundColor();
  
  if (is_marked) {
    p->setBackgroundMode(OpaqueMode);
    p->setBackgroundColor(UmlRedColor);
  }
    
  p->setFont((class_relation)
	     ? ((is_writable()) ? BoldUnderlineFont : UnderlineFont)
	     : ((is_writable()) ? BoldFont : NormalFont));
  QListViewItem::paintCell(p, cg, column, width, alignment);
  
  if (is_marked) {
    p->setBackgroundMode(TransparentMode);
    p->setBackgroundColor(bg);
  }
}

void BrowserRelation::menu() {
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
	if (RelationData::isa_association(def->get_type())) {
	  m.setWhatsThis(m.insertItem("duplicate", 1),
			 "to copy the <em>relation</em> in a new one");
	  if (get_oper == 0)
	    m.setWhatsThis(m.insertItem("add get operation", 4),
			   "to auto define the <em>get operation</em>");
	  if (set_oper == 0)
	    m.setWhatsThis(m.insertItem("add set operation", 5),
			   "to auto define the <em>set operation</em>");
	  if ((get_oper == 0) && (set_oper == 0))
	    m.setWhatsThis(m.insertItem("add get and set operation", 6),
			   "to auto define the <em>get</em> and <em>set operation</em>s");
	  m.insertSeparator();
	}
	m.setWhatsThis(m.insertItem("delete", 2),
		       "to delete the <em>relation</em>. \
Note that you can undelete it after");
      }
      m.insertSeparator();
    }
    m.setWhatsThis(m.insertItem(QString("select ") +
				((def->is_a(this)) 
				 ? def->get_end_class()
				 : def->get_start_class())->get_name(),
				7),
		   "to select the destination class");
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
(except if the class at the other side is also deleted)");
  
  int rank = m.exec(QCursor::pos());
  
  switch (rank) {
  case 0:
    open(FALSE);
    break;
  case 1:
    ((BrowserClass *) parent())->add_relation(this);
    return;
  case 2:
    delete_it();
    break;
  case 3:
    BrowserNode::undelete(FALSE);
    break;
  case 4:
    add_get_oper();
    break;
  case 6:
    add_get_oper();
    // no break !
  case 5:
    add_set_oper();
    break;
  case 7:
    ((def->is_a(this)) ? def->get_end_class()
		       : def->get_start_class())->select_in_browser();
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

bool BrowserRelation::in_edition() const {
  return (def->get_start()->is_edited ||
	  ((def->get_end() != 0) && def->get_end()->is_edited));
}

void BrowserRelation::open(bool) {
  if (! in_edition())
    def->edit();
}

void BrowserRelation::modified() {
  set_name(def->get_name(this));
  update_stereotype(FALSE);
  repaint();
  
  if (get_oper != 0)
    update_get_oper();
  if (set_oper != 0)
     update_set_oper();
}

UmlCode BrowserRelation::get_type() const {
  return (def == 0)
    ? UmlRelations	// legal case to manage get/set operation read
    : def->get_type();
}

BasicData * BrowserRelation::get_data() const {
  return def;
}

bool BrowserRelation::same_name(const QString & s, UmlCode t) const {
  if ((t == UmlAttribute) || IsaRelation(t)) {
    QString role = name;
    
    if (role[0] != '(')
      // role have name, compare with the role name
      // followed by ' ' then '(' then the relation's name
      return (s == role.left(role.find(' ')));
  }

  return FALSE;
}

QString BrowserRelation::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);
  QString n = (def->is_a(this)) ? def->get_role_a() : def->get_role_b();
  
  if (n.isEmpty())
    n = (const char *) name;

  return (rev) ? n + "   [" + p + "]"
	       : p + "::" + n;
}

void BrowserRelation::member_cpp_def(const QString & prefix, const QString &,
				     QString & s, bool templ) const {
  if (! templ) {
    if (def->is_a(this)) {
      const char * decl = def->get_cppdecl_a();
      
      if (def->get_isa_class_relation_a() && decl[0]) {
	s += prefix;
	s += true_name(def->get_role_a(), decl);
	s += "\n\n";
      }
    }
    else {
      const char * decl = def->get_cppdecl_b();
      
      if (def->get_isa_class_relation_b() && decl[0]) {
	s += prefix;
	s += true_name(def->get_role_b(), decl);
	s += "\n\n";
      }
    }
  }
}

void BrowserRelation::write_id(ToolCom * com) {
  com->write_id(this, 0, name);
}

bool BrowserRelation::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case isWritableCmd:
    com->write_bool(def->is_writable(this));
    break;
  case addGetOperCmd:
    if (def->is_writable(this) && (get_oper == 0)) {
      add_get_oper();
      com->write_ack(TRUE);
    }
    else
      com->write_ack(FALSE);
    break;
  case addSetOperCmd:
    if (def->is_writable(this) && (set_oper == 0)) {
      add_set_oper();
      com->write_ack(TRUE);
    }
    else
      com->write_ack(FALSE);
    break;
  default:
    return (def->tool_cmd(com, this, args) ||
	    BrowserNode::tool_cmd(com, args));
  }
  
  // cmd managed
  return TRUE;
}

void BrowserRelation::DragMoveEvent(QDragMoveEvent * e) {
  ((BrowserNode *) parent())->DragMoveInsideEvent(e);
}

void BrowserRelation::DropEvent(QDropEvent * e) {
  DropAfterEvent(e, 0);
}

void BrowserRelation::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserRelation::drag_key() const {
  return QString::number(UmlRelations)
    + "#" + QString::number((unsigned long) parent());
}

QString BrowserRelation::drag_postfix() const {
  return "#" + QString::number((unsigned long) parent());
}

QString BrowserRelation::drag_key(BrowserNode * p)
{
  return QString::number(UmlRelations)
    + "#" + QString::number((unsigned long) p);
}

// Removes the relation from its parent tree, delete it
// Needed to not show all the sub classes of a class and directional
// relation which are not navigable in the direction of the relation
BrowserNode * BrowserRelation::extract() {
  BrowserNode * p = ((BrowserNode *) parent());
  
  p->takeItem(this);
  def = 0;
  
  if (get_oper != 0) {
    p->takeItem(get_oper);
    delete get_oper;
  }
  if (set_oper != 0) {
    p->takeItem(set_oper);
    delete set_oper;
  }
  
  delete this;
  return p;
}

// the relation returns bi-directionnal
BrowserRelation * BrowserRelation::reinsert(BrowserNode * p, RelationData * d)
{
  p->package_modified();
  return new BrowserRelation(p, d);
}

void BrowserRelation::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "classrelation_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "classrelation " << get_ident() << " // " << get_name();
    indent(+1);
    if (!def->is_a(this)) {
      nl_indent(st);
      def->save(st, TRUE, warning);
    }
    else
      def->save(st, FALSE, warning);
    if (get_oper != 0) {
      nl_indent(st);
      st << "get_oper ";
      get_oper->save(st, TRUE, warning);
    }
    if (set_oper != 0) {
      nl_indent(st);
      st << "set_oper ";
      set_oper->save(st, TRUE, warning);
    }
    BrowserNode::save(st);
    indent(-1);
    nl_indent(st);
    st << "end";
    
    // for saveAs
    if (! is_api_base())
      is_read_only = FALSE;
  }
}

void BrowserRelation::post_load(QList<BrowserRelation> & wrong)
{
  IdIterator<BrowserRelation> it(all);
  BrowserRelation * br;
  
  while ((br = it.current()) != 0) {
    if (((BrowserNode *) br->parent())->get_type() == UmlClass) {
      // not yet moved in UndefinedNodePackage
      
      RelationData * d;
      BrowserRelation * endrel;
      
      if ((d = br->def) == 0)
	// end -> start relation, not read
	wrong.append(br);
      else if (d->get_type() == UmlRelations)
	// temporary data, rel not read,
	wrong.append(br);
      else if ((d->get_start() == br) &&
	       ((endrel = d->get_end()) != 0) &&
	       (endrel->def != d)) {
	// br is the 'forward' direction of a bi dir relation,
	// endrel is the 'backward' direction not read
	if (endrel->def != 0)
	  delete endrel->def;
	endrel->def = br->def; // to help delete
	
	wrong.append(br);
      }
      else {
	// not a temporary relation
	if ((it.current()->get_oper != 0) && 
	    (((BrowserNode *) it.current()->get_oper->parent())->get_type() != UmlClass))
	  // operation was deleted
	  it.current()->get_oper = 0;
	if ((it.current()->set_oper != 0) && 
	    (((BrowserNode *) it.current()->set_oper->parent())->get_type() != UmlClass))
	  // operation was deleted
	  it.current()->set_oper = 0;
      }
    }
    ++it;
  }

}

BrowserRelation * BrowserRelation::read_ref(char * & st)
{
  read_keyword(st, "classrelation_ref");
  return all[read_id(st)];
}

BrowserRelation * BrowserRelation::read_ref(char * & st, char * k)
{
  if (strcmp(k, "classrelation_ref"))
    wrong_keyword(k, "classrelation_ref");
  
  int id = read_id(st);
  BrowserRelation * result = all[id];
  
  return (result == 0)
    ? new BrowserRelation(id)
    : result;
}

BrowserRelation * BrowserRelation::read(char * & st, char * k,
					BrowserNode * parent)
{
  if (! strcmp(k, "classrelation_ref"))
    return read_ref(st, k);
  if (!strcmp(k, "classrelation")) {
    int id = read_id(st);
    
    k = read_keyword(st);
    
    RelationData * d = RelationData::read(st, k);	// updates k
    BrowserRelation * result;
    
    if ((result = all[id]) == 0)
      result = new BrowserRelation(parent, d, id);
    else {
      if (result->def != 0)
	// re-load
	delete result->def;
      result->def = d;
      result->set_parent(parent);
      result->set_name(d->get_name(result));
    }
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    if (!strcmp(k, "get_oper")) {
      BrowserOperation * oper = BrowserOperation::read_ref(st);
      
      if (oper != 0)
	result->set_get_oper(oper);
      
      k = read_keyword(st);
    }
    if (!strcmp(k, "set_oper")) {
      BrowserOperation * oper = BrowserOperation::read_ref(st);
      
      if (oper != 0)
	result->set_set_oper(oper);
      
      k = read_keyword(st);
    }
    
    result->BrowserNode::read(st, k);
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    result->update_stereotype();
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserRelation::get_it(const char * k, int id)
{
  return (!strcmp(k, "classrelation_ref")) ? all[id] : 0;
}
