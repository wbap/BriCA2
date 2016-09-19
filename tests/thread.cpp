#include "brica2/core/vector.hpp"
#include "brica2/core/agent.hpp"
#include "brica2/components/const.hpp"
#include "brica2/components/null.hpp"
#include "brica2/components/wait.hpp"
#include "brica2/schedulers/virtual_time_sync_scheduler.hpp"

#include <cassert>

#include "benchmark/benchmark_api.h"

static void WaitSerial(benchmark::State& state) {
  std::size_t time = static_cast<std::size_t>(state.range_x());
  std::size_t N = static_cast<std::size_t>(state.range_y());

  brica2::core::Vector<int> v0({1024});

  brica2::core::Agent agent;
  brica2::schedulers::VirtualTimeSyncScheduler s(agent, 1.0, 1);

  for(std::size_t i = 0; i < N; ++i) {
    brica2::components::Wait ct("in", "out", v0, time);
    agent.add_component("c"+std::to_string(i), ct);
  }

  s.update();

  while(state.KeepRunning()) {
    s.step();
  }
}

static void WaitParallel(benchmark::State& state) {
  std::size_t time = static_cast<std::size_t>(state.range_x());
  std::size_t N = static_cast<std::size_t>(state.range_y());

  brica2::core::Vector<int> v0({1024});

  brica2::core::Agent agent;
  brica2::schedulers::VirtualTimeSyncScheduler s(agent, 1.0);

  for(std::size_t i = 0; i < N; ++i) {
    brica2::components::Wait ct("in", "out", v0, time);
    agent.add_component("c"+std::to_string(i), ct);
  }

  s.update();

  while(state.KeepRunning()) {
    s.step();
  }
}

//BENCHMARK(WaitSerial)->RangePair(1, 1 << 10, 4, 100);
BENCHMARK(WaitParallel)->RangePair(1, 1 << 10, 4, 100);

BENCHMARK_MAIN();
