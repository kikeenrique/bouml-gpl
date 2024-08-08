#ifndef _UMLACCESSVARIABLEVALUEACTION_H
#define _UMLACCESSVARIABLEVALUEACTION_H


#include "UmlBaseAccessVariableValueAction.h"
#include <qcstring.h>

class FileIn;
class Token;

class UmlAccessVariableValueAction : public UmlBaseAccessVariableValueAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAccessVariableValueAction(void * id, const QByteArray & s) : UmlBaseAccessVariableValueAction(id, s) {
    }

    virtual void import_it(FileIn & in, Token & token);

    void setVariable(QByteArray idref);


  public:
    //  call at end of import : try to solve var
    virtual void solve(QByteArray idref);

};

#endif
