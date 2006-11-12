#ifndef _UMLENTRYPOINTPSEUDOSTATE_H
#define _UMLENTRYPOINTPSEUDOSTATE_H


#include <qcstring.h>
#include "UmlBaseEntryPointPseudoState.h"

class UmlEntryPointPseudoState : public UmlBaseEntryPointPseudoState {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlEntryPointPseudoState(void * id, const QCString & s) : UmlBaseEntryPointPseudoState(id, s) {
    }

};

#endif
