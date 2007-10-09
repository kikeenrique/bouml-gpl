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

#include <qcursor.h>
#include <qfile.h>

#include "OperationData.h"
#include "ClassData.h"
#include "ParamData.h"
#include "ExceptionData.h"
#include "KeyValueData.h"
#include "BrowserClass.h"
#include "OperationDialog.h"
#include "BrowserOperation.h"
#include "ClassDialog.h"
#include "GenerationSettings.h"
#include "myio.h"
#include "ToolCom.h"
#include "strutil.h"
#include "BrowserView.h"
#include "mu.h"

IdDict<OperationData> OperationData::all(1023, __FILE__);

OperationData::OperationData(int id)
    : Labeled<OperationData>(all, id),
      uml_visibility(UmlDefaultVisibility),
      cpp_visibility(UmlDefaultVisibility),
      is_deleted(FALSE), is_get_or_set(FALSE),
      isa_class_operation(FALSE), is_abstract(FALSE), is_volatile(FALSE), 
      cpp_const(FALSE), cpp_friend(FALSE), cpp_virtual(FALSE),
      cpp_inline(FALSE), cpp_get_set_frozen(FALSE),
      java_final(FALSE), java_synchronized(FALSE), java_get_set_frozen(FALSE),
      php_final(FALSE), php_get_set_frozen(FALSE),
      idl_oneway(FALSE), idl_get_set_frozen(FALSE),
      nparams(0), nexceptions(0), params(0), exceptions(0) {
}

OperationData::OperationData(OperationData * model, BrowserNode * bn)
    : ClassMemberData(model), Labeled<OperationData>(all, 0),
      uml_visibility(model->uml_visibility),
      cpp_visibility(UmlDefaultVisibility),
      is_deleted(FALSE), is_get_or_set(FALSE),
      isa_class_operation(model->isa_class_operation),
      is_abstract(model->is_abstract), is_volatile(model->is_volatile),
      cpp_const(model->cpp_const),
      cpp_friend(model->cpp_friend), cpp_virtual(model->cpp_virtual),
      cpp_inline(model->cpp_inline), cpp_get_set_frozen(model->cpp_get_set_frozen),
      java_final(model->java_final), java_synchronized(model->java_synchronized),
      java_get_set_frozen(model->java_get_set_frozen),
      php_final(model->php_final), php_get_set_frozen(model->php_get_set_frozen),
      idl_oneway(model->idl_oneway), idl_get_set_frozen(model->idl_get_set_frozen), 
      nparams(model->nparams),
      nexceptions(model->nexceptions),
      constraint(model->constraint),
      cpp_decl(model->cpp_decl),
      java_annotation(model->java_annotation),
      idl_decl(model->idl_decl) {
  browser_node = bn;
  
  cpp_def.assign((const char *) model->cpp_def, FALSE);
  java_def.assign((const char *) model->java_def, FALSE);
  php_def.assign((const char *) model->php_def, FALSE);
  return_type = model->return_type;
  depend_on(return_type.type);
  
  if (nparams == 0)
    params = 0;
  else {
    params = new ParamData[nparams];
    
    for (unsigned i = 0; i != nparams; i += 1) {
      params[i] = model->params[i];
      depend_on(params[i].get_type().type);
    }
  }
  
  if (nexceptions == 0)
    exceptions = 0;
  else {
    exceptions = new ExceptionData[nexceptions];
    
    for (unsigned i = 0; i != nexceptions; i += 1) {
      exceptions[i] = model->exceptions[i];
      depend_on(exceptions[i].get_type().type);
    }
  }
}

OperationData::~OperationData() {
  if (params)
    delete [] params;
  if (exceptions)
    delete [] exceptions;

  // do not disconnect : deletions may be already made
  unsubscribe_all();
  
  all.remove(get_ident());
}

bool OperationData::deletedp() const {
  return is_deleted;
}

void OperationData::set_deletedp(bool y) {
  is_deleted = y;
  
  if (y)
    create_modified_body_file();
}

void OperationData::clear(bool old)
{
  all.clear(old);
}

void OperationData::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void OperationData::renumber(int phase) {
  new_ident(phase, all);
}

bool OperationData::is_template_operation() const {
  return cpp_decl.isEmpty() && is_template_operation(cpp_def);
}

bool OperationData::is_template_operation(QString def) const {
  int index1 = def.find("${class}");
  
  if (index1 == -1)
    return FALSE;
  
  int index2 = def.find("${name}", index1 + 8);
  
  if (index2 == -1)
    return FALSE;
  
  def = def.mid(index1 + 8, index2 - index1 - 8);
  
  if ((index1 = def.find('<')) == -1)
    return FALSE;
  
  return (def.find('>') > index1);
}

void OperationData::depend_on(BrowserClass * cl) {
  if (cl != 0) {
    BasicData * def = cl->get_data();
    
    if (subscribe(def)) {
      connect(def, SIGNAL(deleted()), this, SLOT(on_delete()));
    }
  }
}

void OperationData::on_delete() {
  if (return_type.type && return_type.type->deletedp()) {
    return_type.explicit_type = return_type.type->get_name();
    return_type.type = 0;
  }
  
  unsigned short i;
  
  for (i = 0; i != nparams; i += 1) {
    AType t = params[i].get_type();
    
    if (t.type && t.type->deletedp()) {
      t.explicit_type = t.type->get_name();
      t.type = 0;
      
      params[i].set_type(t);
    }
  }
  
  for (i = 0; i != nexceptions; i += 1) {
    AType t = exceptions[i].get_type();
    
    if (t.type && t.type->deletedp()) {
      t.explicit_type = t.type->get_name();
      t.type = 0;
      
      exceptions[i].set_type(t);
    }
  }
  
  modified();
}

QString OperationData::default_cpp_decl(const QString & name) {
  QString s = GenerationSettings::cpp_default_oper_decl();
  QString parent_name = ((BrowserNode *) browser_node->parent())->get_name();
  
  if ((name == parent_name) || (name == QString("~") + parent_name)) {
    // constructor or destructor
    int index;
    
    if ((index = s.find("${friend}")) != -1)
      s.remove(index, 9);
    if ((index = s.find("${static}")) != -1)
      s.remove(index, 9);
    if ((index = s.find("${type}")) != -1)
      s.remove(index, (s.at(index + 7) == QChar(' ')) ? 8 : 7);
    if ((index = s.find("${const}")) != -1)
      s.remove(index, 8);
    if ((index = s.find("${abstract}")) != -1)
      s.remove(index, 11);
    if ((name.at(0) != QChar('~')) && ((index = s.find("${virtual}")) != -1))
      s.remove(index, 10);
  }
  
  return s;
}

QString OperationData::default_cpp_def(const QString & name) {
  QString s = GenerationSettings::cpp_default_oper_def();
  QString parent_name = ((BrowserNode *) browser_node->parent())->get_name();
  
  if ((name == parent_name) || (name == QString("~") + parent_name)) {
    // constructor or destructor
    int index;
    
    if ((index = s.find("${type}")) != -1)
      s.remove(index, (s.at(index + 7) == QChar(' ')) ? 8 : 7);
    if ((index = s.find("${const}")) != -1)
      s.remove(index, 8);
    if ((index = s.find("${staticnl}")) != -1)
      s.replace(index, 11, " ");
  }
  
  return s;
}

QString OperationData::default_java_def(const QString & name) {
  QString s = GenerationSettings::java_default_oper_def();
  QString parent_name = ((BrowserNode *) browser_node->parent())->get_name();
  
  if (name == parent_name) {
    // constructor
    int index;
    
    if ((index = s.find("${static}")) != -1)
      s.remove(index, 9);
    if ((index = s.find("${abstract}")) != -1)
      s.remove(index, 11);
    if ((index = s.find("${type}")) != -1)
      s.remove(index, (s.at(index + 7) == QChar(' ')) ? 8 : 7);
    if ((index = s.find("${staticnl}")) != -1)
      s.replace(index, 11, " ");
  } 
  else {
    for (const char * cpponly = "()&^[]%|!+-*/=<>~";
	 *cpponly;
	 cpponly += 1) {
      if (name.find(*cpponly) != -1) {
	// operator or destructor
	return QString::null;
      }
    }
  }
  
  return s;
}

QString OperationData::default_php_def(const QString & name, bool nobody) {
  QString s = GenerationSettings::php_default_oper_def();
  QString parent_name = ((BrowserNode *) browser_node->parent())->get_name();
  
  if ((name == parent_name) ||
      (name == "__construct") || (name == "__destruct")) {
    // constructor destructor
    int index;
    
    if ((index = s.find("${static}")) != -1)
      s.remove(index, 9);
  }
  
  if (nobody) {
    int index = s.find("${)}");
    
    if (index != -1)
      s = s.left(index + 4) + ";";
  }
  
  return s;
}

QString OperationData::default_idl_decl(const QString & name) {
  QString s = GenerationSettings::idl_default_oper_decl();
  QString parent_name = ((BrowserNode *) browser_node->parent())->get_name();
  
  if (name == parent_name) {
    // constructor
    int index = s.find("${name}");
    
    if ((index != -1) &&
	(s.find("factory") == -1) &&
	(s.find("finder") == -1))
      s.insert(index, "factory init_");
    
    if ((index = s.find("${type}")) != -1)
      s.remove(index, (s.at(index + 7) == QChar(' ')) ? 8 : 7);
  } 
  else {
    for (const char * cpponly = "()&^[]%|!+-*/=<>~";
	 *cpponly;
	 cpponly += 1) {
      if (name.find(*cpponly) != -1) {
	// operator or destructor
	return QString::null;
      }
    }
  }
  
  return s;
}

