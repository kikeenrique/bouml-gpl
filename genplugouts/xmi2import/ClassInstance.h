#ifndef _CLASSINSTANCE_H
#define _CLASSINSTANCE_H


#include <qlist.h>
#include <qcstring.h>
#include <qvaluelist.h>

class FileIn;
class Token;
class UmlItem;

class ClassInstance {
  public:
    static void init();

    //import the activity node starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    static void solveThem();


  protected:
    static QList<ClassInstance> All;

    struct Slot {
        //import the activity node starting by 'tk' inside 'where'
        void importIt(FileIn & in, Token & token);

        QByteArray featureId;

        QByteArray value;

        QByteArray valueId;

    };
    
    
    QValueList<Slot> bindings;

    QByteArray id;

    QByteArray name;

    QByteArray classifierId;

    UmlItem * where;

};

#endif
