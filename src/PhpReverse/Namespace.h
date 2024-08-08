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

#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <qvaluelist.h>
#include <qstringlist.h>
#include <qcstring.h>
#include <qdict.h>
#include <qmap.h>

#include "Lex.h"

// it is a singleton but I prefer static members
// Namespace::instance() is too long to hit

class Namespace {
  public:
    static void enter(QByteArray s);
    static void exit();
    
    static const QStringList stack() { return Stack; }
        
    static void add_alias(const QByteArray & a, const QByteArray & s) {
      Aliases.replace(a, s);
    }
    static void clear_aliases() { Aliases.clear(); }
    
    static QString namespacify(QByteArray s);
    static QByteArray current();
    
  private:
    static QStringList Stack;
    static QMap<QByteArray,QByteArray> Aliases;
};

// does not not inherit QDict to not allow to use directly
// a QDict operation, to force to take into account the namespace
// management

template<class T>
class NDict {
  public:
    NDict() {}
    NDict(unsigned n) { d.resize(n); }
  
    void insert(const QByteArray & key, const T * item);
    void replace(const QByteArray & key, const T * item);
    bool remove(const QByteArray & key);
    T * operator[] (const QByteArray & key) const;
      
  private:
    QDict<T> d;
};

template<class T>
void NDict<T>::insert(const QByteArray & key, const T * item) {
  d.insert(Namespace::namespacify(key), item);
}

template<class T>
void NDict<T>::replace(const QByteArray & key, const T * item) {
  d.replace(Namespace::namespacify(key), item);
}

template<class T>
bool NDict<T>::remove(const QByteArray & key) {
  return d.remove(Namespace::namespacify(key));
}

template<class T>
T * NDict<T>::operator[] (const QByteArray & key) const {
  QString k = Namespace::namespacify(key);
  T * r = d[k];
  
  if (r != 0)
    return r;
  
  QString s;
  
  if (((const char *) key)[0] != '\\') {
    QStringList::ConstIterator it;
    
    s = key;
    
    for (it = Namespace::stack().begin();
	 it != Namespace::stack().end();
	 ++it)
      if ((r = d[*it + s]) != 0)
	return r;
  }
  
  return r;
}

#endif
