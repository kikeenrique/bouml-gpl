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

#include "UmlOperation.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "util.h"

void UmlOperation::add_param(int rank, aDirection dir,
			     const char * name, const char * type) {
  UmlParameter p;
  
  p.dir = dir;
  p.type.explicit_type = type;
  p.name = name;
  
  if (!addParameter(rank, p)) {
    QCString msg = QCString("can't add parameter '") + name +
      "' to " + parent()->name() + "::" + this->name();

    UmlCom::trace(msg);
    throw 0;
  }
}

void UmlOperation::add_param(int rank, aDirection dir,
			     const char * name, UmlClass * type) {
  UmlParameter p;
  
  p.dir = dir;
  p.type.type = type;
  p.name = name;
  
  if (!addParameter(rank, p)) {
    QCString msg = QCString("can't add parameter '") + name +
      "' to " + parent()->name() + "::" + this->name();

    UmlCom::trace(msg);
    throw 0;
  }
}

void UmlOperation::set_cpp(const char * return_form_or_inherit,
			   const char * params, const char * body,
			   bool inlinep, const char * if_def,
			   const char * end_if) {
  if (*return_form_or_inherit == ':') {
    // inherit
    if (inlinep) {
      QCString s = CppSettings::operationDecl();
      int index = s.find("${)}");
      
      s.resize(index + 5);
      s.insert(index, params);
      s.append(" ");
      s.append(return_form_or_inherit);
      if (*body) {
	s.append(" {\n  ");
	s.append(body);
	s.append("}\n");
      }
      else
	s.append(" {\n}\n");
      conditional(s, if_def, end_if);
      set_CppDecl(s);
      
      set_CppDef("");
    }
    else {
      QCString s = CppSettings::operationDecl();
      int index = s.find("${)}");
      
      s.resize(index + 5);
      s.insert(index, params);
      s.append(";");
      conditional(s, if_def, end_if);
      set_CppDecl(s);
      
      s = CppSettings::operationDef();
      index = s.find("${)}");
      s.resize(index + 5);
      s.insert(index, params);
      s.append(" ");
      s.append(return_form_or_inherit);
      if (*body) {
	s.append(" {\n  ");
	s.append(body);
	s.append("}\n");
      }
      else
	s.append(" {\n}\n");
      conditional(s, if_def, end_if);
      set_CppDef(s);
    }
  }
  else {
    // return
    if (inlinep) {
      QCString s = CppSettings::operationDecl();
      int index = s.find("${type}");
      
      s.replace(index, 7, return_form_or_inherit);
      s.insert(s.find("${)}", index), params);
      s.resize(s.findRev(";"));
      if (*body) {
	s.append(" {\n  ");
	s.append(body);
	s.append("}\n");
      }
      else
	s.append(" {\n}\n");
      conditional(s, if_def, end_if);
      set_CppDecl(s);
      
      set_CppDef("");
    }
    else {
      QCString s = CppSettings::operationDecl();
      int index = s.find("${type}");
      
      s.replace(index, 7, return_form_or_inherit);
      s.insert(s.find("${)}", index), params);
      conditional(s, if_def, end_if);
      set_CppDecl(s);
      
      s = CppSettings::operationDef();
      index = s.find("${type}");
      s.replace(index, 7, return_form_or_inherit);
      s.insert(s.find("${)}", index), params);
      conditional(s, if_def, end_if);
      set_CppDef(s);
      
      set_CppBody(body);
    }
  }
}


void UmlOperation::set_java(const char * return_form,
			    const char * params, const char * body,
			    bool inlinep) {
  QCString s = JavaSettings::operationDef();
  int index = s.find("${type}");
  
  s.replace(index, 7, return_form);
  s.insert(s.find("${)}", index), params);
  
  if (inlinep) {
    s.replace(s.findRev("${body}"), 7, body);
    set_JavaDef(s);
  }
  else {
    set_JavaDef(s);
    set_JavaBody(body);
  }
}

