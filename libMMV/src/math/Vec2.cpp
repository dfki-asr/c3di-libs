#include "libmmv/math/Vec2.h"

namespace libmmv
{
    template<>
    Vec2<unsigned int> Vec2<unsigned int>::operator-() const
    {
      throw std::runtime_error("cannot negate Vec2<unsigned int>");
    };

    template class Vec2<float>;
    template class Vec2<unsigned int>;
    template class Vec2<int>;
    template class Vec2<double>;
}
