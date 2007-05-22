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

#include "BrowserView.h"
#include "BrowserNode.h"

BrowserView * BrowserView::the;
QDir BrowserView::dir;

BrowserView::BrowserView(QWidget * parent) : QListView(parent) {
  the = this;
  
  setSorting(-1);		// manual sorting
  addColumn("browser          ");
  addColumn("Owner id");
  addColumn("Owner name");
  addColumn("Revision");
  addColumn("Modified by");
  setColumnWidthMode(0, Maximum);
  setColumnWidthMode(1, Maximum);
  setColumnWidthMode(2, Maximum);
  setColumnWidthMode(3, Maximum);
  setColumnWidthMode(4, Maximum);
  setColumnAlignment(1, QObject::AlignHCenter);
  setColumnAlignment(2, QObject::AlignHCenter);
  setColumnAlignment(3, QObject::AlignHCenter);
  setColumnAlignment(4, QObject::AlignHCenter);
  setTreeStepSize(18);
  
  connect(this, SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)),
	  this, SLOT(rightPressed(QListViewItem *)));
}

BrowserView::~BrowserView() {
}

void BrowserView::close() {
  if (firstChild() != 0) {
    dir.rmdir("all.lock");
    clear();
  }
}

BrowserNode * BrowserView::get_project()
{
  return (BrowserNode *) the->firstChild();
}

void BrowserView::set_project(QDir di) {
  dir = di;
  new BrowserNode(the, dir.dirName() + ".prj");
  setRootIsDecorated(TRUE/*FALSE*/);
}

void BrowserView::rightPressed(QListViewItem * item) {
  if (item != 0)
    ((BrowserNode *) item)->menu();
}

void BrowserView::keyPressEvent(QKeyEvent * e) {
  switch (e->state()) {
  case QObject::ControlButton:
  case QObject::AltButton:
    switch (e->key()) {
    case QObject::Key_A:
    case QObject::Key_P:
    case QObject::Key_U:
      {
	QListViewItem * t = selectedItem();
	
	if (t!= 0) {
	  ((BrowserNode *) t)->key_event(e);
	  return;
	}
      }
    }
    break;
  default:
    break;
  }
  
  QListView::keyPressEvent(e);
}

void BrowserView::select(QListViewItem * b)
{
  the->ensureItemVisible(b);
  the->setSelected(b, TRUE);
}
