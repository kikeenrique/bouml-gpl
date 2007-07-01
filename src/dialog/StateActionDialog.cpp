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

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h> 

#include "StateActionDialog.h"
#include "StateActionData.h"
#include "BrowserStateAction.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "strutil.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"

QSize StateActionDialog::previous_size;

StateActionDialog::StateActionDialog(StateActionData * d)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), action(d) {
  d->browser_node->edit_start();
  
  if (d->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  setCaption("State Action dialog");
  visit = !hasOkButton();  

  BrowserNode * bn = action->browser_node;
  QGrid * grid;
  
  //
  // general tab
  //
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(action->get_stereotype()));
  if (!visit) {
    edstereotype->insertStringList(BrowserStateAction::default_stereotypes());
    edstereotype->setAutoCompletion(TRUE);
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
  
  QVBox * vtab = new QVBox(grid);
  new QLabel("description :", vtab);
  if (! visit)
    connect(new SmallPushButton("Editor", vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(bn->get_comment());
  //comment->setFont(font);
  
  addTab(grid, "Uml");

  // OCL
  grid = new QGrid(2, this);
  umltab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("behavior", grid);
  uml = new MultiLineEdit(grid);
  uml->setText(action->uml);
  if (visit)
    uml->setReadOnly(TRUE);
  
  addTab(grid, "Ocl");

  // CPP
  grid = new QGrid(2, this);
  cpptab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("behavior", grid);
  cpp = new MultiLineEdit(grid);
  cpp->setText(action->cpp);
  if (visit)
    cpp->setReadOnly(TRUE);
  
  addTab(grid, "C++");
  
  // Java
  grid = new QGrid(2, this);
  javatab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("behavior", grid);
  java = new MultiLineEdit(grid);
  java->setText(action->java);
  if (visit)
    java->setReadOnly(TRUE);
  
  addTab(grid, "Java");

  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(bn, grid, visit);
  addTab(grid, "Properties");
  
  //
    
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(change_tabs(QWidget *)));
}

StateActionDialog::~StateActionDialog() {
  action->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void StateActionDialog::change_tabs(QWidget * w) {
  if (!visit) {
    if (w == umltab)
      uml->setFocus();
    else if (w == cpptab)
      cpp->setFocus();
    else if (w == javatab)
      java->setFocus();
  }
}

void StateActionDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void StateActionDialog::edit_description() {
  edit(comment->text(), "state_description", action, TxtEdit,
       this, (post_edit) post_edit_description, edits);
}

void StateActionDialog::post_edit_description(StateActionDialog * d, QString s)
{
  d->comment->setText(s);
}

void StateActionDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = action->browser_node;
  
  action->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
  
  action->uml = uml->text();
  action->cpp = cpp->text();
  action->java = java->text();
  
  bn->set_comment(comment->text());
  UmlWindow::update_comment_if_needed(bn);
  
  kvtable->update(bn);
  
  bn->modified();
  bn->package_modified();
  action->modified();
  
  QTabDialog::accept();
}
