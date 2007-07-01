// *************************************************************************
//
// Copyleft 2004-2007 Bruno PAGES  .
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
#include <qpainter.h>
#include <qcursor.h>

#include "BrowserAttribute.h"
#include "BrowserOperation.h"
#include "BrowserClass.h"
#include "AttributeData.h"
#include "ClassData.h"
#include "BrowserActivityAction.h"
#include "BrowserClassInstance.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "ReferenceDialog.h"
#include "mu.h"

IdDict<BrowserAttribute> BrowserAttribute::all(1023);
QStringList BrowserAttribute::its_default_stereotypes;	// unicode

BrowserAttribute::BrowserAttribute(QString s, BrowserNode * p, AttributeData * d, int id)
    : BrowserNode(s, p), Labeled<BrowserAttribute>(all, id),
      def(d), get_oper(0), set_oper(0) {
}

BrowserAttribute::BrowserAttribute(int id)
    : BrowserNode(), Labeled<BrowserAttribute>(all, id),
      def(new AttributeData), get_oper(0), set_oper(0) {
  // not yet read
  def->set_browser_node(this, FALSE, FALSE);
}

BrowserAttribute::BrowserAttribute(const BrowserAttribute * model, BrowserNode * p)
    : BrowserNode(model->name, p), Labeled<BrowserAttribute>(all, 0),
      get_oper(0), set_oper(0) {
  def = new AttributeData(model->def, this);
  comment = model->comment;
}

BrowserAttribute * BrowserAttribute::new_one(QString s, BrowserNode * p,
					     bool enum_item)
{
  AttributeData * d = new AttributeData();
  BrowserAttribute * result = new BrowserAttribute(s, p, d);
  
  d->set_browser_node(result, TRUE, enum_item);
  
  return result;
}

BrowserNode * BrowserAttribute::duplicate(BrowserNode * p, QString name) {
  BrowserAttribute * result = new BrowserAttribute(this, p);
  
  if (name.isEmpty()) {
    result->set_name(get_name());
    result->def->replace((BrowserClass *) parent(), (BrowserClass *) p);
  }
  else
    result->set_name(name);
  result->update_stereotype();
  
  return result;
}

BrowserAttribute::~BrowserAttribute() {
  all.remove(get_ident());
  delete def;
}

void BrowserAttribute::clear(bool old)
{
  all.clear(old);
}

void BrowserAttribute::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserAttribute::renumber(int phase) {
  if (phase != -1)
    new_ident(phase, all);
}

void BrowserAttribute::delete_it() {
  BrowserNode::delete_it();
  
  if (get_oper != 0)
    get_oper->delete_it();
  if (set_oper != 0)
    set_oper->delete_it();
}

void BrowserAttribute::set_get_oper(BrowserOperation * op) {
  if (get_oper == 0) {
    get_oper = op;
    op->set_get_of(this);
  }
}

void BrowserAttribute::set_set_oper(BrowserOperation * op) {
  if (set_oper == 0) {
    set_oper = op;
    op->set_set_of(this);
  }
}

void BrowserAttribute::update_get_oper() {
  get_oper->update_get_of(name, def->get_cppdecl(),
			  def->get_javadecl(), def->get_idldecl(), 
			  def->get_isa_const_attribute(),
			  def->get_isa_class_attribute(),
			  def->get_type());
}

void BrowserAttribute::update_set_oper() {
  set_oper->update_set_of(name, def->get_cppdecl(),
			  def->get_javadecl(), def->get_idldecl(), 
			  def->get_isa_const_attribute(),
			  def->get_isa_class_attribute(),
			  def->get_type());
}

void BrowserAttribute::add_get_oper() {
  set_get_oper(BrowserOperation::new_one(QString::null, (BrowserNode *) parent()));
  update_get_oper();
  ((BrowserNode *) parent())->modified();
}

void BrowserAttribute::add_set_oper() {
  set_set_oper(BrowserOperation::new_one(QString::null, (BrowserNode *) parent()));
  update_set_oper();
  ((BrowserNode *) parent())->modified();
}

const char * BrowserAttribute::constraint() const {
  return def->get_constraint();
}

const QPixmap* BrowserAttribute::pixmap(int) const {
  if (deletedp())
    return DeletedAttributeIcon;
  else {
    switch (def->get_uml_visibility()) {
    case UmlPublic:
      return PublicAttributeIcon;
    case UmlProtected:
      return ProtectedAttributeIcon;
    case UmlPrivate:
      return PrivateAttributeIcon;
    default:
      return PackageAttributeIcon;
    }
  }
}

