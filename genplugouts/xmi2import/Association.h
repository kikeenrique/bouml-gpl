#ifndef _ASSOCIATION_H
#define _ASSOCIATION_H


#include <qcstring.h>
#include "aRelationKind.h"
#include <qmap.h>

class FileIn;
class Token;

// an unused role has an empty id
struct Role {
    QCString name;

    QCString id;

    // idref of the type containing it
    QCString idref;

    aRelationKind aggregate;

    bool navigable;

    bool readOnly;

    bool isStatic;

    QCString multiplicity;

    QCString visibility;

    Role() : aggregate(anAssociation), navigable(TRUE), readOnly(FALSE), isStatic(FALSE) {}

    void setMultiplicity(QCString v, bool upper);

};

class Association {
  public:
    Association() : is_class_association(FALSE) {}

    void import(FileIn & in, Token & token);

    void set_class_association(){ is_class_association = TRUE; }
    // search for the association from its id
    
    static Association & get(QCString id, QCString s = "");

    static void solveThem();


  protected:
    QCString name;

    Role roles[2];

    static QMap<QCString, Association> All;

    bool is_class_association;


  private:
    void solve(QCString id);

};

#endif
