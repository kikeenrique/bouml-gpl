// *************************************************************************
//
// Copyleft 2004-2008 Bruno PAGES  .
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

#include <stdio.h>// debug

#include <qtextstream.h> 

#include "UmlClass.h"
#include "UmlPackage.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlArtifact.h"
#include "UmlSettings.h"
#include "PythonSettings.h"
#include "UmlCom.h"
#include "util.h"

QCString UmlClass::python_stereotype()
{
  QCString s = PythonSettings::classStereotype(stereotype());
  
  return ((s == "ignored") || (s == "enum"))
    ? s : QCString("class");
}

void UmlClass::generate_imports(QTextOStream & f, QCString & made) {
  if (! pythonDecl().isEmpty()) {
    QVector<UmlItem> ch = children();
    unsigned index;
    const unsigned sup = ch.size();
    
    for (index = 0; index != sup; index += 1) {
      UmlClassMember * m = dynamic_cast<UmlClassMember *>(ch[index]);
      
      if (m != 0)
	m->generate_imports(f, made);
    }
  }
}

void UmlClass::generate() {
  if (! managed) {
    managed = TRUE;
    
    if (!isPythonExternal() && !pythonDecl().isEmpty()) {
      if (associatedArtifact() != 0)
	associatedArtifact()->generate();
      else if ((children().size() != 0) && verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica><i> ") + name() +
		      " : </i> does not have associated <i>artifact</i></font><br>");
    }
  }
}

void UmlClass::generate(QTextOStream & f, QCString indent,
			bool & indent_needed) {
  const QCString & stereotype = python_stereotype();
  
  if (stereotype == "ignored")
    return;
  
  const char * p = pythonDecl();
  
  if ((p == 0) || (*p == 0))
    return;
  
  QVector<UmlItem> ch = children();
  const unsigned sup = ch.size();
  int enum_item_rank = 0;
  unsigned index;
  const char * pp = 0;
  QCString saved_indent = indent;
  QCString indent_step = PythonSettings::indentStep();
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      if (*p == 0)
	break;
      pp = 0;
      indent = saved_indent;
    }

    if (*p == '@')
      manage_alias(p, f, indent, indent_needed);
    else if (*p != '$') {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      switch (*p) {
      case ':':
	if (pp == 0) {
	  indent += indent_step;
	  saved_indent = indent;
	  indent_step = "";
	}
	break;
      case '\n':
	indent_needed = TRUE;
	break;
      }
      f << *p++;
    }
    else if (!strncmp(p, "${comment}", 10))
      manage_comment(p, pp);
    else if (!strncmp(p, "${description}", 14))
      manage_description(p, pp);
    else if (!strncmp(p, "${docstring}", 12))
      manage_docstring(p, pp, indent_needed, indent, saved_indent);
    else if (! strncmp(p, "${name}", 7)) {
      p += 7;
      f << name();
    }
    else if (! strncmp(p, "${inherit}", 10)) {
      p += 10;
      
      const char * sep = "(";
      
      for (index = 0; index != sup; index += 1) {
	UmlRelation * r = dynamic_cast<UmlRelation *>(ch[index]);
	
	if ((r != 0) && !r->pythonDecl().isEmpty())
	  r->generate_inherit(sep, f);
      }

      if (*sep != '(')
	f << ")";
      else if (PythonSettings::isPython_2_2())
	f << "(object)";
    }
    else if (! strncmp(p, "${members}", 10)) {
      p += 10;
  
      bool has__init__ = FALSE;
      QCString noself;
      
      for (index = 0; index != sup; index += 1) {
	UmlClassItem * it = dynamic_cast<UmlClassItem *>(ch[index]);
	
	if (it != 0) {
	  it->generate(f, stereotype, indent, indent_needed, enum_item_rank, noself);
	  
	  if ((it->kind() == anOperation) &&
	      !((UmlOperation *) it)->isClassMember() &&
	      (it->name() == "__init__"))
	    has__init__ = TRUE;
	}
      }
      
      if (! has__init__) {
	if (indent_needed)
	  f << indent;
	f << "def __init__(self):\n";
	indent_needed = TRUE;
	QCString ind = indent;
	
	indent += PythonSettings::indentStep();
	generate_instance_att_rel(f, indent, indent_needed, "self.");
	if (indent_needed)
	  f << indent;
	f << "pass\n";
	indent_needed = TRUE;
	indent = ind;
      }
    }
    else {
      // strange
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      f << *p++;
    }
  }
}

void UmlClass::generate(QTextOStream & f, const QCString &,
			QCString indent, bool & indent_needed,
			int &, const QCString &) {
  generate(f, indent, indent_needed);
}

void UmlClass::generate_instance_att_rel(QTextOStream & f, QCString indent,
					 bool & indent_needed, QCString self) {
  const QCString & stereotype = python_stereotype();
  int enum_item_rank = 0;
  
  QVector<UmlItem> ch = children();
  const unsigned sup = ch.size();
  unsigned index;
  
  for (index = 0; index != sup; index += 1) {
    switch (ch[index]->kind()) {
    case anAttribute:
    case aRelation:
       dynamic_cast<UmlClassItem *>(ch[index])
	 ->generate(f, stereotype, indent, indent_needed, enum_item_rank, self);
      break;
    default:
      break;
    }
  }
}

void UmlClass::write(QTextOStream & f, const UmlTypeSpec & t)
{
  if (t.type != 0)
    t.type->write(f);
  else
    f << t.explicit_type;
}

void UmlClass::write(QTextOStream & f) {
  if (isPythonExternal()) {
    QCString s = pythonDecl().stripWhiteSpace();
    int index = s.find('\n');
    
    s = (index == -1) ? s.stripWhiteSpace()
		      : s.left(index).stripWhiteSpace();
      
    if ((index = s.find("${name}")) != -1)
      s.replace(index, 7, name());
    else if ((index = s.find("${Name}")) != -1)
      s.replace(index, 7, capitalize(name()));
    else if ((index = s.find("${NAME}")) != -1)
      s.replace(index, 7, name().upper());
    
    f << s;
  }
  else {
    UmlArtifact * cl_art = associatedArtifact();
    UmlArtifact * gen_art = UmlArtifact::generated_one();
    
    if ((cl_art != gen_art) && (cl_art != 0)) {
      QCString cl_pack =
	((UmlPackage *) cl_art->parent()->parent())->pythonPackage();
      
      if (!cl_pack.isEmpty() && 
	  (cl_pack != ((UmlPackage *) gen_art->parent()->parent())->pythonPackage())) {
	QCString s = "from " + cl_pack + "." + cl_art->name() + " import ";
	const QCString & imports = UmlArtifact::all_imports();
	QCString importit = s + name();
	QCString importstar = s + "*";
	
	if ((imports.find(importit) == -1) &&
	    (imports.find(importstar) == -1) &&
	    (gen_art->pythonSource().find(importit) == -1) &&
	    (gen_art->pythonSource().find(importstar) == -1))
	  f << cl_pack << "." << cl_art->name() << "." << name();
	else
	  // imported
	  f << name();
      }
      else {
	QCString s = "from " + cl_art->name() + " import ";
	const QCString & imports = UmlArtifact::all_imports();
	QCString importit = s + name();
	QCString importstar = s + "*";
	
	if ((imports.find(importit) == -1) &&
	    (imports.find(importstar) == -1) &&
	    (gen_art->pythonSource().find(importit) == -1) &&
	    (gen_art->pythonSource().find(importstar) == -1))
	  f << cl_art->name() << "." << name();
	else
	  // imported
	  f << name();
      }
    }
    else
      // same artifact
      f << name();
  }
}

