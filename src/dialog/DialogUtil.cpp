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
# include <process.h>
#endif

#ifdef WIN32
#pragma warning (disable: 4150)
#endif

#include <stdlib.h>
#include <stdio.h>

#include <qlabel.h>
#include <qmessagebox.h>
#include <qtextstream.h> 
#include <qfontmetrics.h>
#include <qpopupmenu.h>
#include <qtabdialog.h>
#include <qapplication.h>

#include "BrowserClass.h"
#include "DialogUtil.h"
#include "BodyDialog.h"
#include "DialogTimer.h"
#include "BrowserView.h"
#include "MenuFont.h"
#include "UmlCanvas.h"
#include "mu.h"
#include "KeyValueTable.h"
#include "GenerationSettings.h"
#include "strutil.h"

QSize SmallPushButton::sizeHint() const {
  QFontMetrics fm = fontMetrics();
  QSize sz = fm.size(ShowPrefix, text());
  
  return QSize(sz.width() + sz.width()/8 + 4,
	       sz.height() + sz.height()/8 + 4);
}

// setText() redefinition
void MultiLineEdit::setText(const QString & s) {
  QMultiLineEdit::setText(toUnicode(s));
}

// text() redefinition
QString MultiLineEdit::text() const {
  QString t = QMultiLineEdit::text();
  
  return fromUnicode(t);
}

// setText() redefinition
void LineEdit::setText(const QString & s) {
  QLineEdit::setText(toUnicode(s));
}

// text() redefinition
QString LineEdit::text() const {
  QString t = QLineEdit::text();
  
  return fromUnicode(t);
}

//

void init_font_menu(QPopupMenu & fontsubm, UmlCanvas * canvas, int index)
{
  int f;
  
  for (f = 0; f != UmlFontNumber; f += 1) {
    QString s = stringify((UmlFont) f);
    int i = 0;
    
    while ((i = s.find('_', i)) != -1)
      s.replace(i, 1, " ");
  
    fontsubm.insertItem(new MenuFont(s, canvas->get_font((UmlFont) f)),
			index++);
  }
}

void same_width(QWidget * l1, QWidget * l2)
{
  int w1 = l1->sizeHint().width();
  int w2 = l2->sizeHint().width();
  
  if (w1 > w2)
    l2-> setFixedWidth(w1);
  else if (w1 != w2)
    l1-> setFixedWidth(w2);
}

void same_width(QWidget * l1, QWidget * l2, QWidget * l3)
{
  int w1, w2;
  
  w1 = l1->sizeHint().width();
  if ((w2 = l2->sizeHint().width()) > w1)
    w1 = w2;
  if ((w2 = l3->sizeHint().width()) > w1)
    w1 = w2;
  
  l1-> setFixedWidth(w1);
  l2-> setFixedWidth(w1);
  l3-> setFixedWidth(w1);
}

void same_width(QWidget * l1, QWidget * l2, QWidget * l3, QWidget * l4)
{
  int w1, w2;
  
  w1 = l1->sizeHint().width();
  if ((w2 = l2->sizeHint().width()) > w1)
    w1 = w2;
  if ((w2 = l3->sizeHint().width()) > w1)
    w1 = w2;
  if ((w2 = l4->sizeHint().width()) > w1)
    w1 = w2;
  
  l1-> setFixedWidth(w1);
  l2-> setFixedWidth(w1);
  l3-> setFixedWidth(w1);
  l4-> setFixedWidth(w1);
}

void same_width(QWidget * l1, QWidget * l2, QWidget * l3,
		QWidget * l4, QWidget * l5)
{
  int w1, w2;
  
  w1 = l1->sizeHint().width();
  if ((w2 = l2->sizeHint().width()) > w1)
    w1 = w2;
  if ((w2 = l3->sizeHint().width()) > w1)
    w1 = w2;
  if ((w2 = l4->sizeHint().width()) > w1)
    w1 = w2;
  if ((w2 = l5->sizeHint().width()) > w1)
    w1 = w2;
  
  l1-> setFixedWidth(w1);
  l2-> setFixedWidth(w1);
  l3-> setFixedWidth(w1);
  l4-> setFixedWidth(w1);
  l5-> setFixedWidth(w1);
}

