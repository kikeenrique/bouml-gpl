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

#include <stdio.h>
#include <qtextstream.h> 

#include "UmlArtifact.h"
#include "UmlPackage.h"
#include "UmlOperation.h"
#include "UmlClass.h"
//#include "JavaRefType.h"
#include "UmlCom.h"
#include "util.h"

UmlPackage * UmlArtifact::package_of_generated_artifact;
UmlArtifact * UmlArtifact::current;

void UmlArtifact::generate() {
  if (! managed) {
    package_of_generated_artifact = package();
    current = this;
    managed = TRUE;
    
    const QCString filedef = javaSource();
    
    if (filedef.isEmpty())
      return;
    
    const QCString & name = this->name();
    QCString path = package_of_generated_artifact->file_path(name);
    
    UmlCom::message(name);
    if (verbose())
      UmlCom::trace(QCString("<hr><font face=helvetica>Generate code for <i> ")
		    + name + "</i> in " + path + "</i></font><br>");      
    else
      set_trace_header(QCString("<font face=helvetica>Generate code for <i> ")
		       + name + "</i> in " + path + "</i></font><br>"); 
    
    // get bodies if preserve
    const QVector<UmlClass> & cls = associatedClasses();
    
    if (preserve())
      UmlOperation::read_bodies(path);
      
    // compute dependencies
    
    //QList<JavaRefType> dependencies;
    unsigned n = cls.count();
    unsigned index;

    /*for (index = 0; index != n; index += 1)
      cls[index]->compute_dependencies(dependencies);*/
    
    // generate file
    
    QCString incl;
    QByteArray file;
    // note : QTextOStream(FILE *) does not work under windows
    QTextOStream f(file);
    const char * p = filedef;
    const char * pp = 0;
      
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;
	if (*p == 0)
	  break;
      }
      
      if (*p == '@')
	manage_alias(p, f);
      else if (*p != '$')
	f << *p++;
      else if (!strncmp(p, "${comment}", 10))
	manage_comment(p, pp);
      else if (!strncmp(p, "${description}", 14))
	manage_description(p, pp);
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	f << name;
      }
      else if (!strncmp(p, "${Name}", 7)) {
	p += 7;
	f << capitalize(name);
      }
      else if (!strncmp(p, "${NAME}", 7)) {
	p += 7;
	f << name.upper();
      }
      else if (!strncmp(p, "${imports}", 10)) {
	// not yet managed
	p += 10;
	if (*p == '\n')
	  p += 1;
      }
      else if (!strncmp(p, "${package}", 10)) {
	p += 10;
	
	const QCString & package = package_of_generated_artifact->javaPackage();
	
	if (!package.isEmpty())
	  f << "package " << package << ";\n\n";
	if (*p == '\n')
	  p += 1;
      }
      else if (!strncmp(p, "${definition}", 13)) {
	p += 13;
	for (index = 0; index != n; index += 1)
	  cls[index]->generate(f, current_indent(p, filedef));
	if (*p == '\n')
	  p += 1;
      }
      else
	// strange
	f << *p++;
    }
      
    f << '\000';
    
    if (must_be_saved(path, file)) {
      write_trace_header();
	
      FILE * fp;
      
      if ((fp = fopen((const char *) path, "wb")) == 0) {
	write_trace_header();
	UmlCom::trace(QCString("<font face=helvetica><b><i> ")
		      + name + "</i> : cannot open <i> "
		      + path + "</i>, edit the <i> generation settings</i> (tab directory) or the <i>"
		      + package_of_generated_artifact->name()
		      + "</i> Java directory specification</b></font><br>");
      }
      else {
	fputs((const char *) file, fp);
	fclose(fp);
      }
    }
    else if (get_trace_header().isEmpty())
      UmlCom::trace(QCString("<font face=helvetica><i> ")
		    + path + "</i> not modified</font><br>");
    
    if (imports != 0) {
      delete imports;
      imports = 0;
    }
  }
}

UmlPackage * UmlArtifact::generation_package()
{
  return package_of_generated_artifact;
}

UmlArtifact * UmlArtifact::generated_one()
{
  return current;
}


bool UmlArtifact::imported(QCString pack_name, QCString class_name) {
  if (imports == 0) {
    imports = new QAsciiDict<UmlArtifact>[17];
    
    QCString s = javaSource();
    int index = 0;
    
    while ((index = s.find("import", index)) != -1) {
      if ((index == 0) || (s[index - 1] < ' ')) {
	int index2 = index + 6;
	
	while ((s[index2] == ' ') || (s[index2] == '\t'))
	  index2 += 1;
	
	if ((index2 != (index + 6)) &&
	    ((index = s.find(';', index2)) != -1) &&
	    (index != index2)) {
	 QCString p;
	 
	 if (s[index - 1] == '*') {
	   if ((index - 2) <= index2)
	     continue;
	   p = s.mid(index2, index - index2 - 2);
	 }
	 else
	   p = s.mid(index2, index - index2);
	 
	 imports->insert(p, this);
	}
	else
	  index = index2;
      }
      else
	index += 6;
    }
  }
  
  return ((imports->find(pack_name) != 0) ||
	  (imports->find(pack_name + '.' + class_name) != 0));
}

bool UmlArtifact::must_be_saved(const char * path, const char * new_contains)
{
  FILE * fp = fopen(path, "rb");
  
  if (fp == 0)
    return TRUE;
  
  int c;
  
  while ((c = fgetc(fp)) != EOF) {
    if (c != *new_contains++) {
      fclose(fp);
      return TRUE;
    }
  }
  
  fclose(fp);
  
  return *new_contains != 0;
}
