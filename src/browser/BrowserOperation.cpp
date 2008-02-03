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

#include <qpopupmenu.h> 
#include <qfileinfo.h> 
#include <qpainter.h>
#include <qcursor.h>
#include <qptrdict.h>

#include "BrowserOperation.h"
#include "BrowserAttribute.h"
#include "BrowserRelation.h"
#include "BrowserClass.h"
#include "OperationData.h"
#include "GenerationSettings.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "mu.h"

IdDict<BrowserOperation> BrowserOperation::all(1023, __FILE__);
QStringList BrowserOperation::its_default_stereotypes;	// unicode

BrowserOperation::BrowserOperation(int id)
    : BrowserNode(), Labeled<BrowserOperation>(all, id),
      def(new OperationData(id)), get_of(0), set_of(0) {
  // not yet read
}

BrowserOperation::BrowserOperation(QString s, BrowserNode * p, OperationData * d, int id)
    : BrowserNode(s, p), Labeled<BrowserOperation>(all, id), def(d),
      get_of(0), set_of(0) {
}

BrowserOperation::BrowserOperation(const BrowserOperation * model, BrowserNode * p)
    : BrowserNode(model->name, p),
      Labeled<BrowserOperation>(all, 0), get_of(0), set_of(0) {
  def = new OperationData(model->def, this);
  comment = model->comment;
  
  const char * b;
  
  b = model->def->get_body('c');
  if ((b != 0) && (*b != 0))
    def->new_body(b, 'c');
  
  b = model->def->get_body('j');
  if ((b != 0) && (*b != 0))
    def->new_body(b, 'j');
  
  b = model->def->get_body('p');
  if ((b != 0) && (*b != 0))
    def->new_body(b, 'p');
  
  b = model->def->get_body('y');
  if ((b != 0) && (*b != 0))
    def->new_body(b, 'y');
}

BrowserNode * BrowserOperation::duplicate(BrowserNode * p, QString n) {
  BrowserOperation * result = new BrowserOperation(this, p);

  if (n.isEmpty()) {
    if (get_name()[0] == '~')
      // destructor, change the name
      n = QString("~") + p->get_name();
    else if (name == ((BrowserNode *) parent())->get_name())
      // constructor, change the name
      n = p->get_name();
    else
      n = get_name();
    
    result->def->replace((BrowserClass *) parent(), (BrowserClass *) p);
  }

  result->set_name(n);
  result->update_stereotype();
  
  return result;
}

BrowserOperation::~BrowserOperation() {
#if 0
  if (deletedp()) {
    delete_definition(get_ident(), "cpp");
    delete_definition(get_ident(), "java");
    delete_definition(get_ident(), "php");
    delete_definition(get_ident(), "python");
  }
#endif
  
  all.remove(get_ident());
  delete def;
}

void BrowserOperation::clear(bool old)
{
  all.clear(old);
  OperationData::clear(old);
}

void BrowserOperation::update_idmax_for_root()
{
  all.update_idmax_for_root();
  OperationData::update_idmax_for_root();
}
    
bool BrowserOperation::delete_internal(QString &) {
  // redefined because is_writable() must not be taken into account
  // because the fact that the body file is writable is useless
  is_deleted = TRUE;
  if (is_marked) {
    marked_list.removeRef(this);
    is_marked = FALSE;
  }
  
  def->delete_it();
  repaint();

  return TRUE;
}

void BrowserOperation::renumber(int phase) {
  static QPtrDict<char> cpp;
  static QPtrDict<char> java;
  static QPtrDict<char> php;
  static QPtrDict<char> python;
  
  char * b;
  
  if (phase == -1) {  
    if ((b = def->get_body('c')) != 0)
      cpp.insert(this, b);
    if ((b = def->get_body('j')) != 0)
      java.insert(this, b);
    if ((b = def->get_body('p')) != 0)
      php.insert(this, b);
    if ((b = def->get_body('y')) != 0)
      python.insert(this, b);
  }
  else {
    new_ident(phase, all);
    def->renumber(phase);
    
    if ((b = cpp.take(this)) != 0)
      def->new_body(b, 'c');
    if ((b = java.take(this)) != 0)
      def->new_body(b, 'j');
    if ((b = php.take(this)) != 0)
      def->new_body(b, 'p');
    if ((b = python.take(this)) != 0)
      def->new_body(b, 'y');
  }
}

