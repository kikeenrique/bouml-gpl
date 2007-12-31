// *************************************************************************
//
// Copyleft 2004-2008 Bruno PAGES  .
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

#include "UmlWindow.h"
#include "ClassDiagramWindow.h"
#include "ClassDiagramView.h"
#include "BrowserClassDiagram.h"
#include "UmlPixmap.h"
#include "myio.h"

const char * addclassText = "Click this button to add a <em>class</em> in the diagram. <br><br>"
"You can also drop the class from the <b>browser</b>.";
const char * addpackageText = "Click this button to add a <em>package</em> in the diagram. <br><br>"
"You can also drop the package from the <b>browser</b>.";
extern const char * addfragmentText;
extern const char * inheritText;
extern const char * noteText;
extern const char * anchorText;
const char * dependencyText = "Click this button to create a <em>dependency</em>";
extern const char * associationText;
const char * directionalassociationText = "Click this button to create an <em>unidirectional association</em>";
const char * aggregationText = "Click this button to create an <em>aggregation</em>";
const char * aggregationbyvalueText = "Click this button to create a <em>composition</em>";
const char * directionalaggregationText = "Click this button to create a <em>directional aggregation</em>";
const char * directionalaggregationbyvalueText = "Click this button to create a <em>directional composition</em>";
const char * realizeText = "Click this button to create a <em>realization</em>";
extern const char * textText;

