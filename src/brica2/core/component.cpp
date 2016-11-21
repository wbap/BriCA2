/******************************************************************************
 *
 * brica2/core/component.cpp
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

#include "brica2/core/component.hpp"

#include <cassert>

namespace brica2 {
namespace core {

struct Component::impl {
  double interval;
  double offset;
  double last_input_time;
  double last_output_time;
  Dictionary inputs;
  Dictionary outputs;
};

Component::Component(const double interval, const double offset)
  : Unit(), self(std::make_shared<impl>())
{
  self->interval = interval;
  self->offset = offset;
  self->last_input_time = 0.0;
  self->last_output_time = 0.0;
}

Component::~Component() {}

const double& Component::interval() const { return self->interval; }

const double& Component::offset() const { return self->offset; }

void Component::make_in_port(const std::string& key)
{
  Unit::make_in_port(key);
  self->inputs.emplace(std::pair<std::string, Cargo>(key, Cargo()));
}

void Component::make_in_port(const std::string& key, Cargo& init)
{
  Unit::make_in_port(key, init);
  self->inputs.emplace(std::pair<std::string, Cargo>(key, Cargo()));
}

void Component::remove_in_port(const std::string& key)
{
  Unit::remove_in_port(key);
  self->inputs.erase(key);
}

void Component::make_out_port(const std::string& key)
{
  Unit::make_out_port(key);
  self->outputs.emplace(std::pair<std::string, Cargo>(key, Cargo()));
}

void Component::make_out_port(const std::string& key, Cargo& init)
{
  Unit::make_out_port(key, init);
  self->outputs.emplace(std::pair<std::string, Cargo>(key, Cargo()));
}

void Component::remove_out_port(const std::string& key)
{
  Unit::remove_out_port(key);
  self->inputs.erase(key);
}

Cargo& Component::get_input(const std::string& key) const
{ return self->inputs.at(key); }

void Component::set_input(const std::string& key, Cargo& cargo)
{ self->inputs.at(key) = cargo; }

Cargo& Component::get_output(const std::string& key) const
{ return self->outputs.at(key); }

void Component::set_output(const std::string& key, Cargo& cargo)
{ self->outputs.at(key) = cargo; }

void Component::input(const double time)
{
  assert(self->last_input_time <= time);
  self->last_input_time = time;
  for(auto pair: self->inputs) {
    std::string key = pair.first;
    Port port = get_in_port(key);
    port.sync();
    self->inputs.at(key) = port.get_buffer();
  }
}

void Component::output(const double time)
{
  assert(self->last_output_time <= time);
  self->last_output_time = time;
  for(auto pair: self->outputs) {
    std::string key = pair.first;
    Cargo& cargo = pair.second;
    Port port = get_out_port(key);
    port.set_buffer(cargo);
  }
}

void Component::fire()
{ self->outputs = (*this)(self->inputs); }

void Component::reset()
{
  self->last_input_time = 0.0;
  self->last_output_time = 0.0;
}

}
}


