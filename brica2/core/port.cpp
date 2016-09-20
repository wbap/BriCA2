/******************************************************************************
 *
 * brica2/core/port.cpp
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

#include "brica2/core/port.hpp"

#include <mutex>

namespace brica2 {
  namespace core {
    struct Port::impl {
      std::weak_ptr<impl> target;
      VectorBase buffer;
      std::mutex mutex;
    };
    
    Port::Port() : self(std::make_shared<impl>()) {}

    Port::Port(const VectorBase& init) : self(std::make_shared<impl>()) {
      self->buffer = init;
    }
    
    Port::Port(const Port& other) : self(other.self) {}
    
    Port::Port(Port&& other) noexcept : self(other.self) {
      other.self = nullptr;
    }

    Port::~Port() {}
    
    Port& Port::operator =(const Port& other) {
      Port another(other);
      *this = std::move(another);
      return *this;
    }
    
    Port& Port::operator =(Port&& other) noexcept {
      swap(*this, other);
      return *this;
    }
    
    void swap(Port& a, Port& b) {
      std::swap(a.self, b.self);
    }
    
    void Port::detatch() {
      std::shared_ptr<impl> other = self;
      self = std::make_shared<impl>();
      self->buffer = other->buffer.clone();
    }

    Port Port::clone() const {
      Port other(*this);
      other.detatch();
      return other;
    }

    VectorBase& Port::get_buffer() const {
      std::lock_guard<std::mutex> lock(self->mutex);
      return self->buffer;
    }

    void Port::set_buffer(VectorBase& buffer) {
      std::lock_guard<std::mutex> lock(self->mutex);
      self->buffer = buffer;
    }

    void Port::connect(Port target) {
      self->target = target.self;
    }

    bool Port::sync() {
      std::lock_guard<std::mutex> lock(self->mutex);
      if(std::shared_ptr<impl> target = self->target.lock()) {
        self->buffer = target->buffer;
        return true;
      }
      return false;
    }
  }
}
