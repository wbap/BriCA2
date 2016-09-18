/******************************************************************************
 *
 * ./brica2/core/thread_pool.cpp
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

#include "./brica2/core/thread_pool.hpp"
#include <iostream>

namespace brica2 {
  namespace core {
    ThreadPool::ThreadPool(std::size_t size) : idol(false), stop(false) {
      if(size < 1) {
        size = std::thread::hardware_concurrency();
      }

      for(std::size_t i = 0; i < size; ++i) {
        F worker = [this, i, size](){
          for(;;) {
            F task;
            {
              std::unique_lock<std::mutex> lock(mutex);
              condition.wait(lock, [this](){return stop || !tasks.empty();});
              if(stop && tasks.empty()) {
                return;
              }
              std::cout << tasks.size() << std::endl;
              task = std::move(tasks.front());
              tasks.pop();
              std::cout << tasks.size() << std::endl;
            }
            {
              std::unique_lock<std::mutex> lock(mutex);
              condition.wait(lock);
              state[i] = true;
            }
            task();
            {
              std::unique_lock<std::mutex> lock(mutex);
              condition.wait(lock);
              state[i] = false;
              idol = false;
              for(std::size_t j = 0; j < size; ++j) {
                idol |= state[j];
              }
            }
          }
        };
        workers.emplace_back(worker);
      }
    }

    void ThreadPool::enqueue(F&& f) {
      {
        std::unique_lock<std::mutex> lock(mutex);
        if(stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.emplace(std::function<void()>(f));
      }
      condition.notify_one();
    }

    void ThreadPool::exhaust() {
      for(;;) {
        if(!idol) {
          break;
        }
      }
    }

    ThreadPool::~ThreadPool() {
      {
        std::unique_lock<std::mutex> lock(mutex);
        stop = true;
      }
      condition.notify_all();
      for(std::thread& worker: workers) {
        worker.join();
      }
    }
  }
}
