#ifndef _UMLCOMPONENT_H
#define _UMLCOMPONENT_H


#include "UmlBaseComponent.h"
#include <qcstring.h>

class FileIn;
class Token;
class UmlItem;

// This class manages 'components'
//
// You can modify it as you want (except the constructor)
class UmlComponent : public UmlBaseComponent {
  public:
    UmlComponent(void * id, const QCString & n) : UmlBaseComponent(id, n) { NumberOf += 1; };

    static void init();

    //import the component starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    void manageInterface(Token & token, FileIn & in);

    //  call at end of import : try to solve interfaces
    virtual void solve(int context, QCString idref);

    static int numberOf() { return NumberOf; };


  protected:
    static int NumberOf;

};

#endif
