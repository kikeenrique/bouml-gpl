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

#include <qcursor.h>

#include "Settings.h"
#include "SettingsDialog.h"
#include "ClassSettingsDialog.h"
#include "UmlCanvas.h"
#include "myio.h"

DrawingSettings * DrawingSettings::instance() {
  static DrawingSettings * the;
  
  if (the == 0)
    the = new DrawingSettings;
  
  return the;
}

void DrawingSettings::modified() {
  UmlCanvas::update_all_shadow();
  instance()->is_modified();
}

void DrawingSettings::is_modified() {
  emit changed();
}

//

#define check_default(att, default)		\
	if ((result.att != default) ||		\
	    ((result.att = att) != default))	\
	  done += 1

//
  
ClassSettings::ClassSettings() {
  attribute_visibility = UmlDefaultVisibility;
  relation_visibility = UmlDefaultVisibility;
  operation_visibility = UmlDefaultVisibility;
}
 
void ClassSettings::save(QTextStream & st) const {
  bool nl;
  
  if (attribute_visibility != UmlDefaultVisibility) {
    nl_indent(st);
    nl = TRUE;
    st << "default_attribute_visibility " << stringify(attribute_visibility);
  }
  else
    nl = FALSE;
  
  if (relation_visibility != UmlDefaultVisibility) {
    if (nl)
      st << ' ';
    else {
      nl_indent(st);
      nl = TRUE;
    }
    st << "default_relation_visibility " << stringify(relation_visibility);
  }
  
  if (operation_visibility != UmlDefaultVisibility) {
    if (nl)
      st << ' ';
    else
      nl_indent(st);
    st << "default_operation_visibility " << stringify(operation_visibility);
  }
}

void ClassSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "default_attribute_visibility") ||
      !strcmp(k, "default_attribut_visibility")) {
    attribute_visibility = visibility(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "default_relation_visibility")) {
    relation_visibility = visibility(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "default_operation_visibility")) {
    operation_visibility = visibility(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ClassSettings::edit(UmlCode who) {
  ClassSettingsDialog dialog(this, (who == UmlProject));
  
  return (dialog.exec() == QDialog::Accepted);
}

//

ClassDiagramSettings::ClassDiagramSettings() {
  draw_all_relations = UmlDefaultState;
  hide_attributes = UmlDefaultState;
  hide_operations = UmlDefaultState;
  show_full_members_definition = UmlDefaultState;
  show_members_visibility = UmlDefaultState;
  package_name_in_tab = UmlDefaultState;
  class_drawing_mode = DefaultClassDrawingMode;
  drawing_language = DefaultDrawingLanguage;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  shadow = UmlDefaultState;
}

void ClassDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "draw_all_relations " << stringify(draw_all_relations)
    << " hide_attributes " << stringify(hide_attributes)
      << " hide_operations " << stringify(hide_operations)
	<< " show_members_full_definition " << stringify(show_full_members_definition)
	  << " show_members_visibility " << stringify(show_members_visibility)
	    << " package_name_in_tab " << stringify(package_name_in_tab)
	      << " class_drawing_mode " << stringify(class_drawing_mode)
	        << " drawing_language " << stringify(drawing_language)
		  << " show_context_mode " << stringify(show_context_mode)
		    << " auto_label_position " << stringify(auto_label_position)
		      << " shadow " << stringify(shadow);
}

void ClassDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "hide_attributes") ||
      !strcmp(k, "hide_attributs")) {
    hide_attributes = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "hide_operations")) {
    hide_operations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_full_members_definition")) {
    // old version
    show_full_members_definition = state(read_keyword(st));
    show_members_visibility = show_full_members_definition;
    k = read_keyword(st);
  }
  else {
    if (!strcmp(k, "show_members_full_definition")) {
      // new version
      show_full_members_definition = state(read_keyword(st));
      k = read_keyword(st);
    }
    if (!strcmp(k, "show_members_visibility")) {
      // new version
      show_members_visibility = state(read_keyword(st));
      k = read_keyword(st);
    }
  }
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if ((!strcmp(k, "class_view_mode")) ||	// old
      (!strcmp(k, "class_drawing_mode"))) {
    class_drawing_mode = drawing_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context_mode")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
}


