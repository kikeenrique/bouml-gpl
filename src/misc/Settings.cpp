// *************************************************************************
//
// Copyleft 2004-2009 Bruno PAGES  .
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





#include <qcursor.h>

#include "Settings.h"
#include "SettingsDialog.h"
#include "ClassSettingsDialog.h"
#include "UmlCanvas.h"
#include "myio.h"

bool DrawingSettings::just_changed;

DrawingSettings * DrawingSettings::instance() {
  static DrawingSettings * the;
  
  if (the == 0)
    the = new DrawingSettings;
  
  return the;
}

void DrawingSettings::modified() {
  UmlCanvas::update_global_settings();
  instance()->is_modified();
}

void DrawingSettings::is_modified() {
  just_changed = TRUE;  // to optimize 'draw all relations'
  emit changed();
  just_changed = FALSE;
}

bool DrawingSettings::just_modified()
{
  return just_changed;
}

//

#define check_default(att, default)		\
	if ((result.att != default) ||		\
	    ((result.att = att) != default))	\
	  done += 1

#define assign_default(att, default)		\
	if (att == default) att = from.att

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
  hide_attributes = UmlDefaultState;
  hide_operations = UmlDefaultState;
  show_full_members_definition = UmlDefaultState;
  show_members_visibility = UmlDefaultState;
  show_members_stereotype = UmlDefaultState;
  show_members_multiplicity = UmlDefaultState;
  show_members_initialization = UmlDefaultState;
  show_attribute_modifiers = UmlDefaultState;
  member_max_width = UmlDefaultMaxMemberWidth;
  show_parameter_dir = UmlDefaultState;
  show_parameter_name = UmlDefaultState;
  package_name_in_tab = UmlDefaultState;
  class_drawing_mode = DefaultClassDrawingMode;
  drawing_language = DefaultDrawingLanguage;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  show_relation_modifiers = UmlDefaultState;
  show_infonote = UmlDefaultState;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void ClassDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "draw_all_relations " << stringify(draw_all_relations)
    << " hide_attributes " << stringify(hide_attributes)
      << " hide_operations " << stringify(hide_operations)
	<< " show_members_full_definition " << stringify(show_full_members_definition)
	  << " show_members_visibility " << stringify(show_members_visibility)
	    << " show_members_stereotype " << stringify(show_members_stereotype)
	      << " show_members_multiplicity " << stringify(show_members_multiplicity)
		<< " show_members_initialization " << stringify(show_members_initialization)
		  << " show_attribute_modifiers " << stringify(show_attribute_modifiers)
		    << " member_max_width " << ((int) member_max_width)
		      << " show_parameter_dir " << stringify(show_parameter_dir)
			<< " show_parameter_name " << stringify(show_parameter_name)
			  << " package_name_in_tab " << stringify(package_name_in_tab)
			    << " class_drawing_mode " << stringify(class_drawing_mode)
			      << " drawing_language " << stringify(drawing_language)
				<< " show_context_mode " << stringify(show_context_mode)
				  << " auto_label_position " << stringify(auto_label_position)
				    << " show_relation_modifiers " << stringify(show_relation_modifiers)
				      << " show_infonote " << stringify(show_infonote)
					<< " shadow " << stringify(shadow)
					  << " show_stereotype_properties " << stringify(show_stereotype_properties);
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
    show_members_stereotype = UmlNo;
    show_members_multiplicity = UmlNo;
    show_members_initialization = UmlNo;
    show_attribute_modifiers = UmlNo;
    member_max_width = UmlUnlimitedMemberWidth;
    show_parameter_dir = UmlYes;
    show_parameter_name = UmlYes;
    show_relation_modifiers = UmlNo;
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
    if (!strcmp(k, "show_members_stereotype")) {
      // new version
      show_members_stereotype = state(read_keyword(st));
      k = read_keyword(st);
    }
    if (!strcmp(k, "show_members_multiplicity")) {
      // new version
      show_members_multiplicity = state(read_keyword(st));
      k = read_keyword(st);
    }    
    if (!strcmp(k, "show_members_initialization")) {
      // new version
      show_members_initialization = state(read_keyword(st));
      k = read_keyword(st);
    }   
    if (!strcmp(k, "show_attribute_modifiers")) {
      // new version
      show_attribute_modifiers = state(read_keyword(st));
      k = read_keyword(st);
    }      
    if (!strcmp(k, "member_max_width")) {
      // new version
      member_max_width = (char) read_unsigned(st);
      // to manage erroneous value set directly in the file by user
      switch (member_max_width) {
      case UmlDefaultMaxMemberWidth:
      case UmlUnlimitedMemberWidth:
	break;
      default:
	if (member_max_width < MinMemberWidthValue)
	  member_max_width = MinMemberWidthValue;
	else if (member_max_width >= SupMemberWidthValue)
	  member_max_width = UmlUnlimitedMemberWidth;
      }
      k = read_keyword(st);
    }      
    if (!strcmp(k, "show_parameter_dir")) {
      // new version
      show_parameter_dir = state(read_keyword(st));
      k = read_keyword(st);
    }
    if (!strcmp(k, "show_parameter_name")) {
      // new version
      show_parameter_name = state(read_keyword(st));
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
  if (!strcmp(k, "show_relation_modifiers")) {
    show_relation_modifiers = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_infonote")) {
    show_infonote = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
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
  check_default(show_members_stereotype, UmlDefaultState);
  check_default(show_members_multiplicity, UmlDefaultState);
  check_default(show_members_initialization, UmlDefaultState);
  check_default(show_attribute_modifiers, UmlDefaultState);
  check_default(member_max_width, UmlDefaultMaxMemberWidth);
  check_default(show_parameter_dir, UmlDefaultState);
  check_default(show_parameter_name, UmlDefaultState);
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(class_drawing_mode, DefaultClassDrawingMode);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(show_relation_modifiers, UmlDefaultState);
  check_default(show_infonote, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 21;
}

void ClassDiagramSettings::assign(const ClassDiagramSettings & from) {
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(hide_attributes, UmlDefaultState);
  assign_default(hide_operations, UmlDefaultState);
  assign_default(show_full_members_definition, UmlDefaultState);
  assign_default(show_members_visibility, UmlDefaultState);
  assign_default(show_members_stereotype, UmlDefaultState);
  assign_default(show_members_multiplicity, UmlDefaultState);
  assign_default(show_members_initialization, UmlDefaultState);
  assign_default(show_attribute_modifiers, UmlDefaultState);
  assign_default(member_max_width, UmlDefaultMaxMemberWidth);
  assign_default(show_parameter_dir, UmlDefaultState);
  assign_default(show_parameter_name, UmlDefaultState);
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(class_drawing_mode, DefaultClassDrawingMode);
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(show_relation_modifiers, UmlDefaultState);
  assign_default(show_infonote, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void ClassDiagramSettings::complete(QArray<StateSpec> & a, UmlCode who) {
  int i = a.size();
  
  switch (who) {
  case UmlClass:
    // order known by ClassDiagramSettings::set
    a.resize(i + 16);
  
    a[i].set("drawing language", &drawing_language);
    a[i + 1].set("drawing mode", &class_drawing_mode);
    a[i + 2].set("show context", &show_context_mode);
    a[i + 3].set("hide attributes", &hide_attributes);
    a[i + 4].set("hide operations", &hide_operations);
    a[i + 5].set("show members full definition",
		 &show_full_members_definition);
    a[i + 6].set("show members visibility",
		 &show_members_visibility);
    a[i + 7].set("show members stereotype",
		 &show_members_stereotype);
    a[i + 8].set("show attributes multiplicity",
		 &show_members_multiplicity);
    a[i + 9].set("show attributes initialisation",
		 &show_members_initialization);
    a[i + 10].set("show attribute modifiers",
		 &show_attribute_modifiers);
    a[i + 11].set("show parameter direction",
		  &show_parameter_dir);
    a[i + 12].set("show parameter name",
		  &show_parameter_name);
    a[i + 13].set("members max width",
		  &member_max_width);
    a[i + 14].set("show information note",
		  &show_infonote);
    a[i + 15].set("show stereotype properties",
		  &show_stereotype_properties);
    break;
  case UmlPackage:
    a.resize(i + 3);
  
    a[i].set("name in tab", &package_name_in_tab);
    a[i + 1].set("show context", &show_context_mode);
    a[i + 2].set("show stereotype properties", &show_stereotype_properties);
    break;
  case UmlClassDiagram:
    a.resize(i + 21);
  
    a[i].set("drawing language", &drawing_language);
    a[i + 1].set("classes drawing mode", &class_drawing_mode);
    a[i + 2].set("hide classes attributes", &hide_attributes);
    a[i + 3].set("hide classes operations", &hide_operations);
    a[i + 4].set("show classes members full definition",
		 &show_full_members_definition);
    a[i + 5].set("show members visibility",
		 &show_members_visibility);
    a[i + 6].set("show members stereotype",
		 &show_members_stereotype);
    a[i + 7].set("show attributes multiplicity",
		 &show_members_multiplicity);
    a[i + 8].set("show attributes initialization",
		 &show_members_initialization);
    a[i + 9].set("show attribute modifiers",
		 &show_attribute_modifiers);
    a[i + 10].set("show parameter direction",
		  &show_parameter_dir);
    a[i + 11].set("show parameter name",
		  &show_parameter_name);
    a[i + 12].set("members max width",
		  &member_max_width);
    a[i + 13].set("draw all relations", &draw_all_relations);
    a[i + 14].set("show relation modifiers", &show_relation_modifiers);
    a[i + 15].set("show packages name in tab", &package_name_in_tab);
    a[i + 16].set("show classes and packages context", &show_context_mode);
    a[i + 17].set("automatic labels position", &auto_label_position);
    a[i + 18].set("show information note", &show_infonote);
    a[i + 19].set("show stereotype properties",
		  &show_stereotype_properties);
    a[i + 20].set("draw shadow", &shadow);
    break;
  default:
    a.resize(i + 21);
  
    a[i].set("class#drawing language", &drawing_language);
    a[i + 1].set("class#classes drawing mode", &class_drawing_mode);
    a[i + 2].set("class#hide classes attributes", &hide_attributes);
    a[i + 3].set("class#hide classes operations", &hide_operations);
    a[i + 4].set("class#show classes members full definition",
		 &show_full_members_definition);
    a[i + 5].set("class#show classes members visibility",
		 &show_members_visibility);
    a[i + 6].set("class#show classes members stereotype",
		 &show_members_stereotype);
    a[i + 7].set("class#show attribute multiplicity",
		 &show_members_multiplicity);
    a[i + 8].set("class#show attribute initialization",
		 &show_members_initialization);
    a[i + 9].set("class#show attribute modifiers",
		 &show_attribute_modifiers);
    a[i + 10].set("class#show operation parameter direction",
		  &show_parameter_dir);
    a[i + 11].set("class#show operation parameter name",
		  &show_parameter_name);
    a[i + 12].set("class#max members width",
		  &member_max_width);
    a[i + 13].set("class#draw all relations", &draw_all_relations);
    a[i + 14].set("class#show packages name in tab", &package_name_in_tab);
    a[i + 15].set("class#show relation modifiers", &show_relation_modifiers);
    a[i + 16].set("class#show classes and packages context", &show_context_mode);
    a[i + 17].set("class#automatic labels position", &auto_label_position);
    a[i + 18].set("class#show information note", &show_infonote);
    a[i + 19].set("class#show stereotype properties",
		  &show_stereotype_properties);
    a[i + 20].set("class#draw shadow", &shadow);
  }
}

// to update class canvas settings
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
    show_members_stereotype = (Uml3States) *((Uml3States *) a[index + 7].state);
  if (a[index + 8].name != 0)
    show_members_multiplicity = (Uml3States) *((Uml3States *) a[index + 8].state);
  if (a[index + 9].name != 0)
    show_attribute_modifiers = (Uml3States) *((Uml3States *) a[index + 9].state);
  if (a[index + 10].name != 0)
    show_members_initialization = (Uml3States) *((Uml3States *) a[index + 10].state);
  if (a[index + 11].name != 0)
    show_parameter_dir = (Uml3States) *((Uml3States *) a[index + 11].state);
  if (a[index + 12].name != 0)
    show_parameter_name = (Uml3States) *((Uml3States *) a[index + 12].state);
  if (a[index + 13].name != 0)
    member_max_width = *((char *) a[index + 13].state);
  if (a[index + 14].name != 0)
    show_infonote = (Uml3States) *((Uml3States *) a[index + 14].state);
  if (a[index + 15].name != 0)
    show_stereotype_properties = (Uml3States) *((Uml3States *) a[index + 15].state);
}


//

SimpleClassDiagramSettings::SimpleClassDiagramSettings() {
  class_drawing_mode = DefaultClassDrawingMode;
  show_context_mode = DefaultShowContextMode;
  show_stereotype_properties = UmlDefaultState;
}

void SimpleClassDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "class_drawing_mode " << stringify(class_drawing_mode)
    << " show_context_mode " << stringify(show_context_mode)
      << " show_stereotype_properties " << stringify(show_stereotype_properties);
}

void SimpleClassDiagramSettings::read(char * & st, char * & k) {
  if ((!strcmp(k, "class_view_mode")) ||	// old
      (!strcmp(k, "class_drawing_mode"))) {
    class_drawing_mode = drawing_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_context_mode")) {
    show_context_mode = context_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

void SimpleClassDiagramSettings::assign(const UseCaseDiagramSettings & from) {
  assign_default(class_drawing_mode, DefaultClassDrawingMode);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void SimpleClassDiagramSettings::complete(QArray<StateSpec> & a) {
  int i = a.size();
  
  a.resize(i + 3);
  
  a[i].set("drawing mode", &class_drawing_mode);
  a[i + 1].set("show context", &show_context_mode);
  a[i + 2].set("show stereotype properties", &show_stereotype_properties);
}

// to update class canvas settings
void SimpleClassDiagramSettings::set(QArray<StateSpec> & a, int index) {
  if (a[index].name != 0)
    class_drawing_mode = (ClassDrawingMode) *((ClassDrawingMode *) a[index].state);
  if (a[index + 1].name != 0)
    show_context_mode = (ShowContextMode) *((ShowContextMode *) a[index + 1].state);
  if (a[index + 2].name != 0)
    show_stereotype_properties = (Uml3States) *((Uml3States *) a[index + 2].state);
}


//

SequenceDiagramSettings::SequenceDiagramSettings() {
  show_full_operations_definition = UmlDefaultState;
  write_horizontally = UmlDefaultState;
  instances_drawing_mode = DefaultClassDrawingMode;
  drawing_language = DefaultDrawingLanguage;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void SequenceDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "show_full_operations_definition " << stringify(show_full_operations_definition)
    << " write_horizontally " << stringify(write_horizontally)
      << " class_drawing_mode " << stringify(instances_drawing_mode)
	<< " drawing_language " << stringify(drawing_language)
	  << " draw_all_relations " << stringify(draw_all_relations)
	    << " shadow " << stringify(shadow)
	      << " show_stereotype_properties " << stringify(show_stereotype_properties);
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
  if (!strcmp(k, "class_drawing_mode")) {
    instances_drawing_mode = drawing_mode(read_keyword(st));
    k = read_keyword(st);
  }  
  if (!strcmp(k, "drawing_language")) {
    drawing_language = ::drawing_language(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool SequenceDiagramSettings::complete(SequenceDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(show_full_operations_definition, UmlDefaultState);
  check_default(write_horizontally, UmlDefaultState);
  check_default(instances_drawing_mode, DefaultClassDrawingMode);  
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 7;
}

void SequenceDiagramSettings::assign(const SequenceDiagramSettings & from) {
  assign_default(show_full_operations_definition, UmlDefaultState);
  assign_default(write_horizontally, UmlDefaultState);
  assign_default(instances_drawing_mode, DefaultClassDrawingMode);  
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void SequenceDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);

  if (local) {
    a[i].set("drawing language", &drawing_language);
    a[i + 1].set("instances drawing mode", &instances_drawing_mode);
    a[i + 2].set("show operations full definition", &show_full_operations_definition);
    a[i + 3].set("write name:type horizontally", &write_horizontally);
    a[i + 4].set("draw all relations", &draw_all_relations);
    a[i + 5].set("show stereotype properties", &show_stereotype_properties);
    a[i + 6].set("draw shadow", &shadow);
  }
  else {
    a[i].set("sequence#drawing language", &drawing_language);
    a[i + 1].set("sequence#instances drawing mode", &instances_drawing_mode);
    a[i + 2].set("sequence#show operations full definition", &show_full_operations_definition);
    a[i + 3].set("sequence#write name:type horizontally", &write_horizontally);
    a[i + 4].set("sequence#draw all relations", &draw_all_relations);
    a[i + 5].set("sequence#show stereotype properties", &show_stereotype_properties);
    a[i + 6].set("sequence#draw shadow", &shadow);
  }
}

//

CollaborationDiagramSettings::CollaborationDiagramSettings() {
  show_full_operations_definition = UmlDefaultState;
  show_hierarchical_rank = UmlDefaultState;
  write_horizontally = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void CollaborationDiagramSettings::save(QTextStream & st) const {
    nl_indent(st);
    st << "show_full_operations_definition " << stringify(show_full_operations_definition)
      << " show_hierarchical_rank " << stringify(show_hierarchical_rank)
	<< " write_horizontally " << stringify(write_horizontally)
	  << " drawing_language " << stringify(drawing_language)
	    << " package_name_in_tab " << stringify(package_name_in_tab)
	      << " show_context " << stringify(show_context_mode)
		<< " draw_all_relations " << stringify(draw_all_relations)
		  << " shadow " << stringify(shadow)
		    << " show_stereotype_properties " << stringify(show_stereotype_properties);
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
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
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
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 9;
}

void CollaborationDiagramSettings::assign(const CollaborationDiagramSettings & from) {
  assign_default(show_full_operations_definition, UmlDefaultState);
  assign_default(show_hierarchical_rank, UmlDefaultState);
  assign_default(write_horizontally, UmlDefaultState);
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void CollaborationDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 9);
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
    a[i+6].set("draw all relations", &draw_all_relations);
    a[i+7].set("show stereotype properties", &show_stereotype_properties);
    a[i+8].set("draw shadow", &shadow);
  }
  else {
    a[i].set("communication#drawing language",
	     &drawing_language);
    a[i+1].set("communication#show operations full definition",
	       &show_full_operations_definition);
    a[i+2].set("communication#show hierarchical rank",
	       &show_hierarchical_rank);
    a[i+3].set("communication#write name:type horizontally",
	       &write_horizontally);
    a[i+4].set("communication#show packages name in tab", &package_name_in_tab);
    a[i+5].set("communication#show packages context", &show_context_mode);
    a[i+6].set("communication#draw all relations", &draw_all_relations);
    a[i+7].set("communication#show stereotype properties", &show_stereotype_properties);
    a[i+8].set("communication#draw shadow", &shadow);
  }
}

//

ObjectDiagramSettings::ObjectDiagramSettings() {
  write_horizontally = UmlDefaultState;
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void ObjectDiagramSettings::save(QTextStream & st) const {
    nl_indent(st);
    st << " write_horizontally " << stringify(write_horizontally)
      << " package_name_in_tab " << stringify(package_name_in_tab)
	<< " show_context " << stringify(show_context_mode)
	  << " auto_label_position " << stringify(auto_label_position)
	    << " draw_all_relations " << stringify(draw_all_relations)
	      << " shadow " << stringify(shadow)
		<< " show_stereotype_properties " << stringify(show_stereotype_properties);
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
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ObjectDiagramSettings::complete(ObjectDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(write_horizontally, UmlDefaultState);
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 7;
}

void ObjectDiagramSettings::assign(const ObjectDiagramSettings & from) {
  assign_default(write_horizontally, UmlDefaultState);
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void ObjectDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);
  if (local) {
    a[i].set("write name:type horizontally", &write_horizontally);
    a[i+1].set("packages name in tab", &package_name_in_tab);
    a[i+2].set("show package context", &show_context_mode);
    a[i+3].set("automatic labels position", &auto_label_position);
    a[i+4].set("draw all relations", &draw_all_relations);
    a[i+5].set("show stereotype properties", &show_stereotype_properties);
    a[i+6].set("draw shadow", &shadow);
  }
  else {
    a[i].set("object#write name:type horizontally", &write_horizontally);
    a[i+1].set("object#show packages name in tab", &package_name_in_tab);
    a[i+2].set("object#show packages context", &show_context_mode);
    a[i+3].set("object#automatic labels position", &auto_label_position);
    a[i+4].set("object#draw all relations", &draw_all_relations);
    a[i+5].set("object#show stereotype properties", &show_stereotype_properties);
    a[i+6].set("object#draw shadow", &shadow);
  }
}

//

UseCaseDiagramSettings::UseCaseDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  class_drawing_mode = DefaultClassDrawingMode;
  shadow = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void UseCaseDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " auto_label_position " << stringify(auto_label_position)
	<< " draw_all_relations " << stringify(draw_all_relations)
	  << " class_drawing_mode " << stringify(class_drawing_mode)
	    << " shadow " << stringify(shadow)
	      << " show_stereotype_properties " << stringify(show_stereotype_properties);
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
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "class_drawing_mode")) {
    class_drawing_mode = drawing_mode(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "shadow")) {
    shadow = state(read_keyword(st));
    k = read_keyword(st);
  }
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool UseCaseDiagramSettings::complete(UseCaseDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(package_name_in_tab, UmlDefaultState);
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(auto_label_position, UmlDefaultState);
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  check_default(class_drawing_mode, DefaultClassDrawingMode);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 7;
}

void UseCaseDiagramSettings::assign(const UseCaseDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  assign_default(class_drawing_mode, DefaultClassDrawingMode);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

bool UseCaseDiagramSettings::complete(SimpleClassDiagramSettings & result) const {
  unsigned done = 0;
  
  check_default(show_context_mode, DefaultShowContextMode);
  check_default(class_drawing_mode, DefaultClassDrawingMode);
  check_default(show_stereotype_properties, UmlDefaultState);
  
  return done == 3;
}

void UseCaseDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("automatic labels position", &auto_label_position);
    a[i + 3].set("draw all relations", &draw_all_relations);
    a[i + 4].set("class drawing mode", &class_drawing_mode);
    a[i + 5].set("show stereotype properties", &show_stereotype_properties);
    a[i + 6].set("draw shadow", &shadow);
  }
  else {
    a[i].set("use case#show packages name in tab", &package_name_in_tab);
    a[i + 1].set("use case#show packages context", &show_context_mode);
    a[i + 2].set("use case#automatic labels position", &auto_label_position);
    a[i + 3].set("use case#draw all relations", &draw_all_relations);
    a[i + 4].set("use case#class drawing mode", &class_drawing_mode);
    a[i + 5].set("use case#show stereotype properties", &show_stereotype_properties);
    a[i + 6].set("use case#draw shadow", &shadow);
  }
}

//

ComponentDiagramSettings::ComponentDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
}

void ComponentDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " auto_label_position " << stringify(auto_label_position)
	<< " draw_all_relations " << stringify(draw_all_relations)
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
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
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
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return componentdrawingsettings.complete(result.componentdrawingsettings)
    && (done == 5);
}

void ComponentDiagramSettings::assign(const ComponentDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  
  componentdrawingsettings.assign(from.componentdrawingsettings);
}

void ComponentDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 5);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("automatic labels position", &auto_label_position);
    a[i + 3].set("draw all relations", &draw_all_relations);
    a[i + 4].set("draw shadow", &shadow);
  }
  else {
    a[i].set("component#show packages name in tab", &package_name_in_tab);
    a[i + 1].set("component#show packages context", &show_context_mode);
    a[i + 2].set("component#automatic labels position", &auto_label_position);
    a[i + 3].set("component#draw all relations", &draw_all_relations);
    a[i + 4].set("component#draw shadow", &shadow);
  }
  componentdrawingsettings.complete(a, local, FALSE);
}

