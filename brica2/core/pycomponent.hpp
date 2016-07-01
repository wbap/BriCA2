/******************************************************************************
 *
 * brica2/core/pycomponent.hpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/06/29 Kotone Itaya -- Created!
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

#ifndef __BRICA2_CORE_PYCOMPONENT__
#define __BRICA2_CORE_PYCOMPONENT__

#include "brica2/core/unit.hpp"

#include <memory>

namespace brica2 {
  namespace core {
    class PyComponent : public Unit {
    public:
      PyComponent();
      PyComponent(const PyComponent& other);
      PyComponent(PyComponent&& other) noexcept;
      PyComponent& operator =(const PyComponent& other);
      PyComponent& operator =(PyComponent&& other) noexcept;
      friend void swap(PyComponent& a, PyComponent& b);
      void detatch();
      PyComponent clone();
    private:
      struct impl; std::shared_ptr<impl> self;
    };
  }
}

#endif
