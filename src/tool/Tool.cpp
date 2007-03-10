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

#include <qpopupmenu.h>
#include <qtextstream.h>
#include <qfiledialog.h>

#include "Tool.h"
#include "myio.h"
#include "BrowserView.h"

unsigned Tool::ntools;
ATool * Tool::tools;
bool Tool::already_read;

ATool::ATool() {
  for (int i = 0; i != UmlCodeSup; i += 1)
    applicable[i] = FALSE;
}

void Tool::init()
{
  already_read = FALSE;
  set_ntools(0);
}

void Tool::defaults()
{
  unsigned index;
  
  already_read = FALSE;
  set_ntools(14);
  
  tools[0].display = "HTML documentation";
  tools[0].cmd = "ghtml";
  for (index = 0; index != UmlCodeSup; index += 1)
    tools[0].applicable[index] = TRUE;
  
  tools[1].display = "HTML doc. (flat)";
  tools[1].cmd = "ghtml -flat";
  for (index = 0; index != UmlCodeSup; index += 1)
    tools[1].applicable[index] = TRUE;
  
  tools[2].display = "Generate .pro";
  tools[2].cmd = "gpro";
  tools[2].applicable[UmlArtifact] = TRUE;
  
  tools[3].display = "Import Rose";
  tools[3].cmd = "irose";
  tools[3].applicable[UmlProject] = TRUE;
  tools[3].applicable[UmlPackage] = TRUE;
  
  tools[4].display = "C++ utilities";
  tools[4].cmd = "cpp_util";
  tools[4].applicable[UmlClass] = TRUE;
  
  tools[5].display = "Generate XMI 1.2";
  tools[5].cmd = "gxmi";
  tools[5].applicable[UmlProject] = TRUE;
  
  tools[6].display = "Generate XMI 2.1";
  tools[6].cmd = "gxmi2";
  tools[6].applicable[UmlProject] = TRUE;
  
  tools[7].display = "Import XMI 2.1";
  tools[7].cmd = "ixmi2";
  tools[7].applicable[UmlProject] = TRUE;
  tools[7].applicable[UmlPackage] = TRUE;
  
  tools[8].display = "C++ state machine";
  tools[8].cmd = "stmgen";
  tools[8].applicable[UmlState] = TRUE;
  
  tools[9].display = "Use case wizard";
  tools[9].cmd = "usecasewizard";
  tools[9].applicable[UmlUseCase] = TRUE;
  
  tools[10].display = "Check-in";
  tools[10].cmd = "file_control ci";
  tools[10].applicable[UmlProject] = TRUE;
  tools[10].applicable[UmlPackage] = TRUE;
  
  tools[11].display = "Check-out";
  tools[11].cmd = "file_control co";
  tools[11].applicable[UmlProject] = TRUE;
  tools[11].applicable[UmlPackage] = TRUE;
  
  tools[12].display = "Deploy classes";
  tools[12].cmd = "deplcl";
  tools[12].applicable[UmlClassView] = TRUE;
  
  tools[13].display = "Global Change";
  tools[13].cmd = "global_change";
  tools[13].applicable[UmlProject] = TRUE;
  tools[13].applicable[UmlPackage] = TRUE;
  tools[13].applicable[UmlClassView] = TRUE;
  tools[13].applicable[UmlDeploymentView] = TRUE;
  tools[13].applicable[UmlClass] = TRUE;
}

bool Tool::menu_insert(QPopupMenu * tool, UmlCode target, 
		       const QObject * receiver, const char * member)
{
  unsigned index;
  bool first = TRUE;
  
  for (index = 0; index != ntools; index += 1) {
    if (tools[index].applicable[target]) {
      if (first) {
	tool->insertSeparator();
	first = FALSE;
      }
      tool->setItemParameter(tool->insertItem(tools[index].display,
					      receiver, member),
			     index);
    }
  }
  
  return !first;
}

bool Tool::menu_insert(QPopupMenu * tool, UmlCode target, int first_id)
{
  unsigned index;
  bool have = FALSE;
  
  for (index = 0; index != ntools; index += 1) {
    if (tools[index].applicable[target]) {
      have = TRUE;
      tool->insertItem(tools[index].display, first_id + index);
    }
  }
  
  return have;
}

