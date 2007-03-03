
#include "Association.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlCom.h"
#include "UmlRelation.h"
#include "UmlClass.h"
void Role::setMultiplicity(QCString v, bool upper) {
  if (!v.isEmpty() && 
      (v != "Unspecified")) {	// VP
    if (multiplicity.isEmpty())
      multiplicity = v;
    else if (multiplicity != v) {
      if (upper)
	multiplicity += ".." + v;
      else
	multiplicity = v + ".." + multiplicity;
    }
  }
}

void Association::import(FileIn & in, Token & token) {
  QCString s = token.xmiId();
  Role & r = ((roles[0].id == s)
	      ? roles[0]
	      : ((roles[1].id == s)
		 ? roles[1]
		 : roles[(roles[0].id.isEmpty()) ? 0 : 1]));
    
  r.id = s;
  if (!(s = token.valueOf("name")).isEmpty())
    r.name = s;
  if (!(s = token.valueOf("type")).isEmpty())
    r.idref = s;
  if (!(s = token.valueOf("visibility")).isEmpty())
    r.visibility = s;
  if (token.valueOf("isnavigable") == "false")
    r.navigable = FALSE;
  if (token.valueOf("isreadonly") == "true")
    r.readOnly = TRUE;
  if (token.valueOf("isstatic") == "true")
    r.isStatic = TRUE;
    
  s = token.valueOf("aggregation");
  if (s == "shared")
    r.aggregate = anAggregation;
  else if ((s == "composite") || (s == "aggregate"))
    r.aggregate = anAggregationByValue;

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
      
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if (s == "type")
	r.idref = token.xmiIdref();
      else if (s == "lowervalue")
	r.setMultiplicity(token.valueOf("value"), FALSE);
      else if (s == "uppervalue")
	r.setMultiplicity(token.valueOf("value"), TRUE);
      
      if (! token.closed())
	in.finish(s);
    }
  }
}

Association & Association::get(QCString id, QCString s)
{
  QMap<QCString, Association>::Iterator it = All.find(id);
    
  if (it == All.end()) {
    Association a;
      
    it = All.insert(id, a);
  }

  if (! s.isEmpty())
    (*it).name = s;
    
  return *it;
}

void Association::solveThem()
{
  QMap<QCString,Association>::Iterator it;
      
  for (it = All.begin(); it != All.end(); ++it)
    (*it).solve(it.key());
    
  All.clear();

  AssociationClass::solveThem();
}

void Association::solve(QCString id) {
  if (roles[0].id.isEmpty() || roles[0].idref.isEmpty() ||
      roles[1].id.isEmpty() || roles[1].idref.isEmpty())
    UmlCom::trace("association '" + id + "' not fully defined<br>");
  else {
    int rank;
    
    if (!roles[0].navigable)
      rank = 1;
    else if (!roles[1].navigable)
      rank = 0;
    else if (roles[0].aggregate == anAssociation)
      rank = 0;
    else
      rank = 1;
    
    Role & a = roles[rank];
    Role & b = roles[1 - rank];
    QMap<QCString, UmlItem *>::Iterator it;
    
    if ((it = UmlItem::All.find(a.idref)) == UmlItem::All.end())
      UmlCom::trace("association : unknown type reference '" + a.idref + "'<br>");
    else if ((*it)->kind() != aClass)
      UmlCom::trace("'" + a.idref + "' is not a class<br>");
    else {
      UmlClass * cla = (UmlClass *) *it;
      
      if ((it = UmlItem::All.find(b.idref)) == UmlItem::All.end())
	UmlCom::trace("association : unknown type reference '" + b.idref + "'<br>");
      else if ((*it)->kind() != aClass)
	UmlCom::trace("'" + b.idref + "' is not a class<br>");
      else {
	UmlClass * clb = (UmlClass *) *it;
	aRelationKind k;
	
	if (!b.navigable) {
	  switch (b.aggregate) {
	  case anAssociation:
	    k = aDirectionalAssociation;
	    break;
	  case anAggregationByValue:
	    k = aDirectionalAggregationByValue;
	    break;
	  default:
	    k = aDirectionalAggregation;
	  }
	}
	else
	  k = b.aggregate;
	
	UmlRelation * ra = UmlRelation::create(k, clb, cla);
	UmlRelation * rb = ra->side(FALSE);
	QCString s;
	static unsigned n = 0;
	
	if (!name.isEmpty())
	  ra->set_Name(name);
	
	UmlItem::All.insert(a.id, ra);
	UmlItem::All.insert(id, ra);	// for association classes
	if (a.name.isEmpty()) {
	  s.sprintf("anonymous_role_%u", ++n);
	  ra->set_RoleName(s);
	}
	else
	  ra->set_RoleName(a.name);
	ra->setVisibility(a.visibility);
	if (a.readOnly)
	  ra->set_isReadOnly(TRUE);
	if (a.isStatic)
	  ra->set_isClassMember(TRUE);
	if (! a.multiplicity.isEmpty())
	  ra->set_Multiplicity(a.multiplicity);
	
	if (rb != 0) {
	  UmlItem::All.insert(b.id, rb);
	  if (b.name.isEmpty()) {
	    s.sprintf("anonymous_role_%u", ++n);
	    rb->set_RoleName(s);
	  }
	  else
	    rb->set_RoleName(b.name);
	  rb->setVisibility(b.visibility);
	  if (b.readOnly)
	    rb->set_isReadOnly(TRUE);
	  if (b.isStatic)
	    rb->set_isClassMember(TRUE);
	  if (! b.multiplicity.isEmpty())
	    rb->set_Multiplicity(b.multiplicity);
	}
      }
    }
  }
}

