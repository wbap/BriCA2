/******************************************************************************
 *
 * tests/brica2/core/component.cpp
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

#include "gtest/gtest.h"
#include "brica2/core/component.hpp"
#include "brica2/core/vector.hpp"

namespace brica2 {
  namespace core {
    namespace test {
      TEST(Component, Output) {
        std::vector<int> u0({1, 2, 3});
        std::vector<int> u1({42, 2, 3});
        Vector<int> v0(u0, {{3}});
        Vector<int> v1(u0, {{3}});

        Component c0;

        c0.make_out_port("out", v0);

        v1 = c0.get_out_port("out").get_buffer();

        ASSERT_TRUE(v0 == v1);
        ASSERT_TRUE(v0 == u0);
        ASSERT_TRUE(v1 == u0);

        v1 = c0.get_output("out");

        ASSERT_TRUE(v0 == v1);
        ASSERT_TRUE(v0 == u0);
        ASSERT_TRUE(v1 == u0);

        v0[0] = 42;

        v1 = c0.get_out_port("out").get_buffer();

        ASSERT_TRUE(v0 == v1);
        ASSERT_TRUE(v0 == u1);
        ASSERT_TRUE(v1 == u1);

        v1 = c0.get_output("out");

        ASSERT_TRUE(v0 != v1);
        ASSERT_TRUE(v0 == u1);
        ASSERT_TRUE(v1 == u0);

        c0.output(0.0);

        v1 = c0.get_out_port("out").get_buffer();

        ASSERT_TRUE(v0 == v1);
        ASSERT_TRUE(v0 == u0);
        ASSERT_TRUE(v1 == u0);

        v1 = c0.get_output("out");

        ASSERT_TRUE(v0 != v1);
        ASSERT_TRUE(v0 == u0);
        ASSERT_TRUE(v1 == u1);
      }
    }
  }
}
