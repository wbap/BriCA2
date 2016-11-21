/******************************************************************************
 *
 * brica2/core/port.hpp
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

#ifndef __BRICA2_CORE_PORT__
#define __BRICA2_CORE_PORT__

#include "brica2/core/cargo.hpp"

#include <memory>

namespace brica2 {
namespace core {

class Port {
public:
  Port();
  Port(Cargo&);
  Port(const Port&);
  Port(Port&&) noexcept;
  Port& operator=(const Port&);
  Port& operator=(Port&&) noexcept;
  friend void swap(Port&, Port&);
  Cargo& get_buffer() const;
  void set_buffer(const Cargo&);
  void connect(const Port&);
  void sync();
private:
  struct impl; std::shared_ptr<impl> self;
};

}
}

#endif

