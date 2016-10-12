/******************************************************************************
 *
 * tests/./brica2/core/interprocess_vector.cpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/10/11 Kotone Itaya -- Created!
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
#include "./brica2/core/vector.hpp"
#include "./brica2/core/interprocess_vector.hpp"

namespace brica2 {
  namespace core {
    namespace test {
      TEST(InterprocessVector, Simple) {
        shape_t shape0({3});
        std::size_t size0(3);
        std::size_t rank0(1);
        std::size_t length0(3);
        std::string dtype0(getdtype<int>());

        Vector<int> v0(shape0);
        InterprocessVector iv0(v0);
        Vector<int> v1 = iv0.getCopy();

        ASSERT_EQ(v1.shape(),  v0.shape());
        ASSERT_EQ(v1.size(),   v0.size());
        ASSERT_EQ(v1.rank(),   v0.rank());
        ASSERT_EQ(v1.length(), v0.length());
        ASSERT_EQ(v1.dtype(),  v0.dtype());
        ASSERT_EQ(0, static_cast<int>(v0[0]));
        ASSERT_EQ(0, static_cast<int>(v0[1]));
        ASSERT_EQ(0, static_cast<int>(v0[2]));
      }
    }
  }
}
