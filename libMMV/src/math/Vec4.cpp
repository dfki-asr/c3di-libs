#include "libmmv/math/Vec4.h"

namespace libmmv
{
  template<>
  Vec4<unsigned int> Vec4<unsigned int>::operator-() const
  {
    throw std::runtime_error("cannot negate Vec4<unsigned int>");
  };

  template class Vec4<float>;
	template class Vec4<unsigned int>;
	template class Vec4<int>;
	template class Vec4<double>;
}