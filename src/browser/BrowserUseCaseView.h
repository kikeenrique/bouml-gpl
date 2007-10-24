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

#ifndef BROWSER_USECASEVIEW_H
#define BROWSER_USECASEVIEW_H

#include <qintdict.h> 

class QPixmap;
class SimpleData;

#include "BrowserNode.h"
#include "Labeled.h"
#include "Settings.h"

class BrowserUseCaseView : public BrowserNode, public Labeled<BrowserUseCaseView> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserUseCaseView> all;
    static QStringList its_default_stereotypes;
    
    SimpleData * def;
    UseCaseDiagramSettings usecasediagram_settings;
    SequenceDiagramSettings sequencediagram_settings;
    CollaborationDiagramSettings collaborationdiagram_settings;
    ObjectDiagramSettings objectdiagram_settings;
    StateDiagramSettings statediagram_settings;
    ActivityDiagramSettings activitydiagram_settings;
    // note : does not have class settings because classes defined under
    //	      a use case cannot be generated
    UmlColor note_color;
    UmlColor fragment_color;
    UmlColor subject_color;
    UmlColor duration_color;
    UmlColor continuation_color;
    UmlColor usecase_color;
    UmlColor package_color;
    UmlColor classinstance_color;
    UmlColor state_color;
    UmlColor stateaction_color;
    UmlColor activity_color;
    UmlColor activityregion_color;
    UmlColor activityaction_color;
    UmlColor parameterpin_color;
    
  protected:
    void exec_menu_choice(int rank);
    
  public:
    BrowserUseCaseView(QString s, BrowserNode * p, int id = 0);
    BrowserUseCaseView(const BrowserUseCaseView * model, BrowserNode * p);
    virtual ~BrowserUseCaseView();
  
    virtual const QPixmap* pixmap (int) const;
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual int get_identifier() const;
    virtual bool may_contains_them(const QList<BrowserNode> &,
				   bool & duplicable) const;
    virtual BrowserNode * container(UmlCode) const; // container for class, state machine and activity
    virtual BasicData * get_data() const;
    virtual void get_usecasediagramsettings(UseCaseDiagramSettings &) const;
    virtual void get_sequencediagramsettings(SequenceDiagramSettings &) const;
    virtual void get_collaborationdiagramsettings(CollaborationDiagramSettings &) const;
    virtual void get_objectdiagramsettings(ObjectDiagramSettings &) const;
    virtual void get_statedrawingsettings(StateDrawingSettings & r) const;
    virtual void get_statediagramsettings(StateDiagramSettings &) const;
    virtual void get_activitydrawingsettings(ActivityDrawingSettings &) const;
    virtual void get_activitydiagramsettings(ActivityDiagramSettings &) const;
    virtual UmlColor get_color(UmlCode) const;
    virtual bool get_shadow(UmlCode) const;
    virtual bool get_draw_all_relations(UmlCode) const;
    virtual bool get_classinstwritehorizontally(UmlCode k) const;
    virtual bool get_auto_label_position(UmlCode who) const;
    virtual bool get_write_label_horizontally(UmlCode who) const;
    virtual bool get_show_trans_definition(UmlCode who) const;
    virtual bool get_show_opaque_action_definition(UmlCode who) const;
    virtual DrawingLanguage get_language(UmlCode who) const;
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual void save(QTextStream &, bool ref, QString & warning);
    
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);

    static BrowserUseCaseView * add_use_case_view(BrowserNode * future_parent);
    
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(QTextStream &);
    
    static BrowserUseCaseView * read(char * &, char *, BrowserNode *, bool);
    static BrowserNode * get_it(const char * k, int id);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
};

#endif