void BrowserAttribute::paintCell(QPainter * p, const QColorGroup & cg, int column,
				int width, int alignment) {
  const QColor & bg = p->backgroundColor();
  
  if (is_marked) {
    p->setBackgroundMode(OpaqueMode);
    p->setBackgroundColor(UmlRedColor);
  }
    
  p->setFont((def->get_isa_class_attribute())
	     ? ((is_writable()) ? BoldUnderlineFont : UnderlineFont)
	     : ((is_writable()) ? BoldFont : NormalFont));
  QListViewItem::paintCell(p, cg, column, width, alignment);
  
  if (is_marked) {
    p->setBackgroundMode(TransparentMode);
    p->setBackgroundColor(bg);
  }
}

void BrowserAttribute::menu() {
  const char * st = ((BrowserClass *) parent())->get_stereotype();
  bool item = !strcmp(st, "enum_pattern") ||
    !strcmp(st, "enum") && strcmp(get_stereotype(), "attribute");
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_edited)
      m.setWhatsThis(m.insertItem("Edit", 0),
		     (item)
		     ? "to edit the <em>item</em>, \
a double click with the left mouse button does the same thing"
		     : "to edit the <em>attribute</em>, \
a double click with the left mouse button does the same thing");
    if (!is_read_only && (edition_number == 0)) {
      if (!item && (get_oper == 0))
	m.setWhatsThis(m.insertItem("New get operation", 3),
		       "to auto define the <em>get operation</em>");
      if (!item && (set_oper == 0))
	m.setWhatsThis(m.insertItem("New set operation", 4),
		       "to auto define the <em>set operation</em>");
      if (!item && (get_oper == 0) && (set_oper == 0))
	m.setWhatsThis(m.insertItem("New get and set operation", 5),
		       "to auto define the <em>get</em> and <em>set operation</em>s");
      m.setWhatsThis(m.insertItem("Duplicate", 6),
		     "to copy the <em>attribute</em> in a new one");
    }
  
    m.insertSeparator();
    m.setWhatsThis(m.insertItem("Referenced by", 7),
		   "to know who reference the <i>class</i>");

    if (!is_read_only && (edition_number == 0)) {
      m.insertSeparator();
      m.setWhatsThis(m.insertItem("Delete", 1),
		     (item) ? "to delete the <em>item</em>. \
Note that you can undelete it after"
			       : "to delete the <em>attribute</em>. \
Note that you can undelete it after");
    }
    mark_menu(m, "attribute", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("Undelete", 2),
		   (item) ? "to undelete the <em>item</em>"
				     : "to undelete the <em>attribute</em>");
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserAttribute::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    open(FALSE);
    return;
  case 1:
    delete_it();
    break;
  case 2:
    undelete(FALSE);
    break;
  case 3:
    add_get_oper();
    break;
  case 5:
    add_get_oper();
    // no break !
  case 4:
    add_set_oper();
    break;
  case 6:
    ((BrowserClass *) parent())->add_attribute(this);
    return;
  case 7:
    ReferenceDialog::show(this);
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

void BrowserAttribute::apply_shortcut(QString s) {
  int choice = -1;
  const char * st = ((BrowserClass *) parent())->get_stereotype();
  bool item = !strcmp(st, "enum_pattern") ||
    !strcmp(st, "enum") && strcmp(get_stereotype(), "attribute");
  
  if (!deletedp()) {
    if (!is_edited)
      if (s == "Edit")
	choice = 0;
    if (!is_read_only && (edition_number == 0)) {
      if (!item && (get_oper == 0))
	if (s == "New get operation")
	  choice = 3;
      if (!item && (set_oper == 0))
	if (s == "New set operation")
	  choice = 4;
      if (!item && (get_oper == 0) && (set_oper == 0))
	if (s == "New get and set operation")
	  choice = 5;
    }
  
    if (s == "Referenced by")
      choice = 7;

    if (!is_read_only && (edition_number == 0)) {
      if (s == "Delete")
	choice = 1;
    }
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0))
    if (s == "Undelete")
      choice = 2;
  
  exec_menu_choice(choice);
}

void BrowserAttribute::open(bool) {
  if (!is_edited)
    def->edit();
}

