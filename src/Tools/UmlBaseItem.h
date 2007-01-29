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

#ifndef UMLBASEITEM_H
#define UMLBASEITEM_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include <qvector.h>
#include <qptrdict.h> 
#include <qstring.h>
#include <qdict.h> 

#include "api.h"
#include "ApiCmd.h"

class UmlItem;
class UmlTypeSpec;

// Base class of all the classes representing browser's objects.
//
// In the API all for each 'system' class exists a 'user' class
// under your responsability which inherits the 'system' class.
//
// You do not have to change the 'system' classes, place your
// attributes and operations in the corresponding 'user' classes :
// all the instances made by the 'system' classes are instance of
// the corresponding 'user' classes.
//
// You must never create or delete yourself a class of the API,
// use the Create operation defined in the 'system' classes.
//
// To not allow you to access to the 'system' attributes or to
// call 'system' internal operations even by error, the 'system' internal
// parts are declared 'private' rather than 'public' or 'protected'.

class UmlBaseItem {
  friend class UmlBaseClassItem;	// allows other 'system' classes 
  friend class UmlBaseAttribute;	// to access to the private part of
  friend class UmlBaseClass;		// UmlBaseItem
  friend class UmlBaseClassMember;
  friend class UmlBaseClassDiagram;
  friend class UmlBaseClassView;
  friend class UmlBaseCollaborationDiagram;
  friend class UmlBaseComponent;
  friend class UmlBaseComponentDiagram;
  friend class UmlBaseComponentView;
  friend class UmlBaseDeployment;
  friend class UmlBaseArtifact;
  friend class UmlBaseDeploymentDiagram;
  friend class UmlBaseDeploymentView;
  friend class UmlBaseExtraClassMember;
  friend class UmlBaseNode;
  friend class UmlBaseOperation;
  friend class UmlBasePackage;
  friend class UmlBaseRelation;
  friend class UmlBaseNcRelation;
  friend class UmlBaseSequenceDiagram;
  friend class UmlBaseUseCase;
  friend class UmlBaseUseCaseDiagram;
  friend class UmlBaseUseCaseView;
  friend class UmlBaseDiagram;
  friend class UmlBaseFormalParameter;
  friend class UmlBaseActualParameter;
  friend class UmlBaseChoicePseudoState;
  friend class UmlBaseDeepHistoryPseudoState;
  friend class UmlBaseEntryPointPseudoState;
  friend class UmlBaseExitPointPseudoState;
  friend class UmlBaseFinalState;
  friend class UmlBaseForkPseudoState;
  friend class UmlBaseInitialPseudoState;
  friend class UmlBaseJoinPseudoState;
  friend class UmlBaseJunctionPseudoState;
  friend class UmlBaseShallowHistoryPseudoState;
  friend class UmlBaseStateAction;
  friend class UmlBaseState;
  friend class UmlBaseStateDiagram;
  friend class UmlBaseTerminatePseudoState;
  friend class UmlBaseRegion;
  friend class UmlBaseTransition;
  friend class UmlCom;
  
  private:
    bool _defined;
    bool _marked;
    void * _identifier;
    QCString _name;
    QCString _stereotype;
    QCString _description;
    UmlItem * _parent;
    QVector<UmlItem> * _children;
    QDict<QCString> _dict;
    
    static QPtrDict<UmlItem> _all;
    
    void read_if_needed_();
    UmlItem * create_(anItemKind k, const char * s);
    
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
    
    void read_children_();
    void reread_children_if_needed_();
    bool set_it_(bool & r, bool v, OnInstanceCmd cmd);
    bool set_it_(aVisibility & r, aVisibility v, OnInstanceCmd cmd);
    bool set_it_(aDirection & r, aDirection v, OnInstanceCmd cmd);
    bool set_it_(QCString & r, const char * v, OnInstanceCmd cmd);
    bool set_it_(UmlTypeSpec & r, const UmlTypeSpec & t, OnInstanceCmd cmd);
    
    static UmlItem * read_();
    
  public:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseItem(void * id, const QCString & n);
    // the destructor, do not delete objects yourself !!!!!!!!!!
    virtual ~UmlBaseItem();
  
    // returns the kind of the item
    virtual anItemKind kind() = 0;
    
    // returns the name
    const QCString & name() { return _name; };
    
    // to set the name, returns FALSE on error
    bool set_Name(const QCString &);
    
    // returns the stereotype
    const QCString & stereotype();
    
    // to set the stereotype, returns FALSE on error
    virtual bool set_Stereotype(const QCString &);
    
    // returns the description
    const QCString & description();
    
    // to set the description, returns FALSE on error
    bool set_Description(const QCString &);
    
    // returns the parent (0 if it is applied on the project itself)
    // to go all over the browser tree
    UmlItem * parent();
    
    // returns the children, to go all over the browser tree
    const QVector<UmlItem> children();
    
    // return TRUE if the children exist and are visible (independently
    // of the scroll bar) in the browser, else FALSE
    bool childrenVisible();
    
    // open the item in the browser (its children are visible) if y is TRUE,
    // else close it (its children are not visible)
    bool set_childrenVisible(bool y);
    
    // in case the key specified by 'k' exist for the current
    // object, the string 'v' is modified which the associated value
    // and TRUE is returned, else returns FALSE. These properties may
    // be edited through the object dialog last tab named 'user'
    bool propertyValue(const QCString & k, QCString & v);
    
    // to set (may be insert a new) the value 'v' associated
    // to the key 'k', returns FALSE on error
    bool set_PropertyValue(const QCString & k, const QCString & v);
    
    // returns all the properties of the object through a dictionnary
    const QDict<QCString> properties();
    
    //  Returns the absolute path name of the file memoring the Item.
    //  In general an item is saved in a file associated to its package,
    //  the exceptions are :
    //
    //  - the body of the operations of a class which definition contains the
    //     keyword '{$body}'. This path name is returned when you apply
    //     supportFile on any operation of the class.
    //
    //  - the drawing path of the diagrams, this path name is returned when
    //     you apply supportFile on the diagram.
    //
    //  - the configuration of the tools edited throw the entry 'Tools Settings' of
    //     the menu Tools is saved in the file 'tools'
    //
    //  - the configurations of the imports edited through the last Java tab of the
    //     'Generation Settings' dialog is saved in the file 'import'
    QCString supportFile();
    
    // return TRUE in case the item may be modified, i.e. it is not
    // an API base 'plug-out' class and the corresponding file(s) are not
    // read-only.
    bool isWritable();

    //  Apply asynchronously the tool on the item, returns an identifier to call isToolDone()
    int apply(QCString cmd);
    
    // If the parameter is null, move the current item to be\n"
    // the first child of its parent. Else the current item and\n"
    // the parameter must have the same parent, the current item\n"
    // is moved to be just after the parameter.\n"
    // On error return FALSE in C++, produce a RuntimeException in Java");
    bool moveAfter(UmlItem * x);
    
    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    // if 'del' is true the sub items are deleted, you will have to call
    // Children() to re-access to them
    virtual void unload(bool rec = FALSE, bool del = FALSE);

    //  'id' is an identifier returned by apply(), indicates if the tool is still running
    static bool isToolRunning(int id);
    
    // not in plug-outs managed through bouml
    void delete_it();
};

#endif
