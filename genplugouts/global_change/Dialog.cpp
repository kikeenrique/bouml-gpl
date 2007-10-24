
#include "Dialog.h"

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qhbuttongroup.h>
#include <qvgroupbox.h> 
#include <qgrid.h>
#include <qmessagebox.h>

#include "UmlCom.h"
#include "Context.h"
#include "UmlItem.h"

Dialog::Dialog() : QDialog(0, 0, TRUE) {
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QVGroupBox * gbox;
  QHButtonGroup * bg;
  QHBox * htab;
  
  vbox->setMargin(5);
  
  gbox = new QVGroupBox("Filters" , this);
  vbox->addWidget(gbox);
  
  htab = new QHBox(gbox);
  //htab->setMargin(5);
  
  new QLabel("filter 1 : ", htab);
  filter1_le = new QLineEdit(htab);
  
  new QLabel("  ", htab);
  
  bg = new QHButtonGroup(htab);
  bg->setRadioButtonExclusive(TRUE);
  
  with1_rb = new QRadioButton("with", bg);
  new QRadioButton("without", bg);
  with1_rb->setChecked(TRUE);
  
  //
  
  htab = new QHBox(gbox);
  //htab->setMargin(5);
  
  new QLabel("", htab);
  bg = new QHButtonGroup(htab);
  bg->setRadioButtonExclusive(TRUE);
  
  and12_rb = new QRadioButton("and", bg);
  new QRadioButton("or", bg);
  and12_rb->setChecked(TRUE);
  
  new QLabel("", htab);
  
  //
  
  htab = new QHBox(gbox);
  //htab->setMargin(5);
  
  new QLabel("filter 2 : ", htab);
  filter2_le = new QLineEdit(htab);
  
  new QLabel("  ", htab);
  
  bg = new QHButtonGroup(htab);
  bg->setRadioButtonExclusive(TRUE);
  
  with2_rb = new QRadioButton("with", bg);
  new QRadioButton("without", bg);
  with2_rb->setChecked(TRUE);
  
  //
  
  htab = new QHBox(gbox);
  //htab->setMargin(5);
  
  new QLabel("", htab);
  bg = new QHButtonGroup(htab);
  bg->setRadioButtonExclusive(TRUE);
  
  and23_rb = new QRadioButton("and", bg);
  new QRadioButton("or", bg);
  and23_rb->setChecked(TRUE);
  
  new QLabel("", htab);
  
  //
  
  htab = new QHBox(gbox);
  //htab->setMargin(5);
  
  new QLabel("filter 3 : ", htab);
  filter3_le = new QLineEdit(htab);
  
  new QLabel("  ", htab);
  
  bg = new QHButtonGroup(htab);
  bg->setRadioButtonExclusive(TRUE);
  
  with3_rb = new QRadioButton("with", bg);
  new QRadioButton("without", bg);
  with3_rb->setChecked(TRUE);
  
  //
  //
  
  gbox = new QVGroupBox("Stereotype" , this);
  vbox->addWidget(gbox);
  
  htab = new QHBox(gbox);
  //htab->setMargin(5);
  
  bg = new QHButtonGroup(htab);
  bg->setRadioButtonExclusive(TRUE);
  
  any_rb = new QRadioButton("any", bg);
  is_rb = new QRadioButton("is", bg);
  isnot_rb = new QRadioButton("is not", bg);
  any_rb->setChecked(TRUE);
  stereotype_le = new QLineEdit(htab);
  
  //
  //
  
  gbox = new QVGroupBox("Targets" , this);
  vbox->addWidget(gbox);
  
  htab = new QHBox(gbox);
  //htab->setMargin(5);
  
  artifact_cb = new QCheckBox("artifact", htab);
  class_cb = new QCheckBox("class", htab);
  operation_cb = new QCheckBox("operation", htab);
  attribute_cb = new QCheckBox("attribute", htab);
  relation_cb = new QCheckBox("relation", htab);
  
  //
  //
  
  gbox = new QVGroupBox("Languages" , this);
  vbox->addWidget(gbox);
  
  htab = new QHBox(gbox);
  //htab->setMargin(5);
  
  cpp_cb = new QCheckBox("C++", htab);
  java_cb = new QCheckBox("Java", htab);
  php_cb = new QCheckBox("Php", htab);
  idl_cb = new QCheckBox("Idl", htab);
  
  //
  //
  
  QGrid * grid = new QGrid(2, this);
  vbox->addWidget(grid);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel("current : ", grid);
  current_le  = new QLineEdit(grid);
  
  new QLabel("new : ", grid);
  new_le  = new QLineEdit(grid);
  
  //
  //
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  new QLabel(htab);
  QPushButton * replace = new QPushButton("Replace", htab);
  new QLabel(htab);
  QPushButton * quit = new QPushButton("Quit", htab);
  new QLabel(htab);
  
  QSize bs(replace->sizeHint());
  
  quit->setFixedSize(bs);
  
  connect(replace, SIGNAL(clicked()), this, SLOT(do_replace()));
  connect(quit, SIGNAL(clicked()), this, SLOT(reject()));
  
}

void Dialog::do_replace() {
  Context ctx(digest(current_le->text()), digest(new_le->text()));
  
  const char * err = 
    ctx.set_filters(digest(filter1_le->text()), digest(filter2_le->text()), digest(filter3_le->text()),
  		  with1_rb->isChecked(), with2_rb->isChecked(), with3_rb->isChecked(),
  		  and12_rb->isChecked(), and23_rb->isChecked());
  
  if (err != 0)
    QMessageBox::critical(this, "Global change", err);
  else {
    ctx.set_stereotype(QCString(stereotype_le->text()), is_rb->isChecked(), isnot_rb->isChecked());
    ctx.set_targets(artifact_cb->isChecked(), class_cb->isChecked(),
  		  operation_cb->isChecked(), attribute_cb->isChecked(), relation_cb->isChecked());
    ctx.set_language(cpp_cb->isChecked(), java_cb->isChecked(), php_cb->isChecked(), idl_cb->isChecked());
    
    UmlCom::targetItem()->change(ctx);
    
    QString msg;
    int n_match = ctx.n_match();
    int n_err = ctx.n_err();
    
    if (n_match == 0)
      msg = "Not found !";
    else if (n_err == 0)
      msg.sprintf("%d replacements done", n_match);
    else
      msg.sprintf("%d replacements done,\n%d not done on read-only elements",
  	      n_match - n_err, n_err);
    
    QMessageBox::information(this, "Global change", msg);
  }
}

QCString Dialog::digest(const QString s) {
  QCString c = (const char *) s;
  int index = 0;
  
  while ((index = c.find("\\n", index)) != -1) {
    c.replace(index, 2, "\n");
    index += 1;
  }
  
  while ((index = c.find("\r", index)) != -1) {
    c.remove(index, 1);
  }
  
  return c;
}

void Dialog::polish() {
  QSize sz = size();
  
  // width = height
  resize(sz.height(), sz.height());
}

