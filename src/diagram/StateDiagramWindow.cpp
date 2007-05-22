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
#include "StateDiagramWindow.h"
#include "StateDiagramView.h"
#include "BrowserStateDiagram.h"
#include "UmlPixmap.h"
#include "myio.h"

const char * addstateText = "Click this button to add a <em>state</em> in the diagram. <br><br>"
"You can also drop the state from the <b>browser</b>.";
extern const char * addpackageText;
extern const char * addfragmentText;
extern const char * noteText;
extern const char * anchorText;
//extern const char * dependencyText;
extern const char * textText;
const char * addentrypointText = "Click this button to add a <em>entry point</em> in the diagram. <br><br>"
"You can also drop the <em>entry point</em> from the <b>browser</b>.";
const char * addexitpointText = "Click this button to add a <em>exit point</em> in the diagram. <br><br>"
"You can also drop the <em>exit point</em> from the <b>browser</b>.";
const char * addinitialText = "Click this button to add a <em>initial pseudo state</em> in the diagram. <br><br>"
"You can also drop the <em>initial pseudo state</em> from the <b>browser</b>.";
const char * addfinalText = "Click this button to add a <em>final state</em> in the diagram. <br><br>"
"You can also drop the <em>final state</em> from the <b>browser</b>.";
const char * addterminateText = "Click this button to add a <em>terminate node</em> in the diagram. <br><br>"
"You can also drop the <em>terminate node</em> from the <b>browser</b>.";
const char * adddeephistoryText = "Click this button to add a <em>deep history</em> in the diagram. <br><br>"
"You can also drop the <em>deep history</em> from the <b>browser</b>.";
const char * addshallowhistoryText = "Click this button to add a <em>shallow history</em> in the diagram. <br><br>"
"You can also drop the <em></em> from the <b>browser</b>."; 
const char * addjunctionText = "Click this button to add a <em>junction</em> in the diagram. <br><br>"
"You can also drop the <em>junction</em> from the <b>browser</b>.";
const char * addchoiceText = "Click this button to add a <em>choice</em> in the diagram. <br><br>"
"You can also drop the <em>choice</em> from the <b>browser</b>.";
const char * addforkText = "Click this button to add a <em>fork</em> in the diagram. <br><br>"
"You can also drop the <em>foek</em> from the <b>browser</b>.";
const char * addjoinText = "Click this button to add a <em>join</em> in the diagram. <br><br>"
"You can also drop the <em>join</em> from the <b>browser</b>.";
const char * addtransitionText = "Click this button to add a <em>transition</em> in the diagram. <br><br>"
"You can also drop the <em>transition</em> from the <b>browser</b>.";
const char * addregionText = "Click this button to add a <em>region</em> in a <em>state</em>.";
const char * addactionText = "Click this button to add an <em>action</em> in a <em>state</em>.";
const char * addsignalinText = "Click this button to add a <em>receive signal action</em> in a <em>state</em>.";
const char * addsignaloutText = "Click this button to add a <em>send signal action</em> in a <em>state</em>.";