void OperationData::set_browser_node(BrowserOperation * o, bool update) {
  BasicData::set_browser_node(o);
  
  if (update) {
    if (uml_visibility == UmlDefaultVisibility)
      uml_visibility = 
	((BrowserNode *) browser_node->parent())->get_visibility(UmlOperation);
    
    QString st = ((BrowserClass *) o->parent())->get_stereotype();
    
    if (GenerationSettings::cpp_get_default_defs()) {
      if (ClassDialog::cpp_stereotype(st) != "enum") {
	cpp_decl = default_cpp_decl(browser_node->get_name());
	cpp_def.assign(default_cpp_def(browser_node->get_name()), TRUE);
      }
      else {
	cpp_decl = "";
	cpp_def.assign(0, FALSE);
      }
    }
    
    if (GenerationSettings::java_get_default_defs())
      java_def.assign(default_java_def(browser_node->get_name()), TRUE);
    
    if (GenerationSettings::php_get_default_defs())
      php_def.assign(default_php_def(browser_node->get_name(),
				     ClassDialog::php_stereotype(st) == "interface"),
		     TRUE);
    
    if (GenerationSettings::idl_get_default_defs()) {
      if (ClassDialog::idl_stereotype(st) != "enum")
	idl_decl = default_idl_decl(browser_node->get_name());
      else
	idl_decl = "";
    }
  }
}

QString OperationData::definition(bool full) const {
  return definition(full, TRUE, TRUE);
}

QString OperationData::definition(bool full, bool withdir, bool withname) const {
  QString result;
  
  if (full) {
    if (nparams == 0)
      result = browser_node->get_name() + QString("()");
    else {
      result = browser_node->get_name();
      const char * sep = "(";
      unsigned index;
      
      for (index = 0; index != nparams; index += 1) {
	result += sep;
	result += params[index].definition(withdir, withname);
	sep = ", ";
      }
      result += ")";
    }
    
    QString rt = get_return_type().get_type();
    
    return (rt.isEmpty()) ? result : result + " : " + rt;
  }
  else
    return browser_node->get_name() + QString("()");
}

QString OperationData::definition(bool full, DrawingLanguage language,
				  bool withdir, bool withname) const {
  switch (language) {
  case UmlView:
    return definition(full, withdir, withname);
  case CppView:
    if (full)
      return OperationDialog::cpp_decl((BrowserOperation *) browser_node, withname);
    else if (!cpp_decl.isEmpty())
      return definition(FALSE);
    else
      return QString::null;
  case JavaView:
    if (full)
      return OperationDialog::java_decl((BrowserOperation *) browser_node, withname);
    else if (!java_def.isEmpty())
      return definition(FALSE);
    else
      return QString::null;
  case PhpView:
    if (full)
      return OperationDialog::php_decl((BrowserOperation *) browser_node, withname);
    else if (!php_def.isEmpty())
      return definition(FALSE);
    else
      return QString::null;
  default:
    if (full)
      return OperationDialog::idl_decl((BrowserOperation *) browser_node, withdir, withname);
    else if (!idl_decl.isEmpty())
      return definition(FALSE);
    else
      return QString::null;
  }
}

bool OperationData::decldefbody_contain(const QString & s, bool cs,
					BrowserNode *) {
  return ((QString(get_cppdecl()).find(s, 0, cs) != -1) ||
	  (QString(get_cppdef()).find(s, 0, cs) != -1) ||
	  (QString(get_javadef()).find(s, 0, cs) != -1) ||
	  (QString(get_phpdef()).find(s, 0, cs) != -1) ||
	  (QString(get_idldecl()).find(s, 0, cs) != -1) ||
	  (QString(get_body('c')).find(s, 0, cs) != -1) ||
	  (QString(get_body('j')).find(s, 0, cs) != -1) ||
	  (QString(get_body('p')).find(s, 0, cs) != -1));
}

UmlVisibility OperationData::get_visibility(BrowserNode *) {
  return uml_visibility;
}

void OperationData::set_is_abstract(bool yes) {
  is_abstract = yes;
  if (yes) {
    ClassData * d = (ClassData *) ((BrowserNode *) browser_node->parent())->get_data();
    
    if (strcmp(d->get_stereotype(), "enum"))
      d->set_is_abstract(TRUE);
  }
}

void OperationData::set_return_type(const AType & t) {
  if (return_type.type != t.type) {
    if ((return_type.type != 0) && unsubscribe(return_type.type->get_data()))
      disconnect(return_type.type->get_data(), 0, this, 0);
    depend_on(return_type.type = t.type);
  }

  return_type.explicit_type = t.explicit_type;
}

const char * OperationData::get_param_name(int rank) const {
  return params[rank].get_name();
}

void OperationData::set_param_name(int rank, const char * str) {
  params[rank].set_name(str);
}

UmlParamDirection OperationData::get_param_dir(int rank) const {
  return params[rank].get_dir();
}

void OperationData::set_param_dir(int rank, UmlParamDirection dir) {
  params[rank].set_dir(dir);
}

const AType & OperationData::get_param_type(int rank) const {
  return params[rank].get_type();
}

const char * OperationData::get_param_default_value(int rank) const {
  return params[rank].get_default_value();
}

void OperationData::set_param_default_value(int rank, const char * str) {
  params[rank].set_default_value(str);
}

void OperationData::set_param_type(int rank, const AType & t) {
  const AType & old = params[rank].get_type();
  
  if (old.type != t.type) {
    if ((old.type != 0) && unsubscribe(old.type->get_data()))
      disconnect(old.type->get_data(), 0, this, 0);
    depend_on(t.type);
  }

  params[rank].set_type(t);
}

void OperationData::set_n_params(unsigned n) {
  if (n > nparams) {
    if (params)
      delete [] params;
    params = new ParamData[n];
  }
  
  nparams = n;
}

const AType & OperationData::get_exception(int rank) const {
  return exceptions[rank].get_type();
}

void OperationData::set_exception(int rank, const AType & t) {
  const AType & old = exceptions[rank].get_type();
  
  if (old.type != t.type) {
    if ((old.type != 0) && unsubscribe(old.type->get_data()))
      disconnect(old.type->get_data(), 0, this, 0);
    depend_on(t.type);
  }

  exceptions[rank].set_type(t);
}

void OperationData::set_n_exceptions(unsigned n) {
  if (n > nexceptions) {
    if (exceptions)
      delete [] exceptions;
    exceptions = new ExceptionData[n];
  }
  
  nexceptions = n;
}

void OperationData::edit(DrawingLanguage l) {
  setName(browser_node->get_name());
    
  (new OperationDialog(this, l))->show();
}

//

void OperationData::update_cpp_get_of(QCString & decl, QCString & def,
				      const QString & attr_name,
				      QString attcpp_decl, bool attis_const,
				      QString multiplicity)
{
  attcpp_decl = attcpp_decl.stripWhiteSpace();
 
  int index;
  
  if ((index = attcpp_decl.find("${comment}")) != -1)
    attcpp_decl.remove(index, 10);
  if ((index = attcpp_decl.find("${description}")) != -1)
    attcpp_decl.remove(index, 14);
  if ((index = attcpp_decl.find("${visibility}")) != -1)
    attcpp_decl.remove(index, 13);
  if ((index = attcpp_decl.find("${static}")) != -1)
    attcpp_decl.remove(index, 9);
  if ((index = attcpp_decl.find("${mutable}")) != -1)
    attcpp_decl.remove(index, 10);
  if ((index = attcpp_decl.find("${volatile}")) != -1)
    attcpp_decl.remove(index, 11);
  if ((index = attcpp_decl.find("${value}")) != -1)
    attcpp_decl.remove(index, 8);
  if ((index = attcpp_decl.find("${h_value}")) != -1)
    attcpp_decl.remove(index, 10);
  if ((index = attcpp_decl.find(";")) != -1)
    attcpp_decl.remove(index, 1);
  
  QString attr_name_spec = extract_name(attcpp_decl);
  
  if (attr_name_spec.isEmpty()) {
    decl = 0;
    def = 0;
  }
  else {  
    int index2;
    bool has_multiplicity;
    QString mult;
    QString type_spec_name_spec = attcpp_decl;
    QString type_spec_name;
    bool has_stereotype = (attcpp_decl.find("${stereotype}") != -1);
    
    type_spec_name_spec.replace(type_spec_name_spec.find(attr_name_spec),
				attr_name_spec.length(), "${name}");
    
    if ((index = type_spec_name_spec.find("${multiplicity}")) != -1) {
      type_spec_name_spec.remove(index, 15);
      if ((index = multiplicity.find(']')) != -1) {
	mult = QString(")") + multiplicity.right(index + 1);
	has_multiplicity = TRUE;
      }
      else
	has_multiplicity = FALSE;
    }
    else
      has_multiplicity = FALSE;
    
    if ((index = type_spec_name_spec.find("${const}")) != -1) {
      if (attis_const || GenerationSettings::cpp_default_get_value_const())
	type_spec_name_spec.replace(index, 8, "const ");
      else
	type_spec_name_spec.remove(index, 8);
    }
    
    // manage the declaration
    
    QString type_spec_oper_name = type_spec_name_spec;
    
    if ((index = type_spec_oper_name.find("${name}")) != -1)
      type_spec_oper_name.replace(index, 7, (has_multiplicity) ? "(* $$" : "$$");
    
    QCString d = (const char *) GenerationSettings::cpp_default_oper_decl();
    
    if (((index = d.find("${type}")) != -1) &&
	((index2 = d.find("${name}", index + 1)) != -1)) {
      d.replace(index, index2 - index + 7, type_spec_oper_name);
      
      if ((index = d.find("${abstract}")) != -1)
	d.remove(index, 11);
      
      if ((index = d.find("$$")) != -1)
	d.replace(index, 2, (has_stereotype) ? "& ${name}" : "${name}");
      
      if (has_multiplicity) {
	if ((index = d.find("${)}")) != -1)
	  d.insert(index + 4, mult);
	else
	  d = 0;
      }
      
      decl = d;
    }
    else
      decl = 0;
    
    // manage the definition
    
    type_spec_name = type_spec_name_spec;
    
    if ((index = type_spec_name.find("${name}")) != -1)
      type_spec_name.replace(index, 7,
			     (has_multiplicity) 
			     ? "(* ${class}::$$"
			     : (has_stereotype) ? "& ${class}::$$"
						: "${class}::$$");
    
    d = GenerationSettings::cpp_default_oper_def();
    
    if (((index = d.find("${type}")) != -1) &&
	((index2 = d.find("${name}", index + 1)) != -1)) {
      d.replace(index, index2 - index + 7, type_spec_name);
      
      QString attr_full_name = attr_name_spec;
      
      if ((index = attr_full_name.find("${name}")) != -1)
	attr_full_name.replace(index, 7, attr_name);
      
      if ((index = d.find("$$")) != -1)
	d.replace(index, 2, "${name}");
      
      if (has_multiplicity) {
	if ((index = d.find("${)}")) != -1)
	  d.insert(index + 4, mult);
	else
	  d = 0;
      }
      
      if ((index = d.find("${body}")) != -1) {
	QString indent;
	QString end = (d[index + 7] == '}') ? ";\n" : ";";
	
	if (d[index - 1] == '\n')
	  indent = "  ";
	
	d.replace(index, 7, indent + "return " + attr_full_name + end);
      }
      def = d;
    }
    else
      def = 0;
  }
}

