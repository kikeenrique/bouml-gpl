
#include "ConstrDestrCopyDialog.h"
#include "UmlClass.h"

#include <qlist.h>
#include <qhgroupbox.h> 
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qvbox.h> 
ConstrDestrCopyDialog::ConstrDestrCopyDialog(UmlClass * cl, bool have_constructor,
                                  bool have_destructor, bool have_copy,
                                  bool have_const_copy, bool have_assignment,
                                  bool have_const_assignment)
   : QDialog(0, 0, TRUE), target(cl) {
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHGroupBox * g;
  QList<QLabel> labels;
  QLabel * lbl;

  vbox->setSpacing(5);
  vbox->setMargin(5);
  
  lbl = new QLabel(QString("<big><b>C++ utilities for the class <i><tt>") + 
		   (const char *) cl->name() + "</tt></i></b></big>",
		   this);
  lbl->setAlignment(AlignCenter);
  vbox->addWidget(lbl);

  // constructor
  
  g = new QHGroupBox(this);
  vbox->addWidget(g);
  
  labels.append(new QLabel((have_constructor)
			   ? "the class already have contructor  "
			   : "the class doesn't have contructor  ",
			   g));
  
  add_constr = new QCheckBox("add constructor", g);
  
  // destructor
  
  if (! have_destructor) {
    g = new QHGroupBox(this);
    vbox->addWidget(g);
  
    labels.append(new QLabel("the class doesn't have destructor  ",
			     g));
    
    QHBox * h = new QHBox(g);
    
    add_destr = new QCheckBox("add destructor", h);
    virtual_destr = new QCheckBox("virtual", h);
  }
  else
    add_destr = 0;
  
  // copy contructor

  if (have_copy) {
    add_copy = 0;
    if (!have_const_copy) {
      g = new QHGroupBox(this);
      vbox->addWidget(g);
      
      labels.append(new QLabel("the class doesn't have copy contructor  \nwith const argument  ",
			       g));
      add_const_copy = new QCheckBox("add copy constructor\nwith const argument",
				     g);
    }
    else
      add_const_copy = 0;
  }
  else if (!have_const_copy) {
    g = new QHGroupBox(this);
    vbox->addWidget(g);
    
    labels.append(new QLabel("the class doesn't have copy contructor  ",
			     g));
    
    QVBox * v = new QVBox(g);
    
    add_const_copy = new QCheckBox("add copy constructor\nwith const argument",
				   v);      
    add_copy = new QCheckBox("add copy constructor\nwith non const argument",
			     v);
  }
  else {
    g = new QHGroupBox(this);
    vbox->addWidget(g);
    
    labels.append(new QLabel("the class doesn't have copy contructor  \nwith non const argument  ",
			     g));
    add_copy = new QCheckBox("add copy constructor\nwith non const argument",
			     g);
    add_const_copy = 0;
  }

  // assignment

  if (have_assignment) {
    add_assign = 0;
    if (!have_const_assignment) {
      g = new QHGroupBox(this);
      vbox->addWidget(g);
    
      labels.append(new QLabel("the class doesn't have assignment\noperator with const argument  ",
			       g));
      add_const_assign = new QCheckBox("add assignment\nwith const argument",
				       g);
    }
    else
      add_const_assign = 0;
  }
  else if (!have_const_assignment) {
    g = new QHGroupBox(this);
    vbox->addWidget(g);
    
    labels.append(new QLabel("the class doesn't have assignment operator  ", g));
    
    QVBox * v = new QVBox(g);
    
    add_const_assign = new QCheckBox("add assignment\nwith const argument",
				     v);      
    add_assign = new QCheckBox("add assignment\nwith non const argument",
			       v);
  }
  else {
    g = new QHGroupBox(this);
    vbox->addWidget(g);
    
    labels.append(new QLabel("the class doesn't have assignment operator  \nwith non const argument  ",
			     g));
    add_assign = new QCheckBox("add assignment\nwith non const argument",
			       g);
    add_const_assign = 0;
  }

  // use the same width for all the labels on the first column

  QSize sz(labels.first()->sizeHint());
  
  while ((lbl = labels.next()) != 0) {
    if (lbl->sizeHint().width() > sz.width())
      sz.setWidth(lbl->sizeHint().width());
  }

  for (lbl = labels.first(); lbl != 0; lbl = labels.next()) {
    sz.setHeight(lbl->sizeHint().height());
    lbl->setFixedSize(sz);
  }
    
  // ok & cancel buttons
  
  QHBox * hbox = new QHBox(this);
  
  vbox->addWidget(hbox);
  
  QPushButton * ok = new QPushButton("&OK", hbox);
  QPushButton * cancel = new QPushButton("&Cancel", hbox);

  QSize bs(cancel->sizeHint());
  
  ok->setDefault(TRUE);
  
  ok->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void ConstrDestrCopyDialog::accept() {
  if (add_constr->isChecked())
    target->addContructor();
    
  if ((add_destr != 0) && add_destr->isChecked())
    target->addDestructor(virtual_destr->isChecked());

  if ((add_copy != 0) && add_copy->isChecked())
    target->addCopy(FALSE);

  if ((add_const_copy != 0) && add_const_copy->isChecked())
    target->addCopy(TRUE);

    if ((add_assign != 0) && add_assign->isChecked())
    target->addAssign(FALSE);

  if ((add_const_assign != 0) && add_const_assign->isChecked())
    target->addAssign(TRUE);
    
  QDialog::accept();
}

