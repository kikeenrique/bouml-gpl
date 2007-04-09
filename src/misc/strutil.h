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

#ifndef STRUTIL_H
#define STRUTIL_H

#include "qstring.h"

extern bool manage_comment(QString comment, const char *& p,
			   const char *& pp);
extern bool manage_description(QString comment, const char *& p,
			       const char *& pp);
extern QString capitalize(const QString & s);
extern QString true_name(const QString & name, const QString & decl);
extern bool is_char_of_name(char c);
extern QString extract_name(QString s);
extern QString quote(QString s);
extern void remove_comments(QCString & s);
extern void remove_preprocessor(QCString & s);

extern void initCodec();
extern QString toUnicode(const char *);
extern QCString fromUnicode(const QString &);
extern bool hasCodec();

#endif
