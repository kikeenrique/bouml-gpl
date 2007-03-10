#ifndef _UMLITEM_H
#define _UMLITEM_H


#include <qcstring.h>
#include <qvaluelist.h>
#include "UmlBaseItem.h"
#include "anItemKind.h"
#include <qmap.h>

#include "UmlTypeSpec.h"
#include "Association.h"
class FileIn;
class Token;
class UmlTypeSpec;

typedef void (*PFunc)(FileIn &, Token &, UmlItem *);
//  'element' has an unresolved reference
class Unresolved {
  public:
    static void addGeneralization(UmlItem * e, QCString & id);

    static void addRef(UmlItem * e, QCString & id);

    //  for QValueList
    Unresolved(){};


  protected:
    Unresolved(UmlItem * e, QCString id) : element(e), idref(id) {}

    UmlItem * element;

    QCString idref;

    static QValueList<Unresolved> Generalizations;

    static QValueList<Unresolved> Refs;


  public:
    static void solveThem();

};

//  'element' has an unresolved reference
class UnresolvedWithContext {
  public:
    static void add(UmlItem * e, QCString id, int c);

    static void solveThem();

    //  for QValueList
    UnresolvedWithContext(){};


  protected:
    UnresolvedWithContext(UmlItem * e, QCString id, int c) : element(e), idref(id), context(c) {}

    UmlItem * element;

    QCString idref;

    int context;

    static QValueList<UnresolvedWithContext> All;

};

class UnresolvedRelation {
  public:
    static void add(int ctx, QCString idFrom, QCString idTo, QCString label);

    //  for QValueList
    UnresolvedRelation();


  protected:
    UnresolvedRelation(int ctx, QCString idFrom, QCString idTo, QCString label)
      : context(ctx), from(idFrom), to(idTo), name(label) {}

    int context;

    QCString from;

    QCString to;

    QCString name;

    static QValueList<UnresolvedRelation> All;


  public:
    static void solveThem();

};

// This class is a mother class for all the other Uml* classes, this
// allows to generalize their management, declare virtual operations etc ...
//
// You can modify it as you want (except the constructor)
class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QCString & n) : UmlBaseItem(id, n) {};

    virtual ~UmlItem();

    QCString id() const {
      return _xmi_id;
    }

    //Import an xmi file, only allowed in a package
    virtual void import();

    void addItem(QCString id, FileIn & in);

    //look at the token kind to call the function managing
    //this case or bypass the token
    
    virtual void import(FileIn & in, Token & tk);

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, const Token & token, FileIn & in);

    //  call at end of import : try to solve reference
    //  does nothing at this level : never called
    virtual void solve(QCString idref);

    //  call at end of import : try to solve reference
    //  does nothing at this level : never called
    virtual void solve(int context, QCString idref);

    //  call at end of import : try to solve generalization dependencies and realization
    //  not from a class
    virtual void generalizeDependRealize(UmlItem * target, FileIn & in, int context, QCString label);

    //  call at end of import : try to solve generalization dependencies and realization
    //  not from a class
    virtual void solveGeneralizationDependencyRealization(int context, QCString idref, QCString label);


  protected:
    //  try to solve type, return true is type already known
    //  else memorize unsolved couple if needed and return false
    bool setType(QCString idref, UmlTypeSpec & type);

    //  try to solve type, return true is type already known
    //  else memorize unsolved couple if needed and return false
    bool setType(QCString idref, int context, UmlTypeSpec & type);

    //  try to solve type, return true if type already known
    //  and set 'type', else return false
    static bool getType(QCString idref, UmlTypeSpec & type);


  public:
    static void declareFct(QCString what, QCString type, PFunc fct);

    static void init();

    static void importComment(FileIn & in, Token & token, UmlItem * where);

    static void importExtension(FileIn & in, Token & token, UmlItem * where);

    static void importOpaqueDef(FileIn & in, Token & token, UmlItem * where);

    static void importGeneralization(FileIn & in, Token & token, UmlItem * where);

    static void importDependency(FileIn & in, Token & token, UmlItem * where);

    static void importRealization(FileIn & in, Token & token, UmlItem * where);

    //
    
    static void incoming(FileIn & in, Token & token, UmlItem * where);

    //
    
    static void outgoing(FileIn & in, Token & token, UmlItem * where);


  protected:
    static bool FromBouml;


  public:
    static QMap<QCString, UmlItem *> All;


  protected:
    static QMap<QCString, PFunc> Functions;

    static QMap<QCString, UmlTypeSpec> PrimitiveTypes;

    static QMap<QCString, QCString> OpaqueDefs;

    static QMap<QCString,UmlItem*> Incomings;

    static QMap<QCString, UmlItem*> Outgoings;


  private:
    QCString _xmi_id;

};

#endif