// id is an old ident in case of an import
ClassDiagramWindow::ClassDiagramWindow(const QString & s, BrowserClassDiagram * b, int id)
    : DiagramWindow(b, s), view(0) {
  QToolBar * toolbar = new QToolBar(this, "class diagram operations");
  addToolBar(toolbar, "Static Class Diagram Operations", Top, TRUE);
  
  add_edit_button(toolbar);
  
  select =
    new QToolButton(*selectButton, "Select", QString::null,
		    this, SLOT(hit_select()), toolbar, "select");
  select->setToggleButton(TRUE);
  select->setOn(TRUE);
  current_button = UmlSelect;
  
  addClass
    = new QToolButton(*classButton, "Add Class", QString::null,
		      this, SLOT(hit_class()), toolbar, "add class");
  addClass->setToggleButton(TRUE);
  QWhatsThis::add(addClass, addclassText);
  
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
  
  inherit =
    new QToolButton(*generalisationButton, "Generalisation", QString::null,
		    this, SLOT(hit_inherit()), toolbar, "generalisation");
  inherit->setToggleButton(TRUE);
  QWhatsThis::add(inherit, inheritText);
  
  realize =
    new QToolButton(*realizeButton, "Realize", QString::null,
		    this, SLOT(hit_realize()), toolbar, "realize");
  realize->setToggleButton(TRUE);
  QWhatsThis::add(realize, realizeText);
  
  dependency =
    new QToolButton(*dependencyButton, "Dependency", QString::null,
		    this, SLOT(hit_dependency()), toolbar, "dependency");
  dependency->setToggleButton(TRUE);
  QWhatsThis::add(dependency, dependencyText);
  
  association =
    new QToolButton(*associationButton, "Association", QString::null,
		    this, SLOT(hit_association()), toolbar, "association");
  association->setToggleButton(TRUE);
  QWhatsThis::add(association, associationText);
  
  directionalassociation =
    new QToolButton(*directionalAssociationButton, "Directional Association", QString::null,
		    this, SLOT(hit_directionalassociation()), toolbar, "directional association");
  directionalassociation->setToggleButton(TRUE);
  QWhatsThis::add(directionalassociation, directionalassociationText);
  
  aggregation =
    new QToolButton(*aggregationButton, "Aggregation", QString::null,
		    this, SLOT(hit_aggregation()), toolbar, "aggregation");
  aggregation->setToggleButton(TRUE);
  QWhatsThis::add(aggregation, aggregationText);

  aggregationbyvalue =
    new QToolButton(*aggregationByValueButton, "Composition", QString::null,
		    this, SLOT(hit_aggregationbyvalue()), toolbar, "composition");
  aggregationbyvalue->setToggleButton(TRUE);
  QWhatsThis::add(aggregationbyvalue, aggregationbyvalueText);
  
  directionalaggregation =
    new QToolButton(*directionalAggregationButton, "Directional Aggregation", QString::null,
		    this, SLOT(hit_directionalaggregation()), toolbar, "directional aggregation");
  directionalaggregation->setToggleButton(TRUE);
  QWhatsThis::add(directionalaggregation, directionalaggregationText);
  
  directionalaggregationbyvalue =
    new QToolButton(*directionalAggregationByValueButton, "Directional Composition", QString::null,
		    this, SLOT(hit_directionalaggregationbyvalue()), toolbar, "directional composition");
  directionalaggregationbyvalue->setToggleButton(TRUE);
  QWhatsThis::add(directionalaggregationbyvalue, directionalaggregationbyvalueText);
  
  note =
    new QToolButton(*noteButton, "Note", QString::null,
		    this, SLOT(hit_note()), toolbar, "note");
  note->setToggleButton(TRUE);
  QWhatsThis::add(note, noteText);
  
  anchor =
    new QToolButton(*anchorButton, "Anchor / Class association", QString::null,
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
  
  view = new ClassDiagramView(this, canvas, (id != -1) ? id : b->get_ident());
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

ClassDiagramWindow::~ClassDiagramWindow() {
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

DiagramView * ClassDiagramWindow::get_view() const {
  return view;
}

void ClassDiagramWindow::hit_button(UmlCode c, QToolButton * b) {
  view->abort_line_construction();
  
  select->setOn(FALSE);
  addClass->setOn(FALSE);
  addPackage->setOn(FALSE);
  addFragment->setOn(FALSE);
  inherit->setOn(FALSE);
  dependency->setOn(FALSE);
  association->setOn(FALSE);
  directionalassociation->setOn(FALSE);
  aggregation->setOn(FALSE);
  aggregationbyvalue->setOn(FALSE);
  directionalaggregation->setOn(FALSE);
  directionalaggregationbyvalue->setOn(FALSE);
  realize->setOn(FALSE);
  note->setOn(FALSE);
  anchor->setOn(FALSE);
  text->setOn(FALSE);
  
  b->setOn(TRUE);
  current_button = c;
}

void ClassDiagramWindow::hit_class() {
  hit_button(UmlClass, addClass);
}

void ClassDiagramWindow::hit_package() {
  hit_button(UmlPackage, addPackage);
}

void ClassDiagramWindow::hit_fragment() {
  hit_button(UmlFragment, addFragment);
}

void ClassDiagramWindow::hit_inherit() {
  hit_button(UmlGeneralisation, inherit);
}

void ClassDiagramWindow::hit_dependency() {
  hit_button(UmlDependency, dependency);
}

void ClassDiagramWindow::hit_association() {
  hit_button(UmlAssociation, association);
}

void ClassDiagramWindow::hit_directionalassociation() {
  hit_button(UmlDirectionalAssociation, directionalassociation);
}

void ClassDiagramWindow::hit_aggregation() {
  hit_button(UmlAggregation, aggregation);
}

void ClassDiagramWindow::hit_aggregationbyvalue() {
  hit_button(UmlAggregationByValue, aggregationbyvalue);
}

void ClassDiagramWindow::hit_directionalaggregation() {
  hit_button(UmlDirectionalAggregation, directionalaggregation);
}

void ClassDiagramWindow::hit_directionalaggregationbyvalue() {
  hit_button(UmlDirectionalAggregationByValue, directionalaggregationbyvalue);
}

void ClassDiagramWindow::hit_realize() {
  hit_button(UmlRealize, realize);
}

void ClassDiagramWindow::hit_note() {
  hit_button(UmlNote, note);
}

void ClassDiagramWindow::hit_anchor() {
  hit_button(UmlAnchor, anchor);
}

void ClassDiagramWindow::hit_text() {
  hit_button(UmlText, text);
}

