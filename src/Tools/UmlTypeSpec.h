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

#ifndef UMLTYPESPEC_H
#define UMLTYPESPEC_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include <qstring.h>

class UmlClass;

// this class memorize a type specification, which may be
// - a class reference
// - an explicit type

class UmlTypeSpec {
  public:
    QCString explicit_type;	// significant in case type == 0
    UmlClass * type;		// if != 0 the class representing the type
    
  public:
    UmlTypeSpec() : type(0) {};
    const QCString & toString() const;	// returns the type as a string in all cases
};

#endif
