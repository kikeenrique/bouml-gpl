#ifndef _UMLREGION_H
#define _UMLREGION_H


#include "UmlBaseRegion.h"
#include "UmlStateItem.h"
#include <qcstring.h>
#include <qlist.h>

class UmlTransition;
class FileOut;

class UmlRegion : public UmlBaseRegion, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRegion(void * id, const QCString & s) : UmlBaseRegion(id, s) {
    }

    //goes among tree to memorize incoming transitions
    
    virtual void memo_incoming_trans();

    //memorize trans starting in sub elements to produce them later
    
    virtual void memo_trans(UmlTransition * tr);

    virtual void write(FileOut & out);


  protected:
    QList<UmlTransition> _trans;

};

#endif
