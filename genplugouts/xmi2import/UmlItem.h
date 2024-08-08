#ifndef _UMLITEM_H
#define _UMLITEM_H


#include <qcstring.h>
#include <qvaluelist.h>
#include "UmlBaseItem.h"
#include <qstring.h>
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
    static void addGeneralization(UmlItem * e, QByteArray & id, QByteArray cstr);

    static void addRef(UmlItem * e, QByteArray & id);

    //  for QValueList
    Unresolved(){};


  protected:
    Unresolved(UmlItem * e, QByteArray id, QByteArray cstr) : element(e), idref(id), constraint(cstr) {}

    UmlItem * element;

    QByteArray idref;

    QByteArray constraint;

    static QValueList<Unresolved> Generalizations;

    static QValueList<Unresolved> Refs;


  public:
    static void solveThem();

};

//  'element' has an unresolved reference
class UnresolvedWithContext {
  public:
    static void add(UmlItem * e, QByteArray id, int c);

    static void solveThem();

    //  for QValueList
    UnresolvedWithContext(){};


  protected:
    UnresolvedWithContext(UmlItem * e, QByteArray id, int c) : element(e), idref(id), context(c) {}

    UmlItem * element;

    QByteArray idref;

    int context;

    static QValueList<UnresolvedWithContext> All;

};

class UnresolvedRelation {
  public:
    static void add(int ctx, QByteArray idFrom, QByteArray idTo, QByteArray label, QByteArray constraint);

    //  for QValueList
    UnresolvedRelation();


  protected:
    UnresolvedRelation(int ctx, QByteArray idFrom, QByteArray idTo, QByteArray label, QByteArray cnst)
      : context(ctx), from(idFrom), to(idTo), name(label), constraint(cnst) {}

    int context;

    QByteArray from;

    QByteArray to;

    QByteArray name;

    QByteArray constraint;

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
    UmlItem(void * id, const QByteArray & n) : UmlBaseItem(id, n) {};

    virtual ~UmlItem();

    QByteArray id() const {
      return _xmi_id;
    }

    //Import an xmi file, only allowed in a package
    virtual void import(QString path);

    void addItem(QByteArray id, FileIn & in);

    //look at the token kind to call the function managing
    //this case or bypass the token
    
    virtual void import(FileIn & in, Token & tk);

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

    //  call at end of import : try to solve reference
    //  does nothing at this level : never called
    virtual void solve(QByteArray idref);

    //  call at end of import : try to solve reference
    //  does nothing at this level : never called
    virtual void solve(int context, QByteArray idref);

    //  call at end of import : try to solve generalization dependencies and realization
    //  not from a class
    virtual void generalizeDependRealize(UmlItem * target, FileIn & in, int context, QByteArray label, QByteArray constraint);

    //  call at end of import : try to solve generalization dependencies and realization,
    //  not from a class
    virtual void solveGeneralizationDependencyRealization(int context, QByteArray idref, QByteArray label, QByteArray constraint);


  protected:
    //  try to solve type, return true is type already known
    //  else memorize unsolved couple if needed and return false
    bool setType(QByteArray idref, UmlTypeSpec & type);

    //  try to solve type, return true is type already known
    //  else memorize unsolved couple if needed and return false
    bool setType(QByteArray idref, int context, UmlTypeSpec & type);

    //  try to solve token being <type ..>, return true is type already known
    //  else memorize unsolved couple if needed and return false
    bool setType(Token & token, UmlTypeSpec & type);

    //  try to solve type, return true is type already known
    //  else memorize unsolved couple if needed and return false
    bool setType(Token & token, int context, UmlTypeSpec & type);


  public:
    //  element defined in a profile, if xmiId property defined update All
    
    void loadFromProfile();


  protected:
    //  try to solve type, return true if type already known
    //  and set 'type', else return false
    static bool getType(QByteArray idref, UmlTypeSpec & type);


  public:
    static void declareFct(QByteArray what, QByteArray type, PFunc fct);

    static PFunc getFct(const Token & tk);

    static QByteArray readComment(FileIn & in, Token & token);

    static void init();

    static void importDocumentation(FileIn & in, Token & token, UmlItem * where);

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

    static QByteArray legalName(QByteArray s);

    static bool fromEclipse();

    static QByteArray readConstraint(FileIn & in, Token & token);

    static QMap<QByteArray, QByteArray> OpaqueDefs;


  protected:
    static bool FromBouml;

    static bool FromEclipse;


  public:
    static QMap<QByteArray, UmlItem *> All;


  protected:
    static QMap<QByteArray, PFunc> Functions;

    static QMap<QByteArray, UmlTypeSpec> PrimitiveTypes;

    static QMap<QByteArray,UmlItem*> Incomings;

    static QMap<QByteArray, UmlItem*> Outgoings;


  private:
    QByteArray _xmi_id;

};

#endif
