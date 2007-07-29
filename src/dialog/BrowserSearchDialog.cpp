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

#include <qlayout.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qgroupbox.h> 
#include <qbuttongroup.h>
#include <qapplication.h>

#include "BrowserSearchDialog.h"
#include "BrowserView.h"
#include "UmlDesktop.h"
#include "BrowserPackage.h"
#include "UmlEnum.h"
#include "DialogUtil.h"

int BrowserSearchDialog::saved_kind;
QString BrowserSearchDialog::saved_ed;
bool BrowserSearchDialog::saved_case_sensitive;
bool BrowserSearchDialog::saved_even_deleted;
bool BrowserSearchDialog::saved_name = TRUE;
bool BrowserSearchDialog::saved_comment;
bool BrowserSearchDialog::saved_decldefbody;
QSize BrowserSearchDialog::previous_size;

static const struct {
  const char * lbl;
  UmlCode k;
} Kinds[] = {
  { "<Any>", UmlCodeSup },
  { "activity", UmlActivity },
  { "activity action", UmlActivityAction },
  { "activity diagram", UmlActivityDiagram },
  { "activity node", UmlActivityNode },
  { "activity object", UmlActivityObject },
  { "activity parameter", UmlParameter },
  { "attribute", UmlAttribute },
  { "artifact ", UmlArtifact },
  { "class ", UmlClass },
  { "class instance", UmlClassInstance },
  { "class diagram", UmlClassDiagram },
  { "class view", UmlClassView },
  { "collaboration diagram", UmlColDiagram },
  { "component", UmlComponent },
  { "component diagram", UmlComponentDiagram },
  { "component view", UmlComponentView },
  { "deployment diagram", UmlDeploymentDiagram },
  { "deployment view", UmlDeploymentView },
  { "extra class member", UmlExtraMember },
  { "flow", UmlFlow },
  { "interruptible activity region", UmlInterruptibleActivityRegion },
  { "expansion region", UmlExpansionRegion },
  { "node", UmlDeploymentNode },
  { "object diagram", UmlObjectDiagram },
  { "operation", UmlOperation },
  { "package", UmlPackage },
  { "parameter set", UmlParameterSet },
  { "pin", UmlActivityPin },
  { "relation", UmlRelations },
  { "sequence diagram", UmlSeqDiagram },
  { "state", UmlState },
  { "state action", UmlStateAction, },
  { "state diagram", UmlTransition },  
  { "state region", UmlRegion },
  { "use case", UmlUseCase },  
  { "use case diagram", UmlUseCaseDiagram },
  { "use case view", UmlUseCaseView }
};

BrowserSearchDialog::BrowserSearchDialog()
    : QDialog(0, "Browser search", TRUE) {
  setCaption("Browser search");
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  
  vbox->setMargin(5);
  
  QGridLayout * gl = new QGridLayout(vbox, 4, 2, 5/*space*/);

  gl->addWidget(new QLabel("Kind", this), 0, 0, Qt::AlignLeft);
  kind = new QComboBox(FALSE, this);
  for (int index = 0; index != sizeof(Kinds)/sizeof(*Kinds); index += 1)
    kind->insertItem(Kinds[index].lbl);
  kind->setCurrentItem(saved_kind);
  gl->addWidget(kind, 0, 1);
  
  ed = new LineEdit(this);
  ed->setText(saved_ed);
  gl->addWidget(new QLabel("Containing", this), 1, 0, Qt::AlignLeft);
  gl->addWidget(ed, 1, 1);
  ed->setFocus();
  
  QHBox * hb = new QHBox(this);
  
  gl->addWidget(hb, 2, 1);
  
  QGroupBox * gb = new QGroupBox(2, QGroupBox::Horizontal, hb);
  
  case_sensitive = new QCheckBox("case sensitive", gb);
  case_sensitive->setChecked(saved_case_sensitive);
  even_deleted = new QCheckBox("even deleted", gb);
  even_deleted->setChecked(saved_even_deleted);

  QButtonGroup * bg = new QButtonGroup(3, QGroupBox::Horizontal, hb);
  
  bg->setExclusive(TRUE);
  for_name = new QRadioButton("name", bg);
  for_name->setChecked(saved_name);
  for_comment = new QRadioButton("description", bg);
  for_comment->setChecked(saved_comment);
  for_decldefbody = new QRadioButton("declaration/definition/body", bg);
  for_decldefbody->setChecked(saved_decldefbody);
  
  gl->addWidget(new QLabel("Result", this), 3, 0, Qt::AlignLeft);
  results = new QComboBox(FALSE, this);
  gl->addWidget(results, 3, 1);

  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * search_b = new QPushButton("Search", this);
  QPushButton * select_b = new QPushButton("Select", this);
  QPushButton * close_b = new QPushButton("Close", this);
  
  search_b->setDefault(TRUE);
  
  hbox->addWidget(search_b);
  hbox->addWidget(select_b);
  hbox->addWidget(close_b);
  
  connect(search_b, SIGNAL(clicked()), this, SLOT(search()));
  connect(select_b, SIGNAL(clicked()), this, SLOT(select()));
  connect(close_b, SIGNAL(clicked()), this, SLOT(reject()));
  
  if ((saved_kind != 0) || !saved_ed.isEmpty())
    search();
}

void BrowserSearchDialog::polish() {
  QDialog::polish();
  //setMaximumHeight(sizeHint().height());
  //setMaximumWidth(29*UmlDesktop::width()/30)
  UmlDesktop::limitsize_move(this, previous_size, 29.0/30, 0.9);
}

BrowserSearchDialog::~BrowserSearchDialog() {
  saved_kind = kind->currentItem();
  saved_ed = ed->text();
  saved_case_sensitive = case_sensitive->isChecked();
  saved_even_deleted = even_deleted->isChecked();
  saved_name = for_name->isChecked();
  saved_comment = for_comment->isChecked();
  saved_decldefbody = for_decldefbody->isChecked();
  previous_size = size();
}

void BrowserSearchDialog::search() {
  QApplication::setOverrideCursor(Qt::waitCursor);

  nodes.clear();
  if (for_name->isChecked())
    nodes.search(BrowserView::get_project(), Kinds[kind->currentItem()].k,
		 ed->text(), case_sensitive->isChecked(),
		 even_deleted->isChecked(), TRUE);
  else if (for_comment->isChecked())
    nodes.search(BrowserView::get_project(), Kinds[kind->currentItem()].k,
		 ed->text(), case_sensitive->isChecked(),
		 even_deleted->isChecked(), FALSE);
  else
    nodes.search_ddb(BrowserView::get_project(), Kinds[kind->currentItem()].k,
		     ed->text(), case_sensitive->isChecked(),
		     even_deleted->isChecked());
  nodes.sort();
  
  results->clear();
  
  BrowserNode * bn;
  
  for (bn = nodes.first(); bn != 0; bn = nodes.next())
    results->insertItem(*(bn->pixmap(0)), bn->full_name(TRUE));

  QApplication::restoreOverrideCursor();
}

void BrowserSearchDialog::select() {
  if (results->count() != 0)
    nodes.at(results->currentItem())->select_in_browser();
}