//

DeploymentDiagramSettings::DeploymentDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  write_horizontally = UmlDefaultState;
  auto_label_position = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
}

void DeploymentDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " write_horizontally " << stringify(write_horizontally)
	<< " auto_label_position " << stringify(auto_label_position)
	  << " draw_all_relations " << stringify(draw_all_relations)
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
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
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
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return componentdrawingsettings.complete(result.componentdrawingsettings)
    && (done == 6);
}

void DeploymentDiagramSettings::assign(const DeploymentDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(write_horizontally, UmlDefaultState);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);

  componentdrawingsettings.assign(from.componentdrawingsettings);
}

void DeploymentDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 6);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("write node instance horizontally", &write_horizontally);
    a[i + 3].set("automatic labels position", &auto_label_position);
    a[i + 4].set("draw all relations", &draw_all_relations);
    a[i + 5].set("draw shadow", &shadow);
  }
  else  {
    a[i].set("deployment#show packages name in tab", &package_name_in_tab);
    a[i + 1].set("deployment#show packages context", &show_context_mode);
    a[i + 2].set("deployment#write node instances horizontally", &write_horizontally);
    a[i + 3].set("deployment#automatic labels position", &auto_label_position);
    a[i + 4].set("deployment#draw all relations", &draw_all_relations);
    a[i + 5].set("deployment#draw shadow", &shadow);
  }
  
  componentdrawingsettings.complete(a, local, TRUE);
}

