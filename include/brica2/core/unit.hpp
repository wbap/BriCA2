/******************************************************************************
 *
 * brica2/core/unit.hpp
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

#ifndef __BRICA2_CORE_UNIT__
#define __BRICA2_CORE_UNIT__

#include "brica2/core/port.hpp"

#include <memory>
#include <string>

namespace brica2 {
namespace core {

class Unit {
public:
  Unit();
  Unit(const Unit&);
  Unit(Unit&&) noexcept;
  virtual ~Unit();
  Unit& operator=(const Unit&);
  Unit& operator=(Unit&&) noexcept;
  friend void swap(Unit&, Unit&);
  virtual void make_in_port(const std::string&);
  virtual void make_in_port(const std::string&, Cargo&);
  virtual Port& get_in_port(const std::string&) const;
  virtual void set_in_port(const std::string&, const Port&);
  virtual void remove_in_port(const std::string&);
  virtual void make_out_port(const std::string&);
  virtual void make_out_port(const std::string&, Cargo&);
  virtual Port& get_out_port(const std::string&) const;
  virtual void set_out_port(const std::string&, const Port&);
  virtual void remove_out_port(const std::string&);
private:
  struct impl; std::shared_ptr<impl> self;
};

void connect(Unit&, const std::string&, Unit&, const std::string&);
void alias_in_port(Unit&, const std::string&, Unit&, const std::string&);
void alias_out_port(Unit&, const std::string&, Unit&, const std::string&);

}
}

#endif

