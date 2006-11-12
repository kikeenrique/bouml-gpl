
#include "UmlArtifact.h"

#include <qdir.h>
#include <qmessagebox.h>
#include <qtextstream.h>

#include "CppSettings.h"
#include "UmlPackage.h"
#include "UmlCom.h"
#include "UmlClass.h"
#include "Dialog.h"

void UmlArtifact::genpro() {
  UmlPackage * pack = (UmlPackage *) parent()->parent();
  
  QCString path;

  if (! propertyValue("genpro path", path)) {

    path = pack->cppSrcDir();
  
    if (path.isEmpty()) {
      path = CppSettings::rootDir();
      if (path.isEmpty()) {
        QMessageBox::critical((QWidget *) 0, "Error", "Generation directory must be set");
        return;
      }
    }
    else if (QDir::isRelativePath(path)) {
      QDir d(CppSettings::rootDir());
      
      d.cd(path);
      path = d.absPath();
    }
  }

  if (stereotype() == "executable") {
    gen_app(path);
  }
  else
    UmlCom::trace(stereotype() + " : not managed");
}

void UmlArtifact::gen_app(const QCString & path) {
  QCString target;
  QCString pro;

  propertyValue("genpro target", target);
  propertyValue("genpro pro", pro);

  if (target.isEmpty()) {
    if ((target = name()) == "executable")
      target = UmlPackage::getProject()->name();
#ifdef WIN32
    target += ".exe";
#endif
  }

  if (pro.isEmpty()) {
    pro = target;
#ifdef WIN32
    if (pro.right(4) == ".exe")
      pro.resize(pro.length() - 4);
#endif
    
    QDir d(path);
    
    pro = d.absFilePath(pro + ".pro");
  }

  QCString tmplt;
  QCString config;
  QCString defines;
  QCString includepath;
  QCString dependpath;
  QCString objectsdir;

  if (!propertyValue("genpro tmplt", tmplt))
    tmplt = "app";
  if (!propertyValue("genpro config", config))
    config = "debug warn_on qt";
  if (!propertyValue("genpro defines", defines))
    defines = "WITHCPP WITHJAVA WITHIDL";
  propertyValue("genpro includepath", includepath);
  propertyValue("genpro dependpath", dependpath);
  propertyValue("genpro objectsdir", objectsdir);
  
  for (;;) {
    Dialog dialog(this, path, pro, target, tmplt, config,
		  defines, includepath, dependpath, objectsdir);
    
    if (dialog.exec() != QDialog::Accepted)
      return;
    
    set_PropertyValue("genpro pro", pro);
    set_PropertyValue("genpro path", path);
    set_PropertyValue("genpro target", target);
    set_PropertyValue("genpro tmplt", tmplt);
    set_PropertyValue("genpro config", config);
    set_PropertyValue("genpro defines", defines);
    set_PropertyValue("genpro includepath", includepath);
    set_PropertyValue("genpro dependpath", dependpath);
    set_PropertyValue("genpro objectsdir", objectsdir);

    QFile f(pro);
    
    if (! f.open(IO_WriteOnly))
      QMessageBox::critical((QWidget *) 0, "Error", "Cannot open " + QString(pro));
    else {
      QTextStream t(&f);
      QFileInfo tfi(target);
      QFileInfo pfi(pro);
      
      t << "TEMPLATE\t= " << tmplt << '\n';
      t << "TARGET\t\t= " << tfi.fileName() << '\n';
      if ((target.find('/') != -1) &&
	  (pro.find('/') != -1) &&
	  (tfi.dirPath(TRUE) != pfi.dirPath(TRUE)))
	t << "DESTDIR\t\t= " << tfi.dirPath(TRUE) << '\n';
      if (! objectsdir.isEmpty())
	t << "OBJECTS_DIR\t= " << objectsdir << '\n';
      t << "CONFIG\t\t+= " << config << '\n';
      if (!includepath.isEmpty())
	t << "INCLUDEPATH\t= " << includepath << '\n';
      if (!dependpath.isEmpty())
	t << "DEPENDPATH\t= " << dependpath << '\n';
      if (!defines.isEmpty())
	t << "DEFINES\t\t= " << defines << '\n';
      
      QString prodir = pfi.dirPath(TRUE);
      const QVector<UmlArtifact> & arts = associatedArtifacts();
      unsigned index;
      const char * sep;
      QCString ext;
      
      ext = CppSettings::headerExtension();
      sep = "HEADERS\t\t= ";
      for (index = 0; index != arts.count(); index += 1) {
	UmlArtifact * art = arts[index];
	
	if ((art->stereotype() == "source") && !art->cppHeader().isEmpty()) {
	  QString s = art->way(prodir, TRUE);
	  
	  if (! s.isEmpty()) {
	    t << sep << s << art->name() << '.' << ext;
	    sep = " \\\n\t\t  ";
	  }
	}
      }
      
      t << '\n';
      
      ext = CppSettings::sourceExtension();
      sep = "SOURCES\t\t= ";
      for (index = 0; index != arts.count(); index += 1) {
	UmlArtifact * art = arts[index];
	
	if ((art->stereotype() != "source") || !art->cppSource().isEmpty()) {
	  QString s = art->way(prodir, FALSE);
	  
	  if (! s.isEmpty()) {
	    t << sep << s << art->name();
	    if (art->stereotype() == "source")
	      t << '.' << ext;
	    sep = " \\\n\t\t  ";
	  }
	}
      }
      
      t << '\n';
      
      f.close();
      return;
    }
  }
}

QString UmlArtifact::way(QString pro_dir, bool header) {
  QString dir;
  
  if (stereotype() != "source") {
    return stereotype() + " not yet managed";
  }
  else if (header) {
    if (cppHeader().isEmpty())
      return 0;
      
    dir = ((UmlPackage *) parent()->parent())->cppHDir();
  }
  else {
    if (cppSource().isEmpty())
      return 0;
      
    dir = ((UmlPackage *) parent()->parent())->cppSrcDir();
  }

  if (QDir::isRelativePath(dir)) {
    QDir d(CppSettings::rootDir());
    
    d.cd(dir);
    dir = d.absPath();
  }

  if (dir.at(dir.length() - 1) != '/')
    dir += "/";
    
  if (pro_dir.at(pro_dir.length() - 1) != '/')
    pro_dir += "/";
    
  if (dir == pro_dir)
    return "./";
    
  // bypass common path header
  
  unsigned index = 0;
  unsigned i;
  QString s;
  
  for (;;) {
    int i;
    
    if ((i = pro_dir.find("/", index) + 1) != 0) {
      if (pro_dir.left(i) == dir.left(i))
	index = i;
      else if (dir.length() == index) {
	// pro_dir.left(index) == dir
	do
	  s += "../";
	while ((i = pro_dir.find("/", i) + 1) != 0);
	return s;
      }
      else
	break;
    }
    else
      // dir.left(index + 1) == pro_dir
      return dir.mid(index);
  }

  if (index == 0)
    // path fully different
    return dir;
  
  // dir.left(index) == pro_dir.left(index)
  // dir.mid(index) != pro_dir.mid(index) and they are not empty
  // dir.at(index - 1) == pro_dir.at(index - 1) == '/'
  i = index;
  while ((i = pro_dir.find("/", i) + 1) != 0)
    s += "../";
  return s + dir.mid(index);
}

