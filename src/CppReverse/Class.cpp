// *************************************************************************
//
// Copyright 2004-2009 Bruno PAGES  .
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

#ifdef DEBUG_BOUML
#include <iostream>

using namespace std;
#endif

#include <qstringlist.h>

#include "Class.h"
#include "Package.h"
#include "UmlPackage.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "CppSettings.h"
#include "UmlExtraClassMember.h"
#include "Lex.h"
#include "UmlCom.h"
#ifdef REVERSE
#include "CppCatWindow.h"
#endif
#include "Statistic.h"


Class::Class(BrowserNode * parent, const char * name, const QCString & st)
    : BrowserNode(parent, name), uml(0), reversedp(FALSE),
#ifdef REVERSE
      from_lib(FALSE)
#else
      have_formals(FALSE)
#endif
{        
  if (!st.isEmpty()) {
    stereotype_declared = TRUE;
    if (st != "class")
      stereotype = st;
  }
  else
    stereotype_declared = FALSE;
}

// to not have warning
Class::~Class() {
}

Class * Class::reverse(ClassContainer * container, QCString stereotype, 
		       const QValueList<FormalParameterList> & tmplts,
		       const QCString & path, QCString name)
{
  bool from_typedef = !name.isEmpty();
  QCString comment = Lex::get_comments();
  QCString description = Lex::get_description();
  QCString q_modifier;
  
  if (!from_typedef)
    if ((name = Lex::read_word()).isEmpty())
      return 0;
  
  if ((strncmp(name, "Q_EXPORT", 8) == 0) ||
      (strncmp(name, "QM_EXPORT", 9) == 0) ||
      (strncmp(name, "Q_PNGEXPORT", 11) == 0)) {
    q_modifier = name + " ";
    
    if ((name = Lex::read_word()).isEmpty())
      return 0;
  }
  
  QCString s;
  
  if ((s = Lex::read_word()).isEmpty()) {
    Lex::premature_eof();
    return 0;
  }
  
  if (s == ";") {
    if (!Package::scanning()) {
      s = "    " + stereotype + ' ' + name + ";\n";
      container->declaration(name, stereotype, s);
    }
    Lex::finish_line();
    Lex::clear_comments();
    
    return 0;
  }
  
  // class definition
  
  Class * cl = container->define(Lex::normalize(name), stereotype);
  UmlClass * cl_uml = 0;
  
  if ((cl == 0) ||  cl->reversedp) {
    Lex::unread_word();
    return 0;
  }
  
  if (Package::scanning()) {
    cl->filename = path;
    cl->its_namespace = Namespace::current();
#ifdef REVERSE
    if (! tmplts.isEmpty())
      cl->formals = tmplts.first();
#else
    cl->have_formals = !tmplts.isEmpty();
    cl->description = comment;
#endif
  }
  else {
#ifdef REVERSE
    Statistic::one_class_more();
#endif
    cl->reversedp = TRUE;
    
    cl_uml = cl->get_uml();
    
    if (cl->already_in_bouml()) {
      cl_uml->unload();
      return 0;
    }
    
    QCString d;
    
    if (stereotype == "union")
      d = CppSettings::unionDecl();
    else if (stereotype == "struct")
      d = CppSettings::structDecl();
    else if (! q_modifier.isEmpty())
      d = CppSettings::classDecl();
    
    if (! d.isEmpty()) {
      if (! q_modifier.isEmpty()) {
	int index = d.find("${name}");
	
	if (index != -1)
	  d.insert(index, q_modifier);
      }
      cl_uml->set_CppDecl(d);
    }
    
    if (! comment.isEmpty()) {
      if (d.isEmpty())
	d = CppSettings::classDecl();
      cl_uml->set_Description((d.find("${description}") != -1)
			      ? description : comment);
    }
  }
  
  if (s == ":") {
    if (Package::scanning()) {
      char c;
      
      do 
	c = Lex::read_word_bis(FALSE, FALSE);
      while ((c != '{') && (c != 0));
      s = (c == 0) ? "" : "{";
    }
    else {
      if (! cl_uml->manage_inherit(container, tmplts
#ifdef REVERSE
				   ,cl->from_libp()
#endif
		  ))
	return 0;
      s = Lex::read_word();
    }
  }
  
  if (from_typedef &&
      (s != "{") &&
      (s = Lex::read_word()).isEmpty()) {
    if (! Package::scanning())
      Lex::premature_eof();
    return 0;
  }
  
  if (s != "{") {
    if (! Package::scanning())
      Lex::syntax_error("<font color =\"red\">{</font> is missing");
    UmlOperation::skip_body(0);
    return 0;
  }
  
  aVisibility visibility = 
    (stereotype == "class") ? PrivateVisibility : PublicVisibility;
  bool qt_object = (name == "QObject");
  bool contains_using_namespace = FALSE;
  bool contains_using_class = FALSE;
      
  if (cl_uml)
    cl_uml->set_under_construction(TRUE);
  
  while (Lex::mark(), ((s = Lex::read_word()) != "}")) {
    if (s.isEmpty()) {
      if (! Package::scanning())
	Lex::premature_eof();
      break;
    }
    
#ifdef DEBUG_BOUML
    cout << "in class def a lu '" << s << "'\n";
#endif
    
    if (s == "public")
      visibility = PublicVisibility;
    else if (s == "protected")
      visibility = ProtectedVisibility;
    else if (s == "private")
      visibility = PrivateVisibility;
    else if (qt_object && (s == "signals")) {
      if (Lex::read_word() != ":") {
	if (! Package::scanning())
	  Lex::syntax_error("<font color =\"red\">:</font> is missing after <font color =\"red\"> signals</font>");
	Lex::unread_word();
      }
      if (!Package::scanning())
	cl->declaration("signals", 0, "  signals:");
      continue;
    }
    else if (s == "Q_OBJECT") {
      if (!Package::scanning())
	cl->declaration("Q_OBJECT", 0, "  Q_OBJECT");
      qt_object = TRUE;
      continue;
    }
    else if ((s == "Q_PROPERTY") || (s == "Q_ENUMS") ||
	     (s == "Q_OVERRIDE") || (s == "Q_SETS") ||
	     (s == "Q_DUMMY_COMPARISON_OPERATOR")) {
      Lex::mark();
      Lex::read_word();	// (
      UmlOperation::skip_expr(")");
      if (!Package::scanning())
	cl->declaration(s, 0, "    " + s + Lex::region());
      continue;
    }
    else if (s == "using") {
      if ((s = Lex::read_word()).isEmpty()) {
	if (!Package::scanning())
	  Lex::premature_eof();
	break;
      }
      else if (s == "namespace") {
	if (Package::scanning())
	  UmlOperation::skip_body(0);
	else if ((s = Lex::read_word()).isEmpty()) {
	  Lex::premature_eof();
	  break;
	}
	else {
	  if (!contains_using_namespace) {
	    contains_using_namespace = TRUE;
	    Namespace::save_using_scope();
	  }
	  Namespace::add_using(s);
	  cl->declaration("using", 0, "  using namespace " + s + ";");
	}
      }
      else if (!Package::scanning()) {
	s = Lex::complete_template_type(s);
	
	UmlTypeSpec typespec;
	
	if (!cl->find_type(s, typespec)) {
#if 0
	  int index = s.findRev("::");
	  
	  if ((index == -1) || !cl->find_type(s.left(index), typespec))
	    Lex::warn("unknown class <font color =\"red\">" +
		      Lex::quote((index == -1) ? s : s.left(index)) + "</font>");
#endif
	  cl->declaration("using", 0, "  using " + s + ";");
	}
	else {
	  if (!contains_using_class) {
	    contains_using_class = TRUE;
	    UmlClass::save_using_scope();
	  }
	  typespec.type->using_it();
	  cl->declaration("using", 0, "  using " + s + ";");
	}
      }
      else
	UmlOperation::skip_body(0);      
      continue;
    }
    else {
      cl->manage_member(s, visibility, container, path);	
      continue;
    }
    
#ifdef DEBUG_BOUML
    cout << "visi = " << s << '\n';
#endif
    
    s = Lex::read_word();
    
    if (qt_object && (s == "slots")) {
      s = Lex::read_word();
      if (!Package::scanning() && (s == ":")) {
	switch (visibility) {
	case PublicVisibility:
	  cl->declaration("slots", 0, "  public slots:");
	  break;
	case ProtectedVisibility:
	  cl->declaration("slots", 0, "  protected slots:");
	  break;
	default:
	  cl->declaration("slots", 0, "  private slots:");
	  break;
	}
      }
    }
    
    if ((s != ":") && !Package::scanning())
      Lex::syntax_error("<font color =\"red\">:</font> is missing after visibility");
  }
  
  if (!from_typedef && !Package::scanning()) {
    if ((s = Lex::read_word()).isEmpty())
      Lex::premature_eof();
    else if (s != ";") {
      Lex::error_near(s);
      UmlOperation::skip_body((s == "{") ? 1 : 0);
    }
  }
  
  if (cl_uml) {
    cl_uml->set_under_construction(FALSE);
#ifdef REVERSE
    if (!cl->from_libp())
      cl_uml->need_artifact(cl->get_namespace());
#endif
    
    // do not unload cl_uml because of the (sub class) friend operations
  }
  
  if (contains_using_namespace)
    Namespace::restore_using_scope();
  
  if (contains_using_class)
    UmlClass::restore_using_scope();

  return cl;
}

