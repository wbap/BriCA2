/******************************************************************************
 *
 * brica2/core/unit.hpp
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

#ifndef __BRICA2_CORE_UNIT__
#define __BRICA2_CORE_UNIT__

#include "brica2/core/vector_base.hpp"
#include "brica2/core/port.hpp"

#include <memory>
#include <string>

namespace brica2 {
  namespace core {
    class Unit {
    public:
      Unit();
      Unit(const Unit& other);
      Unit(Unit&& other) noexcept;
      virtual ~Unit();
      Unit& operator =(const Unit& other);
      Unit& operator =(Unit&& other) noexcept;
      friend void swap(Unit& a, Unit& b);
      void detatch();
      Unit clone() const;
      virtual void make_in_port(std::string key, const VectorBase& init);
      virtual Port get_in_port(std::string) const;
      virtual void set_in_port(std::string, Port port);
      virtual void remove_in_port(std::string);
      virtual void alias_in_port(Unit& from_unit, std::string from_id, std::string to_id);
      virtual void make_out_port(std::string key, const VectorBase& init);
      virtual Port get_out_port(std::string) const;
      virtual void set_out_port(std::string, Port port);
      virtual void remove_out_port(std::string);
      virtual void alias_out_port(Unit& from_unit, std::string from_id, std::string to_id);
      void connect(Unit& from_unit, std::string from_id, std::string to_id);
    private:
      struct impl; std::shared_ptr<impl> self;
    };

    void connect(Unit& from_unit, std::string from_id, Unit& to_unit, std::string to_id);
    void alias_in_port(Unit& from_unit, std::string from_id, Unit& to_unit, std::string to_id);
    void alias_out_port(Unit& from_unit, std::string from_id, Unit& to_unit, std::string to_id);
  }
}

#endif
