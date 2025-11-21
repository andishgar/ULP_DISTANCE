//
// Created by arashandishgar on 11/19/25.
//

#pragma once

#include <cinttypes>
#include <random>
#include <limits>

namespace ULP
{
    namespace detail
    {
        template <typename Float>
        struct UvalType;

        template <>
        struct UvalType<float>
        {
            using Type = int32_t;
        };

        template <>
        struct UvalType<double>
        {
            using Type = int64_t;
        };
    }

    template <typename Float>
    std::pair<std::vector<Float>, std::vector<Float>>
    GenerateData(int64_t size, double nan_probability = 0.2, double inf_probability = 0.1)
    {
        std::vector<Float> v1;
        std::vector<Float> v2;
        v1.reserve(size);
        v2.reserve(size);
        std::mt19937_64 float_generator(2);
        std::mt19937_64 ulp_generate(2);
        std::mt19937_64 nan_generator(2);
        std::mt19937_64 inf_generator(2);
        std::mt19937_64 second_generator(2);
        std::uniform_real_distribution<Float> float_dist(-1000.0, 1000.0);
        std::uniform_real_distribution<double> nan_dist(0, 1);
        std::uniform_real_distribution<double> inf_dist(0, 1);
        std::uniform_int_distribution<int> second_dist(0, 2);
        std::uniform_int_distribution<typename detail::UvalType<Float>::Type> int_dist(-10, 10);
        for (int64_t i = 0; i < size; ++i)
        {
            auto nan_chance = nan_dist(nan_generator);
            if (nan_probability != 0 && nan_chance <= nan_probability)
            {
                v1.push_back(std::numeric_limits<Float>::quiet_NaN());
                auto second_chance = second_dist(second_generator);
                if (second_chance == 0)
                {
                    v2.push_back(std::numeric_limits<Float>::signaling_NaN());
                }
                else if (second_chance == 1)
                {
                    v2.push_back(std::numeric_limits<Float>::infinity());
                }
                else
                {
                    v2.push_back(float_dist(float_generator));
                }
            }
            else if (inf_probability != 0 && inf_dist(inf_generator) <= inf_probability)
            {
                v1.push_back(std::numeric_limits<Float>::infinity());
                auto second_chance = second_dist(second_generator);
                if (second_chance == 0)
                {
                    v2.push_back(std::numeric_limits<Float>::infinity());
                }
                else if (second_chance == 1)
                {
                    v2.push_back(-1 * std::numeric_limits<Float>::infinity());
                }
                else
                {
                    v2.push_back(float_dist(float_generator));
                }
            }
            else
            {
                auto float_value = float_dist(float_generator);
                auto uval = std::bit_cast<typename detail::UvalType<Float>::Type>(float_value);
                auto ulp_distance = int_dist(ulp_generate);
                v1.push_back(float_value);
                v2.push_back(std::bit_cast<Float>(ulp_distance + uval));
            }
        }
        return {v1, v2};
    }
}
