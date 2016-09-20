/******************************************************************************
 *
 * tests/./brica2/core/thread_pool.cpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/09/18 Kotone Itaya -- Created!
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
#include "./brica2/core/thread_pool.hpp"

#include <chrono>
#include <iostream>

namespace brica2 {
  namespace core {
    namespace test {
      TEST(Thread, Simple) {
        ThreadPool pool(0);

        for(std::size_t i = 0; i < 8; ++i) {
          pool.enqueue([i](){
            std::this_thread::sleep_for(std::chrono::seconds(i));
            std::cout << "Task " << i << std::endl;
          });
        }
      }
    }
  }
}
