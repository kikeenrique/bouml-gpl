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
    Dialog(QByteArray & path, QByteArray & encoding, QByteArray & nomodel, QByteArray & genview, QByteArray & uml20, QByteArray & pk, QByteArray & vis, QByteArray & primitivetype, QByteArray & genextension, QByteArray & geneclipse, QByteArray & commentexporter, QByteArray & linefeed, Language & lang);


  protected:
    QByteArray & _path;

    QByteArray & _encoding;

    QByteArray & _nomodel;

    QByteArray & _genview;

    //yes => generate pk_ prefix
    QByteArray & _uml20;

    //yes => generate pk_ prefix
    QByteArray & _pk;

    //yes => generate vis_ prefix
    QByteArray & _vis;

    //yes => use primitiveType rather than dataType for int ...
    QByteArray & _primitivetype;

    QByteArray & _genextension;

    QByteArray & _geneclipse;

    QByteArray & _commentexporter;

    //yes => generate linefeed inside string, else &#10;
    QByteArray & _linefeed;

    Language & _lang;

    QLineEdit * ed;

    QRadioButton * uml20_rb;

    QRadioButton * uml21_rb;

    QComboBox * encoding_cb;

    QCheckBox * nomodel_cb;

    QCheckBox * genview_cb;

    QCheckBox * pk_cb;

    QCheckBox * vis_cb;

    QCheckBox * primitivetype_cb;

    QCheckBox * genextension_cb;

    QCheckBox * geneclipse_cb;

    QCheckBox * commentexporter_cb;

    QCheckBox * linefeed_cb;

  protected slots:
    virtual void polish();

    void browse();

    void accept_cpp();

    void accept_uml();

    void accept_java();

    void accept();

};

#endif