void Tool::shortcut(QString s, int & choice, UmlCode target,
		    int first_id)
{
  unsigned index;
  
  for (index = 0; index != ntools; index += 1) {
    if (tools[index].applicable[target] &&
	(tools[index].display == s)) {
      choice = first_id + index;
      return;
    }
  }
}

QStringList Tool::all_display()
{
  QStringList r;
  unsigned index;
  
  for (index = 0; index != ntools; index += 1) {
    QString s = tools[index].display;
    
    if (r.findIndex(s) == -1)
      r.append(s);
  }
  
  r.sort();
  return r;
}

const char * Tool::command(int param)
{
  return tools[param].cmd;
}

void Tool::set_ntools(unsigned n) {
  if (tools)
    delete [] tools;
  
  tools = ((ntools = n) == 0) ? 0 : new ATool[n];
}

static const struct{
  UmlCode kind;
  const char * key;
} ToolCase[] = {
  { UmlClass, "Class" },
  { UmlOperation, "Operation" },
  { UmlAttribute, "Attribute" },
  { UmlGeneralisation, "Generalisation" },
  { UmlRealize, "Realize" },
  { UmlDependency, "Dependency" },
  { UmlAssociation, "Association" },
  { UmlDirectionalAssociation, "DirectionalAssociation" },
  { UmlAggregation, "Aggregation" },
  { UmlAggregationByValue, "AggregationByValue" },
  { UmlDirectionalAggregation, "DirectionalAggregation" },
  { UmlDirectionalAggregationByValue, "DirectionalAggregationByValue" },
  { UmlExtraMember, "ExtraMember" },
  
  { UmlState, "State" },
  { UmlRegion, "Region" },
  { UmlStateAction, "StateAction" },
  { InitialPS, "Initial" },
  { EntryPointPS, "EntryPoint" },
  { FinalPS, "Final" },
  { TerminatePS, "Terminate" },
  { ExitPointPS, "ExitPoint" },
  { DeepHistoryPS, "DeepHistory" },
  { ShallowHistoryPS, "ShallowHistory" }, 
  { JunctionPS, "Junction" },
  { ChoicePS, "Choice" },
  { ForkPS, "Fork" },
  { JoinPS, "Join" },
  { UmlTransition, "Transition" },  
  
  { UmlActivity, "Activity" },
  { UmlInterruptibleActivityRegion, "InterruptibleActivityRegion" },
  { UmlExpansionRegion, "ExpansionRegion" },
  { UmlActivityObject, "ActivityObject" },
  { UmlActivityAction, "ActivityAction" },
  { UmlParameter, "Parameter" },
  { UmlParameterSet, "ParameterSet" },
  { UmlActivityPin, "Pin" },
  { UmlExpansionNode, "ExpansionNode" },
  { InitialAN, "InitialActivityNode" },
  { ActivityFinalAN, "FinalActivityNode" },
  { FlowFinalAN, "ExitPointActivityNode" },
  { DecisionAN, "DecisionActivityNode" },
  { MergeAN, "MergeActivityNode" },
  { ForkAN, "ForkActivityNode" },
  { JoinAN, "JoinActivityNode" },
  { UmlFlow, "Flow" },
  
  { UmlProject ,"Project" },
  { UmlPackage, "Package" },
  { UmlUseCaseView, "UseCaseView" },
  { UmlClassView, "ClassView" },
  { UmlComponentView, "ComponentView" },
  { UmlDeploymentView, "DeploymentView" },
  { UmlUseCaseDiagram, "UseCaseDiagram" },
  { UmlSeqDiagram, "SeqDiagram" },
  { UmlColDiagram, "ColDiagram" },
  { UmlClassDiagram, "ClassDiagram" },
  { UmlObjectDiagram, "ObjectDiagram" },
  { UmlStateDiagram, "StateDiagram" },
  { UmlActivityDiagram, "ActivityDiagram" },
  { UmlComponentDiagram, "ComponentDiagram" },
  { UmlDeploymentDiagram, "DeploymentDiagram" },
  { UmlUseCase, "UseCase" },
  { UmlComponent, "Component" },
  { UmlDeploymentNode, "Node" },
  { UmlArtifact, "Artifact" },
  { UmlInherit, "Inherit" },
  { UmlDependOn, "DependOn" },
};