void BrowserAttribute::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
  
  if (get_oper != 0)
    update_get_oper();
  if (set_oper != 0)
    update_set_oper();
}

UmlCode BrowserAttribute::get_type() const {
  return UmlAttribute;
}

int BrowserAttribute::get_identifier() const {
  return get_ident();
}

BasicData * BrowserAttribute::get_data() const {
  return def;
}

bool BrowserAttribute::allow_spaces() const {
  return FALSE;
}

bool BrowserAttribute::same_name(const QString & s, UmlCode t) const {
  return (((t == UmlAttribute) || IsaRelation(t)) && (name == s));
}
    
QString BrowserAttribute::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  return (rev) ? name + "   [" + p + "]"
	       : p + "::" + name;
}

void BrowserAttribute::member_cpp_def(const QString & prefix, const QString &, 
				      QString & s, bool templ) const {
  if (! templ) {
    const char * decl = def->get_cppdecl();
    
    if (def->get_isa_class_attribute() && decl[0]) {
      s += prefix;
      s += true_name(get_name(), decl);
      s += "\n\n";
    }
  }
}

void BrowserAttribute::compute_referenced_by(QList<BrowserNode> & l,
					     BrowserNode * target)
{
  IdIterator<BrowserAttribute> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp()) {
      const AType & t = it.current()->def->get_type();
      
      if (t.type == target)
	l.append(it.current());
    }
    ++it;
  }
}

void BrowserAttribute::referenced_by(QList<BrowserNode> & l) {
  BrowserNode::referenced_by(l);
  BrowserActivityAction::compute_referenced_by(l, this);
  BrowserClassInstance::compute_referenced_by(l, this);
}

bool BrowserAttribute::tool_cmd(ToolCom * com, const char * args) {
  return (def->tool_cmd(com, args, this, comment) ||
	  BrowserNode::tool_cmd(com, args));
}

void BrowserAttribute::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

// unicode
  const QStringList & BrowserAttribute::default_stereotypes()
{
  return its_default_stereotypes;
}

void BrowserAttribute::post_load()
{
  // to manage deleted get/set operation
  IdIterator<BrowserAttribute> it(all);
  
  while (it.current()) {
    if ((it.current()->get_oper != 0) && 
	(((BrowserNode *) it.current()->get_oper->parent())->get_type() != UmlClass))
      // operation was deleted
      it.current()->get_oper = 0;
    if ((it.current()->set_oper != 0) && 
	(((BrowserNode *) it.current()->set_oper->parent())->get_type() != UmlClass))
      // operation was deleted
      it.current()->set_oper = 0;      
    
    ++it;  
  }
}

void BrowserAttribute::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "attribute_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserAttribute::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "attribute_stereotypes") ||
      !strcmp(k, "attribut_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserAttribute::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "attribute_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "attribute " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, warning);
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

BrowserAttribute * BrowserAttribute::read_ref(char * & st)
{
  read_keyword(st, "attribut_ref", "attribute_ref");
  
  int id = read_id(st);
  BrowserAttribute * result = all[id];
  
  return (result == 0)
    ? new BrowserAttribute(id)
    : result;
}

BrowserAttribute * BrowserAttribute::read(char * & st, char * k,
					  BrowserNode * parent,
					  bool force)
{
  BrowserAttribute * result;
  int id;
  
  if (!strcmp(k, "attribute_ref") || !strcmp(k, "attribut_ref")) {
    if (((result = all[id = read_id(st)]) == 0) && force)
      result = new BrowserAttribute(id);
    return result;
  }
  else if (!strcmp(k, "attribute") || !strcmp(k, "attribut")) {
    id = read_id(st);
    
    QString s = read_string(st);
    
    k = read_keyword(st);
    
    if ((result = all[id]) == 0) {
      result = new BrowserAttribute(s, parent, new AttributeData, id);
      result->def->read(st, k);	// updates k2
    }
    else {
      result->def->read(st, k);	// updates k2
      result->set_parent(parent);
      result->set_name(s);
    }
    
    result->is_read_only = (!in_import() && read_only_file()) || 
      (user_id() != 0) && result->is_api_base();
    result->def->set_browser_node(result, FALSE, FALSE);
    
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
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserAttribute::get_it(const char * k, int id)
{
  return (!strcmp(k, "attribute_ref")) ? all[id] : 0;
}
