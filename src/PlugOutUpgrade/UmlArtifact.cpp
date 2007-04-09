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

#include <stdio.h>
#include <qtextstream.h> 

#include "UmlArtifact.h"
#include "UmlDeploymentView.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"

UmlArtifact * UmlArtifact::made(UmlDeploymentView * depl_view,
				const QCString & s)
{
  UmlArtifact * art = UmlBaseArtifact::create(depl_view, s);
  
  if (art == 0) {
    QCString msg = "can't create artifact " + s + " in " + depl_view->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  UmlCom::trace("add artifact " + s + "<br>\n");
  art->set_Stereotype("source");
  art->set_CppHeader(CppSettings::headerContent());
  art->set_CppSource(CppSettings::sourceContent());
  art->set_JavaSource(JavaSettings::sourceContent());
  
  return art;
}