void Class::declaration(const QCString & name, const QCString & stereotype,
			const QCString & decl) {
  UmlExtraClassMember * ex = UmlExtraClassMember::create(get_uml(), name);
  
  if (ex != 0) {
    if (! stereotype.isEmpty())
      ex->set_Stereotype(stereotype);
    if (! decl.isEmpty())
      ex->set_CppDecl(decl);
  }
}

void Class::manage_member(QCString s, aVisibility visibility,
			  ClassContainer * /*container*/,
			  const QCString & path) {
  QCString comment = Lex::get_comments();
  QCString description = Lex::get_description();
  
  if (s == ";")
    return;
  
  bool destructor = FALSE;
  bool staticp = FALSE;
  bool virtualp = FALSE;
  bool constp = FALSE;
  bool mutablep = FALSE;
  bool volatilep = FALSE;
  bool typenamep = FALSE;
  bool inlinep = FALSE;
  bool explicitp = FALSE;
  bool friendp = FALSE;
  QCString q_modifier;	// not yet used
  QCString modifier;
  QCString type;
  QCString name;
  QCString array;
  QCString value;
  QCString bitfield;
  QCString pretype;	// struct/union/class/enum
  QValueList<FormalParameterList> tmplts;
  QCString friend_template;
  
#ifdef DEBUG_BOUML
  cout << "Class::manage_member(" << s << ")\n";
#endif
  
  for (;;) {
    if (s == "static") {
      staticp = TRUE;
      Lex::mark();
    }
    else if (s == "virtual") {
      virtualp = TRUE;
      Lex::mark();
    }
    else if (s == "const") {
      if (!modifier.isEmpty())
	modifier += QCString(" ") + s;
      else {
	constp = TRUE;
	Lex::mark();
      }
    }
    else if (s == "mutable") {
      if (!modifier.isEmpty())
	modifier += QCString(" ") + s;
      else {
	mutablep = TRUE;
	Lex::mark();
      }
    }
    else if (s == "volatile") {
      if (!modifier.isEmpty())
	modifier += QCString(" ") + s;
      else {
	volatilep = TRUE;
	Lex::mark();
      }
    }
    else if (s == "typename") {
      // note : may have "const typename ...", but not "typename const ..."
      if (!modifier.isEmpty())
	modifier += QCString(" ") + s;
      else {
	typenamep = TRUE;
	Lex::mark();
      }
    }
    else if (s == "inline") {
      inlinep = TRUE;
      Lex::mark();
    }
    else if (s == "explicit") {
      explicitp = TRUE;
      Lex::mark();
    }
    else if ((s == "unsigned") || (s == "signed") ||
	     (s == "void") || (s == "bool"))
      type = s;
    else if ((s == "char") || (s == "short") || (s == "int") || 
	     (s == "long") || (s == "float") || (s == "double")) {
      type = (type.isEmpty()) ? s : type + ' ' + s;
    }
    else if (Lex::star(s) || (s == "&") || (s == "volatile")) {
      if (!modifier.isEmpty())
	modifier += QCString(" ") + s;
      else if (!type.isEmpty())
	modifier = s;
      else {
#ifdef DEBUG_BOUML
	cout << "ERROR modifier " << s << " et type empty\n";
#endif
	if (!Package::scanning())
	  Lex::error_near(s);
	break;
      }
    }
    else if (s == "(") {
      // suppose an operation
      if (Package::scanning())
	break;
      
      if (type.isEmpty()) {
	if (!Package::scanning())
	  Lex::error_near("(");
	break;
      }
      
      bool func = FALSE;
      QCString cl_name = QCString(text(0));
      QCString cl_full_name = cl_name;
      int index;
      
      if ((index = cl_name.find('<')) != -1)
	cl_name.truncate(index);
	  
      if (name.isEmpty()) {
	if ((type == cl_full_name) ||
	    (type == (QCString("~") + cl_full_name))) {
	  name = type;
	  type = "";
	}
	else if ((type == cl_name) ||
		 (type == (QCString("~") + cl_name))) {
	  name = type;
	  type = "";
	}
	else if (type.left(9) == "operator ") {
	  name = type;
	  type = 0;
	}
	else {
	  QCString typ;
	  
	  if (((index = type.find('<')) != -1) &&
	      // wrong tests : not so simple
	      (((typ = type.left(index)) == cl_name) ||
	       (typ == (QCString("~") + cl_name)))) {
	    name = type;
	    type = "";
	  }
	  else {
	    if (! UmlOperation::pfunc(func, name, type, "${name}"))
	      break;
	    //???modifier = FALSE;
	    if (!func) {
	      s = Lex::read_word();
	      continue;
	    }
	  }
	}
      }
      
      UmlOperation::new_one(this, name, tmplts, type, modifier, pretype, visibility,
			    inlinep, virtualp, staticp, constp, volatilep,
			    typenamep, explicitp, friendp, friend_template,
			    comment, description, func);
      return;
    }
    else if (s == "=") {
      // initialized variable, by pass value
      Lex::mark();
      UmlOperation::skip_expr(";");
      Lex::unread_word();	// ';'

      if (Package::scanning())
	break;
      
      value = "= " + Lex::region();
    }
    else if (s == ";") {
      if (Package::scanning())
	return;
      
      if (name.isEmpty() || type.isEmpty()) {
	Lex::error_near(";");
	return;
      }
      
      UmlTypeSpec dest_type;
      QCString typeform = "${type}";
      
      if (CppSettings::umlType(type).isEmpty() &&
	  (modifier.isEmpty() || (modifier == "*") || (modifier == "&")) &&
	  bitfield.isEmpty() &&
	  !typenamep &&
	  (compute_type(type, dest_type, typeform, TRUE), dest_type.type != 0) &&
	  ((typeform == "${type}") || (array.isEmpty() && modifier.isEmpty())))
	UmlRelation::new_one(get_uml(), name, dest_type.type, modifier, pretype,
			     array, typeform, visibility, staticp, constp,
			     mutablep, volatilep, value, comment, description);
      else
	UmlAttribute::new_one(this, name, type, modifier, pretype, array,
			      visibility, staticp, constp, typenamep, mutablep,
			      volatilep, bitfield, value, comment, description);
      
      return;
    }
    else if (s == "friend") {
      s = Lex::read_word();
      if (s == "QMAC_PASCAL")
	s = Lex::read_word();
      if (s.isEmpty()) {
	if (! Package::scanning())
	  Lex::premature_eof();
	break;
      }
      else if ((s != "class") && (s != "struct") && (s != "union")) {
#if 0
	if (! Package::scanning()) {
	  Lex::unread_word();
	  UmlOperation::skip_body(0);
	  s = Lex::region() + '\n';
	  declaration(0, "friend", s);
	  return;
	}
	break;
#else
	friendp = TRUE;
	Lex::unread_word();
	Lex::mark();
#endif
      }
      else if (Package::scanning())
	break;
      else {      
	s = Lex::read_word();
	
	int index = s.find('<');
	
	if (index != -1)
	  s.truncate(index);
	
	UmlTypeSpec other;
	QCString typeform;
	
	if (s.isEmpty()) {
	  Lex::error_near("friend");
	  break;
	}
	else if (compute_type(s, other, typeform), other.type == 0)
	  Lex::warn("unknown class <font color =\"red\">" + Lex::quote(s) + "</font>");
	s = Lex::read_word();
	if (s.isEmpty() || (s != ";")) {
	  Lex::error_near("friend");
#ifdef DEBUG_BOUML
	  cout << "ERROR ';' doit terminer un 'friend'\n";
#endif
	  break;
	}
	
	if (other.type != 0)
	  UmlRelation::new_friend(get_uml(), other.type);
	else {
	  UmlExtraClassMember * ex = 
	    UmlExtraClassMember::create(get_uml(), "friend");
	  
	  ex->set_CppDecl(Lex::region());
	}
	
	Lex::finish_line();
	Lex::clear_comments();      
	return;
      }
    }
    else if ((s == "struct") ||
	     (s == "union") ||
	     (s == "enum")) {
      Lex::mark();
      
      QCString s2 = Lex::read_word();
	
      if ((strncmp(s2, "Q_EXPORT", 8) == 0) ||
	  (strncmp(s2, "QM_EXPORT", 9) == 0)||
	  (strncmp(s2, "Q_PNGEXPORT", 11) == 0))
	s2 = Lex::read_word();
      
      if (s2 == "{") {
	// anonymous type
	UmlOperation::skip_body(1);
	
	if (Package::scanning())
	  return;
	
	if (! type.isEmpty()) {
	  if (! Package::scanning())
	    Lex::error_near(s);
#ifdef DEBUG_BOUML
	  cout << "ERROR '" << s << " {' alors qu a deja le type '" << type << "' et le nom '" << name << "'\n";
#endif
	  return;
	}
	
	type = s + " " + Lex::region();
	if ((s2 = Lex::read_word()) == ";") {
	  type = 0;
	  Lex::come_back();
	  if (s == "enum")
	    reverse_enum(this, path);
	  else
	    reverse(this, s, tmplts, path);
	  return;
	}
	else
	  Lex::unread_word();
      }
      else if (! Lex::identifierp(s2, TRUE)) {
	if (! Package::scanning())
	  Lex::error_near(s2);
	return;	
      }
      else if ((s2 = Lex::read_word()) == ";") {
	// form like 'struct X;'
	Lex::come_back();
	reverse(this, s, tmplts, path);
	return;
      }
      else if ((s2 != "{") && (s2 != ":")) {
	// form like 'struct X Y'
	pretype = s;
	Lex::come_back();
      }
      else {
	Lex::come_back();
	
	if (s == "enum")
	  reverse_enum(this, path);
	else
	  reverse(this, s, tmplts, path);
	return;
      }
    }
    else if (s == "class") {
      Lex::mark();
      
      QCString s2 = Lex::read_word();
	
      if ((strncmp(s2, "Q_EXPORT", 8) == 0) ||
	  (strncmp(s2, "QM_EXPORT", 9) == 0)||
	  (strncmp(s2, "Q_PNGEXPORT", 11) == 0))
	s2 = Lex::read_word();
      
      if (! Lex::identifierp(s2, TRUE)) {
	if (! Package::scanning())
	  Lex::error_near(s2);
	return;	
      }
      else if (((s2 = Lex::read_word()) != "{") && (s2 != ":") && (s2 != ";")) {
	// form like 'class X Y'
	pretype = s;
	Lex::come_back();
      }
      else {
	Lex::come_back();
	reverse(this, s, tmplts, path);
	return;
      }
    }
    else if (s == "typedef") {
      reverse_typedef(this, path, tmplts);
      return;
    }
    else if (s == "template") {
      FormalParameterList tmplt;
      
      tmplts.append(tmplt);
      if (!get_template(tmplts.last()))
	break;
    }
    else if (s == "QT_STATIC_CONST")
      staticp = constp = TRUE;
    else if ((strncmp(s, "Q_EXPORT", 8) == 0) ||
	     (strncmp(s, "QM_EXPORT", 9) == 0) ||
	     (strncmp(s, "Q_PNGEXPORT", 11) == 0))
      q_modifier = s;
    else if ((s == "QMAC_PASCAL")|| (s == "Q_TYPENAME"))
      ; // lost
    else if (Lex::identifierp(s, FALSE)) {
      if (type.isEmpty()) {
	s = Lex::complete_template_type(s);
#ifdef DEBUG_BOUML
	cout << "type = '" << s << "'\n";
#endif
	type = (destructor) ? QCString("~") + s : s;	// will be the name if ~
      }
      else if (name.isEmpty()) {
	name = s;
#ifdef DEBUG_BOUML
	cout << "name = '" << name << "'\n";
#endif
      }
      else {
	if (!Package::scanning())
	  Lex::error_near(s);
#ifdef DEBUG_BOUML
	cout << "ERROR '" << s << "' alors qu a deja le type '" << type << "' et le nom '" << name << "'\n";
#endif
	break;
      }
    }
    else if (s == "~")
      destructor = TRUE;
    else if (s.isEmpty()) {
      if (!Package::scanning())
	Lex::premature_eof();
      return;
    }
    else if (*((const char *) s) == '[')
      array += s;
    else if (s == ":") {
      bitfield = Lex::read_word();
    }
    else if ((s == "<") && friendp &&
	     friend_template.isEmpty() && Lex::finish_template(s))
      friend_template = " " + s;
    else {
      if (!Package::scanning())
	Lex::error_near(s);
#ifdef DEBUG_BOUML
      cout << "ERROR : '" << s << "'\n";
#endif
      break;
    }
    
    s = Lex::read_word();
  }
  
  // here the end of member must be skip, because of an error
  // or because it is useless in scan case
  UmlOperation::skip_body(0);
}

