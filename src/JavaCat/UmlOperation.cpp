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

#ifdef TRACE
#include <iostream>

using namespace std;
#endif
#include <qstringlist.h> 

#include "UmlOperation.h"
#include "UmlPackage.h"
#include "Class.h"
#include "UmlClass.h"
#include "UmlAttribute.h"
#include "JavaCatWindow.h"
#include "JavaSettings.h"
#include "Lex.h"
#ifdef REVERSE
#include "Statistic.h"
#endif

bool UmlOperation::new_one(Class * container, const QCString & name,
			   const QValueList<FormalParameterList> & tmplts,
			   const QCString & oper_templ,
			   UmlTypeSpec & type, QCString str_actuals,
			   UmlClass * first_actual_class, QCString type_def,
			   aVisibility visibility,
			   bool finalp, bool abstractp, bool staticp,
			   bool nativep, bool strictfp, bool synchronizedp, 
			   const QCString & array, QCString comment,
			   QCString description, QCString annotation)
{
  // the "(" was read
  
#ifdef TRACE
  cout << "OPERATION '" << name << "'\n";
#endif
  
  UmlClass * cl = container->get_uml();
  UmlOperation * op;
  
  if (
#ifdef REVERSE
      container->from_libp() &&
#endif
      (visibility == PrivateVisibility))
    op = 0;
  else {
    op = UmlBaseOperation::create(cl, name);
    
    if (op == 0) {
      JavaCatWindow::trace(QCString("<font face=helvetica><b>cannot add operation <i>")
			   + name + "</i> in <i>" + cl->name() 
			   + "</i></b></font><br>");  
      return FALSE;
    }
    
#ifdef REVERSE
    Statistic::one_operation_more();
#endif
  }
    
  QCString def;
    
  if (op != 0) {
    op->set_Visibility(visibility);
    if (staticp) op->set_isClassMember(TRUE);
    if (abstractp) op->set_isAbstract(TRUE);
    if (finalp) op->set_isJavaFinal(TRUE);
    if (synchronizedp) op->set_isJavaSynchronized(TRUE);
    if (! annotation.isEmpty()) op->set_JavaAnnotations(annotation);
  
    def = JavaSettings::operationDef();
  
    int index;
    
    if (((index = def.find("${(}")) == -1) ||
	(def.find("${)}", index + 4) == -1) ||
	((index = def.find("${type}")) == -1) ||
	(def.find("${throws}") == -1) ||
	(def.find("${body}") == -1)) {
      // use a definition where ${body] is not indented
      def = "  ${comment}${@}${visibility}${final}${static}${abstract}${synchronized}${type} ${name}${(}${)}${throws}${staticnl}{\n${body}}\n";
      index = def.find("${type}");
    }
    else {
      // unindent ${body}
      int index2 = def.find("${body}", index);
      int index3 = index2;
      
      while ((def[--index3] == ' ') || (def[index3] == '\t'))
	;
      
      def.remove((unsigned) (index3 + 1),
		 (unsigned) (index2 - index3 - 1));
    }
    
    if (!array.isEmpty())
      def.insert(index + 7, array);
    
    if (nativep) {
      def.insert(index, "native ");
      index += 7;
    }
    
    if (strictfp) {
      def.insert(index, "strictfp ");
      index += 9;
    }
    
    if (! oper_templ.isEmpty())
      def.insert(index, oper_templ + " ");
    
    if (name == cl->name()) {
      // constructor, remove useless ${}
      if ((index = def.find("${static}")) != -1)
	def.remove(index, 9);
      if ((index = def.find("${type}")) != -1)
	def.remove(index, (((const char *) def)[index + 7] == ' ') ? 8 : 7);
      if ((index = def.find("${final}")) != -1)
	def.remove(index, 8);
      if ((index = def.find("${abstract}")) != -1)
	def.remove(index, 11);
    }
    
    if (type.type != 0) {
      UmlClass::manage_generic(def, type, str_actuals, "${type}"); 
      op->set_ReturnType(type);
    }
    else if (first_actual_class != 0) {
      UmlTypeSpec t;
      
      t.type = first_actual_class;
      def.replace(def.find("${type}"), 7, type_def);
      op->set_ReturnType(t);
    }
    else if (!type.explicit_type.isEmpty()) {
      // not a contructor 
      op->set_ReturnType(type);
    }
  }
  
  // parameters
  
  unsigned rank = 0;
  UmlParameter param;
  
  while (read_param(container, rank, tmplts, param, def, op == 0)) {
    if ((op != 0) && ! op->addParameter(rank++, param)) {
      JavaCatWindow::trace(QCString("<font face=helvetica><b>cannot add param <i>")
			   + name + "</i> in <i>" + cl->name() 
			   + "</i></b></font><br>");  
#ifdef TRACE
      cout << "ERROR cannot add param '" << param.name << "' type '" << param.type.Type() << '\n';
#endif
      return FALSE;
    }
  }
  
  QCString s = Lex::read_word();
  
  if (!s.isEmpty() && (*((const char *) s) == '[')) {
    if (op != 0) {
      // do not place it at the same place
      def.insert(def.find("${type}") + 7, s);
    }
    s = Lex::read_word();
  }
  
  if (s.isEmpty()) {
    Lex::premature_eof();
    return FALSE;
  }
  
  if (s == "throws") {
    
    // throws
    
    rank = 0;
    
    for (;;) {
      if ((s = Lex::read_word()).isEmpty()) {
	Lex::premature_eof();
	return FALSE;
      }
      
      if (op != 0) {
	UmlTypeSpec typespec;
	
	container->compute_type(s, typespec, tmplts);
	if (! op->addException(rank++, typespec)) {
#ifdef TRACE
	  cout << "cannot add exception " << s << '\n';
#endif
	  return FALSE;
	}
      }
      
      if (((s = Lex::read_word()) == "{") || (s == ";"))
	break;
      
      if (s != ",") {
	Lex::error_near(s);
	return FALSE;
      }
    }
  }
  
  // definition
  
  if (abstractp || nativep ||
      (cl->stereotype() == "interface") ||
      (cl->stereotype() == "@interface")) {
    if ((s == "default") && (cl->stereotype() == "@interface")) {
      int index = def.find("${)}");
      
      Lex::mark();
      s = Lex::read_word();
      if (s == "{") {
	int level = 1;
	
	for (;;) {
	  if ((s = Lex::read_word()).isEmpty())
	    return FALSE;
	  else if (s == "{")
	    level += 1;
	  else if ((s == "}") && (--level == 0))
	    break;
	}
	s = Lex::region();
      }
	
      def.insert(index + 4, " default" + s);
      s = Lex::read_word();	
    }
    if (s != ";") {
      Lex::error_near(s);
      return FALSE;
    }
#ifdef REVERSE
    if ((op != 0) && !container->from_libp())
      op->set_JavaBody(0);
#endif
  }
  else if (s != "{") {
    Lex::error_near(s);
    return FALSE;
  }
  else {
    Lex::mark();
    
    // goto the end of the body
    
    QCString e;
    int level = 1;	// '{' already read
    
    for (;;) {
      if ((e = Lex::read_word()).isEmpty())
	return FALSE;
      else if (e == "{")
	level += 1;
      else if ((e == "}") && (--level == 0))
	break;
    }
    
#ifdef REVERSE
    if ((op != 0) && !container->from_libp()) {
      e = Lex::region();
      e.truncate(e.length() - 1);	// remove }

      // remove fist \n
      if (*((const char *) e) == '\n')
	e.remove(0, 1);
      
      // remove last spaces and tabs
      int ln = e.length();
      
      while (ln && ((e[ln - 1] == ' ') || (e[ln - 1] == '\t')))
	ln -= 1;
      e.truncate(ln);
      
      op->set_JavaBody(e);
    }
#endif
  }
  
  if ((op != 0) && (def != JavaSettings::operationDef()))
    op->set_JavaDecl(def);
  
  Lex::clear_comments();	// params & body comments
  Lex::finish_line();
  if (!comment.isEmpty())
    if (op != 0) 
      op->set_Description((def.find("${description}") != -1)
			  ? description : comment);
  return TRUE;
}

