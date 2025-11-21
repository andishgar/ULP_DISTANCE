//
// Created by arashandishgar on 11/19/25.
//

#include "ulp.h"

#include <cstdint>
#include <algorithm>
#include <cmath>

namespace ULP
{
    namespace
    {
        template <typename T>
        struct UvalType;

        template <>
        struct UvalType<float>
        {
            using Type = uint32_t;
        };

        template <>
        struct UvalType<double>
        {
            using Type = uint64_t;
        };

        template <typename Float>
        union FloatUnion
        {
            explicit FloatUnion(Float value) : value(value)
            {
            }

            Float value;
            typename UvalType<Float>::Type uval;
        };

        template <typename UVal>
        UVal ConvertSignAndMagnitudeToBiased(UVal uval)
        {
            auto size = sizeof(UVal) * 8;
            UVal signed_mask = UVal{1} << (size - 1);
            if (uval & signed_mask)
            {
                return ~uval + 1;
            }
            {
                return uval | signed_mask;
            }
        }

        template <typename T>
        bool WithinUlpImpl(T v1, T v2, int distance)
        {
            if (std::isnan(v1) || std::isnan(v2))
            {
                return false;
            }
            else if (!std::isfinite(v1) || !std::isfinite(v2))
            {
                return v1 == v2;
            }

            FloatUnion<T> v1_union(v1), v2_union(v2);
            auto v1_biased = ConvertSignAndMagnitudeToBiased(v1_union.uval);
            auto v2_biased = ConvertSignAndMagnitudeToBiased(v2_union.uval);
            if (v1_biased > v2_biased)
            {
                std::swap(v1_biased, v2_biased);
            }
            return v2_biased - v1_biased <= static_cast<uint64_t>(distance);
        }

        template <typename  T>
        typename UvalType<T>::Type DistanceImpl(T v1, T v2)
        {
            if (std::isnan(v1) || std::isnan(v2))
            {
                return false;
            }
            if (!std::isfinite(v1) || !std::isfinite(v2))
            {
                return v1 == v2;
            }

            FloatUnion<T> v1_union(v1), v2_union(v2);
            auto v1_biased = ConvertSignAndMagnitudeToBiased(v1_union.uval);
            auto v2_biased = ConvertSignAndMagnitudeToBiased(v2_union.uval);
            if (v1_biased > v2_biased)
            {
                std::swap(v1_biased, v2_biased);
            }
            uint64_t result = v2_biased - v1_biased;
            return result;
        }
    }

    bool MyWithinUlp(double v1, double v2, int distance)
    {
        return WithinUlpImpl<double>(v1, v2, distance);
    }

    bool MyWithinUlp(float v1, float v2, int distance)
    {
        return WithinUlpImpl<float>(v1, v2, distance);
    }

    uint32_t Distance(float v1, float v2)
    {
        return DistanceImpl(v1, v2);
    }

    uint64_t Distance(double v1, double v2)
    {
        return DistanceImpl(v1, v2);
    }
}
