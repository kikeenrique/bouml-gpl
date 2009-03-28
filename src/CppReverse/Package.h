// *************************************************************************
//
// Copyleft 2004-2009 Bruno PAGES  .
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
#include <qlist.h>

#include "BrowserNode.h"
#include "ClassContainer.h"

class QDir;
class UmlPackage;
class Progress;
class QApplication;

class Package : public BrowserNode, public ClassContainer {
  public:
#ifndef REVERSE
    Package(BrowserView * parent, UmlPackage * u);
#endif
    Package(Package * parent, QString p, QString n);
    virtual ~Package();	// just to not have warning
    
    void reverse_file(QCString f);
    void reverse_variable(const QCString & name);
    
    Class * declare_if_needed(const QCString & name,
			      QCString stereotype = 0);
    virtual Class * define(const QCString & name, QCString stereotype = 0);
    virtual void declare_if_needed(QCString name, Class * cl);
    virtual void define(QCString name, Class * cl);
    virtual bool find_type(QCString type, UmlTypeSpec & typespec);
    virtual void declaration(const QCString & name, const QCString & stereotype,
			     const QCString & decl);

    virtual bool isa_package() const;
    const QCString & get_path() const { return path; };
    UmlPackage * get_uml(bool mandatory = TRUE);
#ifndef REVERSE
    virtual void menu();
    virtual void refer(const QString & name);
    
    const QPixmap * pixmap(int) const;
    
    virtual void backup(QDataStream  & dts) const;
    void backup_children(QDataStream  & ts) const;
#endif
    void restore_children(QDataStream & dts);
    static void restore(QDataStream  & dt, Package *);
    
    static void init(UmlPackage *, QApplication *);
    static bool scanning() { return Scan; };
    static bool scan_dirs();
    static void send_dirs(bool rec);
    
    static void progress_closed();
    static const QCString & get_fname() { return fname; }
    
    //static Package * unknown();
    
  private:
    static QApplication * app;
    static QList<Package> Choozen;
    static Progress * ProgressBar;
    static int Nfiles;
    static bool Scan;
    static Package * Root;
    //static Package * Unknown;
    static QValueList<FormalParameterList> Formals;
    static QList<UmlClass> UsedClasses;
    
    static NDict<Class> Declared;
    static NDict<Class> Defined;
    
    UmlPackage * uml;
    QCString namespace_;
    QCString path;
    static QCString fname;	// without extension, empty for non header file
    
    void reverse_directory(QDir & dir, bool rec, QString ext, bool h);
    void reverse_toplevel_forms(QCString f, bool sub_block);
    void reverse_toplevel_form(QCString f, QCString s);
        
    Class * new_class(const QCString & name, const QCString & stereotype,
		      bool declaration);

    void set_namespace(QCString s);
    Package * find(QCString s);
    
    static int file_number(QDir & d, bool rec, const char * h, const char * cpp);
};

#endif
