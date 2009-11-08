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

#ifndef _ERR_H
#define _ERR_H

#include <stdlib.h>

// to manage case where Qt is compiled with -fnoexceptions
extern bool ExitOnError;
#define THROW_ERROR if (ExitOnError) ::exit(0); else throw 
#define PRE_TRY bool previous_ExitOnError = ExitOnError; ExitOnError = FALSE
#define POST_TRY ExitOnError = previous_ExitOnError

#endif
