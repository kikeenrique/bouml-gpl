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
    
    while ((p2 = strstr(p1, BodyPrefix)) != 0) {
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
	
	n.sprintf("%x", id);
	UmlCom::trace(QCString("<font  color =\"red\"> Error in ") + path + 
		      " : unvalid operation id " + n + "</font><br>");
	UmlCom::bye();
	UmlCom::fatal_error("read_bodies 3");
	return;
      }
      
      if (((p1 = strstr(body, BodyPostfix)) == 0) ||
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
      
      QCString previous;
      
      switch (who) {
      case cppLanguage:
	previous = op->cppBody();
	break;
      case javaLanguage:
	previous = op->javaBody();
	break;
      default:
	previous = op->phpBody();
      }
	  
      if (!op->isBodyGenerationForced() && (body != previous)) {
	bool ok;
	
	switch (who) {
	case cppLanguage:
	  ok = op->set_CppBody(body);
	  if (ok)
	    op->set_CppContextualBodyIndent(FALSE);
	  break;
	case javaLanguage:
	  ok = op->set_JavaBody(body);
	  if (ok)
	    op->set_JavaContextualBodyIndent(FALSE);
	  break;
	default:
	  ok = op->set_PhpBody(body);
	  if (ok)
	    op->set_PhpContextualBodyIndent(FALSE);
	}
	
	if (! ok) {
	  write_trace_header();
	  UmlCom::trace("<tt>        </tt><font color=\"red\"><b>cannot update body of <i>"
			+ op->name() + "</i></b></font><br>");
	  incr_error();
	}
	else {
	  write_trace_header();
	  UmlCom::trace("<tt>        </tt>update body of <i>"
			+ op->name() + "</i><br>");
	}
      }
      else if (verbose()) {
	write_trace_header();
	UmlCom::trace("<tt>        </tt>body of <i>"
		      + op->name() + "</i> unchanged<br>");
      }
      
      p1 += BodyPostfixLength + 8;
    }
    
    delete [] s;
  }
}
