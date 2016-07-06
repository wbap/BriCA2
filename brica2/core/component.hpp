/******************************************************************************
 *
 * brica2/core/component.hpp
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

#ifndef __BRICA2_CORE_COMPONENT__
#define __BRICA2_CORE_COMPONENT__

#include "brica2/core/vector_base.hpp"
#include "brica2/core/unit.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <cassert>

namespace brica2 {
  namespace core {
    using Dictionary = std::unordered_map<std::string, VectorBase>;

    class Component : public Unit {
    public:
      Component(double interval=1.0, double offset=0.0) : Unit(), self(std::make_shared<impl>()) {
        self->interval = interval;
        self->offset = offset;
      }

      virtual ~Component() {}

      void detatch() {
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

      virtual void make_in_port(std::string key, const VectorBase& init) final {
        Unit::make_in_port(key, init);
        self->inputs.emplace(std::pair<std::string, VectorBase>(key, init));
      }

      virtual void remove_in_port(std::string key) final {
        Unit::remove_in_port(key);
        self->inputs.erase(key);
      }

      virtual void make_out_port(std::string key, const VectorBase& init) final {
        Unit::make_out_port(key, init);
        self->outputs.emplace(std::pair<std::string, VectorBase>(key, init));
      }

      virtual void remove_out_port(std::string key) final {
        Unit::remove_out_port(key);
      }

      VectorBase get_input(std::string key) const {
        return self->inputs.at(key);
      }

      void set_input(std::string key, const VectorBase& vector) {
        self->inputs.at(key) = vector;
      }

      VectorBase get_output(std::string key) const {
        return self->outputs.at(key);
      }

      void set_output(std::string key, const VectorBase& vector) {
        self->outputs.at(key) = vector;
      }

      virtual void input(double time) {
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

      virtual void output(double time) {
        assert(self->last_output_time <= time);
        self->last_output_time = time;
        for(auto iter = self->outputs.begin(); iter != self->outputs.end(); ++iter) {
          std::string key = iter->first;
          VectorBase& vector = iter->second;
          Port port = get_out_port(key);
          port.set_buffer(vector);
        }
      }

      void operator ()() {
        self->outputs = fire(self->inputs);
      }

      virtual Dictionary fire(Dictionary& inputs)=0;

      void reset() {
        self->last_input_time = 0.0;
        self->last_output_time = 0.0;
      }

      template<class C>
      operator C&() {
        return dynamic_cast<C&>(*this);
      }
    private:
      struct impl {
        Dictionary inputs;
        Dictionary outputs;
        double last_input_time = 0.0;
        double last_output_time = 0.0;
        double offset = 0.0;
        double interval = 1.0;
      }; std::shared_ptr<impl> self;
    };
  }
}

#endif
