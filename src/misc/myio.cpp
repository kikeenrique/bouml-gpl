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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <stdlib.h>
#include <qtextstream.h> 
#include <qdir.h>
#include <qfile.h>
#include <qstring.h>
#include <qcanvas.h>
#include <qmultilineedit.h>
#include <qtextview.h> 
#include <qlayout.h> 
#include <qdialog.h>
#include <qlabel.h>
#include <qfont.h>
#include <qworkspace.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <qbuffer.h>

#include "myio.h"
#include "UmlWindow.h"
#include "BrowserView.h"
#include "Labeled.h"
#include "strutil.h"
#include "DialogUtil.h"
#include "mu.h"

static ReadContext Context;
static bool ReadOnlyForced;

static int Indent;

ReadContext current_context()
{
  return Context;
}

void restore_context(ReadContext & c)
{
  Context = c;
}

bool read_only_file()
{
  return Context.read_only;
}

void set_read_only_file()
{
  Context.read_only = TRUE;
}

void force_read_only(bool y)
{
  ReadOnlyForced = y;
}

const char * stringify(UmlVisibility v) {
  switch(v) {
  case UmlPublic:
    return "public";
  case UmlProtected:
    return "protected";
  case UmlPrivate:
    return "private";
  case UmlPackageVisibility:
    return "package";
  case UmlDefaultVisibility:
    return "default";
  default:
    return "WRONG VISIBILITY";
  }
}

const char * stringify(UmlParamDirection d) {
  switch(d) {
  case UmlInOut:
    return "inout";
  case UmlIn:
    return "in";
  case UmlOut:
    return "out";
  default:
    return "return";
  }
}

const char * stringify(UmlParamEffect e) {
  switch(e) {
  case UmlNoEffect:
    return "none";
  case UmlCreate:
    return "create";
  case UmlRead:
    return "read";
  case UmlUpdate:
    return "update";
  case UmlDelete:
    return "delete";

  default:
    return "WRONG CODE";
  }
}

const char * stringify(UmlActionKind a)
{
  switch(a) {
  case UmlOpaqueAction:
    return "opaque_action";
  case UmlAcceptEventAction:
    return "accept_event_action";
  case UmlReadVariableValueAction:
    return "read_variable_value_action";
  case UmlClearVariableValueAction:
    return "clear_variable_value_action";
  case UmlWriteVariableValueAction:
    return "write_variable_value_action";
  case UmlAddVariableValueAction:
    return "add_variable_value_action";
  case UmlRemoveVariableValueAction:
    return "remove_variable_value_action";
  case UmlCallBehaviorAction:
    return "call_behavior_action";
  case UmlCallOperationAction:
    return "call_operation_action";
  case UmlSendObjectAction:
    return "send_object_action";
  case UmlSendSignalAction:
    return "send_signal_action";
  case UmlBroadcastSignalAction:
    return "broadcast_signal_action";
  case UmlUnmarshallAction:
    return "unmarshall_action";
  case UmlValueSpecificationAction:
    return "value_specification_action";

  default:
    return "WRONG CODE";
  }
}

const char * stringify(UmlExpansionKind k)
{
  switch(k) {
  case UmlParallel:
    return "parallel";
  case UmlIterative:
    return "iterative";
  case UmlStream:
    return "stream";
    
  default:
    return "WRONG CODE";
  }
}

const char * stringify(UmlOrderingKind k)
{
  switch (k) {
  case UmlUnordered:
    return "unordered";
  case UmlOrdered:
    return "ordered";
  case UmlLifo:
    return "lifo";
  case UmlFifo:
    return "fifo";
    
  default:
    return "WRONG CODE";
  }
}


const char * stringify(UmlCode c) {
  switch (c) {
  case UmlAssociation:
    return "----";
  case UmlDirectionalAssociation:
    return "--->";
  case UmlGeneralisation:
  case UmlInherit:
    return "---|>";
  case UmlAggregation:
    return "o---";
  case UmlAggregationByValue:
    return "*---";
  case UmlDirectionalAggregation:
    return "o-->";
  case UmlDirectionalAggregationByValue:
    return "*-->";
  case UmlDependency:
  case UmlDependOn:
    return "-_->";
  case UmlRealize:
    return "-_-|>";
  case UmlAnchor:
    return "-_-_";
  case UmlProvided:
    return "---O";
  case UmlRequired:
    return "---(";

  case InitialPS:
    return "initial";
  case EntryPointPS:
    return "entry_point";
  case FinalPS:
    return "final";
  case TerminatePS:
    return "terminate";
  case ExitPointPS:
    return "exit_point";
  case DeepHistoryPS:
    return "deep_history";
  case ShallowHistoryPS:
    return "shallow_history";
  case JunctionPS:
    return "junction";
  case ChoicePS:
    return "choice";
  case ForkPS:
    return "fork";
  case JoinPS:
    return "join";
    
  case InitialAN:
    return "initial";
  case FlowFinalAN:
    return "flow_final";
  case ActivityFinalAN:
    return "activity_final";
  case DecisionAN:
    return "decision";
  case MergeAN:
    return "merge";
  case ForkAN:
    return "fork";
  case JoinAN:
    return "join";

  case UmlActivityAction:     // for InfoCanvas
    return "activity action";
  case UmlFlow:		      // for InfoCanvas
    return "flow";

  default:
    return "WRONG CODE";
  }
}

const char * stringify(Uml3States s)
{
  switch (s) {
  case UmlYes:
    return "yes";
  case UmlNo:
    return "no";
  case UmlDefaultState:
    return "default";
  default:
    return "WRONG 3STATES";
  }
}

