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

#ifdef DEBUG_BOUML
#include <iostream>

using namespace std;
#endif

#include "UmlRelation.h"
#include "UmlPackage.h"
#include "CppSettings.h"
#include "UmlClass.h"
#include "UmlCom.h"
#include "Lex.h"
#include "Statistic.h"

bool UmlRelation::new_one(UmlClass * cl, const QCString & name,
			  UmlClass * dest, const QCString & modifier, 
			  const QCString & pretype, const QCString & array, 
			  const QCString & typeform, aVisibility visibility,
			  bool staticp, bool constp, bool mutablep, bool volatilep, 
			  const QCString & value, QCString comment,
			  QCString description)
{
#ifdef DEBUG_BOUML
  cout << "RELATION '" << name << "' from '" << cl->name() << "' to '" << dest->name()
    << "' modifier '" << modifier << "' array '" << array
      << "' typeform '" << typeform << "'\n";
#endif
  
#ifndef REVERSE
  if (visibility == PrivateVisibility) {
    Lex::finish_line();
    Lex::clear_comments();
    return TRUE;
  }
#endif
  
  UmlRelation * rel = 
    UmlBaseRelation::create((modifier.isEmpty()) ? aDirectionalAggregationByValue
						 : aDirectionalAssociation,
			    cl, dest);
  
  if (rel == 0) {
    UmlCom::trace(QCString("<font face=helvetica><b>cannot add relation <i>")
		  + name + "</i> in <i>" + cl->name() + "</i> to <i>"
		  + dest->name() + "</i></b></font><br><hr>");  
    return FALSE;
  }
  
  Statistic::one_relation_more();
  
  Lex::finish_line();
  
  comment = Lex::get_comments();
  description = Lex::get_description();
  
  rel->set_Visibility(visibility);
  if (constp) rel->set_isReadOnly(TRUE);
  if (staticp) rel->set_isClassMember(TRUE);
  if (mutablep) rel->set_isCppMutable(TRUE);
  if (volatilep) rel->set_isVolatile(TRUE);
  
  QCString decl;
  
  if (typeform != "${type}") {
    // array & modified are empty, pretype is empty ?
    decl = CppSettings::relationDecl(TRUE, "*");
    
    int index = typeform.find("<");	// cannot be -1
    QCString st = typeform.left(index);
    QCString st_uml = CppSettings::umlType(st);

    rel->set_Stereotype((st_uml.isEmpty()) ? st : st_uml);
    
    int index2;
    
    if ((index2 = decl.find("<${type}>")) == -1) {
      decl = "    ${comment}${static}${mutable}${volatile}${const}${stereotype}<${type}> ${name}${value};";
      index2 = decl.find("<${type}>");
    }
    decl.replace(index2, 9, typeform.mid(index));
  }
  else {
    if (!array.isEmpty())
      rel->set_Multiplicity(array);
    decl = CppSettings::relationDecl(modifier.isEmpty(), array);
    
    int index;
    
    if (!pretype.isEmpty() &&
	((index = decl.find("${type}")) != 0))
      decl.insert(index, pretype + " ");
  }
  
  if (! value.isEmpty()) {
    int index = decl.find("${value}");
    
    if (index != -1)
      decl.insert(index + 2,  "h_");
    rel->set_DefaultValue(value);
  }
  
  rel->set_CppDecl(decl);
  
  if (!comment.isEmpty())
    rel->set_Description((decl.find("${description") != -1)
			 ? description : comment);
  
  return rel->set_RoleName(name);
}

bool UmlRelation::new_friend(UmlClass * from, UmlClass * to)
{
#ifdef DEBUG_BOUML
  cout << "FRIEND from '" << from->name() << "' to '" << to->name() << "'\n";
#endif
  
  UmlRelation * rel = UmlBaseRelation::create(aDependency, from, to);
  
  if (rel == 0) {
    UmlCom::trace(QCString("<font face=helvetica><b>cannot add friend relation in <i>")
		  + from->name() + "</i> to <i>"
		  + to->name() + "</i></b></font><br><hr><br>");  
    return FALSE;
  }
  
  Statistic::one_relation_more();
  
  rel->set_CppDecl("Generated");
  
  return rel->set_Stereotype("friend");
}
