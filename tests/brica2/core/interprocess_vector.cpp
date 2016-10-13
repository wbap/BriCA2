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

#include <unistd.h>
#include <sys/wait.h>

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

        v0[0] = 1;
        v0[1] = 2;
        v0[2] = 3;

        InterprocessVector iv0(v0);
        Vector<int> v1 = iv0.get();

        ASSERT_EQ(v1.shape(),  v0.shape());
        ASSERT_EQ(v1.size(),   v0.size());
        ASSERT_EQ(v1.rank(),   v0.rank());
        ASSERT_EQ(v1.length(), v0.length());
        ASSERT_EQ(v1.dtype(),  v0.dtype());
        ASSERT_EQ(static_cast<int>(v1[0]), static_cast<int>(v0[0]));
        ASSERT_EQ(static_cast<int>(v1[1]), static_cast<int>(v0[1]));
        ASSERT_EQ(static_cast<int>(v1[2]), static_cast<int>(v0[2]));
      }

      TEST(InterprocessVector, SharedRead) {
        shape_t shape0({3});
        std::size_t size0(3);
        std::size_t rank0(1);
        std::size_t length0(3);
        std::string dtype0(getdtype<int>());

        Vector<int> v0(shape0);

        v0[0] = 1;
        v0[1] = 2;
        v0[2] = 3;

        InterprocessVector iv0(v0);
        InterprocessVector iv1(iv0);
        Vector<int> v1 = iv1.get();

        ASSERT_EQ(v1.shape(),  v0.shape());
        ASSERT_EQ(v1.size(),   v0.size());
        ASSERT_EQ(v1.rank(),   v0.rank());
        ASSERT_EQ(v1.length(), v0.length());
        ASSERT_EQ(v1.dtype(),  v0.dtype());
        ASSERT_EQ(static_cast<int>(v1[0]), static_cast<int>(v0[0]));
        ASSERT_EQ(static_cast<int>(v1[1]), static_cast<int>(v0[1]));
        ASSERT_EQ(static_cast<int>(v1[2]), static_cast<int>(v0[2]));
      }

      TEST(InterprocessVector, SharedWrite) {
        shape_t shape0({3});
        std::size_t size0(3);
        std::size_t rank0(1);
        std::size_t length0(3);
        std::string dtype0(getdtype<int>());

        Vector<int> v0(shape0);
        Vector<int> v1(shape0);

        v0[0] = 1;
        v0[1] = 2;
        v0[2] = 3;

        v1[0] = 3;
        v1[1] = 2;
        v1[2] = 1;

        InterprocessVector iv0(v0);
        InterprocessVector iv1(iv0);
        Vector<int> v2 = iv1.get();

        ASSERT_EQ(v2.shape(),  v0.shape());
        ASSERT_EQ(v2.size(),   v0.size());
        ASSERT_EQ(v2.rank(),   v0.rank());
        ASSERT_EQ(v2.length(), v0.length());
        ASSERT_EQ(v2.dtype(),  v0.dtype());
        ASSERT_EQ(static_cast<int>(v2[0]), static_cast<int>(v0[0]));
        ASSERT_EQ(static_cast<int>(v2[1]), static_cast<int>(v0[1]));
        ASSERT_EQ(static_cast<int>(v2[2]), static_cast<int>(v0[2]));

        iv1.set(v1);

        Vector<int> v3 = iv0.get();

        ASSERT_EQ(v3.shape(),  v1.shape());
        ASSERT_EQ(v3.size(),   v1.size());
        ASSERT_EQ(v3.rank(),   v1.rank());
        ASSERT_EQ(v3.length(), v1.length());
        ASSERT_EQ(v3.dtype(),  v1.dtype());
        ASSERT_EQ(static_cast<int>(v3[0]), static_cast<int>(v1[0]));
        ASSERT_EQ(static_cast<int>(v3[1]), static_cast<int>(v1[1]));
        ASSERT_EQ(static_cast<int>(v3[2]), static_cast<int>(v1[2]));
      }

      TEST(InterprocessVector, InterprocessRead) {
        shape_t shape0({3});
        std::size_t size0(3);
        std::size_t rank0(1);
        std::size_t length0(3);
        std::string dtype0(getdtype<int>());

        Vector<int> v0(shape0);

        v0[0] = 1;
        v0[1] = 2;
        v0[2] = 3;

        InterprocessVector iv0(v0);

        pid_t pid = fork();

        ASSERT_GE(pid, 0);

        if(pid == 0) {
          InterprocessVector iv1(iv0);
          Vector<int> v1 = iv1.get();

          ASSERT_EQ(v1.shape(),  v0.shape());
          ASSERT_EQ(v1.size(),   v0.size());
          ASSERT_EQ(v1.rank(),   v0.rank());
          ASSERT_EQ(v1.length(), v0.length());
          ASSERT_EQ(v1.dtype(),  v0.dtype());
          ASSERT_EQ(static_cast<int>(v1[0]), static_cast<int>(v0[0]));
          ASSERT_EQ(static_cast<int>(v1[1]), static_cast<int>(v0[1]));
          ASSERT_EQ(static_cast<int>(v1[2]), static_cast<int>(v0[2]));
          exit(0);
        } else {
          int status;
          waitpid(pid, &status, 0);
          ASSERT_EQ(0, status);
        }
      }

      TEST(InterprocessVector, InterprocessWrite) {
        shape_t shape0({3});
        std::size_t size0(3);
        std::size_t rank0(1);
        std::size_t length0(3);
        std::string dtype0(getdtype<int>());

        Vector<int> v0(shape0);
        Vector<int> v1(shape0);

        v0[0] = 1;
        v0[1] = 2;
        v0[2] = 3;

        v1[0] = 3;
        v1[1] = 2;
        v1[2] = 1;

        InterprocessVector iv0(v0);

        pid_t pid = fork();

        ASSERT_GE(pid, 0);

        if(pid == 0) {
          InterprocessVector iv1(iv0);
          Vector<int> v2 = iv1.get();

          ASSERT_EQ(v2.shape(),  v0.shape());
          ASSERT_EQ(v2.size(),   v0.size());
          ASSERT_EQ(v2.rank(),   v0.rank());
          ASSERT_EQ(v2.length(), v0.length());
          ASSERT_EQ(v2.dtype(),  v0.dtype());
          ASSERT_EQ(static_cast<int>(v2[0]), static_cast<int>(v0[0]));
          ASSERT_EQ(static_cast<int>(v2[1]), static_cast<int>(v0[1]));
          ASSERT_EQ(static_cast<int>(v2[2]), static_cast<int>(v0[2]));

          iv1.set(v1);

          exit(0);
        } else {
          int status;
          waitpid(pid, &status, 0);
          ASSERT_EQ(0, status);

          Vector<int> v3 = iv0.get();

          ASSERT_EQ(v3.shape(),  v1.shape());
          ASSERT_EQ(v3.size(),   v1.size());
          ASSERT_EQ(v3.rank(),   v1.rank());
          ASSERT_EQ(v3.length(), v1.length());
          ASSERT_EQ(v3.dtype(),  v1.dtype());
          ASSERT_EQ(static_cast<int>(v3[0]), static_cast<int>(v1[0]));
          ASSERT_EQ(static_cast<int>(v3[1]), static_cast<int>(v1[1]));
          ASSERT_EQ(static_cast<int>(v3[2]), static_cast<int>(v1[2]));
        }
      }
    }
  }
}
