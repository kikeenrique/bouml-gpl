#ifndef _UMLOPERATION_H
#define _UMLOPERATION_H


#include "UmlBaseOperation.h"
#include <qcstring.h>
#include <qdict.h>

class File;
class UmlClass;

// This class manages 'operations', notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlOperation : public UmlBaseOperation {
  public:
    UmlOperation(void * id, const QByteArray & n) : UmlBaseOperation(id, n) {
      cpt[kind()] += 1;
    };

    static void import(File & f, UmlClass * parent);


  private:
    void import(File & f);

    void importParameters(File & f);

    void importExceptions(File & f);

    void cplusplus(QDict<QByteArray> & prop);

    void oracle8(QDict<QByteArray> & prop);

    void corba(QDict<QByteArray> & prop);

    void java(QDict<QByteArray> & prop);

};

#endif
