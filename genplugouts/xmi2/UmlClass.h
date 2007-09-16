#ifndef _UMLCLASS_H
#define _UMLCLASS_H


#include "UmlBaseClass.h"
#include <qcstring.h>

class FileOut;
class UmlRelation;

// This class manages 'classes', notes that the class 'ClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)

class UmlClass : public UmlBaseClass {
  public:
    UmlClass(void * id, const QCString & n) : UmlBaseClass(id, n), _assoc(0) {};

    virtual void write(FileOut & out);


  private:
    void write_formals(FileOut & out);


  public:
    void write_actuals(FileOut & out);

    virtual void search_class_assoc();

    //return 0 in case the class is already an association class 
    UmlClass * set_assoc(UmlRelation * rel);


  protected:
    //relation when the class is an association class
    UmlRelation * _assoc;

};

#endif
