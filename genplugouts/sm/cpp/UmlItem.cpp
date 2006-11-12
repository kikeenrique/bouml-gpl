
#include "UmlItem.h"
#include "UmlClass.h"
#include "UmlState.h"

#include "UmlCom.h"
#include "UmlRelation.h"

 UmlItem::~UmlItem() {
}

UmlItem * UmlItem::getChild(anItemKind k, QCString s) {
  const QVector<UmlItem> ch = children();
  unsigned index;
  
  if (k == aRelation) {
    for (index = 0; index != ch.count(); index += 1)
      if ((ch[index]->kind() == aRelation) &&
	  (((UmlRelation *) ch[index])->roleName() == s))
	return ch[index];
  }
  else {
    for (index = 0; index != ch.count(); index += 1)
      if ((ch[index]->kind() == k) && (ch[index]->name() == s))
	return ch[index];
  }

  return 0;
}

QCString UmlItem::quotedName() {
  QCString r;
  const char * s = name();

  for (;;) {
    char c = *s++;
    
    if (((c >= 'a') && (c <= 'z')) ||
	((c >= 'A') && (c <= 'Z')) ||
	((c >= '0') && (c <= '9')) ||
	(c == '_'))
      r += c;
    else if (c == ' ')
      r += '_';
    else if (c == 0)
      return r;
  }

}

void UmlItem::init(UmlClass *, QCString, UmlState *) {
}

void UmlItem::generate() {
  UmlCom::trace("Error : must be applied on a state machine<br>");
}

void UmlItem::generate(UmlClass *, UmlClass *, UmlState *) {
}

void UmlItem::generate(UmlClass *, UmlClass *, UmlState *, QCString &, QCString) {
}

void UmlItem::setComment(QCString) {
}