bool BrowserOperation::undelete(bool rec, QString & warning, QString & renamed) {
  if (deletedp()) {
    if (get_of != 0) {
      if (get_of->deletedp()) {
	warning += QString("<li><b>") + quote(def->definition(TRUE)) + "</b>\n";
	return FALSE;
      }
    }
    else if (set_of != 0) {
      if (set_of->deletedp()) {
	warning += QString("<li><b>") + quote(def->definition(TRUE)) + "</b>\n";
	return FALSE;
      }
    }
    
    return BrowserNode::undelete(rec, warning, renamed);
  }
  else
    return FALSE;
}

BrowserOperation * BrowserOperation::new_one(QString s, BrowserNode * p)
{
  OperationData * d = new OperationData();
  BrowserOperation * result =
    new BrowserOperation(s, p, d, d->get_ident());
  
  d->set_browser_node(result, TRUE);
  
  return result;
}

void BrowserOperation::set_get_of(BrowserNode * o) {
  if (get_of ==  0) {
    get_of = o;
    
    if (o->get_type() == UmlAttribute)
      ((BrowserAttribute *) o)->set_get_oper(this);
    else
      ((BrowserRelation *) o)->set_get_oper(this);
  }
}

void BrowserOperation::set_set_of(BrowserNode * o) {
  if (set_of ==  0) {
    set_of = o;
    
    if (o->get_type() == UmlAttribute)
      ((BrowserAttribute *) o)->set_set_oper(this);
    else
      ((BrowserRelation *) o)->set_set_oper(this);
  }
}

static QString substr_name(QString s, const QString & a)
{
  int index;
  
  if ((index = s.find("${name}")) != -1)
    s.replace(index, 7, a);
  else if ((index = s.find("${Name}")) != -1)
    s.replace(index, 7, capitalize(a));
  else if ((index = s.find("${NAME}")) != -1)
    s.replace(index, 7, a.upper());
  
  return s;
}

void BrowserOperation::update_get_of(const QString & attr_name,
				     const QString & cpp_decl,
				     const QString & java_decl,
				     const QString & php_decl,
				     const QString & python_decl,
				     const QString & idl_decl,
				     bool is_const, bool is_class_member,
				     const AType & cl,
				     QString multiplicity,
				     QString ste) {
  bool create = name.isEmpty();
  
  switch (GenerationSettings::uml_default_get_name()) {
  case CppView:
    set_name(substr_name(GenerationSettings::cpp_default_get_name(), attr_name));
    break;
  case JavaView:
    set_name(substr_name(GenerationSettings::java_default_get_name(), attr_name));
    break;
  case PhpView:
    set_name(substr_name(GenerationSettings::php_default_get_name(), attr_name));
    break;
  case PythonView:
    set_name(substr_name(GenerationSettings::python_default_get_name(), attr_name));
    break;
  case IdlView:
    set_name(substr_name(GenerationSettings::idl_default_get_name(), attr_name));
    break;
  default:
    set_name(QString("get_") + attr_name);
  }
  
  def->update_get_of(attr_name, cpp_decl, java_decl, php_decl, python_decl, idl_decl,
		     is_const, is_class_member, cl, multiplicity, ste, create);
  
  update_stereotype();
}

void BrowserOperation::update_set_of(const QString & attr_name,
				     const QString & cpp_decl,
				     const QString & java_decl,
				     const QString & php_decl,
				     const QString & python_decl,
				     const QString & idl_decl,
				     bool is_const, bool is_class_member,
				     const AType & cl,
				     QString multiplicity,
				     QString ste) {
  bool create = name.isEmpty();
  
  switch (GenerationSettings::uml_default_set_name()) {
  case CppView:
    set_name(substr_name(GenerationSettings::cpp_default_set_name(), attr_name));
    break;
  case JavaView:
    set_name(substr_name(GenerationSettings::java_default_set_name(), attr_name));
    break;
  case PhpView:
    set_name(substr_name(GenerationSettings::php_default_set_name(), attr_name));
    break;
  case PythonView:
    set_name(substr_name(GenerationSettings::python_default_set_name(), attr_name));
    break;
  case IdlView:
    set_name(substr_name(GenerationSettings::idl_default_set_name(), attr_name));
    break;
  default:
    set_name(QString("set_") + attr_name);
  }

  def->update_set_of(attr_name, cpp_decl, java_decl, php_decl, python_decl, idl_decl,
		     is_const, is_class_member, cl, multiplicity, ste, create);
  
  update_stereotype();
}

