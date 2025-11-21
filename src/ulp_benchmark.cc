//
// Created by arashandishgar on 11/19/25.
//

#include <benchmark/benchmark.h>

#include "ulp.h"
#include "generator_util.h"

#define CUSTOM_RANGE(BM_NAME)->Name(BM_NAME)->RangeMultiplier(10)->Range(1000, 10000000);

template <typename Float>
void BM_ARROW_ULP(benchmark::State& state)
{
    auto size = state.range(0);
    auto [v1,v2] = ULP::GenerateData<Float>(size);
    for (auto _ : state)
    {
        for (int64_t i = 0; i < size; i++)
        {
            benchmark::DoNotOptimize(ULP::ArrowWithinULP(v1[i], v2[i], 2));
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
    state.SetBytesProcessed(state.iterations() * 2 * size * sizeof(Float));
}

template <typename Float>
void BM_MY_ULP(benchmark::State& state)
{
    auto size = state.range(0);
    auto [v1,v2] = ULP::GenerateData<Float>(size);
    for (auto _ : state)
    {
        for (int64_t i = 0; i < size; i++)
        {
            benchmark::DoNotOptimize(ULP::MyWithinUlp(v1[i], v2[i], 2));
        }
    }
    state.SetItemsProcessed(state.iterations() * size);
    state.SetBytesProcessed(state.iterations() * 2 * size * sizeof(Float));
}

BENCHMARK(BM_MY_ULP<float>)CUSTOM_RANGE("my_float_version");
BENCHMARK(BM_MY_ULP<double>)CUSTOM_RANGE("my_double_verion")

BENCHMARK(BM_ARROW_ULP<float>)CUSTOM_RANGE("arrow_float_version");
BENCHMARK(BM_ARROW_ULP<double>)CUSTOM_RANGE("arrow_double_version");

BENCHMARK_MAIN();