//

ComponentDrawingSettings::ComponentDrawingSettings() {
  draw_component_as_icon = UmlDefaultState;
  show_component_req_prov = UmlDefaultState;
  show_component_rea = UmlDefaultState;
  show_stereotype_properties = UmlDefaultState;
}

void ComponentDrawingSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "draw_component_as_icon " << stringify(draw_component_as_icon)
    << " show_component_req_prov " << stringify(show_component_req_prov)
      << " show_component_rea " << stringify(show_component_rea)
	<< " show_stereotype_properties " << stringify(show_stereotype_properties);
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
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ComponentDrawingSettings::complete(ComponentDrawingSettings & result) const {
  unsigned done = 0;
  
  check_default(draw_component_as_icon, UmlDefaultState);
  check_default(show_component_req_prov, UmlDefaultState);
  check_default(show_component_rea, UmlDefaultState);
  check_default(show_stereotype_properties, UmlDefaultState);
  
 return (done == 4);
}

void ComponentDrawingSettings::assign(const ComponentDrawingSettings & from) {
  assign_default(draw_component_as_icon, UmlDefaultState);
  assign_default(show_component_req_prov, UmlDefaultState);
  assign_default(show_component_rea, UmlDefaultState);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void ComponentDrawingSettings::assign(const ComponentDiagramSettings & from) {
  assign(from.componentdrawingsettings);
}

void ComponentDrawingSettings::complete(QArray<StateSpec> & a,
					bool local, bool depl) {
  int i = a.size();
  
  a.resize(i + 4);
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
  a[i + 3].set((local) ? "show stereotype properties"
		       : ((depl) ? "deployment#show stereotype properties"
				 : "component#show stereotype properties"),
	       &show_stereotype_properties);
}

//

StateDiagramSettings::StateDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  auto_label_position = UmlDefaultState;
  write_label_horizontally = UmlDefaultState;
  show_trans_definition = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
}

void StateDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " auto_label_position " << stringify(auto_label_position)
	<< " write_trans_label_horizontally " << stringify(write_label_horizontally)
	  << " show_trans_definition " << stringify(show_trans_definition)
	    << " draw_all_relations " << stringify(draw_all_relations)
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
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
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
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return statedrawingsettings.complete(result.statedrawingsettings)
    && (done == 7);
}

void StateDiagramSettings::assign(const StateDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(write_label_horizontally, UmlDefaultState);
  assign_default(show_trans_definition, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);
  
  statedrawingsettings.assign(from.statedrawingsettings);
}

void StateDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("automatic labels position", &auto_label_position);
    a[i + 3].set("write transition horizontally", &write_label_horizontally);
    a[i + 4].set("show transition definition", &show_trans_definition);
    a[i + 5].set("draw all relations", &draw_all_relations);
    a[i + 6].set("draw shadow", &shadow);
  }
  else {
    a[i].set("state#show packages name in tab", &package_name_in_tab);
    a[i + 1].set("state#show packages context", &show_context_mode);
    a[i + 2].set("state#automatic labels position", &auto_label_position);
    a[i + 3].set("state#write transition horizontally", &write_label_horizontally);
    a[i + 4].set("state#show transition definition", &show_trans_definition);
    a[i + 5].set("state#draw all relations", &draw_all_relations);
    a[i + 6].set("state#draw shadow", &shadow);
  }
  statedrawingsettings.complete(a, local);
}

