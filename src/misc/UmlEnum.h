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

#ifndef UMLENUM_H
#define UMLENUM_H

#define RTTI_ARROW	1000
#define RTTI_COL_MSG	1001
#define RTTI_LABEL	1002

// warning : enums items order are linked with the ones in api.h
// and known by ToolCom.cpp

enum UmlCode {
  UmlAssociation, UmlDirectionalAssociation, UmlGeneralisation, 
  UmlAggregation, UmlAggregationByValue, UmlDirectionalAggregation,
  UmlDirectionalAggregationByValue, UmlDependency, UmlRealize,
  UmlRelations, // the first one just after the relations
  UmlAttribute, UmlOperation, UmlExtraMember,
  UmlClass,
  // all the previous ones are class members
  UmlUseCase, UmlComponent, UmlDeploymentNode, UmlArtifact,
  UmlSimpleRelations,	// mainly defined for api
  UmlClassDiagram, UmlUseCaseDiagram, UmlSeqDiagram, UmlColDiagram,
  UmlComponentDiagram, UmlDeploymentDiagram,
  UmlObjectDiagram, UmlActivityDiagram,	// api >= 24
  UmlClassView, UmlUseCaseView, UmlComponentView, UmlDeploymentView,
  UmlPackage,
  UmlState, UmlTransition, UmlRegion, UmlStateDiagram, UmlStateAction,
  InitialPS, EntryPointPS,	// warning : PS order used
  FinalPS, TerminatePS, ExitPointPS,
  DeepHistoryPS, ShallowHistoryPS, 
  JunctionPS, ChoicePS, ForkPS, JoinPS,
  UmlActivity, UmlFlow, UmlParameter, UmlParameterSet,
  UmlPartition, UmlExpansionRegion, UmlInterruptibleActivityRegion,
  UmlActivityAction,
  // only for plug-out exchange, api know the order ...
  umlOpaqueAction = UmlActivityAction, umlAcceptEventAction,
  umlReadVariableValueAction, umlClearVariableValueAction,
  umlWriteVariableValueAction,
  umlAddVariableValueAction, umlRemoveVariableValueAction,
  umlCallBehaviorAction, umlCallOperationAction,
  umlSendObjectAction, umlSendSignalAction, umlBroadcastSignalAction,
  umlUnmarshallAction, umlValueSpecificationAction,
  // ... end of only for plug-out exchange
  UmlActivityObject, UmlExpansionNode, UmlActivityPin, 
  InitialAN, FlowFinalAN, ActivityFinalAN,	// warning : ActivityNode order used
  DecisionAN, MergeAN, ForkAN, JoinAN,
  
  // the followings are unknown by api.h
  UmlDependOn, UmlInherit, UmlObjectLink,	// not between classes
  UmlArrowPoint,
  UmlNote, UmlAnchor, UmlLabel, UmlText,
  UmlSyncMsg, UmlAsyncMsg, UmlSyncSelfMsg, UmlAsyncSelfMsg, UmlReturnMsg,
  UmlLifeLine, UmlActivityDuration,
  UmlLink, UmlSelfLink, UmlLinkDirs,
  UmlContain, 
  UmlTemplate, UmlIcon,
  UmlProject, UmlSelect,
  UmlHub,
  UmlRequired, UmlProvided, UmlArrowJunction,
  UmlFragment, UmlContinuation,
  UmlPseudoState,		// usefull in some case
  UmlSignalIn,
  UmlSignalOut,
  UmlSubject,
  UmlActivityNode,		// usefull in some case
  UmlInfo,
  UmlFragmentSeparator,

  UmlCodeSup
};

#define IsaRelation(x) (x < UmlRelations)
#define IsaPseudoState(x) ((x >= InitialPS) && (x <= JoinPS))
#define IsaActivityNode(x) ((x >= InitialAN) && (x <= JoinAN))

// warning : only from plug-out
#define IsaActivityAction(x) ((x >= umlOpaqueAction) && (x <= umlValueSpecificationAction))

inline bool IsaActivityContainer(UmlCode k)
{
  switch (k) {
  case UmlActivity:
  case UmlExpansionRegion:
  case UmlInterruptibleActivityRegion:
    return TRUE;
  default:
    return FALSE;
  }
}

enum aCorner { NoCorner, TopLeft, TopRight, BottomLeft, BottomRight };

enum UmlVisibility { UmlPublic, UmlProtected, UmlPrivate, UmlPackageVisibility, UmlDefaultVisibility };

enum UmlParamDirection { UmlInOut, UmlIn, UmlOut, UmlReturn };

enum UmlParamEffect { UmlNoEffect, UmlCreate, UmlRead, UmlUpdate, UmlDelete };

enum UmlExpansionKind { UmlParallel, UmlIterative, UmlStream };

