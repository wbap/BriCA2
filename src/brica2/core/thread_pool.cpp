/******************************************************************************
 *
 * brica2/core/thread_pool.cpp
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

#include "brica2/core/thread_pool.hpp"

#include <vector>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "asio.hpp"

#include <iostream>

namespace brica2 {
namespace core {

struct ThreadPool::impl {
  asio::io_service io_service;
  std::shared_ptr<asio::io_service::work> work;
  std::vector<std::thread> threads;
  std::mutex mutex;
  std::condition_variable cond;
  std::size_t count;
};

ThreadPool::ThreadPool(std::size_t size) : self(std::make_shared<impl>())
{
  if(size < 1) {
    size = std::thread::hardware_concurrency();
  }

  self->count = 0;

  self->work.reset(new asio::io_service::work(self->io_service));

  for(std::size_t i = 0; i < size; ++i) {
    self->threads.emplace_back([this]{ self->io_service.run(); });
  }
}

ThreadPool::~ThreadPool()
{
  self->work.reset();
  for(std::thread& thread: self->threads) {
    thread.join();
  }
}

void ThreadPool::enqueue(std::function<void()> f)
{
  {
    std::lock_guard<std::mutex> lock(self->mutex);
    self->count += 1;
  }

  self->io_service.post([this, f]{
      f();

      {
        std::lock_guard<std::mutex> lock(self->mutex);
        self->count -= 1;

        if(self->count == 0) {
          self->cond.notify_one();
        }
      }
  });
}

bool ThreadPool::running() const
{
  std::lock_guard<std::mutex> lock(self->mutex);
  return self->count != 0;
}

void ThreadPool::wait() const
{
  while(running());
}

const std::size_t ThreadPool::size() const { return self->threads.size(); }

}
}


