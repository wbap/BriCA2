/******************************************************************************
 *
 * brica2/core/utils.cpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/06/29 Kotone Itaya -- Created!
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

#include "brica2/core/utils.hpp"

#include <algorithm>
#include <functional>
#include <numeric>

namespace brica2 {
  namespace core {
    namespace utils {
      std::size_t accumulate(std::list<std::size_t> shape) {
        return std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<std::size_t>());
      }

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
    }
  }
}
