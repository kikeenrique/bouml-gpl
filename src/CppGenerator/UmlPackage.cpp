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

#include <qdir.h>

#include "UmlPackage.h"
#include "UmlCom.h"
#include "CppSettings.h"

UmlPackage::UmlPackage(void * id, const QCString & n)
    : UmlBasePackage(id, n) {
  dir.read = FALSE;
}

static void create_directory(QCString s)
{
  int index = 0;
  char sep = QDir::separator();
  
  if (sep != '/') {
    while ((index = s.find(sep, index)) != -1)
      s.replace(index++, 1, "/");
  }
  
  s = QDir::cleanDirPath(s) + "/";
  index = s.find("/");
  
  int index2;
  
  while ((index2 = s.find("/", index + 1)) != -1) {
    QCString s2 = s.left(index2);
    QDir sd(s2);
    
    if (!sd.exists()) {
      if (!sd.mkdir(s2)) {
	UmlCom::trace(QCString("<font color=\"red\"><b> cannot create directory <i>")
		      + s2 + "</i></b></font><br>");
	UmlCom::bye();
	UmlCom::fatal_error("UmlPackage::file_path");
      }
    }
    index = index2;
  }
}

static bool RootDirRead;
static QCString RootDir;

QCString UmlPackage::source_path(const QCString & f) {
  if (!dir.read) {
    dir.src = cppSrcDir();
    dir.h = cppHDir();
    
    if (! RootDirRead) {
      RootDirRead = TRUE;
      RootDir = CppSettings::rootDir();

      if (!RootDir.isEmpty() && // empty -> error
	  QDir::isRelativePath(RootDir)) {
	QFileInfo f(getProject()->supportFile());
	QDir d(f.dirPath());

	RootDir = d.filePath(RootDir);
      }
    }

    QDir d_root(RootDir);
    
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
  
  if (! d.exists())
    create_directory(dir.src);	// don't return on error
  
  return QCString(d.filePath(f)) + QCString(".") + 
    CppSettings::sourceExtension();
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
  
  if (! d.exists())
    create_directory(dir.h);	// don't return on error
  
  return QCString(d.filePath(f)) + QCString(".") + 
    CppSettings::headerExtension();
}

void UmlPackage::generate() {
  QVector<UmlItem> ch = UmlItem::children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    ch[index]->generate();
}

UmlPackage * UmlPackage::package() {
  return this;
}
