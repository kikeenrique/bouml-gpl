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

// lexer written by hand rather than using [f]lex as usual,
// BOUML is also my first development including a source code
// reader where I do not use yacc/bison, to see how it is
// without ...

#ifdef TRACE
#include <iostream>

using namespace std;
#endif

// Because stdio and fstream are bugged under Windows
// concerning fseek and equivalent, I read all the file in a buffer
// and manage it after.
#include <qfile.h>

#include "Lex.h"
#include "JavaCatWindow.h"

// the current context
Lex::Context Lex::context;

// to save/restore the current context
QStack<Lex::Context> Lex::stack;

void Lex::push_context()
{
  Context * c = new Context;
  
  *c = context;
  stack.push(c);
}

void Lex::pop_context()
{
  Context * c = stack.pop();
  
  context = *c;
  delete c;  
}

int Lex::get() 
{
  return (*context.pointer == 0) ? EOF : *context.pointer++;
}

int Lex::peek()
{
  return (*context.pointer == 0) ? EOF : *context.pointer;
}

void Lex::unget()
{
  context.pointer -= 1;
}

static QCString Separators = " \r\t\f\n&~\"#{'(-|`)[]=}%*<>?,;/:!@";

const QString & Lex::filename()
{
  return context.filename;
}

unsigned Lex::line_number()
{
  return context.line_number;
}

bool Lex::open(const QString & f)
{
#ifdef TRACE
  cout << "Lex::open(" << f << ")\n";
#endif
  
  context.filename = f;
  context.line_number = 1;
  context.comments = "";
  context.description = "";
  context.reread = "";
  context.mark = 0;

  QFile in(f);
  unsigned sz;
  
  if (!in.open(IO_ReadOnly) ||
      ((context.buffer = new char[(sz = in.size()) + 1]) == 0))
    return FALSE;
    
  unsigned offset = 0;
  
  do offset += in.readBlock(context.buffer + offset, sz - offset);
  while (offset != sz);

  context.buffer[sz] = 0;
  context.pointer = context.buffer;
  
  return TRUE;
}

void Lex::close()
{
#ifdef TRACE
  cout << "Lex::close()\n";
#endif
  delete [] context.buffer;
}

void Lex::unread_word(const char * s)
{
  context.reread = s;
}

void Lex::complete_template(QString & result)
{
  // template
  unsigned level = 1;
	  
  result += '<';
  
  for (;;) {
    int c = get();
    
    if (c == EOF)
      return;
    
    result += c;
    
    if (c == '<')
      level += 1;
    else if ((c == '>') && (--level == 0))
      break;
  }
}

void Lex::bypass_cpp_comment()
{
  get();	// second '/'
  
  if (! context.comments.isEmpty())
    context.comments += '\n';
  context.description += (! context.description.isEmpty())
    ? "\n//" :  "//";
  
  int c;
  char * p = context.pointer;
  
  for (;;) {
    c = get();
    if (c == EOF) {
      context.comments += p;
      context.description += p;
      return;
    }
    if (c == '\n') {
      context.line_number += 1;
      context.pointer[-1] = 0;
      context.comments += p;
      context.description += p;
      context.pointer[-1] = '\n';
      return;
    }
  }
}

// '/' read, '*' only peek

void Lex::bypass_c_comment()
{
  while (*context.pointer == '*')
    context.pointer += 1;
  
  if ((*context.pointer == '/') && (context.pointer[-2] != '/')) {
    // /**/ or /***/ etc ...
    context.pointer += 1;
    return;
  }
  
  if (! context.comments.isEmpty())
    context.comments += '\n';
  context.description += (! context.description.isEmpty())
    ? "\n/*" :  "/*";
  
  char * p = context.pointer;	// start of the comment on the line
  
  for (;;) {
    int c = get();
    
    switch (c) {
    case EOF:
      return;
    case '\n':
      context.line_number += 1;
      if ((p != 0) && (context.pointer != (p + 1))) {
	// non empty line
	c = *context.pointer;
	*context.pointer = 0;
	context.comments += p;
	context.description += p;
	*context.pointer = c;
      }
      else {
	context.comments += c;
	context.description += c;
      }
  
      p = 0;
      break;
    case '*':
      if (peek() == '/') {
	if ((p != 0) && (context.pointer != (p + 1))) {
	  // non empty line
	  context.pointer[-1] = 0;
	  context.comments += p;
	  context.pointer[-1] = '*';
	  c = context.pointer[1];
	  context.pointer[1] = 0;
	  context.description += p;
	  context.pointer[1] = c;
	}
	get();	// '/'
	return;
      }
      if (p == 0) {
	if (peek() == ' ')
	  get();
	p = context.pointer;
      }
      break;
    case ' ':
    case '\t':
      break;
    default:
      if (p == 0)
	p = context.pointer - 1;
    }
  }
}

