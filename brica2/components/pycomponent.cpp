/******************************************************************************
 *
 * brica2/components/pycomponent.cpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/07/02 Kotone Itaya -- Created!
 * @@
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 *****************************************************************************/

#include "brica2/components/pycomponent.hpp"
#include <iostream>

namespace brica2 {
  namespace components {
    using namespace core;

    py::object buffer2py(const char* buffer, std::size_t bytes) {
      PyObject* pBuffer = PyByteArray_FromStringAndSize(buffer, bytes);
      return py::object(py::handle<>(pBuffer));
    }

    py::tuple shape2py(shape_t shape) {
      py::list slist;
      for(auto iter = shape.begin(); iter != shape.end(); ++iter) {
        slist.append(py::object(*iter));
      }
      return py::tuple(slist);
    }

    py::object vector2py(VectorBase& vector) {
      py::object buffer = buffer2py(vector.buffer(), vector.bytes());
      py::tuple shape = shape2py(vector.shape());
      py::object numpy = py::import("numpy");
      py::object frombuffer = numpy.attr("frombuffer");
      py::object int32 = numpy.attr("int32");
      return frombuffer(buffer, int32).attr("reshape")(shape);
    }

    VectorBase py2vector(py::object ndarray) {
      py::object buffer = ndarray.attr("tobytes")();
      py::str string = py::extract<py::str>(buffer);
      std::size_t bytes =  py::len(string);
      py::tuple shape = py::extract<py::tuple>(ndarray.attr("shape"));
      char* b = py::extract<char*>(buffer);
      shape_t s;
      for(std::size_t i = 0; i < py::len(shape); ++i) {
        s.push_back(py::extract<std::size_t>(shape[i]));
      }
      return VectorBase(b, s, bytes);
    }

    struct PyComponent::impl {
      py::object component;
      py::object fire;
    };

    PyComponent::PyComponent(py::object component)
      : self(std::make_shared<impl>()) {
      self->component = component;
      self->fire = self->component.attr("fire");
    }
    
    PyComponent::PyComponent(const PyComponent& other) : self(other.self) {}
    
    PyComponent::PyComponent(PyComponent&& other) noexcept : self(other.self) {
      other.self = nullptr;
    }
    
    PyComponent& PyComponent::operator =(const PyComponent& other) {
      PyComponent another(other);
      *this = std::move(another);
      return *this;
    }
    
    PyComponent& PyComponent::operator =(PyComponent&& other) noexcept {
      swap(*this, other);
      return *this;
    }

    void swap(PyComponent& a, PyComponent& b) {
      std::swap(a.self, b.self);
    }

    Dictionary PyComponent::fire(Dictionary& inputs) {
      Dictionary outputs;
      py::dict input_dict;

      for(auto iter = inputs.begin(); iter != inputs.end(); ++iter) {
        std::string key = iter->first;
        VectorBase& vector = iter->second;
        input_dict[key] = vector2py(vector);
      }

      py::dict output_dict = py::extract<py::dict>(self->fire(input_dict));
      py::list output_keys = output_dict.keys();

      for(std::size_t i = 0; i < py::len(output_dict); ++i) {
        std::string key = py::extract<std::string>(output_keys[i]);
        VectorBase vector = py2vector(output_dict[key]);
        outputs.emplace(std::pair<std::string, VectorBase>(key, vector));
      }

      return outputs;
    }
  }
}
