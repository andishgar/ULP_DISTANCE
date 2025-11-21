cmake_minimum_required(VERSION 3.20)
project(MyLibraryProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Boost REQUIRED COMPONENTS math)
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

enable_testing()

add_library(ULPLibrary
        src/arrow_ulp.cc
        src/generator_util.h
        src/my_ulp.cc
        src/ulp.h
)

target_include_directories(ULPLibrary PUBLIC include)


add_executable(test_ulp src/ulp_test.cc)
target_link_libraries(test_ulp PRIVATE ULPLibrary GTest::gtest_main Boost::math)


include(GoogleTest)
gtest_discover_tests(test_ulp)



add_executable(Benchmark_ULP src/ulp_benchmark.cc)
target_link_libraries(Benchmark_ULP PRIVATE ULPLibrary benchmark::benchmark)
