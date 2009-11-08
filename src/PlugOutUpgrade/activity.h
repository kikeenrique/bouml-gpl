// *************************************************************************
//
// Copyright 2004-2009 Bruno PAGES  .
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

#ifndef ACTIVITY_H
#define ACTIVITY_H

extern void add_activity(UmlClass * base_item, UmlClass * user_item);
extern void fixe_activity(UmlClass * base_pinparam);
extern void add_partition(UmlClass * base_item, UmlClass * user_item);
extern void add_additionalactions(UmlClass * base_item, UmlClass * user_item);
extern void add_activity_specification();
#endif
