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

#ifndef BROWSER_STATE_H
#define BROWSER_STATE_H

class QPixmap;
class StateData;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserStateDiagram;
class BrowserTransition;

class BrowserState : public BrowserNode, public Labeled<BrowserState> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserState> all;
    static QStringList its_default_stereotypes;
  
    StateData * def;
    BrowserStateDiagram * associated_diagram;
    
  protected:
    BrowserState(int id);
    
  public:
    BrowserState(QString s, BrowserNode * p, StateData * d, int id = 0);
    BrowserState(const BrowserState * model, BrowserNode * p);
    virtual ~BrowserState();
  
    virtual bool may_contains_them(const QList<BrowserNode> & l,
				   bool & duplicable) const;
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    static BrowserNodeList & instances(BrowserNodeList &, bool sort);
    static BrowserState * add_state(BrowserNode * future_parent, bool machine);
    static BrowserState * add_state(BrowserNode * future_parent, const char * name);
    static BrowserState * get_state(BrowserNode * parent);
    void add_state_diagram();
    BrowserTransition * add_transition(BrowserNode * end);
    virtual BasicData * add_relation(UmlCode, BrowserNode * end);
    
    virtual const QPixmap* pixmap (int) const;

    virtual void menu();
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    bool sub_state_of(BrowserState * st) const;
    
    virtual BrowserNode * get_associated() const;
    void set_associated_diagram(BrowserStateDiagram *, bool on_read = FALSE);
    
    virtual void save(QTextStream &, bool ref, QString & warning);
    static BrowserState * read(char * &, char *, BrowserNode *);
    static BrowserState * read_ref(char * & st);
    static BrowserNode * get_it(const char * k, int id);

    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual bool api_compatible(unsigned v) const;
    
    virtual void referenced_by(QList<BrowserNode> &);
    static void compute_referenced_by(QList<BrowserNode> &, BrowserState *);
    
    static void init();
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(QTextStream &);
    
    static QString drag_key(BrowserNode * p);
    virtual QString drag_key() const;
    virtual QString drag_postfix() const;
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
};

#endif

