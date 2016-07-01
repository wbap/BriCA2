/******************************************************************************
 *
 * brica2/core/component.cpp
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

#include "brica2/core/component.hpp"

#include <functional>
#include <cassert>

namespace brica2 {
  namespace core {
    struct Component::impl {
      Dictionary inputs;
      Dictionary outputs;
      Dictionary buffer;
      double last_input_time = 0.0;
      double last_output_time = 0.0;
      double offset = 0.0;
      double interval = 1.0;
    };
    
    Component::Component(double interval, double offset) : self(std::make_shared<impl>()) {
      self->interval = interval;
      self->offset = offset;
    }

    Component::Component(const Component& other) : Unit(other), self(other.self) {}
    
    Component::Component(Component&& other) noexcept : Unit(other), self(other.self) {
      other.self = nullptr;
    }

    Component::~Component() {}
    
    Component& Component::operator =(const Component& other) {
      Component another(other);
      *this = std::move(another);
      return *this;
    }
    
    Component& Component::operator =(Component&& other) noexcept {
      swap(*this, other);
      return *this;
    }
    
    void swap(Component& a, Component& b) {
      swap(dynamic_cast<Unit&>(a), dynamic_cast<Unit&>(b));
      std::swap(a.self, b.self);
    }
    
    void Component::detatch() {
      std::shared_ptr<impl> other = self;
      self = std::make_shared<impl>();

      for(auto iter = other->inputs.begin(); iter != other->inputs.end(); ++iter) {
        std::string key = iter->first;
        VectorBase vector = iter->second;
        vector.detatch();
        self->inputs.emplace(std::pair<std::string, VectorBase>(key, vector));
      }

      for(auto iter = other->outputs.begin(); iter != other->outputs.end(); ++iter) {
        std::string key = iter->first;
        VectorBase vector = iter->second;
        vector.detatch();
        self->outputs.emplace(std::pair<std::string, VectorBase>(key, vector));
      }
    }

    Component Component::clone() const {
      Component other(*this);
      other.detatch();
      return other;
    }
    
    void Component::make_in_port(std::string key, const VectorBase& init) {
      Unit::make_in_port(key, init);
      self->inputs.emplace(std::pair<std::string, VectorBase>(key, init));
    }

    void Component::remove_in_port(std::string key) {
      Unit::remove_in_port(key);
      self->inputs.erase(key);
    }

    void Component::make_out_port(std::string key, const VectorBase& init) {
      Unit::make_out_port(key, init);
      self->outputs.emplace(std::pair<std::string, VectorBase>(key, init.clone()));
    }

    void Component::remove_out_port(std::string key) {
      Unit::remove_out_port(key);
      self->outputs.erase(key);
    }

    VectorBase Component::get_input(std::string key) const {
      return self->outputs.at(key);
    }

    void Component::set_input(std::string key, const VectorBase& vector) {
      self->outputs.at(key) = vector;
    }

    VectorBase Component::get_output(std::string key) const {
      return self->outputs.at(key);
    }

    void Component::set_output(std::string key, const VectorBase& vector) {
      self->outputs.at(key) = vector;
    }

    void Component::input(double time) {
      assert(self->last_input_time <= time);
      self->last_input_time = time;
      for(auto iter = self->inputs.begin(); iter != self->inputs.end(); ++iter) {
        std::string key = iter->first;
        VectorBase& vector = iter->second;
        Port port = get_in_port(key);
        port.sync();
        vector = port.get_buffer();
      }
    }

    void Component::output(double time) {
      assert(self->last_output_time <= time);
      self->last_output_time = time;
      for(auto iter = self->outputs.begin(); iter != self->outputs.end(); ++iter) {
        std::string key = iter->first;
        VectorBase& vector = iter->second;
        Port port = get_out_port(key);
        port.swap_buffer(vector);
      }
    }

    void Component::operator ()() {
      self->buffer = fire(self->inputs, self->buffer);
    }

    Dictionary& Component::fire(Dictionary& inputs, Dictionary& outputs) {
      return outputs;
    }

    void Component::reset() {
      self->last_input_time = 0.0;
      self->last_output_time = 0.0;
    }
  }
}
