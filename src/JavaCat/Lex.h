// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
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

#ifndef LEX_H
#define LEX_H

#include <qstack.h>

// fstream is bugged under Windows
#include <stdio.h>
#include <qstring.h>

class Lex {
  public:
    typedef struct {
      QString filename;
      unsigned line_number;
      char * buffer;
      char * pointer;
      QString reread;
      QString comments;
      QString description;
      char * mark;
    } Context;
  
  private:
    static int get();
    static int peek();
    static void unget();
    
    static void complete_template(QString & result);
    static QByteArray manage_operator(QString  & result, int c);  
    static QByteArray read_string();     
    static QByteArray read_character();    
    static QByteArray read_array_dim();  
    static QByteArray read_annotation();
    static void bypass_template();
    static void bypass_cpp_comment();
    static void bypass_c_comment();
    static char bypass_operator(int c);  
    static void bypass_string();     
    static void bypass_character();    
    static void bypass_annotation();  
    static void bypass_array_dim();  
    
    static Context context;
    static QStack<Context> stack;
    
  public:
    static QByteArray read_word(bool in_templ = FALSE);
    static char read_word_bis(bool in_templ = FALSE);
    static void unread_word(const char * s);
    static QByteArray get_comments();
    static QByteArray get_comments(QByteArray & co);
    static QByteArray get_description();
    static QByteArray get_description(QByteArray & co);
    static void clear_comments();
    static void finish_line();
    static bool open(const QString &);
    static void close();
    static const QString & filename();
    static unsigned line_number();
    static bool identifierp(const char *);
    static void mark();
    static QByteArray region();
    static void syntax_error(QByteArray = 0);
    static void premature_eof();
    static void error_near(QByteArray, const char * m = "");
    static QByteArray quote(QByteArray);
    static void push_context();
    static void pop_context();
    static QByteArray simplify_comment(QByteArray &);
    static bool bypass_type(QByteArray s);
};

bool neq(const QByteArray & s1, const QByteArray & s2);
bool nequal(const QByteArray & s1, const QByteArray & s2);

#endif