const char * stringify(UmlColor c)
{
  switch (c) {
  case UmlTransparent:
    return "transparent";
  case UmlWhite:
    return "white";
  case UmlLightYellow:
    return "lightyellow";
  case UmlYellow:
    return "yellow";
  case UmlMediumYellow:
    return "mediumyellow";
  case UmlDarkYellow:
    return "gold";
  case UmlLightBlue:
    return "lightblue";
  case UmlBlue:
    return "blue";
  case UmlMediumBlue:
    return "mediumblue";
  case UmlDarkBlue:
    return "darkblue";
  case UmlLightGreen:
    return "lightgreen";
  case UmlGreen:
    return "green";
  case UmlMediumGreen:
    return "mediumgreen";
  case UmlDarkGreen:
    return "darkgreen";
  case UmlRed:
    return "red";
  case UmlLightGray:
    return "lightgray";
  case UmlGray:
    return "gray";
    
  case UmlLightRed:
    return "lightred";
  case UmlMidRed:
    return "midred";
  case UmlVeryLightOrange:
    return "verylightorange";
  case UmlLightOrange:
    return "lightorange";
  case UmlOrange:
    return "orange";
  case UmlDarkOrange:
    return "darkorange";
  case UmlLightMagenta:
    return "lightmagenta";
  case UmlMagenta:
    return "magenta";
  case UmlMidMagenta:
    return "midmagenta";
  case UmlDarkMagenta:
    return "darkmagenta";
    
  case UmlDefaultColor:
    return "default";
  default:
    return "WRONG COLOR";    
  }
}

const char * stringify(UmlFont f)
{
  switch (f) {
  case UmlNormalFont:
    return "normal";
  case UmlNormalBoldFont:
    return "bold";
  case UmlNormalItalicFont:
    return "italic";
  case UmlNormalBoldItalicFont:
    return "bold_italic";
  case UmlSmallFont:
    return "small";
  case UmlSmallBoldFont:
    return "small_bold";
  case UmlSmallItalicFont:
    return "small_italic";
  case UmlSmallBoldItalicFont:
    return "small_bold_italic";
  case UmlLargeFont:
    return "large";
  case UmlLargeBoldFont:
    return "large_bold";
  case UmlLargeItalicFont:
    return "large_italic";
  case UmlLargeBoldItalicFont:
    return "large_bold_italic";
  case UmlNormalUnderlinedFont:
    return "underlined";
  case UmlNormalStrikeOutFont:
    return "strikeout";
  default:
    return "WRONG FONT";    
  }
}

const char * stringify(ClassDrawingMode v)
{
  switch (v) {
  case asClass:
    return "class";
  case asControl:
    return "control";
  case asBoundary:
    return "boundary";
  case asEntity:
    return "entity";
  case asActor:
    return "actor";
  case Natural:
    return "natural";
  case DefaultClassDrawingMode:
    return "default";
  default:
    return "WRONG CLASSDRAWINGMODE";
  }
}

const char * stringify(DrawingLanguage l)
{
  switch (l) {
  case UmlView:
    return "uml";
  case CppView:
    return "c++";
  case JavaView:
    return "java";
  case IdlView:
    return "idl";
  case DefaultDrawingLanguage:
    return "default";
  default:
    return "WRONG DRAWINGLANGUAGE";
  }
}

const char * stringify(ShowContextMode v)
{
  switch (v) {
  case noContext:
    return "no";
  case umlContext:
    return "uml";
  case namespaceContext:
    return "namespace";
  case packageContext:
    return "package";
  case moduleContext:
    return "module";
  case DefaultShowContextMode:
    return "default";
  default:
    return "WRONG CLASSVIEWMODE";
  }
}

const char * stringify(CanvasFormat v)
{
  switch (v) {
  case IsoA0: 
    return "A0";
  case IsoA1: 
    return "A1";
  case IsoA2: 
    return "A2";
  case IsoA3: 
    return "A3";
  case IsoA4: 
    return "A4";
  case IsoA5: 
    return "A5";
  case UsA: 
    return "A";
  case UsB: 
    return "B";
  case UsC: 
    return "C";
  case UsD: 
    return "D";
  case UsE: 
    return "E";
  case IsoA0Landscape: 
    return "A0-Landscape";
  case IsoA1Landscape: 
    return "A1-Landscape";
  case IsoA2Landscape: 
    return "A2-Landscape";
  case IsoA3Landscape: 
    return "A3-Landscape";
  case IsoA4Landscape: 
    return "A4-Landscape";
  case IsoA5Landscape: 
    return "A5-Landscape";
  case UsALandscape: 
    return "A-Landscape";
  case UsBLandscape: 
    return "B-Landscape";
  case UsCLandscape: 
    return "C-Landscape";
  case UsDLandscape: 
    return "D-Landscape";
  case UsELandscape: 
    return "E-Landscape";
  default:
    return "WRONG CanvasFormat";
  }
}

const char * stringify(LineGeometry v)
{
  switch (v) {
  case HVGeometry:
    return "HV";
  case HVrGeometry:
    return "HVr";
  case VHGeometry:
    return "VH";
  case VHrGeometry:
    return "VHr";
  case HVHGeometry:
    return "HVH";
  case VHVGeometry:
    return "VHV";
  default:
    return "WRONG LineGeometry";
  }
}

UmlCode arrow_type(const char * s)
{
  if (!strcmp(s, "---|>"))
    return UmlGeneralisation;
  if (!strcmp(s, "----"))
    return UmlAssociation;
  if (!strcmp(s, "--->"))
    return UmlDirectionalAssociation;
  if (!strcmp(s, "-_->"))
    return UmlDependency;
  if (!strcmp(s, "-_-_"))
    return UmlAnchor;
  if (!strcmp(s, "---("))
    return UmlRequired;
  if (!strcmp(s, "---O"))
    return UmlProvided;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a line kind");
  throw 0;
}

UmlCode relation_type(const char * s, bool simple_relation)
{
  if (!strcmp(s, "----"))
    return UmlAssociation;
  if (!strcmp(s, "--->"))
    return UmlDirectionalAssociation;
  if (!strcmp(s, "---|>"))
    return (simple_relation) 
      ? UmlInherit
      : UmlGeneralisation;
  if (!strcmp(s, "o---"))
    return UmlAggregation;
  if (!strcmp(s, "*---"))
    return UmlAggregationByValue;
  if (!strcmp(s, "<>-->") || !strcmp(s, "o-->"))
    return UmlDirectionalAggregation;
  if (!strcmp(s, "<#>-->") || !strcmp(s, "*-->"))
    return UmlDirectionalAggregationByValue;
  if (!strcmp(s, "-_->"))
    return (simple_relation) 
      ? UmlDependOn
      : UmlDependency;
  if (!strcmp(s, "-_-|>"))
    return UmlRealize;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a relation kind");
  throw 0;
}

