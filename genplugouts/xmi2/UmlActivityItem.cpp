
#include "UmlActivityItem.h"
#include "FileOut.h"
#include "UmlActivity.h"

void UmlActivityItem::write_selection(FileOut & out, QCString s) {
  if (! s.isEmpty())
    out.ref(activity()->add_opaque_behavior(s), "selection", "OPAQUE_BEHAVIOR_");
}

UmlActivity * UmlActivityItem::activity() {
  UmlItem * r = dynamic_cast<UmlItem *>(this);
  
  do {
    r = r->parent();
  } while (r->kind() != anActivity);
    
  return (UmlActivity *) r;
}

