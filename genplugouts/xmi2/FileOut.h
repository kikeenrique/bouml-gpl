#ifndef _FILEOUT_H
#define _FILEOUT_H


#include <qtextstream.h>
#include <qmap.h>
#include <qcstring.h>

class QFile;
class UmlItem;

//QTextOStream + usefull operations
class FileOut : public QTextStream {
  public:
    FileOut(QFile * fp, bool lf);

    void indent();

    void indent(int dir) { _indent += dir; }

    void id(const UmlItem * x);

    void id_prefix(const UmlItem * x, const char * pfix);

    void idref(const UmlItem * x);

    //for C++/Java types
    void idref(QCString s, const UmlItem * x);

    void idref_prefix(const UmlItem * x, const char * pfix);

    void idref_datatype(const QCString & t);

    //output <space><pfix1>="<pfix2>BOUML_<x>"
    void ref(const UmlItem * x, const char * pfix1, const char * pfix2 = "");

    void define_datatypes(bool uml_20, bool primitive_type, bool gen_extension);

    void quote(const char * s);


  protected:
    bool _lf;

    int _indent;

    QMap<QCString,int> _datatypes;

    QMap<QCString,int> _modifiedtypes;

};

#endif
