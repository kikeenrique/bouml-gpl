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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qintdict.h> 
#include <qdatetime.h> 

#include "mu.h"
#include "Labeled.h"
#include "myio.h"

// to manage renumber case : old diagram (format < 9) and
// import project

static bool NeedRenumber;

void set_in_import(bool y)
{
  NeedRenumber = y;
}

bool in_import()
{
  return NeedRenumber;
}

//
    
int place(IdDict<void> & d, int id, void * x)
{
  if (id != -1) {
    // id is relevant
    if (id == 0) {
      // compute a new id
      if ((d.idmax < FIRST_ID) && (user_id() != 0))
	d.idmax = FIRST_ID - 128;
      id = (d.idmax += 128) | user_id();
    }
    else if (d.old_diagram) {
      // place id unchanged among the old ones
      d.dict[1].insert(id, x);
      
      if ((d.dict[1].count() / 2) >= d.dict[1].size())
	d.dict[1].resize(d.dict[1].size() * 2 - 1);
      
      // id doesn't contains a user_id field
      // create new one for the current user_id
      if ((d.idmax < FIRST_ID) && (user_id() != 0))
	d.idmax = FIRST_ID - 128;
      id = (d.idmax += 128) | user_id();
    }
    else if (NeedRenumber) {
      // place id unchanged among the old ones
      d.dict[1].insert(id, x);
      
      if ((d.dict[1].count() / 2) >= d.dict[1].size())
	d.dict[1].resize(d.dict[1].size() * 2 - 1);
      
      if ((d.dict[0][id] != 0) || ((id & 127) != user_id())) {
	// already used or for an other user, change id to a new one
	if ((id & 127) < 2) {
	  // import a plug out in a plug out !!!!!
	  // user_id part is unchanged
	  id = (id & 127) + FIRST_ID;
	  
	  while (d.dict[0][id] != 0)
	    id += 128;
	}
	else {
	  // create new id for the current user_id
	  if ((d.idmax < FIRST_ID) && (user_id() != 0))
	    d.idmax = FIRST_ID - 128;
	  id = (d.idmax += 128) | user_id();
	}
      }
      else
	// id unchanged for the current user
	if ((((unsigned) (id & ~127)) > ((unsigned) d.idmax)) &&
	    ((id & 127) == user_id()))
	  d.idmax = id & ~127;
    }
    else {
      // no renum, id unchanged
      if ((((unsigned) (id & ~127)) > ((unsigned) d.idmax)) &&
	  ((id & 127) == user_id()))
	d.idmax = id & ~127;
    }
    
    d.dict[0].insert(id, x);
    
    if ((d.dict[0].count() / 2) >= d.dict[0].size())
      d.dict[0].resize(d.dict[0].size() * 2 - 1);
  }
  
  return id;
}

int new_place(IdDict<void> & d, int user_id, void * x)
{
  if (d.idmax == FIRST_ID)
    d.idmax = FIRST_BASE_ID | user_id;
  else
    d.idmax = (d.idmax + 128) | user_id;
  
  d.dict[0].insert(d.idmax, x);
  return d.idmax;
}
