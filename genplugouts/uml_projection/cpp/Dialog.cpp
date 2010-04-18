
#include "Dialog.h"

#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qcheckbox.h>

Dialog::Dialog(BooL & rec, char & lang) : QDialog(0, 0, TRUE), _rec(rec), _lang(lang) {
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBox * htab;
  
  vbox->setMargin(5);
  
  // recursive checkbox
  if (rec) {
    htab = new QHBox(this);
    htab->setMargin(5);
    vbox->addWidget(htab);
    
    rec_cb = new QCheckBox("Do recursively", htab);
  }
  else
    rec_cb = 0;

  // langs + cancel buttons
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  QPushButton * cpp = new QPushButton("&C++", htab);
  new QLabel(htab);
  QPushButton * java = new QPushButton("&Java", htab);
  new QLabel(htab);
  QPushButton * idl = new QPushButton("&Idl", htab);
  new QLabel(htab);
  QPushButton * php = new QPushButton("P&hp", htab);
  new QLabel(htab);
  QPushButton * python = new QPushButton("P&ython", htab);
  new QLabel(htab);
  QPushButton * cancel = new QPushButton("&Cancel", htab);
  new QLabel(htab);
  QSize bs(cancel->sizeHint());
  
  cpp->setFixedSize(bs);
  java->setFixedSize(bs);
  
  connect(cpp, SIGNAL(clicked()), this, SLOT(accept_cpp()));
  connect(java, SIGNAL(clicked()), this, SLOT(accept_java()));
  connect(idl, SIGNAL(clicked()), this, SLOT(accept_idl()));
  connect(php, SIGNAL(clicked()), this, SLOT(accept_php()));
  connect(python, SIGNAL(clicked()), this, SLOT(accept_python()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

  // help
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  new QLabel(htab);
  new QLabel("Warning : reset the declarations/definitions to\n"
	     "their default value from the 'generation settings'", htab);
  new QLabel(htab);
}

void Dialog::accept_cpp() {
  _lang = 'c';
  
  accept();
}

void Dialog::accept_java() {
  _lang = 'j';
  accept();
}

void Dialog::accept_idl() {
  _lang = 'i';
  accept();
}

void Dialog::accept_php() {
  _lang = 'h';
  accept();
}

void Dialog::accept_python() {
  _lang = 'y';
  accept();
}

void Dialog::accept() {
  _rec = (rec_cb) ? rec_cb->isChecked() : FALSE;
  
  QDialog::accept();
}