bool IsaSimpleRelation(UmlCode c)
{
  switch (c) {
  case UmlDependOn:
  case UmlInherit:
    return TRUE;
  default:
    return FALSE;
  }
}

UmlVisibility visibility(const char * s)
{
  if (!strcmp(s, "public"))
    return UmlPublic;
  else if (!strcmp(s, "protected"))
    return UmlProtected;
  else if (!strcmp(s, "private"))
    return UmlPrivate;
  else if (!strcmp(s, "package"))
    return UmlPackageVisibility;
  else if (!strcmp(s, "default"))
    return UmlDefaultVisibility;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a visibility");
  throw 0;
}

UmlParamDirection direction(const char * s)
{
  if (!strcmp(s, "inout"))
    return UmlInOut;
  if (!strcmp(s, "in"))
    return UmlIn;
  if (!strcmp(s, "out"))
    return UmlOut;
  if (!strcmp(s, "return"))
    return UmlReturn;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a direction");
  throw 0;
}

UmlParamEffect effect(const char * s)
{
  if (!strcmp(s, "none"))
    return UmlNoEffect;
  if (!strcmp(s, "create"))
    return UmlCreate;
  if (!strcmp(s, "read"))
    return UmlRead;
  if (!strcmp(s, "update"))
    return UmlUpdate;
  if (!strcmp(s, "delete"))
    return UmlDelete;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a parameter effect");
  throw 0;
}

UmlActionKind activity_action_kind(const char * s)
{
  if (!strcmp(s, "opaque_action"))
    return UmlOpaqueAction;
  if (!strcmp(s, "accept_event_action"))
    return UmlAcceptEventAction;
  if (!strcmp(s, "read_variable_value_action"))
    return UmlReadVariableValueAction;
  if (!strcmp(s, "clear_variable_value_action"))
    return UmlClearVariableValueAction;
  if (!strcmp(s, "write_variable_value_action"))
    return UmlWriteVariableValueAction;
  if (!strcmp(s, "add_variable_value_action"))
    return UmlAddVariableValueAction;
  if (!strcmp(s, "remove_variable_value_action"))
    return UmlRemoveVariableValueAction;
  if (!strcmp(s, "call_behavior_action"))
    return UmlCallBehaviorAction;
  if (!strcmp(s, "call_operation_action"))
    return UmlCallOperationAction;
  if (!strcmp(s, "send_object_action"))
    return UmlSendObjectAction;
  if (!strcmp(s, "send_signal_action"))
    return UmlSendSignalAction;
  if (!strcmp(s, "broadcast_signal_action"))
    return UmlBroadcastSignalAction;
  if (!strcmp(s, "unmarshall_action"))
    return UmlUnmarshallAction;
  if (!strcmp(s, "value_specification_action"))
    return UmlValueSpecificationAction;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not an activity action kind");
  throw 0;
}

UmlExpansionKind expansion_mode_kind(const char * s)
{
  if (! strcmp(s, "parallel"))
    return UmlParallel;
  if (! strcmp(s, "iterative"))
    return UmlIterative;
  if (! strcmp(s, "stream"))
    return UmlStream;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not an expansion node kind");
  throw 0;
}

UmlOrderingKind ordering(const char * s)
{
  if (! strcmp(s, "unordered"))
    return UmlUnordered;
  if (! strcmp(s, "ordered"))
    return UmlOrdered;
  if (! strcmp(s, "lifo"))
    return UmlLifo;
  if (! strcmp(s, "fifo"))
    return UmlFifo;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not an ordering");
  throw 0;
}

Uml3States state(const char * s)
{
  if (!strcmp(s, "yes"))
    return UmlYes;
  if (!strcmp(s, "no"))
    return UmlNo;
  if (!strcmp(s, "default"))
    return UmlDefaultState;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a state");
  throw 0;
}

ClassDrawingMode drawing_mode(const char * s)
{
  if (!strcmp(s, "class"))
    return asClass;
  if (!strcmp(s, "control"))
    return asControl;
  if (!strcmp(s, "boundary"))
    return asBoundary;
  if (!strcmp(s, "entity"))
    return asEntity;
  if (!strcmp(s, "actor"))
    return asActor;
  if (!strcmp(s, "natural"))
    return Natural;
  if (!strcmp(s, "default"))
    return DefaultClassDrawingMode;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a class drawing mode");
  throw 0;
}

DrawingLanguage drawing_language(const char * s)
{
  if (!strcmp(s, "uml"))
    return UmlView;
  if (!strcmp(s, "c++"))
    return CppView;
  if (!strcmp(s, "java"))
    return JavaView;
  if (!strcmp(s, "idl"))
    return IdlView;
  if (!strcmp(s, "default"))
    return DefaultDrawingLanguage;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a drawing language");
  throw 0;
}

ShowContextMode context_mode(const char * s)
{
  if (!strcmp(s, "no"))
    return noContext;
  if (!strcmp(s, "uml"))
    return umlContext;
  if (!strcmp(s, "namespace"))
    return namespaceContext;
  if (!strcmp(s, "package"))
    return packageContext;
  if (!strcmp(s, "module"))
    return moduleContext;
  if (!strcmp(s, "default"))
    return DefaultShowContextMode;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a context mode");
  throw 0;
}

