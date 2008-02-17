
#include "UmlBaseFragmentCompartment.h"
#include "UmlBaseFragment.h"

void UmlBaseFragmentCompartment::add_contained_(UmlFragment * x) {
  _contained.resize(_contained.size() + 1);
  _contained.insert(_contained.size() - 1, x);
}

void UmlBaseFragmentCompartment::add_text_(QCString x) {
  _texts.resize(_texts.size() + 1);
  _texts.insert(_texts.size() - 1, strdup(x));
}

bool UmlBaseFragmentCompartment::smaller(const UmlBaseFragmentCompartment * x) const {
  return ((_fragment->w() < x->_fragment->w()) &&
          (_fragment->h() < x->_fragment->h()));
}

void UmlBaseFragmentCompartment::init(UmlBaseFragment * fragment, int rank, int y) {
 _fragment = (UmlFragment *) fragment;
 _rank = rank;
 _y = y;
}

 UmlBaseFragmentCompartment::~UmlBaseFragmentCompartment() {
}