QMap<QCString, Association> Association::All;

AssociationClass::AssociationClass() {
}

void AssociationClass::init()
{
  UmlItem::declareFct("ownedelement", "uml:AssociationClass", &importIt);
  UmlItem::declareFct("ownedmember", "uml:AssociationClass", &importIt);
  UmlItem::declareFct("packagedelement", "uml:AssociationClass", &importIt);
}

void AssociationClass::importIt(FileIn & in, Token & token, UmlItem *)
{
  QCString ref1;
  QCString ref2;
  
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      QCString s = token.what();
      
      if (s == "memberend") {
	if (ref1.isEmpty())
	  ref1 = token.xmiIdref();
	else
	  ref2 = token.xmiIdref();
      }
      if (! token.closed())
	in.finish(s);
    }
  }
      
  if (ref1.isEmpty() || ref2.isEmpty())
    in.warning("association class not fully defined");
  else
    All.append(AssociationClass(ref1, ref2));
}

void AssociationClass::solveThem()
{
  QValueList<AssociationClass>::Iterator it;
  
  for (it = All.begin(); it != All.end(); ++it)
    (*it).solve();
    
  All.clear();
}

void AssociationClass::solve() {
  QMap<QCString, UmlItem*>::Iterator it1 = UmlItem::All.find(ref1);
  QMap<QCString, UmlItem*>::Iterator it2 = UmlItem::All.find(ref2);
  UmlTypeSpec t;
    
  if (it1 == UmlItem::All.end())
    UmlCom::trace("association class : unknown reference '" + ref1 + "'<br>");
  else if (it2 == UmlItem::All.end())
    UmlCom::trace("association class : unknown reference '" + ref2 + "'<br>");
  else if ((*it1)->kind() == aRelation) {
    if ((*it2)->kind() != aClass)
      UmlCom::trace("'" + ref2 + "' must be a class<br>");
    else {
      t.type = (UmlClass *) *it2;
      ((UmlRelation *) *it1)->set_Association(t);
    }
  }
  else if ((*it1)->kind() == aClass) {
    if ((*it2)->kind() != aRelation)
      UmlCom::trace("'" + ref2 + "' must be an association<br>");
    else {
      t.type = (UmlClass *) *it1;
      ((UmlRelation *) *it2)->set_Association(t);
    }
  }
  else
    UmlCom::trace("'" + ref1 + "' must be a class or an association<br>");
}

QValueList<AssociationClass> AssociationClass::All;

