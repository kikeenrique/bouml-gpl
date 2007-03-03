#ifndef _UMLONSIGNALACTION_H
#define _UMLONSIGNALACTION_H


#include "UmlBaseOnSignalAction.h"
#include <qcstring.h>

class FileIn;
class Token;

class UmlOnSignalAction : public UmlBaseOnSignalAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlOnSignalAction(void * id, const QCString & s) : UmlBaseOnSignalAction(id, s) {
    }


  public:
    //look at the token kind to call the function managing
    //this case or bypass the token
    
    virtual void import(FileIn & in, Token & tk);

    //  call at end of import : try to solve signal
    
    virtual void solve(QCString idref);


  protected:
    //import the activity signal action starting by 'tk'
    
    virtual void import_it(FileIn & in, Token & token);

};

#endif
