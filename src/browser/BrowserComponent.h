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

#ifndef BROWSER_COMPONENT_H
#define BROWSER_COMPONENT_H

class QPixmap;

#include "BrowserNode.h"
#include "Labeled.h"

class SimpleData;
class BrowserClass;
class BrowserComponentDiagram;

class BrowserComponent : public BrowserNode, public Labeled<BrowserComponent> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserComponent> all;
    static QStringList its_default_stereotypes;
    
    SimpleData * def;
    BrowserComponentDiagram * associated_diagram;
    QValueList<BrowserClass *> realized_classes;
    QValueList<BrowserClass *> provided_classes;
    QValueList<BrowserClass *> required_classes;
    
    void associate_class(BrowserClass * c);
  
  public:
    BrowserComponent(QString s, BrowserNode * p, int id = 0);
    BrowserComponent(const BrowserComponent * model, BrowserNode * p);
    BrowserComponent(int id);
    
    virtual ~BrowserComponent();
    
    virtual BrowserNode * get_associated() const;
    void set_associated_diagram(BrowserComponentDiagram *, bool on_read = FALSE);
    
    void set_associated_classes(const QValueList<BrowserClass *> & rz,
				const QValueList<BrowserClass *> & pr,
				const QValueList<BrowserClass *> & rq,
				bool on_read = FALSE);
    void remove_associated_class(BrowserClass *);
    const QValueList<BrowserClass *> & get_provided_classes() const {
      return provided_classes;
    }
    const QValueList<BrowserClass *> & get_required_classes() const {
      return required_classes;
    }
    const QValueList<BrowserClass *> & get_realized_classes() const {
      return realized_classes;
    }
    QValueList<BrowserClass *> get_all_provided_classes(bool sorted) const;
    QValueList<BrowserClass *> get_all_required_classes(bool sorted) const;
    
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual void menu();
    virtual void open(bool force_edit);
    virtual UmlCode get_type() const;
    virtual BasicData * get_data() const;
    virtual void on_delete();
    virtual const char * check_inherit(const BrowserNode * parent) const;
    virtual void save(QTextStream &, bool ref, QString & warning);
    static BrowserComponent * read_ref(char * &, const char *);
    static BrowserComponent * read(char * &, char *, BrowserNode *);
    static BrowserNode * get_it(const char * k, int id);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    
    static BrowserNodeList & instances(BrowserNodeList &, const char * st = 0, bool non_nested = FALSE);
    static BrowserComponent * get_component(BrowserNode * future_parent,
					    bool existing = FALSE);
    static BrowserComponent * add_component(BrowserNode * future_parent);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    
    virtual const QPixmap* pixmap (int) const;
    
    virtual void referenced_by(QList<BrowserNode> &);
    static void compute_referenced_by(QList<BrowserNode> &, BrowserClass *);
    
    static void init();
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(QTextStream &);
    
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
};

#endif

