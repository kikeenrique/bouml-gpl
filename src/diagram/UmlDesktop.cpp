// *************************************************************************
//
// Copyright (C) 2004-2006 Bruno PAGES  All rights reserved.
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

#include <stdlib.h>
#include <stdio.h>

#include <qapplication.h>
#include <qmessagebox.h>
#include <qcursor.h>

#include "UmlDesktop.h"

int UmlDesktop::w;
int UmlDesktop::h;
QPoint UmlDesktop::c;
bool UmlDesktop::fixedp;

int UmlDesktop::width()
{
  return w;
}

int UmlDesktop::height()
{
  return h;
}

void UmlDesktop::tocenter(QWidget * who)
{
  who->move(who->x() + c.x() - (who->x() + who->width() / 2),
	    who->y() + c.y() - (who->y() + who->height() / 2));
}

bool UmlDesktop::fixed()
{
  return fixedp;
}

void UmlDesktop::setsize_center(QWidget * who, QSize & previous,
				double pw, double ph)
{
  if (previous.width() <= 0) {
    previous.setWidth((int) (w * pw));
    previous.setHeight((int) (h * ph));
  }
  
  who->resize(previous);
  
  if (fixedp)
    tocenter(who);
}

void UmlDesktop::limitsize_center(QWidget * who, QSize & previous, 
				  double max_w, double max_h)
{
  if (previous.width() > 0)
    who->resize(previous);
  else {
    int wi = (int) (w * max_w);
    int he = (int) (h * max_h);
    
    // warning who->width() or who->height()
    // may be greater than the reality
    if ((who->width() > wi) || (who->height() > he)) {
      previous.setWidth((who->width() > wi) ? wi : who->width());
      previous.setHeight((who->height() > he) ? he : who->height());
      who->resize(previous);
    }
  }
  
  if (fixedp)
    tocenter(who);
}

void UmlDesktop::limitsize_move(QWidget * who, QSize & previous,
				double max_w, double max_h)
{
  if (previous.width() > 0)
    who->resize(previous);
  else {
    int wi = (int) (w * max_w);
    int he = (int) (h * max_h);
    
    // warning who->width() or who->height()
    // may be greater than the reality
    if ((who->width() > wi) || (who->height() > he)) {
      previous.setWidth((who->width() > wi) ? wi : who->width());
      previous.setHeight((who->height() > he) ? he : who->height());
      who->resize(previous);
    }
  }
  
  if (fixedp)
    tocenter(who);
  else
    who->move(QCursor::pos());
}

void UmlDesktop::init()
{
  QString limits = getenv("BOUML_LIMIT_DESKTOP");
  
  if (!limits.isEmpty()) {
    int top, left, bottom, right;
    
    if (sscanf((const char *) limits, "%d,%d,%d,%d",
	       &left, &top, &right, &bottom)
	!= 4)
      QMessageBox::warning(0, "Bouml",
			   "BOUML_LIMIT_DESKTOP wrong defined\n\n"
			   "must be '{left},{top},{right},{bottom}'");
    else {
      w = right - left + 1;
      h = bottom - top + 1;
      c.setX((right + left) /2);
      c.setY((top+bottom) / 2);
      fixedp = TRUE;
      return;
    }
  }
  
  // default case or error
  
  fixedp = FALSE;
  w = QApplication::desktop()->width();
  h = QApplication::desktop()->height();
  c.setX(w / 2);
  c.setY(h / 2);
}