void OperationData::update_java_get_of(QCString & def, const QString & attr_name,
				       QString attjava_decl)
{
  attjava_decl = attjava_decl.stripWhiteSpace();
  
  int index;
  
  if ((index = attjava_decl.find("${comment}")) != -1)
    attjava_decl.remove(index, 10);
  if ((index = attjava_decl.find("${@}")) != -1)
    attjava_decl.remove(index, 4);
  if ((index = attjava_decl.find("${description}")) != -1)
    attjava_decl.remove(index, 14);
  if ((index = attjava_decl.find("${visibility}")) != -1)
    attjava_decl.remove(index, 13);
  if ((index = attjava_decl.find("${static}")) != -1)
    attjava_decl.remove(index, 9);
  if ((index = attjava_decl.find("${final}")) != -1)
    attjava_decl.remove(index, 8);
  if ((index = attjava_decl.find("${transient}")) != -1)
    attjava_decl.remove(index, 12);
  if ((index = attjava_decl.find("${volatile}")) != -1)
    attjava_decl.remove(index, 11);
  if ((index = attjava_decl.find("${value}")) != -1)
    attjava_decl.remove(index, 8);
  if ((index = attjava_decl.find(";")) != -1)
    attjava_decl.remove(index, 1);
  
  QString attr_name_spec = extract_name(attjava_decl);
  
  if (attr_name_spec.isEmpty())
    def = QString::null;
  else {
    int index2;
    QCString d = (const char *) GenerationSettings::java_default_oper_def();
    QString type_spec_name = attjava_decl;
    
    type_spec_name.replace(type_spec_name.find(attr_name_spec),
			   attr_name_spec.length(), "$$");
    
    if (((index = d.find("${type}")) != -1) &&
	((index2 = d.find("${name}", index + 1)) != -1)) {
      d.replace(index, index2 - index + 7, type_spec_name);
      
      QString attr_full_name = attr_name_spec;
      
      if ((index = attr_full_name.find("${name}")) != -1)
	attr_full_name.replace(index, 7, attr_name);
      
      if ((index = d.find("$$")) != -1)
	d.replace(index, 2, "${name}");
      
      if ((index = d.find("${body}")) != -1) {
	QString indent;
	QString end = (d[index + 7] == '}') ? ";\n" : ";";
	
	if (d[index - 1] == '\n')
	  indent = "  ";
	
	d.replace(index, 7, indent + "return " + attr_full_name + end);
      }
      def = d;
    }
    else
      def = 0;
  }
}

void OperationData::update_php_get_of(QCString & def, const QString & attr_name,
				      QString attphp_decl)
{
  attphp_decl = attphp_decl.stripWhiteSpace();
  
  int index;
  
  if ((index = attphp_decl.find("${comment}")) != -1)
    attphp_decl.remove(index, 10);
  if ((index = attphp_decl.find("${description}")) != -1)
    attphp_decl.remove(index, 14);
  if ((index = attphp_decl.find("${visibility}")) != -1)
    attphp_decl.remove(index, 13);
  if ((index = attphp_decl.find("${static}")) != -1)
    attphp_decl.remove(index, 9);
  if ((index = attphp_decl.find("${var}")) != -1)
    attphp_decl.remove(index, 6);
  if ((index = attphp_decl.find("${const}")) != -1)
    attphp_decl.remove(index, 8);
  if ((index = attphp_decl.find("${value}")) != -1)
    attphp_decl.remove(index, 8);
  if ((index = attphp_decl.find(";")) != -1)
    attphp_decl.remove(index, 1);
  
  QString attr_name_spec = extract_name(attphp_decl);
  
  if (attr_name_spec.isEmpty())
    def = QString::null;
  else {
    QCString d = (const char *) GenerationSettings::php_default_oper_def();
    QString type_spec_name = attphp_decl;
    
    type_spec_name.replace(type_spec_name.find(attr_name_spec),
			   attr_name_spec.length(), "$$");
    
    if ((index = d.find("${name}")) != -1) {
      d.replace(index, 7, type_spec_name);

      QString attr_full_name = attr_name_spec;
      
      if ((index = attr_full_name.find("${name}")) != -1)
	attr_full_name.replace(index, 7, attr_name);
      
      if ((index = d.find("$$")) != -1)
	d.replace(index, 2, "${name}");
      
      if ((index = d.find("${body}")) != -1) {
	QString indent;
	QString end = (d[index + 7] == '}') ? ";\n" : ";";
	
	if (d[index - 1] == '\n')
	  indent = "  ";
	
	d.replace(index, 7, indent + "return $this->" + attr_full_name + end);
      }
      def = d;
    }
    else
      def = 0;
  }
}

void OperationData::update_idl_get_of(QCString & decl, QString attidl_decl,
				      QString multiplicity)
{
  attidl_decl = attidl_decl.stripWhiteSpace();
  
  int index;
  
  if ((index = attidl_decl.find("${comment}")) != -1)
    attidl_decl.remove(index, 10);
  if ((index = attidl_decl.find("${description}")) != -1)
    attidl_decl.remove(index, 14);
  if ((index = attidl_decl.find("${visibility}")) != -1)
    attidl_decl.remove(index, 13);
  if ((index = attidl_decl.find("${readonly}")) != -1)
    attidl_decl.remove(index, 11);
  if ((index = attidl_decl.find("${attribut}")) != -1)
    // old version
    attidl_decl.remove(index, 11);
  if ((index = attidl_decl.find("${attribute}")) != -1)
    attidl_decl.remove(index, 12);
  if ((index = attidl_decl.find("const ")) != -1)
    attidl_decl.remove(index, 6);
  if ((index = attidl_decl.find("${value}")) != -1)
    attidl_decl.remove(index, 8);
  if ((index = attidl_decl.find(";")) != -1)
    attidl_decl.remove(index, 1);
  
  QString attr_name_spec = extract_name(attidl_decl);
  
  if (attr_name_spec.isEmpty())
    decl = 0;
  else {
    QCString d = (const char *) GenerationSettings::idl_default_oper_decl();
    int index2;
    QString mult;
    QString type_spec_name = attidl_decl;
    
    type_spec_name.replace(type_spec_name.find(attr_name_spec),
			   attr_name_spec.length(), "$$");
    
    if (((index = d.find("${type}")) != -1) &&
	((index2 = d.find("${name}", index + 1)) != -1)) {
      d.replace(index, index2 - index + 7, type_spec_name);
      
      if ((index = d.find("$$")) != -1)
	d.replace(index, 2, "${name}");
      
      if ((index = d.find("${multiplicity}")) != -1)
	d.replace(index, 15, multiplicity);
      
      decl = d;
    }
    else
      decl = 0;
  }
}

