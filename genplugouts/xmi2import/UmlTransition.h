#ifndef _UMLTRANSITION_H
#define _UMLTRANSITION_H


#include "UmlBaseTransition.h"
#include "UmlStateItem.h"
#include <qcstring.h>
#include <qvaluelist.h>

class FileIn;
class Token;
class UmlItem;

class UmlTransition : public UmlBaseTransition, public UmlStateItem {
  public:
     UmlTransition(void * id, const QByteArray & n) : UmlBaseTransition(id, n) {
    }

    static void init();

    static void solveThem();

    //import the transition starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    struct Transition {
        QByteArray id;

        QByteArray name;

        QByteArray source;

        QByteArray target;

        QByteArray guard;

        QByteArray effect;

        QByteArray trigger;

        QByteArray triggerRef;

        QByteArray kind;

    };
    
    

  protected:
    static QValueList<Transition> All;

};

#endif
