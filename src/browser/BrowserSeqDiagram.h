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

#ifndef BROWSER_SEQDIAGRAM_H
#define BROWSER_SEQDIAGRAM_H

#include "BrowserDiagram.h"
#include "Settings.h"

class QPixmap;
class SeqDiagramWindow;
class SimpleData;

class BrowserSeqDiagram : public BrowserDiagram {
  friend class StereotypesDialog;
  
  protected:
    static QList<BrowserSeqDiagram> imported;
    static QValueList<int> imported_ids;
    static QStringList its_default_stereotypes;
  
    SimpleData * def;
    SeqDiagramWindow * window;
    SequenceDiagramSettings settings;
    UmlColor note_color;
    UmlColor fragment_color;
    UmlColor duration_color;
    UmlColor continuation_color;
    UmlColor class_instance_color;
  
    BrowserSeqDiagram(BrowserSeqDiagram * model, BrowserNode * p);
    BrowserSeqDiagram(int id);
    void make();
    
  public:
    BrowserSeqDiagram(QString s, BrowserNode * p, int id = 0);
    virtual ~BrowserSeqDiagram();
      
    virtual void delete_it();
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
  
    virtual const QPixmap* pixmap (int) const;
  
    virtual void menu();
    virtual void open(bool);
    virtual void on_close();
    virtual void read_session(char * & st);
    virtual UmlCode get_type() const;
    virtual BasicData * get_data() const;
    virtual void set_name(const char * s);
    virtual void get_sequencediagramsettings(SequenceDiagramSettings &) const;
    virtual UmlColor get_color(UmlCode) const;
    virtual bool get_shadow() const;
    virtual bool get_classinstwritehorizontally(UmlCode k) const;
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual void save(QTextStream &, bool ref, QString & warning);
    static BrowserSeqDiagram * read(char * &, char *, BrowserNode *);
    static BrowserNode * get_it(const char * k, int id);
    
    void edit_settings();
    
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(QTextStream &);
    
    virtual void renumber(int phase);
    static void open_all();
    static void import();
    
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
};

#endif
