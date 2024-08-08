#ifndef _UMLENTRYPOINTPSEUDOSTATE_H
#define _UMLENTRYPOINTPSEUDOSTATE_H


#include "UmlBaseEntryPointPseudoState.h"
#include <qcstring.h>

class UmlEntryPointPseudoState : public UmlBaseEntryPointPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlEntryPointPseudoState(void * id, const QByteArray & s) : UmlBaseEntryPointPseudoState(id, s) {
    }

    //  call at end of import : try to solve referenced entry point
    
    virtual void solve(QByteArray idref);

};

#endif
