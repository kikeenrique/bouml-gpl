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

#include <qdir.h>

#include "UmlPackage.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"

UmlPackage::UmlPackage(void * id, const QCString & n)
    : UmlBasePackage(id, n) {
  dir.read = FALSE;
}

static bool RootDirRead;
static QCString RootDir;

QCString UmlPackage::rootDir(bool cpp)
{
  if (! RootDirRead) {
    RootDirRead = TRUE;
    RootDir = (cpp) ? CppSettings::rootDir() : JavaSettings::rootDir();
    
    if (!RootDir.isEmpty() && // empty -> error
	QDir::isRelativePath(RootDir)) {
      QFileInfo f(getProject()->supportFile());
      QDir d(f.dirPath());
      
      RootDir = d.filePath(RootDir);
    }
  }

  return RootDir;
}

QCString UmlPackage::source_path(const QCString & f) {
  if (!dir.read) {
    dir.src = cppSrcDir();
    dir.h = cppHDir();
    
    QDir d_root(rootDir(TRUE));
    
    if (dir.src.isEmpty())
      dir.src = RootDir;
    else if (QDir::isRelativePath(dir.src))
      dir.src = d_root.filePath(dir.src);

    if (dir.h.isEmpty())
      dir.h = RootDir;
    else if (QDir::isRelativePath(dir.h))
      dir.h = d_root.filePath(dir.h);
   
    if (dir.src.isEmpty()) {
      UmlCom::trace(QCString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
		    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
		    "or edit the package (tab 'C++')</b></font><br>");
      UmlCom::bye();
      UmlCom::fatal_error("UmlPackage::source_path");
    }
        
    dir.read = TRUE;
  }
  
  QDir d(dir.src);
  
  return QCString(d.filePath(f)) + QCString(".") + CppSettings::sourceExtension();
}

QCString UmlPackage::header_path(const QCString & f) {
  if (!dir.read) {
    source_path(f);
   
    if (dir.h.isEmpty()) {
      UmlCom::trace(QCString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
		    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
			    "or edit the package (tab 'C++')</b></font><br>");
      UmlCom::bye();
      UmlCom::fatal_error("UmlPackage::source_path");
    }
    
    if (QDir::isRelativePath(dir.h)) {
      UmlCom::trace(QCString("<font color=\"red\"><b><i>")
		    + name() + "</i>'s header path <i>(" + dir.h
		    + "</i>) is not absolute, edit the <i> generation settings</i> "
		    "(tab 'directory'), or edit the package (tab 'C++')</b></font><br>");
      UmlCom::bye();
      UmlCom::fatal_error("UmlPackage::source_path");
    }
  }
  
  QDir d(dir.h);
  
  return QCString(d.filePath(f)) + QCString(".") + CppSettings::headerExtension();
}

void UmlPackage::roundtrip_cpp() {
  QVector<UmlItem> ch = UmlItem::children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    ch[index]->roundtrip_cpp();
}

QCString UmlPackage::java_path(const QCString & f) {
  if (!dir.read) {
    dir.src = javaDir();
    
    QDir d_root(rootDir(FALSE));
    
    if (dir.src.isEmpty())
      dir.src = RootDir;
    else if (QDir::isRelativePath(dir.src))
      dir.src = d_root.filePath(dir.src);

    if (dir.src.isEmpty()) {
      UmlCom::trace(QCString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
		    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
		    "or edit the package (tab 'Java')</b></font><br>");
      UmlCom::bye();
      UmlCom::fatal_error("UmlPackage::java_path");
    }
        
    dir.read = TRUE;
  }
  
  QDir d(dir.src);
  
  return QCString(d.filePath(f)) + QCString(".") + JavaSettings::sourceExtension();
}

void UmlPackage::roundtrip_java() {
  QVector<UmlItem> ch = UmlItem::children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    ch[index]->roundtrip_java();
}

UmlPackage * UmlPackage::package() {
  return this;
}