QCString Lex::manage_operator(QString & result, int c)
{
  result += c;
  
  int next = peek();
  
  switch (c) {
  case '!':
  case '%':
  case '*':
  case '/':
  case '=':
  case '^':
    if (next == '=')
      result += get();
    break;
  case '<':
  case '>':
    if (next == '=')
      result += get();
    else if (next == c) {
      result += get();
      if (peek() == '=')
	result += get();
    }
    break;
  case '-':
  case '&':
  case '+':
  case '|':
    if ((next == '=') || (next == c))
      result += get();
    break;
  }
  
#ifdef TRACE
  cout << "retourne '" << result << "'\n";
#endif
  return QCString(result);
}

QCString Lex::read_string()
{
  QString result = "\"";;
  
  for (;;) {
    int c = get();
    
    switch (c) {
    case EOF:
      return 0;
    case '\\':
      c = get();
      if (c == '\n')
	context.line_number += 1;
      result += '\\';
      result += c;
      break;
    case '"':
      return QCString(result += c);
    default:
      result += c;
    }
  }
}

QCString Lex::read_character()
{
  QCString result = "'";
  
  for (;;) {
    int c = get();
    
    switch (c) {
    case EOF:
      return 0;
    case '\'':
      return QCString(result += c);
    case '\\':
      result += c;
      result += get();
      break;
    default:
      result += c;
    }
  }
}

QCString Lex::read_array_dim() 
{
  QCString result = "[";
	  
  for (;;) {
    int c = get();
    
    if (c == EOF)
      break;
    
    result += c;
    
    if (c == ']')
      break;
  }
  
#ifdef TRACE
  cout << "retourne '" << result << "'\n";
#endif
  return result;
}

// read all sequential annotations

QCString Lex::read_annotation()
{
  char * p1 = context.pointer - 1;	// '@' was read

  for (;;) {
    if (read_word().isEmpty())
      // eof
      return p1;
  
    char * p2 = context.pointer;
    QCString s = read_word();
    
    if (s == "(") {
      int level = 1;
      
      for (;;) {
	s = read_word();
	
	if (s.isEmpty())
	  // eof
	  return p1;
	else if (s == "(")
	  level += 1;
	else if ((s == ")") && (--level == 0))
	  break;
      }
      
      p2 = context.pointer;
      s = read_word();
    }
    
    if (s.isEmpty())
      return p1;
    else if (*s == '@') {
      int c = *context.pointer;
      
      *context.pointer = 0;
      
      QCString result = p1;
      
      *context.pointer = c;
      return result;
    }
    else  {
      context.pointer = p2;
      
      int c = *p2;
      
      *p2 = 0;
      
      QCString result = p1;
      
      *p2 = c;
      return result;
    }
  }
}


