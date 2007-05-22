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

#ifdef DEBUG_BOUML
#include <iostream>

using namespace std;
#endif

#include "UmlAttribute.h"
#include "UmlPackage.h"
#include "Class.h"
#include "UmlClass.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "Lex.h"
#include "Statistic.h"

bool UmlAttribute::new_one(Class * cl, const QCString & name,
			   const QCString & type, const QCString & modifier,
			   const QCString & pretype, const QCString & array,
			   aVisibility visibility, bool staticp, bool constp,
			   bool typenamep, bool mutablep, bool volatilep,
			   const QCString & bitfield, const QCString & value,
			   QCString comment, QCString description)
{
#ifdef DEBUG_BOUML
  cout << "ATTRIBUTE '" << name << "' type '" << type << "' modifier '" << modifier << "' array '" << array << "'\n";
#endif
  
#ifndef REVERSE
  if (visibility == PrivateVisibility) {
    Lex::finish_line();
    Lex::clear_comments();
    return TRUE;
  }
#endif
  
  UmlAttribute * at = UmlBaseAttribute::create(cl->get_uml(), name);
  
  if (at == 0) {
    UmlCom::trace(QCString("<font face=helvetica><b>cannot add attribute <i>")
		  + name + "</i> in <i>" + QCString(cl->text(0))
		  + "</i></b></font><br><hr>");  
    return FALSE;
  }
  
  Statistic::one_attribute_more();
  
  Lex::finish_line();
  
  comment = Lex::get_comments();
  description = Lex::get_description();
  
  bool pfunc = (type.find('$') != -1);
  UmlTypeSpec typespec;
  QCString typeform;
  
  if (! pfunc) {
    typeform = (pretype.isEmpty())
      ? QCString("${type}")
      : pretype + " ${type}";
    
    cl->compute_type(type, typespec, typeform);
  }
  else {
    typespec.explicit_type = type.simplifyWhiteSpace();
    typespec.explicit_type.remove(typespec.explicit_type.find("${name}"), 7);
  }
  at->set_Type(typespec);
  at->set_Visibility(visibility);
  if (constp) at->set_isReadOnly(TRUE);
  if (staticp) at->set_isClassMember(TRUE);
  if (volatilep) at->set_isVolatile(TRUE);
  if (mutablep) at->set_isCppMutable(TRUE);
  
  QCString decl = CppSettings::attributeDecl();
  int index = decl.find("${type}");
  
  if ((index == -1) ||
      (decl.find("${const}") == -1) ||
      (decl.find("${name}") == -1) ||
      (decl.find("${mutable}") == -1) ||
      (decl.find("${volatile}") == -1) ||
      (decl.find(';') == -1)) {
    decl = "    ${comment}${static}${mutable}${volatile}${const}${type} ${name}${value};";
    index = decl.find("${type}");
  }
  
  if (!comment.isEmpty())
    at->set_Description((decl.find("${description}") != -1)
			? description : comment);
  
  if (pfunc)
    decl.replace(index, decl.find("${name}") + 7 - index, type);
  else {
    if (!modifier.isEmpty())
      decl.insert(index + 7, QCString(" ") + modifier);
    
    decl.replace(index, 7, typeform);
    
    if (!array.isEmpty())
      decl.insert(decl.find("${name}") + 7, array);

    if (!bitfield.isEmpty())
      decl.insert(decl.find(';'), QCString(" : ") + bitfield);
  }
  
  if (typenamep) {
    int index = decl.find("${const}") + 8; // find cannot return -1
    int index2 = decl.find("${mutable}") + 10; // find cannot return -1
    int index3 = decl.find("${volatile}") + 11; // find cannot return -1
    
    if (index2 > index) index = index2;
    if (index3 > index) index = index3;
    
    decl.insert(index, "typename ");
  }
  
  if (! value.isEmpty()) {
    if ((index = decl.find("${value}")) != -1)
      decl.insert(index + 2,  "h_");
    at->set_DefaultValue(value);
  }
  
  at->set_CppDecl(decl);
  
  return TRUE;
}
