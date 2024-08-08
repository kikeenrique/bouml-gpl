#ifndef _MANIFESTATION_H
#define _MANIFESTATION_H


#include <qcstring.h>
#include <qvaluelist.h>

class UmlArtifact;
class FileIn;
class Token;

class Manifestation {
  public:
    //for QValueList
    Manifestation(){}

    Manifestation(QByteArray s, UmlArtifact * art, QByteArray client) : name(s), artifact(art), utilized(client) {}

    static void import(FileIn & in, Token & token, UmlArtifact * artifact);

    static void solveThem();


  protected:
    QByteArray name;

    UmlArtifact * artifact;

    QByteArray utilized;

    static QValueList<Manifestation> All;

};

#endif
