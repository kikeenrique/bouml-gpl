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

#ifndef UMLSETTINGS
#define UMLSETTINGS

//  Do NOT use it, refer to CppSettings, JavaSettings and IdlSettings classes
/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include <qdict.h>

struct UmlBuiltin {
  QCString uml;
#ifdef WITHCPP
  QCString cpp;
  QCString cpp_in;
  QCString cpp_out;
  QCString cpp_inout;
  QCString cpp_return;
#endif
#ifdef WITHJAVA
  QCString java;
#endif
#ifdef WITHIDL
  QCString idl;
#endif
};

struct UmlStereotype {
  QCString uml;
#ifdef WITHCPP
  QCString cpp;
#endif
#ifdef WITHJAVA
  QCString java;
#endif
#ifdef WITHIDL
  QCString idl;
#endif
};

// this class manage data configured through the 'Generation settings' dialog
//
// This class may be defined as a 'singleton', but I prefer to use static 
// members allowing to just write 'UmlSettings::member' rather than
// 'UmlSettings::instance()->member' or other long sentence like this.

class UmlSettings {
  protected:
    // never called !
    UmlSettings() {};
  
    static bool _defined;
  
    static QDict<UmlBuiltin> _map_builtins;
    static UmlBuiltin * _builtins;
    static QDict<UmlStereotype> _map_relation_stereotypes;
    static UmlStereotype * _relation_stereotypes;
    static QDict<UmlStereotype> _map_class_stereotypes;
    static UmlStereotype * _class_stereotypes;
    
    static void read_();
    static void read_if_needed_();
    
    static unsigned multiplicity_column(const QCString & mult);
    static QCString uml_type(const QCString & t, QCString UmlBuiltin::* f);
    static QCString uml_rel_stereotype(const QCString & t, QCString UmlStereotype::* f);
    static QCString uml_class_stereotype(const QCString & t, QCString UmlStereotype::* f);
    static UmlBuiltin * add_type(const QCString & s);
    static UmlStereotype * add_rel_stereotype(const QCString & s);
    static UmlStereotype * add_class_stereotype(const QCString & s);
};

#endif