UmlColor color(const char * s)
{
  if (!strcmp(s, "transparent"))
    return UmlTransparent;
  if (!strcmp(s, "white"))
    return UmlWhite;
  if (!strcmp(s, "lightyellow"))
    return UmlLightYellow;
  if (!strcmp(s, "yellow"))
    return UmlYellow;
  if (!strcmp(s, "mediumyellow"))
    return UmlMediumYellow;
  if (!strcmp(s, "gold"))
    return UmlDarkYellow;
  if (!strcmp(s, "lightblue"))
    return UmlLightBlue;
  if (!strcmp(s, "mediumblue"))
    return UmlMediumBlue;
  if (!strcmp(s, "blue"))
    return UmlBlue;
  if (!strcmp(s, "darkblue"))
    return UmlDarkBlue;
  if (!strcmp(s, "lightgreen"))
    return UmlLightGreen;
  if (!strcmp(s, "green"))
    return UmlGreen;
  if (!strcmp(s, "mediumgreen"))
    return UmlMediumGreen;
  if (!strcmp(s, "darkgreen"))
    return UmlDarkGreen;
  if (! strcmp(s, "lightred"))
    return UmlLightRed;
  if (! strcmp(s, "midred"))
    return UmlMidRed;
  if (!strcmp(s, "red"))
    return UmlRed;
  if (!strcmp(s, "lightgray"))
    return UmlLightGray;
  if (!strcmp(s, "gray"))
    return UmlGray;
  
  if (! strcmp(s, "verylightorange"))
    return UmlVeryLightOrange;
  if (! strcmp(s, "lightorange"))
    return UmlLightOrange;
  if (! strcmp(s, "orange"))
    return UmlOrange;
  if (! strcmp(s, "darkorange"))
    return UmlDarkOrange;
  if (! strcmp(s, "lightmagenta"))
    return UmlLightMagenta;
  if (! strcmp(s, "magenta"))
    return UmlMagenta;
  if (! strcmp(s, "midmagenta"))
    return UmlMidMagenta;
  if (! strcmp(s, "darkmagenta"))
    return UmlDarkMagenta;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a color");
  throw 0;
}

UmlFont font(const char * s)
{
  if (!strcmp(s, "normal"))
    return UmlNormalFont;
  if (!strcmp(s, "bold"))
    return UmlNormalBoldFont;
  if (!strcmp(s, "italic"))
    return UmlNormalItalicFont;
  if (!strcmp(s, "bold_italic"))
    return UmlNormalBoldItalicFont;
  if (!strcmp(s, "small"))
    return UmlSmallFont;
  if (!strcmp(s, "small_bold"))
    return UmlSmallBoldFont;
  if (!strcmp(s, "small_italic"))
    return UmlSmallItalicFont;
  if (!strcmp(s, "small_bold_italic"))
    return UmlSmallBoldItalicFont;
  if (!strcmp(s, "large"))
    return UmlLargeFont;
  if (!strcmp(s, "large_bold"))
    return UmlLargeBoldFont;
  if (!strcmp(s, "large_italic"))
    return UmlLargeItalicFont;
  if (!strcmp(s, "large_bold_italic"))
    return UmlLargeBoldItalicFont;
  if (!strcmp(s, "underlined"))
    return UmlNormalUnderlinedFont;
  if (!strcmp(s, "strikeout"))
    return UmlNormalStrikeOutFont;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a font");
  throw 0;
}

CanvasFormat canvas_format(const char * s)
{
  if (! strcmp(s, "A0"))
    return IsoA0;
  if (! strcmp(s, "A1"))
    return IsoA1;
  if (! strcmp(s, "A2"))
    return IsoA2;
  if (! strcmp(s, "A3"))
    return IsoA3;
  if (! strcmp(s, "A4"))
    return IsoA4;
  if (! strcmp(s, "A5"))
    return IsoA5;
  if (! strcmp(s, "A"))
    return UsA;
  if (! strcmp(s, "B"))
    return UsB;
  if (! strcmp(s, "C"))
    return UsC;
  if (! strcmp(s, "D"))
    return UsD;
  if (! strcmp(s, "E"))
    return UsE;
  
  if (! strcmp(s, "A0-Landscape"))
    return IsoA0Landscape;
  if (! strcmp(s, "A1-Landscape"))
    return IsoA1Landscape;
  if (! strcmp(s, "A2-Landscape"))
    return IsoA2Landscape;
  if (! strcmp(s, "A3-Landscape"))
    return IsoA3Landscape;
  if (! strcmp(s, "A4-Landscape"))
    return IsoA4Landscape;
  if (! strcmp(s, "A5-Landscape"))
    return IsoA5Landscape;
  if (! strcmp(s, "A-Landscape"))
    return UsALandscape;
  if (! strcmp(s, "B-Landscape"))
    return UsBLandscape;
  if (! strcmp(s, "C-Landscape"))
    return UsCLandscape;
  if (! strcmp(s, "D-Landscape"))
    return UsDLandscape;
  if (! strcmp(s, "E-Landscape"))
    return UsELandscape;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a canvas format");
  throw 0;
}

UmlCode pseudo_state_kind(const char * s)
{
  if (! strcmp(s, "initial"))
    return InitialPS;
  if (! strcmp(s, "entry_point"))
    return EntryPointPS;
  if (! strcmp(s, "final"))
    return FinalPS;
  if (! strcmp(s, "terminate"))
    return TerminatePS;
  if (! strcmp(s, "exit_point"))
    return ExitPointPS;
  if (! strcmp(s, "deep_history"))
    return DeepHistoryPS;
  if (! strcmp(s, "shallow_history"))
    return ShallowHistoryPS;
  if (! strcmp(s, "junction"))
    return JunctionPS;
  if (! strcmp(s, "choice"))
    return ChoicePS;
  if (! strcmp(s, "fork"))
    return ForkPS;
  if (! strcmp(s, "join"))
    return JoinPS;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a pseudo state kind");
  throw 0;
}

UmlCode activity_node_kind(const char * s)
{
  if (! strcmp(s, "initial"))
    return InitialAN;
  if (! strcmp(s, "flow_final"))
    return FlowFinalAN;
  if (! strcmp(s, "activity_final"))
    return ActivityFinalAN;
  if (! strcmp(s, "decision"))
    return DecisionAN;
  if (! strcmp(s, "merge"))
    return MergeAN;
  if (! strcmp(s, "fork"))
    return ForkAN;
  if (! strcmp(s, "join"))
    return JoinAN;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not an activity node kind");
  throw 0;
}

