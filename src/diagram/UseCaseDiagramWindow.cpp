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

#include <qapplication.h>
#include <qworkspace.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qspinbox.h>

#include "UmlWindow.h"
#include "UseCaseDiagramWindow.h"
#include "UseCaseDiagramView.h"
#include "BrowserUseCaseDiagram.h"
#include "UmlPixmap.h"
#include "myio.h"

const char * addactorText = "Click this button to add an <em>actor</em> in the diagram. <br><br>"
  "You can also drop the class from the <b>browser</b>.";
const char * addusecaseText = "Click this button to add an <em>use case</em> in the diagram. <br><br>"
  "You can also drop the use case from the <b>browser</b>.";
extern const char * addpackageText;
const char * addfragmentText = "Click this button to add a <em>fragment</em>.";
const char * addsubjectText = "Click this button to add a <em>subject</em>.";
const char * associationText = "Click this button to create an <em>association</em>";
extern const char * dependencyText;
const char * inheritText = "Click this button to create a <em>generalisation</em>";
const char * noteText = "Click this button to create a <em>note</em>";
const char * anchorText = "Click this button to create a connection between a "
  "<em>note</em> and any other item, or between an <em>association</em> "
  "and a <em>class</em> in case of an <em>association class</em>.";
extern const char * textText;

// id is an old ident in case of an import
UseCaseDiagramWindow::UseCaseDiagramWindow(const QString & s, BrowserUseCaseDiagram * b, int id)
    : DiagramWindow(b, s), view(0) {
  QToolBar * toolbar = new QToolBar(this, "use case operations");
  addToolBar(toolbar, "Use Case Operations", Top, TRUE);
  
  add_edit_button(toolbar);
  
  select =
    new QToolButton(*selectButton, "Select", QString::null,
		    this, SLOT(hit_select()), toolbar, "select");
  select->setToggleButton(TRUE);
  select->setOn(TRUE);
  current_button = UmlSelect;
  
  addClass =
    new QToolButton(*actorButton, "Actor", QString::null,
		    this, SLOT(hit_class()), toolbar, "actor");
  addClass->setToggleButton(TRUE);
  QWhatsThis::add(addClass, addactorText);
  
  addUseCase =
    new QToolButton(*usecaseButton, "Use Case", QString::null,
		    this, SLOT(hit_usecase()), toolbar, "use case");
  addUseCase->setToggleButton(TRUE);
  QWhatsThis::add(addUseCase, addusecaseText);
  
  addSubject
    = new QToolButton(*subjectButton, "Add Subject", QString::null,
		      this, SLOT(hit_subject()), toolbar, "add subject");
  addSubject->setToggleButton(TRUE);
  QWhatsThis::add(addSubject, addsubjectText);
  
  addPackage
    = new QToolButton(*packageButton, "Add Package", QString::null,
		      this, SLOT(hit_package()), toolbar, "add package");
  addPackage->setToggleButton(TRUE);
  QWhatsThis::add(addPackage, addpackageText);
  
  addFragment
    = new QToolButton(*fragmentButton, "Add Fragment", QString::null,
		      this, SLOT(hit_fragment()), toolbar, "add fragment");
  addFragment->setToggleButton(TRUE);
  QWhatsThis::add(addFragment, addfragmentText);
  
  association =
    new QToolButton(*associationButton, "Association", QString::null,
		    this, SLOT(hit_association()), toolbar, "association");
  association->setToggleButton(TRUE);
  QWhatsThis::add(association, associationText);
  
  directionalassociation =
    new QToolButton(*directionalAssociationButton, "Association", QString::null,
		    this, SLOT(hit_directionalassociation()), toolbar, "association");
  directionalassociation->setToggleButton(TRUE);
  QWhatsThis::add(directionalassociation, associationText);
  
  dependency =
    new QToolButton(*dependencyButton, "Dependency", QString::null,
		    this, SLOT(hit_dependency()), toolbar, "dependency");
  dependency->setToggleButton(TRUE);
  QWhatsThis::add(dependency, dependencyText);
  
  inherit =
    new QToolButton(*generalisationButton, "Generalisation", QString::null,
		    this, SLOT(hit_inherit()), toolbar, "generalisation");
  inherit->setToggleButton(TRUE);
  QWhatsThis::add(inherit, inheritText);
  
  note =
    new QToolButton(*noteButton, "Note", QString::null,
		    this, SLOT(hit_note()), toolbar, "note");
  note->setToggleButton(TRUE);
  QWhatsThis::add(note, noteText);
  
  anchor =
    new QToolButton(*anchorButton, "Anchor", QString::null,
		    this, SLOT(hit_anchor()), toolbar, "anchor");
  anchor->setToggleButton(TRUE);
  QWhatsThis::add(anchor, anchorText);
  
  text =
    new QToolButton(*textButton, "Text", QString::null,
		    this, SLOT(hit_text()), toolbar, "text");
  text->setToggleButton(TRUE);
  QWhatsThis::add(text, textText);
  
  toolbar->addSeparator();
  
  add_scale_cmd(toolbar);
  
  //
  
  view = new UseCaseDiagramView(this, canvas, (id != -1) ? id : b->get_ident());
  setFocusProxy(view);
  setCentralWidget(view);
  
  //qApp->setMainWidget(this);
  
  QWorkspace * w = UmlWindow::get_workspace();

  resize((w->width() * 4)/5, (w->height() * 4)/5);
  
  /*if (w->windowList().isEmpty())
    showMaximized();
  else*/
    show();
  
  view->preferred_size_zoom();
    
  //qApp->setMainWidget(0);
}

