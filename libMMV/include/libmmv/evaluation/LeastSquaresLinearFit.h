#pragma once
#include <vector>
#include "libmmv/math/Vec2.h"

namespace libmmv
{
    class LeastSquaresLinearFit
    {
    public:
        LeastSquaresLinearFit(const std::vector<Vec2f>& points);
        virtual ~LeastSquaresLinearFit();
        float getYAxisIntersection();
        float getSlope();

    protected:
        float remainingErrorOfPoint(Vec2f p);
        float linearModelValueAt(float x);
        float totalVarianceOfValues();

    protected:
        void computeFit();

        const std::vector<Vec2f>& points;
        float yAxisIntersection;
        float slope;
    };
}