void edit(const QString & s, QString name, void * id, EditType k,
	  QTabDialog * d, post_edit pf, QList<BodyDialog> & edits)
{
  QString ed = getenv("BOUML_EDITOR");
  
  if (!ed.isEmpty()) {
    // try to use it
    QString f;
    
    switch (k) {
    case CppEdit:
      f.sprintf("%s_%lx_%d.%s", (const char *) name, (unsigned long) id, user_id(),
		(const char *) GenerationSettings::get_cpp_src_extension());
      break;
    case JavaEdit:
      f.sprintf("%s_%lx_%d.%s", (const char *) name, (unsigned long) id, user_id(),
		(const char *) GenerationSettings::get_java_extension());
      break;
    default: // TxTEdit
      f.sprintf("%s_%lx_%d.txt", (const char *) name, (unsigned long) id, user_id());
    }

#ifdef WIN32
    for (int index = 0; index != (int) name.length(); index += 1)
      if (!f[index].isLetterOrNumber())
	f.replace(index, 1, "_");
#endif
    
    QString path = BrowserView::get_dir().absFilePath(f);
    
    FILE * fp = fopen((const char *) path, "wb");
    
    if (fp != 0) {
      if (!s.isEmpty())
	fputs((const char *) s, fp);
      fclose(fp);
#ifdef WIN32
      QString arg0 = "\"" + ed + "\"";
      QString arg =	// convert separators else PsPad doesn't work !
	"\"" + QDir::convertSeparators(path) + "\"";
      
      if (_spawnlp(_P_DETACH, ed, arg0, (const char *) arg, 0) == -1) {
	QMessageBox::critical(0, "Bouml",
			      "can't start the editor '" + ed +"'\n"
			      "perhaps you have to redefine BOUML_EDITOR\n"
			      "to give the absolute path ?");
	return;
      }
#else
      ed += " \"" + path + "\"&";
      system(ed);
#endif
      if (d->hasOkButton())
	(new DialogTimer(s, path, d, pf))->start(1000);
      return;
    }
    else
      QMessageBox::critical(0, "Error", QString("Cannot open ") + path);
  }
  else if (d->isModal()) {
    BodyDialog * bd = new BodyDialog(s, d, pf, k, name, edits);
    
    bd->exec();	// will be deleted elsewhere
  }
  else
    (new BodyDialog(s, d, pf, k, name, edits))->show();
}

bool check_edits(QList<BodyDialog> & edits)
{
  if (edits.isEmpty())
    return TRUE;
  
  return (QMessageBox::critical(0, "Bouml",
				"Sub dialog(s) still opened\n"
				"If you choose 'Ok' the dialog will be closed\n"
				"without taking into account their content",
				QMessageBox::Ok, QMessageBox::Abort)
	  == QMessageBox::Ok);
}

AType the_type(const QString & t, const QStringList & types,
	       BrowserNodeList & nodes)
{
  AType result;
  int rank = types.findIndex(t);
  
  if (rank != -1) 
    result.type = ((BrowserClass *) nodes.at(rank));
  else
    result.explicit_type = t;
  
  return result;
}

QString type(const QString & t, const QStringList & types,
	     BrowserNodeList & nodes)
{
  int rank = types.findIndex(t);
  
  return (rank != -1) 
    ? QString(((BrowserClass *) nodes.at(rank))->get_name())
    : t;
}

void manage_alias(const BrowserNode * node,
		  const char *& p, QString & s,
		  KeyValuesTable * kvt)
{
  // p starts by '@'
  const char * pclosed;
  
  if ((p[1] == '{') && ((pclosed = strchr(p + 2, '}')) != 0)) {
    static char st[256];
    char * key = ((pclosed - p) > 255) ? new char[pclosed - p - 1] : st;
    QString qsvalue;
    
    strncpy(key, p + 2, pclosed - p - 2);
    key[pclosed - p - 2] = 0;

    if ((kvt != 0) && kvt->get_value(key, qsvalue)) {
      // find in dialog, insert the value
      s += qsvalue;
    }
    else {
      const char * value = 0;
      const BrowserNode * nd = (kvt != 0)
	? ((const BrowserNode *) node->parent())
	: node;
      
      while (nd != 0) {
	if ((value = nd->get_value(key)) != 0)
	  break;
	nd = (BrowserNode *) nd->parent();
      }
      
      if (key != st)
	delete [] key;
      
      if (value != 0)
	// find, insert the value
	s += value;
      else {
	// not find, insert the key
	s += "@{";
	s += key;
	s += "}";
      }
    }
    
    // bypass the key
    p += strlen(key) + 3;
  }
  else
    // bypass '@'
    s += *p++;
}
