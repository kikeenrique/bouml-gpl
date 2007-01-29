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

#ifndef DEPLOYMENTDIAGRAMVIEW_H
#define DEPLOYMENTDIAGRAMVIEW_H

#include "DiagramView.h"

class DeploymentDiagramWindow;
class DiagramItem;
class UmlCanvas;

class DeploymentDiagramView : public DiagramView {
  Q_OBJECT

  public:
    DeploymentDiagramView(QWidget * parent, UmlCanvas * canvas, int id);
  
    virtual void menu(const QPoint&);
    virtual void read(char *, char * k);
    virtual void save(QTextStream & st, QString & warning, bool copy) const;
    
  private:
    DeploymentDiagramWindow * window() {
      return (DeploymentDiagramWindow *) parent();
    };
  
  protected:
    virtual void contentsMousePressEvent(QMouseEvent *);
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dropEvent(QDropEvent *);
};

#endif
