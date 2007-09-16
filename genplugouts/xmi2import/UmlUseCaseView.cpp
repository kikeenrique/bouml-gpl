
#include "UmlUseCaseView.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

UmlItem * UmlUseCaseView::container(anItemKind kind, const Token & token, FileIn & in) {
  switch (kind) {
  case aClass:
  case aClassInstance:
  case anUseCase:
    return this;
  default:
    return parent()->container(kind, token, in);
  }
}

