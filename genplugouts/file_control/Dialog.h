#ifndef _DIALOG_H
#define _DIALOG_H


#include <qcstring.h>

#include <qdialog.h>
class QLineEdit;
class QCheckBox;
//A dialog to set the check-in/out command and set the recursive flag

class Dialog : public QDialog {
  public:
    Dialog(bool ci, QCString & cmd, bool & rec, bool & reload);

    virtual void accept();


  protected:
    QCString & _cmd;

    bool & _rec;

    bool & _reload;

    QCheckBox * rec_cb;

    QLineEdit * cmd_ed;

    QCheckBox * reload_cb;

};

#endif
