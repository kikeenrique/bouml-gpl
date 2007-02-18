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

#include <qtextstream.h> 

#include "UmlRelation.h"
#include "UmlClass.h"
#include "UmlSettings.h"
#include "CppSettings.h"
#include "CppRefType.h"
#include "UmlCom.h"
#include "util.h"

void UmlRelation::compute_dependency(QList<CppRefType> & dependencies,
				     const QCString & cl_stereotype,
				     bool all_in_h) {
  if (cl_stereotype == "enum")
    return;
  
  switch (relationKind()) {
  case aDependency:
    if (stereotype() == "friend")
      break;
    CppRefType::add(roleType(), dependencies, cppDecl() != "#include in source");
    break;
  case aGeneralisation:
  case aRealization:
    CppRefType::add(roleType(), dependencies, TRUE);
    break;
  default:
    QCString decl = cppDecl();
    int index;
    
    if ((index = decl.find("${static}")) != -1)
      decl.remove((unsigned) index, 9);
    if ((index = decl.find("${mutable}")) != -1)
      decl.remove((unsigned) index, 10);
    if ((index = decl.find("${volatile}")) != -1)
      decl.remove((unsigned) index, 11);
    if ((index = decl.find("${const}")) != -1)
      decl.remove((unsigned) index, 8);
    if ((index = decl.find("${multiplicity}")) != -1)
      decl.remove((unsigned) index, 15);
    if ((index = decl.find("${name}")) != -1)
      decl.remove((unsigned) index, 7);
    if ((index = decl.find("${value}")) != -1)
      decl.remove((unsigned) index, 8);
    if ((index = decl.find("${h_value}")) != -1)
      decl.remove((unsigned) index, 10);
    if ((index = decl.find("${stereotype}")) != -1)
      decl.replace((unsigned) index, 13,
		   CppSettings::relationStereotype(stereotype()));
    if ((index = decl.find("${association}")) != -1) {
      decl.replace((unsigned) index, 14,
		   association().toString());
    }
    replace_alias(decl);
    
    UmlTypeSpec type;
    
    type.type = roleType();
    UmlClassMember::compute_dependency(dependencies, decl, type, all_in_h);
  }
}

void UmlRelation::generate_inherit(const char *& sep, QTextOStream & f_h, 
				   const QValueList<UmlActualParameter> & actuals,
				   const QCString & cl_stereotype) {
  switch (relationKind()) {
  default:
    break;
  case aGeneralisation:
  case aRealization:
    if ((cl_stereotype == "union") || (cl_stereotype == "enum")) {
      write_trace_header();
      UmlCom::trace(QCString("<tt>        </tt><font face=helvetica><b>an <i>")
		    + cl_stereotype + "</i> cannot inherits</b></font><br>");
      return;
    }
    else if (cl_stereotype == "typedef") {
      write_trace_header();
      UmlCom::trace("<tt>        </tt><font face=helvetica><b>a <i>typedef</i> cannot inherits</b></font><br>");
      return;
    }
    
    UmlClass * role_type = roleType();
    const QCString & other_stereotype = role_type->stereotype();
    
    if ((other_stereotype == "union") ||
	(other_stereotype == "enum")) {
      write_trace_header();
      UmlCom::trace(QCString("<tt>        </tt><font face=helvetica><b>cannot inherits an <i>")
		    + other_stereotype + "</i></b></font><br>");
      return;
    }
    
    f_h << sep;
    sep = ", ";
    
    if (cppVirtualInheritance())
      f_h << "virtual ";
    
    aVisibility visibility = (cppVisibility() == DefaultVisibility)
      ? this->visibility() : cppVisibility();
    
    switch (visibility) {
    case PublicVisibility:
    case PackageVisibility:
      f_h << "public ";
      break;
    case ProtectedVisibility:
      f_h << "protected ";
      break;
    default:
      f_h  << "private ";
    }
    
    const char * p = cppDecl();
    
    while (*p) {
      if (!strncmp(p, "${type}", 7)) {
	role_type->write(f_h, FALSE);
	p += 7;
    
	if (!actuals.isEmpty()) {
	  QValueList<UmlActualParameter>::ConstIterator ita;
	  bool need_space = FALSE;
	  bool used = FALSE;
	  
	  for (ita = actuals.begin(); ita != actuals.end(); ++ita) {
	    if ((*ita).superClass() == role_type) {
	      used = TRUE;
	      UmlClass * cl = (*ita).value().type;
	      
	      if (cl != 0)
		need_space = !cl->formals().isEmpty();
	      else {
		QCString s = (*ita).value().explicit_type;
		
		need_space = (!s.isEmpty() && (s.at(s.length() - 1) == '>'));
	      }
	      (*ita).generate(f_h);
	    }
	  }
	  
	  if (used) {
	    if (need_space)
	      f_h << " >";
	    else
	      f_h << ">";
	  }
	}
      }
      else
	f_h << *p++;
    }
  }
}

