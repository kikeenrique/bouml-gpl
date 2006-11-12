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

#include <qtextstream.h> 

#include "UmlAttribute.h"
#include "UmlSettings.h"
#include "JavaSettings.h"
#include "UmlClass.h"
#include "UmlCom.h"

void UmlAttribute::generate(QTextOStream & f, const QCString & st, QCString indent) {
  if (!javaDecl().isEmpty()) {
    const char * p = javaDecl();
    const char * pp = 0;
    
    while ((*p == ' ') || (*p == '\t'))
      indent += *p++;

    f << indent;
    
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;
	if (*p == 0)
	  break;
	f << indent;
      }
      
      if (*p == '\n') {
	f << *p++;
	if (*p)
	  f << indent;
      }
      else if (*p == '@')
	manage_alias(p, f);
      else if (*p != '$')
	f << *p++;
      else if (!strncmp(p, "${comment}", 10))
	manage_comment(p, pp);
      else if (!strncmp(p, "${description}", 14))
	manage_description(p, pp);
      else if (!strncmp(p, "${visibility}", 13)) {
	p += 13;
	generate_visibility(f);
      }
      else if (!strncmp(p, "${static}", 9)) {
	p += 9;
	if (isClassMember())
	  f << "static ";
      }
      else if (!strncmp(p, "${final}", 8)) {
	p += 8;
	if (isReadOnly())
	  f << "final ";
      }
      else if (!strncmp(p, "${transient}", 12)) {
	p += 12;
	if (isJavaTransient())
	  f << "transient ";
      }
      else if (!strncmp(p, "${volatile}", 11)) {
	p += 11;
	if (isVolatile())
	  f << "volatile ";
      }
      else if (!strncmp(p, "${type}", 7)) {
	p += 7;
	UmlClass::write(f, type());
      }
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	f << name();
      }
      else if (!strncmp(p, "${value}", 8)) {
	p += 8;
	if (!defaultValue().isEmpty()) {
	  if (*((const char *) defaultValue()) == '=')
	    f << ' ' << defaultValue();
	  else
	    f << defaultValue();
	}
      }
      else if (!strncmp(p, "${@}", 4)) {
	p += 4;
	if (pp != 0)
	  f << "${@}";
	else if (! javaAnnotations().isEmpty()) {
	  pp = p;
	  p = javaAnnotations();
	}
      }
      else
	// strange
	f << *p++;
    }
      
    if (st != "enum")
      f << '\n';
  }
}

void UmlAttribute::generate_enum_pattern_item(QTextOStream & f, int & current_value,
					       const QCString & class_name, QCString indent) {
  if (!javaDecl().isEmpty()) {
    const char * p = javaDecl();
    const char * pp = 0;
    
    while ((*p == ' ') || (*p == '\t'))
      indent += *p++;

    f << indent;
    
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;
	if (*p == 0)
	  break;
	f << indent;
      }
      
      if (!strncmp(p, "${comment}", 10))
	manage_comment(p, pp);
      else if (!strncmp(p, "${description}", 14))
	manage_description(p, pp);
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	f << name();
      }
      else if (!strncmp(p, "${class}", 8)) {
	p += 8;
	f << class_name;
      }
      else if (!strncmp(p, "${value}", 8)) {
	p += 8;
	if (!defaultValue().isEmpty()) {
	  QCString d = defaultValue().simplifyWhiteSpace();
	  
	  if (*((const char *) d) == '=')
	    d = d.mid(1).simplifyWhiteSpace();
	  
	  bool ok;
	  int v;
	  
	  v = d.toInt(&ok);
	  if (! ok) 
	    UmlCom::trace(QCString("<tt>        </tt><font face=helvetica><b><i>") +
			  defaultValue() + "</i> wrong value for <i> "
			  + name() + "</i></b></font><br>");
	  else
	    current_value = v;
	}
	f << " = " << current_value++;
      }
      else if (*p == '\n') {
	f << *p++;
	if (*p)
	  f << indent;
      }
      else if (!strncmp(p, "${@}", 4)) {
	p += 4;
	if (pp != 0)
	  f << "${@}";
	else if (! javaAnnotations().isEmpty()) {
	  pp = p;
	  p = javaAnnotations();
	}
      }
      else if (*p == '@')
	manage_alias(p, f);
      else
	f << *p++;
    }
  }
}

void UmlAttribute::generate_enum_pattern_case(QTextOStream & f, QCString indent) {
  if (!javaDecl().isEmpty()) {
    const char * p = JavaSettings::enumPatternItemCase();
    const char * pp = 0;
    
    while ((*p == ' ') || (*p == '\t'))
      indent += *p++;

    f << indent;
    
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;
	if (*p == 0)
	  break;
	f << indent;
      }
      
      if (!strncmp(p, "${comment}", 10))
	manage_comment(p, pp);
      else if (!strncmp(p, "${description}", 14))
	manage_description(p, pp);
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	f << name();
      }
      else if (*p == '\n') {
	f << *p++;
	if (*p)
	  f << indent;
      }
      else if (*p == '@')
	manage_alias(p, f);
      else
	f << *p++;
    }
  }
}

void UmlAttribute::generate_enum_item(QTextOStream & f, QCString indent,
				      bool & first) {
  if (stereotype() != "attribute") {
    if (first)
      first = FALSE;
    else
      f << '\n';
    
    generate(f, "enum", indent);
  }
}

void UmlAttribute::generate_enum_member(QTextOStream & f, QCString indent) {
  if (stereotype() == "attribute")
    generate(f, "enum", indent);
}

