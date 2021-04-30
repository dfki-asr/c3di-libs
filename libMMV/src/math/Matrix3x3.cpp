#include "libmmv/math/Matrix3x3.h"

namespace libmmv
{ 
    template <typename _ValType>
    const Matrix3x3<_ValType> Matrix3x3<_ValType>::identity(1, 0, 0, 0, 1, 0, 0, 0, 1);

    template class Matrix3x3<float>;
    template class Matrix3x3<double>;
}
