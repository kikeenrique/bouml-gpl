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

#ifndef PACKAGE_H
#define PACKAGE_H

#include <qdict.h>
#include <qstringlist.h> 
#include <qstack.h>

#include "Class.h"
#include "UmlClass.h"

#include "BrowserNode.h"
#include "ClassContainer.h"

class QPixmap;
class QDir;

class UmlPackage;
class BrowserView;
class UmlClass;
class UmlItem;

class Progress;
class QApplication;

class Package : public BrowserNode, public ClassContainer {
  public:
#ifndef REVERSE
    Package(BrowserView * parent, UmlPackage * u);
#endif
    Package(Package * parent, const char * p, const char * n);
  
    virtual bool isa_package() const;

    void reverse_file(QCString f);
    
    const QCString & get_path() { return path; }
    UmlPackage * get_uml(bool mandatory = TRUE);
#ifdef REVERSE
    void send_dir(bool rec);
    const QStringList get_imports() { return imports; }
    const bool is_java_lang_added() { return java_lang_added; }
    const QStringList get_static_imports() { return static_imports; }
#endif
    
    void new_class(Class *);
    
    virtual void compute_type(QCString type, UmlTypeSpec & typespec,
			      const QValueList<FormalParameterList> & tmplts,
			      Class ** need_object = 0);
    virtual Class * define(const QCString & name, char st);
    virtual void declare(const QCString &, Class *);
    void force_class(QCString name, UmlTypeSpec & typespec, Class ** need_object);
    
#ifndef REVERSE
    virtual QString get_path() const;
    
    virtual void menu();
    virtual void refer(const QString & name);
    
    const QPixmap * pixmap(int) const;
    
    virtual void backup(QDataStream  & dts) const;
    void backup_children(QDataStream  & ts) const;
#endif
    void restore_children(QDataStream & dts);
    static void restore(QDataStream  & dt, Package *);

    static void init(UmlPackage *, QApplication *);
    static bool scanning() { return scan; };
    static Package * scan_dir();
    static Package * get_root() { return root; };
    
    static void push_context();
    static void pop_context();
    
    static void progress_closed();
    
  private:
    UmlPackage * uml;
    QCString package;		// java package may be with '.'
    QCString path;
    QDict<Class> Undefined;
  
    static bool scan;
    static Package * root;
    static Package * unknown;
    static QDict<Class> classes;
    static QDict<Class> java_classes;
    static QDict<UmlClass> user_classes;
    static QDict<Package> known_packages;
    static QDict<UmlPackage> user_packages;
    static QStringList unknown_packages;
    static QStringList imports;
    static bool java_lang_added;
    static QStringList static_imports;
    static QStack<QStringList> stack;
    static QValueList<FormalParameterList> Formals;
    static Progress * progress;
    static QApplication * app;
    
    static int file_number(QDir & dir, bool rec);
    
    static void manage_import();
    void reverse_toplevel_form(QCString s);
    void reverse_directory(QDir & dir, bool rec);
    
    void set_package(QCString s);
    Package * find(QCString s);
    static Package * package_unknown();
    
    static void update_package_list(QCString name);
    static void update_class_list(QCString pack, UmlItem * container);
    
    Class * declare_if_needed(QCString name, char st);
    Class * new_class(const QCString & name, char st);
};

#endif
