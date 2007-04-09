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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include "BrowserDiagram.h"
#include "BrowserClassDiagram.h"
#include "BrowserSeqDiagram.h"
#include "BrowserColDiagram.h"
#include "BrowserUseCaseDiagram.h"
#include "BrowserStateDiagram.h"
#include "BrowserActivityDiagram.h"
#include "BrowserComponentDiagram.h"
#include "BrowserDeploymentDiagram.h"
#include "BrowserObjectDiagram.h"
#include "BrowserActivityDiagram.h"
#include "BrowserView.h"
#include "UmlWindow.h"
#include "myio.h"

// use the same extension for all the diagrams : must share dict
IdDict<BrowserDiagram> BrowserDiagram::all(257);

BrowserDiagram::BrowserDiagram(QString s, BrowserNode * parent, int id)
    : BrowserNode(s, parent), Labeled<BrowserDiagram>(all, id) {
  canvas_size = UmlWindow::default_format();
  
  if (id == 0) {
    // delete files in case one re-use the id of
    // a previously deleted diagram
    QString fn = 
      BrowserView::get_dir().absFilePath(QString::number(get_ident()) + ".d");
    
    QFile::remove(fn);
    QFile::remove(fn + "iagram");
  }
}

BrowserDiagram::BrowserDiagram(int id)
    : BrowserNode(), Labeled<BrowserDiagram>(all, id) {
  canvas_size = IsoA4;
}

BrowserDiagram::~BrowserDiagram() {
}

void BrowserDiagram::package_modified() {
  is_modified = TRUE;
  BrowserNode::package_modified();
}

bool BrowserDiagram::get_auto_label_position(UmlCode) const {
  return TRUE;
}

void BrowserDiagram::read_stereotypes(char * & st, char * & k)
{
  BrowserClassDiagram::read_stereotypes(st, k);		// updates k
  BrowserSeqDiagram::read_stereotypes(st, k);		// updates k
  BrowserColDiagram::read_stereotypes(st, k);		// updates k
  BrowserUseCaseDiagram::read_stereotypes(st, k);	// updates k
  if (read_file_format() >= 21)
    BrowserStateDiagram::read_stereotypes(st, k);	// updates k
  if (read_file_format() >= 25)
    BrowserActivityDiagram::read_stereotypes(st, k);	// updates k
  BrowserComponentDiagram::read_stereotypes(st, k);	// updates k
  BrowserDeploymentDiagram::read_stereotypes(st, k);	// updates k
}

void BrowserDiagram::save_stereotypes(QTextStream & st)
{
  BrowserClassDiagram::save_stereotypes(st);
  BrowserSeqDiagram::save_stereotypes(st);
  BrowserColDiagram::save_stereotypes(st);
  BrowserUseCaseDiagram::save_stereotypes(st);
  BrowserStateDiagram::save_stereotypes(st);
  BrowserActivityDiagram::save_stereotypes(st);
  BrowserComponentDiagram::save_stereotypes(st);
  BrowserDeploymentDiagram::save_stereotypes(st);
}
 
void BrowserDiagram::save() {
  // diagram is modified
  char * diagram_def = 
    read_definition(get_ident(), "d");
  
  if (diagram_def != 0) {
    save_definition(get_ident(), "diagram", diagram_def, is_new);
    delete [] diagram_def;
  }
}

void BrowserDiagram::clear(bool old)
{
  all.clear(old);
}

void BrowserDiagram::update_idmax_for_root()
{
  all.update_idmax_for_root();
}
    
void BrowserDiagram::import()
{
  BrowserClassDiagram::import();
  BrowserComponentDiagram::import();
  BrowserDeploymentDiagram::import();
  BrowserUseCaseDiagram::import();
  BrowserSeqDiagram::import();
  BrowserColDiagram::import();  
  BrowserStateDiagram::import();
  BrowserObjectDiagram::import();
  BrowserActivityDiagram::import();
}

BrowserNode * BrowserDiagram::read_diagram_ref(char * & st)
{
  BrowserNode * bn;
  char * k = read_keyword(st);
  
  if (((bn = BrowserClassDiagram::read(st, k, 0)) == 0) &&
      ((bn = BrowserColDiagram::read(st, k, 0)) == 0) &&
      ((bn = BrowserSeqDiagram::read(st, k, 0)) == 0) &&
      ((bn = BrowserUseCaseDiagram::read(st, k, 0)) == 0) &&
      ((bn = BrowserComponentDiagram::read(st, k, 0)) == 0) &&
      ((bn = BrowserDeploymentDiagram::read(st, k, 0)) == 0) &&
      ((bn = BrowserStateDiagram::read(st, k, 0)) == 0) &&
      ((bn = BrowserObjectDiagram::read(st, k, 0)) == 0) &&
      ((bn = BrowserActivityDiagram::read(st, k, 0)) == 0))
    unknown_keyword(k);
  
  return bn;  
}