void UmlRelation::generate_decl(aVisibility & current_visibility, QTextOStream & f_h,
				const QCString & cl_stereotype, QCString indent,
				bool & first, bool) {
  switch (relationKind()) {
  case aDependency:
    if (stereotype() == "friend") {
      QValueList<UmlFormalParameter> formals = roleType()->formals();
      
      if (! formals.isEmpty()) {
	const char * sep = "  template <";
	QValueList<UmlFormalParameter>::Iterator it;
	
	for (it = formals.begin(); it != formals.end(); ++it) {
	  f_h << sep << (*it).type();
	  sep = ", ";
	}
	
	f_h << "> ";
      }
      else
	f_h << "  ";
      
      f_h << "friend " << roleType()->cpp_stereotype()
	<< " " << roleType()->name() << ";\n";
      first = FALSE;
    }
    break;
  case aGeneralisation:
  case aRealization:
    break;
  default:
    if (!cppDecl().isEmpty()) {
      if (cl_stereotype == "enum") {
	write_trace_header();
	UmlCom::trace("<tt>        </tt><font face=helvetica><b>an <i>enum</i> cannot have relation</b></font><br>");
	return;
      }
      if (cl_stereotype == "typedef") {
	write_trace_header();
	UmlCom::trace("<tt>        </tt><font face=helvetica><b>a <i>typedef</i> cannot have relation</b></font><br>");
	return;
      }
      generate_visibility(current_visibility, f_h, first, indent);
      first = FALSE;
    
      const char * p = cppDecl();
      const char * pp = 0;
      QCString s;
      
      while ((*p == ' ') || (*p == '\t'))
	indent += *p++;
      
      if (*p != '#')
	f_h << indent;
      
      for (;;) {
	if (*p == 0) {
	  if (pp == 0)
	    break;
	  
	  // comment management done
	  p = pp;
	  pp = 0;
	  if (*p == 0)
	    break;
	  if (*p != '#')
	    f_h << indent;
	}

	if (*p == '\n') {
	  f_h << *p++;
	  if (*p && (*p != '#'))
	    f_h << indent;
	}
	else if (*p == '@')
	  manage_alias(p, f_h);
	else if (*p != '$')
	  f_h << *p++;
	else if (!strncmp(p, "${comment}", 10))
	  manage_comment(p, pp);
	else if (!strncmp(p, "${description}", 14))
	  manage_description(p, pp);
	else if (!strncmp(p, "${static}", 9)) {
	  p += 9;
	  if (isClassMember())
	    f_h << "static ";
	}
	else if (!strncmp(p, "${const}", 8)) {
	  p += 8;
	  if (isReadOnly())
	    f_h << "const ";
	}
	else if (!strncmp(p, "${volatile}", 11)) {
	  p += 11;
	  if (isVolatile())
	    f_h << "volatile ";
	}
	else if (!strncmp(p, "${mutable}", 10)) {
	  p += 10;
	  if (isCppMutable())
	    f_h << "mutable ";
	}
	else if (!strncmp(p, "${type}", 7)) {
	  p += 7;
	  roleType()->write(f_h);
	}
	else if (!strncmp(p, "${name}", 7)) {
	  p += 7;
	  f_h << roleName();
	}
	else if (!strncmp(p, "${multiplicity}", 15)) {
	  p += 15;
	  
	  if (*((const char *) multiplicity()) == '[')
	    f_h << multiplicity();
	  else
	    f_h << '[' << multiplicity() << ']';
	}
	else if (!strncmp(p, "${stereotype}", 13)) {
	  p += 13;
	  f_h << CppSettings::relationStereotype(stereotype());
	}
	else if (!strncmp(p, "${value}", 8))
	  p += 8;
	else if (!strncmp(p, "${h_value}", 10)) {
	  p += 10;
	  if (!defaultValue().isEmpty() && isClassMember()) {
	    if (*((const char *) defaultValue()) == '=')
	      f_h << ' ' << defaultValue();
	    else
	      f_h << defaultValue();
	  }
	}
	else if (!strncmp(p, "${association}", 14)) {
	  p += 14;
	  UmlClass::write(f_h, association());
	}
	else
	  // strange
	  f_h << *p++;
      }
    
      f_h << '\n';
    }
  }
}

