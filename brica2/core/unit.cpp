/******************************************************************************
 *
 * brica2/core/unit.cpp
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

#include "brica2/core/unit.hpp"

#include <map>

namespace brica2 {
  namespace core {
    struct Unit::impl {
      std::map<std::string, Port> in_ports;
      std::map<std::string, Port> out_ports;
    };
    
    Unit::Unit() : self(std::make_shared<impl>()) {}
    
    Unit::Unit(const Unit& other) : self(other.self) {}
    
    Unit::Unit(Unit&& other) noexcept : self(other.self) {
      other.self = nullptr;
    }

    Unit::~Unit() {}
    
    Unit& Unit::operator =(const Unit& other) {
      Unit another(other);
      *this = std::move(another);
      return *this;
    }
    
    Unit& Unit::operator =(Unit&& other) noexcept {
      swap(*this, other);
      return *this;
    }
    
    void swap(Unit& a, Unit& b) {
      std::swap(a.self, b.self);
    }

    void detatch(std::pair<std::string, Port> pair) {
      pair.second.detatch();
    }

    void Unit::detatch() {
      std::shared_ptr<impl> other = self;
      self = std::make_shared<impl>();

      for(auto iter = other->in_ports.begin(); iter != other->in_ports.end(); ++iter) {
        std::string key = iter->first;
        Port port = iter->second;
        port.detatch();
        self->in_ports.emplace(std::pair<std::string, Port>(key, port));
      }

      for(auto iter = other->out_ports.begin(); iter != other->out_ports.end(); ++iter) {
        std::string key = iter->first;
        Port port = iter->second;
        port.detatch();
        self->out_ports.emplace(std::pair<std::string, Port>(key, port));
      }
    }

    Unit Unit::clone() const {
      Unit other(*this);
      other.detatch();
      return other;
    }

    void Unit::make_in_port(std::string key, const VectorBase& init) {
      self->in_ports.emplace(std::pair<std::string, Port>(key, Port(init)));
    }

    Port Unit::get_in_port(std::string key) const {
      return self->in_ports.at(key);
    }

    void Unit::set_in_port(std::string key, Port port) {
      self->in_ports.at(key) = port;
    }

    void Unit::remove_in_port(std::string key) {
      self->in_ports.erase(key);
    }

    void Unit::alias_in_port(Unit& from_unit, std::string from_id, std::string to_id) {
      Port from = from_unit.get_in_port(from_id);
      set_in_port(to_id, from);
    }

    void Unit::make_out_port(std::string key, const VectorBase& init) {
      self->out_ports.emplace(std::pair<std::string, Port>(key, Port(init)));
    }

    Port Unit::get_out_port(std::string key) const {
      return self->out_ports.at(key);
    }

    void Unit::set_out_port(std::string key, Port port) {
      self->out_ports.at(key) = port;
    }

    void Unit::remove_out_port(std::string key) {
      self->out_ports.erase(key);
    }

    void Unit::alias_out_port(Unit& from_unit, std::string from_id, std::string to_id) {
      Port from = from_unit.get_out_port(from_id);
      set_out_port(to_id, from);
    }

    void Unit::connect(Unit& from_unit, std::string from_id, std::string to_id) {
      Port from = from_unit.get_out_port(from_id);
      Port to = get_in_port(to_id);
      to.connect(from);
    }

    void connect(Unit& from_unit, std::string from_id, Unit& to_unit, std::string to_id) {
      to_unit.connect(from_unit, from_id, to_id);
    }

    void alias_in_port(Unit& from_unit, std::string from_id, Unit& to_unit, std::string to_id) {
      to_unit.alias_in_port(from_unit, from_id, to_id);
    }

    void alias_out_port(Unit& from_unit, std::string from_id, Unit& to_unit, std::string to_id) {
      to_unit.alias_out_port(from_unit, from_id, to_id);
    }
  }
}
