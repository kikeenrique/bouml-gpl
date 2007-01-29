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

#ifndef REVERSE
#include <qapplication.h>
#include <qmessagebox.h>
#endif

#include "UmlPackage.h"
#include "UmlCom.h"
#include "UmlClassView.h"
#ifdef REVERSE
#include "UmlDeploymentView.h"
#endif

UmlPackage::UmlPackage(void * id, const QCString & n)
    : UmlBasePackage(id, n) {
  classview = 0;
#ifdef REVERSE
  deploymentview = 0;
#endif
}

UmlClassView * UmlPackage::get_classview() {
  if (classview == 0) {
    QVector<UmlItem> ch = UmlItem::children();
    
    for (unsigned index = 0; index != ch.size(); index += 1)
      // return the first class view find
      if (ch[index]->kind() == aClassView)
	return classview = (UmlClassView *) ch[index];
    
    if ((classview = UmlBaseClassView::create(this, name())) == 0) {
#ifdef REVERSE
      UmlCom::trace(QCString("<font face=helvetica><b>cannot create class view <i>")
		    + name() + "</i> under package <i>"
		    + name() + "</b></font><br>");
      UmlCom::message("");
      throw 0;
#else
      QMessageBox::critical(0, "Fatal Error", 
			    QCString("<font face=helvetica><b>cannot create class view <i>")
			    + name() + "</i> under package <i>"
			    + name() + "</b></font><br>");
      QApplication::exit(1);
#endif
    }
  }
  
  return classview;
}

#ifdef REVERSE

UmlDeploymentView * UmlPackage::get_deploymentview() {
  if (deploymentview == 0) {
    QVector<UmlItem> ch = UmlItem::children();
    
    for (unsigned index = 0; index != ch.size(); index += 1)
      // return the first class view find
      if (ch[index]->kind() == aDeploymentView)
	return deploymentview = (UmlDeploymentView *) ch[index];
    
    if ((deploymentview = UmlBaseDeploymentView::create(this, name())) == 0) {
#ifdef REVERSE
      UmlCom::trace(QCString("<font face=helvetica><b>cannot create deployment view <i>")
		    + name() + "</i> under package <i>"
		    + name() + "</b></font><br>");
      UmlCom::message("");
      throw 0;
#else
      QMessageBox::critical(0, "Fatal Error", 
			    QCString("<font face=helvetica><b>cannot create deployment view <i>")
			    + name() + "</i> under package <i>"
			    + name() + "</b></font><br>");
      QApplication::exit(1);
#endif
    }
  }
  
  return deploymentview;
}

#endif
