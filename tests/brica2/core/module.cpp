/******************************************************************************
 *
 * tests/brica2/core/module.cpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/06/30 Kotone Itaya -- Created!
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
#include "brica2/core/vector.hpp"
#include "brica2/core/component.hpp"
#include "brica2/core/module.hpp"

namespace brica2 {
  namespace core {
    namespace test {
      TEST(Module, Simple) {
        Module m0;
        Component c0;
        Component c1;
        Vector<int> v0({1, 2, 3}, {{3}});
        Vector<int> v1({1, 2, 3}, {{3}});

        c0.make_in_port("in", v0.clone());
        c0.make_out_port("out", v0.clone());

        v1 = c0.get_in_port("in").get_buffer();

        ASSERT_TRUE(v0 == v1);

        v1 = c0.get_out_port("out").get_buffer();

        ASSERT_TRUE(v0 == v1);

        m0.add_component("c0", c0);

        c1 = m0.get_component("c0");

        v1 = c1.get_in_port("in").get_buffer();

        ASSERT_TRUE(v0 == v1);

        v1 = c1.get_out_port("out").get_buffer();

        ASSERT_TRUE(v0 == v1);
      }

      TEST(Module, Submodule) {
        Module m0;
        Module m1;
        Module m2;
        Component c0;
        Component c1;
        Vector<int> v0({1, 2, 3}, {{3}});
        Vector<int> v1({1, 2, 3}, {{3}});

        c0.make_in_port("in", v0.clone());
        c0.make_out_port("out", v0.clone());

        v1 = c0.get_in_port("in").get_buffer();

        ASSERT_TRUE(v0 == v1);

        v1 = c0.get_out_port("out").get_buffer();

        ASSERT_TRUE(v0 == v1);

        m0.add_submodule("m1", m1);
        m1.add_component("c0", c0);

        m2 = m0.get_submodule("m1");
        c1 = m2.get_component("c0");

        v1 = c1.get_in_port("in").get_buffer();

        ASSERT_TRUE(v0 == v1);

        v1 = c1.get_out_port("out").get_buffer();

        ASSERT_TRUE(v0 == v1);
      }
    }
  }
}
