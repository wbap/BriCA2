/******************************************************************************
 *
 * brica2/core/unit.cpp
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

#include "brica2/core/unit.hpp"

#include <unordered_map>

namespace brica2 {
namespace core {

struct Unit::impl {
  std::unordered_map<std::string, Port> in_ports;
  std::unordered_map<std::string, Port> out_ports;
};

Unit::Unit() : self(std::make_shared<impl>()) {}

Unit::Unit(const Unit& other) : self(other.self) {}

Unit::Unit(Unit&& other) noexcept : self(other.self) { other.self = nullptr; }

Unit::~Unit() {}

Unit& Unit::operator=(const Unit& other)
{
  Unit another(other);
  *this = std::move(another);
  return *this;
}

Unit& Unit::operator=(Unit&& other) noexcept
{
  swap(*this, other);
  return *this;
}

void swap(Unit& a, Unit& b) { std::swap(a.self, b.self); }

void Unit::make_in_port(const std::string& key)
{ self->in_ports.emplace(std::pair<std::string, Port>(key, Port())); }

void Unit::make_in_port(const std::string& key, Cargo& init)
{ self->in_ports.emplace(std::pair<std::string, Port>(key, Port(init))); }

Port& Unit::get_in_port(const std::string& key) const
{ return self->in_ports.at(key); }

void Unit::set_in_port(const std::string& key, const Port& port)
{ self->in_ports.at(key) = port; }

void Unit::remove_in_port(const std::string& key)
{ self->in_ports.erase(key); }

void Unit::make_out_port(const std::string& key)
{ self->out_ports.emplace(std::pair<std::string, Port>(key, Port())); }

void Unit::make_out_port(const std::string& key, Cargo& init)
{ self->out_ports.emplace(std::pair<std::string, Port>(key, Port(init))); }

Port& Unit::get_out_port(const std::string& key) const
{ return self->out_ports.at(key); }

void Unit::set_out_port(const std::string& key, const Port& port)
{ self->out_ports.at(key) = port; }

void Unit::remove_out_port(const std::string& key)
{ self->out_ports.erase(key); }

void connect(Unit& from_unit, const std::string& from_id,
             Unit& to_unit, const std::string& to_id)
{
  Port from = from_unit.get_out_port(from_id);
  Port to = to_unit.get_in_port(to_id);
  to.connect(from);
}

void alias_in_port(Unit& from_unit, const std::string& from_id,
                   Unit& to_unit, const std::string& to_id)
{
  Port from = from_unit.get_in_port(from_id);
  to_unit.set_in_port(to_id, from);
}

void alias_out_port(Unit& from_unit, const std::string& from_id,
                    Unit& to_unit, const std::string& to_id)
{
  Port from = from_unit.get_out_port(from_id);
  to_unit.set_out_port(to_id, from);
}

}
}


