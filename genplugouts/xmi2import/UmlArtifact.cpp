
#include "UmlArtifact.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

void UmlArtifact::init()
{
  declareFct("ownedmember", "uml:Artifact", &importIt);
  declareFct("packagedelement", "uml:Artifact", &importIt);

}

void UmlArtifact::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anArtifact, token, in);
  
  if (where == 0) {
    if (! token.closed())
      in.finish(token.what());
    return;
  }
    
  QCString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous_artifact_%u", ++n);
  }
    
  UmlArtifact * artifact = create((UmlDeploymentView *) where, s);
  
  if (artifact == 0)
    in.error("cannot create artifact '" + s +
	     "' in '" + where->name() + "'");
  
  artifact->addItem(token.xmiId(), in);

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr))
      artifact->UmlItem::import(in, token);
  }

  artifact->unload(TRUE, FALSE);
}

int UmlArtifact::NumberOf;