LineGeometry line_geometry(const char * s)
{
  if (! strcmp(s, "HV"))
    return HVGeometry;
  if (! strcmp(s, "HVr"))
    return HVrGeometry;
  if (! strcmp(s, "VH"))
    return VHGeometry;
  if (! strcmp(s, "VHr"))
    return VHrGeometry;
  if (! strcmp(s, "HVH"))
    return HVHGeometry;
  if (! strcmp(s, "VHV"))
    return VHVGeometry;
  
  msg_critical("Error",
	       Context.filename + " : " + s + " is not a line geometry");
  throw 0;
}


//

void set_read_file_format(unsigned u)
{
  Context.read_file_format = u;
}

unsigned read_file_format()
{
  return Context.read_file_format;
}

//

static void set_previous_word(char * s)
{
  if (Context.previous_word_allocated) {
    Context.previous_word_allocated = FALSE;
    delete [] Context.previous_word;
  }
  Context.previous_word = s;
}

void backup(QDir & d, QString fn) 
{
  QString s;

  s.sprintf("_%u.bak", user_id());

  while (d.exists(fn) &&
	 !d.rename(fn, fn + s) &&
	 (msg_warning("Uml",
		      d.absFilePath(fn) + 
		      "\ncannot be renamed " + fn + s + 
		      ", retry ?\n",
		      QMessageBox::Yes, QMessageBox::No)
	  == QMessageBox::Yes))
    ;
}

void delete_backup(QDir & d)
{
  QString s;

  s.sprintf("*_%u.bak", user_id());

  const QFileInfoList * l = d.entryInfoList(s);
  
  if (l) {
    QListIterator<QFileInfo> it(*l);
    QFileInfo *fi;
    
    while ((fi = it.current()) != 0) {
      QFile::remove(fi->absFilePath());
      ++it;
    }
  }
}

int open_file(QFile & fp, int mode, bool silent)
{
  Context.filename = fp.name();
  
  while (! fp.open(mode)) {
    if (silent ||
	(msg_critical("Uml",
		      Context.filename +
		      ((mode != IO_ReadOnly)
		       ? "\ncannot be openned for write, retry ?\n"
		       : "\ncannot be openned for read, retry ?\n"),
		      QMessageBox::Yes, QMessageBox::No)
	 != QMessageBox::Yes))
      return -1;
  }
  
  if (mode == IO_ReadOnly) {
    set_previous_word("");
    Context.linenum = 1;
    Context.removed_char = ' ';
  }
  
  QFileInfo fi(fp);
  
  Context.read_only = !fi.isWritable() || ReadOnlyForced;
  return fi.size();
}

bool copy_file(QFileInfo * src, const QDir & dest)
{
  QString src_path = src->absFilePath();
  FILE * fp_src;
  
  if ((fp_src = fopen((const char *) src_path, "rb")) == 0) {
    QString err;
    
    err.sprintf("cannot open %s to read it", (const char *) src_path);
    
    msg_critical("Error", err);
    return FALSE;
  }

  QString dest_path = dest.absFilePath(src->fileName());
  FILE * fp_dest;
    
  if ((fp_dest = fopen((const char *) dest_path, "wb")) == 0) {
    QString err;
    
    err.sprintf("cannot create %s", (const char *) dest_path);
    
    msg_critical("Error", err);
    fclose(fp_src);
    return FALSE;
  }
  
  char b[4096];
  size_t n;

  while ((n = fread(b, 1, sizeof(b) - 1, fp_src)) > 0) {
    if (fwrite(b, 1, n, fp_dest) != n) {
      QString err;
      
      err.sprintf("error when writting %s", (const char *) dest_path);
      
      msg_critical("Error", err);
      fclose(fp_src);
      fclose(fp_dest);
      return FALSE;
    }
  }
  
  if (! feof(fp_src)) {
    QString err;
    
    err.sprintf("error when reading %s", (const char *) src_path);
    
    msg_critical("Error", err);
    fclose(fp_src);
    fclose(fp_dest);
    return FALSE;
  }

  fclose(fp_src);
  fclose(fp_dest);
  return TRUE;
}

void save_if_needed(const char * filename, const char * newdef)
{
  QDir d = BrowserView::get_dir();
  QString path = BrowserView::get_dir().absFilePath(filename);
  FILE * fp = fopen((const char *) path, "rb");
  bool needed;
   
  if (fp == 0)
    needed = TRUE;
  else {
    int c;
    const char * new_contains = newdef;
    
    needed = FALSE;
    
    while ((c = fgetc(fp)) != EOF) {
      if (c != *new_contains++) {
	needed = TRUE;
	break;
      }
    }
    
    fclose(fp);
  
    needed |= (*new_contains != 0);
  }

  if (needed) {
    backup(d, filename);
    
    if ((fp = fopen((const char *) path, "wb")) == 0)
      msg_warning("Uml",
			   path + " can't be modified");
    else {
      fputs(newdef, fp);
      fclose(fp);
    }
  }
}

char * read_file(QString filename)
{
  filename = (in_import())
    ? BrowserView::get_import_dir().absFilePath(filename)
    : BrowserView::get_dir().absFilePath(filename);
  
  QFile fp(filename);
  int size;
  
  if ((size = open_file(fp, IO_ReadOnly, TRUE)) != -1) {
    char * s = new char[size + 1];
    
    if (fp.readBlock(s, size) == -1) {
      msg_critical("Error", filename + "cannot be read");
      delete [] s;
      return 0;
    }
    else {
      s[size] = 0;
      
      return s;
    }
  }
  else
    return 0;
}

QString abs_file_path(int id, const char * ext)
{
  QString s;
  
  if (ext[1] == 0)
    s.sprintf("%d_%d.%c", id,
	      root_permission() ? root_permission() : user_id(),
	      *ext);
  else
    s.sprintf("%d.%s", id, ext);
  
  return BrowserView::get_dir().absFilePath(s);
}

char * read_definition(int id, const char * ext)
{
  QString s;
  char * r;
  
  // try to read modified definition
  
  s.sprintf("%d_%d.%c", id,
	    root_permission() ? root_permission() : user_id(),
	    *ext);
  if ((r = read_file(s)) != 0)
    return r;
  
  if (ext[1] == 0)
    // want the modified definition
    return 0;
    
  // try to read saved definition
  s.sprintf("%d.%s", id, ext);

  return read_file(s);
}