// id is an old ident in case of an import
StateDiagramWindow::StateDiagramWindow(const QString & s, BrowserStateDiagram * b, int id)
    : DiagramWindow(b, s), view(0) {
  QToolBar * toolbar = new QToolBar(this, "state operations");
  addToolBar(toolbar, "State Operations", Top, TRUE);
  
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
  
  addState =
    new QToolButton(*stateButton, "Add State", QString::null,
		    this, SLOT(hit_state()), toolbar, "add state");
  addState->setToggleButton(TRUE);
  QWhatsThis::add(addState, addstateText);
  
  addRegion =
    new QToolButton(*regionButton, "Add Region", QString::null,
		    this, SLOT(hit_region()), toolbar, "add region");
  addRegion->setToggleButton(TRUE);
  QWhatsThis::add(addRegion, addregionText);
  
  addInitial =
    new QToolButton(*initialButton, "Add Initial pseudo state", QString::null,
		    this, SLOT(hit_initial()), toolbar, "add initial");
  addInitial->setToggleButton(TRUE);
  QWhatsThis::add(addInitial, addinitialText);

  addEntryPoint =
    new QToolButton(*entrypointButton, "Add Entry Point", QString::null,
		    this, SLOT(hit_entryPoint()), toolbar, "add entry point");
  addEntryPoint->setToggleButton(TRUE);
  QWhatsThis::add(addEntryPoint, addentrypointText);

  addFinal =
    new QToolButton(*finalButton, "Add Final state", QString::null,
		    this, SLOT(hit_final()), toolbar, "add final");
  addFinal->setToggleButton(TRUE);
  QWhatsThis::add(addFinal, addfinalText);

  addExitPoint =
    new QToolButton(*exitpointButton, "Add Exit Point", QString::null,
		    this, SLOT(hit_exitPoint()), toolbar, "add exit point");
  addExitPoint->setToggleButton(TRUE);
  QWhatsThis::add(addExitPoint, addexitpointText);

  addTerminate =
    new QToolButton(*terminateButton, "Add Terminate node", QString::null,
		    this, SLOT(hit_terminate()), toolbar, "add terminate");
  addTerminate->setToggleButton(TRUE);
  QWhatsThis::add(addTerminate, addterminateText);

  addDeepHistory =
    new QToolButton(*deephistoryButton, "Add Deep History", QString::null,
		    this, SLOT(hit_deepHistory()), toolbar, "add deep history");
  addDeepHistory->setToggleButton(TRUE);
  QWhatsThis::add(addDeepHistory, adddeephistoryText);

  addShallowHistory =
    new QToolButton(*shallowhistoryButton, "Add Shallow History", QString::null,
		    this, SLOT(hit_shallowHistory()), toolbar, "add shallow history");
  addShallowHistory->setToggleButton(TRUE);
  QWhatsThis::add(addShallowHistory, addshallowhistoryText);

  addJunction =
    new QToolButton(*junctionButton, "Add Junction", QString::null,
		    this, SLOT(hit_junction()), toolbar, "add junction");
  addJunction->setToggleButton(TRUE);
  QWhatsThis::add(addJunction, addjunctionText);

  addChoice =
    new QToolButton(*choiceButton, "Add Choice", QString::null,
		    this, SLOT(hit_choice()), toolbar, "add choice");
  addChoice->setToggleButton(TRUE);
  QWhatsThis::add(addChoice, addchoiceText);

  addFork =
    new QToolButton(*forkButton, "Add Fork", QString::null,
		    this, SLOT(hit_fork()), toolbar, "add fork");
  addFork->setToggleButton(TRUE);
  QWhatsThis::add(addFork, addforkText);

  addJoin =
    new QToolButton(*joinButton, "Add Join", QString::null,
		    this, SLOT(hit_join()), toolbar, "add join");
  addJoin->setToggleButton(TRUE);
  QWhatsThis::add(addJoin, addjoinText);

  addAction =
    new QToolButton(*actionButton, "Add Action", QString::null,
		    this, SLOT(hit_action()), toolbar, "add action");
  addAction->setToggleButton(TRUE);
  QWhatsThis::add(addAction, addactionText);

  addSignalIn =
    new QToolButton(*signalinButton, "Add receive signal action", QString::null,
		    this, SLOT(hit_signalin()), toolbar, "add receive signal action");
  addSignalIn->setToggleButton(TRUE);
  QWhatsThis::add(addSignalIn, addsignalinText);

  addSignalOut =
    new QToolButton(*signaloutButton, "Add send signal action", QString::null,
		    this, SLOT(hit_signalout()), toolbar, "add send signal action");
  addSignalOut->setToggleButton(TRUE);
  QWhatsThis::add(addSignalOut, addsignaloutText);

  addTransition =
    new QToolButton(*directionalAssociationButton, "Add Transition", QString::null,
		    this, SLOT(hit_transition()), toolbar, "add transition");
  addTransition->setToggleButton(TRUE);
  QWhatsThis::add(addTransition, addtransitionText);
  
  /*
  dependency =
    new QToolButton(*dependencyButton, "Dependency", QString::null,
		    this, SLOT(hit_dependency()), toolbar, "dependency");
  dependency->setToggleButton(TRUE);
  QWhatsThis::add(dependency, dependencyText);
  */
  
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
  
  view = new StateDiagramView(this, canvas, (id != -1) ? id : b->get_ident());
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

StateDiagramWindow::~StateDiagramWindow() {
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

DiagramView * StateDiagramWindow::get_view() const {
  return view;
}

void StateDiagramWindow::hit_button(UmlCode c, QToolButton * b) {
  view->abort_line_construction();
  
  select->setOn(FALSE);
  addPackage->setOn(FALSE);
  addFragment->setOn(FALSE);
  addState->setOn(FALSE);
  addRegion->setOn(FALSE);
  addEntryPoint->setOn(FALSE);
  addExitPoint->setOn(FALSE);
  addInitial->setOn(FALSE);
  addFinal->setOn(FALSE);
  addTerminate->setOn(FALSE);
  addDeepHistory->setOn(FALSE);
  addShallowHistory->setOn(FALSE); 
  addJunction->setOn(FALSE);
  addChoice->setOn(FALSE);
  addFork->setOn(FALSE);
  addJoin->setOn(FALSE);
  addAction->setOn(FALSE);
  addSignalIn->setOn(FALSE);
  addSignalOut->setOn(FALSE);
  addTransition->setOn(FALSE);
  //dependency->setOn(FALSE);
  note->setOn(FALSE);
  anchor->setOn(FALSE);
  text->setOn(FALSE);
  
  b->setOn(TRUE);
  current_button = c;
}

void StateDiagramWindow::hit_package() {
  hit_button(UmlPackage, addPackage);
}

void StateDiagramWindow::hit_fragment() {
  hit_button(UmlFragment, addFragment);
}

void StateDiagramWindow::hit_state() {
  hit_button(UmlState, addState);
}

void StateDiagramWindow::hit_region() {
  hit_button(UmlRegion, addRegion);
}

void StateDiagramWindow::hit_entryPoint() {
  hit_button(EntryPointPS, addEntryPoint);
}

void StateDiagramWindow::hit_exitPoint() {
  hit_button(ExitPointPS, addExitPoint);
}

void StateDiagramWindow::hit_initial() {
  hit_button(InitialPS, addInitial);
}

void StateDiagramWindow::hit_final() {
  hit_button(FinalPS, addFinal);
}

void StateDiagramWindow::hit_terminate() {
  hit_button(TerminatePS, addTerminate);
}

void StateDiagramWindow::hit_deepHistory() {
  hit_button(DeepHistoryPS, addDeepHistory);
}

void StateDiagramWindow::hit_shallowHistory() {
  hit_button(ShallowHistoryPS, addShallowHistory);
}

void StateDiagramWindow::hit_junction() {
  hit_button(JunctionPS, addJunction);
}

void StateDiagramWindow::hit_choice() {
  hit_button(ChoicePS, addChoice);
}

void StateDiagramWindow::hit_fork() {
  hit_button(ForkPS, addFork);
}

void StateDiagramWindow::hit_join() {
  hit_button(JoinPS, addJoin);
}

void StateDiagramWindow::hit_action() {
  hit_button(UmlStateAction, addAction);
}

void StateDiagramWindow::hit_signalin() {
  hit_button(UmlSignalIn, addSignalIn);
}

void StateDiagramWindow::hit_signalout() {
  hit_button(UmlSignalOut, addSignalOut);
}

void StateDiagramWindow::hit_transition() {
  hit_button(UmlTransition, addTransition);
}

/*
void StateDiagramWindow::hit_dependency() {
  hit_button(UmlDependOn, dependency);
}
*/

void StateDiagramWindow::hit_note() {
  hit_button(UmlNote, note);
}

void StateDiagramWindow::hit_anchor() {
  hit_button(UmlAnchor, anchor);
}

void StateDiagramWindow::hit_text() {
  hit_button(UmlText, text);
}

