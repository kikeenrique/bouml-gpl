// *************************************************************************
//
// Copyright (C) 2004-2007 Bruno PAGES  All rights reserved.
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

#ifndef UMLCOM_H
#define UMLCOM_H

/* !!!!!!!!!! Do not modify this file !!!!!!!!!! */

#include <qvector.h>
#include <qstring.h>

#include "api.h"
#include "ApiCmd.h"

class QSocketDevice;
class UmlItem;
class UmlTypeSpec;
class UmlClass;

// This class manages the communications
//
// This class may be defined as a 'singleton', but I prefer to use static 
// members allowing to just write 'UmlCom::member' rather than
// 'UmlCom::instance()->member' or other long sentence like this.
//
// The operation you can use yourself are :
// - connect()
// - read_item_list()
// - trace()
// - traceAutoRaise()
// - showTrace()
// - message()
// - bye()
// - close()
// 
// you must NOT call the others

class UmlCom {
  public:
    // does the connexion, return FALSE on error
    static bool connect(unsigned port);
  
    // returns the item on which the tool is applied
    static UmlItem * targetItem();
    
    // to write messages in the tool window,
    // use Qt rich text : allows HTML like formatting
    static void trace(const char * s);
    
    // to show the trace window
    static void showTrace();
    
    // to automatically raise the trace window
    // each time trace() is called, even with ""
    static void traceAutoRaise(bool y);
    
    // to write a message in the status bar line
    // does not use Qt rich text
    static void message(const char * s);
    
    // must be called just before the disconnexion
    static void bye();
    
    // disconnexion
    static void close();
    
  protected:
    static QSocketDevice * sock;
  
    static char * buffer_in;
    static unsigned buffer_in_size;
    static char * p_buffer_in;
    static char * buffer_in_end;
    
    static char * buffer_out;
    static char * p_buffer_out;
    static unsigned buffer_out_size;
    
    static bool need_ack;
  
    static void check_size_out(unsigned len);
    static void read_if_needed();
    static void read_buffer(unsigned ln);
    
    static void write_bool(bool b);
    static void write_char(char c);
    static void write_unsigned(unsigned u);
    static void write_id(const void * id);
    static void write_string(const char * p);

  public:
    // do NOT call the followings yourself !!!!
    
    static void send_cmd(CmdFamily f, unsigned cmd);
    static void send_cmd(CmdFamily f, unsigned cmd, char);
    static void send_cmd(CmdFamily f, unsigned cmd, void *);
    static void send_cmd(CmdFamily f, unsigned cmd, const char * n);
    static void send_cmd(CmdFamily f, unsigned cmd, void *, const char * n);
    static void send_cmd(CmdFamily f, unsigned cmd, const char * s,
			 const char * v);
    static void send_cmd(CmdFamily f, unsigned int cmd, bool b, const char * s,
			 const char * v);
    static void send_cmd(const void * id, OnInstanceCmd cmd);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const char * arg);
    static void send_cmd(const void * id, OnInstanceCmd cmd, char arg);
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned arg);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const UmlTypeSpec & arg);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const char * arg1,
			 const char * arg2);
    static void send_cmd(const void * id, OnInstanceCmd cmd, anItemKind arg1,
			 const char * arg2);
    static void send_cmd(const void * id, OnInstanceCmd cmd, anItemKind arg1,
			 aRelationKind arg2, const void * id3);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const void * id1);
    static void send_cmd(const void * id, OnInstanceCmd cmd, const void * id1,
			 const char * arg2);
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned arg1,
			 const UmlTypeSpec & arg2);
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned arg1,
			 const char * arg2, const char * arg3,
			 const UmlTypeSpec & arg4, const UmlTypeSpec & arg5);
    static void send_cmd(const void * id, OnInstanceCmd cmd, unsigned arg1,
			 char arg2, const char * arg3, 
			 const char * arg4, const UmlTypeSpec & arg5);
    static void send_cmd(const void * id, OnInstanceCmd cmd,
			 const QVector<UmlClass> & l);
    static void send_cmd(const void * id, OnInstanceCmd cmd,
			 anItemKind arg, const void * id2);
    static void read_item_list(QVector<UmlItem> & l);
    static void * read_id();
    static const char * read_string();
    static bool read_bool();
    static char read_char();
    static unsigned read_unsigned();
    static unsigned user_id();
    static void set_user_id(unsigned uid);
    static bool set_root_permission(bool);
    static void with_ack(bool y);
    static bool read_ack();
    
    static void fatal_error(const QCString & msg);
    static void flush();
};

#endif
