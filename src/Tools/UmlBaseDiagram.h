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

#ifndef UMLBASEDIAGRAM_H
#define UMLBASEDIAGRAM_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlItem.h"

class UmlBaseDiagram : public UmlItem {
  public:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseDiagram(void * id, const QCString & n) 
      : UmlItem(id, n) {};
    
    // saves the diagram picture in the file whose pathname is given
    // in argument, the produced file is a PNG file.
    // The aspect of the diagram is :
    // - its visible part in case it is open
    // - the aspect specified by its preferred size and scale in case
    //   these last are specified (see menu on a diagram view)
    // - else the default diagram aspect, which is probably not as you want
    // Returns FALSE on error
    bool saveIn(const QCString & s) const;
};

#endif
