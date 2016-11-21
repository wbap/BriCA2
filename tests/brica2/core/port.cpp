/******************************************************************************
 *
 * brica2/core/port.cpp
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

#include "brica2/core/port.hpp"
#include "gtest/gtest.h"

namespace brica2 {
namespace core {

TEST(Port, Simple) {
  Port p0;
}

TEST(Port, Accessor) {
  Port p0;

  Cargo c0;
  Cargo c1;

  int i0 = 42;

  c0.set(i0);

  p0.set_buffer(c0);
  c1 = p0.get_buffer();

  int& i1 = c0;
  int& i2 = c1;

  ASSERT_EQ(c0.get<int>(), i0);
  ASSERT_EQ(c1.get<int>(), i0);
  ASSERT_EQ(c0.get<int>(), c1.get<int>());
  ASSERT_EQ((void*)(&i1), (void*)(&i2));
}

TEST(Port, Sync) {
  Port p0;
  Port p1;

  Cargo c0;
  Cargo c1;

  int i0 = 42;

  p1.connect(p0);

  c0.set(i0);

  p0.set_buffer(c0);
  p1.sync();
  c1 = p1.get_buffer();

  int& i1 = c0;
  int& i2 = c1;

  ASSERT_EQ(c0.get<int>(), i0);
  ASSERT_EQ(c1.get<int>(), i0);
  ASSERT_EQ(c0.get<int>(), c1.get<int>());
  ASSERT_EQ((void*)(&i1), (void*)(&i2));
}

TEST(Port, Alias) {
  Port p0;
  Port p1;

  Cargo c0;
  Cargo c1;

  int i0 = 42;
  int i1 = 43;

  ASSERT_TRUE(c0.empty());
  ASSERT_TRUE(c1.empty());

  p0.set_buffer(c0);

  p1 = p0;

  c1 = p1.get_buffer();

  c0.set(i0);

  ASSERT_FALSE(c0.empty());
  ASSERT_FALSE(c0.unique());
  ASSERT_FALSE(c1.empty());
  ASSERT_FALSE(c1.unique());
  ASSERT_EQ(c0.use_count(), 3);
  ASSERT_EQ(c1.use_count(), 3);

  int& i2 = c0;
  int& i3 = c1;

  ASSERT_EQ(c0.get<int>(), i0);
  ASSERT_EQ(c1.get<int>(), i0);
  ASSERT_EQ(c0.get<int>(), c1.get<int>());
  ASSERT_EQ((void*)(&i2), (void*)(&i3));

  c0.set(i1);

  int& i4 = c0;
  int& i5 = c1;

  ASSERT_EQ(c0.get<int>(), i1);
  ASSERT_EQ(c1.get<int>(), i1);
  ASSERT_EQ(c0.get<int>(), c1.get<int>());
  ASSERT_EQ((void*)(&i4), (void*)(&i5));
}

}
}


