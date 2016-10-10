#include "brica2/core/vector.hpp"
#include "brica2/core/agent.hpp"
#include "brica2/components/const.hpp"
#include "brica2/components/pipe.hpp"
#include "brica2/components/null.hpp"
#include "brica2/components/wait.hpp"
#include "brica2/schedulers/virtual_time_sync_scheduler.hpp"

#include <vector>
#include <cassert>

#include "benchmark/benchmark_api.h"

static void ConstPipeNull(benchmark::State& state) {
  std::size_t size = static_cast<std::size_t>(state.range());

  brica2::core::Vector<int> v0({size});
  brica2::core::Vector<int> v1({size});

  for(std::size_t i = 0; i < size; ++i) {
    v1[i] = static_cast<int>(size);
  }

  brica2::components::Const c0("out", v1);
  brica2::components::Pipe c1("in", "out", v0);
  brica2::components::Null c2("in", v0);

  connect(c0, "out", c1, "in");
  connect(c1, "out", c2, "in");

  brica2::core::Agent agent;
  brica2::schedulers::VirtualTimeSyncScheduler s(agent, 1.0, 1);

  agent.add_component("c0", c0);
  agent.add_component("c1", c1);
  agent.add_component("c2", c2);

  s.update();

  while(state.KeepRunning()) {
    s.step();
  }
}

BENCHMARK(ConstPipeNull)->Range(4, 1 << 20);

BENCHMARK_MAIN();
