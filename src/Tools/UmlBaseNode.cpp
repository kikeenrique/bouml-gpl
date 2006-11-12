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

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlBaseNode.h"
#include "UmlDeploymentDiagram.h"
#include "UmlCom.h"

anItemKind UmlBaseNode::kind() {
  return aNode;
}

UmlNode * UmlBaseNode::create(UmlItem * parent, const char * s)
{
  return (UmlNode *) parent->create_(aNode, s);
}

void UmlBaseNode::read_uml_() {
  _assoc_diagram = (UmlDeploymentDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
}

UmlDeploymentDiagram * UmlBaseNode::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBaseNode::set_AssociatedDiagram(UmlDeploymentDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd,
		   ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_ack()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}


