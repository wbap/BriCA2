/******************************************************************************
 *
 * tests/brica2/schedulers/virtual_time_sync_scheduler.cpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/07/01 Kotone Itaya -- Created!
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
#include "brica2/schedulers/virtual_time_sync_scheduler.hpp"

#include "brica2/core/vector.hpp"
#include "brica2/core/module.hpp"
#include "brica2/components/const.hpp"
#include "brica2/components/pipe.hpp"
#include "brica2/components/null.hpp"

namespace brica2 {
  namespace schedulers {
    namespace test {
      TEST(VirtualTimeSyncScheduler, Simple) {
        core::Agent agent;
        VirtualTimeSyncScheduler s(agent);

        core::Vector<int> zero({3});
        core::Vector<int> v ({1, 2, 3}, {{3}});

        components::Const c0("out", v);
        components::Pipe c1("in", "out", zero);
        components::Null c2("in", zero);

        connect(c0, "out", c1, "in");
        connect(c1, "out", c2, "in");

        agent.add_component("c0", c0);
        agent.add_component("c1", c1);
        agent.add_component("c2", c2);

        s.update();

        ASSERT_TRUE(zero == core::Vector<int>(c0.get_out_port("out").get_buffer()));
        ASSERT_TRUE(zero == core::Vector<int>(c1.get_in_port("in").get_buffer()));
        ASSERT_TRUE(zero == core::Vector<int>(c1.get_out_port("out").get_buffer()));
        ASSERT_TRUE(zero == core::Vector<int>(c2.get_in_port("in").get_buffer()));

        s.step();

        ASSERT_TRUE(v    == core::Vector<int>(c0.get_out_port("out").get_buffer()));
        ASSERT_TRUE(zero == core::Vector<int>(c1.get_in_port("in").get_buffer()));
        ASSERT_TRUE(zero == core::Vector<int>(c1.get_out_port("out").get_buffer()));
        ASSERT_TRUE(zero == core::Vector<int>(c2.get_in_port("in").get_buffer()));

        s.step();

        ASSERT_TRUE(v    == core::Vector<int>(c0.get_out_port("out").get_buffer()));
        ASSERT_TRUE(v    == core::Vector<int>(c1.get_in_port("in").get_buffer()));
        ASSERT_TRUE(v    == core::Vector<int>(c1.get_out_port("out").get_buffer()));
        ASSERT_TRUE(zero == core::Vector<int>(c2.get_in_port("in").get_buffer()));

        s.step();

        ASSERT_TRUE(v == core::Vector<int>(c0.get_out_port("out").get_buffer()));
        ASSERT_TRUE(v == core::Vector<int>(c1.get_in_port("in").get_buffer()));
        ASSERT_TRUE(v == core::Vector<int>(c1.get_out_port("out").get_buffer()));
        ASSERT_TRUE(v == core::Vector<int>(c2.get_in_port("in").get_buffer()));
      }
    }
  }
}
