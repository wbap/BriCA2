/******************************************************************************
 *
 * cargo_bm.cpp
 *
 * MIT License
 *
 * Copyright (c) 2016 Kotone Itaya
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 *****************************************************************************/

#include "benchmark/benchmark.h"
#include "brica2/core/cargo.hpp"

#include <vector>

namespace b2c = brica2::core;

static void Pointer(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());
  std::vector<float> v0;

  for(std::size_t i = 0; i < size; ++i) {
    v0.push_back((float)i);
  }

  while(state.KeepRunning()) {
    std::vector<float>* v1 = &v0;
  }
}

static void Lazy(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());
  std::vector<float> v0;

  for(std::size_t i = 0; i < size; ++i) {
    v0.push_back((float)i);
  }

  b2c::Cargo c0(v0);

  while(state.KeepRunning()) {
    b2c::Cargo c1 = c0;
    std::vector<float>& v1 = c1;
  }
}

static void Deep(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());
  std::vector<float> v0;

  for(std::size_t i = 0; i < size; ++i) {
    v0.push_back((float)i);
  }

  b2c::Cargo c0(v0);

  while(state.KeepRunning()) {
    std::vector<float> v1 = c0.get<std::vector<float>>();
    b2c::Cargo c1(v1);
  }
}

BENCHMARK(Pointer)->Range(4, 1 << 20);
BENCHMARK(Lazy)->Range(4, 1 << 20);
BENCHMARK(Deep)->Range(4, 1 << 20);

BENCHMARK_MAIN();
