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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <qobject.h>
#include <qtextstream.h> 

#include "StateSpec.h"

class DrawingSettings : public QObject {
  // defined just to have its signal
  Q_OBJECT
    
  private:
    static bool just_changed;

    DrawingSettings(){};
    void is_modified();
  
  public:
    static DrawingSettings * instance();
    static void modified();
    static bool just_modified();
  
  signals:
    void changed();
};

class ClassSettings {
  public:
    UmlVisibility attribute_visibility;
    UmlVisibility relation_visibility;
    UmlVisibility operation_visibility;
    
    ClassSettings();
    
    bool edit(UmlCode who);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class ClassDiagramSettings {
  public:
    Uml3States hide_attributes;
    Uml3States hide_operations;
    Uml3States show_full_members_definition;
    Uml3States show_members_visibility;
    Uml3States show_parameter_dir;
    Uml3States package_name_in_tab;
    ClassDrawingMode class_drawing_mode;
    DrawingLanguage drawing_language;
    ShowContextMode show_context_mode;
    Uml3States auto_label_position;
    Uml3States draw_all_relations;
    Uml3States shadow;
    
    ClassDiagramSettings();
    
    bool complete(ClassDiagramSettings & result) const;
    void complete(QArray<StateSpec> & a, UmlCode who);
    void set(QArray<StateSpec> & a, int index);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class SequenceDiagramSettings {
  public:
    Uml3States show_full_operations_definition;
    Uml3States write_horizontally;
    DrawingLanguage drawing_language;
    Uml3States draw_all_relations;
    Uml3States shadow;
  
    SequenceDiagramSettings();
    
    bool complete(SequenceDiagramSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class CollaborationDiagramSettings {
  public:
    Uml3States show_full_operations_definition;
    Uml3States show_hierarchical_rank;
    Uml3States write_horizontally;
    DrawingLanguage drawing_language;
    Uml3States package_name_in_tab;
    ShowContextMode show_context_mode;
    Uml3States draw_all_relations;
    Uml3States shadow;
  
    CollaborationDiagramSettings();
    
    bool complete(CollaborationDiagramSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class ObjectDiagramSettings {
  public:
    Uml3States write_horizontally;
    Uml3States package_name_in_tab;
    ShowContextMode show_context_mode;
    Uml3States auto_label_position;
    Uml3States draw_all_relations;
    Uml3States shadow;
  
    ObjectDiagramSettings();
    
    bool complete(ObjectDiagramSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class UseCaseDiagramSettings {
  public:
    Uml3States package_name_in_tab;
    ShowContextMode show_context_mode;
    Uml3States auto_label_position;
    Uml3States draw_all_relations;
    Uml3States shadow;
  
    UseCaseDiagramSettings();
    
    bool complete(UseCaseDiagramSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class ComponentDrawingSettings {
  public:
    Uml3States draw_component_as_icon;
    Uml3States show_component_req_prov;
    Uml3States show_component_rea;
    
    ComponentDrawingSettings();
    
    bool complete(ComponentDrawingSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local, bool depl);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class ComponentDiagramSettings {
  public:
    Uml3States package_name_in_tab;
    ShowContextMode show_context_mode;
    Uml3States auto_label_position;
    Uml3States draw_all_relations;
    Uml3States shadow;
    ComponentDrawingSettings componentdrawingsettings;
  
    ComponentDiagramSettings();
    
    bool complete(ComponentDiagramSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class DeploymentDiagramSettings {
  public:
    Uml3States package_name_in_tab;
    ShowContextMode show_context_mode;
    Uml3States write_horizontally;
    Uml3States auto_label_position;
    Uml3States draw_all_relations;
    Uml3States shadow;
    ComponentDrawingSettings componentdrawingsettings;
  
    DeploymentDiagramSettings();
    
    bool complete(DeploymentDiagramSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class StateDrawingSettings {
  public:
    Uml3States show_activities;
    Uml3States region_horizontally;
    DrawingLanguage drawing_language;
    
    StateDrawingSettings();
    
    bool complete(StateDrawingSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class StateDiagramSettings {
  public:
    Uml3States package_name_in_tab;
    ShowContextMode show_context_mode;
    Uml3States auto_label_position;
    Uml3States write_label_horizontally;
    Uml3States show_trans_definition;
    Uml3States draw_all_relations;
    Uml3States shadow;
    StateDrawingSettings statedrawingsettings;
  
    StateDiagramSettings();
    
    bool complete(StateDiagramSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class ActivityDrawingSettings {
  public:
    Uml3States show_infonote;
    DrawingLanguage drawing_language;
    
    ActivityDrawingSettings();
    
    bool complete(ActivityDrawingSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void set(QArray<StateSpec> & a, int index);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

class ActivityDiagramSettings {
  public:
    Uml3States package_name_in_tab;
    ShowContextMode show_context_mode;
    Uml3States auto_label_position;
    Uml3States write_label_horizontally;
    Uml3States show_opaque_action_definition;
    Uml3States draw_all_relations;
    Uml3States shadow;
    ActivityDrawingSettings activitydrawingsettings;
  
    ActivityDiagramSettings();
    
    bool complete(ActivityDiagramSettings & result) const;
    void complete(QArray<StateSpec> & a, bool local);
    void save(QTextStream &) const;
    void read(char * &, char * &);
};

#endif
