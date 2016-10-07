#include "brica2/core/vector.hpp"
#include "brica2/core/agent.hpp"
#include "brica2/core/component.hpp"
#include "brica2/schedulers/virtual_time_sync_scheduler.hpp"

#include <chrono>
#include <thread>
#include <cassert>

#include "benchmark/benchmark_api.h"

namespace brica2 {
namespace components{
class Work : public core::Component {
public:
  Work(std::string from, std::string to, core::VectorBase& init, std::size_t time) : from(from), to(to), time(time) {
    make_in_port(from, init);
    make_out_port(to, init);
  }

  core::Dictionary fire(core::Dictionary& inputs) {
    core::Dictionary outputs;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    while(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count() < time);
    outputs.emplace(std::pair<std::string, core::VectorBase>(to, inputs[from]));
    return outputs;
  }
private:
  std::string from;
  std::string to;
  std::size_t time;
};
}
}

static void Work10usec(benchmark::State& state) {
  std::size_t N = static_cast<std::size_t>(state.range());

  brica2::core::Vector<int> v0({1024});

  brica2::core::Agent agent;
  brica2::schedulers::VirtualTimeSyncScheduler s(agent, 1.0, N);

  for(std::size_t i = 0; i < 1000; ++i) {
    brica2::components::Work ct("in", "out", v0, 10);
    agent.add_component("c"+std::to_string(i), ct);
  }

  for(std::size_t i = 1; i < 1000; ++i) {
    brica2::components::Work prev = agent.get_component("c" + std::to_string(i-1));
    brica2::components::Work curr = agent.get_component("c" + std::to_string(i));
    brica2::core::connect(prev, "out", curr, "in");
  }

  s.update();

  while(state.KeepRunning()) {
    s.step();
  }
}

static void Work100usec(benchmark::State& state) {
  std::size_t N = static_cast<std::size_t>(state.range());

  brica2::core::Vector<int> v0({1024});

  brica2::core::Agent agent;
  brica2::schedulers::VirtualTimeSyncScheduler s(agent, 1.0, N);

  for(std::size_t i = 0; i < 1000; ++i) {
    brica2::components::Work ct("in", "out", v0, 100);
    agent.add_component("c"+std::to_string(i), ct);
  }

  for(std::size_t i = 1; i < 1000; ++i) {
    brica2::components::Work prev = agent.get_component("c" + std::to_string(i-1));
    brica2::components::Work curr = agent.get_component("c" + std::to_string(i));
    brica2::core::connect(prev, "out", curr, "in");
  }

  s.update();

  while(state.KeepRunning()) {
    s.step();
  }
}

static void Work1000usec(benchmark::State& state) {
  std::size_t N = static_cast<std::size_t>(state.range());

  brica2::core::Vector<int> v0({1024});

  brica2::core::Agent agent;
  brica2::schedulers::VirtualTimeSyncScheduler s(agent, 1.0, N);

  for(std::size_t i = 0; i < 1000; ++i) {
    brica2::components::Work ct("in", "out", v0, 1000);
    agent.add_component("c"+std::to_string(i), ct);
  }

  for(std::size_t i = 1; i < 1000; ++i) {
    brica2::components::Work prev = agent.get_component("c" + std::to_string(i-1));
    brica2::components::Work curr = agent.get_component("c" + std::to_string(i));
    brica2::core::connect(prev, "out", curr, "in");
  }

  s.update();

  while(state.KeepRunning()) {
    s.step();
  }
}

static void Step(benchmark::internal::Benchmark* b) {
  std::size_t M = std::thread::hardware_concurrency();
  std::size_t N = 1;
  while(N < M) {
    b->Arg(N);
    N *= 2;
  }

  b->Arg(M);
}

BENCHMARK(Work10usec)->Apply(Step)->UseRealTime();
BENCHMARK(Work100usec)->Apply(Step)->UseRealTime();
BENCHMARK(Work1000usec)->Apply(Step)->UseRealTime();

BENCHMARK_MAIN();
