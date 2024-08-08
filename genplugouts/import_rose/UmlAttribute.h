#ifndef _UMLATTRIBUTE_H
#define _UMLATTRIBUTE_H


#include "UmlBaseAttribute.h"
#include <qcstring.h>
#include <qdict.h>

class File;
class UmlClass;

// This class manages 'attribute', notes that the class 'UmlClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)
class UmlAttribute : public UmlBaseAttribute {
  public:
    UmlAttribute(void * id, const QByteArray & n) :  UmlBaseAttribute(id, n) {
      cpt[kind()] += 1;
    };


  private:
    void import(File & f);


  public:
    static void import(File & f, UmlClass * parent);

    static void importIdlConstant(UmlClass * parent, const QByteArray & id, const QByteArray & s, const QByteArray & doc, QDict<QByteArray> & prop);


  private:
    void cplusplus(QDict<QByteArray> & prop);

    void oracle8(QDict<QByteArray> & prop);

    void corba(QDict<QByteArray> & prop);

    void java(QDict<QByteArray> & prop);

};

#endif
