/******************************************************************************
 *
 * brica2/core/unit.cpp
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

#include "brica2/core/unit.hpp"
#include "gtest/gtest.h"

namespace brica2 {
namespace core {

TEST(Unit, Simple) {
  Unit u0;
}

TEST(Unit, Connect) {
  Unit u0;
  Unit u1;

  Cargo c0;
  Cargo c1;
  Cargo c2;

  c0 = 42;

  u0.make_out_port("out", c0);
  u1.make_in_port("in");

  c1 = u0.get_out_port("out").get_buffer();
  c2 = u1.get_in_port("in").get_buffer();

  ASSERT_EQ(c0.get<int>(), c1.get<int>());

  ASSERT_TRUE(c2.empty());

  connect(u0, "out", u1, "in");

  u1.get_in_port("in").sync();

  c1 = u0.get_out_port("out").get_buffer();
  c2 = u1.get_in_port("in").get_buffer();

  ASSERT_EQ(c1.get<int>(), c2.get<int>());
}

TEST(Unit, Alias) {
  Unit u0;
  Unit u1;

  Cargo c0;
  Cargo c1;
  Cargo c2;

  c0 = 42;

  u0.make_in_port("in", c0);
  u1.make_in_port("in");

  c1 = u0.get_in_port("in").get_buffer();
  c2 = u1.get_in_port("in").get_buffer();

  ASSERT_EQ(c0.get<int>(), c1.get<int>());
  ASSERT_TRUE(c2.empty());

  u0.make_out_port("out", c0);
  u1.make_out_port("out");

  c1 = u0.get_out_port("out").get_buffer();
  c2 = u1.get_out_port("out").get_buffer();

  ASSERT_EQ(c0.get<int>(), c1.get<int>());
  ASSERT_TRUE(c2.empty());

  alias_in_port(u0, "in", u1, "in");
  alias_out_port(u0, "out", u1, "out");

  c1 = u0.get_in_port("in").get_buffer();
  c2 = u1.get_in_port("in").get_buffer();

  ASSERT_EQ(c0.get<int>(), c1.get<int>());
  ASSERT_EQ(c0.get<int>(), c2.get<int>());

  c1 = u0.get_out_port("out").get_buffer();
  c2 = u1.get_out_port("out").get_buffer();

  ASSERT_EQ(c0.get<int>(), c1.get<int>());
  ASSERT_EQ(c0.get<int>(), c2.get<int>());
}

}
}