void OperationData::update_get_of(const QString & attr_name,
				  QString attcpp_decl, QString attjava_decl,
				  QString attphp_decl, QString attidl_decl,
				  bool attis_const, bool attis_class_member,
				  const AType & cl, QString multiplicity,
				  QString relstereotype, bool create, bool update) {
  set_return_type(cl);
  isa_class_operation = attis_class_member;
  stereotype = relstereotype.stripWhiteSpace();
  is_get_or_set = TRUE;
  
  QString st = ((BrowserClass *) browser_node->parent())->get_stereotype();
  QCString decl;
  QCString def;
  
  // C++
  
  if (!update || !cpp_get_set_frozen) {
    if (ClassDialog::cpp_stereotype(st) != "enum") {
      if (create) {
	if (cpp_name_spec.isEmpty())
	  cpp_name_spec = GenerationSettings::cpp_default_get_name();
	cpp_const = GenerationSettings::cpp_default_get_const();
	cpp_inline = GenerationSettings::cpp_default_get_inline();
	if (GenerationSettings::cpp_default_get_visibility() !=
	    GenerationSettings::javaphp_default_get_visibility())
	  cpp_visibility = GenerationSettings::cpp_default_get_visibility();
      }
    
      update_cpp_get_of(decl, def, attr_name, attcpp_decl,
			attis_const, multiplicity);
      cpp_decl = decl;
      if (def.isEmpty())
	cpp_def.assign(0, TRUE);
      else
	cpp_def.assign(def, FALSE);
    }
    else {
      cpp_decl = "";
      cpp_def.assign(0, TRUE);
    }
  }
  
  // Java
  if (create) {
    if (java_name_spec.isEmpty())
      java_name_spec = GenerationSettings::java_default_get_name();
    java_final = GenerationSettings::java_default_get_final();  
    uml_visibility = GenerationSettings::javaphp_default_get_visibility();
  }

  if (!update || !java_get_set_frozen) {
    update_java_get_of(def, attr_name, attjava_decl);
    if (def.isEmpty())
      java_def.assign(QString::null, TRUE);
    else
      java_def.assign(def, FALSE);
  }
  
  // Php
  if (create) {
    if (php_name_spec.isEmpty())
      php_name_spec = GenerationSettings::php_default_get_name();
    php_final = GenerationSettings::php_default_get_final();  
  }

  if (!update || !php_get_set_frozen) {
    update_php_get_of(def, attr_name, attphp_decl);
    if (def.isEmpty())
      php_def.assign(QString::null, TRUE);
    else
      php_def.assign(def, FALSE);
  }
  
  // Idl
  if (!update || !idl_get_set_frozen) {
    if (ClassDialog::idl_stereotype(st) != "enum") {
      if (create && idl_name_spec.isEmpty())
	idl_name_spec = GenerationSettings::idl_default_get_name();
  
      update_idl_get_of(decl, attidl_decl, multiplicity);
      idl_decl = decl;
    }
    else {
      idl_decl = "";
    }
  }
}

void OperationData::update_cpp_set_of(QCString & decl, QCString & def,
				      const QString & attr_name,
				      QString attcpp_decl, bool attis_const, 
				      QString multiplicity)
{
  attcpp_decl = attcpp_decl.stripWhiteSpace();
  
  int index;
  
  if ((index = attcpp_decl.find("${comment}")) != -1)
    attcpp_decl.remove(index, 10);
  if ((index = attcpp_decl.find("${description}")) != -1)
    attcpp_decl.remove(index, 14);
  if ((index = attcpp_decl.find("${static}")) != -1)
    attcpp_decl.remove(index, 9);
  if ((index = attcpp_decl.find("${mutable}")) != -1)
    attcpp_decl.remove(index, 10);
  if ((index = attcpp_decl.find("${volatile}")) != -1)
    attcpp_decl.remove(index, 11);
  if ((index = attcpp_decl.find("${value}")) != -1)
    attcpp_decl.remove(index, 8);
  if ((index = attcpp_decl.find("${h_value}")) != -1)
    attcpp_decl.remove(index, 10);
  if ((index = attcpp_decl.find(";")) != -1)
    attcpp_decl.remove(index, 1);
  
  QString attr_name_spec = extract_name(attcpp_decl);
  
  if (attr_name_spec.isEmpty()) {
    decl = 0;
    def = 0;
  }
  else {  
    bool has_stereotype = (attcpp_decl.find("${stereotype}") != -1);
    bool byref = GenerationSettings::cpp_default_set_param_ref();
    QString arg_spec = attcpp_decl;
    bool has_multiplicity;
    
    if ((index = arg_spec.find("${multiplicity}")) != -1) {
      arg_spec.replace(index, 15, multiplicity);
      has_multiplicity = TRUE;
    }
    else
      has_multiplicity = FALSE;
    
    if (byref && ((index = arg_spec.find("${type}")) != -1)) {
      // don't add ref for a pointer or a ref
      int index2 = arg_spec.find("${name}", index + 7);

      if (index2 != -1) {
	QString modifiers = arg_spec.mid(index + 7, index2 - index - 7);

	byref = (modifiers.find("*") == -1) && (modifiers.find("&") == -1);
      }
    }

    arg_spec.replace(arg_spec.find(attr_name_spec),
		     attr_name_spec.length(),
		     (has_stereotype || byref) ? "& ${p0}" : "${p0}");
    
    if ((index = arg_spec.find("${type}")) != -1)
      arg_spec.replace(index, 7, "${t0}");
    
    if ((index = arg_spec.find("${const}")) != -1) {
      if (attis_const || GenerationSettings::cpp_default_set_param_const())
	arg_spec.replace(index, 8, "const ");
      else
	arg_spec.remove(index, 8);
    }
    
    QCString d = (const char *) GenerationSettings::cpp_default_oper_decl();
    
    if ((index = d.find("${)}")) != -1) {
      d.insert(index, arg_spec);
      
      if ((index = d.find("${abstract}")) != -1)
	d.remove(index, 11);
      
      decl = d;
    }
    else
      decl = 0;
    
    // definition
    
    d = (const char *) GenerationSettings::cpp_default_oper_def();
    
    if ((index = d.find("${)}")) != -1) {
      d.insert(index, arg_spec);
      
      QString attr_full_name = attr_name_spec;
      
      if ((index = attr_full_name.find("${name}")) != -1)
	attr_full_name.replace(index, 7, attr_name);
      
      if ((index = d.find("${body}")) != -1) {
	QString indent;
	QString end = (d[index + 7] == '}') ? ";\n" : ";";
	
	if (d[index - 1] == '\n')
	  indent = "  ";
	
	if (has_multiplicity)
	  d.replace(index, 7,
		    indent + "memcpy(" + attr_full_name + ", ${p0}, sizeof(${p0}))" + end);
	else
	  d.replace(index, 7,
		    indent + attr_full_name + " = ${p0}" + end);
	
	def = d;
      }
      else
	def = 0;
    }
    else
      def = 0;
  }
}

void OperationData::update_java_set_of(QCString & def,
				       const QString & attr_name,
				       QString attjava_decl)
{
  attjava_decl = attjava_decl.stripWhiteSpace();
  
  int index;
  
  if ((index = attjava_decl.find("${comment}")) != -1)
    attjava_decl.remove(index, 10);
  if ((index = attjava_decl.find("${@}")) != -1)
    attjava_decl.remove(index, 4);
  if ((index = attjava_decl.find("${description}")) != -1)
    attjava_decl.remove(index, 14);
  if ((index = attjava_decl.find("${visibility}")) != -1)
    attjava_decl.remove(index, 13);
  if ((index = attjava_decl.find("${static}")) != -1)
    attjava_decl.remove(index, 9);
  if ((index = attjava_decl.find("${final}")) != -1)
    attjava_decl.remove(index, 8);
  if ((index = attjava_decl.find("${transient}")) != -1)
    attjava_decl.remove(index, 12);
  if ((index = attjava_decl.find("${volatile}")) != -1)
    attjava_decl.remove(index, 11);
  if ((index = attjava_decl.find("${value}")) != -1)
    attjava_decl.remove(index, 8);
  if ((index = attjava_decl.find(";")) != -1)
    attjava_decl.remove(index, 1);
  
  QString attr_name_spec = extract_name(attjava_decl);
  
  if (attr_name_spec.isEmpty())
    def= 0;
  else {  
    QString arg_spec = attjava_decl;
    
    arg_spec.replace(arg_spec.find(attr_name_spec),
		     attr_name_spec.length(),"${p0}");
    
    if ((index = arg_spec.find("${type}")) != -1)
      arg_spec.replace(index, 7, "${t0}");
    
    if ((index = arg_spec.find("${final}")) != -1) {
      if (GenerationSettings::java_default_set_param_final())
	arg_spec.replace(index, 8, "final ");
      else
	arg_spec.remove(index, 8);
    }
    
    QCString d = (const char *) GenerationSettings::java_default_oper_def();
    
    if ((index = d.find("${)}")) != -1) {
      d.insert(index, arg_spec);
      
      QString attr_full_name = attr_name_spec;
      
      if ((index = attr_full_name.find("${name}")) != -1)
	attr_full_name.replace(index, 7, attr_name);
	
      if ((index = d.find("${body}")) != -1) {
	QString indent;
	QString end = (d[index + 7] == '}') ? ";\n" : ";";
	
	if (d[index - 1] == '\n')
	  indent = "  ";
	
	d.replace(index, 7, indent + attr_full_name + " = ${p0}" + end);
		
	def = d;
      }    
      else
	def = 0;
    }
    else
      def = 0;
  }
}

