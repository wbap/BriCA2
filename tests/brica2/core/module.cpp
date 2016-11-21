/******************************************************************************
 *
 * brica2/core/module.cpp
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

#include "brica2/core/module.hpp"
#include "gtest/gtest.h"

namespace brica2 {
namespace core {

class MyComponent : public Component {
public:
  Dictionary operator()(Dictionary& inputs) { return Dictionary(); }
};

TEST(Module, Simple) {
  Module m0;

  MyComponent mc0;
  MyComponent mc1;

  Cargo c0;
  Cargo c1;
  Cargo c2;
  Cargo c3;

  c0 = 42;
  c1 = 43;

  mc0.make_in_port("in", c0);
  mc0.make_out_port("out", c1);

  c2 = mc0.get_in_port("in").get_buffer();
  c3 = mc0.get_out_port("out").get_buffer();

  ASSERT_EQ(c0.get<int>(), c2.get<int>());
  ASSERT_EQ(c1.get<int>(), c3.get<int>());

  m0.add_component("mc0", mc0);
  mc1 = m0.get_component<MyComponent>("mc0");

  c2 = mc1.get_in_port("in").get_buffer();
  c3 = mc1.get_out_port("out").get_buffer();

  ASSERT_EQ(c0.get<int>(), c2.get<int>());
  ASSERT_EQ(c1.get<int>(), c3.get<int>());
}

TEST(Module, Submodule) {
  Module m0;
  Module m1;
  Module m2;

  MyComponent mc0;
  MyComponent mc1;

  Cargo c0;
  Cargo c1;
  Cargo c2;
  Cargo c3;

  c0 = 42;
  c1 = 43;

  mc0.make_in_port("in", c0);
  mc0.make_out_port("out", c1);

  c2 = mc0.get_in_port("in").get_buffer();
  c3 = mc0.get_out_port("out").get_buffer();

  ASSERT_EQ(c0.get<int>(), c2.get<int>());
  ASSERT_EQ(c1.get<int>(), c3.get<int>());

  m0.add_submodule("m1", m1);
  m1.add_component("mc0", mc0);

  m2 = m0.get_submodule("m1");
  mc1 = m2.get_component<MyComponent>("mc0");

  c2 = mc1.get_in_port("in").get_buffer();
  c3 = mc1.get_out_port("out").get_buffer();

  ASSERT_EQ(c0.get<int>(), c2.get<int>());
  ASSERT_EQ(c1.get<int>(), c3.get<int>());
}

}
}


