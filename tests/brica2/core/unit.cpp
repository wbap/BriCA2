/******************************************************************************
 *
 * tests/brica2/core/unit.cpp
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
#include "brica2/core/unit.hpp"
#include "brica2/core/vector.hpp"

namespace brica2 {
  namespace core {
    namespace test {
      TEST(Unit, Simple) {
        Unit u0;
      }

      TEST(Unit, Connect) {
        Unit u0;
        Unit u1;
        Vector<int> v0({1});
        Vector<int> v1({1});
        Vector<int> v2({1});
        Vector<int> v3({1});

        v0[0] = 42;

        u0.make_out_port("out", v0);
        u1.make_in_port("in", v1);

        v2 = u0.get_out_port("out").get_buffer();
        v3 = u1.get_in_port("in").get_buffer();

        ASSERT_NE(static_cast<int>(v2[0]), static_cast<int>(v3[0]));

        connect(u0, "out", u1, "in");

        u1.get_in_port("in").sync();

        v2 = u0.get_out_port("out").get_buffer();
        v3 = u1.get_in_port("in").get_buffer();

        ASSERT_EQ(static_cast<int>(v2[0]), static_cast<int>(v3[0]));
      }

      TEST(Unit, Alias) {
        Unit u0;
        Unit u1;

        Vector<int> v0({1});
        Vector<int> v1({1});
        Vector<int> v2({1});
        Vector<int> v3({1});

        v0[0] = 42;

        u0.make_in_port("in", v0);
        u1.make_in_port("in", v1);

        v2 = u0.get_in_port("in").get_buffer();
        v3 = u1.get_in_port("in").get_buffer();

        ASSERT_NE(static_cast<int>(v2[0]), static_cast<int>(v3[0]));

        u0.make_out_port("out", v0);
        u1.make_out_port("out", v1);

        v2 = u0.get_out_port("out").get_buffer();
        v3 = u1.get_out_port("out").get_buffer();

        ASSERT_NE(static_cast<int>(v2[0]), static_cast<int>(v3[0]));

        alias_in_port(u1, "in", u0, "in");
        alias_out_port(u1, "out", u0, "out");

        v2 = u0.get_in_port("in").get_buffer();
        v3 = u1.get_in_port("in").get_buffer();

        ASSERT_EQ(static_cast<int>(v2[0]), static_cast<int>(v3[0]));

        v2 = u0.get_out_port("out").get_buffer();
        v3 = u1.get_out_port("out").get_buffer();

        ASSERT_EQ(static_cast<int>(v2[0]), static_cast<int>(v3[0]));
      }
    }
  }
}
