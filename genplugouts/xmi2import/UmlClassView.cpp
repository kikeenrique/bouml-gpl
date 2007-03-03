
#include "UmlClassView.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

UmlItem * UmlClassView::container(anItemKind kind, const Token & token, FileIn & in) {
  switch (kind) {
  case aClass:
  case aState:
  case anActivity:
    return this;
  default:
    return parent()->container(kind, token, in);
  }

}

