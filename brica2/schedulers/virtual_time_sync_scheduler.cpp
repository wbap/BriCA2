/******************************************************************************
 *
 * brica2/schedulers/virtual_time_sync_scheduler.cpp
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

#include "brica2/schedulers/virtual_time_sync_scheduler.hpp"

#include <functional>
#include <thread>
#include <queue>

namespace brica2 {
  namespace schedulers {
    VirtualTimeSyncScheduler::VirtualTimeSyncScheduler(core::Agent agent, double interval, std::size_t threads)
      : Scheduler(agent), interval(interval), threads(threads) {}

    double VirtualTimeSyncScheduler::step() {
      const std::size_t n_components = components.size();

      std::vector<std::queue<std::function<void(void)> > > input_queue(threads);
      std::vector<std::queue<std::function<void(void)> > > output_queue(threads);

      std::queue<std::thread> thread_queue;

      for(std::size_t i = 0; i < n_components; ++i) {
        std::shared_ptr<core::Component> component = components[i];
        if(threads) {
          std::function<void(void)> input = [this, component]() mutable {
            (*component).input(time);
            (*component)();
          };
          input_queue[i%threads].push(input);
        } else {
          (*component).input(time);
          (*component)();
        }
      }

      if(threads) {
        for(std::size_t i = 0; i < threads; ++i) {
          std::queue<std::function<void(void)> > queue = input_queue[i];
          std::function<void(void)> input = [queue]() mutable {
            while(!queue.empty()) {
              queue.front()();
              queue.pop();
            }
          };
          thread_queue.push(std::thread(input));
        }

        while(!thread_queue.empty()) {
          thread_queue.front().join();
          thread_queue.pop();
        }
      }

      time += interval;

      for(std::size_t i = 0; i < n_components; ++i) {
        std::shared_ptr<core::Component> component = components[i];
        if(threads) {
          std::function<void(void)> output = [this, component]() mutable {
            (*component).output(time);
          };
          output_queue[i%threads].push(output);
        } else {
          (*component).output(time);
        }
      }

      if(threads) {
        for(std::size_t i = 0; i < threads; ++i) {
          std::queue<std::function<void(void)> > queue = output_queue[i];
          std::function<void(void)> output = [queue]() mutable {
            while(!queue.empty()) {
              queue.front()();
              queue.pop();
            }
          };
          thread_queue.push(std::thread(output));
        }

        while(!thread_queue.empty()) {
          thread_queue.front().join();
          thread_queue.pop();
        }
      }

      return time;
    }
  }
}
