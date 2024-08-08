#ifndef _FLOWBEHAVIOR_H
#define _FLOWBEHAVIOR_H


#include <qcstring.h>

class FlowBehavior {
  public:
    QByteArray weight;

    QByteArray guard;

    QByteArray selection;

    QByteArray transformation;

    void read();

    void unload();

};

#endif
