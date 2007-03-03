#ifndef _UMLCLASS_H
#define _UMLCLASS_H


#include "UmlBaseClass.h"
#include <qcstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

// This class manages 'classes', notes that the class 'ClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)
class UmlClass : public UmlBaseClass {
  public:
    UmlClass(void * id, const QCString & n) : UmlBaseClass(id, n) { NumberOf += 1; };

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, const Token & token, FileIn & in);

    static void init();

    //import the class starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);


  private:
    void readFormal(FileIn & in, Token & token);


  public:
    //import the primitive type starting by 'tk' inside 'where'
    static void importPrimitiveType(FileIn & in, Token & token, UmlItem * where);

    //  call at end of import : try to solve generalization dependencies and realization
    
    virtual void generalizeDependRealize(UmlItem * target, FileIn & in, int context, QCString label);

    //  call at end of import : try to solve generalization dependencies or realization
    
    virtual void solveGeneralizationDependencyRealization(int context, QCString idref, QCString label);

    static int numberOf() { return NumberOf; };


  protected:
    static int NumberOf;

};

#endif
