#ifndef _UMLENTRYPOINTPSEUDOSTATE_H
#define _UMLENTRYPOINTPSEUDOSTATE_H


#include "UmlBaseEntryPointPseudoState.h"
#include <qcstring.h>

class FileOut;

class UmlEntryPointPseudoState : public UmlBaseEntryPointPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlEntryPointPseudoState(void * id, const QByteArray & s) : UmlBaseEntryPointPseudoState(id, s) {
    }

    virtual void write(FileOut & out);

    virtual const char * sKind() const;

};

#endif
