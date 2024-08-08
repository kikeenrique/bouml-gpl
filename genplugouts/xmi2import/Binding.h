#ifndef _BINDING_H
#define _BINDING_H


#include <qlist.h>
#include <qcstring.h>

class FileIn;
class Token;
class UmlClass;

//Manages template actuals

class Binding {
  public:
    static void import(FileIn & in, Token & token, UmlClass * where);

    static void solveThem();


  protected:
    static QList<Binding> All;

    //xmiId of the class having the binbing
    QByteArray boundId;

    //xmiId of the formal
    QByteArray formalId;

    //xmiId of the actual, may be a class, or an opaque expression
    QByteArray actualId;

    //the actual if actualId is empty
    QByteArray actual;

    //xmiId of the signature
    
    QByteArray signatureId;

};

#endif