//

StateDrawingSettings::StateDrawingSettings() {
  show_activities = UmlDefaultState;
  region_horizontally = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
  show_stereotype_properties = UmlDefaultState;
}

void StateDrawingSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "show_activities " << stringify(show_activities)
    << " region_horizontally " << stringify(region_horizontally)
      << " drawing_language " << stringify(drawing_language)
	<< " show_stereotype_properties " << stringify(show_stereotype_properties);
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
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool StateDrawingSettings::complete(StateDrawingSettings & result) const {
  unsigned done = 0;
  
  check_default(show_activities, UmlDefaultState);
  check_default(region_horizontally, UmlDefaultState);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(show_stereotype_properties, UmlDefaultState);
  
 return (done == 4);
}

void StateDrawingSettings::assign(const StateDrawingSettings & from) {
  assign_default(show_activities, UmlDefaultState);
  assign_default(region_horizontally, UmlDefaultState);
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void StateDrawingSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 4);

  if (local) {
    a[i].set("show state activities", &show_activities);
    a[i + 1].set("draw state's regions horizontally", &region_horizontally);
    a[i + 2].set("drawing language", &drawing_language);
    a[i + 3].set("show stereotype properties", &show_stereotype_properties);
    }
  else {
    a[i].set("state#show state activities", &show_activities);
    a[i + 1].set("state#draw state's regions horizontally", &region_horizontally);
    a[i + 2].set("state#drawing language", &drawing_language);
    a[i + 3].set("state#show stereotype properties", &show_stereotype_properties);
  }
}

