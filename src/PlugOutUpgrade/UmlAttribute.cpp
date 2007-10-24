#include "UmlAttribute.h"

#include "UmlClass.h"
#include "UmlCom.h"
#include "util.h"


UmlAttribute * UmlAttribute::java2Php(UmlClass * php, UmlClass * java,
				      const char * javaname,
				      const char * phpname)
{
  if (phpname == 0)
    phpname = javaname;
  
  UmlAttribute * from = java->get_attribute(javaname);
  
  if (from == 0) {
    QCString err = QCString("cannot find attribute '") + 
      javaname + QCString("' in class '") + java->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlAttribute * to = UmlBaseAttribute::create(php, phpname);
  
  if (to == 0) {
    QCString err = QCString("cannot create attribute '") + 
      phpname + QCString("' in class '") + php->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlCom::trace("add attribute " + php->name() + "::" + phpname + "<br>\n");

  to->set_Type(from->type());
  to->set_isClassMember(from->isClassMember());
  to->set_Visibility(from->visibility());
  to->set_CppVisibility(from->cppVisibility());
  to->set_CppDecl(::java2Php(from->cppDecl()));
  to->set_JavaDecl(from->javaDecl());
  
  return to;
}
