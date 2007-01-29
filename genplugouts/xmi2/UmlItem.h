#ifndef _UMLITEM_H
#define _UMLITEM_H


#include <qmap.h>
#include "UmlBaseItem.h"
#include <qcstring.h>
#include "Language.h"
#include <qlist.h>

class FileOut;
class FileOut;
class UmlTypeSpec;

// This class is a mother class for all the other Uml* classes, this
// allows to generalize their management, declare virtual operations etc ...
//
// You can modify it as you want (except the constructor)
class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QCString & n)
      : UmlBaseItem(id, n), _written(FALSE) {};

    virtual ~UmlItem();

    virtual void xmi(int argc, char ** argv);

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

    //inside is true when the generation is done inside the parent definition
    //does nothing at this level
    virtual void write(FileOut & out, bool inside);

    void write_description_properties(FileOut & out);

    //Memorize the relation in the package (or the view in case the views
    //are generated as package) containing the current element, to generate
    //the relation at the package/view level
    
    virtual void memo_relation(UmlItem * r);

    static void write_multiplicity(FileOut & out, QCString s);

    //if the type is specified write it in a form <type ... />
    static void write_type(FileOut & out, const UmlTypeSpec & t);

    static void write_default_value(FileOut & out, QCString v);


  protected:
    bool _written;

    static bool _gen_views;

    static Language _lang;

    static bool _uml_20;

    static bool _use_profile;

    static bool _pk_prefix;

    static bool _vis_prefix;

    static bool _primitive_type;

    static bool _linefeed;

    static bool _gen_extension;

    QMap<QCString, QList<UmlItem> > _stereotypes;

};

#endif
