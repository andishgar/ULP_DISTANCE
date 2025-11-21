//
// Created by arashandishgar on 11/19/25.
//


#pragma once
#include <cinttypes>

namespace ULP
{
    bool ArrowWithinULP(double a, double b, int distance);
    bool ArrowWithinULP(float a, float b, int distance);
    bool MyWithinUlp(double a, double b, int distance);
    bool MyWithinUlp(float a, float b, int distance);
    uint64_t Distance(double v1, double v2);
    uint32_t Distance(float v1, float v2);
}