void OperationData::update_php_set_of(QCString & def,
				       const QString & attr_name,
				       QString attphp_decl)
{
  attphp_decl = attphp_decl.stripWhiteSpace();
  
  int index;
  
  if ((index = attphp_decl.find("${comment}")) != -1)
    attphp_decl.remove(index, 10);
  if ((index = attphp_decl.find("${description}")) != -1)
    attphp_decl.remove(index, 14);
  if ((index = attphp_decl.find("${visibility}")) != -1)
    attphp_decl.remove(index, 13);
  if ((index = attphp_decl.find("${static}")) != -1)
    attphp_decl.remove(index, 9);
  if ((index = attphp_decl.find("${var}")) != -1)
    attphp_decl.remove(index, 6);
  if ((index = attphp_decl.find("${const}")) != -1)
    attphp_decl.remove(index, 8);
  if ((index = attphp_decl.find("${value}")) != -1)
    attphp_decl.remove(index, 8);
  if ((index = attphp_decl.find(";")) != -1)
    attphp_decl.remove(index, 1);
  
  QString attr_name_spec = extract_name(attphp_decl);
  
  if (attr_name_spec.isEmpty())
    def= 0;
  else {  
    QString arg_spec = attphp_decl;
    
    arg_spec.replace(arg_spec.find(attr_name_spec),
		     attr_name_spec.length(),"${p0}");
    
    if ((index = arg_spec.find("${final}")) != -1) {
      arg_spec.remove(index, 8);
    }
    
    QCString d = (const char *) GenerationSettings::php_default_oper_def();
    
    if ((index = d.find("${)}")) != -1) {
      d.insert(index, arg_spec);
      
      QString attr_full_name = attr_name_spec;
      
      if ((index = attr_full_name.find("${name}")) != -1)
	attr_full_name.replace(index, 7, attr_name);
	
      if ((index = d.find("${body}")) != -1) {
	QString indent;
	QString end = (d[index + 7] == '}') ? ";\n" : ";";
	
	if (d[index - 1] == '\n')
	  indent = "  ";
	
	d.replace(index, 7, indent + "$this->" + attr_full_name + " = ${p0}" + end);
		
	def = d;
      }    
      else
	def = 0;
    }
    else
      def = 0;
  }
}

void OperationData::update_idl_set_of(QCString & decl, QString attidl_decl,
				      QString multiplicity)
{
  attidl_decl = attidl_decl.stripWhiteSpace();
  
  int index;
 QString arg_spec;
  
  if ((index = attidl_decl.find("${comment}")) != -1)
    attidl_decl.remove(index, 10);
  if ((index = attidl_decl.find("${description}")) != -1)
    attidl_decl.remove(index, 14);
  if ((index = attidl_decl.find("${visibility}")) != -1)
    attidl_decl.remove(index, 13);
  if ((index = attidl_decl.find("${readonly}")) != -1)
    attidl_decl.remove(index, 11);
  if ((index = attidl_decl.find("${attribut}")) != -1)
    // old version
    attidl_decl.remove(index, 11);
  if ((index = attidl_decl.find("${attribute}")) != -1)
    attidl_decl.remove(index, 12);
  if ((index = attidl_decl.find("const ")) != -1)
    attidl_decl.remove(index, 6);
  if ((index = attidl_decl.find("${value}")) != -1)
    attidl_decl.remove(index, 8);
  if ((index = attidl_decl.find(";")) != -1)
    attidl_decl.remove(index, 1);
  
  QString attr_name_spec = extract_name(attidl_decl);
  
  if (attr_name_spec.isEmpty())
    decl = 0;
  else {  
    QString arg_spec = attidl_decl;
    
    arg_spec.replace(arg_spec.find(attr_name_spec),
		     attr_name_spec.length(),"${p0}");
    
    if ((index = arg_spec.find("${type}")) != -1)
      arg_spec.replace(index, 7, "${t0}");
    
    QCString d = (const char *) GenerationSettings::idl_default_oper_decl();
    
    if ((index = d.find("${)}")) == -1)
      decl = 0;
    else {
      d.insert(index, arg_spec);
      if ((index = d.find("${multiplicity}")) != -1)
	d.replace(index, 15, multiplicity);
      decl = d;
    }
  }
}

void OperationData::update_set_of(const QString & attr_name,
				  QString attcpp_decl, QString attjava_decl,
				  QString attphp_decl, QString attidl_decl,
				  bool attis_const, bool attis_class_member,
				  const AType & cl, QString multiplicity,
				  QString relstereotype, bool create, bool update) {
//#warning warning si attis_const;
  
  stereotype = relstereotype.stripWhiteSpace();
  return_type.type = 0;
  return_type.explicit_type = "void";
  isa_class_operation = attis_class_member;
  is_get_or_set = TRUE;
  if (nparams == 0) {
    set_n_params(1);
    set_param_dir(0, UmlIn);
  }
  set_param_name(0, (attr_name == "value") ? "new_value" : "value");
  set_param_type(0, cl);
  
  QString st = ((BrowserClass *) browser_node->parent())->get_stereotype();
  QCString decl;
  QCString def;
  
  // C++
  if (!update || !cpp_get_set_frozen) {
    if (ClassDialog::cpp_stereotype(st) != "enum") {
      if (create) {
	if (cpp_name_spec.isEmpty())
	  cpp_name_spec = GenerationSettings::cpp_default_set_name();
	cpp_inline = GenerationSettings::cpp_default_set_inline();
	if (GenerationSettings::cpp_default_set_visibility() !=
	    GenerationSettings::javaphp_default_set_visibility())
	  cpp_visibility = GenerationSettings::cpp_default_set_visibility(); 
      }
    
      update_cpp_set_of(decl, def, attr_name, attcpp_decl,
			attis_const, multiplicity);
      cpp_decl = decl;
      if (def.isEmpty())
	cpp_def.assign(QString::null, TRUE);
      else
	cpp_def.assign(def, FALSE);
    }
    else {
      cpp_decl = "";
      cpp_def.assign(0, TRUE);
    }
  }
  
  // Java
  if (create) {
    if (java_name_spec.isEmpty())
      java_name_spec = GenerationSettings::java_default_set_name();
    java_final = GenerationSettings::java_default_set_final();
    uml_visibility = GenerationSettings::javaphp_default_set_visibility();
  }
  
  if (!update || !java_get_set_frozen) {
    update_java_set_of(def, attr_name, attjava_decl);
    if (def.isEmpty())
      java_def.assign(QString::null, TRUE);
    else
      java_def.assign(def, FALSE);
  }
  
  // Php
  if (create) {
    if (php_name_spec.isEmpty())
      php_name_spec = GenerationSettings::php_default_set_name();
    php_final = GenerationSettings::php_default_set_final();
  }
  
  if (!update || !php_get_set_frozen) {
    update_php_set_of(def, attr_name, attphp_decl);
    if (def.isEmpty())
      php_def.assign(QString::null, TRUE);
    else
      php_def.assign(def, FALSE);
  }
  
  // Idl
  if (!update || !idl_get_set_frozen) {
    if (ClassDialog::idl_stereotype(st) != "enum") {
      if (create) {
	if (idl_name_spec.isEmpty())
	  idl_name_spec = GenerationSettings::idl_default_set_name();
	idl_oneway = GenerationSettings::idl_default_set_oneway();
      }
      update_idl_set_of(decl, attidl_decl, multiplicity);
      idl_decl = decl;
    }
    else {
      idl_decl = "";
    }
  }
}

//

bool OperationData::reference(BrowserClass * target) const {
  if (return_type.type == target)
    return TRUE;
  
  unsigned i;
  
  for (i = 0; i != nparams; i += 1)
    if (params[i].get_type().type == target)
      return TRUE;
  
  for (i = 0; i != nexceptions; i += 1)
    if (exceptions[i].get_type().type == target)
      return TRUE;
  
  return FALSE;
}

//

void OperationData::replace(BrowserClass * old, BrowserClass * nw) {
  AType t;
  
  t.type = nw;
  
  if (return_type.type == old)
    set_return_type(t);
  
  unsigned i;
  for (i = 0; i != nparams; i += 1)
    if (params[i].get_type().type == old)
      params[i].set_type(t);
  
  for (i = 0; i != nexceptions; i += 1)
    if (exceptions[i].get_type().type == old)
      // almost no chance to be true ...
      exceptions[i].set_type(t);
}

//

void OperationData::send_uml_def(ToolCom * com, BrowserNode * bn, 
				 const QString & comment) {
  int api = com->api_format();
  
  BasicData::send_uml_def(com, bn, comment);
  com->write_bool(isa_class_operation);
  if (api >= 13)
    com->write_bool(is_volatile);
  com->write_char(((api >= 23) ||
		   (uml_visibility != UmlPackageVisibility))
		  ? uml_visibility : UmlPublic);
  if (api >= 30)
    com->write_string(constraint);
  return_type.send_def(com);
  com->write_bool(is_abstract);
  
  unsigned n;
  ParamData * p;
  
  com->write_unsigned(nparams);
  for (p = params, n = nparams; n; p += 1, n -=1)
    p->send_uml_def(com);
  
  ExceptionData * ep;
  
  com->write_unsigned(nexceptions);
  for (ep = exceptions, n = nexceptions; n; ep += 1, n -=1)
    ep->send_uml_def(com);    
  
  if (((BrowserOperation *) browser_node)->get_get_of() != 0)
    ((BrowserOperation *) browser_node)->get_get_of()->write_id(com);
  else
    com->write_id(0);
  
  if (((BrowserOperation *) browser_node)->get_set_of() != 0)
    ((BrowserOperation *) browser_node)->get_set_of()->write_id(com);
  else
    com->write_id(0);
}

void OperationData::send_cpp_def(ToolCom * com) {
  com->write_string(cpp_decl);
  if (com->api_format() >= 23)
    com->write_char(cpp_visibility);
  else {
    switch(cpp_visibility) {
    case UmlPackageVisibility:
      com->write_char(UmlPublic);
      break;
    case UmlDefaultVisibility:
      com->write_char(UmlDefaultVisibility - 1);
      break;
    default:
      com->write_char(cpp_visibility);
    }
  }
  if (com->api_format() < 13)
    com->write_bool(is_volatile);
  com->write_bool(cpp_const);
  com->write_bool(cpp_friend);
  com->write_bool(cpp_virtual);
  com->write_bool(cpp_inline);
  com->write_string(cpp_def);
  com->write_string(cpp_name_spec);
  if (com->api_format() >= 26)
    com->write_bool(cpp_get_set_frozen);
}
    
