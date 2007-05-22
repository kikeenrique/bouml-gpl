// *************************************************************************
//
// Copyleft 2004-2007 Bruno PAGES  .
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

#ifndef BROWSER_CLASSVIEW_H
#define BROWSER_CLASSVIEW_H

#include <qintdict.h> 

class QPixmap;
class BrowserDeploymentView;
class SimpleData;

#include "BrowserNode.h"
#include "Labeled.h"
#include "Settings.h"

class BrowserClassView : public BrowserNode, public Labeled<BrowserClassView> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserClassView> all;
    static QStringList its_default_stereotypes;
  
    SimpleData * def;
    ClassDiagramSettings classdiagram_settings;
    SequenceDiagramSettings sequencediagram_settings;
    CollaborationDiagramSettings collaborationdiagram_settings;
    ObjectDiagramSettings objectdiagram_settings;
    StateDiagramSettings statediagram_settings;
    ActivityDiagramSettings activitydiagram_settings;
    ClassSettings class_settings;
    UmlColor class_color;
    UmlColor note_color;
    UmlColor fragment_color;
    UmlColor package_color;
    UmlColor duration_color;
    UmlColor continuation_color;
    UmlColor state_color;
    UmlColor stateaction_color;
    UmlColor activity_color;
    UmlColor activityregion_color;
    UmlColor activityaction_color;
    UmlColor parameterpin_color;
    BrowserDeploymentView * associated_deployment_view;
    
    void exec_menu_choice(int rank);

  public:
    BrowserClassView(QString s, BrowserNode * p, int id = 0);
    BrowserClassView(const BrowserClassView * model, BrowserNode * p);
    virtual ~BrowserClassView();
    
    virtual const QPixmap* pixmap (int) const;
  
    BrowserNode * get_class();
    void add_class_diagram();
    void add_object_diagram();
    void add_sequence_diagram();
    void add_collaboration_diagram();
    virtual BrowserNode * get_associated() const;
    void set_associated_deploymentview(BrowserDeploymentView *, bool on_read = FALSE);
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual bool may_contains_them(const QList<BrowserNode> &,
				   bool & duplicable) const;
    virtual BrowserNode * container(UmlCode) const; // container for class, state machine and activity
    virtual BasicData * get_data() const;
    virtual void get_classdiagramsettings(ClassDiagramSettings &) const;
    virtual void get_sequencediagramsettings(SequenceDiagramSettings &) const;
    virtual void get_collaborationdiagramsettings(CollaborationDiagramSettings &) const;
    virtual void get_objectdiagramsettings(ObjectDiagramSettings &) const;
    virtual void get_statedrawingsettings(StateDrawingSettings & r) const;
    virtual void get_statediagramsettings(StateDiagramSettings &) const;
    virtual void get_activitydrawingsettings(ActivityDrawingSettings &) const;
    virtual void get_activitydiagramsettings(ActivityDiagramSettings &) const;
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual UmlColor get_color(UmlCode) const;
    virtual bool get_shadow(UmlCode) const;
    virtual bool get_draw_all_relations(UmlCode) const;
    virtual UmlVisibility get_visibility(UmlCode) const;
    virtual bool get_auto_label_position(UmlCode who) const;
    virtual bool get_write_label_horizontally(UmlCode who) const;
    virtual bool get_show_trans_definition(UmlCode who) const;
    virtual bool get_show_opaque_action_definition(UmlCode who) const;
    virtual DrawingLanguage get_language(UmlCode who) const;
    virtual void on_delete();
    virtual void save(QTextStream &, bool ref, QString & warning);
    
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
    
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(QTextStream &);

    static BrowserClassView * read(char * &, char *, BrowserNode *, bool);
    static BrowserNode * get_it(const char * k, int id);

    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
};

#endif