char * read_file(QString filename, int offset, int len)
{
  filename = BrowserView::get_dir().absFilePath(filename);
  
  QFile fp(filename);
  
  if (open_file(fp, IO_ReadOnly, TRUE) != -1) {
    char * s = new char[len + 1];
    
    fp.at(offset);
    if (fp.readBlock(s, len) == -1) {
      msg_critical("Error", filename + "cannot be read");
      delete [] s;
      return 0;
    }
    else {
      s[len] = 0;
      
      return s;
    }
  }
  else
    return 0;
}

char * read_definition(int id, const char * ext, int offset, int len)
{
  if (len == 0)
    return 0;
  
  QString s;
  char * r;
  
  // try to read modified definition
  
  s.sprintf("%d_%d.%c", id,
	    root_permission() ? root_permission() : user_id(),
	    *ext);
  if ((r = read_file(s, offset, len)) != 0)
    return r;
  
  if (ext[1] == 0)
    // want the modified definition
    return 0;
    
  // try to read saved definition
  s.sprintf("%d.%s", id, ext);

  return read_file(s, offset, len);
}

void save_definition(int id, const char * ext, const char * def,
		     bool & is_new)
{
  QString s;
  FILE * fp;
  QDir d = BrowserView::get_dir();
  
  if (ext[1] != 0) {
    // saved definition
    s.sprintf("%d.%s", id, ext);
    if (is_new)
      is_new = FALSE;
    else
      backup(d, s);
  }
  else
    // modified definition
    s.sprintf("%d_%d.%s", id,
	      root_permission() ? root_permission() : user_id(),
	      ext);
  
  s = d.absFilePath(s);
  
  while ((fp = fopen((const char *) s, "wb")) == 0)
    if (msg_critical("Error", QString("Cannot save definition in\n") + s,
		     QMessageBox::Retry, QMessageBox::Abort)
	== QMessageBox::Abort) {
    if (ext[1] != 0)
      exit(1);
    else
      return;
  }

  if ((def != 0) && *def)
    fputs(def, fp);
  
  fclose(fp);
}

void delete_definition(int id, const char * ext)
{
  QDir d = BrowserView::get_dir();
  QString s;
  
  if (ext[1] != 0) {
    // saved definition
    s.sprintf("%d.%s", id, ext);
    backup(d, s);
  }
  else {
    // modified definition
    s.sprintf("%d_%d.%s", id, user_id(), ext);
    QFile::remove(d.absFilePath(s));
  }
}

void read_in(const QString & s)
{
  set_previous_word("");
  Context.linenum = 1;
  Context.removed_char = ' ';
  Context.filename = s;
}

static QString where()
{
  return Context.filename + " line " + QString::number(Context.linenum) + "\n";
}

//

void save_string(const char * p, QTextStream & st)
{
  if ((p == 0) || (*p == 0))
    st << "\"\"";
  else {
    st << '"';
    
    for (;;) {
      const char * p2 = strchr(p, '"');
      const char * p3 = strchr(p, '\\');
      
      if ((p2 != 0) || (p3 != 0)) {
	if ((p2 == 0) || ((p3 != 0) && (p3 < p2)))
	  p2 = p3;
	// do NOT use writeRawBytes !
	while (p != p2)
	  st << *p++;
	st << '\\';
	st << *p++;
	if (*p == 0)
	  break;
      }
      else {
	st << p;
	break;
      }
    }
    
    st << '"';
  }
}
      
void save_string_list(QStringList & list, QTextStream & st)
{
  st << ' ' << list.count();
  
  for (QStringList::Iterator it = list.begin();
       it != list.end();
       ++it ) {
    st << ' ';
    save_string(*it, st);
  }
}

void save_unicode_string_list(QStringList & list, QTextStream & st)
{
  st << ' ' << list.count();
  
  for (QStringList::Iterator it = list.begin();
       it != list.end();
       ++it ) {
    st << ' ';
    save_string(fromUnicode(*it), st);
  }
}

void nl_indent(QTextStream & st) {
  int i = Indent;
  
  st << '\n';
  
  while (i >= 4) {
    st << '\t';
    i -= 4;
  }
  
  static const char * const ss[] = { "", "  ", "    ", "      " };
  
  st << ss[i];
}

void indent(int d)
{
  Indent += d;
}

int indent()
{
  return Indent;
}

void indent0()
{
  Indent = 0;
}

//

static void skip_whitespace_comment(char * & s)
{
  if (Context.removed_char != ' ')
    *--s = Context.removed_char;
  
  for (;;) {
    switch (*s) {
    case '\n':
      Context.linenum += 1;
    case ' ':
    case '\r':
    case '\t':
      s += 1;
      break;
    case '/':
      for (s += 2; *s != '\n'; s += 1)
	if (*s == 0)
	  return;
      Context.linenum += 1;
      s += 1;
      break;
    default:
      return;
    }
  }
}

bool at_end(char * & st)
{
  skip_whitespace_comment(st);

  return (*st == 0);
}

char * read_string(char * & st)
{
  skip_whitespace_comment(st);

  if (*st == 0) {
    msg_critical("Error", 
			  where() + "premature end of file, string expected");
    throw 0;
  }
    
  if (*st != '"') {
    msg_critical("Error", 
			  where() + "string expected after '"
			  + Context.previous_word + "'");
    throw 0;
  }
  
  char * r = st + 1;
  
  for (;;) {
    switch (*++st) {
    case 0:
      msg_critical("Error",
			    where() + "premature end of file reading a string");
      throw 0;
    case '"':
      Context.removed_char = ' ';
      *st++ = 0;
      if (Context.previous_word_allocated)
	delete [] Context.previous_word;
      else
	Context.previous_word_allocated = TRUE;
      {
	char * p = r;
	char * p2;
	
	while ((p2 = strchr(p, '\\')) != 0) {
	  strcpy(p2, p2 + 1);
	  p = p2 + 1;
	}
	
	p = r;
	
	while ((p2 = strchr(p, '\r')) != 0) {
	  strcpy(p2, p2 + 1);
	  p = p2 + 1;
	}
      }
      Context.previous_word = new char[st - r + 1];
      memcpy(Context.previous_word, r, st - r + 1);
      return r;
    case '\\':
      if (*++st  == 0) {
	msg_critical("Error",
			      where() + "premature end of file reading a string");
	throw 0;
      }
      break;
    case '\n':
      Context.linenum += 1;
    }
  }
}