bool ClassDiagramSettings::complete(ClassDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(draw_all_relations, UmlDefaultState);
  check_default(hide_attributes, UmlDefaultState);
  check_default(hide_operations, UmlDefaultState);
  check_default(show_full_members_definition, UmlDefaultState);
  check_default(show_members_visibility, UmlDefaultState);
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(class_drawing_mode, DefaultClassDrawingMode);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return done == 11;
}

void ClassDiagramSettings::complete(QArray<StateSpec> & a, UmlCode who) {
  int i = a.size();
  
  switch (who) {
  case UmlClass:
    a.resize(i + 8);
  
    a[i].set("drawing language", &drawing_language);
    a[i + 1].set("drawing mode", &class_drawing_mode);
    a[i + 2].set("show context", &show_context_mode);
    a[i + 3].set("hide attributes", &hide_attributes);
    a[i + 4].set("hide operations", &hide_operations);
    a[i + 5].set("show members full definition",
		 &show_full_members_definition);
    a[i + 6].set("show members visibility",
		 &show_members_visibility);
    a[i + 7].set("draw all relations", &draw_all_relations);
    break;
  case UmlPackage:
    a.resize(i + 2);
  
    a[i].set("name in tab", &package_name_in_tab);
    a[i + 1].set("show context", &show_context_mode);
    break;
  case UmlClassDiagram:
    a.resize(i + 11);
  
    a[i].set("drawing language", &drawing_language);
    a[i + 1].set("classes drawing mode", &class_drawing_mode);
    a[i + 2].set("hide classes attributes", &hide_attributes);
    a[i + 3].set("hide classes operations", &hide_operations);
    a[i + 4].set("show classes members full definition",
		 &show_full_members_definition);
    a[i + 5].set("show members visibility",
		 &show_members_visibility);
    a[i + 6].set("draw all classes relations", &draw_all_relations);
    a[i + 7].set("show packages name in tab", &package_name_in_tab);
    a[i + 8].set("show classes and packages context", &show_context_mode);
    a[i + 9].set("automatic labels position", &auto_label_position);
    a[i + 10].set("draw shadow", &shadow);
    break;
  default:
    a.resize(i + 11);
  
    a[i].set("class#drawing language", &drawing_language);
    a[i + 1].set("class#classes drawing mode", &class_drawing_mode);
    a[i + 2].set("class#hide classes attributes", &hide_attributes);
    a[i + 3].set("class#hide classes operations", &hide_operations);
    a[i + 4].set("class#show classes members full definition",
		 &show_full_members_definition);
    a[i + 5].set("class#show classes members visibility",
		 &show_members_visibility);
    a[i + 6].set("class#draw all classes relations", &draw_all_relations);
    a[i + 7].set("class#show packages name in tab", &package_name_in_tab);
    a[i + 8].set("class#show classes and packages context", &show_context_mode);
    a[i + 9].set("class#automatic labels position", &auto_label_position);
    a[i + 10].set("class#draw shadow", &shadow);
  }
}

void ClassDiagramSettings::set(QArray<StateSpec> & a, int index) {
  if (a[index].name != 0)
    drawing_language = (DrawingLanguage) *((DrawingLanguage *) a[index].state);
  if (a[index + 1].name != 0)
    class_drawing_mode = (ClassDrawingMode) *((ClassDrawingMode *) a[index + 1].state);
  if (a[index + 2].name != 0)
    show_context_mode = (ShowContextMode) *((ShowContextMode *) a[index + 2].state);
  if (a[index + 3].name != 0)
    hide_attributes = (Uml3States) *((Uml3States *) a[index + 3].state);
  if (a[index + 4].name != 0)
    hide_operations = (Uml3States) *((Uml3States *) a[index + 4].state);
  if (a[index + 5].name != 0)
    show_full_members_definition = (Uml3States) *((Uml3States *) a[index + 5].state);
  if (a[index + 6].name != 0)
    show_members_visibility = (Uml3States) *((Uml3States *) a[index + 6].state);
  if (a[index + 7].name != 0)
    draw_all_relations = (Uml3States) *((Uml3States *) a[index + 7].state);
}