Class * Class::reverse_enum(ClassContainer * container, 
			    const QCString & path, QCString name)
{
  bool from_typedef = !name.isEmpty();
  QCString comment = Lex::get_comments();
  QCString description = Lex::get_description();
  QCString s;
    
  if (!from_typedef) {
    if ((name = Lex::read_word()).isEmpty()) {
      if (!Package::scanning())
	Lex::premature_eof();
      return 0;
    }
    
    if (name == "{") {
      if (!Package::scanning()) {
	Lex::unread_word();
	name = 0;
      }
      else {
	// will see later
	UmlOperation::skip_body(1);
	return 0;
      }
    }
  }
  
  if ((s = Lex::read_word()).isEmpty()) {
    if (!Package::scanning())
      Lex::premature_eof();
    return 0;
  }
  
  if (s == ";") {
    Lex::finish_line();
    Lex::clear_comments();
    return (Package::scanning())
      ? 0 : container->declare_if_needed(name, "enum");
  }
  
  if (from_typedef &&
      (s != "{") &&
      (s = Lex::read_word()).isEmpty()) {
    if (!Package::scanning())
      Lex::premature_eof();
    return 0;
  }
  
  if (s != "{") {
    if (!Package::scanning())
      Lex::syntax_error(QCString("font color =\"red\">{</font> expected rather than <font color =\"red\"> ")
			+ s + "</font>");
    return 0;
  }

  // enum definition
  Class * cl = container->define(name, "enum");
  
  if ((cl == 0) || cl->reversedp) {
    Lex::unread_word();
    return 0;
  }
  
  if (Package::scanning()) {
    cl->filename = path;
    cl->its_namespace = Namespace::current();
#ifndef REVERSE
    cl->description = comment;
#endif
    UmlOperation::skip_body(1);
    return cl;
  }

#ifdef REVERSE
  Statistic::one_class_more();
#endif
  cl->reversedp = TRUE;
  
  UmlClass * cl_uml = cl->get_uml();
  
  if (!name.isEmpty() && cl->already_in_bouml()) {
    cl_uml->unload();
    UmlOperation::skip_body(1);
    return 0;
  }
  
  int index;
  
  s = CppSettings::enumDecl();
  
  if (! comment.isEmpty())
    cl_uml->set_Description((s.find("${description}") != -1)
			    ? description : comment);
  
  if (name.isEmpty() && ((index = s.find("${name}")) != -1))
    // define a C++ anomymous enum even the enum have a name in uml
    s.remove(index, 7);
  cl_uml->set_CppDecl(s);
  
  for (;;) {
    if ((s = Lex::read_word()).isEmpty()) {
      Lex::premature_eof();
      return 0;
    }
    
    if (! Lex::identifierp(s, TRUE)) {
      if (s == "}")
	break;
      
      Lex::syntax_error(QCString("enum item expected rather than <font color =\"red\"> ")
			+ s + "</font>");
      UmlOperation::skip_body(1);
      return 0;
    }
    
    UmlAttribute * it = UmlBaseAttribute::create(cl_uml, s);
    
    if (it == 0) {
      Lex::warn("<font color =\"red\"> " + Lex::quote(s) +
                "</font> multiply defined");
      UmlOperation::skip_body(1);
      return 0;
    }
    
    Statistic::one_attribute_more();
    
    it->set_CppDecl(CppSettings::enumItemDecl());
    it->set_Visibility(PublicVisibility);
    
    if ((s = Lex::read_word()).isEmpty()) {
      Lex::premature_eof();
      return 0;
    }
    
    if (s == "}")
      break;
    
    if (s == "=") {
      Lex::unread_word();
      Lex::mark();
      UmlOperation::skip_expr("},");
      Lex::unread_word();	// '}' or ','
      it->set_DefaultValue(Lex::region());
      
      if ((s = Lex::read_word()).isEmpty()) {
	Lex::premature_eof();
	return 0;
      }
      if (s == "}")
	break;
    }
    
    if (s != ",") {
      Lex::error_near(s);
      UmlOperation::skip_body(1);
      return 0;
    }
  }
  
#ifdef REVERSE
  if (!cl->from_libp())
    cl_uml->need_artifact(cl->get_namespace());
#endif
  
  cl_uml->unload();
  
  return cl;  
}

