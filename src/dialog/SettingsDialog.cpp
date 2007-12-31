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

#include <qhbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h>

#include "Settings.h"
#include "SettingsDialog.h"
#include "UmlPixmap.h"
#include "UmlDesktop.h"

class ComboStates : public QComboBox {
  public:
    ComboStates(QWidget * parent, Uml3States v, bool nodefault, bool unchanged);
    ComboStates(QWidget * parent, ClassDrawingMode v, bool nodefault, bool unchanged);
    ComboStates(QWidget * parent, DrawingLanguage v, bool nodefault, bool unchanged);
    ComboStates(QWidget * parent, ShowContextMode v, bool nodefault, bool unchanged);
};

ComboStates::ComboStates(QWidget * parent, Uml3States v,
			 bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  insertItem(stringify((Uml3States) 0));
  insertItem(stringify((Uml3States) 1));
  // the last value MUST be default
  if (!nodefault)
    insertItem(stringify((Uml3States) 2));
  if (unchanged) {
    insertItem("<unchanged>");
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);
}

ComboStates::ComboStates(QWidget * parent, ClassDrawingMode v,
			 bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  int i;
  
  for (i = 0; i != (int) DefaultClassDrawingMode; i += 1)
    insertItem(stringify((ClassDrawingMode) i));
  // the last value MUST be default
  if (!nodefault)
    insertItem(stringify((ClassDrawingMode) i));
  if (unchanged) {
    insertItem("<unchanged>");
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);
}

ComboStates::ComboStates(QWidget * parent, DrawingLanguage v,
			 bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  int i;
  
  for (i = 0; i != (int) DefaultDrawingLanguage; i += 1)
    insertItem(stringify((DrawingLanguage) i));
  // the last value MUST be default
  if (!nodefault)
    insertItem(stringify((DrawingLanguage) i));
  if (unchanged) {
    insertItem("<unchanged>");
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);
}

ComboStates::ComboStates(QWidget * parent, ShowContextMode v,
			 bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  int i;
  
  for (i = 0; i != (int) DefaultShowContextMode; i += 1)
    insertItem(stringify((ShowContextMode) i));
  // the last value MUST be default
  if (!nodefault)
    insertItem(stringify((ShowContextMode) i));
  if (unchanged) {
    insertItem("<unchanged>");
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);
}

class ComboColor : public QComboBox {
  public:
    ComboColor(QWidget * parent, UmlColor v,
	       bool nodefault, bool unchanged);
};

ComboColor::ComboColor(QWidget * parent, UmlColor v,
		       bool nodefault, bool unchanged) 
    : QComboBox(FALSE, parent) {
  // the last value MUST be default
  for (int i = 0; i != (int) UmlDefaultColor; i += 1) {
    // use switch to not be dependant on the colors items order and number
    switch (i) {
    case UmlTransparent:
      insertItem("Transparent");
      break;
    case UmlWhite:
      insertItem(* WhitePixmap);
      break;
    case UmlLightYellow:
      insertItem(* LightYellowPixmap);
      break;
    case UmlYellow:
      insertItem(* YellowPixmap);
      break;
    case UmlMediumYellow:
      insertItem(* MediumYellowPixmap);
      break;
    case UmlDarkYellow:
      insertItem(* DarkYellowPixmap);
      break;
    case UmlLightBlue:
      insertItem(* LightBluePixmap);
      break;
    case UmlBlue:
      insertItem(* BluePixmap);
      break;
    case UmlLightMediumBlue:
      insertItem(* LightMediumBluePixmap);
      break;
    case UmlMediumBlue:
      insertItem(* MediumBluePixmap);
      break;
    case UmlDarkBlue:
      insertItem(* DarkBluePixmap);
      break;
    case UmlLightGreen:
      insertItem(* LightGreenPixmap);
      break;
    case UmlGreen:
      insertItem(* GreenPixmap);
      break;
    case UmlLightMediumGreen:
      insertItem(* LightMediumGreenPixmap);
      break;
    case UmlMediumGreen:
      insertItem(* MediumGreenPixmap);
      break;
    case UmlDarkGreen:
      insertItem(* DarkGreenPixmap);
      break;
    case UmlLightRed:
      insertItem(* LightRedPixmap);
      break;
    case UmlMidRed:
      insertItem(* MidRedPixmap);
      break;
    case UmlRed:
      insertItem(* RedPixmap);
      break;
    case UmlVeryLightOrange:
      insertItem(* VeryLightOrangePixmap);
      break;
    case UmlLightOrange:
      insertItem(* LightOrangePixmap);
      break;
    case UmlOrange:
      insertItem(* OrangePixmap);
      break;
    case UmlDarkOrange:
      insertItem(* DarkOrangePixmap);
      break;
    case UmlLightMagenta:
      insertItem(* LightMagentaPixmap);
      break;
    case UmlMagenta:
      insertItem(* MagentaPixmap);
      break;
    case UmlMidMagenta:
      insertItem(* MidMagentaPixmap);
      break;
    case UmlDarkMagenta:
      insertItem(* DarkMagentaPixmap);
      break;
    case UmlVeryLightGray:
      insertItem(* VeryLightGrayPixmap);
      break;
    case UmlLightGray:
      insertItem(* LightGrayPixmap);
      break;
    case UmlGray:
      insertItem(* GrayPixmap);
      break;
    case UmlDarkGray:
      insertItem(* DarkGrayPixmap);
      break;
    case UmlBlack:
      insertItem(* BlackPixmap);
      break;
    default:
      insertItem("Unknown color");
    }
  }
  if (!nodefault)
    insertItem(stringify(UmlDefaultColor));
  if (unchanged) {
    insertItem("<unchanged>");
    setCurrentItem(count() - 1); 
  }
  else
    setCurrentItem(v);    
  
#ifdef WIN32
  setSizeLimit(34);	// yes !, don't set it to count() !
#else
  setSizeLimit(25);	// yes !, don't set it to count() !
#endif
}

