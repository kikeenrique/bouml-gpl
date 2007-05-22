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

#include <qbuttongroup.h>
#include <qradiobutton.h> 

#include "VisibilityGroup.h"

QButtonGroup * VisibilityGroup::init(QWidget * parent, UmlVisibility v, 
				     bool pack_allowed,
				     const char * title,
				     const char * default_prefix) {
   QButtonGroup * bg =
     new QButtonGroup((default_prefix != 0) ? 5 : 4,
		      QGroupBox::Horizontal, title, parent);
   bg->setExclusive(TRUE);
   if (default_prefix != 0) {
     default_pfix = default_prefix;
     default_visibility_rb =
       new QRadioButton(default_pfix + " (protected)", bg);
   }
   else
     default_visibility_rb = 0;
   
   public_rb = new QRadioButton("public", bg);
   protected_rb = new QRadioButton("protected", bg);
   private_rb = new QRadioButton("private", bg);
   package_rb = (pack_allowed) ? new QRadioButton("package", bg) : 0;
  
   switch (v) {
    case UmlPublic:
      public_rb->setChecked(TRUE);
      break;
    case UmlProtected:
      protected_rb->setChecked(TRUE);
      break;
    case UmlPrivate:
      private_rb->setChecked(TRUE);
      break;
    case UmlPackageVisibility:
      ((pack_allowed) ? package_rb : public_rb)->setChecked(TRUE);
      break;
    default:
      if (default_visibility_rb != 0)
	default_visibility_rb->setChecked(TRUE);
   }
   
   return bg;
}

void VisibilityGroup::update_default(const VisibilityGroup & default_grp) {
  if (default_visibility_rb != 0) {
    if (default_grp.public_rb->isChecked())
      default_visibility_rb->setText(default_pfix + " (public)");
    else if (default_grp.protected_rb->isChecked())
      default_visibility_rb->setText(default_pfix + " (protected)");
    else if (default_grp.private_rb->isChecked())
      default_visibility_rb->setText(default_pfix + " (private)");
    else
      default_visibility_rb->setText(default_pfix + " (package)");
  }
}

UmlVisibility VisibilityGroup::value() {
  if ((default_visibility_rb != 0) && default_visibility_rb->isChecked())
    return UmlDefaultVisibility;
  else if (public_rb->isChecked())
    return UmlPublic;
  else if (protected_rb->isChecked())
    return UmlProtected;
  else if (private_rb->isChecked())
    return UmlPrivate;
  else
    return UmlPackageVisibility;
}

QString VisibilityGroup::state() {
  if (public_rb->isChecked())
    return "public";
  else if (protected_rb->isChecked())
    return "protected";
  else if (private_rb->isChecked())
    return "private";
  else
    return "package";
}
