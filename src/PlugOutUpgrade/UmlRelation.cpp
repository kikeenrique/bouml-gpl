#include "UmlRelation.h"
#include "UmlClass.h"
#include "UmlCom.h"

UmlRelation * UmlRelation::java2Php(UmlClass * php, UmlClass * java,
				    const char * name)
{
  UmlRelation * from = java->get_relation(name);
  
  if (from == 0) {
    QByteArray err = QByteArray("cannot find relation '") + 
      name + QByteArray("' in class '") + java->name()
	+ QByteArray("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlRelation * to = 
    UmlBaseRelation::create(from->relationKind(), php, from->roleType());
  
  if (to == 0) {
    QByteArray err = QByteArray("cannot create relation '") + 
      name + QByteArray("' in class '") + php->name()
	+ QByteArray("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlCom::trace("add relation " + php->name() + "::" + name + "<br>\n");

  to->set_RoleName(name);
  to->set_Multiplicity(from->multiplicity());
  to->set_isClassMember(from->isClassMember());
  to->set_Visibility(from->visibility());
  to->set_CppVisibility(from->cppVisibility());
  to->set_CppDecl(from->cppDecl());
  to->set_JavaDecl(from->javaDecl());
  
  return to;
}
