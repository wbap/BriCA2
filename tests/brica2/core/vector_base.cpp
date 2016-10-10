/******************************************************************************
 *
 * tests/brica2/core/vector_base.cpp
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
#include "brica2/core/vector_base.hpp"
#include "brica2/core/vector.hpp"

namespace brica2 {
  namespace core {
    namespace test {
      TEST(VectorBase, Size) {
        shape_t shape0({3});
        Vector<int> v0(shape0);
        VectorBase vb0(v0);
        std::cout << sizeof(vb0) << std::endl;

        shape_t shape1({4});
        Vector<int> v1(shape1);
        VectorBase vb1(v1);
        std::cout << sizeof(vb1) << std::endl;
      }
    }
  }
}
