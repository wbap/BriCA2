/******************************************************************************
 *
 * brica2/core/component.hpp
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

#ifndef __BRICA2_CORE_COMPONENT__
#define __BRICA2_CORE_COMPONENT__

#include "brica2/core/unit.hpp"

#include <unordered_map>
#include <memory>
#include <string>

namespace brica2 {
namespace core {

using Dictionary = std::unordered_map<std::string, Cargo>;

class Component : public Unit {
public:
  Component(const double interval=1.0, const double offset=0.0);
  virtual ~Component();
  const double& interval() const;
  const double& offset() const;
  virtual void make_in_port(const std::string&);
  virtual void make_in_port(const std::string&, Cargo&);
  virtual void remove_in_port(const std::string&);
  virtual void make_out_port(const std::string&);
  virtual void make_out_port(const std::string&, Cargo&);
  virtual void remove_out_port(const std::string&);
  Cargo& get_input(const std::string&) const;
  void set_input(const std::string&, Cargo&);
  Cargo& get_output(const std::string&) const;
  void set_output(const std::string&, Cargo&);
  virtual void input(const double);
  virtual void output(const double);
  virtual Dictionary operator()(Dictionary&)=0;
  virtual void fire();
  void reset();
private:
  struct impl; std::shared_ptr<impl> self;
};

}
}

#endif

