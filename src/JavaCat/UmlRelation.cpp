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

#include "UmlRelation.h"
#include "UmlPackage.h"
#include "JavaSettings.h"
#include "UmlClass.h"
#include "Class.h"
#include "JavaCatWindow.h"
#include "Lex.h"
#ifdef REVERSE
#include "Statistic.h"
#endif

bool UmlRelation::new_one(Class * container, const QCString & name,
			  UmlTypeSpec & dest, QCString str_actuals,
			  aVisibility visibility, bool staticp,
			  bool constp, bool transientp, bool volatilep,
			  const QCString & array, const QCString & value,
			  QCString comment, QCString description,
			  QCString annotation)
{
#ifdef TRACE
  cout << "RELATION '" << name << "' from '" << cl->Name() << "' to '" << dest.type->Name()
    << "' array '" << array << "'\n";
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
  UmlRelation * rel = 
    UmlBaseRelation::create(aDirectionalAssociation, cl, dest.type);
  
  if (rel == 0) {
    JavaCatWindow::trace(QCString("<font face=helvetica><b>cannot add relation <i>")
			 + name + "</i> in <i>" + cl->name() + "</i> to <i>"
			 + dest.type->name() + "</i></b></font><br>");  
    return FALSE;
  }
    
#ifdef REVERSE
  Statistic::one_relation_more();
#endif
  
  rel->set_Visibility(visibility);
  
  Lex::finish_line();
  
  comment = Lex::get_comments(comment);
  description = Lex::get_description(description);
  
  QCString decl = JavaSettings::relationDecl(array);
  
  if (!comment.isEmpty())
    rel->set_Description((decl.find("${description}") != -1)
			 ? description : comment);
  
  if (constp)
    rel->set_isReadOnly(TRUE);
  
  if (transientp)
    rel->set_isJavaTransient(TRUE);
  
  if (volatilep)
    rel->set_isVolatile(TRUE);
  
  if (staticp)
    rel->set_isClassMember(TRUE);
  
  if (!array.isEmpty())
    rel->set_Multiplicity(array);
  
  if (! value.isEmpty())
    rel->set_DefaultValue(value);
  
  if (! annotation.isEmpty())
    rel->set_JavaAnnotations(annotation);
  
  UmlClass::manage_generic(decl, dest, str_actuals, "${type}");  
  rel->set_JavaDecl(decl);

  return rel->set_RoleName(name);
}
