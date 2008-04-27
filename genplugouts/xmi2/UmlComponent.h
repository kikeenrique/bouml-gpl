#ifndef _UMLCOMPONENT_H
#define _UMLCOMPONENT_H


#include "UmlBaseComponent.h"
#include <qcstring.h>
#include <qvector.h>

class FileOut;
class UmlClass;

// This class manages 'components'
//
// You can modify it as you want (except the constructor)
class UmlComponent : public UmlBaseComponent {
  public:
    UmlComponent(void * id, const QCString & n) : UmlBaseComponent(id, n) {};

    virtual void write(FileOut & out);

    void write(FileOut & out, const QVector< UmlClass > & v, const char * k1, const char * k2);

};

#endif