char * read_keyword(char * & st)
{
  skip_whitespace_comment(st);

  if (*st == 0) {
    msg_critical("Error",
			  where() + "premature end of file, keyword expected");
    throw 0;
  }
  
  if (*st == '"') {
    msg_critical("Error",
			  where() + "keyword expected after '"
			  + Context.previous_word + "'");
    throw 0;
  }
  
  char * r = st;
  char c;
  
  do 
    c = *++st;
  while ((c > ' ') && (c != '"'));
  
  Context.removed_char = c;
  *st++ = 0;
  set_previous_word(r);
  return r;
}

char * read_keyword(char * & st, const char * expected)
{
  skip_whitespace_comment(st);

  if (*st == 0) {
    msg_critical("Error",
			  where() + "premature end of file, '" 
			  + expected + "' expected");
    throw 0;
  }
  
  if (*st == '"') {
    msg_critical("Error",
			  where() + "'" + expected + 
			  "' expected rather than a string after '"
			  + Context.previous_word + "'");
    throw 0;
  }
  
  char * r = st;
  char c;
  
  do 
    c = *++st;
  while ((c > ' ') && (c != '"'));
  
  *st++ = 0;

  if (strcmp(r, expected)) {
    msg_critical("Error",
			  where() + "'" + expected + "' expected rather than '"
			  + r + "' after '" + Context.previous_word + "'");
    throw 0;
  }

  set_previous_word(r);
  Context.removed_char = c;
  
  return r;
}

// for old versions
char * read_keyword(char * & st, const char * expected1,
		    const char * expected2)
{
  skip_whitespace_comment(st);

  if (*st == 0) {
    msg_critical("Error",
			  where() + "premature end of file, '" 
			  + expected1 + "' expected");
    throw 0;
  }
  
  if (*st == '"') {
    msg_critical("Error",
			  where() + "'" + expected1 + 
			  "' expected rather than a string after '"
			  + Context.previous_word + "'");
    throw 0;
  }
  
  char * r = st;
  char c;
  
  do 
    c = *++st;
  while ((c > ' ') && (c != '"'));
  
  *st++ = 0;

  if (strcmp(r, expected1) && strcmp(r, expected2)) {
    msg_critical("Error",
			  where() + "'" + expected1 + "' expected rather than '"
			  + r + "' after '" + Context.previous_word + "'");
    throw 0;
  }

  set_previous_word(r);
  Context.removed_char = c;
  
  return r;
}

void unread_keyword(char * keyword, char * & st)
{
  // not at the beginning !
  st[-1] = Context.removed_char;
  Context.removed_char = ' ';

  st = keyword;
  set_previous_word("???");	// to free Context.previous_word if necessary
}


int read_id(char * & st)
{
  skip_whitespace_comment(st);

  if (*st == 0) {
    msg_critical("Error",
			  where() + "premature end of file, id expected");
    throw 0;
  }
  
  if (*st == '"') {
    msg_critical("Error",
			  where() + "id expected after '" + Context.previous_word + "'");
    throw 0;
  }
  
  int result = 0;
  char * r = st;
  char c;
  
  while (((c = *st++) >= '0') && (c <= '9'))
    result = result*10 + c - '0';
  
  if (st == r+1) {
    msg_critical("Error", 
			  where() + "id expected after '" + Context.previous_word + "'");
    throw 0;
  }
  
  st[-1] = 0;
  set_previous_word(r);
  Context.removed_char = c;
  
  return result;
}

unsigned read_unsigned(char * & st)
{
  skip_whitespace_comment(st);

  if (*st == 0) {
    msg_critical("Error",
			  where() + "premature end of file, unsigned expected");
    throw 0;
  }
  
  if (*st == '"') {
    msg_critical("Error",
			  where() + "unsigned expected after '" + Context.previous_word + "'");
    throw 0;
  }
  
  int result = 0;
  char * r = st;
  char c;
  
  while (((c = *st++) >= '0') && (c <= '9'))
    result = result*10 + c - '0';
  
  if (st == r+1) {
    msg_critical("Error", 
			  where() + "unsigned expected after '" + Context.previous_word + "'");
    throw 0;
  }
  
  st[-1] = 0;
  set_previous_word(r);
  Context.removed_char = c;
  
  return result;
}

double read_double(char * & st)
{
  skip_whitespace_comment(st);

  if (*st == 0) {
    msg_critical("Error",
			  where() + "premature end of file, float expected");
    throw 0;
  }
  
  if (*st == '"') {
    msg_critical("Error",
			  where() + "float expected after '" + Context.previous_word + "'");
    throw 0;
  }
  
  char * r = st;
  QString s;
  char c;
  
  while (((c = *st++) > ' ') && (c != '"'))
    s += c;

  bool ok;
  double result = s.toDouble(&ok);
  
  if (!ok) {
    // '.' or ',' problem ?
    int index = s.find(',');
    
    if (index != -1) {
      s.replace(index, 1, ".");
      result = s.toDouble(&ok);
    }
    else if ((index = s.find('.')) != -1) {
      s.replace(index, 1, ",");
      result = s.toDouble(&ok);
    }
    
    if (!ok) {
      // '.' or ',' problem ?
      msg_critical("Error", 
			    where() + "float expected rather than '"
			    + s + "' after '" + Context.previous_word + "'");
      throw 0;
    }
  }
  
  Context.removed_char = c;
  st[-1] = 0;
  set_previous_word(r);
  
  return result;
}

void read_string_list(QStringList & list, char * & st)
{
  int n = read_unsigned(st);
  
  list.clear();
  
  while (n--)
    list.append(read_string(st));
}

void read_unicode_string_list(QStringList & list, char * & st)
{
  int n = read_unsigned(st);
  
  list.clear();
  
  while (n--)
    list.append(toUnicode(read_string(st)));
}

