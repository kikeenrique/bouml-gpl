#ifndef _UMLEXITPOINTPSEUDOSTATE_H
#define _UMLEXITPOINTPSEUDOSTATE_H


#include "UmlBaseExitPointPseudoState.h"
#include <qcstring.h>

class UmlExitPointPseudoState : public UmlBaseExitPointPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlExitPointPseudoState(void * id, const QByteArray & s) : UmlBaseExitPointPseudoState(id, s) {
    }

    //  call at end of import : try to solve referenced exit point
    
    virtual void solve(QByteArray idref);

};

#endif
