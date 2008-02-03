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

#include <stdio.h>
#include <stdlib.h>
#include <qtextstream.h> 
#include <qfile.h>
#include <qfileinfo.h>

#include "UmlOperation.h"
#include "UmlCom.h"
#include "util.h"

const char * BodyPrefix = "// Bouml preserved body begin ";
const char * BodyPostfix = "// Bouml preserved body end ";
const char * BodyPythonPrefix = "## Bouml preserved body begin ";
const char * BodyPythonPostfix = "## Bouml preserved body end ";
const int BodyPrefixLength = 30;
const int BodyPostfixLength = 28;

static char * read_file(const char * filename)
{
  QFile fp(filename);
  
  if (fp.open(IO_ReadOnly)) {
    QFileInfo fi(fp);
    int size = fi.size();
    char * s = new char[size + 1];
    
    if (fp.readBlock(s, size) == -1) {
      delete [] s;
      return 0;
    }
    else {
      s[size] = 0;
      
      return s;
    }
  }
  else
    return 0;
}

void UmlOperation::roundtrip(const char * path, aLanguage who)
{
  char * s = read_file(path);
  
  if (s != 0) {
    char * p1 = s;
    char * p2;
    QCString (UmlOperation::*get_body)();
    bool (UmlOperation::*set_body)(const char * s);
    bool (UmlOperation::*set_contextualbodyindent)(bool v);
    const char * prefix;
    const char * postfix;
    
    switch (who) {
    case cppLanguage:
      get_body = &UmlOperation::cppBody;
      set_body = &UmlOperation::set_CppBody;
      set_contextualbodyindent = &UmlOperation::set_CppContextualBodyIndent;
      prefix = BodyPrefix;
      postfix = BodyPostfix;
      break;
    case javaLanguage:
      get_body = &UmlOperation::javaBody;
      set_body = &UmlOperation::set_JavaBody;
      set_contextualbodyindent = &UmlOperation::set_JavaContextualBodyIndent;
      prefix = BodyPrefix;
      postfix = BodyPostfix;
      break;
    case phpLanguage:
      get_body = &UmlOperation::phpBody;
      set_body = &UmlOperation::set_PhpBody;
      set_contextualbodyindent = &UmlOperation::set_PhpContextualBodyIndent;
      prefix = BodyPrefix;
      postfix = BodyPostfix;
      break;
    default:
      // python
      get_body = &UmlOperation::pythonBody;
      set_body = &UmlOperation::set_PythonBody;
      set_contextualbodyindent = &UmlOperation::set_PythonContextualBodyIndent;
      prefix = BodyPythonPrefix;
      postfix = BodyPythonPostfix;
    }
    
    while ((p2 = strstr(p1, prefix)) != 0) {
      p2 += BodyPrefixLength;
      
      char * body;
      long id = strtol(p2, &body, 16);
      
      if (body != (p2 + 8)) {
	UmlCom::trace(QCString("<font color =\"red\"> Error in ") + path +
		      " : unvalid preserve body identifier</font><br>");
	UmlCom::bye();
	UmlCom::fatal_error("read_bodies 1");
      }
      
      if (*body == '\r')
	body += 1;
      if (*body == '\n')
	body += 1;
      else {
	UmlCom::trace(QCString("<font  color =\"red\"> Error in ") + path + 
		      " : unvalid preserve body block, end of line expected</font><br>");
	UmlCom::bye();
	UmlCom::fatal_error("read_bodies 2");
      }

      UmlOperation * op = (UmlOperation *)
	UmlBaseItem::from_id((unsigned) id, anOperation);
      
      if (op == 0) {
	QCString n;
	
	n.sprintf("%x", (unsigned) id);
	UmlCom::trace(QCString("<font  color =\"red\"> Error in ") + path + 
		      " : unvalid operation id " + n + "</font><br>");
	UmlCom::bye();
	UmlCom::fatal_error("read_bodies 3");
	return;
      }
      
      if (((p1 = strstr(body, postfix)) == 0) ||
	  (strncmp(p1 + BodyPostfixLength, p2, 8) != 0)) {
	UmlCom::trace(QCString("<font  color =\"red\"> Error in ") + path + 
		      " : unvalid preserve body block, wrong balanced</font><br>");
	UmlCom::bye();
	UmlCom::fatal_error("read_bodies 4");
      }

      p2 = p1;
      while ((p2 != body) && (p2[-1] != '\n'))
	p2 -= 1;
      *p2 = 0;
      
      QCString previous = (op->*get_body)();
	  
      if (!op->isBodyGenerationForced() && (body != previous)) {	
	if (! (op->*set_body)(body)) {
	  write_trace_header();
	  UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>cannot update body of <i>"
			+ op->name() + "</i></b></font><br>");
	  incr_error();
	}
	else {
	  (op->*set_contextualbodyindent)(FALSE);
	  
	  write_trace_header();
	  UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;update body of <i>"
			+ op->name() + "</i><br>");
	}
      }
      else if (verbose()) {
	write_trace_header();
	UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;body of <i>"
		      + op->name() + "</i> unchanged<br>");
      }
      
      p1 += BodyPostfixLength + 8;
    }
    
    delete [] s;
  }
}
