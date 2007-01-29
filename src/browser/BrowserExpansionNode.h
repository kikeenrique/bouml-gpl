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

#ifndef BROWSEREXPANSIONNODE_H
#define BROWSEREXPANSIONNODE_H

class QPixmap;
class ActivityObjectData;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserExpansionNode : public BrowserNode, public Labeled<BrowserExpansionNode> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserExpansionNode> all;
    static QStringList its_default_stereotypes;
  
    ActivityObjectData * def;
    
  protected:
    BrowserExpansionNode(int id);
  
    void exec_menu_choice(int rank);
    
  public:
    BrowserExpansionNode(QString s, BrowserNode * p, int id = 0);
    BrowserExpansionNode(const BrowserExpansionNode * model, BrowserNode * p);
    virtual ~BrowserExpansionNode();
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    
    virtual const QPixmap* pixmap (int) const;

    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual bool allow_empty() const;
    
    BrowserExpansionNode * get_expansionnode(BrowserNode * future_parent);
    virtual BasicData * add_relation(UmlCode, BrowserNode * end);
    const char * may_start() const;
    const char * may_connect(const BrowserNode * dest) const;

    virtual void save(QTextStream &, bool ref, QString & warning);
    static BrowserExpansionNode * read_ref(char * &);
    static BrowserExpansionNode * read(char * &, char *, BrowserNode *);
    static BrowserNode * get_it(const char * k, int id);

    static BrowserExpansionNode * get_pin(BrowserNode * future_parent);

    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    
    virtual void referenced_by(QList<BrowserNode> &);
    static void compute_referenced_by(QList<BrowserNode> &, BrowserNode *);
    
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
    QString drag_key() const;
    QString drag_postfix() const;
    static QString drag_key(BrowserNode * p);
    
    static void init();
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(QTextStream &);
};

#endif