void OperationData::send_java_def(ToolCom * com) {
  com->write_string(java_def);
  if (com->api_format() >= 21)
    com->write_string(java_annotation);
  com->write_bool(java_final);
  com->write_bool(java_synchronized);
  com->write_string(java_name_spec);
  if ((com->api_format() >= 19) && (com->api_format() < 21))
    com->write_string(java_annotation);
  if (com->api_format() >= 26)
    com->write_bool(java_get_set_frozen);
}
    
void OperationData::send_php_def(ToolCom * com) {
  com->write_string(php_def);
  com->write_bool(php_final);
  com->write_string(php_name_spec);
  com->write_bool(php_get_set_frozen);
}

void OperationData::send_idl_def(ToolCom * com) {
  com->write_string(idl_decl);
  com->write_bool(idl_oneway);
  com->write_string(idl_name_spec);
  if (com->api_format() >= 26)
    com->write_bool(idl_get_set_frozen);
}
    
//

// translate plug out release < 2.0
void OperationData::convert(OperationData * comp, OperationData * art)
{
  // UmlComponent constructor definition
  comp->cpp_decl =
    "    ${comment}${name}${(}${t0} ${p0}, const ${t1} & ${p1}${)} : UmlBaseComponent(id, n) {};\n";
  comp->cpp_def.assign(QString::null, TRUE);
  comp->java_def.assign("  ${comment}${@}${visibility}${final}${name}${(}${t0} ${p0}, ${t1} ${p1}${)}${throws}{ super(id, n); }\n",
			FALSE);
  
  // replace call to UmlBaseComponent by a call to UmlBaseArtifact
  // in the UmlArtifact contructor

  int index;
  QCString s = art->cpp_decl;
  
  if ((index = s.find("UmlBaseComponent")) != -1) {
    s.replace(index+7, 9, "Artifact");
    art->cpp_decl = s;
  }
  else if (strstr((const char *) art->cpp_def, "${body}") == 0) {
    s = (const char *) art->cpp_def;
  
    if ((index = s.find("UmlBaseComponent")) != -1) {
      s.replace(index+7, 9, "UmlArtifact");
      art->cpp_def.assign(s, FALSE);
    }
  }
  else {
    char * b = art->get_body('c');
    
    if (b != 0) {
      s = b;
      if ((index = s.find("UmlBaseComponent")) != -1) {
	s.replace(index+7, 9, "Artifact");
	art->new_body(s, 'c');
      }
      
      delete [] b;
    }
  }
}

//

bool OperationData::tool_cmd(ToolCom * com, const char * args,
			     BrowserNode * bn,
			     const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setIsClassMemberCmd:
	isa_class_operation = (*args != 0);
	break;
      case setVisibilityCmd:
	{
	  UmlVisibility v;
	  
	  if (! com->get_visibility(v, args)) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
	  else
	    uml_visibility = v;
	}
	break;
      case setIsAbstractCmd:
	is_abstract = (*args != 0);
	break;
      case setReturnTypeCmd:
	{
	  AType t;
	  
	  com->get_type(t, args);
	  set_return_type(t);
	}
	break;
      case setConstraintCmd:
	constraint = args;
	break;
      case setCppVisibilityCmd:
	{
	  UmlVisibility v;
	  
	  if (! com->get_extended_visibility(v, args)) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
	  else
	    cpp_visibility = v;
	}
	break;
      case setCppDeclCmd:
	cpp_decl = args;
	break;
      case setIsCppConstCmd:
	cpp_const = (*args != 0);
	break;
      case setIsCppFriendCmd:
	cpp_friend = (*args != 0);
	break;
      case setIsCppVirtualCmd:
	cpp_virtual = (*args != 0);
	break;
      case setIsCppInlineCmd:
	cpp_inline = (*args != 0);
	break;
      case setCppDefCmd:
	cpp_def.assign(args, is_abstract || (strstr(args, "${body}") != 0));
	break;
      case setCppBodyCmd:
	{
	  char * b = get_body('c');
	  
	  if (b != 0) {
	    if (strcmp(b, args))
	      new_body(args, 'c');
	    delete [] b;
	  }
	  else if (*args)
	    new_body(args, 'c');
	}
	break;
      case setCppNameSpecCmd:
	cpp_name_spec = args;
	break;
      case setCppFrozenCmd:
	cpp_get_set_frozen = (*args != 0);
	break;
      case setJavaDeclCmd:
	{
	  QString ste = GenerationSettings::java_class_stereotype(stereotype);
	  
	  java_def.assign(args,
			  is_abstract ||
			  (ste == "interface") ||
			  (ste == "@interface") ||
			  (strstr(args, "${body}") != 0));
	}
	break;
      case setJavaAnnotationCmd:
	{
	  QCString s = args;
	  
	  s = s.stripWhiteSpace();
	  if (! s.isEmpty())
	    s += '\n';
	  java_annotation = s;
	}
	break;
      case setJavaFinalCmd:
	java_final = (*args != 0);
	break;
      case setJavaSynchronizedCmd:
	java_synchronized = (*args != 0);
	break;
      case setJavaBodyCmd:
	{
	  char * b = get_body('j');
	  
	  if (b != 0) {
	    if (strcmp(b, args))
	      new_body(args, 'j');
	    delete [] b;
	  }
	  else if (*args)
	    new_body(args, 'j');
	}
	break;
      case setJavaNameSpecCmd:
	java_name_spec = args;
	break;
      case setJavaFrozenCmd:
	java_get_set_frozen = (*args != 0);
	break;
      case setPhpDeclCmd:
	{
	  QString ste = GenerationSettings::php_class_stereotype(stereotype);
	  
	  php_def.assign(args,
			 is_abstract ||
			 (ste == "interface") ||
			 (strstr(args, "${body}") != 0));
	}
	break;
      case setPhpFinalCmd:
	php_final = (*args != 0);
	break;
      case setPhpBodyCmd:
	{
	  char * b = get_body('p');
	  
	  if (b != 0) {
	    if (strcmp(b, args))
	      new_body(args, 'p');
	    delete [] b;
	  }
	  else if (*args)
	    new_body(args, 'p');
	}
	break;
      case setPhpNameSpecCmd:
	php_name_spec = args;
	break;
      case setPhpFrozenCmd:
	php_get_set_frozen = (*args != 0);
	break;
      case setIsIdlOnewayCmd:
	idl_oneway = (*args != 0);
	break;
      case setIdlDeclCmd:
	idl_decl = args;
	break;
      case setIdlNameSpecCmd:
	idl_name_spec = args;
	break;
      case setIdlFrozenCmd:
	idl_get_set_frozen = (*args != 0);
	break;
      case addParameterCmd:
	{
	  unsigned rank = com->get_unsigned(args);
	  UmlParamDirection dir;
	  bool valid_dir = com->get_direction(dir, args);
	  
	  if ((rank > nparams) || !valid_dir || (dir == UmlReturn)) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
    
	  QString name = com->get_string(args);
	  QString dflt = com->get_string(args);
	  AType t;
	  ParamData * new_params = new ParamData[nparams + 1];
	  unsigned index;
	  
	  com->get_type(t, args);

	  for (index = 0; index != rank; index += 1)
	    new_params[index] = params[index];
	  
	  new_params[index].set_name(name);
	  new_params[index].set_dir(dir);
	  new_params[index].set_default_value((dflt.isEmpty()) ? QString::null : dflt);
	  new_params[index].set_type(t);
	  depend_on(t.type);
	  
	  while (index != nparams) {
	    new_params[index + 1] = params[index];
	    index += 1;
	  }
	  
	  delete [] params;
	  params = new_params;
	  nparams += 1;
	}
	break;
      case replaceParameterCmd:
	{
	  unsigned rank = com->get_unsigned(args);
	  UmlParamDirection dir;
	  bool valid_dir = com->get_direction(dir, args);
	  
	  if ((rank >= nparams) || !valid_dir || (dir == UmlReturn)) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
	  
	  QString name = com->get_string(args);
	  QString dflt = com->get_string(args);
	  AType t;
	  
	  com->get_type(t, args);
	  
	  params[rank].set_name(name);
	  params[rank].set_dir(dir);
	  params[rank].set_default_value((dflt.isEmpty()) ? QString::null : dflt);
	  set_param_type(rank, t);
	}
	break;
      case removeParameterCmd:
	{
	  unsigned rank = com->get_unsigned(args);
	  
	  if (rank >= nparams) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
    
	  while (++rank != nparams)
	    params[rank - 1] = params[rank];
	  nparams -= 1;
	}
	break;
      case addExceptionCmd:
	{
	  unsigned rank = com->get_unsigned(args);
	  AType t;
	  
	  com->get_type(t, args);
	  
	  if (rank > nexceptions) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
	  
	  ExceptionData * new_exceptions = new ExceptionData[nexceptions + 1];
	  unsigned index;
	  
	  for (index = 0; index != rank; index += 1)
	    new_exceptions[index] = exceptions[index];
	  
	  new_exceptions[index].set_type(t);
	  depend_on(t.type);
	  
	  while (index != nexceptions) {
	    new_exceptions[index + 1] = exceptions[index];
	    index += 1;
	  }
	  
	  delete [] exceptions;
	  exceptions = new_exceptions;
	  nexceptions += 1;
	}
	break;
      case replaceExceptionCmd:
	{
	  unsigned rank = com->get_unsigned(args);
	  AType t;
	  
	  com->get_type(t, args);
	  
	  if (rank >= nexceptions) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
	  
	  set_exception(rank, t);
	}
	break;
      case removeExceptionCmd:
	{
	  unsigned rank = com->get_unsigned(args);
	  
	  if (rank >= nexceptions) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
    
	  while (++rank != nexceptions)
	    exceptions[rank - 1] = exceptions[rank];
	  nexceptions -= 1;
	}
	break;
      default:
	return BasicData::tool_cmd(com, args, bn, comment);
      }
  
      // ok case
      bn->modified();
      bn->package_modified();
      modified();
      com->write_ack(TRUE);
    }
  }
  else {
    switch ((unsigned char) args[-1]) {
    case cppBodyCmd:
      {
	char * body = get_body('c');
	
	if (body != 0) {
	  com->write_string(body);
	  delete [] body;
	}
	else
	  com->write_string("");
      }
      break;
    case javaBodyCmd:
      {
	char * body = get_body('j');
	
	if (body != 0) {
	  com->write_string(body);
	  delete [] body;
	}
	else
	  com->write_string("");
	return TRUE;
      }
      break;
    case phpBodyCmd:
      {
	char * body = get_body('p');
	
	if (body != 0) {
	  com->write_string(body);
	  delete [] body;
	}
	else
	  com->write_string("");
	return TRUE;
      }
      break;
    default:
      return BasicData::tool_cmd(com, args, bn, comment);
    }
  }
  
  return TRUE;
}

