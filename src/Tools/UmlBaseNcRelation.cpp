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

#include "UmlNcRelation.h"
#include "UmlCom.h"

UmlNcRelation * UmlBaseNcRelation::create(aRelationKind kind, UmlItem * start, UmlItem * end)
{
  UmlCom::send_cmd(start->_identifier, createCmd, aNcRelation,
		   kind, end->_identifier);
  UmlNcRelation * result = (UmlNcRelation *) UmlBaseItem::read_();
  
  if (result != 0)
    start->reread_children_if_needed_();
  
  return result;
}

anItemKind UmlBaseNcRelation::kind() {
  return aNcRelation;
}

aRelationKind UmlBaseNcRelation::relationKind() {
  read_if_needed_();
  
  return _rel_kind;
}

UmlItem * UmlBaseNcRelation::target() {
  read_if_needed_();
  
  return _target;
}

void UmlBaseNcRelation::read_uml_() {
  UmlBaseItem::read_uml_();
  _rel_kind = (aRelationKind) UmlCom::read_char();
  _target = UmlBaseItem::read_();

}