// rename call to operation of java enum settings
// rename cmd enum item usage
// do the rename in C++ decl, def and body, in Java def and body

static bool is_sep(char c)
{
  return !((c == '_') ||
	   ((c >= 'a') && (c <= 'z')) ||
	   ((c >= 'A') && (c <= 'Z')) ||
	   ((c >= '0') && (c <= '9')));
}

static bool rename(QCString & s, bool java, bool javasettings)
{
  static const struct {
    const char * o;
    const char * n;
    bool java_only;
  } T[] = {
    { "enumDecl", "enumPatternDecl", FALSE },
    { "set_EnumDecl", "set_EnumPatternDecl", FALSE },
    { "enumItemDecl", "enumPatternItemDecl", FALSE },
    { "set_EnumItemDecl", "set_EnumPatternItemDecl", FALSE },
    { "enumItemCase", "enumPatternItemCase", FALSE },
    { "set_EnumItemCase", "set_EnumPatternItemCase", FALSE },
    
    { "setJavaEnumDeclCmd", "setJavaEnumPatternDeclCmd", FALSE },
    { "setJavaEnumItemDeclCmd", "setJavaEnumPatternItemDeclCmd", FALSE },
    { "setJavaEnumItemCaseCmd", "setJavaEnumPatternItemCaseCmd", FALSE },
    
    { "_setJavaEnumDeclCmd", "_setJavaEnumPatternDeclCmd", TRUE },
    { "_setJavaEnumItemDeclCmd", "_setJavaEnumPatternItemDeclCmd", TRUE },
    { "_setJavaEnumItemCaseCmd", "_setJavaEnumPatternItemCaseCmd", TRUE },
    
    { "_enum_decl", "_enum_pattern_decl", FALSE },
    { "_enum_item_decl", "_enum_pattern_item_decl", FALSE },
    { "_enum_item_case", "_enum_pattern_item_case", FALSE }
  };
  int t_index;
  bool changed = FALSE;
  
  for (t_index = 0; t_index != sizeof(T)/sizeof(T[0]); t_index += 1) {
    if (!T[t_index].java_only || java) {
      QCString o = T[t_index].o;
      
      if (!javasettings)
	o = ((java) ? "JavaSettings." : "JavaSettings::") + o;
      
      int o_len = o.length();
      QCString n = T[t_index].n;
      
      if (!javasettings)
	n = ((java) ? "JavaSettings." : "JavaSettings::") + n;
      
      int n_len = n.length();
      int index = 0;
      
      while ((index = s.find(o, index)) != -1) {
	if (((index == 0) || is_sep(s[index - 1])) &&
	    is_sep(s[index + o_len])) {
	  s.replace(index, o_len, n);
	  index += n_len;
	  changed = TRUE;
	}
	else
	  index += 1;
      }
    }
  }

  return changed;
}

void UmlOperation::rename_jdk5() {
  QCString s;
  bool changed = FALSE;
  bool javasettings = (parent()->name() == "JavaSettings");
  
  s = cppDecl();
  if (rename(s, FALSE, javasettings)) {
    changed = TRUE;
    set_CppDecl(s);
  }
  
  s = cppDef();
  if (rename(s, FALSE, javasettings)) {
    changed = TRUE;
    set_CppDef(s);
  }
  
  s = cppBody();
  if (rename(s, FALSE, javasettings)) {
    changed = TRUE;
    set_CppBody(s);
  }
  
  s = javaDef();
  if (rename(s, TRUE, javasettings)) {
    changed = TRUE;
    set_JavaDef(s);
  }
  
  s = javaBody();
  if (rename(s, TRUE, javasettings)) {
    changed = TRUE;
    set_JavaBody(s);
  }
  
  if (changed)
    UmlCom::trace(parent()->name() + "::" + name() +
		  " upgraded to take into account the rename of 'enum' to 'enum pattern'");
  
}
