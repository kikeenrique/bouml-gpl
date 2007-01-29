// *************************************************************************
//
// Copyright (C) 2004-2007 Bruno PAGES  All rights reserved.
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

#include "UmlAttribute.h"
#include "UmlPackage.h"
#include "UmlOperation.h"
#include "Package.h"
#include "UmlClass.h"
#include "Class.h"
#include "JavaCatWindow.h"
#include "JavaSettings.h"
#include "Lex.h"
#ifdef REVERSE
#include "Statistic.h"
#endif

bool UmlAttribute::new_one(Class * container, const QCString & name,
			   UmlTypeSpec typespec, aVisibility visibility,
			   bool staticp, bool finalp, bool transientp,
			   bool volatilep, const QCString & array,
			   const QCString & value, QCString comment,
			   QCString description, QCString annotation)
{
#ifdef TRACE
  cout << "ATTRIBUTE '" << name << "'\n";
#endif
  
  if (
#ifdef REVERSE
      container->from_libp() &&
#endif
      (visibility == PrivateVisibility)) {
    Lex::finish_line();
    Lex::clear_comments();
    return TRUE;
  }
  
  UmlClass * cl = container->get_uml();
  UmlAttribute * at = UmlBaseAttribute::create(cl, name);
  
  if (at == 0) {
    JavaCatWindow::trace(QCString("<font face=helvetica><b>cannot add attribute <i>")
			 + name + "</i> in <i>" + cl->name() 
			 + "</i></b></font><br>");  
    return FALSE;
  }
#ifdef REVERSE
  Statistic::one_attribute_more();
#endif
  
  Lex::finish_line();
  
  comment = Lex::get_comments(comment);
  description = Lex::get_description(description);
  
  QCString decl = JavaSettings::attributeDecl();
  int index = decl.find("${type}");
  
  if ((index == -1) || (decl.find("${name}") == -1)) {
    decl = "  ${comment}${@}${visibility}${static}${final}${transient}${volatile}${type} ${name}${value};";
    index = decl.find("${type}");
  }
  
  if (!comment.isEmpty())
    at->set_Description((decl.find("${description}") != -1)
			? description : comment);
  
  if (finalp)
    at->set_isReadOnly(TRUE);
  
  if (transientp)
    at->set_isJavaTransient(TRUE);
  
  if (volatilep)
    at->set_isVolatile(TRUE);
  
  if (staticp)
    at->set_isClassMember(TRUE);
  
  if (!array.isEmpty())
    decl.insert(index + 7, array);
  
  if (! value.isEmpty())
    at->set_DefaultValue(value);
  
  if (! annotation.isEmpty())
    at->set_JavaAnnotations(annotation);

  at->set_Visibility(visibility);
  
  if (cl->stereotype() == "enum") {
    at->set_JavaDecl(decl);
    at->set_Stereotype("attribute");
  }
  else if (decl != JavaSettings::attributeDecl())
    at->set_JavaDecl(decl);
  
  return at->set_Type(typespec);
}

bool UmlAttribute::manage_enum_item(QCString name, UmlClass * cl)
{   
  QCString comment = Lex::get_comments();
  QCString description = Lex::get_description();
  UmlAttribute * item = 0;	// initialize to avoid warning
  
  if (!Package::scanning()) {
    if ((item = UmlBaseAttribute::create(cl, name)) == 0) {
      JavaCatWindow::trace(QCString("<font face=helvetica><b>cannot add enum item <i>")
			   + name + "</i> in <i>" + cl->name() 
			   + "</i></b></font><br>");  
      return FALSE;
    }
    
    item->set_Visibility(PublicVisibility);
  }
  Lex::mark();
  
  QCString aux;

  QCString s;
  
  if ((s = Lex::read_word()).isEmpty()) {
    if (! Package::scanning())
      Lex::premature_eof();
    return FALSE;
  }
  else if ((s == ";") || (s == "}")) {
    aux = Lex::region();
    Lex::unread_word(s);
  }
  else if (s == ",") {
    aux = Lex::region();
    Lex::finish_line();
    comment = Lex::get_comments(comment);
    description = Lex::get_description(description);
  }
  else if ((s == "(") || (s == "{")) {
    s = UmlOperation::skip_expr(1);	// goes after opt init and body
    if (s.isEmpty()) {
      if (! Package::scanning())
	Lex::premature_eof();
      return FALSE;
    }
    // s is "," or ";"
    if (s == ";")
      Lex::unread_word(s);
    aux = Lex::region();
  }
  else {
    if (! Package::scanning())
      Lex::error_near(s);
    return FALSE;
  }
  
  if (!Package::scanning()) {
    // here aux = opt init and body + final character , ; or }
    QCString decl = JavaSettings::enumItemDecl();
    int index;
    
    if ((decl.find("${name}") == -1) ||
	((index = decl.find("${value}")) == -1)) {
      decl = "  ${name}${value},${comment}";
      index = decl.find("${value}");
    }
    
    aux.resize(aux.length()); // remove , ; or }, warning resize count \000
    if (!aux.stripWhiteSpace().isEmpty())
      decl.replace(index, 8, aux);
    
    if (!comment.isEmpty())
      item->set_Description((decl.find("${description}") != -1)
			    ? description : comment);
    
    item->set_JavaDecl(decl);
  }
    
  return TRUE;
}