//

SequenceDiagramSettings::SequenceDiagramSettings() {
  show_full_operations_definition = UmlDefaultState;
  write_horizontally = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
  shadow = UmlDefaultState;
}

void SequenceDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "show_full_operations_definition " << stringify(show_full_operations_definition)
    << " write_horizontally " << stringify(write_horizontally)
      << " drawing_language " << stringify(drawing_language)
	<< " shadow " << stringify(shadow);
}

void SequenceDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "show_full_operations_definition")) {
    show_full_operations_definition = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_horizontally") ||
      !strcmp(k, "write_horizontaly")) {
    write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool SequenceDiagramSettings::complete(SequenceDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(show_full_operations_definition, UmlDefaultState);
  check_default(write_horizontally, UmlDefaultState);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(shadow, UmlDefaultState);
  
  return done == 4;
}

void SequenceDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 4);
  a[i].set((local) ? "drawing language"
		   : "sequence#drawing language",
	   &drawing_language);
  a[i + 1].set((local) ? "show operations full definition"
		       : "sequence#show operations full definition",
	       &show_full_operations_definition);
  a[i + 2].set((local) ? "write name:type horizontally"
		       : "sequence#write name:type horizontally",
	       &write_horizontally);
  a[i + 3].set((local) ? "draw shadow" : "sequence#draw shadow",
	       &shadow);
}

//

CollaborationDiagramSettings::CollaborationDiagramSettings() {
  show_full_operations_definition = UmlDefaultState;
  show_hierarchical_rank = UmlDefaultState;
  write_horizontally = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  shadow = UmlDefaultState;
}

void CollaborationDiagramSettings::save(QTextStream & st) const {
    nl_indent(st);
    st << "show_full_operations_definition " << stringify(show_full_operations_definition)
      << " show_hierarchical_rank " << stringify(show_hierarchical_rank)
	<< " write_horizontally " << stringify(write_horizontally)
	  << " drawing_language " << stringify(drawing_language)
	    << " package_name_in_tab " << stringify(package_name_in_tab)
	      << " show_context " << stringify(show_context_mode)
		<< " shadow " << stringify(shadow);
}

void CollaborationDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "show_full_operations_definition")) {
    show_full_operations_definition = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_hierarchical_rank")) {
    show_hierarchical_rank = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_horizontally") ||
      !strcmp(k, "write_horizontaly")) {
    write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool CollaborationDiagramSettings::complete(CollaborationDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(show_full_operations_definition, UmlDefaultState);
  check_default(show_hierarchical_rank, UmlDefaultState);
  check_default(write_horizontally, UmlDefaultState);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(shadow, UmlDefaultState);
  
  return done == 7;
}

void CollaborationDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);
  if (local) {
    a[i].set("drawing language", &drawing_language);
    a[i+1].set("show operations full definition",
	       &show_full_operations_definition);
    a[i+2].set("show hierarchical rank",
	       &show_hierarchical_rank);
    a[i+3].set("write name:type horizontally",
	       &write_horizontally);
    a[i+4].set("packages name in tab", &package_name_in_tab);
    a[i+5].set("show package context", &show_context_mode);
    a[i+6].set("draw shadow", &shadow);
  }
  else {
    a[i].set("collaboration#drawing language",
	     &drawing_language);
    a[i+1].set("collaboration#show operations full definition",
	       &show_full_operations_definition);
    a[i+2].set("collaboration#show hierarchical rank",
	       &show_hierarchical_rank);
    a[i+3].set("collaboration#write name:type horizontally",
	       &write_horizontally);
    a[i+4].set("collaboration#show packages name in tab", &package_name_in_tab);
    a[i+5].set("collaboration#show packages context", &show_context_mode);
    a[i+6].set("collaboration#draw shadow", &shadow);
  }
}

//

ObjectDiagramSettings::ObjectDiagramSettings() {
  write_horizontally = UmlDefaultState;
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  shadow = UmlDefaultState;
}

