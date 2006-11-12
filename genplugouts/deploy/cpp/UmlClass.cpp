
#include "UmlClass.h"
#include "UmlDeploymentView.h"

#include "UmlArtifact.h"
#include "UmlCom.h"
void UmlClass::deploy(UmlDeploymentView * view) {
  if (associatedArtifact() == 0) {
    UmlCom::trace("Create <i>artifact</i> " + name());
    
    UmlArtifact * a = UmlArtifact::create(view, name());
      
    if (a == 0)
      UmlCom::trace("cannot create <i>artifact</i> named " + name());
    else{
      a->set_Stereotype("source");
      a->addAssociatedClass(this);
    }
  }
}

