/******************************************************************************
 *
 * brica2/core/cargo.cpp
 *
 * Copyright (C) 2016 Kotone Itaya
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

#include "brica2/core/cargo.hpp"
#include "utility.hpp"
#include "gtest/gtest.h"

namespace brica2 {
namespace core {

std::size_t Counter::count = 0;

TEST(Cargo, Simple) {
  Something s0;

  {
    Cargo c0(s0);
    Cargo c1 = c0;

    ASSERT_FALSE(c0.empty());
    ASSERT_FALSE(c1.empty());

    ASSERT_FALSE(c0.unique());
    ASSERT_FALSE(c0.unique());

    Something& s1 = c0;
    Something& s2 = c1;

    ASSERT_EQ(s1.answer, 42);
    ASSERT_EQ(s2.answer, 42);
    ASSERT_EQ((void*)(&s1), (void*)(&s2));
  }
}

TEST(Cargo, Counter) {
  ASSERT_EQ(Counter::count, 0);

  {
    Counter i0;
    ASSERT_EQ(Counter::count, 1);

    {
      Cargo c0;
      ASSERT_EQ(Counter::count, 1);
      ASSERT_TRUE(c0.empty());

      {
        Cargo c1(i0);
        ASSERT_EQ(Counter::count, 2);
        ASSERT_FALSE(c1.empty());
        ASSERT_TRUE(c1.unique());

        c0 = c1;
        ASSERT_EQ(Counter::count, 2);
        ASSERT_FALSE(c0.empty());
        ASSERT_FALSE(c0.unique());
        ASSERT_FALSE(c1.unique());
      }

      ASSERT_EQ(Counter::count, 2);
      ASSERT_FALSE(c0.empty());
      ASSERT_TRUE(c0.unique());
    }

    ASSERT_EQ(Counter::count, 1);
  }

  ASSERT_EQ(Counter::count, 0);
}

TEST(Cargo, Pointer) {
  ASSERT_EQ(Counter::count, 0);

  {
    Counter* pi0 = new Counter();
    ASSERT_EQ(Counter::count, 1);

    {
      Cargo c0(pi0);
      ASSERT_EQ(Counter::count, 1);
      ASSERT_TRUE(c0.unique());
    }

    ASSERT_EQ(Counter::count, 0);
  }

  ASSERT_EQ(Counter::count, 0);
}

TEST(Cargo, Accessor) {
  ASSERT_EQ(Counter::count, 0);

  {
    Counter i0;
    ASSERT_EQ(Counter::count, 1);

    {
      Cargo c0;
      ASSERT_TRUE(c0.empty());
      ASSERT_EQ(Counter::count, 1);

      c0.set(i0);
      ASSERT_FALSE(c0.empty());
      ASSERT_TRUE(c0.unique());
      ASSERT_EQ(Counter::count, 2);
    }

    ASSERT_EQ(Counter::count, 1);

    {
      Cargo c0;
      ASSERT_TRUE(c0.empty());
      ASSERT_EQ(Counter::count, 1);

      c0 = i0;
      ASSERT_FALSE(c0.empty());
      ASSERT_TRUE(c0.unique());
      ASSERT_EQ(Counter::count, 2);
    }

    ASSERT_EQ(Counter::count, 1);

    {
      Counter* pi1 = new Counter();
      ASSERT_EQ(Counter::count, 2);

      {
        Cargo c0;
        ASSERT_TRUE(c0.empty());
        ASSERT_EQ(2, Counter::count);

        c0.set(pi1);
        ASSERT_FALSE(c0.empty());
        ASSERT_TRUE(c0.unique());
        ASSERT_EQ(Counter::count, 2);
      }

      ASSERT_EQ(Counter::count, 1);
    }

    ASSERT_EQ(Counter::count, 1);
  }

  ASSERT_EQ(Counter::count, 0);
}

}
}