void ObjectDiagramSettings::save(QTextStream & st) const {
    nl_indent(st);
    st << " write_horizontally " << stringify(write_horizontally)
      << " package_name_in_tab " << stringify(package_name_in_tab)
	<< " show_context " << stringify(show_context_mode)
	  << " auto_label_position " << stringify(auto_label_position)
	    << " shadow " << stringify(shadow);
}

void ObjectDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "write_horizontally") ||
      !strcmp(k, "write_horizontaly")) {
    write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ObjectDiagramSettings::complete(ObjectDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(write_horizontally, UmlDefaultState);
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return done == 5;
}

void ObjectDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 5);
  if (local) {
    a[i].set("write name:type horizontally",
	       &write_horizontally);
    a[i+1].set("packages name in tab", &package_name_in_tab);
    a[i+2].set("show package context", &show_context_mode);
    a[i+3].set("automatic labels position", &auto_label_position);
    a[i+4].set("draw shadow", &shadow);
  }
  else {
    a[i].set("object#write name:type horizontally",
	       &write_horizontally);
    a[i+1].set("object#show packages name in tab", &package_name_in_tab);
    a[i+2].set("object#show packages context", &show_context_mode);
    a[i+4].set("object#automatic labels position", &auto_label_position);
    a[i+3].set("object#draw shadow", &shadow);
  }
}

//

UseCaseDiagramSettings::UseCaseDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  shadow = UmlDefaultState;
}

void UseCaseDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " auto_label_position " << stringify(auto_label_position)
	<< " shadow " << stringify(shadow);
}

void UseCaseDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool UseCaseDiagramSettings::complete(UseCaseDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return done == 4;
}

void UseCaseDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 4);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("automatic labels position", &auto_label_position);
    a[i + 3].set("draw shadow", &shadow);
  }
  else {
    a[i].set("use case#show packages name in tab", &package_name_in_tab);
    a[i + 1].set("use case#show packages context", &show_context_mode);
    a[i + 2].set("use case#automatic labels position", &auto_label_position);
    a[i + 3].set("use case#draw shadow", &shadow);
  }
}

//

ComponentDiagramSettings::ComponentDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  shadow = UmlDefaultState;
}

void ComponentDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " auto_label_position " << stringify(auto_label_position)
	<< " shadow " << stringify(shadow);
  componentdrawingsettings.save(st);
}

void ComponentDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  componentdrawingsettings.read(st, k);
}

bool ComponentDiagramSettings::complete(ComponentDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return componentdrawingsettings.complete(result.componentdrawingsettings)
    && (done == 4);
}

void ComponentDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 4);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("automatic labels position", &auto_label_position);
    a[i + 3].set("draw shadow", &shadow);
  }
  else {
    a[i].set("component#show packages name in tab", &package_name_in_tab);
    a[i + 1].set("component#show packages context", &show_context_mode);
    a[i + 2].set("component#automatic labels position", &auto_label_position);
    a[i + 3].set("component#draw shadow", &shadow);
  }
  componentdrawingsettings.complete(a, local, FALSE);
}

//

DeploymentDiagramSettings::DeploymentDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  write_horizontally = UmlDefaultState;
  auto_label_position = UmlDefaultState;
  shadow = UmlDefaultState;
}

void DeploymentDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " write_horizontally " << stringify(write_horizontally)
	<< " auto_label_position " << stringify(auto_label_position)
	  << " shadow " << stringify(shadow);
  componentdrawingsettings.save(st);
}

void DeploymentDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_horizontally") ||
      !strcmp(k, "write_horizontaly")) {
    write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  componentdrawingsettings.read(st, k);
}

bool DeploymentDiagramSettings::complete(DeploymentDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(write_horizontally, UmlDefaultState);
  check_default(auto_label_position, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return componentdrawingsettings.complete(result.componentdrawingsettings)
    && (done == 5);
}

void DeploymentDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 5);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("write node instance horizontally", &write_horizontally);
    a[i + 3].set("automatic labels position", &auto_label_position);
    a[i + 4].set("draw shadow", &shadow);
  }
  else  {
    a[i].set("deployment#show packages name in tab",
	     &package_name_in_tab);
    a[i + 1].set("deployment#show packages context",
		 &show_context_mode);
    a[i + 2].set("deployment#write node instances horizontally",
		 &write_horizontally);
    a[i + 3].set("deployment#automatic labels position",
		 &auto_label_position);
    a[i + 4].set("deployment#draw shadow", &shadow);
  }
  
  componentdrawingsettings.complete(a, local, TRUE);
}

