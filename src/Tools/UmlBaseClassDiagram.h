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

#ifndef UMLBASECLASSDIAGRAM_H
#define UMLBASECLASSDIAGRAM_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlDiagram.h"

class UmlClassDiagram;
class UmlClassView;

class UmlBaseClassDiagram : public UmlDiagram {
  public:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseClassDiagram(void * id, const QCString & n) 
      : UmlDiagram(id, n) {};
  
    // returns a new class diagram named 'name' created under 'parent', or
    // the null pointer in case it cannot be created (the name is
    // already used or invalid, 'parent' cannot contain it etc ...)
    static UmlClassDiagram * create(UmlClassView * parent, const char * name);
    
    // returns the kind of the item
    virtual anItemKind kind();
};

#endif
