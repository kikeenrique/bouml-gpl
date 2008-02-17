
#include "UmlAttribute.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlRelation.h"
#include "UmlCom.h"
void UmlAttribute::init()
{
  declareFct("ownedattribute", "uml:Property", &importIt);
  declareFct("ownedliteral", "uml:EnumerationLiteral", &importIt);
}

void UmlAttribute::importIt(FileIn & in, Token & token, UmlItem * where)
{
  if (!token.valueOf("association").isNull())
    UmlRelation::importAsAttribute(in, token, where);
  else {
    where = where->container(anAttribute, token, in);
    
    if (where != 0) {
      QCString s = token.valueOf("name");
      
      if (s.isEmpty()) {
	static unsigned n = 0;
	
	s.sprintf("anonymous_attribute_%u", ++n);
      }
      else
        s = legalName(s);
      
      UmlAttribute * att = create((UmlClass *) where, s);
      
      if (att == 0)
	in.error("cannot create attribute '" + s +
		 "' in '" + where->name() + "'");
      
      att->addItem(token.xmiId(), in);
      
      att->setVisibility(token.valueOf("visibility"));
      
      if (token.valueOf("isreadonly") == "true")
	 att->set_isReadOnly(TRUE);
      
      if (!(s = token.valueOf("type")).isEmpty()) {
	UmlTypeSpec ts;
	
	if (att->setType(s, ts))
	  att->set_Type(ts);
      }
      if (!(s = token.valueOf("defaultvalue")).isEmpty())
	att->set_DefaultValue(s);
      if (token.valueOf("isstatic") == "true")
	att->set_isClassMember(TRUE);
	  
      if (! token.closed()) {
	QCString k = token.what();
	const char * kstr = k;
	
	while (in.read(), !token.close(kstr)) {
	  s = token.what();
	  
	  if (s == "type") {
	    UmlTypeSpec ts;
	    
	    if (att->setType(token, ts))
	      att->set_Type(ts);
	    if (! token.closed())
	      in.finish(s);
	  }
	  else if (s == "defaultvalue") {
	    att->set_DefaultValue(token.valueOf("value"));
	    if (! token.closed())
	      in.finish(s);
	  }
	  else if (s == "lowervalue")
	    att->importMultiplicity(in, token, FALSE);
	  else if (s == "uppervalue")
	    att->importMultiplicity(in, token, TRUE);
	  else
	    att->UmlItem::import(in, token);
	}
      }
    }
  }
}

void UmlAttribute::solve(QCString idref) {
  UmlTypeSpec ts;
  
  if (getType(idref, ts))
    set_Type(ts);
  else
    UmlCom::trace("attribute : unknown type reference '" + idref + "'<br>");
}

void UmlAttribute::importMultiplicity(FileIn & in, Token & token, bool upper) {
  QCString s = token.valueOf("value");
  
  if (!s.isEmpty() && 
      (s != "Unspecified")) {	// VP
    QCString m = multiplicity();
    
    if (m.isEmpty())
      m = s;
    else if (m != s) {
      if (upper)
	m += ".." + s;
      else
	m = s + ".." + m;
    }
    
    set_Multiplicity(m);
  }

  if (! token.closed())
    in.finish(token.what());
}

