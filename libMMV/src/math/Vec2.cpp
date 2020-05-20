#include "stdafx.h"

#include "Vec2.h"

namespace ettention
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