UseCaseDiagramWindow::~UseCaseDiagramWindow() {
  if (! no_save) {
    QString warning = "!";
    bool is_new = FALSE;
    
    save("d", warning, is_new);
    warning.remove(0, 1);	// removes !
    view->hide();
    
    if (!warning.isEmpty())
      warn(warning);
  }
  
  browser_node->on_close();
}

DiagramView * UseCaseDiagramWindow::get_view() const {
  return view;
}

void UseCaseDiagramWindow::hit_button(UmlCode c, QToolButton * b) {
  view->abort_line_construction();
  
  select->setOn(FALSE);
  addClass->setOn(FALSE);
  addPackage->setOn(FALSE);
  addFragment->setOn(FALSE);
  addSubject->setOn(FALSE);
  addUseCase->setOn(FALSE);
  association->setOn(FALSE);
  directionalassociation->setOn(FALSE);
  dependency->setOn(FALSE);
  inherit->setOn(FALSE);
  note->setOn(FALSE);
  anchor->setOn(FALSE);
  text->setOn(FALSE);
  
  b->setOn(TRUE);
  current_button = c;
}

void UseCaseDiagramWindow::hit_class() {
  hit_button(UmlClass, addClass);
}

void UseCaseDiagramWindow::hit_usecase() {
  hit_button(UmlUseCase, addUseCase);
}

void UseCaseDiagramWindow::hit_package() {
  hit_button(UmlPackage, addPackage);
}

void UseCaseDiagramWindow::hit_fragment() {
  hit_button(UmlFragment, addFragment);
}

void UseCaseDiagramWindow::hit_subject() {
  hit_button(UmlSubject, addSubject);
}

void UseCaseDiagramWindow::hit_association() {
  hit_button(UmlAssociation, association);
}

void UseCaseDiagramWindow::hit_directionalassociation() {
  hit_button(UmlDirectionalAssociation, directionalassociation);
}

void UseCaseDiagramWindow::hit_dependency() {
  hit_button(UmlDependency, dependency);
}

void UseCaseDiagramWindow::hit_inherit() {
  hit_button(UmlGeneralisation, inherit);
}

void UseCaseDiagramWindow::hit_note() {
  hit_button(UmlNote, note);
}

void UseCaseDiagramWindow::hit_anchor() {
  hit_button(UmlAnchor, anchor);
}

void UseCaseDiagramWindow::hit_text() {
  hit_button(UmlText, text);
}

