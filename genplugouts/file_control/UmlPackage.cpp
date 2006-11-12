
#include "UmlPackage.h"

#include <stdlib.h>
#include <qfileinfo.h>

#include "Dialog.h"
#include "UmlCom.h"

void UmlPackage::fileControl(bool ci) {
  UmlPackage * prj = getProject();
  QCString prjfile = prj->supportFile();
  bool rec;
  bool reload;
  QCString cmd;
  
  if (! prj->propertyValue((ci) ? "check-in-cmd" : "check-out-cmd", cmd))
    cmd = "specify the command with the file replaced by %file";
  
  Dialog dialog(ci, cmd, rec, reload);	// the dialog execution set 'cmd' and 'rec'
  
  if (dialog.exec() == QDialog::Accepted) {
    // save the command for a future usage
    prj->set_PropertyValue((ci) ? "check-in-cmd" : "check-out-cmd", cmd);
    
    if (reload)
      saveProject();
    
    // get files list
    QDict<void> files;
  
    getFiles(files, (rec) ? ~0u : 1);
    if (this == prj)
      getAuxFiles(files);
  
    // apply the command on each file
    QDictIterator<void> it(files);
    int index_file = cmd.find("%file");
    bool has_dir = (cmd.find("%dir") != -1);
    QFileInfo prjpath(prjfile);
    QString dir = prjpath.dirPath(TRUE);
  
    while (it.current()) {
      QString s = cmd;
  
      s.replace(index_file, 5, it.currentKey());
      if (has_dir) {
        int index_dir = s.find("%dir");
        
        s.replace(index_dir, 4, dir);
      }
      
      system((const char *) s);
      ++it;
    }
    
    UmlCom::trace("Done.");
  
    if (reload)
      loadProject(prjfile);
  }
}

void UmlPackage::getFiles(QDict<void> & files, unsigned rec) {
  if (rec != 0)
    UmlItem::getFiles(files, rec - 1);
}

void UmlPackage::getAuxFiles(QDict<void> & files) {
  static const char * aux[] = {
    "cpp_includes", "generation_settings", "idl_includes",
    "java_imports", "stereotypes", "tools", 0
  };
  			
  const char ** p = aux;
  QFileInfo prjpath(supportFile());
  QString dir = prjpath.dirPath(TRUE) + "/";
  
  while(*p != 0) {
    QFileInfo fi(dir + *p);
    
    if (fi.exists())
      files.replace(*p, (void *) 1);
    
    p += 1;
  }
}

