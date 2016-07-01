/******************************************************************************
 *
 * brica2/core/pycomponent.cpp
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

#include "brica2/core/pycomponent.hpp"

namespace brica2 {
  namespace core {
    struct PyComponent::impl {
      
    };
    
    PyComponent::PyComponent() : self(std::make_shared<impl>()) {}
    
    PyComponent::PyComponent(const PyComponent& other) : self(other.self) {}
    
    PyComponent::PyComponent(PyComponent&& other) noexcept : self(other.self) {
      other.self = nullptr;
    }
    
    PyComponent& PyComponent::operator =(const PyComponent& other) {
      PyComponent another(other);
      *this = std::move(another);
      return *this;
    }
    
    PyComponent& PyComponent::operator =(PyComponent&& other) noexcept {
      swap(*this, other);
      return *this;
    }
    
    void swap(PyComponent& a, PyComponent& b) {
      std::swap(a.self, b.self);
    }
    
    PyComponent PyComponent::detatch() {
      std::shared_ptr<impl> other = self;
      self = std::make_shared<impl>();
      
    }
    
    PyComponent PyComponent::clone() {
      
    }
  }
}
