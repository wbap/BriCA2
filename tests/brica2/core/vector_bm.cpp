#include "brica2/core/vector.hpp"

#include <cassert>

#include "benchmark/benchmark_api.h"

namespace bc = brica2::core;

static void Vector1dSimple(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());

  bc::Vector<int> v({size});
  bc::Vector<int> u({size});

  while(state.KeepRunning()) {
    u = v;
  }
}

static void Vector1dClone(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());

  bc::Vector<int> v({size});
  bc::Vector<int> u({size});

  while(state.KeepRunning()) {
    u = v.clone();
  }
}

static void Vector2dSimple(benchmark::State& state) {
  std::size_t size_x = static_cast<std::size_t>(state.range(0));
  std::size_t size_y = static_cast<std::size_t>(state.range(1));

  bc::Vector<int> v({size_x, size_y});
  bc::Vector<int> u({size_x, size_y});

  while(state.KeepRunning()) {
    u = v;
  }
}

static void Vector2dClone(benchmark::State& state) {
  std::size_t size_x = static_cast<std::size_t>(state.range(0));
  std::size_t size_y = static_cast<std::size_t>(state.range(1));

  bc::Vector<int> v({size_x, size_y});
  bc::Vector<int> u({size_x, size_y});

  while(state.KeepRunning()) {
    u = v.clone();
  }
}

BENCHMARK(Vector1dSimple)->Range(4, 1 << 20);
BENCHMARK(Vector1dClone )->Range(4, 1 << 20);
BENCHMARK(Vector2dSimple)->RangePair(4, 1 << 10, 4, 1 << 10);
BENCHMARK(Vector2dClone )->RangePair(4, 1 << 10, 4, 1 << 10);

BENCHMARK_MAIN();
