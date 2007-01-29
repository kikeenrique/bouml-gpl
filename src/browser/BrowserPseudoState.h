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

#ifndef BROWSER_PSEUDOSTATE_H
#define BROWSER_PSEUDOSTATE_H

class QPixmap;
class SimpleData;
class BrowserTransition;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserPseudoState : public BrowserNode, public Labeled<BrowserPseudoState> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserPseudoState> all;
    static QStringList its_default_stereotypes;
  
    UmlCode kind;
    SimpleData * def;
    
  protected:
    BrowserPseudoState(int id);
  
    void exec_menu_choice(int rank);
    
  public:
    BrowserPseudoState(UmlCode c, QString s, BrowserNode * p, SimpleData * d, int id = 0);
    BrowserPseudoState(const BrowserPseudoState * model, BrowserNode * p);
    virtual ~BrowserPseudoState();
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    static BrowserPseudoState * add_pseudostate(BrowserNode * future_parent, UmlCode c);
    static BrowserPseudoState * add_pseudostate(BrowserNode * future_parent,
						UmlCode c, const char * name);
    static BrowserPseudoState * get_pseudostate(BrowserNode * future_parent, UmlCode c);
    virtual BasicData * add_relation(UmlCode, BrowserNode * end);
    BrowserTransition * add_transition(BrowserNode * end);
    const char *may_start() const;
    const char * may_connect(const BrowserNode * dest) const;
    
    virtual const QPixmap* pixmap (int) const;

    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    
    virtual void save(QTextStream &, bool ref, QString & warning);
    static BrowserPseudoState * read(char * &, char *, BrowserNode *);
    static BrowserPseudoState * read_ref(char * & st);
    static BrowserNode * get_it(const char * k, int id);

    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    
    virtual void referenced_by(QList<BrowserNode> &);
    static void compute_referenced_by(QList<BrowserNode> &, BrowserPseudoState *);
    
    bool allow_empty() const;
    static bool allow_empty(UmlCode c);

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

