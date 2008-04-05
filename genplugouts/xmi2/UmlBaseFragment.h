#ifndef _UMLBASEFRAGMENT_H
#define _UMLBASEFRAGMENT_H


#include <qcstring.h>
#include <qvector.h>

class UmlFragmentCompartment;
class UmlFragment;

// this class manages fragments
class UmlBaseFragment {
  public:
    // return the name
    const QCString & name() const {
        return _name;
    }

    // return the compartments, at least one compartment exists
    const QVector<UmlFragmentCompartment> & compartments() const {
        return _compartments;
    }

    // return the fragment compartment containing the fragment,
    // or 0/null
    UmlFragmentCompartment * container() const {
        return _container;
    }


  private:
    UmlFragmentCompartment * _container;

    QVector<UmlFragmentCompartment> _compartments;

    QCString _name;

    int _x;

    int _y;

    int _w;

    int _h;

    // internal, don't call it
    void read_();


  public:
    // internal
    static UmlFragmentCompartment * get_container_(int x, int y, int w, int h, const QVector<UmlFragment> & fragments);


  private:
    // internal
    static void compute_container_(QVector<UmlFragment> & fragments);


  public:
    int w() const {
        return _w;
    }

    int h() const {
        return _h;
    }

    //  to avoid compiler warning, don't call it
     ~UmlBaseFragment();

  friend class UmlBaseUseCaseDiagramDefinition;
  friend class UmlBaseSequenceDiagramDefinition;
};

#endif
