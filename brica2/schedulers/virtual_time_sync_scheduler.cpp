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

namespace brica2 {
  namespace schedulers {
    VirtualTimeSyncScheduler::VirtualTimeSyncScheduler(core::Agent agent, double interval, std::size_t threads)
      : Scheduler(agent), interval(interval), threads(threads), pool(threads) {}

    double VirtualTimeSyncScheduler::step() {
      const std::size_t n_components = components.size();

      for(std::size_t i = 0; i < n_components; ++i) {
        std::shared_ptr<core::Component> component = components[i];
        if(threads != 1) {
          pool.enqueue([this, component](){
            component->input(time);
            component->operator()();
          });
        } else {
          component->input(time);
          component->operator()();
        }
      }

      pool.wait();

      time += interval;

      for(std::size_t i = 0; i < n_components; ++i) {
        std::shared_ptr<core::Component> component = components[i];
        if(threads != 1) {
          pool.enqueue([this, component](){
            component->output(time);
          });
        } else {
          component->output(time);
        }
      }

      pool.wait();

      return time;
    }
  }
}
