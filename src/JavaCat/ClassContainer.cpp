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

#include "ClassContainer.h"
#include "UmlClass.h"
#include "Class.h"
#include "Lex.h"

ClassContainer::~ClassContainer() {
}

// on exit t.explicit_type is non empty if
// the type is ...<...>... ie *cl and typespec.type are wrong

bool ClassContainer::read_type(UmlTypeSpec & typespec, Class ** cl, 
			       const QValueList<FormalParameterList> & tmplts,
			       QValueList<UmlTypeSpec> * actuals,
			       QCString & str_actuals, QCString s) {
  QCString path; // type without <..>
  QCString type; // real type form
  bool internal_template = FALSE;
  
  str_actuals = 0;
  if (actuals != 0)
    actuals->clear();
  
  if (s.isEmpty() && (s = Lex::read_word()).isEmpty()) {
    Lex::premature_eof();
    return FALSE;
  }
    
  for (;;) {
    internal_template = (path != type);
    
    path += s;
    type += s;
    
    s = Lex::read_word();
  
    if (s != "<")
      break;
    
    type += s;
    str_actuals = s;
    
    do {
      Lex::mark();
      
      int level = 0;
      
      for (;;) {
	s = Lex::read_word(TRUE);
	
	if (s == ",") {
	  if (level == 0)
	    break;
	}
	else if (s == ">") {
	  if (level-- == 0)
	    break;
	}
	else if (s == "]")
	  level -= 1;
	else if ((s == "<") || (s == "["))
	  level += 1;
	else if (s.isEmpty()) {
	  Lex::premature_eof();
	  return FALSE;
	}
      }
      
      QCString e = Lex::region();
      
      type += e;
      str_actuals += e;

      if (actuals != 0) {
	UmlTypeSpec t;
	
	e.resize(e.length()); // remove , or >
	e = e.stripWhiteSpace();
	if (! e.isEmpty())
	  compute_type(e, t, tmplts);
	actuals->append(t);
      }
      
    } while (s == ",");
    
    s = Lex::read_word();
    if (s.isEmpty() || (*s != '.'))
      break;
    str_actuals = 0;
    if (actuals != 0)
      actuals->clear();
  }
  
  if (! s.isEmpty())
    Lex::unread_word(s);
  
  compute_type(path, typespec, tmplts, cl);
  if (typespec.type != 0) {
    if (internal_template)
      // typespec.type stay unchanged
      typespec.explicit_type = type;
  }
  else
    // path may be not good
    typespec.explicit_type = type;
  
  return TRUE;
}

// don't produce error

bool ClassContainer::bypass_type(QCString s) {
  if (s.isEmpty() && (s = Lex::read_word()).isEmpty())
    return FALSE;
    
  for (;;) {
    s = Lex::read_word();
  
    if (s != "<")
      break;
    
    do {
      int level = 0;
      
      for (;;) {
	s = Lex::read_word(TRUE);
	
	if (s == ",") {
	  if (level == 0)
	    break;
	}
	else if (s == ">") {
	  if (level-- == 0)
	    break;
	}
	else if (s == "]")
	  level -= 1;
	else if ((s == "<") || (s == "["))
	  level += 1;
	else if (s.isEmpty())
	  return FALSE;
      }
    } while (s == ",");
    
    s = Lex::read_word();
    if (s.isEmpty() || (*s != '.'))
      break;
  }
  
  if (! s.isEmpty())
    Lex::unread_word(s);
  
  return TRUE;
}
