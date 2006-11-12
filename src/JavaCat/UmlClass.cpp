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

#include "UmlClass.h"
#ifdef REVERSE
#include "UmlArtifact.h"
#include "UmlDeploymentView.h"
#include "UmlPackage.h"
#include "JavaCatWindow.h"
#include "JavaSettings.h"
#include "Class.h"
#endif

UmlFormalParameter::UmlFormalParameter(QCString n, const UmlTypeSpec & e) {
  _name = n;
  _extends = e;
}

UmlClass::UmlClass(void * id, const QCString & n) 
    : UmlBaseClass(id, n) {
}

#ifdef REVERSE

void UmlClass::need_artifact(const QStringList & imports,
			     const QStringList & static_imports,
			     UmlArtifact *& cp) {
  if (parent()->kind() == aClassView) {
    if (cp != 0)
      cp->addAssociatedClass(this);
    else {
      UmlPackage * pack = (UmlPackage *) parent()->parent();
      
      if ((cp = associatedArtifact()) == 0) {
	// create associated artifact
	if ((cp = UmlBaseArtifact::create(pack->get_deploymentview(), name())) == 0) {
	  UmlCom::trace(QCString("<font face=helvetica><b>cannot create<i> artifact ")
			+ name() + "</i></b></font><br>"); 
	  return;
	}
	cp->addAssociatedClass(this);
      }
      
      cp->set_Stereotype("source");
      
      QString s = JavaSettings::sourceContent();
      int index = s.find("${definition}");
      
      if (index != -1) {
	QStringList::ConstIterator it;
	
	for (it = imports.begin(); *it; ++it) {
	  QString import = *it;
	  
	  import += (((const char *) import)[import.length() - 1] == '.')
	    ? "*;\n" : ";\n";
	  
	  s.insert(index, QString("import ") + import);
	  index = s.find("${definition}", index);
	}
	
	for (it = static_imports.begin(); *it; ++it) {
	  s.insert(index, QString("import static") + *it + "\n");
	  index = s.find("${definition}", index);
	}
      }
      
      cp->set_JavaSource(QCString(s));
    }
  }
}

#endif

void UmlClass::manage_generic(QCString & form, UmlTypeSpec & typespec,
			      QCString str_actuals, const char * k)
{
  if (typespec.type != 0) {
    int index;
  
    if (!typespec.explicit_type.isEmpty()) {
      if ((index = form.find(k)) != -1)
	form.replace(index, strlen(k), typespec.explicit_type);
    }
    else if (! str_actuals.isEmpty()) {
      if ((index = form.find(k)) != -1)
	form.insert(index + strlen(k), str_actuals);
    }
  }
}
