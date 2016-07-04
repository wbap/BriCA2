/******************************************************************************
 *
 * brica2/components/pycomponent.hpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/07/02 Kotone Itaya -- Created!
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

#ifndef __BRICA2_COMPONENTS_PYCOMPONENT__
#define __BRICA2_COMPONENTS_PYCOMPONENT__

#include <boost/python.hpp>

#include "brica2/core/component.hpp"
#include "brica2/core/utils.hpp"

namespace brica2 {
  namespace components {
    namespace py = boost::python;
    using namespace core;

    class PyComponent : public Component {
    public:
      PyComponent(py::object component);
      PyComponent(const PyComponent& other);
      PyComponent(PyComponent&& other) noexcept;
      PyComponent& operator =(const PyComponent& other);
      PyComponent& operator =(PyComponent&& other) noexcept;
      friend void swap(PyComponent& a, PyComponent& b);
      virtual Dictionary fire(Dictionary& inputs);
    private:
      struct impl; std::shared_ptr<impl> self;
    };
  }
}

#endif