bool Class::reverse_typedef(ClassContainer *  container, const QCString & path,
			    QValueList<FormalParameterList> & tmplts)
{
  Lex::mark();
  
  QCString s;
  QCString prefix;
  
  if ((s = Lex::read_word(/*TRUE*/)).isEmpty()) {
    if (!Package::scanning())
      Lex::premature_eof();
    return FALSE;
  }

  if ((s == "enum") || (s == "struct") || (s == "union") || (s == "class")) {
    Lex::mark();
  
    QCString name1 = Lex::read_word();
    QCString name2;
    
    UmlOperation::skip_body((name1 == "{") ? 1 : 0);
    
    if ((name1 != "{") && (Lex::region().find('{') == -1)) {
      // form like 'typedef struct X Y;'
      Lex::come_back();
      prefix = s;
      s = Lex::read_word(TRUE);
    }
    else {
      do {
	if ((name2 = Lex::read_word()).isEmpty()) {
	  if (!Package::scanning())
	    Lex::premature_eof();
	  return FALSE;
	}
      } while ((name2 != ";") && !Lex::identifierp(name2, TRUE));
      
      Lex::come_back();
      
      if (name1 == "{") {
	if (name2 == ";") {
	  if (!Package::scanning())
	    Lex::syntax_error();
	  return FALSE;
	}
	name1 = name2;
      }
      else if (name2 == ";")
	// for instance 'typedef struct name1 { ...};'
	name2 = name1;
      
      Class * cl;
      
      if (s == "enum") {
	if ((cl = Class::reverse_enum(container, path, name1)) == 0)
	  return FALSE;
      }
      else if ((cl = Class::reverse(container, s, tmplts, path, name1)) == 0)
	return FALSE;
      
      // here '}' is the last item read
      
      Lex::mark();
      
      char c;
      
      while ((c = Lex::read_word_bis(TRUE, FALSE)) != ';') {
	if (c == 0) {
	  if (!Package::scanning())
	    Lex::premature_eof();
	  return FALSE;
	}
      }
      
      if (name1 != name2) {
	Class * ty = container->define(name2, "typedef");
	
	if (Package::scanning()) {
	  if (ty != 0)
	    ty->its_namespace = Namespace::current();
	  return (ty != 0);
	}
	
	UmlClass * ty_uml;
	
	if ((ty == 0) || ((ty_uml = ty->get_uml()) == 0))
	  return FALSE;
	
#ifdef REVERSE
	Statistic::one_class_more();
#endif
	ty->reversedp = TRUE;
	ty->its_namespace = Namespace::current();
	
	if (ty->already_in_bouml()) {
	  ty_uml->unload();
	  return 0;
	}
	
	QCString decl = CppSettings::typedefDecl();
	int index = decl.find("${name}");
	
	if (index == -1) {
	  decl = "${comment}typedef ${type} ${name};";
	  index = decl.find("${name}");
	}
	
	s = Lex::region();
	s.truncate(s.length() - 1);	// remove ';'
	if (s[0] == ' ')
	  s.remove(0, 1);

	int index2 = s.find(name2);
	
	if (index2 == -1) {
	  // severe syntax error
	  Lex::come_back();
	  return FALSE;
	}

	s.replace(s.find(name2), name2.length(), "${name}");
	decl.replace(index, 7, s);
	ty_uml->set_CppDecl(decl);
	
	UmlTypeSpec t;
	
	if ((t.type = cl->get_uml()) == 0)
	  return FALSE;
	ty_uml->set_BaseType(t);
	
#ifdef REVERSE
	if (!ty->from_libp())
	  ty_uml->need_artifact(ty->get_namespace());
#endif
	
	ty_uml->unload();
      }
      
      return TRUE;
    }
  }
  
  QCString comment = Lex::get_comments();
  QCString description = Lex::get_description();
  //QValueList<QCString> actuals;
  //bool tmpl = FALSE;
  QCString type;
  UmlTypeSpec base_type;
  QCString typeform = "${type}";
  
  for (;;) {
    if ((s == "unsigned") || (s == "signed") ||
	(s == "void") || (s == "bool")) {
      type = s;
      Lex::mark();
    }
    else if ((s == "char") || (s == "short") || (s == "int") || 
	     (s == "long") || (s == "float") || (s == "double")) {
      type = (type.isEmpty()) ? s : type + ' ' + s;
      Lex::mark();
    }
    else if (s == ";") {
      if (!Package::scanning())
	Lex::error_near(";");
      return FALSE;
    }
    else if (Lex::identifierp(s, FALSE) &&
	     (s != "typename") && (s != "Q_TYPENAME") &&
	     (s != "const") && (s != "mutable") && (s != "volatile")) {
      if (type.isEmpty()) {
	type = s = Lex::complete_template_type(s);
	Lex::mark();
	
	if (!Package::scanning())
	  container->compute_type(s, base_type, typeform, TRUE);
      }
      else
	break;
    }
    else if (type.isEmpty()) {
      if (prefix.isEmpty())
	prefix = Lex::region();
      else
	prefix = prefix + " " + Lex::region();
    }
    
    if ((s = Lex::read_word(/*!tmpl*/)).isEmpty()) {
      if (!Package::scanning())
	Lex::premature_eof();
      return FALSE;
    }
  }
  
  // here 'type' is the base type, 's' is the typedef name (I hope !)
  // prefix if the code before 'type' and Lex::region() the code after
  // 'type' up to 's' (including 's', may be except template actuals)
  
#ifdef DEBUG_BOUML
  cout << "typedef " << s << "...\n";
#endif    
  Class * ty = container->define(s, "typedef");
  
  if (Package::scanning()) {
    UmlOperation::skip_body();
    if (ty != 0)
      ty->its_namespace = Namespace::current();
    return (ty != 0);
  }
  
  QCString intermediate = Lex::region();
  UmlClass * ty_uml;
  
  if ((ty == 0) || ((ty_uml = ty->get_uml()) == 0)) {
#ifdef DEBUG_BOUML
    cout << ((ty == 0) ? "typedef ty\n" : "typedef ty_uml\n");
#endif    
    UmlOperation::skip_body();
    return FALSE;
  }
  
#ifdef REVERSE
  Statistic::one_class_more();
#endif
  ty->reversedp = TRUE;
  ty->its_namespace = Namespace::current();
  
  if (ty->already_in_bouml()) {
    UmlOperation::skip_body();
    ty_uml->unload();
    return TRUE;
  }
  
  if (base_type.type == 0)
    // for builtin types
    base_type.explicit_type = type;
  else
    UmlBaseRelation::create(aDependency, ty_uml, base_type.type);
  
  ty_uml->set_BaseType(base_type);
  
  /*
  if (tmpl && (base_type.type != 0)) {
    unsigned n = base_type.type->Formals().count();
    unsigned index = 0;
    
    for (QStringList::Iterator it = actuals.begin();
	 it != actuals.end();
	 ++it) {
      if (index == n) {
	Lex::warn("too many actuals for " + Lex::quote(base_type.type->Name()));
	break;
      }
      
      
      QCString typeform;
      UmlTypeSpec t;
      
      container->compute_type(QCString(*it), t, typeform);
      ty_uml->replaceActual(index++, t);
    }
  }*/
  
  intermediate.truncate(intermediate.length() - s.length()); // remove s
  if (intermediate[0] == ' ')
    // normally it is the case
    intermediate.remove(0, 1);
  
  char c;
  
  Lex::mark();
  
  while ((c = Lex::read_word_bis(TRUE, FALSE)) != ';') {
    if (c == 0) {
      Lex::premature_eof();
      return FALSE;
    }
  }
  
  s = Lex::region();
  s.truncate(s.length() - 1);	// remove ';', 's' is the code after the typedef name
  
  QCString decl = CppSettings::typedefDecl();
  int index;
  
  if ((decl.find("${name}") == -1) ||
      ((index = decl.find("${type}")) == -1)) {
    decl = "${comment}typedef ${type} ${name};";
    index = decl.find("${type}");
  }
  
  if (!prefix.isEmpty()) {
    decl.insert(index, prefix + " ");
    index += prefix.length() + 1;
  }
  
  if (!intermediate.isEmpty())
    decl.insert(index + 7, intermediate);
  
  if ((base_type.type != 0) && (typeform != "${type}"))
    decl.replace(index, 7, typeform);
  
  if (!s.isEmpty())
    decl.insert(decl.find("${name}") + 7, s);	// find cannot return -1
  
  ty_uml->set_CppDecl(decl);
  if (! comment.isEmpty())
    ty_uml->set_Description((decl.find("${description}") != -1)
			    ? description : comment);
  
  Lex::finish_line();
  Lex::clear_comments();
  
#ifdef REVERSE
  if (!ty->from_libp())
    ty_uml->need_artifact(ty->get_namespace());
#endif
  
  ty_uml->unload();
  
  return TRUE;
}

