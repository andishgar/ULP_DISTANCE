//
// Created by arashandishgar on 11/19/25.
//

#include "ulp.h"

#include <cmath>
#include <limits>

namespace ULP
{
    namespace
    {
        template <typename Float>
        bool WithinUlpOneWay(Float left, Float right, int n_ulps)
        {
            // The delta between 1.0 and the FP value immediately before it.
            // We're using this value because `frexp` returns a mantissa between 0.5 and 1.0.
            static const Float kOneUlp = Float(1.0) - std::nextafter(Float(1.0), Float(0.0));

            if (left == 0)
            {
                return left == right;
            }
            if (left < 0)
            {
                left = -left;
                right = -right;
            }

            int left_exp;
            Float left_mant = std::frexp(left, &left_exp);
            Float delta = static_cast<Float>(n_ulps) * kOneUlp;
            Float lower_bound = std::ldexp(left_mant - delta, left_exp);
            Float upper_bound = std::ldexp(left_mant + delta, left_exp);
            return right >= lower_bound && right <= upper_bound;
        }

        template <typename Float>
        bool WithinUlpGeneric(Float left, Float right, int n_ulps)
        {
            if (std::isnan(left) || std::isnan(right))
            {
                return std::isnan(left) == std::isnan(right);
            }
            if (!std::isfinite(left) || !std::isfinite(right))
            {
                return left == right;
            }
            if (n_ulps == 0)
            {
                return left == right;
            }
            return (std::abs(left) <= std::abs(right))
                       ? WithinUlpOneWay(left, right, n_ulps)
                       : WithinUlpOneWay(right, left, n_ulps);
        }
    } // namespace

    bool ArrowWithinULP(float left, float right, int n_ulps)
    {
        return WithinUlpGeneric(left, right, n_ulps);
    }

    bool ArrowWithinULP(double left, double right, int n_ulps)
    {
        return WithinUlpGeneric(left, right, n_ulps);
    }
}