QSize SettingsDialog::previous_size;

SettingsDialog::SettingsDialog(QArray<StateSpec> * st, QArray<ColorSpec> * co,
			       bool nodefault, bool own, bool unchanged,
			       const char * title)
    : QTabDialog(0, title, TRUE),
      states(st), colors(co), several(unchanged) {
  setCaption(title);
  
  QGrid * grid = 0;
  QString tabname;
  unsigned i;
  unsigned n;
  
  if (states != 0) {
    n = states->count();
    cbstates = new QVector<ComboStates>(n);
    
    for (i = 0; i != n; i += 1) {
      StateSpec & st = states->at(i);
      QString s = st.name;
      int index = s.find('#');
      QString tbn;
      
      if (index != -1) {
	tbn = s.left(index);
	s = s.mid(index + 1);
      }
      else
	tbn = "diagram";
      
      if ((grid == 0) || (tabname != tbn)) {
	if (grid != 0)
	  addTab(grid, tabname);
	grid = new QGrid(5, this);
	grid->setMargin(10);
	grid->setSpacing(10);
	tabname = tbn;
      }
      
      new QLabel("", grid);
      new QLabel(s + " : ", grid);
      QHBox * hb = new QHBox(grid);
            
      switch (st.who) {
      case StateSpec::is3states:
	cbstates->insert(i, new ComboStates(hb, *((Uml3States *) st.state),
					    nodefault, unchanged));
	break;
      case StateSpec::isClassDrawingMode:
	cbstates->insert(i, new ComboStates(hb, *((ClassDrawingMode *) st.state),
					    nodefault, unchanged));
	break;
      case StateSpec::isDrawingLanguage:
	cbstates->insert(i, new ComboStates(hb, *((DrawingLanguage *) st.state),
					    nodefault, unchanged));
	break;
      default:
	cbstates->insert(i, new ComboStates(hb, *((ShowContextMode *) st.state),
					    nodefault, unchanged));
      }
      new QLabel("", hb);
      new QLabel("", grid);
      new QLabel("", grid);
    }
    
    addTab(grid, tabname);
  }
  
  if (colors != 0) {
    const char * lbl = "color";
    
    n = colors->count();
    cbcolors = new QVector<ComboColor>(n);
    grid = new QGrid(5, this);
    grid->setMargin(10);
    grid->setSpacing(10);
    
    for (i = 0; i != n; i += 1) {
      if (i == 11) {
	addTab(grid, "color [1]");
	lbl = "color [2]";
	grid = new QGrid(5, this);
	grid->setMargin(10);
	grid->setSpacing(10);
      }
      new QLabel("", grid);
      QString s = (own) ? "" : "default ";
      s += colors->at(i).name;
      s += " : ";
      new QLabel(s, grid);
      cbcolors->insert(i, new ComboColor(grid, *(colors->at(i).color),
					 nodefault, unchanged));
      new QLabel("", grid);
      new QLabel("", grid);
    }
    
    addTab(grid, lbl);
  }
  
  setCancelButton();
}

void SettingsDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

SettingsDialog::~SettingsDialog() {
  if (colors != 0)
    delete cbcolors;
  if (states != 0)
    delete cbstates;
  previous_size = size();
}

void SettingsDialog::accept() {
  unsigned i, n;
  
  if (states != 0) {
    n = states->count();
    
    for (i = 0; i != n; i += 1) {
      if (cbstates->at(i)->currentText() == "<unchanged>")
	states->at(i).name = 0;
      else
	states->at(i).set_state(cbstates->at(i)->currentItem());
    }
  }
  
  if (colors != 0) {
    n = colors->count();
    
    for (i = 0; i != n; i += 1) {
      if (cbcolors->at(i)->currentText() == "<unchanged>")
	colors->at(i).name = 0;
      else
	*(colors->at(i).color) = (UmlColor) cbcolors->at(i)->currentItem();
    }
  }

  QDialog::accept();
}