//

void OperationData::set_bodies_info() {
  BrowserClass * cl = (BrowserClass *) browser_node->parent();
  char * s = set_bodies_info(cl, cl->get_ident());
  
  if (s != 0)
    delete [] s;
}

// id may be the old cl's ident in case of an import
char * OperationData::set_bodies_info(BrowserClass * cl, int id)
{
  cl->set_bodies_read(TRUE);
  
  // no bodies by default
  for (QListViewItem * child = cl->firstChild(); child; child = child->nextSibling()) {
    if (((BrowserNode *) child)->get_type() == UmlOperation) {
      OperationData * d = (OperationData *) ((BrowserNode *) child)->get_data();
      
      d->cpp_body.length = 
	d->java_body.length =
	  d->php_body.length = 0;
    }
  }

  char * s = read_file(QString::number(id) + ".bodies");
  
  if (s == 0)
    return 0;
  
  char * p = strstr(s, "\n!!!");
  
  if (p == 0) {
    // no body
    delete [] s;
    return 0;
  }
  
  // some operations have a body
  OperationBody * b = 0;
  char * start = 0;
  
  for (;;) {
    p += 4;
    
    char * p2;
    
    if (sscanf(p, "%d", &id) == 1) {
      p2 = p + QString::number(id).length();
      
      bool find = FALSE;
      OperationData * d;
      
      if (strncmp(p2, ".cpp!!!\t", 8) == 0) {
	if (b != 0)
	  b->length = p - start - 3;
	
	if (((d = all[id]) != 0) && (d->browser_node->parent() == cl)) {
	  b = &d->cpp_body;
	  find = TRUE;
	}
	else
	  // !! wrong file
	  b = 0;
      }
      if (strncmp(p2, ".java!!!\t", 9) == 0) {
	if (b != 0)
	  b->length = p - start - 3;
	
	if (((d = all[id]) != 0) && (d->browser_node->parent() == cl)) {
	  b = &d->java_body;
	  find = TRUE;
	}
	else
	  // !! wrong file
	  b = 0;
      }
      if (strncmp(p2, ".php!!!\t", 8) == 0) {
	if (b != 0)
	  b->length = p - start - 3;
	
	if (((d = all[id]) != 0) && (d->browser_node->parent() == cl)) {
	  b = &d->php_body;
	  find = TRUE;
	}
	else
	  // !! wrong file
	  b = 0;
      }
      
      if (find) {
	if ((p = strchr(p2, '\n')) != 0) {
	  start = p + 1;
	  b->offset = start - s;
	}
	else
	  // wrong file
	  break;
      }
    }
    
    p2 = strstr(p, "\n!!!");
    
    if (p2 == 0) {
      if (b != 0)
	b->length = p + strlen(p) - start;
      break;
    }
    p = p2;
  }
  
  return s;
}

void OperationData::create_modified_body_file() {
  if (browser_node == 0)
      return;
      
  BrowserClass * cl = (BrowserClass *) browser_node->parent();
  
  if (! cl->get_bodies_modified()) {
    QFile fp(abs_file_path(cl->get_ident(), "b"));
    
    if (open_file(fp, IO_WriteOnly) == -1)
      return;
  
    cl->set_bodies_modified(TRUE);
    
    if (! cl->get_bodies_read())
      set_bodies_info();
    
    char * old = read_file(QString::number(cl->get_ident()) + ".bodies");
      
    if (old != 0) {
      fp.writeBlock(old, strlen(old));
      delete [] old;
    }
    else {
      QString header =
	QString("class ") + cl->get_data()->definition(TRUE) + '\n';
      
      fp.writeBlock(header, header.length());
    }
  }
}

char * OperationData::get_body(int who) {
  OperationBody * body_info;
  
  switch (who) {
  case 'c':
    body_info = &cpp_body;
    break;
  case 'j':
    body_info = &java_body;
    break;
  default: // 'p'
    body_info = &php_body;
  }
  
  switch (body_info->length) {
  case -1:
    set_bodies_info();
    break;
  case 0:
    return 0;
  }
  
  return read_definition(((BrowserClass *) browser_node->parent())->get_ident(),
			 "bodies", body_info->offset, body_info->length);
}

void OperationData::new_body(QString s, int who) {
  OperationBody * body_info;
  QString key;
  
  switch (who) {
  case 'c':
    body_info = &cpp_body;
    key = ".cpp!!!\t";
    break;
  case 'j':
    body_info = &java_body;
    key = ".java!!!\t";
    break;
  default: // 'p'
    body_info = &php_body;
    key = ".php!!!\t";
  }
  
  if (! ((BrowserClass *) browser_node->parent())->get_bodies_read())
    set_bodies_info();
  
  BrowserClass * cl = (BrowserClass *) browser_node->parent();
  QFile fp(abs_file_path(cl->get_ident(), "b"));

  if (open_file(fp, IO_WriteOnly | IO_Append) == -1)
    return;

  cl->set_bodies_modified(TRUE);
  
  if (fp.at() == 0) {
    char * old = read_file(QString::number(cl->get_ident()) + ".bodies");
    
    if (old != 0) {
      fp.writeBlock(old, strlen(old));
      delete [] old;
    }
    else {
      QString header =
	QString("class ") + cl->get_data()->definition(TRUE) + '\n';
      
      fp.writeBlock(header, header.length());
    }
  }
  
  if (!s.isEmpty()) {
    if (s.at(s.length() - 1) != QChar('\n'))
      s += '\n';
    
    QString op_header = QString("!!!") + QString::number(get_ident()) +
      key + definition(TRUE) + "\n";
    
    fp.writeBlock(op_header, op_header.length());
    
    body_info->offset = fp.at();
    body_info->length = s.length();
    
    fp.writeBlock(s, s.length());
  }
  else
    body_info->length = 0;
}
	
void OperationData::save_body(QFile & qf, char * modified_bodies, int who) {
  OperationBody * body_info;
  QString key;
  
  switch (who) {
  case 'c':
    body_info = &cpp_body;
    key = ".cpp!!!\t";
    break;
  case 'j':
    body_info = &java_body;
    key = ".java!!!\t";
    break;
  default: // 'p'
    body_info = &php_body;
    key = ".php!!!\t";
  }
  
  if (body_info->length > 0) {
    if (!qf.isOpen()) {
      BrowserClass * cl = (BrowserClass *) browser_node->parent();
      QDir d = BrowserView::get_dir();
      QString filename = abs_file_path(cl->get_ident(),
				       (in_import()) ? "b" : "bodies");
      
      backup(d, filename);
      qf.setName(filename);
      if (open_file(qf, IO_WriteOnly) == -1)
	throw 0;
      
      QString header =
	QString("class ") + cl->get_data()->definition(TRUE) + '\n';
      
      qf.writeBlock(header, header.length());
    }
    
    QString op_header = QString("!!!") + QString::number(get_ident()) +
      key + definition(TRUE) + "\n";
    
    qf.writeBlock(op_header, op_header.length());
    
    int new_offset = qf.at();
    
    qf.writeBlock(modified_bodies + body_info->offset,
		  body_info->length);
    body_info->offset = new_offset;
  }
}

// save all the operations's body of cl, id is its old ident
void OperationData::import(BrowserClass * cl, int id)
{
  char * s = set_bodies_info(cl, id);
  
  if (s == 0)
    return;
   
  QFile qf;
  
  for (QListViewItem * child = cl->firstChild(); child; child = child->nextSibling()) {
    if (((BrowserNode *) child)->get_type() == UmlOperation) {
      OperationData * d = (OperationData *) ((BrowserNode *) child)->get_data();

      d->save_body(qf, s, 'c');
      d->save_body(qf, s, 'j');
      d->save_body(qf, s, 'p');
    }
  }

  delete [] s;  
}

