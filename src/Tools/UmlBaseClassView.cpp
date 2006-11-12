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

#include "UmlBaseClassView.h"
#include "UmlComponentView.h"
#include "UmlPackage.h"
#include "UmlCom.h"

anItemKind UmlBaseClassView::kind() {
  return aClassView;
}

UmlClassView * UmlBaseClassView::create(UmlPackage * parent, const char * s)
{
  return (UmlClassView *) parent->create_(aClassView, s);
}

void UmlBaseClassView::read_uml_() {
  _assoc_view = (UmlComponentView *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
}

UmlComponentView * UmlBaseClassView::associatedComponentView() {
  read_if_needed_();
  
  return _assoc_view;
}

bool UmlBaseClassView::set_AssociatedComponentView(UmlComponentView * v) {
  UmlCom::send_cmd(_identifier, setAssocViewCmd,
		   ((UmlBaseItem *) v)->_identifier);
  if (UmlCom::read_ack()) {
    _assoc_view = v;
    return TRUE;
  }
  else
    return FALSE;
}
