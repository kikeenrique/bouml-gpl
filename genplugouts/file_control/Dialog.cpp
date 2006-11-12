
#include "Dialog.h"

#include <qlayout.h> 
#include <qlineedit.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
Dialog::Dialog(bool ci, QCString & cmd, bool & rec, bool & reload)
  : QDialog(0, 0, TRUE), _cmd(cmd), _rec(rec), _reload(reload) {
    QVBoxLayout * vbox = new QVBoxLayout(this);
    QHBox * htab;
    
    vbox->setMargin(5);
    
    // add recursive check box
    
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    
    rec_cb = new QCheckBox((ci) ? "Check-in sub packages" : "Check-out sub-packages",
  			 htab);
    
    // add comment
    
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    new QLabel("In the command %file will be replaced by the file's name"
  	     " and %dir by the directory where is the file", htab);
    
    // add command edit
    
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
  
    new QLabel("Command : ", htab);
    cmd_ed = new QLineEdit(htab);
    cmd_ed->setText(_cmd);
  
    // save & load
    
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    
    reload_cb = new QCheckBox("Save then reload the project to see the new write permission of the files",
  			    htab);  
    reload_cb->setChecked(TRUE);
    
    // ok & cancel
      htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    
    new QLabel(htab);
    QPushButton * ok = new QPushButton("&Ok", htab);
    new QLabel(htab);
    QPushButton * cancel = new QPushButton("&Cancel", htab);
    new QLabel(htab);
    QSize bs(cancel->sizeHint());
    
    ok->setFixedSize(bs);
    cancel->setFixedSize(bs);
    
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void Dialog::accept() {
  if (cmd_ed->text().find("%file") == -1)
    QMessageBox::critical(this, "Error", "the command must contain %file");
  else {
    _cmd = cmd_ed->text();
    _rec = rec_cb->isChecked();
    _reload = reload_cb->isChecked();
    QDialog::accept();
  }
}

