#ifndef _UMLBASEFRAGMENTCOMPARTMENT_H
#define _UMLBASEFRAGMENTCOMPARTMENT_H


// to avoid compiler warning
#include "UmlFragment.h"

#include <qvector.h>
#include <qcstring.h>

class UmlFragment;
class UmlBaseFragment;

// this class manages fragments compartments,
// a fragment without separator contains one compartment
class UmlBaseFragmentCompartment {
  public:
    // return the fragment owning the compartment
    UmlFragment * fragment() const {
        return _fragment;
    }

    // the rank of the compartment in the fragment (0..)
    int rank() const {
        return _rank;
    }

    // the texts placed in the compartment
    const QVector<const char> & texts() const {
        return (const QVector<const char> &) _texts;
    }

    // return the fragments contained in the compartment,
    // may be none
    const QVector<UmlFragment> & contained() const {
        return _contained;
    }


  private:
    UmlFragment * _fragment;

    QVector<UmlFragment> _contained;

    int _rank;

    QVector<char> _texts;

    int _y;


  public:
    void add_contained_(UmlFragment * x);

    void add_text_(QCString x);

    int b() const {
        return _y;
    }

    bool smaller(const UmlBaseFragmentCompartment * x) const;


  private:
    // internal, don't call it
    void init(UmlBaseFragment * fragment, int rank, int y);


  public:
    //  to avoid compiler warning, don't call it
     ~UmlBaseFragmentCompartment();

  friend class UmlBaseFragment;
  friend class UmlBaseUseCaseDiagramDefinition;
  friend class UmlBaseSequenceDiagramDefinition;
};

#endif
