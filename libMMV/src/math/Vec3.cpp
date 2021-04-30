#include "libmmv/math/Vec3.h"

namespace libmmv
{
    template<>
    Vec3<unsigned int> Vec3<unsigned int>::operator-() const
    {
      throw std::runtime_error("cannot negate Vec3<unsigned int>");
    };

    template class Vec3<float>;
    template class Vec3<unsigned int>;
    template class Vec3<int>;
    template class Vec3<double>;
}