//

bool Class::set_stereotype(const QCString & st) {
  if (!st.isEmpty()) {
    if (stereotype_declared) {
      if (st != ((stereotype.isEmpty()) ? QCString("class") : stereotype)) {
	UmlCom::trace(QCString("<font face=helvetica><b><i>") +
		      QCString(text(0)) +
		      "</i> have several stereotypes</b></font><br><hr><br>");
	return FALSE;
      }
    }
    else if (st != "class")
      stereotype = st;
  }
  
  return TRUE;
}

Class * Class::declare_if_needed(const QCString & name,
				 QCString stereotype) {
  Class * result = 
    ClassContainer::declare_if_needed(name, stereotype, formals, 
				      declared, defined);

  if (result != 0)
    declare_if_needed(name, result);
  return result;
}

void Class::declare_if_needed(QCString name, Class * cl) {
  BrowserNode * p = (BrowserNode *) parent();
  
  ((p->isa_package()) ? ((ClassContainer *) ((Package *) p))
		      : ((ClassContainer *) ((Class *) p)))
    ->declare_if_needed(QCString(text(0)) + "::" + name, cl);
}

Class * Class::define(const QCString & name, QCString stereotype) {
  Class * result = 
    ClassContainer::define(name, stereotype, declared, defined);
  
  if (result != 0)
    define(name, result);
  return result;
}

