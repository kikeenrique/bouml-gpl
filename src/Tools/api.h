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

#ifndef API_H
#define API_H

// The king of a relation, returned by UmlBaseRelation::RelationKind()
// and UmlBaseSimpleRelationKind()
// concern classes
enum aRelationKind {
  anAssociation,
  aDirectionalAssociation,
  aGeneralisation,
  anAggregation,
  anAggregationByValue,
  aDirectionalAggregation,
  aDirectionalAggregationByValue,
  aDependency,
  aRealization
};

// The direction of an operation's parameter, see UmlParameter
enum aDirection {
  InputOutputDirection, InputDirection, OutputDirection
};

// The visibility of a class member
// The default case is used to configure the Drawing Settings and
// for the C++ cases (see UmlBaseClassMember::CppVisibility())
enum aVisibility {
  PublicVisibility,
  ProtectedVisibility,
  PrivateVisibility,
  PackageVisibility,
  DefaultVisibility
};

// the king of any item, returned by UmlBaseItem::Kind()
enum anItemKind {
  aRelation, anAttribute, anOperation, anExtraClassMember,
  aClass, anUseCase, aComponent, aNode, anArtifact,
  aNcRelation,
  aClassDiagram, anUseCaseDiagram, aSequenceDiagram,
  aCollaborationDiagram, aComponentDiagram, aDeploymentDiagram,
  //anObjectDiagram, anActivityDiagram,	// api >= 24
  aClassView, anUseCaseView, aComponentView, aDeploymentView,
  aPackage,
  
  aState, aTransition, aRegion, aStateDiagram, aStateAction,
  anInitialPseudoState, anEntryPointPseudoState,
  aFinalState, aTerminatePseudoState,
  anExitPointPseudoState, aDeepHistoryPseudoState,
  aShallowHistoryPseudoState, aJunctionPseudoState,
  aChoicePseudoState, aForkPseudoState, aJoinPseudoState,
};

#endif