QString BrowserOperation::get_of_name() const {
  QString result;
  
  if (get_of)
    result = get_of->get_name();
  else if (set_of)
    result = set_of->get_name();
  else
    return QString::null;
  
  int index;
  
  return ((index = result.find(' ')) != -1)
    ? result.left(index)
    : result;
}

AType BrowserOperation::get_of_association() const {
  if (get_of)
    return get_of->class_association();
  else if (set_of)
    return set_of->class_association();
  else {
    AType result;
    
    return result;
  }  
}

QString BrowserOperation::compute_name(const char * name_spec) const {
   if (*name_spec != 0) {
    QString s = (const char *) name_spec;
    int index;
    
    if ((index = s.find("${name}")) != -1)
      s.replace(index, 7, get_of_name());
    else if ((index = s.find("${Name}")) != -1)
      s.replace(index, 7, capitalize(get_of_name()));
    return s;
  }
  else
    return get_name();
}

const char * BrowserOperation::constraint() const {
  return def->get_constraint();
}

const QPixmap* BrowserOperation::pixmap(int) const {
  if (deletedp())
    return DeletedOperationIcon;
  else {
    switch (def->get_uml_visibility()) {
    case UmlPublic:
      return PublicOperationIcon;
    case UmlProtected:
      return ProtectedOperationIcon;
    case UmlPrivate:
      return PrivateOperationIcon;
    default:
      return PackageOperationIcon;
    }
  }
}

void BrowserOperation::paintCell(QPainter * p, const QColorGroup & cg, int column,
				int width, int alignment) {
  const QColor & bg = p->backgroundColor();
  
  if (is_marked) {
    p->setBackgroundMode(OpaqueMode);
    p->setBackgroundColor(UmlRedColor);
  }
    
  if (def->get_isa_class_operation())
    p->setFont((is_writable()) ? BoldUnderlineFont : UnderlineFont);
  else if (def->get_is_abstract())
    p->setFont((is_writable()) ? BoldItalicFont : ItalicFont);
  else
    p->setFont((is_writable()) ? BoldFont : NormalFont);
  QListViewItem::paintCell(p, cg, column, width, alignment);
  
  if (is_marked) {
    p->setBackgroundMode(TransparentMode);
    p->setBackgroundColor(bg);
  }
}

void BrowserOperation::menu() {
  QPopupMenu m(0, name);
  QPopupMenu toolm(0);
  
  m.insertItem(new MenuTitle(name, m.font()), -1);
  m.insertSeparator();
  if (!deletedp()) {
    if (!is_edited) {
      m.setWhatsThis(m.insertItem("Edit", 0),
		     "to edit the <em>operation</em>, \
a double click with the left mouse button does the same thing");
      if (strstr(def->get_cppdef(), "${body}") != 0)
	m.setWhatsThis(m.insertItem("Edit C++ body", 4),
		       "to edit the <em>operation</em> and its C++ body");
      if (strstr(def->get_javadef(), "${body}") != 0)
	m.setWhatsThis(m.insertItem("Edit Java body", 5),
		       "to edit the <em>operation</em> and its Java body");
      if (strstr(def->get_phpdef(), "${body}") != 0)
	m.setWhatsThis(m.insertItem("Edit Php body", 6),
		       "to edit the <em>operation</em> and its Php body");
      if (strstr(def->get_pythondef(), "${body}") != 0)
	m.setWhatsThis(m.insertItem("Edit Python body", 7),
		       "to edit the <em>operation</em> and its Python body");
      m.setWhatsThis(m.insertItem("Duplicate", 1),
		     "to copy the <em>operation</em> in a new one");
      if (!is_read_only && (edition_number == 0)) {
	m.insertSeparator();
	m.setWhatsThis(m.insertItem("Delete", 2),
		       "to delete the <em>operation</em>. \
Note that you can undelete it after");
      }
    }
    mark_menu(m, "operation", 90);
    if ((edition_number == 0) &&
	Tool::menu_insert(&toolm, get_type(), 100)) {
      m.insertSeparator();
      m.insertItem("Tool", &toolm);
    }
  }
  else if (!is_read_only && (edition_number == 0))
    m.setWhatsThis(m.insertItem("Undelete", 3),
		   "to undelete the <em>operation</em>");
  
  exec_menu_choice(m.exec(QCursor::pos()));
}

