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

#ifndef BROWSER_ACTIVITYOBJECT_H
#define BROWSER_ACTIVITYOBJECT_H

class QPixmap;

class ActivityObjectData;
class BrowserActivityDiagram;
class BrowserFlow;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserActivityObject : public BrowserNode, public Labeled<BrowserActivityObject> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserActivityObject> all;
    static QStringList its_default_stereotypes;
  
    ActivityObjectData * def;
    BrowserActivityDiagram * associated_diagram;
    
  protected:
    BrowserActivityObject(int id);
    
  public:
    BrowserActivityObject(QString s, BrowserNode * p, int id = 0);
    BrowserActivityObject(const BrowserActivityObject * model, BrowserNode * p);
    virtual ~BrowserActivityObject();
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    virtual bool may_contains_them(const QList<BrowserNode> & l,
				   bool & duplicable) const;
    static BrowserActivityObject * add_activityobject(BrowserNode * future_parent,
						      const char * s);
    static BrowserActivityObject * get_activityobject(BrowserNode * future_parent);
    virtual BasicData * add_relation(UmlCode, BrowserNode * end);
    const char * may_start() const;
    const char * may_connect(UmlCode &, const BrowserNode * dest) const;
    
    virtual const QPixmap* pixmap (int) const;

    virtual void menu();
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual bool allow_empty() const;
    virtual bool same_name(const QString & s, UmlCode type) const;
    
    virtual BrowserNode * get_associated() const;
    void set_associated_diagram(BrowserActivityDiagram *, bool on_read = FALSE);
    
    virtual void save(QTextStream &, bool ref, QString & warning);
    static BrowserActivityObject * read(char * &, char *, BrowserNode *);
    static BrowserActivityObject * read_ref(char * & st);
    static BrowserNode * get_it(const char * k, int id);

    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual bool api_compatible(unsigned v) const;
    
    virtual void referenced_by(QList<BrowserNode> &);
    static void compute_referenced_by(QList<BrowserNode> &, BrowserNode *);
    
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