char * skip_until(char * & st, const char * expected)
{
  char * k;
  
  for (;;) {
    skip_whitespace_comment(st);
    
    if (*st == 0) {
      msg_critical("Error",
			    where() + "premature end of file");
      throw 0;
    }
  
  if (*st == '"') 
    read_string(st);
  else if (strcmp((k = read_keyword(st)), expected) == 0)
    return k;
  }
}

void wrong_keyword(const char * k, const char * expected)
{
  msg_critical("Error",
			where() + "'" + expected
			+ "' expected rather than '" + k + "'");
  throw 0;
}

void unknown_keyword(const char * k)
{
  msg_critical("Error",
			where() + "unknown or unexpected keyword '" + k + "'");
  throw 0;
}

void unknown_ref(const char * kind, int id)
{
  msg_critical("Error", 
			where() + QString::number(id) +
			" unknown " + kind + " identifier");
  throw 0;
}

//

void save_xy(QTextStream & st, const QCanvasItem * c, const char * s) {
  // save an int because there is a bug in << float in qt 3.3.3
  st << s << ' ' << (int) c->x() << ' ' << (int) c->y();
}

void save_xyz(QTextStream & st, const QCanvasItem * c, const char * s) {
  st << s << ' ' << (int) c->x() << ' ' << (int) c->y() << ' ' << (int) c->z();
}

void save_xyzwh(QTextStream & st, const QCanvasRectangle * c, const char * s) {
  st << s << ' ' << (int) c->x() << ' ' << (int) c->y() << ' ' << (int) c->z()
    << ' ' << (int) c->width() << ' ' << (int) c->height();
}

void read_xy(char * & st, QCanvasItem * c)
{
  double px = read_double(st);
  double py = read_double(st);

  c->moveBy(px - c->x(), py - c->y());
}

void bypass_xy(char * & st)
{
  read_double(st);
  read_double(st);
}

void read_xyz(char * & st, QCanvasItem * c)
{
  double px = read_double(st);
  double py = read_double(st);

  c->moveBy(px - c->x(), py - c->y());
  c->setZ(read_double(st));
}

void read_xyzwh(char * & st, QCanvasRectangle * c)
{
  double px = read_double(st);
  double py = read_double(st);

  c->moveBy(px - c->x(), py - c->y());
  c->setZ(read_double(st));
  
  double w = read_double(st);
  
  c->setSize((int) w, (int) read_double(st));
}

void read_zwh(char * & st, QCanvasRectangle * c)
{
  c->setZ(read_double(st));
  
  double w = read_double(st);
  
  c->setSize((int) w, (int) read_double(st));
}

void save_color(QTextStream & st, const char * s, UmlColor c, bool & nl)
{
  if (c != UmlDefaultColor) {
    if (!nl) {
      nl_indent(st);
      nl = TRUE;
    }
    st << s << ' ' << stringify(c) << ' ';
  }
}

void read_color(char *& st, const char * key, UmlColor & c, char * & k)
{
  if (!strcmp(k, key)) {
    c = color(read_keyword(st));
    if ((Context.read_file_format < 23) && (c == UmlWhite))
      c = UmlTransparent;
      
    k = read_keyword(st);
  }
}

void read_font(char *& st, const char * key, UmlFont & f, char * & k)
{
  if (!strcmp(k, key)) {
    f = font(read_keyword(st));
    k = read_keyword(st);
  }
  else
    f = UmlNormalFont;
}

//

void warn(const QString & s)
{
  // not showed dialog to compute needed size
  
  QDialog d_aux;
  QVBoxLayout * vbox_aux = new QVBoxLayout(&d_aux);
  
  vbox_aux->setMargin(5);
  QMultiLineEdit * e = new QMultiLineEdit(&d_aux);
  e->setText(s);
  
  // showed dialog
  
  QDialog * d = new QDialog;
  
  d->setCaption("Uml");

  QVBoxLayout * vbox = new QVBoxLayout(d);  

  vbox->setMargin(5);
  
  QTextView * t = new QTextView(d);
  QFontMetrics fm(QApplication::font());
  //int w = e->maxLineWidth() + e->minimumSizeHint().width();
  int maxw = (UmlWindow::get_workspace()->height() * 4)/5;
  
  t->setText(s);
  t->setMinimumSize(maxw, //(w > maxw) ? maxw : w,
		    ((e->numLines() > 15) ? 15 : e->numLines()) * fm.height());
		    
  vbox->addWidget(t);
  
  d->show();
}

// history save/load

void save(const DiagramItem * di, QBuffer & b)
{
  b.writeBlock((char *) &di, sizeof(di));
}

DiagramItem * load_item(QBuffer & b)
{
  DiagramItem * di;
  
  b.readBlock((char *) &di, sizeof(di));
  return di;
}

void save(const QPoint & p, QBuffer & b)
{
  int i;
  
  i = p.x();
  b.writeBlock((char *) &i, sizeof(i));
  i = p.y();
  b.writeBlock((char *) &i, sizeof(i));
}

void load(QPoint & p, QBuffer & b)
{
  int i;
  
  b.readBlock((char *) &i, sizeof(i));
  p.setX(i);
  
  b.readBlock((char *) &i, sizeof(i));
  p.setY(i);
}

void save(const QPointArray & a, QBuffer & b)
{
  QPoint * p = a.data();
  
  for (unsigned i = 0; i != a.size(); i += 1)
    save(*p++, b);
}

void load(QPointArray & a, QBuffer & b)
{
  QPoint * p = a.data();
  
  for (unsigned i = 0; i != a.size(); i += 1)
    load(*p++, b);
}

void save(double d, QBuffer & b)
{
  b.writeBlock((char *) &d, sizeof(d));
}

void load(double & d, QBuffer & b)
{
  b.readBlock((char *) &d, sizeof(d));
}

double load_double(QBuffer & b)
{
  double d;
  
  b.readBlock((char *) &d, sizeof(d));
  return d;
}

void save(int i, QBuffer & b)
{
  b.writeBlock((char *) &i, sizeof(i));
}

void load(int & i, QBuffer & b)
{
  b.readBlock((char *) &i, sizeof(i));
}