void Class::define(QCString name, Class * cl) {
  if (! name.isEmpty()) {
    BrowserNode * p = (BrowserNode *) parent();
    
    ((p->isa_package()) ? ((ClassContainer *) ((Package *) p))
			: ((ClassContainer *) ((Class *) p)))
      ->define(QCString(text(0)) + "::" + name, cl);
  }
}

bool Class::find_type(QCString type, UmlTypeSpec & typespec) {
  if (! formals.isEmpty()) {
    FormalParameterList::ConstIterator it;
    
    for (it = formals.begin(); it != formals.end(); ++it) {
      if ((*it).name() == type) {
	typespec.type = 0;
	typespec.explicit_type = type;
	return TRUE;
      }
    }
  }
  
  return ((ClassContainer::find_type(type, typespec, defined)) ||
	  ((BrowserNode *) parent())->find_type(type, typespec));
}

Class * Class::new_class(const QCString & name,
			 const QCString & stereotype,
			 bool declaration) {
  Class * cl = new Class(this, name, stereotype);

  if (! name.isEmpty())
    ((declaration) ? declared : defined).insert(name, cl);
  
  return cl;
}

UmlClass * Class::get_uml() {
  if (uml != 0)
    return uml;

  UmlItem * p = (((BrowserNode *) parent())->isa_package())
    ? (UmlItem *) ((Package *) parent())->get_uml()->get_classview(get_namespace())
    : (UmlItem *) ((Class *) parent())->get_uml();
  QCString str = QCString(text(0));
  bool anonymous = str.isEmpty();
  
  for (;;) {
    if (anonymous) {
      static unsigned n = 0;
      
      str.sprintf("anonymous%u", ++n);
    }
    
    uml = UmlBaseClass::create(p, str);
    
    if (uml == 0) {
      // probably already exist
      QVector<UmlItem> ch = p->children();
      UmlItem * x;
      
      for (unsigned chindex = 0; chindex != ch.size(); chindex += 1) {
	if (((x = ch[chindex])->kind() == aClass) && (x->name() == str)) {
	  uml = (UmlClass *) x;

	  if (anonymous)
	    break;

	  FormalParameterList xformals = uml->formals();
	  
	  if (formals.count() == xformals.count()) {
	    FormalParameterList::ConstIterator end = formals.end();
	    FormalParameterList::ConstIterator it;
	    FormalParameterList::ConstIterator xit;
	    
	    for (it = formals.begin(), xit = xformals.begin();
		 it != end; 
		 ++it, ++xit) {
	      if (((*it).type() != (*xit).type()) ||
		  ((*it).name() != (*xit).name()) ||
		  ((*it).defaultValue().toString() !=
		   (*xit).defaultValue().toString())) {
		// uncompatible
		uml = 0;
		break;
	      }
	    }
	  }
	  else
	    // uncompatible
	    uml = 0;
	  break;
	}
      }
      
      if (uml == 0) {
#ifdef REVERSE
	UmlCom::message("");
	CppCatWindow::trace(QString("<font face=helvetica><b>cannot create class <i>")
			    + text(0) + "</i> under <i>"
			    + parent()->text(0) + "</b></font><br>");
	throw 0;
#else
	QMessageBox::critical(0, "Fatal Error", 
			      QString("<font face=helvetica><b>cannot create class <i>")
			      + text(0) + "</i> under <i>"
			      + parent()->text(0) + "</b></font><br>");
	QApplication::exit(1);
#endif
      }
      else if (! anonymous)
	break;
    }
    else {
      FormalParameterList::ConstIterator end = formals.end();
      FormalParameterList::ConstIterator it;
      unsigned index;
	
      for (index = 0, it = formals.begin(); it != end; index += 1, ++it)
	uml->addFormal(index, *it);
      break;
    }
  }
  
  if (!stereotype.isEmpty())
    uml->set_Stereotype(stereotype);
  
  return uml;
}

void Class::set_uml(UmlClass * c) {
  uml = c;
}

bool Class::already_in_bouml() {
  QVector<UmlItem> ch = get_uml()->children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    if (ch[index]->kind() != aClass)
      return TRUE;

  return FALSE;
}

bool Class::isa_package() const {
  return FALSE;
}

QString Class::get_path() const {
  return filename;
}
