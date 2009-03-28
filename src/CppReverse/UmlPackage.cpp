// *************************************************************************
//
// Copyleft 2004-2009 Bruno PAGES  .
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

#include "UmlPackage.h"
#include "UmlClassView.h"
#include "UmlArtifact.h"
#include "UmlDeploymentView.h"
#include "UmlOperation.h"
#include "Lex.h"
#include "UmlCom.h"

UmlPackage::UmlPackage(void * id, const QCString & n)
    : UmlBasePackage(id, n) {
  class_view = 0;
#ifdef REVERSE
  deployment_view = 0;
#endif
  namespace_fixedp = FALSE;
}

UmlClassView * UmlPackage::get_classview(const QCString & nmsp) {
  UmlPackage * pack;
  
  if (nmsp != cppNamespace()) {
    if (namespace_fixedp) {
      if ((pack = findCppNamespace(nmsp)) == 0) {
	QCString s = nmsp;
	
	if (s.isEmpty())
	  s = name();
	else {
	  int index = 0;
	  
	  while ((index = s.find("::", index)) != -1)
	    s.replace(index++, 2, " ");
	}
	
	if (((pack = UmlBasePackage::create(this, s)) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0)) {
#ifdef REVERSE
	  UmlCom::trace(QCString("<font face=helvetica><b>cannot create package <i>")
			+ s + "</i> under package <i>"
			+ name() + "</b></font><br>");
	  UmlCom::message("");
	  throw 0;
#else
	  QMessageBox::critical(0, "Fatal Error", 
				QCString("<font face=helvetica><b>cannot create package <i>")
				+ s + "</i> under package <i>"
				+ Name() + "</b></font><br>");
	  QApplication::exit(1);
#endif	  
	}
	
	pack->set_CppNamespace(nmsp);
	pack->namespace_fixedp = TRUE;
      }
    }
    else {
      pack = this;
      pack->set_CppNamespace(nmsp);
      pack->namespace_fixedp = TRUE;
    }
  }
  else
    pack = this;
  
  if (pack->class_view == 0) {
    QVector<UmlItem> ch = pack->children();
    
    for (unsigned index = 0; index != ch.size(); index += 1)
      // return the first class view find
      if (ch[index]->kind() == aClassView)
	return pack->class_view = (UmlClassView *) ch[index];
    
    if ((pack->class_view = UmlBaseClassView::create(pack, name())) == 0) {
#ifdef REVERSE
      UmlCom::trace(QCString("<font face=helvetica><b>cannot create class view <i>")
		    + name() + "</i> under package <i>"
		    + pack->name() + "</b></font><br>");
      UmlCom::message("");
      throw 0;
#else
      QMessageBox::critical(0, "Fatal Error", 
			    QCString("<font face=helvetica><b>cannot create class view <i>")
			    + name() + "</i> under package <i>"
			    + pack->name() + "</b></font><br>");
      QApplication::exit(1);
#endif
    }
  }
  
  return pack->class_view;
}

#ifdef REVERSE

void UmlPackage::reverse_main(const QCString & type, QCString comment) {
  // do not lost main !
  Lex::mark();
  UmlOperation::skip_body();
  
  UmlArtifact * cp;
  
  if ((cp = UmlBaseArtifact::create(get_deploymentview(0), "main")) == 0)
    UmlCom::trace("<font face=helvetica><b>cannot create <i>artifact main</i></b></font><br><hr>");
  else {
    cp->set_Stereotype("source");

    if (! comment.isEmpty()) {
      unsigned start = 0;
      
      do {
	comment.insert(start, "//");
	start = comment.find('\n', start + 2) + 1;
      } while (start != 0);
      comment.append("\n\n");
    }
    
    comment.append(type);
    comment.append(" main(");
    comment.append(Lex::region());
    comment.append("\n");
    
    cp->set_CppSource(comment);
    cp->set_CppHeader(0);
  }
}

UmlDeploymentView * UmlPackage::get_deploymentview(const QCString & nmsp) {
  UmlPackage * pack;
  
  if (nmsp != cppNamespace()) {
    if (namespace_fixedp) {
      if ((pack = findCppNamespace(nmsp)) == 0) {
	QCString s = nmsp;
	
	if (s.isEmpty())
	  s = name();
	else {
	  int index = 0;
	  
	  while ((index = s.find("::", index)) != -1)
	    s.replace(index++, 2, " ");
	}
	
	if (((pack = UmlBasePackage::create(this, s)) == 0) &&
	    ((pack = UmlBasePackage::create(this, s += "_")) == 0)) {
#ifdef REVERSE
	  UmlCom::trace(QCString("<font face=helvetica><b>cannot create package <i>")
			+ s + "</i> under package <i>"
			+ name() + "</b></font><br>");
	  UmlCom::message("");
	  throw 0;
#else
	  QMessageBox::critical(0, "Fatal Error", 
				QCString("<font face=helvetica><b>cannot create package <i>")
				+ s + "</i> under package <i>"
				+ Name() + "</b></font><br>");
	  QApplication::exit(1);
#endif	  
	}
	
	pack->set_CppNamespace(nmsp);
	pack->namespace_fixedp = TRUE;
      }
    }
    else {
      pack = this;
      pack->set_CppNamespace(nmsp);
      pack->namespace_fixedp = TRUE;
    }
  }
  else
    pack = this;
  
  if (pack->deployment_view == 0) {
    QVector<UmlItem> ch = pack->children();
    
    for (unsigned index = 0; index != ch.size(); index += 1)
      // return the first class view find
      if (ch[index]->kind() == aDeploymentView)
	return pack->deployment_view = (UmlDeploymentView *) ch[index];
    
    if ((pack->deployment_view = UmlBaseDeploymentView::create(pack, name())) == 0) {
#ifdef REVERSE
      UmlCom::trace(QCString("<font face=helvetica><b>cannot create deployment view <i>")
		    + name() + "</i> under package <i>"
		    + pack->name() + "</b></font><br>");
      UmlCom::message("");
      throw 0;
#else
      QMessageBox::critical(0, "Fatal Error", 
			    QCString("<font face=helvetica><b>cannot create deployment view <i>")
			    + name() + "</i> under package <i>"
			    + pack->name() + "</b></font><br>");
      QApplication::exit(1);
#endif
    }
  }
  
  return pack->deployment_view;
}

#endif

