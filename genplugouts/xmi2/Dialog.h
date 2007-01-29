#ifndef _DIALOG_H
#define _DIALOG_H


#include <qcstring.h>
#include "Language.h"

#include <qdialog.h>

class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QRadioButton;

class Dialog : public QDialog {
  Q_OBJECT
  public:
    Dialog(QCString & path, QCString & encoding, QCString & genview, QCString & uml20, QCString & profile, QCString & pk, QCString & vis, QCString & primitivetype, QCString & genextension, QCString & linefeed, Language & lang);


  protected:
    QCString & _path;

    QCString & _encoding;

    QCString & _genview;

    //yes => generate pk_ prefix
    QCString & _uml20;

    //yes => generate profile for stereotype
    QCString & _profile;

    //yes => generate pk_ prefix
    QCString & _pk;

    //yes => generate vis_ prefix
    QCString & _vis;

    //yes => use primitiveType rather than dataType for int ...
    QCString & _primitivetype;

    QCString & _genextension;

    //yes => generate linefeed inside string, else &#10;
    QCString & _linefeed;

    Language & _lang;

    QLineEdit * ed;

    QRadioButton * uml20_rb;

    QRadioButton * uml21_rb;

    QComboBox * encoding_cb;

    QCheckBox * genview_cb;

    QCheckBox * profile_cb;

    QCheckBox * pk_cb;

    QCheckBox * vis_cb;

    QCheckBox * primitivetype_cb;

    QCheckBox * genextension_cb;

    QCheckBox * linefeed_cb;

  protected slots:
    void browse();

    void accept_cpp();

    void accept_uml();

    void accept_java();

    void accept();

};

#endif
