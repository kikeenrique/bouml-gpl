#ifndef _UMLPACKAGE_H
#define _UMLPACKAGE_H


#include "UmlBasePackage.h"
#include <qcstring.h>
#include <qlist.h>

class FileOut;
class UmlItem;

// This class manages a 'package', notes that the project itself is a package
//
// You can modify it as you want (except the constructor)
class UmlPackage : public UmlBasePackage {
  public:
    UmlPackage(void * id, const QCString & n) : UmlBasePackage(id, n) {};

    virtual void xmi(int argc, char ** argv);

    virtual void write(FileOut & out);


  private:
    void search_profiles(QList<UmlPackage> & l);


  public:
    virtual void search_class_assoc();

    //Memorize the relation in the package (or the view in case the views
    //are generated as package) containing the current element, to generate
    //the relation at the package/view level
    
    virtual void memo_relation(UmlItem * r);


  protected:
    QList<UmlItem> _relations;

};

#endif
