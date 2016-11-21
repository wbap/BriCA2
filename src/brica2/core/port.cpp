/******************************************************************************
 *
 * brica2/core/port.cpp
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

#include "brica2/core/port.hpp"

namespace brica2 {
namespace core {

struct Port::impl {
  std::weak_ptr<impl> target;
  Cargo buffer;
};

Port::Port() : self(std::make_shared<impl>()) {}

Port::Port(Cargo& init) : self(std::make_shared<impl>()) { set_buffer(init); }

Port::Port(const Port& other) : self(other.self) {}

Port::Port(Port&& other) noexcept : self(other.self) { other.self = nullptr; }

Port& Port::operator=(const Port& other)
{
  Port another(other);
  *this = std::move(another);
  return *this;
}

Port& Port::operator=(Port&& other) noexcept
{
  swap(*this, other);
  return *this;
}

void swap(Port& a, Port& b) { std::swap(a.self, b.self); }

Cargo& Port::get_buffer() const
{
  return self->buffer;
}

void Port::set_buffer(const Cargo& value)
{
  self->buffer = value;
}

void Port::connect(const Port& target)
{
  self->target = target.self;
}

void Port::sync()
{
  if(std::shared_ptr<impl> target = self->target.lock()) {
    self->buffer = target->buffer;
  }
}

}
}