void Tool::save() 
{
  QByteArray newdef;
  QTextOStream st(newdef);
	
  st.setEncoding(QTextStream::Latin1);

  st << "// 'tool' \"the executable\" \"displayed string\" {target}+";
  
  for (unsigned rank = 0; rank != ntools; rank += 1) {
    ATool & tool = tools[rank];
    
    st << "\ntool ";
    save_string(tools[rank].display, st);
    st << ' ';
    save_string(tools[rank].cmd, st);
    
    for (int index = 0; index != sizeof(ToolCase) / sizeof(*ToolCase); index += 1) {
      if (tool.applicable[ToolCase[index].kind]) {
	st << ' ' << ToolCase[index].key;
      }
    }
  }
  st << '\n';
  
  st << '\000';
  save_if_needed("tools", newdef);
}

// try to read tool settings in the prj file

void Tool::read(char * & st, char * & k)
{
  if (!strcmp(k, "tools_number")) {
    // old format
    set_ntools(read_unsigned(st));
    read(st, k, FALSE);
  }
  else
    set_ntools(0);
}

// try to read tool settings

bool Tool::read(const char * f)
{
  if (!already_read) {
    char * s = read_file((f == 0) ? "tools" : f);
    
    if (s != 0) {
      try {
	char * st = s;
	char * k;
	
	read(st, k, TRUE);
      }
      catch (int) {
	;
      }
      delete [] s;
      return TRUE;
    }
  }
  
  return FALSE;
}

// try to read tool settings of an other project

bool Tool::import()
{
  QString fn = QFileDialog::getOpenFileName("tools", "tools");
      
  if (!fn.isEmpty()) {
    already_read = FALSE;
    set_ntools(0);
    return read((const char *) fn);
  }
  
  return FALSE;
}

// for all cases

void Tool::read(char * & st, char * & k, bool new_format)
{
  if (new_format && at_end(st)) {
    ntools = 0;
    return;
  }
    
  unsigned rank = 0;
    
  k = read_keyword(st);
  already_read = TRUE;
  
  for (;;) {
    if (strcmp(k, "tool")) {
      if (new_format) {
	ntools = rank;
	wrong_keyword(k, "tool");
      }
      else if (strcmp(k, "end"))
	wrong_keyword(k, "end");
      else
	k = read_keyword(st);
      
      return;
    }
    
    if (rank == ntools) {
      // too small table
      ATool * t = new ATool[ntools + 16];
      
      for (rank = 0; rank != ntools; rank += 1)
	t[rank] = tools[rank];
      
      if (tools)
	delete [] tools;
      tools = t;
      ntools += 16;
    }
    
    ATool & tool = tools[rank++];
    
    tool.display = read_string(st);
    tool.cmd = read_string(st);

    // fixe ghtml default conf bug
    int index = tool.cmd.find("ghtml - flat");
    
    if (index != -1)
      tool.cmd.remove(index + 7, 1);
    
    if (new_format && at_end(st)) {
      ntools = rank;
      return;
    }
    
    k = read_keyword(st);
    
    if (!strcmp(k, "Project")) {
      tool.applicable[UmlProject] = TRUE;
      
      if (new_format && at_end(st)) {
	ntools = rank;
	return;
      }
      k = read_keyword(st);
    }
    
    for (;;) {
      // old format
      if (!strcmp(k, "Attribut"))
	k = "Attribute";
      
      if (((index = strlen(k)) > 4) && 
	  !strcmp(k+index-4, "Icon"))
	k[index-4] = 0;
      
      for (index = 0; index != sizeof(ToolCase) / sizeof(*ToolCase); index += 1) {
	if (!strcmp(k, ToolCase[index].key))
	  break;
      }
      
      if (index != sizeof(ToolCase) / sizeof(*ToolCase))
	tool.applicable[ToolCase[index].kind] = TRUE;
      else
	break;
      
      if (new_format && at_end(st)) {
	ntools = rank;
	return;
      }
	
      k = read_keyword(st);
    }
  }
}

//
// 
//

static int Verbose;

void toggle_verbose_generation()
{
  Verbose ^= 1;
}

bool verbose_generation()
{
  return (Verbose != 0);
}

//
// 
//

static int PreserveBodies;

void toggle_preserve_bodies()
{
  PreserveBodies ^= 1;
}

bool preserve_bodies()
{
  return (PreserveBodies != 0);
}

//

static int AddOperationProfile;

void toggle_add_operation_profile()
{
	AddOperationProfile ^= 1;
}

bool add_operation_profile()
{
	return (AddOperationProfile != 0);
}