void BrowserOperation::exec_menu_choice(int rank) {
  switch (rank) {
  case 0:
    open(FALSE);
    return;
  case 1:
    ((BrowserClass *) parent())->add_operation(this);
    return;
  case 2:
    delete_it();
    break;
  case 3:
    BrowserNode::undelete(FALSE);
    break;
  case 4:
    def->edit(CppView);
    return;
  case 5:
    def->edit(JavaView);
    return;
  case 6:
    def->edit(PhpView);
    return;
  case 7:
    def->edit(PythonView);
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

void BrowserOperation::apply_shortcut(QString s) {
  int choice = -1;

  if (!deletedp()) {
    if (!is_edited) {
      if (s == "Edit")
	choice = 0;
      else if (s == "Duplicate")
	choice = 1;
      if (strstr(def->get_cppdef(), "${body}") != 0)
	if (s == "Edit C++ body")
	  choice = 4;
      if (strstr(def->get_javadef(), "${body}") != 0)
	if (s == "Edit Java body")
	  choice = 5;
      if (strstr(def->get_phpdef(), "${body}") != 0)
	if (s == "Edit Php body")
	  choice = 6;
      if (strstr(def->get_pythondef(), "${body}") != 0)
	if (s == "Edit Python body")
	  choice = 7;
      if (s == "Duplicate")
	choice = 1;
      if (!is_read_only && (edition_number == 0)) {
	if (s == "Delete")
	  choice = 2;
      }
    }
    mark_shortcut(s, choice, 90);
    if (edition_number == 0)
      Tool::shortcut(s, choice, get_type(), 100);
  }
  else if (!is_read_only && (edition_number == 0))
    if (s == "Undelete")
      choice = 3;
  
  exec_menu_choice(choice);
}

void BrowserOperation::open(bool) {
  if (!is_edited)
    def->edit(UmlView);
}

void BrowserOperation::modified() {
  repaint();
  ((BrowserNode *) parent())->modified();
}

UmlCode BrowserOperation::get_type() const {
  return UmlOperation;
}

int BrowserOperation::get_identifier() const {
  return get_ident();
}

BasicData * BrowserOperation::get_data() const {
  return def;
}

bool BrowserOperation::allow_spaces() const {
  return FALSE;
}

bool BrowserOperation::same_name(const QString &, UmlCode) const {
  // no check
  return FALSE;
}

QString BrowserOperation::full_name(bool rev, bool) const {
  QString p = ((BrowserNode *) parent())->full_name(FALSE, FALSE);

  return (rev) ? name + "   [" + p + "]"
	       : p + "::" + name;
}

void BrowserOperation::member_cpp_def(const QString & prefix,
				      const QString & prefix_tmplop, 
				      QString & s, bool templ) const {
  if (! templ) {
    const char * df = def->get_cppdef();
    
    if (!def->get_cpp_inline() && df[0]) {
      s += (def->is_template_operation()) ? prefix_tmplop : prefix;
      s += true_name(get_name(), df);
      s += "(...){...}\n\n";
    }
  }
}

void BrowserOperation::compute_referenced_by(QList<BrowserNode> & l,
					     BrowserClass * target)
{
  IdIterator<BrowserOperation> it(all);
  
  while (it.current()) {
    if (!it.current()->deletedp() && it.current()->def->reference(target))
      l.append(it.current());
    ++it;
  }
}

QString BrowserOperation::python_init_self(BrowserNode * cl)
{
  QListViewItem * child;
  
  for (child = cl->firstChild(); child; child = child->nextSibling()) {
    if ((((BrowserNode *) child)->get_type() == UmlOperation) &&
	!strcmp(((BrowserNode *) child)->get_name(), "__init__")) {
      return (((BrowserOperation *) child)->def->get_n_params() != 0)
	? ((BrowserOperation *) child)->def->get_param_name(0)
	: "self";
    }
  }
  
  // not find
  return "self";
}

bool BrowserOperation::tool_cmd(ToolCom * com, const char * args) {
  switch ((unsigned char) args[-1]) {
  case supportFileCmd:
    com->write_string(((BrowserClass *) parent())->bodies_file());
    return TRUE;
  case getIdCmd:
    // not for a user, old plug-out
    com->write_unsigned(get_ident());
    return TRUE;
  default:
    return (def->tool_cmd(com, args, this, comment) ||
	    BrowserNode::tool_cmd(com, args));
  }
}

void BrowserOperation::DropAfterEvent(QDropEvent * e, BrowserNode * after) {
  if (after == 0)
    ((BrowserNode *) parent())->DropAfterEvent(e, this);
  else
    e->ignore();
}

QString BrowserOperation::drag_key() const {
  return ((get_of == 0) && (set_of == 0))
    ? QString::number(UmlOperation)
    : QString::number(UmlOperation)
      + "#" + QString::number((unsigned long) parent());
}

QString BrowserOperation::drag_key(BrowserNode * p)
{
  return QString::number(UmlOperation)
    + "#" + QString::number((unsigned long) p);
}

//

// unicode
const QStringList & BrowserOperation::default_stereotypes()
{
  return its_default_stereotypes;
}

void BrowserOperation::save_stereotypes(QTextStream & st)
{
  nl_indent(st);
  st << "operation_stereotypes ";
  save_unicode_string_list(its_default_stereotypes, st);
}

void BrowserOperation::read_stereotypes(char * & st, char * & k)
{
  if (!strcmp(k, "operation_stereotypes")) {
    read_unicode_string_list(its_default_stereotypes, st);
    k = read_keyword(st);
  }
}

void BrowserOperation::save(QTextStream & st, bool ref, QString & warning) {
  if (ref)
    st << "operation_ref " << get_ident() << " // " << get_name();
  else {
    nl_indent(st);
    st << "operation " << get_ident() << ' ';
    save_string(name, st);
    indent(+1);
    def->save(st, FALSE, warning);
    
    if ((get_of != 0) && !get_of->deletedp()) {
      nl_indent(st);
      st << ((get_of->get_type() == UmlAttribute) ? "get_of_attribute " 
						 : "get_of_relation ");
      get_of->save(st, TRUE, warning);
    }
    else if ((set_of != 0) && !set_of->deletedp()) {
      nl_indent(st);
      st << ((set_of->get_type() == UmlAttribute) ? "set_of_attribute " 
						 : "set_of_relation ");
      set_of->save(st, TRUE, warning);
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

BrowserOperation * BrowserOperation::read_ref(char * & st)
{
  read_keyword(st, "operation_ref");

  int id = read_id(st);
  BrowserOperation * result = all[id];
  
  return (result == 0)
    ? new BrowserOperation(id)
    : result;
}

BrowserOperation * BrowserOperation::read(char * & st, char * k,
					  BrowserNode * parent,
					  bool force)
{
  BrowserOperation * result;
  int id;

  if (!strcmp(k, "operation_ref")) {
    if (((result = all[id = read_id(st)]) == 0) && force)
      result = new BrowserOperation(id);
    return result;
  }
  else if (!strcmp(k, "operation")) {
    id = read_id(st);

    QString s = read_string(st);
        
    if ((result = all[id]) == 0)
      result = new BrowserOperation(s, parent, new OperationData(id), id);
    else if (result->is_defined) {
      BrowserOperation * already_exist = result;

      result = new BrowserOperation(s, parent, new OperationData(id), id);

      already_exist->must_change_id(all);
      already_exist->unconsistent_fixed("operation", result);
    }
    else {
      result->set_parent(parent);
      result->set_name(s);
    }

    result->is_defined = TRUE;
    result->def->read(st, k);	// updates k
    
    result->is_read_only = !in_import() && read_only_file() || 
      (user_id() != 0) && result->is_api_base();
    
    result->def->set_browser_node(result, FALSE);
    
    if (!strcmp(k, "get_of_attribute") ||
	!strcmp(k, "get_of_attribut")) {
      BrowserAttribute * att = BrowserAttribute::read_ref(st);
      
      if (att != 0)
	result->set_get_of(att);
      
      k = read_keyword(st);
    }
    else if (!strcmp(k, "set_of_attribute") ||
	     !strcmp(k, "set_of_attribut")) {
      BrowserAttribute * att = BrowserAttribute::read_ref(st);
      
      if (att != 0)
	result->set_set_of(att);
      
      k = read_keyword(st);
    }
    if (!strcmp(k, "get_of_relation")) {
      BrowserRelation * rel = BrowserRelation::read_ref(st);
      
      if (rel != 0)
	result->set_get_of(rel);
      
      k = read_keyword(st);
    }
    else if (!strcmp(k, "set_of_relation")) {
      BrowserRelation * rel = BrowserRelation::read_ref(st);
      
      if (rel != 0)
	result->set_set_of(rel);
      
      k = read_keyword(st);
    }
    
    result->BrowserNode::read(st, k);	// updates k
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    return result;
  }
  else
    return 0;
}

BrowserNode * BrowserOperation::get_it(const char * k, int id)
{
  return (!strcmp(k, "operation_ref")) ? all[id] : 0;
}