bool UmlOperation::read_param(Class * container, unsigned rank,
			      const QValueList<FormalParameterList> & tmplts,
			      UmlParameter & param, QCString & def, bool bypass)
{
#ifdef TRACE
  cout << "UmlOperation::manage_param " << rank << "\n";
#endif
  
  bool finalp = FALSE;
  bool in = FALSE;
  bool ellipsis = FALSE;
  QCString array;
  bool type_read = FALSE;
  QValueList<UmlTypeSpec> actuals;
  QCString str_actuals;
  QCString annotation;
  
  param.name = param.default_value = 0;
  
  QCString s = Lex::read_word();
  
#ifdef TRACE
  cout << "commence par " << s << '\n';
#endif
  
  if (s == ")")
    return FALSE;
  
  for (;;) {
    if (s == "final")
      finalp = TRUE;
    else if ((s == "void") || (s == "byte") || (s == "char") ||
	     (s == "short") || (s == "int") || (s == "long") ||
	     (s == "float") || (s == "double")) {
      if (type_read) {
	Lex::error_near(s);
	return FALSE;
      }
      param.type.type = 0;
      param.type.explicit_type = s;
      type_read = TRUE;
      in = TRUE;
    }
    else if ((s == ")") || (s == ",")) {
      if (param.name.isEmpty() && !type_read) {
	Lex::error_near(s);
	return FALSE;
      }
      if (s == ")")
	Lex::unread_word(s);
      
      if (! bypass) {      
	param.dir = (finalp || in)
	  ? InputDirection : InputOutputDirection;
	
	QCString s;
	
	if (rank != 0)
	  s = ", ";
	if (! annotation.isEmpty())
	  s += annotation + " ";
	if (finalp)
	  s += "final ";
	if ((param.type.type != 0) &&
	    !param.type.explicit_type.isEmpty())
	  s += param.type.explicit_type;
	else {
	  s += "${t";
	  s += QCString().setNum(rank);
	  s += "}";
	  if (param.type.type != 0)
	    s += str_actuals;
	}
	s += array;
	s += (ellipsis) ? " ... ${p": " ${p";
	s += QCString().setNum(rank);
	s += "}";
	def.insert(def.find("${)}"), 	// cannot be -1
		   s);
      }
      return TRUE;
    }
    else if (Lex::identifierp(s)) {
      if (!type_read) {
	while (s.at(s.length() - 1) == '.') {
	  // type on several lines, managed in this case
	  QCString s2 = Lex::read_word();
	  
	  if (Lex::identifierp(s2))
	    s += s2;
	  else {
	    Lex::error_near(s);
	    return FALSE;
	  }
	}
#ifdef TRACE
	cout << "type = '" << s << "...'\n";
#endif
	if (! bypass) {
	  QCString dummy;
	  
	  container->read_type(param.type, 0, tmplts, 0, str_actuals, s,
			       0, dummy, dummy);

	  if (param.type.explicit_type == "String")
	    // at least for it !
	    in = TRUE;
	}
	else
	  container->bypass_type(s);
	
	type_read = TRUE;
      }
      else if (param.name.isEmpty()) {
	if (s == "...")
	  ellipsis = TRUE;
	else {
	  param.name = s;
#ifdef TRACE
	  cout << "name = '" << param.name << "'\n";
#endif
	}
      }
      else {
	Lex::error_near(s);
#ifdef TRACE
	cout << "ERROR '" << s << "' alors qu a deja le type et le nom '" << param.name << "'\n";
#endif
	return FALSE;
      }
    }
    else if (*((const char *) s) == '@')
      annotation = s;
    else if (*((const char *) s) == '[')
      array = s;
    else {
      Lex::error_near(s);
#ifdef TRACE
      cout << "ERROR : '" << s << "'\n";
#endif
      return FALSE;
    }
    
    s = Lex::read_word();
  }
}
	 
void UmlOperation::skip_body(int level) {
  QCString e;
    
  while (!((e = Lex::read_word()).isEmpty()) &&	// end of file
	 ((e != "}") || (--level != 0)) &&	// end of body
	 ((e != ";") || (level != 0)))		// no body
    if (e == "{")
      level += 1;
  
  Lex::clear_comments();
}
	 
QCString UmlOperation::skip_expr(int level) {
  QCString e;
  int templ_level = 0;
    
  while (!((e = Lex::read_word(templ_level != 0)).isEmpty()) &&
	 ((e != ";") || (level != 0)) &&
	 ((e != ",") || (level != 0))) {
    if ((e == "{") || (e == "(") || (e == "["))
      level += 1;
    else if ((e == "}") || (e == ")") || (e == "]"))
      level -= 1;
    else if (e == "<") {
      if ((level == 0) || (templ_level != 0)) {
	level += 1;
	templ_level += 1;
      }
    }
    else if (e == ">") {
      if (templ_level != 0) {
	level -= 1;
	templ_level -= 1;
      }
    }
  }
  
  Lex::clear_comments();
  return e;
}