//

ActivityDiagramSettings::ActivityDiagramSettings() {
  package_name_in_tab = UmlDefaultState;
  show_context_mode = DefaultShowContextMode;
  show_opaque_action_definition = UmlDefaultState;
  auto_label_position = UmlDefaultState;
  write_label_horizontally = UmlDefaultState;
  draw_all_relations = UmlDefaultState;
  shadow = UmlDefaultState;
}

void ActivityDiagramSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "package_name_in_tab " << stringify(package_name_in_tab)
    << " show_context " << stringify(show_context_mode)
      << " show_opaque_action_definition " << stringify(show_opaque_action_definition)
        << " auto_label_position " << stringify(auto_label_position)
	  << " write_flow_label_horizontally " << stringify(write_label_horizontally)
	    << " draw_all_relations " << stringify(draw_all_relations)
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
  if (!strcmp(k, "draw_all_relations")) {
    draw_all_relations = state(read_keyword(st));
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
  check_default(draw_all_relations, UmlDefaultState);
  check_default(shadow, UmlDefaultState);
  
  return activitydrawingsettings.complete(result.activitydrawingsettings)
    && (done == 7);
}

void ActivityDiagramSettings::assign(const ActivityDiagramSettings & from) {
  assign_default(package_name_in_tab, UmlDefaultState);
  assign_default(show_context_mode, DefaultShowContextMode);
  assign_default(show_opaque_action_definition, UmlDefaultState);
  assign_default(auto_label_position, UmlDefaultState);
  assign_default(write_label_horizontally, UmlDefaultState);
  assign_default(draw_all_relations, UmlDefaultState);
  assign_default(shadow, UmlDefaultState);

  activitydrawingsettings.assign(from.activitydrawingsettings);
}

void ActivityDiagramSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 7);
  if (local) {
    a[i].set("packages name in tab", &package_name_in_tab);
    a[i + 1].set("show package context", &show_context_mode);
    a[i + 2].set("automatic labels position", &auto_label_position);
    a[i + 3].set("write flow label horizontally", &write_label_horizontally);
    a[i + 4].set("show opaque action definition", &show_opaque_action_definition);
    a[i + 5].set("draw all relations", &draw_all_relations);
    a[i + 6].set("draw shadow", &shadow);
  }
  else {
    a[i].set("activity#show packages name in tab", &package_name_in_tab);
    a[i + 1].set("activity#show packages context", &show_context_mode);
    a[i + 2].set("activity#automatic labels position", &auto_label_position);
    a[i + 3].set("activity#write flow label horizontally", &write_label_horizontally);
    a[i + 4].set("activity#show opaque action definition", &show_opaque_action_definition);
    a[i + 5].set("activity#draw all relations", &draw_all_relations);
    a[i + 6].set("activity#draw shadow", &shadow);
  }
  activitydrawingsettings.complete(a, local);
}

//

ActivityDrawingSettings::ActivityDrawingSettings() {
  show_infonote = UmlDefaultState;
  drawing_language = DefaultDrawingLanguage;
  show_stereotype_properties = UmlDefaultState;
}

