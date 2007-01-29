
#include "Dialog.h"

#include <stdlib.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>

#include "SmallPushButton.h"

Dialog::Dialog(QCString & path, QCString & encoding, QCString & genview, QCString & uml20, QCString & profile, QCString & pk, QCString & vis, QCString & primitivetype, QCString & genextension, QCString & linefeed, Language & lang)
  : QDialog(0, 0, TRUE), _path(path), _encoding(encoding), _genview(genview), _uml20(uml20), _profile(profile), _pk(pk), _vis(vis), _primitivetype(primitivetype), _genextension(genextension), _linefeed(linefeed), _lang(lang) {
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBox * htab;
  
  vbox->setMargin(5);
  
  // get xmi pathname
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  new QLabel("generated \nXMI file : ", htab);
  
  ed = new QLineEdit(htab);
  ed->setText(_path);
  
  new QLabel(" ", htab);
  
  SmallPushButton * br = new SmallPushButton("browse", htab);
  
  connect(br, SIGNAL(clicked ()), this, SLOT(browse()));

  // to choose encoding
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  new QLabel("Encoding : ", htab);
  
  QString charset = getenv("BOUML_CHARSET");
  int index = 0;
  
  while ((index = charset.find('_')) != -1) {
    charset[index] = '-';
    index += 1;
  }

  encoding_cb = new QComboBox(TRUE, htab);
  encoding_cb->insertItem(QString(_encoding));
  if (!charset.isEmpty() && (_encoding != (const char *) charset))
    encoding_cb->insertItem(charset);
  if (_encoding != "UTF-8")
    encoding_cb->insertItem("UTF-8");
#ifdef WIN32
  if ((_encoding != "windows-1252") && (charset != "windows-1252"))
    encoding_cb->insertItem("windows-1252");
  if ((_encoding != "ISO-8859-1") && (charset != "ISO-8859-1"))
    encoding_cb->insertItem("ISO-8859-1");
#else
  if ((_encoding != "ISO-8859-1") && (charset != "ISO-8859-1"))
    encoding_cb->insertItem("ISO-8859-1");
  if ((_encoding != "windows-1252") && (charset != "windows-1252"))
    encoding_cb->insertItem("windows-1252");
#endif
  
  // uml 2.0 or uml2.1
    
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  QButtonGroup * bg = new QButtonGroup(2, Qt::Horizontal, "Uml", htab);
  
  uml20_rb = new QRadioButton("uml 2.0", bg);
  uml21_rb = new QRadioButton("uml 2.1", bg);

  if (uml20 == "yes")
    uml20_rb->setChecked(TRUE);
  else
    uml21_rb->setChecked(TRUE);
  
  // generate view checkbox
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  genview_cb = new QCheckBox("Generate views as package", htab);
  if (_genview == "yes")
    genview_cb->setChecked(TRUE);
    
  // generate profile
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  profile_cb = new QCheckBox("Use profile (stereotype)", htab);
  if (_profile == "yes")
    profile_cb->setChecked(TRUE);
  profile_cb->setEnabled(FALSE);	// !!!!!!!!!!!!!!!!!
    
  // generate pk_ prefix
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  pk_cb = new QCheckBox("Generate 'pk_' prefix for parameter direction", htab);
  if (_pk == "yes")
    pk_cb->setChecked(TRUE);
    
  // generate vis_ prefix
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  vis_cb = new QCheckBox("Generate 'vis_' prefix for visibility", htab);
  if (_vis == "yes")
    vis_cb->setChecked(TRUE);
    
  // use PrimitiveType rather than DataType
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  primitivetype_cb = new QCheckBox("Use PrimitiveType rather than DataType", htab);
  if (_primitivetype == "yes")
    primitivetype_cb->setChecked(TRUE);
  
  // generate extension
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  genextension_cb = new QCheckBox("Generate extensions", htab);
  if (_genextension == "yes")
    genextension_cb->setChecked(TRUE);
    
  // generate &#10; rather than linefeed
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  linefeed_cb = new QCheckBox("Generate lf and cr characters in string rather than '&&#10;' and '&&#13;'", htab);
  if (_linefeed == "yes")
    linefeed_cb->setChecked(TRUE);
    
  // uml , c++, java, cancel buttons
  
  htab = new QHBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  new QLabel(htab);
  QPushButton * uml = new QPushButton("&Uml", htab);
  new QLabel(htab);
  QPushButton * cpp = new QPushButton("&C++", htab);
  new QLabel(htab);
  QPushButton * java = new QPushButton("&Java", htab);
  new QLabel(htab);
  QPushButton * cancel = new QPushButton("&Cancel", htab);
  new QLabel(htab);
  QSize bs(cancel->sizeHint());
  
  uml->setFixedSize(bs);
  cpp->setFixedSize(bs);
  java->setFixedSize(bs);
  
  connect(uml, SIGNAL(clicked()), this, SLOT(accept_uml()));
  connect(cpp, SIGNAL(clicked()), this, SLOT(accept_cpp()));
  connect(java, SIGNAL(clicked()), this, SLOT(accept_java()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void Dialog::browse() {
  QString s = QFileDialog::getSaveFileName(_path, "*.xmi", 0);
  
  if (! s.isEmpty()) {
    if (s.right(4).lower() != ".xmi")
      ed->setText(s + ".xmi");
    else
      ed->setText(s);
  }
}

void Dialog::accept_cpp() {
  _lang = Cpp;
  
  accept();
}

void Dialog::accept_uml() {
  _lang = Uml;
  accept();
}

void Dialog::accept_java() {
  _lang = Java;
  accept();
}

void Dialog::accept() {
  _path = ed->text();
  _encoding = encoding_cb->currentText();
  _genview = (genview_cb->isChecked()) ? "yes" : "no";
  _uml20 = (uml20_rb->isChecked()) ? "yes" : "no";
  _profile = (profile_cb->isChecked()) ? "yes" : "no";
  _pk = (pk_cb->isChecked()) ? "yes" : "no";
  _vis = (vis_cb->isChecked()) ? "yes" : "no";
  _primitivetype = (primitivetype_cb->isChecked()) ? "yes" : "no";
  _genextension = (genextension_cb->isChecked()) ? "yes" : "no";
  _linefeed = (linefeed_cb->isChecked()) ? "yes" : "no";
  
  QDialog::accept();
}

