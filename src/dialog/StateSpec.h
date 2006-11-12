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

#ifndef STATESPEC_H
#define STATESPEC_H

#include "UmlEnum.h"

class StateSpec {
  public:
    enum kind {
      is3states, isClassDrawingMode, isDrawingLanguage, isShowContextMode
    };
  
    const char * name;
    void * state;
    kind who;
    
    void set(const char * n, Uml3States * s) {
      name = n;
      state = s;
      who = is3states;
    };
    void set(const char * n, ClassDrawingMode * s) {
      name = n;
      state = s;
      who = isClassDrawingMode;
    };
    void set(const char * n, DrawingLanguage * s) {
      name = n;
      state = s;
      who = isDrawingLanguage;
    };
    void set(const char * n, ShowContextMode * s) {
      name = n;
      state = s;
      who = isShowContextMode;
    };
    
    void set_state(int v) {
      switch (who) {
      case is3states:
	*((Uml3States *) state) = (Uml3States) v;
	break;
      case isClassDrawingMode:
	*((ClassDrawingMode *) state) = (ClassDrawingMode) v;
	break;
      case isDrawingLanguage:
	*((DrawingLanguage *) state) = (DrawingLanguage) v;
	break;
      default:
	*((ShowContextMode *) state) = (ShowContextMode) v;
      };
    };
};

#endif