//

ComponentDrawingSettings::ComponentDrawingSettings() {
  draw_component_as_icon = UmlDefaultState;
  show_component_req_prov = UmlDefaultState;
  show_component_rea = UmlDefaultState;
}

void ComponentDrawingSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "draw_component_as_icon " << stringify(draw_component_as_icon)
    << " show_component_req_prov " << stringify(show_component_req_prov)
      << " show_component_rea " << stringify(show_component_rea);
}
  
void ComponentDrawingSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "draw_component_as_icon") ||
      !strcmp(k, "drawn_as_icon") /* old format */) {
    draw_component_as_icon = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_component_req_prov")) {
    show_component_req_prov = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_component_rea")) {
    show_component_rea = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ComponentDrawingSettings::complete(ComponentDrawingSettings & result) const {
  unsigned done = 0;
  
  check_default(draw_component_as_icon, UmlDefaultState);
  check_default(show_component_req_prov, UmlDefaultState);
  check_default(show_component_rea, UmlDefaultState);
  
 return (done == 3);
}

void ComponentDrawingSettings::complete(QArray<StateSpec> & a,
					bool local, bool depl) {
  int i = a.size();
  
  a.resize(i + 3);
  a[i].set((local) ? "draw component as icon"
		   : ((depl) ? "deployment#draw component as icon"
			     : "component#draw component as icon"),
	   &draw_component_as_icon);
  a[i + 1].set((local) ? "show component's required and\nprovided interfaces"
		       : ((depl) ? "deployment#show component's required\nand provided interfaces"
				 : "component#show component's required\nand provided interfaces"),
	       &show_component_req_prov);
  a[i + 2].set((local) ? "show component's realizations"
		       : ((depl) ? "deployment#show component's realizations"
				 : "component#show component's realizations"),
	       &show_component_rea);
}

//

StateDiagramSettings::StateDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  write_label_horizontally = UmlDefaultState;
  show_trans_definition = UmlDefaultState;
  shadow = UmlDefaultState;
}

void StateDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " auto_label_position " << stringify(auto_label_position)
	<< " write_trans_label_horizontally " << stringify(write_label_horizontally)
	  << " show_trans_definition " << stringify(show_trans_definition)
	    << " shadow " << stringify(shadow);
  statedrawingsettings.save(st);
}

void StateDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_trans_label_horizontally")) {
    write_label_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_trans_definition")) {
    show_trans_definition = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  statedrawingsettings.read(st, k);
}

bool StateDiagramSettings::complete(StateDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(write_label_horizontally, UmlDefaultState);
  check_default(show_trans_definition, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return statedrawingsettings.complete(result.statedrawingsettings)
    && (done == 6);
}

void StateDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 6);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("automatic labels position", &auto_label_position);
    a[i + 3].set("write transition horizontally", &write_label_horizontally);
    a[i + 4].set("show transition definition", &show_trans_definition);
    a[i + 5].set("draw shadow", &shadow);
  }
  else {
    a[i].set("state#show packages name in tab", &package_name_in_tab);
    a[i + 1].set("state#show packages context", &show_context_mode);
    a[i + 2].set("state#automatic labels position",
		 &auto_label_position);
    a[i + 3].set("state#write transition horizontally",
		 &write_label_horizontally);
    a[i + 4].set("state#show transition definition",
		 &show_trans_definition);
    a[i + 5].set("state#draw shadow", &shadow);
  }
  statedrawingsettings.complete(a, local);
}

//

StateDrawingSettings::StateDrawingSettings() {
  show_activities = UmlDefaultState;
  region_horizontally = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
}

void StateDrawingSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "show_activities " << stringify(show_activities)
    << " region_horizontally " << stringify(region_horizontally)
      << " drawing_language " << stringify(drawing_language);
}
  
void StateDrawingSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "show_activities")) {
    show_activities = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "region_horizontally")) {
    region_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
}

bool StateDrawingSettings::complete(StateDrawingSettings & result) const {
  unsigned done = 0;
  
  check_default(show_activities, UmlDefaultState);
  check_default(region_horizontally, UmlDefaultState);
  check_default(drawing_language, DefaultDrawingLanguage);
  
 return (done == 3);
}

void StateDrawingSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 3);
  a[i].set((local) ? "show state activities"
		   : "state#show state activities",
	   &show_activities);
  a[i + 1].set((local) ? "draw state's regions horizontally"
		       : "state#draw state's regions horizontally",
	       &region_horizontally);
  a[i + 2].set((local) ? "drawing language"
		       : "state#drawing language", 
	       &drawing_language);
}

//

ActivityDiagramSettings::ActivityDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  show_opaque_action_definition = UmlDefaultState;
  auto_label_position = UmlDefaultState;
  write_label_horizontally = UmlDefaultState;
  shadow = UmlDefaultState;
}

void ActivityDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " show_opaque_action_definition " << stringify(show_opaque_action_definition)
        << " auto_label_position " << stringify(auto_label_position)
	  << " write_flow_label_horizontally " << stringify(write_label_horizontally)
	    << " shadow " << stringify(shadow);
  activitydrawingsettings.save(st);
}

void ActivityDiagramSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "package_name_in_tab")) {
    package_name_in_tab = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_opaque_action_definition")) {
    show_opaque_action_definition = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "auto_label_position")) {
    auto_label_position = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "write_flow_label_horizontally")) {
    write_label_horizontally = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  activitydrawingsettings.read(st, k);
}

bool ActivityDiagramSettings::complete(ActivityDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(show_opaque_action_definition, UmlDefaultState);
  check_default(auto_label_position, UmlDefaultState);
  check_default(write_label_horizontally, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return activitydrawingsettings.complete(result.activitydrawingsettings)
    && (done == 6);
}

void ActivityDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 6);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("automatic labels position", &auto_label_position);
    a[i + 3].set("write flow label horizontally", &write_label_horizontally);
    a[i + 4].set("show opaque action definition", &show_opaque_action_definition);
    a[i + 5].set("draw shadow", &shadow);
  }
  else {
    a[i].set("activity#show packages name in tab", &package_name_in_tab);
    a[i + 1].set("activity#show packages context", &show_context_mode);
    a[i + 2].set("activity#automatic labels position",
		 &auto_label_position);
    a[i + 3].set("activity#write flow label horizontally",
		 &write_label_horizontally);
    a[i + 4].set("activity#show opaque action definition",
		 &show_opaque_action_definition);
    a[i + 5].set("activity#draw shadow", &shadow);
  }
  activitydrawingsettings.complete(a, local);
}

//

ActivityDrawingSettings::ActivityDrawingSettings() {
  show_infonote = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
}

void ActivityDrawingSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "show_infonote " << stringify(show_infonote)
    << " drawing_language " << stringify(drawing_language);
}
  
void ActivityDrawingSettings::read(char * & st, char * & k) {
  if (!strcmp(k, "show_infonote")) {
    show_infonote = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ActivityDrawingSettings::complete(ActivityDrawingSettings & result) const {
  unsigned done = 0;
  
  check_default(show_infonote, UmlDefaultState);
  check_default(drawing_language, DefaultDrawingLanguage);
  
 return (done == 2);
}

void ActivityDrawingSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 2);
  a[i].set((local) ? "show information note"
		   : "activity#show information note",
	   &show_infonote);
  a[i + 1].set((local) ? "drawing language"
		       : "activity#drawing language", 
	       &drawing_language);
}

void ActivityDrawingSettings::set(QArray<StateSpec> & a, int index) {
  if (a[index].name != 0)
    show_infonote = (Uml3States) *((Uml3States *) a[index].state);
  if (a[index + 1].name != 0)
    drawing_language = (DrawingLanguage) *((DrawingLanguage *) a[index + 1].state);
}
