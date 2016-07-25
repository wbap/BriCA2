#include <boost/python.hpp>

#include "brica2/core/vector.hpp"
#include "brica2/components/pycomponent.hpp"
#include "brica2/schedulers/virtual_time_sync_scheduler.hpp"

namespace py = boost::python;

int main() {
  Py_Initialize();

  try {
    py::object main = py::import("__main__");
    py::object global(main.attr("__dict__"));
    py::exec("import sys; sys.path.append('examples/python/chainer')", global, global);

    py::object components = py::import("components");
    py::object mnist_object = components.attr("MNISTComponent")(100);
    py::object autoencoder_object = components.attr("AutoencoderComponent")(28 * 28, 1000);
    py::object perceptron_object = components.attr("PerceptronComponent")(1000, 10);

    brica2::components::PyComponent mnist(mnist_object);
    brica2::components::PyComponent autoencoder(autoencoder_object);
    brica2::components::PyComponent perceptron(perceptron_object);

    brica2::core::Vector<float> x({100, 28 * 28});
    brica2::core::Vector<float> h({100, 1000});
    brica2::core::Vector<int>   t({100});
    brica2::core::Vector<float> y({100, 10});
    brica2::core::Vector<float> loss0({1});
    brica2::core::Vector<float> loss1({1});
    brica2::core::Vector<float> accuracy({1});

    mnist.make_out_port("x", x);
    mnist.make_out_port("t", t);

    autoencoder.make_in_port("x", x);
    autoencoder.make_in_port("t", t);

    autoencoder.make_out_port("y", y);
    autoencoder.make_out_port("h", h);
    autoencoder.make_out_port("t", t);
    autoencoder.make_out_port("loss", loss0);

    perceptron.make_in_port("x", x);
    perceptron.make_in_port("t", t);

    perceptron.make_out_port("y", y);
    perceptron.make_out_port("t", t);
    perceptron.make_out_port("loss", loss1);
    perceptron.make_out_port("accuracy", accuracy);

    connect(mnist, "x", autoencoder, "x");
    connect(mnist, "t", autoencoder, "t");
    connect(autoencoder, "h", perceptron, "x");
    connect(autoencoder, "t", perceptron, "t");

    brica2::core::Agent agent;
    brica2::schedulers::VirtualTimeSyncScheduler s(agent);

    agent.add_component("mnist", mnist);
    agent.add_component("autoencoder", autoencoder);
    agent.add_component("perceptron", perceptron);

    s.update();

    for(std::size_t epoch = 0; epoch < 10; ++epoch) {
      float loss0_value = 0;
      float loss1_value = 0;
      float accuracy_value = 0;

      for(std::size_t i = 0; i < 60000 / 100; ++i) {
        s.step();
        loss0 = autoencoder.get_out_port("loss").get_buffer();
        //std::cout << "Loss0:    " << (void*)autoencoder.get_out_port("loss").get_buffer().buffer() << std::endl;
        loss0_value += loss0;
        loss1 = perceptron.get_out_port("loss").get_buffer();
        //std::cout << "Loss1   : " << (void*)perceptron.get_out_port("loss").get_buffer().buffer() << std::endl;
        loss1_value += loss1;
        accuracy = perceptron.get_out_port("accuracy").get_buffer();
        //std::cout << "Accuracy: " << (void*)perceptron.get_out_port("accuracy").get_buffer().buffer() << std::endl;
        accuracy_value += accuracy;
      }

      std::cerr << loss0_value / (60000 / 100) << std::endl;
      std::cerr << loss1_value / (60000 / 100) << std::endl;
      std::cerr << accuracy_value / (60000 / 100) << std::endl;
    }
  } catch(const py::error_already_set&) {
    PyErr_Print();
  }

  Py_Finalize();

  return 0;
}
