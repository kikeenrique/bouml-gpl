
#include "Token.h"
#include "FileIn.h"

void Token::read(FileIn & in, bool any) {
  _couples.clear();
  
  const char * k;
  bool str;
  
  // bypass comment
  for (;;) {
    k = in.readWord(any, str);
    if (str || (*k != '<')) {
      if (any)
	continue;
      else
	in.error("'&lt;...' expected");
    }
    
    k = in.readWord(any, str);
    if (str || (*k != '!'))
      break;
    
    while ((*in.readWord(any, str) != '>') || str)
      ;
  }


  char last;
  
  // note : k is not a string
  if (*k == '?') {
    last = '?';
    k = in.readWord(any, str);
  }
  else
    last = '/';
  
  if (!str && (*k == '/')) {
    _close = TRUE;
    k = in.readWord(any, str);
  }
  else
    _close = FALSE;
  
  if (str)
    in.error("syntax error \"" + QCString(k) + "\" unexpected");
    
  _what = k;
  
  while (k = in.readWord(any, str), str || (*k != '>')) {
    if (!str && (*k == last)) {
      k = in.readWord(any, str);
      if (str || (*k != '>'))
	in.error("syntax error near '" + QCString(k)  + "'>' expected");
      _closed = TRUE;
      return;
    }
    
    if (! any) {
      if (str)
	in.error("syntax error near '" + QCString(k) + "'");
      
      Couple cpl;
      
      cpl.key = k;
      
      if (!valueOf(k).isNull())
	in.error("'" + cpl.key + "' duplicated");
      
      if ((*in.readWord(FALSE, str) != '=') || str)
	in.error("syntax error near '" + QCString(k) + "', '=' expected");
      
      cpl.value = in.readWord(FALSE, str);
      if (!str)
	in.error("syntax error after '='");
      _couples.append(cpl);
    }
  }

  _closed = FALSE;
}

bool Token::close(const char * what) const {
  return _close && (_what == what);
}

const QCString & Token::valueOf(QCString key) const {
  QValueList<Couple>::ConstIterator iter;
  
  for (iter = _couples.begin(); iter != _couples.end(); ++iter)
    if ((*iter).key == key)
      return (*iter).value;
			  
  static QCString null;
  
  return null;
}