void OperationData::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "operation_ref " << get_ident() << " // ";
    save_string(definition(TRUE), st);
  }
  else {
    BasicData::save(st, warning);
    
    nl_indent(st);
    if (isa_class_operation)
      st << "class_operation ";
    else if (is_abstract)
      st << "abstract ";
    if (is_volatile)
      st << "volatile ";
    
    if (cpp_get_set_frozen)
      st << "cpp_frozen ";   
    if (cpp_const)
      st << "const ";
    if (cpp_friend)
      st << "friend ";
    if (cpp_virtual)
      st << "cpp_virtual ";
    if (cpp_inline)
      st << "cpp_inline ";
    
    st << stringify(uml_visibility);
    
    return_type.save(st, warning, " return_type ", " explicit_return_type ");
    nl_indent(st);
    
    st << "nparams " << nparams;
    for (unsigned i = 0; i != nparams; i += 1)
      params[i].save(st, warning);
    
    if (nexceptions != 0) {
      nl_indent(st);
      st << "nexceptions " << nexceptions;
      for (unsigned i = 0; i != nexceptions; i += 1)
	exceptions[i].save(st, warning);
    }
    
    if (!constraint.isEmpty()) {
      nl_indent(st);
      st << "constraint ";
      save_string(constraint, st);
    }
    
    nl_indent(st);
    if (cpp_visibility != UmlDefaultVisibility)
      st << "cpp_visibility " << stringify(cpp_visibility) << " ";
    if (! cpp_decl.isEmpty()) {
      st << "cpp_decl ";
      save_string(cpp_decl, st);
    }
    
    if (!is_abstract && !cpp_def.isEmpty()) {
      nl_indent(st);
      st << "cpp_def ";
      save_string(cpp_def, st);
    }

    if (!cpp_name_spec.isEmpty()) {
      nl_indent(st);
      st << "cpp_name_spec ";
      save_string(cpp_name_spec, st);
    }

    nl_indent(st);
    if (java_get_set_frozen)
      st << "java_frozen ";   
    if (java_final)
      st << "final ";
    if (java_synchronized)
      st << "synchronized ";
    if (! java_def.isEmpty()) {
      st << "java_def ";
      save_string(java_def, st);
    }
    if (! java_annotation.isEmpty()) {
      nl_indent(st);
      st << "java_annotation ";
      save_string(java_annotation, st);
    }

    if (!java_name_spec.isEmpty()) {
      nl_indent(st);
      st << "java_name_spec ";
      save_string(java_name_spec, st);
    }
    
    nl_indent(st);
    if (php_get_set_frozen)
      st << "php_frozen ";   
    if (php_final)
      st << "php_final ";
    if (! php_def.isEmpty()) {
      st << "php_def ";
      save_string(php_def, st);
    }

    if (!php_name_spec.isEmpty()) {
      nl_indent(st);
      st << "php_name_spec ";
      save_string(php_name_spec, st);
    }
    
    nl_indent(st);
    if (idl_get_set_frozen)
      st << "idl_frozen ";   
    if (idl_oneway)
      st << "oneway ";
    if (! idl_decl.isEmpty()) {
      st << "idl_decl ";
      save_string(idl_decl, st);
    }
    if (!idl_name_spec.isEmpty()) {
      nl_indent(st);
      st << "idl_name_spec ";
      save_string(idl_name_spec, st);
    }
  }
}

OperationData * OperationData::read_ref(char * & st)
{
  read_keyword(st, "operation_ref");
  
  return all[read_id(st)];
}

void OperationData::read(char * & st, char * & k) {
  cpp_body.length = -1;
  java_body.length = -1;
  php_body.length = -1;
  
  k = read_keyword(st);
  BasicData::read(st, k);	// updates k
  
  isa_class_operation = is_abstract = FALSE;
  if (!strcmp(k, "class_operation")) {
    isa_class_operation = TRUE;
    k = read_keyword(st);
  }
  else if (!strcmp(k, "abstract")) {
    is_abstract = TRUE;
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "volatile")) {
    is_volatile = TRUE;
    k = read_keyword(st);
  }
  else
    is_volatile = FALSE;
  
  if (!strcmp(k, "cpp_frozen")) {
    cpp_get_set_frozen = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_get_set_frozen = FALSE;
  
  if (!strcmp(k, "const")) {
    cpp_const = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_const = FALSE;
  
  if (!strcmp(k, "friend")) {
    cpp_friend = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_friend = FALSE;
  
  if (!strcmp(k, "virtual") || !strcmp(k, "cpp_virtual")) {
    cpp_virtual = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_virtual = FALSE;
  
  if (!strcmp(k, "inline") || !strcmp(k, "cpp_inline")) {
    cpp_inline = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_inline = FALSE;
  
  if (!strcmp(k, "cpp_volatile")) {
    // old version
    is_volatile = TRUE;
    k = read_keyword(st);
  }
	
  uml_visibility = ::visibility(k);
    
  k = read_keyword(st);
  
  AType t;
  
  t.read(st, "return_type", "explicit_return_type", k);
  set_return_type(t);

  read_keyword(st, "nparams");
  
  unsigned n = read_unsigned(st);
  
  set_n_params(n);
  k = read_keyword(st);
  for (unsigned i = 0; i != n; i += 1) {
    params[i].read(st, k);	// updates k
    depend_on(params[i].get_type().type);
  }
    
  if (!strcmp(k, "nexceptions")) {
    n = read_unsigned(st);
    
    set_n_exceptions(n);
    k = read_keyword(st);
    for (unsigned i = 0; i != n; i += 1) {
      exceptions[i].read(st, k);	// updates k
      depend_on(exceptions[i].get_type().type);
    }
  }
  else
    set_n_exceptions(0);
  
  if (!strcmp(k, "constraint")) {
    constraint = read_string(st);
    k = read_keyword(st);
  }
  else
    constraint = QString::null;
  
  if (!strcmp(k, "cpp_visibility")) {
    cpp_visibility = ::visibility(read_keyword(st));
    k = read_keyword(st);
  }
  else
    cpp_visibility = UmlDefaultVisibility;

  if (!strcmp(k, "cpp_decl")) {
    cpp_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    cpp_decl = QString::null;
  
  if (!strcmp(k, "cpp_def")) {
    char * d = read_string(st);
    
    cpp_def.assign(d, (is_abstract || (strstr(d, "${body}") != 0)));
    k = read_keyword(st);
  }
  else
    cpp_def.assign(QString::null, TRUE);

  if (!strcmp(k, "cpp_name_spec")) {
    is_get_or_set = TRUE;
    cpp_name_spec = read_string(st);
    k = read_keyword(st);
  } 
  else {
    is_get_or_set = FALSE;
    cpp_name_spec = QString::null;
  }
  
  if (!strcmp(k, "java_frozen")) {
    java_get_set_frozen = TRUE;
    k = read_keyword(st);
  }
  else
    java_get_set_frozen = FALSE;
  
  if (!strcmp(k, "final")) {
    java_final = TRUE;
    k = read_keyword(st);
  }
  else
    java_final = FALSE;
  
  if (!strcmp(k, "synchronized")) {
    java_synchronized = TRUE;
    k = read_keyword(st);
  }
  else
    java_synchronized = FALSE;
  
  if (!strcmp(k, "java_def")) {
    char * d = read_string(st);
    QString ste = GenerationSettings::java_class_stereotype(stereotype);
    
    java_def.assign(d,
		    is_abstract ||
		    (strstr(d, "${body}") != 0) || 
		    (ste == "interface") || 
		    (ste == "@interface"));
    k = read_keyword(st);
  }
  else
    java_def.assign(QString::null, TRUE);
  
  if (!strcmp(k, "java_annotation")) {
    java_annotation = read_string(st);
    k = read_keyword(st);
  }
  else
    java_annotation = QString::null;
  
  if (!strcmp(k, "java_name_spec")) {
    is_get_or_set = TRUE;
    java_name_spec = read_string(st);
    k = read_keyword(st);
  } 
  else {
    is_get_or_set = FALSE;
    java_name_spec = QString::null;
  }
  
  if (!strcmp(k, "php_frozen")) {
    php_get_set_frozen = TRUE;
    k = read_keyword(st);
  }
  else
    php_get_set_frozen = FALSE;
  
  if (!strcmp(k, "php_final")) {
    php_final = TRUE;
    k = read_keyword(st);
  }
  else
    php_final = FALSE;
  
  if (!strcmp(k, "php_def")) {
    char * d = read_string(st);
    QString ste = GenerationSettings::php_class_stereotype(stereotype);
    
    php_def.assign(d,
		   is_abstract ||
		   (strstr(d, "${body}") != 0) || 
		   (ste == "interface"));
    k = read_keyword(st);
  }
  else
    php_def.assign(QString::null, TRUE);
  
  if (!strcmp(k, "php_name_spec")) {
    is_get_or_set = TRUE;
    php_name_spec = read_string(st);
    k = read_keyword(st);
  } 
  else {
    is_get_or_set = FALSE;
    php_name_spec = QString::null;
  }
  
  if (!strcmp(k, "idl_frozen")) {
    idl_get_set_frozen = TRUE;
    k = read_keyword(st);
  }
  else
    idl_get_set_frozen = FALSE;
  
  if (!strcmp(k, "oneway")) {
    idl_oneway = TRUE;
    k = read_keyword(st);
  }
  else
    idl_oneway = FALSE;
  
  if (!strcmp(k, "idl_decl")) {
    idl_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    idl_decl = QString::null;
  
  if (!strcmp(k, "idl_name_spec")) {
    is_get_or_set = TRUE;
    idl_name_spec = read_string(st);
    k = read_keyword(st);
  } 
  else {
    is_get_or_set = FALSE;
    idl_name_spec = QString::null;
  }
}