void UmlRelation::generate_def(QTextOStream & f, QCString indent, bool h,
			       QCString templates, QCString cl_names,
			       QCString, QCString) {
  if (isClassMember() && !cppDecl().isEmpty()) {
    UmlClass * cl = (UmlClass *) parent();
    
    if ((!templates.isEmpty() || (cl->name().find('<') != -1))
	? h : !h) {
      const char * p = cppDecl();
      const char * pp = 0;
      
      while ((*p == ' ') || (*p == '\t'))
	p += 1;
      
      if (! templates.isEmpty())
	insert_template(p, f, indent, templates);
      
      if (*p != '#')
	f << indent;
      
      const char * pname = name_spec(p);
      
      for (;;) {
	if (*p == 0) {
	  if (pp == 0)
	    break;
	  
	  // comment management done
	  p = pp;
	  pp = 0;
	  if (*p == 0)
	    break;
	  if (*p != '#')
	    f << indent;
	}

	if (*p == '\n') {
	  f << *p++;
	  if (*p && (*p != '#'))
	    f << indent;
	}
	else if (*p == '@')
	  manage_alias(p, f);
	else if (*p != '$') {
	  if (p == pname)
	    f << cl_names << "::";
	  f << *p++;
	}
	else if (!strncmp(p, "${comment}", 10))
	  manage_comment(p, pp);
	else if (!strncmp(p, "${description}", 14))
	  manage_description(p, pp);
	else if (!strncmp(p, "${static}", 9)) {
	  p += 9;
	}
	else if (!strncmp(p, "${const}", 8)) {
	  p += 8;
	  if (isReadOnly())
	    f << "const ";
	}
	else if (!strncmp(p, "${volatile}", 11)) {
	  p += 11;
	  if (isVolatile())
	    f << "volatile ";
	}
	else if (!strncmp(p, "${mutable}", 10)) {
	  p += 10;
	}
	else if (!strncmp(p, "${type}", 7)) {
	  p += 7;
	  roleType()->write(f);
	}
	else if (!strncmp(p, "${name}", 7)) {
	  p += 7;
	  if (*pname == '$')
	    f << cl_names << "::";
	  f << roleName();
	}
	else if (!strncmp(p, "${multiplicity}", 15)) {
	  p += 15;
	  
	  if (*((const char *) multiplicity()) == '[')
	    f << multiplicity();
	  else
	    f << '[' << multiplicity() << ']';
	}
	else if (!strncmp(p, "${stereotype}", 13)) {
	  p += 13;
	  f << CppSettings::relationStereotype(stereotype());
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
	else if (!strncmp(p, "${h_value}", 10))
	  p += 10;
	else if (!strncmp(p, "${association}", 14)) {
	  p += 14;
	  UmlClass::write(f, association());
	}
	else
	  // strange
	  f << *p++;
      }
      
      f << '\n';
    }
  }
}

