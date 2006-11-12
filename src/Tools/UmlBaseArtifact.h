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

#ifndef UMLBASEARTIFACT_H
#define UMLBASEARTIFACT_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include "UmlClass.h"

class UmlArtifact;
class UmlDeploymentView;
class UmlDeploymentDiagram;


class UmlBaseArtifact : public UmlItem {    
  public:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseArtifact(void * id, const QCString & n);
  
    // returns a new artifact named 'name' created under 'parent', or
    // the null pointer in case it cannot be created (the name is
    // already used or invalid, 'parent' cannot contain it etc ...)
    static UmlArtifact * create(UmlDeploymentView * parent, const char * name);
    
    // returns the kind of the item
    virtual anItemKind kind();
    
    // returns the optional associated diagram
    UmlDeploymentDiagram * associatedDiagram();
    
    // sets the associated diagram, arg may be 0
    bool set_AssociatedDiagram(UmlDeploymentDiagram *);
    
    // returns the optional associated classes
    // significant when the artifact is stereotyped <<source>>
    const QVector<UmlClass> & associatedClasses();
    
    // adds 'cl' at the end of the associated classes list,
    // returns false on error
    // significant when the artifact is stereotyped <<source>>
    bool addAssociatedClass(UmlClass * cl);
    
    // removes an associated class
    // significant when the artifact is not stereotyped <<source>>
    bool removeAssociatedClass(UmlClass * cl);
    
    // set the associated classes list
    // significant when the artifact is stereotyped <<source>>
    bool set_AssociatedClasses(const QVector<UmlClass> &);
    
    // returns the associated artifacts
    // significant when the artifact is not stereotyped <<source>>
    const QVector<UmlArtifact> & associatedArtifacts();
    
    // adds an associated artifacts, returns false if 'cp' is already
    // an associate artifact.
    // significant when the artifact is not stereotyped <<source>>
    bool addAssociatedArtifact(UmlArtifact * cp);

    // removes an associated artifacts
    // significant when the artifact is not stereotyped <<source>>
    bool removeAssociatedArtifact(UmlArtifact * cp);

    // removes all associated artifacts
    // significant when the artifact is not stereotyped <<source>>
    bool removeAllAssociatedArtifacts();
    
#ifdef WITHCPP
    // returns the C++ header file definition
    const QCString & cppHeader();
    
    // to set the C++ header file definition, returns FALSE on error
    bool set_CppHeader(const QCString &);
    
    // returns the C++ source file definition
    const QCString & cppSource();
    
    // to set the C++ source file definition, returns FALSE on error
    bool set_CppSource(const QCString &);
#endif
    
#ifdef WITHJAVA
    // returns the Java file definition
    const QCString & javaSource();
    
    // to set the Java file definition, returns FALSE on error
    bool set_JavaSource(const QCString &);
#endif
    
#ifdef WITHIDL
    // returns the Idl file definition
    const QCString & idlSource();
    
    // to set the Idl file definition, returns FALSE on error
    bool set_IdlSource(const QCString &);
#endif
    
    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);
    
  private:
    friend class UmlBaseClass;	// allow it to set _assoc_class
  
    UmlDeploymentDiagram * _assoc_diagram;
    QVector<UmlClass> _assoc_classes;
    QVector<UmlArtifact> _associated;

#ifdef WITHCPP
    QCString _cpp_h;
    QCString _cpp_src;
#endif
    
#ifdef WITHJAVA
    QCString _java_src;
#endif
    
#ifdef WITHIDL
    QCString _idl_src;
#endif
    
    virtual void read_uml_();
#ifdef WITHCPP
    virtual void read_cpp_();
#endif
#ifdef WITHJAVA
    virtual void read_java_();
#endif
#ifdef WITHIDL
    virtual void read_idl_();
#endif
};

#endif
