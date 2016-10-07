/******************************************************************************
 *
 * brica2/core/thread_pool.hpp
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

#ifndef __BRICA2_CORE_THREAD_POOL__
#define __BRICA2_CORE_THREAD_POOL__

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "asio.hpp"

namespace brica2 {
  namespace core {
    class ThreadPool {
      using F = std::function<void()>;
    public:
      ThreadPool(std::size_t size);
      void enqueue(F f);
      bool running();
      void wait();
      ~ThreadPool();
    private:
      asio::io_service io_service;
      std::shared_ptr<asio::io_service::work> work;
      std::vector<std::thread> threads;
      std::mutex mtx;
      std::size_t count;
    };
  }
}

#endif
