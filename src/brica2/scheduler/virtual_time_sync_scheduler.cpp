/******************************************************************************
 *
 * brica2/scheduler/virtual_time_sync_scheduler.cpp
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

#include "brica2/scheduler/virtual_time_sync_scheduler.hpp"

namespace brica2 {
namespace scheduler {

VirtualTimeSyncScheduler::VirtualTimeSyncScheduler(core::Module& model, std::size_t threads)
  : core::Scheduler(model), pool(threads) {}

double VirtualTimeSyncScheduler::step()
{
  std::size_t size = pool.size();

  for(auto component: components) {
    if(size > 1) {
      pool.enqueue([this, component]{
        component->input(time);
        component->fire();
      });
    } else {
      component->input(time);
      component->fire();
    }
  }

  time += 1.0;

  pool.wait();

  for(auto component: components) {
    if(size > 1) {
      pool.enqueue([this, component]{
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


