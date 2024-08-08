#ifndef _TOKEN_H
#define _TOKEN_H


#include <qcstring.h>
#include <qvaluelist.h>

class FileIn;

//A token correspond to the information of a form <...> or <... /> or </...>
class Token {
  public:
    void read(FileIn & in, bool any);

    //return the work read just after <, may start by /
     const QByteArray & what() const;

    //return the value of the xmi:type
    const QByteArray & xmiType() const;

    //return the value of the xmi:id
    const QByteArray & xmiId() const;

    //return the value of the xmi:idref
    const QByteArray & xmiIdref() const;

    //return true if the token is </...>
    bool close() const;

    //return true if the token is </what>
    bool close(const char * what) const;

    //return true if the token is <.../>
    bool closed() const;

    //return the value associated to a key, "" if the couple doesn't exist
     const QByteArray & valueOf(QByteArray key) const;

    //return TRUE if the couple exist and set v with the value associated to a key
    bool valueOf(QByteArray key, QByteArray & v) const;


  private:
    //associate a key and a value
    struct Couple {
        QByteArray key;

        QByteArray value;

    };
    
    

  protected:
    QValueList<Couple> _couples;

    //the work after < with out the / if it is </what>
    QByteArray _what;

    //true if the token is </..>
    bool _close;

    //true if the token is <../>
    bool _closed;

};

inline  const QByteArray & Token::what() const {
  return _what;
}

inline const QByteArray & Token::xmiType() const {
  return valueOf("xmi:type");
}

inline const QByteArray & Token::xmiId() const {
  return valueOf("xmi:id");
}

inline const QByteArray & Token::xmiIdref() const {
  return valueOf("xmi:idref");
}

inline bool Token::close() const {
  return _close;
}

inline bool Token::closed() const {
  return _closed;
}

#endif