void ActivityDrawingSettings::save(QTextStream & st) const {
  nl_indent(st);
  st << "show_infonote " << stringify(show_infonote)
    << " drawing_language " << stringify(drawing_language)
      << " show_stereotype_properties " << stringify(show_stereotype_properties);
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
  if (!strcmp(k, "show_stereotype_properties")) {
    show_stereotype_properties = state(read_keyword(st));
    k = read_keyword(st);
  }
}

bool ActivityDrawingSettings::complete(ActivityDrawingSettings & result) const {
  unsigned done = 0;
  
  check_default(show_infonote, UmlDefaultState);
  check_default(drawing_language, DefaultDrawingLanguage);
  check_default(show_stereotype_properties, UmlDefaultState);
  
 return (done == 3);
}

void ActivityDrawingSettings::assign(const ActivityDrawingSettings & from) {
  assign_default(show_infonote, UmlDefaultState);
  assign_default(drawing_language, DefaultDrawingLanguage);
  assign_default(show_stereotype_properties, UmlDefaultState);
}

void ActivityDrawingSettings::complete(QArray<StateSpec> & a, bool local) {
  int i = a.size();
  
  a.resize(i + 3);

  if (local) {
    a[i].set("show information note", &show_infonote);
    a[i + 1].set("drawing language", &drawing_language);
    a[i + 2].set("show stereotype properties", &show_stereotype_properties);
  }
  else {
    a[i].set("activity#show information note", &show_infonote);
    a[i + 1].set("activity#drawing language", &drawing_language);
    a[i + 2].set("activity#show stereotype properties", &show_stereotype_properties);
  }
}

// to update activity element settings
void ActivityDrawingSettings::set(QArray<StateSpec> & a, int index) {
  // follow order of previous operation
  if (a[index].name != 0)
    show_infonote = (Uml3States) *((Uml3States *) a[index].state);
  if (a[index + 1].name != 0)
    drawing_language = (DrawingLanguage) *((DrawingLanguage *) a[index + 1].state);
  if (a[index + 2].name != 0)
    show_stereotype_properties = (Uml3States) *((Uml3States *) a[index + 2].state);
}
