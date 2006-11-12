
#include <stdlib.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qmultilineedit.h>
#include <qgroupbox.h> 
#include <qtextcodec.h>

#include "UmlCom.h"

#include "TabDialog.h"
#include "UmlUseCase.h"

const struct {
  const char * lbl;
  const char * key;
  QMultiLineEdit * (TabDialog::* a);
} Tabs[] = {
  { "The summary of the use case '",
    "Summary", &TabDialog::summary },
  { "The operational context of the use case '",
    "Context", &TabDialog::context },
  { "The pre-conditions of the use case '",
    "Pre-Conditions", &TabDialog::precond },
  { "The detailed description of the use case '",
    "Description", &TabDialog::description },
  { "The post-conditions of the use case '",
    "Post-Conditions", &TabDialog::postcond },
  { "The exceptions of the use case '",
    "Exceptions", &TabDialog::exceptions }
};
  
TabDialog::TabDialog(UmlUseCase * u) : QTabDialog(0, ""), uc(u) {
  setCaption(QCString("Properties of the use case '") + u->name() + "'");

  setCancelButton();
  
  QString cs = getenv("BOUML_CHARSET");
  
  Codec = 0;
  
  if (!cs.isEmpty() && ((Codec = QTextCodec::codecForName(cs)) == 0)) {
    QVBox * vbox = new QVBox(this);
    
    vbox->setMargin(5);
    (new QLabel("ERROR : No codec for '" + cs + "'", vbox))
      ->setAlignment(AlignCenter);
    
    addTab(vbox, "Use case wizard");
    setOkButton(QString::null);
  }
  else {
    for (unsigned i = 0; i != sizeof(Tabs)/sizeof(*Tabs); i += 1) {
      QVBox * vbox = new QVBox(this);
      
      vbox->setMargin(5);
      (new QLabel(QCString(Tabs[i].lbl) + u->name() + "'",
		  new QGroupBox(1, Horizontal, vbox)))
	->setAlignment(AlignCenter);
      this->*(Tabs[i]).a = new QMultiLineEdit(vbox);
      
      QCString v;
      
      if (u->propertyValue(Tabs[i].key, v))
	(this->*(Tabs[i]).a)->setText(toUnicode(v));
      
      addTab(vbox, Tabs[i].key);
    }
  }
}

void TabDialog::accept() {
  for (unsigned i = 0; i != sizeof(Tabs)/sizeof(*Tabs); i += 1)
    uc->set_PropertyValue(Tabs[i].key, fromUnicode((this->*(Tabs[i]).a)->text()));
  UmlCom::bye();
  QTabDialog::accept();
}

void TabDialog::reject() {
  UmlCom::bye();
  QTabDialog::reject();
}

QString TabDialog::toUnicode(const char * str) {
  if (Codec == 0)
    return str;
  else if ((str == 0) || (*str == 0))
    return QString::null;
  else
    return Codec->toUnicode(str);
}

void TabDialog::latinize(QString & s) {
  unsigned i = s.length();
  
  while (i != 0) {
    QChar c = s.at(--i);
    
    if (c.latin1() == 0) {
      if (c.unicode() == 8217)
	// special case for the pseudo ' returned by microsoft editors
	c = '\'';
      else
	c = ' ';
      s.replace(i, 1, &c, 1);
    }
  }
}

QCString TabDialog::fromUnicode(const QString & s) {
  if (Codec == 0) {
    QString str = s;

    latinize(str);
    return QCString(str);
  }
  else if (s.isEmpty())
    return "";
  else
    return Codec->fromUnicode(s);
}

