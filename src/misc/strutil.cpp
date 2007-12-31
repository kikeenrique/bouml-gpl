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

#include <stdlib.h>
#include <qtextcodec.h>

#include "strutil.h"
#include "CharSetDialog.h"

bool manage_comment(QString comment, const char *& p,
		    const char *& pp, bool javadoc)
{
  static QString the_comment;
  
  p += 10;
  
  if ((pp != 0) || // comment contains ${comment} !
      comment.isEmpty())
    return FALSE;
  
  const char * co = comment;
  
  if (javadoc) {
    the_comment = "/**\n * ";
    
    do {
      the_comment += *co;
      if ((*co++ == '\n') && *co)
	the_comment += " * ";
    } while (*co);
    
    if (*p != '\n')
      the_comment += (co[-1] != '\n') ? "\n */\n" : " */\n";
    else
      the_comment += (co[-1] != '\n') ? "\n */" : " */";
  }
  else {
    the_comment = "//";
    
    do {
      the_comment += *co;
      if ((*co++ == '\n') && *co)
	the_comment += "//";
    } while (*co);
    
    if (*p != '\n')
      the_comment += '\n';
  }
    
  pp = p;
  p = the_comment;
  return TRUE;
}

bool manage_description(QString comment, const char *& p, const char *& pp)
{
  static QString the_comment;
  
  p += 14;
  
  if ((pp != 0) || // comment contains ${description} !
      comment.isEmpty())
    return FALSE;
  
  the_comment = comment;
  
  if (*p != '\n')
    the_comment += '\n';
    
  pp = p;
  p = the_comment;
  return TRUE;
}

bool is_char_of_name(char c)
{
  return (((c >= 'a') && (c <= 'z')) ||
	  ((c >= 'A') && (c <= 'Z')) ||
	  ((c >= '0') && (c <= '9')) ||
	  (c == '_'));
}

QString extract_name(QString s)
{
  int index = s.find("${name}");
  
  if (index == -1)
    // !!
    return QString::null;
  
  const char * p = s;

  int start = index;
  
  while (start > 0 && is_char_of_name(p[start - 1]))
    start -= 1;
  
  int end = index + 7;
  
  while (is_char_of_name(p[end]))
    end += 1;
  
  return s.mid(start, end - start);
}

QString true_name(const QString & name, const QString & decl)
{
  int index;
  QString n;
  
  n = ((index = name.find(' ')) == -1)
    ? name
    : name.left(index);
  
  QString s = extract_name(decl);
  
  return (s.isEmpty()) ? n : s.replace(s.find("${name}"), 7, n);
}

QString capitalize(const QString & s)
{
  return (s.isEmpty())
    ? s
    : s.left(1).upper() + s.mid(1);
}

QString quote(QString s)
{
  QString result;
  const char * p = s;
  
  for (;;) {
    switch (*p) {
    case 0:
      return result;
    case '<':
      result += "&lt;";
      break;
    case '>':
      result += "&gt;";
      break;
    case '&':
      result += "&amp;";
      break;
    default:
      result += *p;
    }

    p += 1;
  }
}

void remove_comments(QCString & s)
{
  int index1 = 0;
  
  while ((index1 = s.find('/', index1)) != -1) {
    int index2;
    
    switch (((const char *) s)[index1 + 1]) {
    case '/':
      if ((index2 = s.find('\n', index1 + 2)) != -1)
	s.remove(index1, index2 - index1 + 1);
      else
	s.remove(index1, ~0);
      break;
    case '*':
      if ((index2 = s.find("*/", index1 + 2)) != -1)
	s.replace(index1, index2 - index1 + 1, " ");
      else
	s.remove(index1, ~0);
      break;
    default:
      index1 += 1;
    }
  }
}

void remove_preprocessor(QCString & s)
{
  int index = 0;
  
  while ((index = s.find('#', index)) != -1) {
    // remove all up to the end of line
    int index2 = index + 1;
    int index3;
    
    while ((index3 = s.find('\n', index2)) != -1) {
      // manage multi lines #define
      if (((const char *) s)[index3 - 1] != '\\')
	break;
      else
	index2 = index3 + 1;
    }
    
    // the \n is still here to have a separator
    if (index3 == -1)
      s.truncate(index);
    else
      s.remove(index, index3 - index);
  }
}


QString java_multiplicity(QString m)
{
  QString r;

  if (! m.isEmpty()) {
    if (*m != '[')
      r += "[]";
    else {
      for (unsigned index = 0; index != m.length(); index += 1) {
	switch (m.at(index).latin1()) {
	case '[':
	  r += '[';
	  break;
	case ']':
	  r += ']';
	default:
	  break;
	}
      }
    }
  }
  
  return r;
}

//

static QTextCodec * Codec;

bool hasCodec() 
{
  return Codec != 0;
}


void initCodec()
{
  // QApplication::font().charSet() returns 'AnyCharSet'
  // whose don't have a codec => use BOUML_CHARSET
  QString cs = getenv("BOUML_CHARSET");
  
  if (cs.isEmpty()) 
    Codec = 0;
  else if ((Codec = QTextCodec::codecForName(cs)) == 0) {
    CharSetDialog d(cs);
    
    if (d.exec() == QDialog::Accepted)
      Codec = QTextCodec::codecForName(d.choosen());
  } 
}

QString toUnicode(const char * str)
{
  if (Codec == 0)
    return str;
  else if ((str == 0) || (*str == 0))
    return QString::null;
  else
    return Codec->toUnicode(str);
}

void latinize(QString & s)
{
  unsigned i = s.length();
  
  while (i != 0) {
    QChar c = s.at(--i);
    
    if (c.latin1() == 0) {
      if (c.unicode() == 8217)
	// special case for the pseudo ' returned by microsoft editors
	c = '\'';
      else
	c = ' ';
      s.replace(i, 1, &c, 1);
    }
  }
}

QCString fromUnicode(const QString & s)
{
  if (Codec == 0) {
    QString str = s;

    latinize(str);
    return QCString(str);
  }
  else if (s.isEmpty())	// fromUnicode crash if null string
    return "";
  else
    return Codec->fromUnicode(s);
}