QCString Lex::read_word(bool in_templ)
{
  QString result;
  
  if (!context.reread.isEmpty()) {
    result = context.reread;
    
    if (in_templ && (*result == '>')) {
      // >> and >>= read as >
      context.reread = result.mid(1);
      result = result.left(1);
    }
    else
      context.reread = QString::null;
  }
  else {
    static QCString spaces = " \t\n\r";
    
    for (;;) {
      int c = get();
      
#ifdef TRACE
      //cout << "deja \"" << result << "\", '" << ((char) c) << "'\n";
#endif
      if (c == EOF)
	break;
      else if (Separators.find(c) == -1)
	result += c;
      else if (! result.isEmpty()) {
	unget();
	break;
      }
      else {
	switch (c) {
	case '"':
	  return read_string();
	case '[':
	  return read_array_dim();
	case '\'':
	  return read_character();
	case '/':
	  switch (peek()) {
	  case '/':
	    bypass_cpp_comment();
	    break;
	  case '*':
	    bypass_c_comment();
	    break;
	  case '=':
	    get();
#ifdef TRACE
	    cout << "retourne '/='\n";
#endif
	    return "/=";
	  default:
#ifdef TRACE
	    cout << "retourne '/'\n";
#endif
	    return "/";
	  }
	  break;
	case '\n':
	  context.line_number += 1;
	  break;
	case '@':
	  return read_annotation();
	case '>':
	  if (in_templ)
	    return ">";
	  // no break
	default:
	  if (c > ' ')
	    return manage_operator(result, c);
	  break;
	}
      }
    }
  }
#ifdef TRACE
  cout << "retourne '" << result << "'\n";
#endif
  return QCString(result);
}

void Lex::finish_line()
{
  for (;;) {
    int c = get();
      
    switch (c) {
    case '/':
      switch (peek()) {
      case '/':
	bypass_cpp_comment();
	return;
      case '*':
	bypass_c_comment();
	continue;
      }
      // no break
    default:
      unget();
      return;
    case '\n':
      context.line_number += 1;
      // no break
    case EOF:
      return;
    case ' ':
    case '\t':
      break;
    }
  }
}

QCString Lex::get_comments() 
{
  QCString result = QCString(context.comments);
  
  context.comments = QString::null;
  return result;
}

QCString Lex::get_comments(QCString & co) 
{
  QCString result = QCString(context.comments);
  
  context.comments = QString::null;
  
  return (co.isEmpty())
    ? result
    : co += "\n" + result;
}

QCString Lex::get_description() 
{
  QCString result = QCString(context.description);
  
  context.description = QString::null;
  return result;
}

QCString Lex::get_description(QCString & co) 
{
  QCString result = QCString(context.description);
  
  context.description = QString::null;
  
  return (co.isEmpty())
    ? result
    : co += "\n" + result;
}

void Lex::clear_comments()
{
  context.comments = QString::null;
}

bool Lex::identifierp(const char * s)
{
  return (Separators.find(*s) == -1);
}

void Lex::mark() {
  context.mark = context.pointer;
}

QCString Lex::region() {
  char c = *context.pointer;
  
  *context.pointer = 0;
  
  QCString result = context.mark;
  
  *context.pointer = c;
  
  return result;
}

//

void Lex::syntax_error(QCString s)
{
  JavaCatWindow::trace(QCString("<font face=helvetica>syntax error in <i> ")
			+ QCString(context.filename) + "</i> line " +
			QCString().setNum(context.line_number) + " <b>"
			+ s + "</b></font><br>"); 
  
#ifdef TRACE
  cout << "ERROR IN " << context.filename
    << " LINE " << context.line_number << " : " << s << '\n';
#endif
}

void Lex::premature_eof()
{
  JavaCatWindow::trace(QCString("<font face=helvetica>syntax error in <i> ")
		       + QCString(context.filename) + "</i> line " +
		       QCString().setNum(context.line_number) +
		       " <b>premature eof</b></font><br>"); 
  
#ifdef TRACE
  cout << "SYNTAX ERROR IN " << context.filename 
    << " LINE " << context.line_number << " : premature eof\n";
#endif
}

void Lex::error_near(QCString s)
{
  JavaCatWindow::trace(QCString("<font face=helvetica>syntax error in <i> ")
		       + QCString(context.filename) + "</i> line " +
		       QCString().setNum(context.line_number) + " <b>near <font color =\"red\">"
		       + quote(s) + "</font></b></font><br>"); 
  
#ifdef TRACE
  cout << "SYNTAX ERROR IN " << context.filename
    << " LINE " << context.line_number << " : near '" << s << "'\n";
#endif
}

// allows a string to be written as it is by an html writer

QCString Lex::quote(QCString s)
{
  QCString result;
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
