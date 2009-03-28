#ifndef _DIALOG_H
#define _DIALOG_H


#include <qdialog.h>
class QCheckBox;
class Dialog : public QDialog {
  Q_OBJECT
  public:
    Dialog(bool & rec, char & lang);


  protected:
    bool & _rec;

    //c = c++
    //j = java
    //h = php
    //y = python
    //i = idl
    char & _lang;

    QCheckBox * rec_cb;

  protected slots:    void accept_cpp();
    void accept_java();
    void accept_idl();
    void accept_php();
    void accept_python();
    void accept();
};

#endif
