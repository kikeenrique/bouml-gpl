#ifndef _CONTEXT_H
#define _CONTEXT_H


#include <qcstring.h>

enum Equation {
  NoFilter,
  Filter1,
  And12,
  Or12,
  And12_And23,
  And12_Or23,
  Or12_And23,
  Or12_Or23

};
class Context {
  public:
    Context(const QCString & o, const QCString & f);

    const char * set_filters(const QCString & f1, const QCString & f2, const QCString & f3, bool w1, bool w2, bool w3, bool and12, bool and23);

    void set_stereotype(const QCString & s, bool eq, bool neq);

    void set_targets(bool art, bool cl, bool op, bool att, bool rel);

    void set_language(bool c, bool j, bool p, bool i);

    bool match_stereotype(const QCString & s);

    bool match(QCString s);

    void err();

    QCString replace(QCString s);

    const bool onArtifact() const;

    const bool onClass() const;

    const bool onOperation() const;

    const bool onAttribute() const;

    const bool onRelation() const;

    const bool cpp() const;

    const bool java() const;

    const bool php() const;

    const bool idl() const;

    const int n_match() const;

    const int n_err() const;


  protected:
    QCString _filter1;

    QCString _filter2;

    QCString _filter3;

    bool _with1;

    bool _with2;

    bool _with3;

    QCString _stereotype;

    bool _is;

    bool _isnot;

    bool _onArtifact;

    bool _onClass;

    bool _onOperation;

    bool _onAttribute;

    bool _onRelation;

    bool _cpp;

    bool _java;

    bool _php;

    bool _idl;

    QCString _old;

    QCString _future;

    Equation _equation;

    int _n_match;

    int _n_err;

    bool match(QCString s, const QCString & filter, bool with);

};

inline void Context::set_stereotype(const QCString & s, bool eq, bool neq) {
  _stereotype = s;
  _is = eq;
  _isnot = neq;
}

inline void Context::set_targets(bool art, bool cl, bool op, bool att, bool rel) {
_onArtifact = art;
_onClass = cl;
_onOperation = op;
_onAttribute = att;
_onRelation = rel;
}

inline void Context::set_language(bool c, bool j, bool p, bool i) {
_cpp = c;
_java = j;
_php = p;
_idl = i;
}

inline const bool Context::onArtifact() const {
  return _onArtifact;
}

inline const bool Context::onClass() const {
  return _onClass;
}

inline const bool Context::onOperation() const {
  return _onOperation;
}

inline const bool Context::onAttribute() const {
  return _onAttribute;
}

inline const bool Context::onRelation() const {
  return _onRelation;
}

inline const bool Context::cpp() const {
  return _cpp;
}

inline const bool Context::java() const {
  return _java;
}

inline const bool Context::php() const {
  return _php;
}

inline const bool Context::idl() const {
  return _idl;
}

inline const int Context::n_match() const {
  return _n_match;
}

inline const int Context::n_err() const {
  return _n_err;
}

#endif