enum UmlOrderingKind { UmlUnordered, UmlOrdered, UmlLifo, UmlFifo };

// order known by ActivityActionDialog and api through UmlCode
// order equals umlXX in UmlCode
enum UmlActionKind {
  UmlOpaqueAction, UmlAcceptEventAction,
  UmlReadVariableValueAction, UmlClearVariableValueAction,
  UmlWriteVariableValueAction,
  UmlAddVariableValueAction, UmlRemoveVariableValueAction,
  UmlCallBehaviorAction, UmlCallOperationAction,
  UmlSendObjectAction, UmlSendSignalAction, UmlBroadcastSignalAction,
  UmlUnmarshallAction, UmlValueSpecificationAction
};		     

enum Uml3States { UmlYes, UmlNo, UmlDefaultState };

enum UmlColor { 
  UmlTransparent,
  UmlWhite,
  UmlLightYellow,
  UmlYellow,
  UmlMediumYellow,
  UmlDarkYellow,
  UmlLightBlue,
  UmlBlue,
  UmlMediumBlue,
  UmlDarkBlue,
  UmlLightGreen,
  UmlGreen,
  UmlMediumGreen,
  UmlDarkGreen,
  UmlLightRed,
  UmlMidRed,
  UmlRed,
  UmlVeryLightOrange,
  UmlLightOrange,
  UmlOrange,
  UmlDarkOrange,
  UmlLightMagenta,
  UmlMagenta,
  UmlMidMagenta,
  UmlDarkMagenta,
  UmlLightGray,
  UmlGray,
  UmlDefaultColor
};

enum ClassDrawingMode { asClass, asControl, asBoundary, asEntity, asActor, Natural, DefaultClassDrawingMode };

enum DrawingLanguage { UmlView, CppView, JavaView, IdlView, DefaultDrawingLanguage };

enum ShowContextMode { noContext, umlContext, namespaceContext, packageContext, moduleContext, DefaultShowContextMode };

enum UmlFont {
  UmlSmallFont, UmlSmallItalicFont, UmlSmallBoldFont, UmlSmallBoldItalicFont, 
  UmlNormalFont, UmlNormalItalicFont, UmlNormalBoldFont, UmlNormalBoldItalicFont, 
  UmlNormalUnderlinedFont, UmlNormalStrikeOutFont,
  UmlLargeFont, UmlLargeItalicFont, UmlLargeBoldFont, UmlLargeBoldItalicFont,
  UmlFontNumber
};

enum CanvasFormat {
  IsoA0, IsoA1, IsoA2, IsoA3, IsoA4, IsoA5,
  UsA, UsB, UsC, UsD, UsE,
  IsoA0Landscape, IsoA1Landscape, IsoA2Landscape,
  IsoA3Landscape, IsoA4Landscape, IsoA5Landscape,
  UsALandscape, UsBLandscape, UsCLandscape,
  UsDLandscape, UsELandscape,
  CanvasFormatSup
};

enum LineGeometry {
  NoGeometry,
  HVGeometry, HVrGeometry,
  VHGeometry, VHrGeometry,
  HVHGeometry,
  VHVGeometry,
  GeometrySup
};

extern const char * stringify(UmlVisibility);
extern const char * stringify(UmlParamDirection);
extern const char * stringify(UmlCode);	// lines only
extern const char * stringify(Uml3States);
extern const char * stringify(UmlColor);
extern const char * stringify(UmlFont);
extern const char * stringify(ClassDrawingMode);
extern const char * stringify(DrawingLanguage);
extern const char * stringify(ShowContextMode);
extern const char * stringify(CanvasFormat);
extern const char * stringify(LineGeometry);
extern const char * stringify(UmlParamEffect);
extern const char * stringify(UmlActionKind);
extern const char * stringify(UmlExpansionKind);
extern const char * stringify(UmlOrderingKind);
extern UmlCode arrow_type(const char *);
extern bool IsaSimpleRelation(UmlCode);
extern UmlVisibility visibility(const char * s);
extern UmlCode relation_type(const char * s,
			     bool simple_relation = FALSE);
extern UmlParamDirection direction(const char * s);
extern UmlParamEffect effect(const char * s);
extern Uml3States state(const char * s);
extern UmlColor color(const char * s);
extern UmlFont font(const char * s);
extern ClassDrawingMode drawing_mode(const char * s);
extern DrawingLanguage drawing_language(const char * s);
extern ShowContextMode context_mode(const char * s);
extern CanvasFormat canvas_format(const char * s);
extern UmlCode pseudo_state_kind(const char * s);
extern LineGeometry line_geometry(const char * s);
extern UmlCode activity_node_kind(const char * s);
extern UmlActionKind activity_action_kind(const char * s);
extern UmlExpansionKind expansion_mode_kind(const char * s);
extern UmlOrderingKind ordering(const char * s);

#endif
