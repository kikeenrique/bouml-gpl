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
#include <qspinbox.h>

#include "UmlWindow.h"
#include "SeqDiagramWindow.h"
#include "SeqDiagramView.h"
#include "BrowserSeqDiagram.h"
#include "UmlPixmap.h"
#include "myio.h"

const char * addclassinstanceText = "Click this button to add a <em>class instance</em> in the diagram. <br><br>"
"You can also drop the class from the <b>browser</b>.";
extern const char * addfragmentText;
extern const char * noteText;
extern const char * anchorText;
const char * syncmsgText = "Click this button to send a <em>synchronous message</em>";
const char * asyncmsgText = "Click this button to send an <em>asynchronous message</em>";
const char * syncselfmsgText = "Click this button to send a <em>reflexive synchronous message</em>";
const char * asyncselfmsgText = "Click this button to send a <em>reflexive asynchronous message</em>";
const char * returnmsgText = "Click this button to add an <em>explicitly return</em>";
const char * selfreturnmsgText = "Click this button to add an <em>explicit reflexive return</em>";
const char * textText = "Click this button to add a <em>text</em>";
const char * addcontinuationText = "Click this button to add a <em>continuation</em>";

// id is an old ident in case of an import
SeqDiagramWindow::SeqDiagramWindow(const QString & s, BrowserSeqDiagram * b, int id)
    : DiagramWindow(b, s), view(0) {
  QToolBar * toolbar = new QToolBar(this, "sequence diagram operations");
  addToolBar(toolbar, "Sequence Diagram Operations", Top, TRUE);
  
  add_edit_button(toolbar);
  
  select =
    new QToolButton(*selectButton, "Select", QString::null,
		    this, SLOT(hit_select()), toolbar, "select");
  select->setToggleButton(TRUE);
  select->setOn(TRUE);
  current_button = UmlSelect;
  
  addFragment
    = new QToolButton(*fragmentButton, "Add Fragment", QString::null,
		      this, SLOT(hit_fragment()), toolbar, "add fragment");
  addFragment->setToggleButton(TRUE);
  QWhatsThis::add(addFragment, addfragmentText);
  
  addClassInstance
    = new QToolButton(*classinstanceButton, "Add Class instance", QString::null,
		      this, SLOT(hit_classinstance()), toolbar, "add class instance");
  addClassInstance->setToggleButton(TRUE);
  QWhatsThis::add(addClassInstance, addclassinstanceText);
  
  addClass
    = new QToolButton(*classButton, "Add Class instance", QString::null,
		      this, SLOT(hit_class()), toolbar, "add class");
  addClass->setToggleButton(TRUE);
  QWhatsThis::add(addClass, addclassinstanceText);
  
  addContinuation
    = new QToolButton(*continuationButton, "Add Continuation", QString::null,
		      this, SLOT(hit_continuation()), toolbar, "add continuation");
  addContinuation->setToggleButton(TRUE);
  QWhatsThis::add(addContinuation, addcontinuationText);
  
  syncMsg =
    new QToolButton(*syncmsgButton, "Synchronous message", QString::null,
		    this, SLOT(hit_syncmsg()), toolbar, "synchronous message");
  syncMsg->setToggleButton(TRUE);
  QWhatsThis::add(syncMsg, syncmsgText);
  
  asyncMsg =
    new QToolButton(*directionalAssociationButton, "Asynchronous message", QString::null,
		    this, SLOT(hit_asyncmsg()), toolbar, "asynchronous message");
  asyncMsg->setToggleButton(TRUE);
  QWhatsThis::add(asyncMsg, asyncmsgText);
  
  syncSelfMsg =
    new QToolButton(*syncselfmsgButton, "Synchronous Reflexive message", QString::null,
		    this, SLOT(hit_syncselfmsg()), toolbar, "synchronous reflexive message");
  syncSelfMsg->setToggleButton(TRUE);
  QWhatsThis::add(syncSelfMsg, syncselfmsgText);
  
  asyncSelfMsg =
    new QToolButton(*asyncselfmsgButton, "Asynchronous Reflexive message", QString::null,
		    this, SLOT(hit_asyncselfmsg()), toolbar, "asynchronous reflexive message");
  asyncSelfMsg->setToggleButton(TRUE);
  QWhatsThis::add(asyncSelfMsg, asyncselfmsgText);
  
  returnMsg =
    new QToolButton(*returnmsgButton, "Explicit return", QString::null,
		    this, SLOT(hit_returnmsg()), toolbar, "explicit return");
  returnMsg->setToggleButton(TRUE);
  QWhatsThis::add(returnMsg, returnmsgText);
  
  selfreturnMsg =
    new QToolButton(*selfreturnmsgButton, "Explicit reflexive return", QString::null,
		    this, SLOT(hit_selfreturnmsg()), toolbar, "explicit reflexive return");
  selfreturnMsg->setToggleButton(TRUE);
  QWhatsThis::add( selfreturnMsg,  selfreturnmsgText);
  
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
  
  view = new SeqDiagramView(this, canvas, (id != -1) ? id : b->get_ident());
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

SeqDiagramWindow::~SeqDiagramWindow() {
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

DiagramView * SeqDiagramWindow::get_view() const {
  return view;
}

void SeqDiagramWindow::hit_button(UmlCode c, QToolButton * b) {
  view->abort_line_construction();
  
  select->setOn(FALSE);
  addFragment->setOn(FALSE);
  addClass->setOn(FALSE);
  addClassInstance->setOn(FALSE);
  addContinuation->setOn(FALSE);
  syncMsg->setOn(FALSE);
  asyncMsg->setOn(FALSE);
  syncSelfMsg->setOn(FALSE);
  asyncSelfMsg->setOn(FALSE);
  selfreturnMsg->setOn(FALSE);
  returnMsg->setOn(FALSE);
  note->setOn(FALSE);
  anchor->setOn(FALSE);
  text->setOn(FALSE);
  
  b->setOn(TRUE);
  current_button = c;
}

void SeqDiagramWindow::hit_fragment() {
  hit_button(UmlFragment, addFragment);
}

void SeqDiagramWindow::hit_classinstance() {
  hit_button(UmlClassInstance, addClassInstance);
}

void SeqDiagramWindow::hit_class() {
  hit_button(UmlClass, addClass);
}

void SeqDiagramWindow::hit_continuation() {
  hit_button(UmlContinuation, addContinuation);
}

void SeqDiagramWindow::hit_syncmsg() {
  hit_button(UmlSyncMsg, syncMsg);
}

void SeqDiagramWindow::hit_asyncmsg() {
  hit_button(UmlAsyncMsg, asyncMsg);
}

void SeqDiagramWindow::hit_syncselfmsg() {
  hit_button(UmlSyncSelfMsg, syncSelfMsg);
}

void SeqDiagramWindow::hit_asyncselfmsg() {
  hit_button(UmlAsyncSelfMsg, asyncSelfMsg);
}

void SeqDiagramWindow::hit_returnmsg() {
  hit_button(UmlReturnMsg, returnMsg);
}

void SeqDiagramWindow::hit_selfreturnmsg() {
  hit_button(UmlSelfReturnMsg, selfreturnMsg);
}

void SeqDiagramWindow::hit_note() {
  hit_button(UmlNote, note);
}

void SeqDiagramWindow::hit_anchor() {
  hit_button(UmlAnchor, anchor);
}

void SeqDiagramWindow::hit_text() {
  hit_button(UmlText, text);
}

