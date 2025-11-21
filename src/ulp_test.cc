//
// Created by arashandishgar on 11/19/25.
//

#include<random>

#include <gtest/gtest.h>
#include <boost/math/special_functions/next.hpp>

#include "ulp.h"

#include <bitset>

#include "generator_util.h"

template <typename T>
class ULPTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto vectors = ULP::GenerateData<T>(size, 0, 0);
        this->v1 = std::move(vectors.first);
        this->v2 = std::move(vectors.second);
    }

    void ArrowVersion()
    {
        for (int64_t i = 0; i < size; ++i)
        {
            ASSERT_EQ(std::abs(boost::math::float_distance(v1[i],v2[i]))<=5, ULP::ArrowWithinULP(v1[i],v2[i],5));
        }
    }

    void MyVersion()
    {
        for (int64_t i = 0; i < size; ++i)
        {
            ASSERT_EQ(std::abs(boost::math::float_distance(v1[i],v2[i]))<=5, ULP::MyWithinUlp(v1[i],v2[i],5));
        }
    }

    int64_t size = 1000000;
    std::vector<T> v1;
    std::vector<T> v2;
};

using FloatTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(ULPTest, FloatTypes);

TYPED_TEST(ULPTest, ArrowVersion)
{
    this->ArrowVersion();
}

TYPED_TEST(ULPTest, MyVersion)
{
    this->MyVersion();
}

TEST(F, W)
{
    double a_raw = 4;
    double a = std::bit_cast<double>(std::bit_cast<int64_t>(a_raw) - 1);
    double b = std::nextafter(a, std::numeric_limits<double>::infinity());;
    b = std::nextafter(b, std::numeric_limits<double>::infinity());;
    ASSERT_EQ(std::fabs(boost::math::float_distance(a,b)), 2);
    ASSERT_TRUE(ULP::MyWithinUlp(a,b,2));
    ASSERT_FALSE(ULP::ArrowWithinULP(a,b,2));
}
