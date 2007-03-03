
#include "UmlOperation.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlCom.h"
void UmlOperation::init()
{
  declareFct("ownedoperation", "", &importIt);
  declareFct("ownedoperation", "uml:Operation", &importIt);
}

void UmlOperation::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anOperation, token, in);
  
  if (where == 0) {
    if (! token.closed())
      in.finish(token.what());
    return;
  }
    
  QCString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous_operation_%u", ++n);
  }
    
  UmlOperation * op = create((UmlClass *) where, s);
  
  if (op == 0)
    in.error("cannot create operation '" + s +
	     "' in '" + where->name() + "'");
  
  op->addItem(token.xmiId(), in);
  
  op->setVisibility(token.valueOf("visibility"));
  
  if (token.valueOf("isstatic") == "true")
    op->set_isClassMember(TRUE);

  if (token.valueOf("isabstract") == "true")
    op->set_isAbstract(TRUE);
    
  if (token.valueOf("isquery") == "true")
    op->set_isCppConst(TRUE);

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
	  
      if (s == "raisedexception")
	op->addException(token, in);
      else if (s == "ownedparameter")
	op->addParameter(token, in);
      else
        op->UmlItem::import(in, token);
    }
  }
}

void UmlOperation::addException(Token & token, FileIn & in) {
  QCString idref = token.xmiIdref();
  
  if (! idref.isEmpty()) {
    int n = exceptions().count();
    UmlTypeSpec ts;
    
    (void) setType(idref, -n -1, ts);
    UmlBaseOperation::addException(n, ts);
  }

  if (! token.closed())
    in.finish(token.what());
}

void UmlOperation::addParameter(Token & token, FileIn & in) {
  UmlParameter p;
  QCString s;
  
  p.name = token.valueOf("name");
    
  s = token.valueOf("direction");
  if ((s == "in") || (s == "pk_in"))
    p.dir = InputDirection;
  else if ((s == "out") || (s == "pk_out"))
    p.dir = OutputDirection;
  else if ((s == "inout") || (s == "pk_inout"))
    p.dir = InputOutputDirection;
  else if ((s == "return") || (s == "pk_return"))
    p.dir = ReturnDirection;
  else if (! s.isEmpty())
    in.warning("wrong direction");
    
  if (!(s = token.valueOf("defaultvalue")).isEmpty())
    p.default_value = s;
    
  QCString typeref = token.valueOf("type");
    
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if (s == "type") {
	typeref = token.xmiIdref();
	if (! token.closed())
	  in.finish(s);
      }
      else if (s == "defaultvalue") {
	p.default_value = token.valueOf("value");
	if (! token.closed())
	  in.finish(s);
      }
      else
	in.finish(token.what());
    }
  }

  if (! typeref.isEmpty()) {
    UmlTypeSpec ts;
    
    if (p.dir == ReturnDirection) {
      if (setType(typeref, 0, ts))
	set_ReturnType(ts);
    }
    else {
      int n = params().count();
      
      (void) setType(typeref, n+1, p.type);
      UmlBaseOperation::addParameter(n, p);
    }
  }
}

void UmlOperation::solve(int context, QCString idref) {
  UmlTypeSpec ts;
  
  if (getType(idref, ts)) {
    if (context == 0)
      set_ReturnType(ts);
    else if (context > 0) {
      UmlParameter p = params()[context - 1];
      
      p.type = ts;
      replaceParameter(context - 1, p);
    }
    else
      replaceException(- context + 1, ts);
  }
  else
    UmlCom::trace("operation : unknown type reference '" + idref + "'<br>");
}

