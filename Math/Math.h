#pragma once

#include <algorithm>
#include <limits>

namespace VenusEngine
{
    namespace Math
    {
        float const POS_INFINITY = std::numeric_limits<float>::infinity();
        float const NEG_INFINITY = -std::numeric_limits<float>::infinity();
        float const PI = 3.14159265358979323846264338327950288f;
        float const ONE_OVER_PI = 1.0f / PI;
        float const TWO_PI = 2.0f * PI;
        float const HALF_PI = 0.5f * PI;
        float const fDeg2Rad = PI / 180.0f;
        float const fRad2Deg = 180.0f / PI;
        float const LOG2 = log(2.0f);
        float const EPSILON = 1e-6f;
    }
}