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

#include <qcursor.h>
#include <qpainter.h>
#include <qpopupmenu.h> 

#include "InfoCanvas.h"
#include "DiagramCanvas.h"
#include "UmlGlobal.h"
#include "UmlCanvas.h"
#include "BrowserDiagram.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "DialogUtil.h"

InfoCanvas::InfoCanvas(UmlCanvas * canvas, DiagramItem * a,
		       int x, int y, int id)
    : NoteCanvas(canvas, x, y, id), who(a) {
  // for read
}

InfoCanvas::InfoCanvas(UmlCanvas * canvas, DiagramItem * a, QString s)
    : NoteCanvas(canvas, 0, 0, 0), who(a) {
  note = s;
  
  QFontMetrics fm(the_canvas()->get_font(itsfont));
  QSize sz = fm.size(0, s);
  const int margin = (int) (2 * NOTE_MARGIN * the_canvas()->zoom());
  int w = sz.width() + margin;
  int h = sz.height() + margin;

  if (w < width())
    w = width();
  if (h < height())
    h = height();

  setSize(w, h);
  setZ(a->get_z());

  width_scale100 = w;
  height_scale100 = h;
  set_center100();
}

InfoCanvas::~InfoCanvas() {
}

UmlCode InfoCanvas::type() const {
  return UmlInfo;
}

void InfoCanvas::delete_available(bool & in_model, bool & out_model) const {
  if (who->isSelected())
    who->delete_available(in_model, out_model);
}

bool InfoCanvas::copyable() const {
  return who->isSelected();
}

void InfoCanvas::open() {
  who->open();
}

void InfoCanvas::set(QString s) {
  s = toUnicode(s);

  if (s != note) {
    note = s;
    modified();
  }
}

void InfoCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu fontsubm(0);
  
  m.insertItem(new MenuTitle("Information", m.font()), -1);
  m.insertSeparator();
  m.insertItem("Upper", 0);
  m.insertItem("Lower", 1);
  m.insertSeparator();
  m.insertItem("Edit", 2);
  m.insertSeparator();
  m.insertItem("Font", &fontsubm);  
  init_font_menu(fontsubm, the_canvas(), 10);
  m.insertItem("Edit drawing settings", 3);
  if (linked()) {
    m.insertSeparator();
    m.insertItem("Select linked items", 4);
  }
  m.insertSeparator();

  int index = m.exec(QCursor::pos());
  
  switch (index) {
  case 0:
    upper();
    modified();	// call package_modified()
    return;
  case 1:
    lower();
    modified();	// call package_modified()
    return;
  case 2:
    who->open();
    return;
  case 3:
    {
      QArray<ColorSpec> co(1);
      
      co[0].set("note color", &itscolor);

      SettingsDialog dialog(0, &co, FALSE, TRUE);
      
      dialog.raise();
      if (dialog.exec() == QDialog::Accepted)
	modified();
      return;
    }
    break;
  case 4:
    the_canvas()->unselect_all();
    select_associated();
    return;
  default:
    if (index >= 10) {
      itsfont = (UmlFont) (index - 10);
      modified();
    }
    return;
  }
  
  package_modified();
}

bool InfoCanvas::has_drawing_settings() const {
  return TRUE;
}

void InfoCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  QArray<ColorSpec> co(1);
  UmlColor itscolor;
  
  co[0].set("note color", &itscolor);
  
  SettingsDialog dialog(0, &co, FALSE, TRUE, TRUE);
  
  dialog.raise();
  if ((dialog.exec() == QDialog::Accepted) && (co[0].name != 0)) {
    QListIterator<DiagramItem> it(l);
    
    for (; it.current(); ++it) {
      ((InfoCanvas *) it.current())->itscolor = itscolor;
      ((InfoCanvas *) it.current())->modified();	// call package_modified()
    }
  }
}

void InfoCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref) {
    st << "information_ref " << get_ident();
  }
  else {
    nl_indent(st);
    
    st << "information " << get_ident() << ' ';
    save_internal(st);
  }
}


InfoCanvas * InfoCanvas::read(char * & st, UmlCanvas * canvas, 
			      char * k, DiagramItem * who)
{
  if (!strcmp(k, "information_ref"))
    return (InfoCanvas *) dict_get(read_id(st), "information", canvas);
  else if (!strcmp(k, "information")) {
    int id = read_id(st);
    InfoCanvas * result = new InfoCanvas(canvas, who, 0, 0, id);
    
    result->read_internal(st);
    return result;
  }
  else
    return 0;
}
