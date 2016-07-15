/******************************************************************************
 *
 * tests/brica2/components/pycomponent.cpp
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

#include "gtest/gtest.h"
#include "brica2/components/pycomponent.hpp"
#include "brica2/core/vector.hpp"

namespace brica2 {
  namespace components {
    namespace test {
      TEST(PyComponent, Simple) {
        Py_Initialize();
        try {
          py::object main = py::import("__main__");
          py::object global(main.attr("__dict__"));
          py::exec("import sys; sys.path.append('tests/python')", global, global);

          Vector<int> v0({0, 0, 0}, {{3}});
          Vector<int> v1({1, 2, 3}, {{3}});
          Vector<int> v2({2, 4, 6}, {{3}});

          py::object sample = py::import("sample");
          PyComponent test(sample.attr("Test")());

          test.make_in_port("test", v1);
          test.make_out_port("test", v0);

          ASSERT_TRUE(v1 == Vector<int>(test.get_in_port("test").get_buffer()));
          ASSERT_TRUE(v0 == Vector<int>(test.get_out_port("test").get_buffer()));

          test.input(0.0);
          test();
          test.output(0.0);

          ASSERT_TRUE(v1 == Vector<int>(test.get_in_port("test").get_buffer()));
          ASSERT_TRUE(v2 == Vector<int>(test.get_out_port("test").get_buffer()));

          Vector<int> v3 = test.get_out_port("test").get_buffer();

          test.input(0.0);
          test();
          test.output(0.0);

          ASSERT_TRUE(v1 == Vector<int>(test.get_in_port("test").get_buffer()));
          ASSERT_TRUE(v2 == Vector<int>(test.get_out_port("test").get_buffer()));
          ASSERT_TRUE(v2 == v3);
        } catch(const py::error_already_set&) {
          PyErr_Print();
        }
        Py_Finalize();
      }
    }
  }
}
