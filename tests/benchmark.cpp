#include "brica2/core/vector.hpp"
#include "brica2/core/agent.hpp"
#include "brica2/components/const.hpp"
#include "brica2/components/pipe.hpp"
#include "brica2/components/null.hpp"
#include "brica2/components/wait.hpp"
#include "brica2/components/pycomponent.hpp"
#include "brica2/schedulers/virtual_time_sync_scheduler.hpp"

#include <vector>
#include <cassert>

#include "benchmark/benchmark_api.h"

namespace py = boost::python;

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

static void ConstPythonNull(benchmark::State& state) {
  try {
    std::size_t size = static_cast<std::size_t>(state.range());

    brica2::core::Vector<int> v0({size});
    brica2::core::Vector<int> v1({size});
    brica2::core::Vector<int> v2({size});

    for(std::size_t i = 0; i < size; ++i) {
      v1[i] = static_cast<int>(size);
      v2[i] = static_cast<int>(size * 2);
    }

    py::object main = py::import("__main__");
    py::object global(main.attr("__dict__"));
    py::exec("import sys; sys.path.append('tests/python')", global, global);

    brica2::components::Const c0("out", v1);

    py::object sample = py::import("sample");
    py::object test = sample.attr("Test")();
    brica2::components::PyComponent c1(test);

    c1.make_in_port("test", v0);
    c1.make_out_port("test", v0);

    brica2::components::Null c2("in", v0);

    connect(c0, "out", c1, "test");
    connect(c1, "test", c2, "in");

    brica2::core::Agent agent;
    brica2::schedulers::VirtualTimeSyncScheduler s(agent, 1.0, 1);

    agent.add_component("c0", c0);
    agent.add_component("c1", c1);
    agent.add_component("c2", c2);

    s.update();

    while(state.KeepRunning()) {
      s.step();
    }
  } catch(const py::error_already_set&) {
    PyErr_Print();
  }
}

static void PythonInput(benchmark::State& state) {
  try {
    std::size_t size = static_cast<std::size_t>(state.range());

    brica2::core::Vector<int> v0({size});
    brica2::core::Vector<int> v1({size});
    brica2::core::Vector<int> v2({size});

    for(std::size_t i = 0; i < size; ++i) {
      v1[i] = static_cast<int>(size);
      v2[i] = static_cast<int>(size * 2);
    }

    py::object main = py::import("__main__");
    py::object global(main.attr("__dict__"));
    py::exec("import sys; sys.path.append('tests/python')", global, global);

    brica2::components::Const c0("out", v1);

    py::object sample = py::import("sample");
    py::object test = sample.attr("Test")();
    brica2::components::PyComponent c1(test);

    c1.make_in_port("test", v0);
    c1.make_out_port("test", v0);

    brica2::components::Null c2("in", v0);

    while(state.KeepRunning()) {
      c0.input(0.0);
      c1.input(0.0);
      c2.input(0.0);
    }
  } catch(const py::error_already_set&) {
    PyErr_Print();
  }
}

static void PythonFire(benchmark::State& state) {
  try {
    std::size_t size = static_cast<std::size_t>(state.range());

    brica2::core::Vector<int> v0({size});
    brica2::core::Vector<int> v1({size});
    brica2::core::Vector<int> v2({size});

    for(std::size_t i = 0; i < size; ++i) {
      v1[i] = static_cast<int>(size);
      v2[i] = static_cast<int>(size * 2);
    }

    py::object main = py::import("__main__");
    py::object global(main.attr("__dict__"));
    py::exec("import sys; sys.path.append('tests/python')", global, global);

    brica2::components::Const c0("out", v1);

    py::object sample = py::import("sample");
    py::object test = sample.attr("Test")();
    brica2::components::PyComponent c1(test);

    c1.make_in_port("test", v0);
    c1.make_out_port("test", v0);

    brica2::components::Null c2("in", v0);

    while(state.KeepRunning()) {
      c0();
      c1();
      c2();
    }
  } catch(const py::error_already_set&) {
    PyErr_Print();
  }
}

static void PythonOutput(benchmark::State& state) {
  try {
    std::size_t size = static_cast<std::size_t>(state.range());

    brica2::core::Vector<int> v0({size});
    brica2::core::Vector<int> v1({size});
    brica2::core::Vector<int> v2({size});

    for(std::size_t i = 0; i < size; ++i) {
      v1[i] = static_cast<int>(size);
      v2[i] = static_cast<int>(size * 2);
    }

    py::object main = py::import("__main__");
    py::object global(main.attr("__dict__"));
    py::exec("import sys; sys.path.append('tests/python')", global, global);

    brica2::components::Const c0("out", v1);

    py::object sample = py::import("sample");
    py::object test = sample.attr("Test")();
    brica2::components::PyComponent c1(test);

    c1.make_in_port("test", v0);
    c1.make_out_port("test", v0);

    brica2::components::Null c2("in", v0);

    while(state.KeepRunning()) {
      c0.output(0.0);
      c1.output(0.0);
      c2.output(0.0);
    }
  } catch(const py::error_already_set&) {
    PyErr_Print();
  }
}

static void PythonInputOutput(benchmark::State& state) {
  try {
    std::size_t size = static_cast<std::size_t>(state.range());

    brica2::core::Vector<int> v0({size});
    brica2::core::Vector<int> v1({size});
    brica2::core::Vector<int> v2({size});

    for(std::size_t i = 0; i < size; ++i) {
      v1[i] = static_cast<int>(size);
      v2[i] = static_cast<int>(size * 2);
    }

    py::object main = py::import("__main__");
    py::object global(main.attr("__dict__"));
    py::exec("import sys; sys.path.append('tests/python')", global, global);

    brica2::components::Const c0("out", v1);

    py::object sample = py::import("sample");
    py::object test = sample.attr("Test")();
    brica2::components::PyComponent c1(test);

    c1.make_in_port("test", v0);
    c1.make_out_port("test", v0);

    brica2::components::Null c2("in", v0);

    while(state.KeepRunning()) {
      c0.input(0.0);
      c1.input(0.0);
      c2.input(0.0);
      c0.output(0.0);
      c1.output(0.0);
      c2.output(0.0);
    }
  } catch(const py::error_already_set&) {
    PyErr_Print();
  }
}

BENCHMARK(ConstPipeNull)->Range(4, 1 << 20);
BENCHMARK(ConstPythonNull)->Range(4, 1 << 20);
BENCHMARK(PythonInput)->Range(4, 1 << 20);
BENCHMARK(PythonFire)->Range(4, 1 << 20);
BENCHMARK(PythonOutput)->Range(4, 1 << 20);
BENCHMARK(PythonInputOutput)->Range(4, 1 << 20);

int main(int argc, char** argv) {
  Py_Initialize();
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  Py_Finalize();
  return EXIT_SUCCESS;
}

