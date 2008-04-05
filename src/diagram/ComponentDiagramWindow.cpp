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





#include <qapplication.h>
#include <qworkspace.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qspinbox.h>

#include "UmlWindow.h"
#include "ComponentDiagramWindow.h"
#include "ComponentDiagramView.h"
#include "BrowserComponentDiagram.h"
#include "UmlPixmap.h"
#include "myio.h"

const char * addcomponentText = "Click this button to add a <em>component</em> in the diagram. <br><br>"
"You can also drop the component from the <b>browser</b>.";
extern const char * addpackageText;
extern const char * addfragmentText;
extern const char * dependencyText;
extern const char * inheritText;
extern const char * noteText;
extern const char * anchorText;
extern const char * textText;
static const char * requiredText = "Click this button to show an interface required by a <em>component</em>.";
static const char * providedText = "Click this button to show an interface provided by a <em>component</em>.";

// id is an old ident in case of an import
ComponentDiagramWindow::ComponentDiagramWindow(const QString & s, BrowserComponentDiagram * b, int id)
    : DiagramWindow(b, s), view(0) {
  QToolBar * toolbar = new QToolBar(this, "component operations");
  addToolBar(toolbar, "Component Operations", Top, TRUE);
  
  add_edit_button(toolbar);
  
  select =
    new QToolButton(*selectButton, "Select", QString::null,
		    this, SLOT(hit_select()), toolbar, "select");
  select->setToggleButton(TRUE);
  select->setOn(TRUE);
  current_button = UmlSelect;
  
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
  
  addComponent =
    new QToolButton(*componentButton, "Add Component", QString::null,
		    this, SLOT(hit_component()), toolbar, "add component");
  addComponent->setToggleButton(TRUE);
  QWhatsThis::add(addComponent, addcomponentText);
  
  inherit =
    new QToolButton(*generalisationButton, "Inheritance", QString::null,
		    this, SLOT(hit_inherit()), toolbar, "inheritance");
  inherit->setToggleButton(TRUE);
  QWhatsThis::add(inherit, inheritText);
  
  dependency =
    new QToolButton(*dependencyButton, "Dependency", QString::null,
		    this, SLOT(hit_dependency()), toolbar, "dependency");
  dependency->setToggleButton(TRUE);
  QWhatsThis::add(dependency, dependencyText);

  required =
    new QToolButton(*requiredButton, "Required interface", QString::null,
		    this, SLOT(hit_required()), toolbar, "required interface");
  required->setToggleButton(TRUE);
  QWhatsThis::add(required, requiredText);

  provided =
    new QToolButton(*providedButton, "Provided interface", QString::null,
		    this, SLOT(hit_provided()), toolbar, "provided interface");
  provided->setToggleButton(TRUE);
  QWhatsThis::add(provided, providedText);

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
  
  view = new ComponentDiagramView(this, canvas, (id != -1) ? id : b->get_ident());
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

ComponentDiagramWindow::~ComponentDiagramWindow() {
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

DiagramView * ComponentDiagramWindow::get_view() const {
  return view;
}

void ComponentDiagramWindow::hit_button(UmlCode c, QToolButton * b) {
  view->abort_line_construction();
  
  select->setOn(FALSE);
  addPackage->setOn(FALSE);
  addFragment->setOn(FALSE);
  addComponent->setOn(FALSE);
  inherit->setOn(FALSE);
  dependency->setOn(FALSE);
  required->setOn(FALSE);
  provided->setOn(FALSE);
  note->setOn(FALSE);
  anchor->setOn(FALSE);
  text->setOn(FALSE);
  
  b->setOn(TRUE);
  current_button = c;
}

void ComponentDiagramWindow::hit_package() {
  hit_button(UmlPackage, addPackage);
}

void ComponentDiagramWindow::hit_fragment() {
  hit_button(UmlFragment, addFragment);
}

void ComponentDiagramWindow::hit_component() {
  hit_button(UmlComponent, addComponent);
}

void ComponentDiagramWindow::hit_inherit() {
  hit_button(UmlGeneralisation, inherit);
}

void ComponentDiagramWindow::hit_dependency() {
  hit_button(UmlDependency, dependency);
}

void ComponentDiagramWindow::hit_required() {
  hit_button(UmlRequired, required);
}

void ComponentDiagramWindow::hit_provided() {
  hit_button(UmlProvided, provided);
}

void ComponentDiagramWindow::hit_note() {
  hit_button(UmlNote, note);
}

void ComponentDiagramWindow::hit_anchor() {
  hit_button(UmlAnchor, anchor);
}

void ComponentDiagramWindow::hit_text() {
  hit_button(UmlText, text);
}

