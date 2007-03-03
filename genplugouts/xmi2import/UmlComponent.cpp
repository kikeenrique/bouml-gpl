
#include "UmlComponent.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlCom.h"
void UmlComponent::init()
{
  declareFct("ownedmember", "uml:Component", &importIt);
  declareFct("packagedelement", "uml:Component", &importIt);
}

void UmlComponent::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aComponent, token, in);
  
  if (where == 0) {
    if (! token.closed())
      in.finish(token.what());
    return;
  }
    
  QCString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous_component_%u", ++n);
  }
    
  UmlComponent * component = create((UmlComponentView *) where, s);
  
  if (component == 0)
    in.error("cannot create component '" + s +
	     "' in '" + where->name() + "'");
  
  component->addItem(token.xmiId(), in);

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      QCString ks = token.what();
      
      if ((ks == "provided") || (ks == "required") || (ks == "realization"))
	component->manageInterface(token, in);
      else
	component->UmlItem::import(in, token);
    }
  }

  component->unload(TRUE, FALSE);
}

void UmlComponent::manageInterface(Token & token, FileIn & in) {
  QCString idref = token.xmiIdref();
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  char c = ((const char *) token.what())[2];
  
  if (it != All.end()) {
    if ((*it)->kind() == aClass) {
      switch (c) {
      case 'o':
	// provided
	{
	  QVector<UmlClass> provided = providedClasses();
	  unsigned sz = provided.size();
	  
	  provided.resize(sz + 1);
	  provided.insert(sz, (UmlClass *) *it);
	  set_AssociatedClasses(realizedClasses(), provided, requiredClasses());
	}
	break;
      case 'q':
	// required:
	{
	  QVector<UmlClass> required = requiredClasses();
	  unsigned sz = required.size();
	  
	  required.resize(sz + 1);
	  required.insert(sz, (UmlClass *) *it);
	  set_AssociatedClasses(realizedClasses(), providedClasses(), required);
	}
	break;
      default:
	// realization
	{
	  QVector<UmlClass> realized = realizedClasses();
	  unsigned sz = realized.size();
	  
	  realized.resize(sz + 1);
	  realized.insert(sz, (UmlClass *) *it);
	  set_AssociatedClasses(realized, providedClasses(), requiredClasses());
	}
	break;
      }
    }
  }
  else
    UnresolvedWithContext::add(this, idref, c);
    
  if (! token.closed())
    in.finish(token.what());
}

void UmlComponent::solve(int context, QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it != All.end()) {
    if ((*it)->kind() == aClass) {
      switch (context) {
      case 'o':
	// provided
	{
	  QVector<UmlClass> provided = providedClasses();
	  unsigned sz = provided.size();
	  
	  provided.resize(sz + 1);
	  provided.insert(sz, (UmlClass *) *it);
	  set_AssociatedClasses(realizedClasses(), provided, requiredClasses());
	}
	break;
      case 'q':
	// required:
	{
	  QVector<UmlClass> required = requiredClasses();
	  unsigned sz = required.size();
	  
	  required.resize(sz + 1);
	  required.insert(sz, (UmlClass *) *it);
	  set_AssociatedClasses(realizedClasses(), providedClasses(), required);
	}
	break;
      default:
	// realization
	{
	  QVector<UmlClass> realized = realizedClasses();
	  unsigned sz = realized.size();
	  
	  realized.resize(sz + 1);
	  realized.insert(sz, (UmlClass *) *it);
	  set_AssociatedClasses(realized, providedClasses(), requiredClasses());
	}
	break;
      }
    }
  }
  else
    UmlCom::trace("component : unknown class reference '" + idref + "'<br>");
}

int UmlComponent::NumberOf;

