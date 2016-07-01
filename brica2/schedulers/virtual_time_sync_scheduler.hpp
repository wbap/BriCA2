/******************************************************************************
 *
 * brica2/schedulers/virtual_time_sync_scheduler.hpp
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

#ifndef __BRICA2_SCHEDULERS_VIRTUAL_TIME_SYNC_SCHEDULER__
#define __BRICA2_SCHEDULERS_VIRTUAL_TIME_SYNC_SCHEDULER__

#include "brica2/core/scheduler.hpp"

namespace brica2 {
  namespace schedulers {
    class VirtualTimeSyncScheduler : public core::Scheduler {
    public:
      VirtualTimeSyncScheduler(core::Agent agent, double interval=1.0);
      virtual double step();
    private:
      double interval;
    };
  }
}

#endif
