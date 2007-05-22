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
#include "ActivityDiagramWindow.h"
#include "ActivityDiagramView.h"
#include "BrowserActivityDiagram.h"
#include "UmlPixmap.h"
#include "myio.h"

const char * addactivityText = "Click this button to add an <em>activity</em> in the diagram. <br><br>"
"You can also drop the activity from the <b>browser</b>.";
const char * addinterruptibleactivityregionText = "Click this button to add an <em>interruptible activity region</em> in the diagram. <br><br>"
"You can also drop the interruptible activity region from the <b>browser</b>.";
const char * addexpansionregionText = "Click this button to add an <em>expansion region</em> in the diagram. <br><br>"
"You can also drop the expansion region from the <b>browser</b>.";
extern const char * addpackageText;
extern const char * addfragmentText;
extern const char * noteText;
extern const char * anchorText;
extern const char * textText;
extern const char * dependencyText;
static const char * addinitialText = "Click this button to add an <em>initial node</em> in the diagram. <br><br>"
"You can also drop the <em>initial node</em> from the <b>browser</b>.";
static const char * addactivityfinalText = "Click this button to add an <em>activity final node</em> in the diagram. <br><br>"
"You can also drop the <em>activity final node</em> from the <b>browser</b>.";
static const char * addflowfinalText = "Click this button to add a <em>flow final node</em> in the diagram. <br><br>"
"You can also drop the <em>flow final node</em> from the <b>browser</b>.";
static const char * adddecisionText = "Click this button to add a <em>decision node</em> in the diagram. <br><br>"
"You can also drop the <em>decision node</em> from the <b>browser</b>.";
static const char * addmergeText = "Click this button to add a <em>merge node</em> in the diagram. <br><br>"
"You can also drop the <em>merge node</em> from the <b>browser</b>.";
extern const char * addforkText;
extern const char * addjoinText;
static const char * addactionText = "Click this button to add an <em>action</em> in the diagram.";
static const char * addobjectText = "Click this button to add an <em>object node</em> in the diagram.";
const char * addflowText = "Click this button to add a <em>flow</em> in the diagram. <br><br>"
"You can also drop the <em>flow</em> from the <b>browser</b>.";

