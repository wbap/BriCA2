/******************************************************************************
 *
 * tests/brica2/core/vector.cpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/06/28 Kotone Itaya -- Created!
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

namespace brica2 {
  namespace core {
    namespace test {
      TEST(Vector, Simple) {
        shape_t shape0({3});
        std::size_t size0(3);
        std::size_t rank0(1);
        std::size_t length0(3);
        std::string dtype0(getdtype<int>());

        Vector<int> v0(shape0);

        ASSERT_EQ(shape0,  v0.shape());
        ASSERT_EQ(size0,   v0.size());
        ASSERT_EQ(rank0,   v0.rank());
        ASSERT_EQ(length0, v0.length());
        ASSERT_EQ(dtype0,  v0.dtype());
        ASSERT_EQ(0, static_cast<int>(v0[0]));
        ASSERT_EQ(0, static_cast<int>(v0[1]));
        ASSERT_EQ(0, static_cast<int>(v0[2]));
      }
    }

    TEST(Vector, Equality) {
      std::vector<int> u0({1, 2, 3});
      Vector<int> v0(u0, {{3}});
      Vector<int> v1(u0, {{3}});

      ASSERT_TRUE(v0 == v1);
      ASSERT_TRUE(v0 == u0);

      v0[0] = 42;

      ASSERT_TRUE(v0 != v1);
      ASSERT_TRUE(v0 != u0);
    }

    TEST(Vector, Assign) {
      Vector<int> v0({3});

      ASSERT_EQ(0, static_cast<int>(v0[0]));
      ASSERT_EQ(0, static_cast<int>(v0[1]));
      ASSERT_EQ(0, static_cast<int>(v0[2]));

      v0 = {0, 1, 2};

      ASSERT_EQ(0, static_cast<int>(v0[0]));
      ASSERT_EQ(1, static_cast<int>(v0[1]));
      ASSERT_EQ(2, static_cast<int>(v0[2]));
    }

    TEST(Vector, AssignSub) {
      Vector<int> v0({3, 2});

      ASSERT_EQ(0, static_cast<int>(v0[0][0]));
      ASSERT_EQ(0, static_cast<int>(v0[0][1]));
      ASSERT_EQ(0, static_cast<int>(v0[0][2]));

      v0[0] = {0, 1, 2};

      ASSERT_EQ(0, static_cast<int>(v0[0][0]));
      ASSERT_EQ(1, static_cast<int>(v0[0][1]));
      ASSERT_EQ(2, static_cast<int>(v0[0][2]));
    }
  }
}
