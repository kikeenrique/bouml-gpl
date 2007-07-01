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
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h> 
#include <qvbox.h> 

#include "SimpleRelationDialog.h"
#include "SimpleRelationData.h"
#include "BrowserSimpleRelation.h"
#include "UmlWindow.h"
#include "KeyValueTable.h"
#include "UmlPixmap.h"
#include "DialogUtil.h"
#include "strutil.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"

QSize SimpleRelationDialog::previous_size;

SimpleRelationDialog::SimpleRelationDialog(SimpleRelationData * r)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), rel(r) {  
  r->browser_node->edit_start();
  
  if (r->browser_node->is_writable())
    setCancelButton();
  else {
    setOkButton(QString::null);
    setCancelButton("Close");
  }

  bool visit = !hasOkButton();
  
  // general tab
  
  BrowserNode * bn = rel->get_browser_node();
  QGrid * grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);

  switch (rel->get_type()) {
  case UmlInherit:
    setCaption("Generalisation dialog");
    break;
  case UmlDependOn:
    setCaption("Dependency dialog");
    break;
  default:
    setCaption("unknown relation dialog");
    break;
  }
  
  new QLabel("from : ", grid);
  new QLabel(rel->get_start_node()->full_name(TRUE), grid);
  new QLabel("to : ", grid);
  new QLabel(rel->get_end_node()->full_name(TRUE), grid);
  
  new QLabel("stereotype : ", grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(rel->get_stereotype()));
  if (! visit) {
    edstereotype->insertStringList(rel->get_start_node()
				   ->default_stereotypes(rel->get_type(),
							 rel->get_end_node()));
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

void SimpleRelationDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

SimpleRelationDialog::~SimpleRelationDialog() {
  rel->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
}

void SimpleRelationDialog::edit_description() {
  edit(comment->text(), "simplerelation_description",
       rel, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void SimpleRelationDialog::post_edit_description(SimpleRelationDialog * d, QString s)
{
  d->comment->setText(s);
}

void SimpleRelationDialog::accept() {
  if (!check_edits(edits))
    return;
    
  BrowserNode * bn = rel->get_browser_node();
  
  rel->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
  bn->set_comment(comment->text());
  UmlWindow::update_comment_if_needed(bn);
  
  kvtable->update(bn);
  
  bn->package_modified();
  rel->modified();
  
  QTabDialog::accept();
}
