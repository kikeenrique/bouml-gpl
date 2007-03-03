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

#include <qgrid.h> 
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h> 
#include <qvbox.h> 

#include "ClassViewDialog.h"
#include "BasicData.h"
#include "BrowserClassView.h"
#include "BrowserDeploymentView.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "KeyValueTable.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "strutil.h"

QSize ClassViewDialog::previous_size;

ClassViewDialog::ClassViewDialog(BasicData * nd)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), data(nd) {
  nd->get_browser_node()->edit_start();
  
  if (nd->get_browser_node()->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  setCaption("class view dialog");

  bool visit = !hasOkButton();
  
  // general tab
  
  BrowserClassView * bn = (BrowserClassView *) data->get_browser_node();
  QGrid * grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);

  new QLabel("name : ", grid);
  edname = new LineEdit(bn->get_name(), grid);
  edname->setReadOnly(visit);
    
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(data->get_stereotype()));
  if (!visit) {
    edstereotype->insertStringList(BrowserClassView::default_stereotypes());
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
  
  BrowserNode * bcv = bn->get_associated();
  
  if (visit) {
    if ((bcv != 0) && !bcv->deletedp()) {
      new QLabel("deployment\nview : ", grid);
      deploymentview = new QComboBox(FALSE, grid);
      
      BrowserNode * bcv = bn->get_associated();
      
      deploymentview->insertItem(bcv->full_name(TRUE));
      deploymentview->setCurrentItem(0);
    }
    else
      deploymentview = 0;
  }
  else {
    BrowserDeploymentView::instances(deploymentviews);
    if (!deploymentviews.isEmpty()) {
      QStringList deploymentview_names;
      
      deploymentviews.full_names(deploymentview_names);
      new QLabel("deployment\nview : ", grid);
      deploymentview = new QComboBox(FALSE, grid);
      deploymentview->insertItem("");
      deploymentview->insertStringList(deploymentview_names);
      
      if (bcv != 0) {
	if (bcv->deletedp()) {
	  QString fn = bcv->full_name(TRUE);
	  
	  deploymentview->insertItem(fn);
	  deploymentviews.append(bcv);
	  deploymentview->setCurrentItem(deploymentviews.count());
	}
	else
	  deploymentview->setCurrentItem(deploymentviews.find(bcv) + 1);
      }
      else
	deploymentview->setCurrentItem(0);
    }
    else
      deploymentview = 0;
  }
    
  QVBox * vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (!visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(bn->get_comment());
  QFont font = comment->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  comment->setFont(font);
  
  addTab(grid, "Uml");
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(bn, grid, visit);
  addTab(grid, "Properties");
					   }

void ClassViewDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

ClassViewDialog::~ClassViewDialog() {
  data->get_browser_node()->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void ClassViewDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       data, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void ClassViewDialog::post_edit_description(ClassViewDialog * d, QString s) {
  d->comment->setText(s);
}

void ClassViewDialog::accept() {
  if (!check_edits(edits))
    return;
    
  QString s = edname->text().stripWhiteSpace();
  BrowserClassView * bn = (BrowserClassView *) data->get_browser_node();
  
  if ((s != bn->get_name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, bn->get_type(),
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical("Error", edname->text() + "\n\nillegal name or already used");
  else {  
    bn->set_name(s);
    data->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    if (deploymentview != 0) {
      int index = deploymentview->currentItem();
      
      bn->set_associated_deploymentview((index == 0)
				       ? 0 // "" : no deploymentview
				       : ((BrowserDeploymentView *) deploymentviews.at(index - 1)));
    }
    
    bn->set_comment(comment->text());
    UmlWindow::set_commented(bn);
    
    kvtable->update(bn);
  
    data->modified();
    bn->package_modified();
    
    QTabDialog::accept();
  }
}
