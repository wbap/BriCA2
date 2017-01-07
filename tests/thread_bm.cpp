/******************************************************************************
 *
 * thread_bm.cpp
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
#include "brica2.hpp"

#include <vector>
#include <chrono>

namespace b2c = brica2::core;
namespace b2s = brica2::scheduler;

class Pipe : public b2c::Component {
public:
  Pipe(std::size_t time) : time(time) {
    make_in_port("input");
    make_out_port("output");
  }

  b2c::Dictionary operator()(b2c::Dictionary& inputs) {
    b2c::Dictionary outputs;
    auto t0 = std::chrono::steady_clock::now();
    while(1) {
      auto t1 = std::chrono::steady_clock::now();
      auto diff = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
      if(diff.count() > time) break;
    }
    outputs["output"] = inputs["input"];
    return outputs;
  }
private:
  std::size_t time;
};

static void Thread1usec(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());

  b2c::Module m;

  Pipe fst(1);
  m.add_component("c0", fst);

  for(std::size_t i = 1; i < 1000; ++i) {
    Pipe prev = m.get_component<Pipe>("c" + std::to_string(i - 1));
    Pipe next(1);
    b2c::connect(prev, "output", next, "input");
    m.add_component("c" + std::to_string(i), next);
  }

  b2s::VirtualTimeSyncScheduler s(m, size);

  std::size_t i = 0;

  while(state.KeepRunning()) {
    fst.get_in_port("input").set_buffer(b2c::Cargo(42));
    s.step();
  }
}

static void Thread10usec(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());

  b2c::Module m;

  Pipe fst(10);
  m.add_component("c0", fst);

  for(std::size_t i = 1; i < 1000; ++i) {
    Pipe prev = m.get_component<Pipe>("c" + std::to_string(i - 1));
    Pipe next(10);
    b2c::connect(prev, "output", next, "input");
    m.add_component("c" + std::to_string(i), next);
  }

  b2s::VirtualTimeSyncScheduler s(m, size);

  std::size_t i = 0;

  while(state.KeepRunning()) {
    fst.get_in_port("input").set_buffer(b2c::Cargo(42));
    s.step();
  }
}

static void Thread100usec(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());

  b2c::Module m;

  Pipe fst(100);
  m.add_component("c0", fst);

  for(std::size_t i = 1; i < 1000; ++i) {
    Pipe prev = m.get_component<Pipe>("c" + std::to_string(i - 1));
    Pipe next(100);
    b2c::connect(prev, "output", next, "input");
    m.add_component("c" + std::to_string(i), next);
  }

  b2s::VirtualTimeSyncScheduler s(m, size);

  std::size_t i = 0;

  while(state.KeepRunning()) {
    fst.get_in_port("input").set_buffer(b2c::Cargo(42));
    s.step();
  }
}

static void Thread1000usec(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());

  b2c::Module m;

  Pipe fst(1000);
  m.add_component("c0", fst);

  for(std::size_t i = 1; i < 1000; ++i) {
    Pipe prev = m.get_component<Pipe>("c" + std::to_string(i - 1));
    Pipe next(1000);
    b2c::connect(prev, "output", next, "input");
    m.add_component("c" + std::to_string(i), next);
  }

  b2s::VirtualTimeSyncScheduler s(m, size);

  std::size_t i = 0;

  while(state.KeepRunning()) {
    fst.get_in_port("input").set_buffer(b2c::Cargo(42));
    s.step();
  }
}

static void Step32(benchmark::internal::Benchmark* b) {
  for(int i = 1; i <= 32 ; i *= 2) {
    b->Arg(i);
  }
}

BENCHMARK(Thread1usec)->Apply(Step32);
BENCHMARK(Thread10usec)->Apply(Step32);
BENCHMARK(Thread100usec)->Apply(Step32);
BENCHMARK(Thread1000usec)->Apply(Step32);

BENCHMARK_MAIN();