// id is an old ident in case of an import
ActivityDiagramWindow::ActivityDiagramWindow(const QString & s, BrowserActivityDiagram * b, int id)
    : DiagramWindow(b, s), view(0) {
  QToolBar * toolbar = new QToolBar(this, "activity operations");
  addToolBar(toolbar, "Activity Operations", Top, TRUE);
  
  add_edit_button(toolbar);
  
  select =
    new QToolButton(*selectButton, "Select", QString::null,
		    this, SLOT(hit_select()), toolbar, "select");
  select->setToggleButton(TRUE);
  select->setOn(TRUE);
  current_button = UmlSelect;
  
  addPackage
    = new QToolButton(*packageButton, "New Package", QString::null,
		      this, SLOT(hit_package()), toolbar, "add package");
  addPackage->setToggleButton(TRUE);
  QWhatsThis::add(addPackage, addpackageText);
  
  addFragment
    = new QToolButton(*fragmentButton, "New Fragment", QString::null,
		      this, SLOT(hit_fragment()), toolbar, "add fragment");
  addFragment->setToggleButton(TRUE);
  QWhatsThis::add(addFragment, addfragmentText);
  
  addActivity =
    new QToolButton(*activityButton, "New Activity", QString::null,
		    this, SLOT(hit_activity()), toolbar, "add activity");
  addActivity->setToggleButton(TRUE);
  QWhatsThis::add(addActivity, addactivityText);
  
  addInterruptibleActivityRegion =
    new QToolButton(*interruptibleactivityregionButton,
		    "New Interruptible Activity Region", QString::null,
		    this, SLOT(hit_interruptibleactivityregion()),
		    toolbar, "add interruptible activity region");
  addInterruptibleActivityRegion->setToggleButton(TRUE);
  QWhatsThis::add(addInterruptibleActivityRegion,
		  addinterruptibleactivityregionText);
  
  addExpansionRegion =
    new QToolButton(*expansionregionButton,
		    "New Expansion Region", QString::null,
		    this, SLOT(hit_expansionregion()),
		    toolbar, "add expansion region");
  addExpansionRegion->setToggleButton(TRUE);
  QWhatsThis::add(addExpansionRegion,
		  addexpansionregionText);
    
  addAction =
    new QToolButton(*activityactionButton, "New Action", QString::null,
		    this, SLOT(hit_action()), toolbar, "add action");
  addAction->setToggleButton(TRUE);
  QWhatsThis::add(addAction, addactionText);

  addObject =
    new QToolButton(*actionButton, "New Object Node", QString::null,
		    this, SLOT(hit_object()), toolbar, "add object node");
  addObject->setToggleButton(TRUE);
  QWhatsThis::add(addObject, addobjectText);

  addInitial =
    new QToolButton(*initialButton, "New Initial node", QString::null,
		    this, SLOT(hit_initial()), toolbar, "add initial");
  addInitial->setToggleButton(TRUE);
  QWhatsThis::add(addInitial, addinitialText);

  addActivityFinal =
    new QToolButton(*finalButton, "New Activity Final", QString::null,
		    this, SLOT(hit_activityfinal()), toolbar, "add activity final");
  addActivityFinal->setToggleButton(TRUE);
  QWhatsThis::add(addActivityFinal, addactivityfinalText);

  addFlowFinal =
    new QToolButton(*exitpointButton, "New Flow Final", QString::null,
		    this, SLOT(hit_flowfinal()), toolbar, "add flow final");
  addFlowFinal->setToggleButton(TRUE);
  QWhatsThis::add(addFlowFinal, addflowfinalText);

  addMerge =
    new QToolButton(*mergeButton, "New Merge", QString::null,
		    this, SLOT(hit_merge()), toolbar, "add merge");
  addMerge->setToggleButton(TRUE);
  QWhatsThis::add(addMerge, addmergeText);

  addDecision =
    new QToolButton(*decisionButton, "New Decision", QString::null,
		    this, SLOT(hit_decision()), toolbar, "add decision");
  addDecision->setToggleButton(TRUE);
  QWhatsThis::add(addDecision, adddecisionText);

  addFork =
    new QToolButton(*forkButton, "New Fork", QString::null,
		    this, SLOT(hit_fork()), toolbar, "add fork");
  addFork->setToggleButton(TRUE);
  QWhatsThis::add(addFork, addforkText);

  addJoin =
    new QToolButton(*joinButton, "New Join", QString::null,
		    this, SLOT(hit_join()), toolbar, "add join");
  addJoin->setToggleButton(TRUE);
  QWhatsThis::add(addJoin, addjoinText);

  addFlow =
    new QToolButton(*directionalAssociationButton, "New Flow", QString::null,
		    this, SLOT(hit_flow()), toolbar, "add flow");
  addFlow->setToggleButton(TRUE);
  QWhatsThis::add(addFlow, addflowText);
  
  dependency =
    new QToolButton(*dependencyButton, "Dependency", QString::null,
		    this, SLOT(hit_dependency()), toolbar, "dependency");
  dependency->setToggleButton(TRUE);
  QWhatsThis::add(dependency, dependencyText);
  
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
  
  view = new ActivityDiagramView(this, canvas, (id != -1) ? id : b->get_ident());
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

ActivityDiagramWindow::~ActivityDiagramWindow() {
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

DiagramView * ActivityDiagramWindow::get_view() const {
  return view;
}

void ActivityDiagramWindow::hit_button(UmlCode c, QToolButton * b) {
  view->abort_line_construction();
  
  select->setOn(FALSE);
  addPackage->setOn(FALSE);
  addFragment->setOn(FALSE);
  addActivity->setOn(FALSE);
  addInterruptibleActivityRegion->setOn(FALSE);
  addExpansionRegion->setOn(FALSE);
  addInitial->setOn(FALSE);
  addActivityFinal->setOn(FALSE);
  addFlowFinal->setOn(FALSE);
  addDecision->setOn(FALSE);
  addMerge->setOn(FALSE);
  addFork->setOn(FALSE);
  addJoin->setOn(FALSE);
  addAction->setOn(FALSE);
  addObject->setOn(FALSE);
  addFlow->setOn(FALSE);
  dependency->setOn(FALSE);
  note->setOn(FALSE);
  anchor->setOn(FALSE);
  text->setOn(FALSE);
  
  b->setOn(TRUE);
  current_button = c;
}

void ActivityDiagramWindow::hit_package() {
  hit_button(UmlPackage, addPackage);
}

void ActivityDiagramWindow::hit_fragment() {
  hit_button(UmlFragment, addFragment);
}

void ActivityDiagramWindow::hit_activity() {
  hit_button(UmlActivity, addActivity);
}

void ActivityDiagramWindow::hit_interruptibleactivityregion() {
  hit_button(UmlInterruptibleActivityRegion,
	     addInterruptibleActivityRegion);
}

void ActivityDiagramWindow::hit_expansionregion() {
  hit_button(UmlExpansionRegion,
	     addExpansionRegion);
}

void ActivityDiagramWindow::hit_initial() {
  hit_button(InitialAN, addInitial);
}

void ActivityDiagramWindow::hit_activityfinal() {
  hit_button(ActivityFinalAN, addActivityFinal);
}

void ActivityDiagramWindow::hit_flowfinal() {
  hit_button(FlowFinalAN, addFlowFinal);
}

void ActivityDiagramWindow::hit_decision() {
  hit_button(DecisionAN, addDecision);
}

void ActivityDiagramWindow::hit_merge() {
  hit_button(MergeAN, addMerge);
}

void ActivityDiagramWindow::hit_fork() {
  hit_button(ForkAN, addFork);
}

void ActivityDiagramWindow::hit_join() {
  hit_button(JoinAN, addJoin);
}

void ActivityDiagramWindow::hit_action() {
  hit_button(UmlActivityAction, addAction);
}

void ActivityDiagramWindow::hit_object() {
  hit_button(UmlActivityObject, addObject);
}

void ActivityDiagramWindow::hit_flow() {
  hit_button(UmlFlow, addFlow);
}

void ActivityDiagramWindow::hit_dependency() {
  hit_button(UmlDependOn, dependency);
}

void ActivityDiagramWindow::hit_note() {
  hit_button(UmlNote, note);
}

void ActivityDiagramWindow::hit_anchor() {
  hit_button(UmlAnchor, anchor);
}

void ActivityDiagramWindow::hit_text() {
  hit_button(UmlText, text);
}

