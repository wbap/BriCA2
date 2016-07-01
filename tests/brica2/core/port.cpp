/******************************************************************************
 *
 * tests/brica2/core/port.cpp
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
#include "brica2/core/port.hpp"
#include "brica2/core/vector.hpp"

namespace brica2 {
  namespace core {
    namespace test {
      TEST(Port, Simple) {
        Port p0;
      }

      TEST(Port, Access) {
        Port p0;
        Vector<int> v0({1});
        Vector<int> v1({1});

        v0[0] = 42;

        p0.set_buffer(v0);
        v1 = p0.get_buffer();

        ASSERT_EQ(v0.shape(),  v1.shape());
        ASSERT_EQ(v0.rank(),   v1.rank());
        ASSERT_EQ(v0.size(),   v1.size());
        ASSERT_EQ(v0.length(), v1.length());
        ASSERT_EQ(static_cast<int>(v0[0]), static_cast<int>(v1[0]));
      }

      TEST(Port, Sync) {
        Port p0;
        Port p1;
        Vector<int> v0({1});
        Vector<int> v1({1});

        p1.connect(p0);

        v0[0] = 42;

        p0.set_buffer(v0);
        p1.sync();

        v1 = p1.get_buffer();

        ASSERT_EQ(v0.shape(),  v1.shape());
        ASSERT_EQ(v0.rank(),   v1.rank());
        ASSERT_EQ(v0.size(),   v1.size());
        ASSERT_EQ(v0.length(), v1.length());
        ASSERT_EQ(static_cast<int>(v0[0]), static_cast<int>(v1[0]));
      }

      TEST(Port, Alias) {
        Port p0;
        Port p1;
        Vector<int> v0({1});
        Vector<int> v1({1});

        p0.set_buffer(v0);

        p1 = p0;

        v1 = p1.get_buffer();

        v0[0] = 42;

        ASSERT_EQ(v0.shape(),  v1.shape());
        ASSERT_EQ(v0.rank(),   v1.rank());
        ASSERT_EQ(v0.size(),   v1.size());
        ASSERT_EQ(v0.length(), v1.length());
        ASSERT_EQ(static_cast<int>(v0[0]), static_cast<int>(v1[0]));
      }

      TEST(Port, BufferSwap) {
        Port p0;
        Vector<int> v0({1});
        Vector<int> v1({1});

        v0[0] = 42;

        p0.set_buffer(v0);
        p0.swap_buffer(v1);

        ASSERT_EQ(0,  static_cast<int>(v0[0]));
        ASSERT_EQ(42, static_cast<int>(v1[0]));
      }
    }
  }
}
