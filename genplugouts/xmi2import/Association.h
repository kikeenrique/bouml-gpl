#ifndef _ASSOCIATION_H
#define _ASSOCIATION_H


#include <qcstring.h>
#include "aRelationKind.h"
#include <qmap.h>
#include <qvaluelist.h>

class FileIn;
class Token;
class UmlItem;

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
    void import(FileIn & in, Token & token);

    // search for the association from its id
    
    static Association & get(QCString id, QCString s = "");

    static void solveThem();


  protected:
    QCString name;

    Role roles[2];


  private:
    void solve(QCString id);


  protected:
    static QMap<QCString, Association> All;

};

class AssociationClass {
  public:
    AssociationClass(QCString r1, QCString r2) : ref1(r1), ref2(r2) {}

    //  for QValueList
    AssociationClass();

    static void init();

    //import the association class starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    static void solveThem();


  private:
    void solve();


  protected:
    QCString ref1;

    QCString ref2;

    static QValueList<AssociationClass> All;

};

#endif
