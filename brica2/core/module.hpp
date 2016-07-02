/******************************************************************************
 *
 * brica2/core/module.hpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/06/30 Kotone Itaya -- Created!
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

#ifndef __BRICA2_CORE_MODULE__
#define __BRICA2_CORE_MODULE__

#include "brica2/core/unit.hpp"
#include "brica2/core/component.hpp"

#include <list>
#include <memory>

namespace brica2 {
  namespace core {
    class Module : public Unit {
      friend class Agent;
    public:
      Module() : self(std::make_shared<impl>()) {}

      Module(const Module& other) : Unit(other), self(other.self) {}

      Module(Module&& other) noexcept : Unit(other), self(other.self) { other.self = nullptr; }

      ~Module() {}

      Module& operator =(const Module& other) {
        Module another(other);
        *this = std::move(another);
        return *this;
      }

      Module& operator =(Module&& other) noexcept {
        swap(*this, other);
        return *this;
      }

      friend void swap(Module& a, Module& b) {
        swap(dynamic_cast<Unit&>(a), dynamic_cast<Unit&>(b));
        std::swap(a.self, b.self);
      }

      void detatch() {
        std::shared_ptr<impl> other = self;
        self = std::make_shared<impl>();

        for(auto iter = self->components.begin(); iter != self->components.end(); ++iter) {
          std::string key = iter->first;
          Component* component = iter->second;
          component->detatch();
          other->components.emplace(std::pair<std::string, Component*>(key, component));
        }

        for(auto iter = self->submodules.begin(); iter != self->submodules.end(); ++iter) {
          std::string key = iter->first;
          Module submodule = iter->second;
          submodule.detatch();
          other->submodules.emplace(std::pair<std::string, Module>(key, submodule));
        }
      }

      Module clone() const {
        Module other(*this);
        other.detatch();
        return *this;
      }

      template<class C>
      void add_component(std::string key, const C& component) {
        self->components.emplace(std::pair<std::string, Component*>(key, const_cast<Component*>(dynamic_cast<const Component*>(&component))));
      }

      Component& get_component(std::string key) {
        return *(self->components.at(key));
      }

    private:
      std::list<Component*> get_components() const {
        std::list<Component*> components;
        for(auto iter = self->components.begin(); iter != self->components.end(); ++iter) {
          Component* component = iter->second;
          components.push_back(component);
        }
        return components;
      }

    public:
      void remove_component(std::string key) {
        self->components.erase(key);
      }

      void add_submodule(std::string key, const Module& submodule) {
        self->submodules.emplace(std::pair<std::string, Module>(key, submodule));
      }

      Module& get_submodule(std::string key) const {
        return self->submodules.at(key);
      }

    private:
      std::list<Module> get_submodules() const {
        std::list<Module> submodules;
        for(auto iter = self->submodules.begin(); iter != self->submodules.end(); ++iter) {
          std::string key = iter->first;
          Module submodule = iter->second;
          submodules.push_back(submodule);
        }
        return submodules;
      }

    public:
      void remove_submodule(std::string key) {
        self->submodules.erase(key);
      }

    private:
      struct impl {
        std::map<std::string, Component*> components;
        std::map<std::string, Module> submodules;
      }; std::shared_ptr<impl> self;
    };
  }
}

#endif
