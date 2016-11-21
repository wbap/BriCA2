/******************************************************************************
 *
 * brica2/core/module.hpp
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

#ifndef __BRICA2_CORE_MODULE__
#define __BRICA2_CORE_MODULE__

#include "brica2/core/unit.hpp"
#include "brica2/core/component.hpp"

#include <vector>
#include <memory>

namespace brica2 {
namespace core {

class Module : public Unit {
public:
  Module() : Unit(), self(std::make_shared<impl>()) {}
  
  Module(const Module& other) : Unit(other), self(other.self) {}
  
  Module(Module&& other) noexcept : Unit(other), self(other.self)
  { other.self = nullptr; }

  virtual ~Module() {}
  
  Module& operator=(const Module& other)
  {
    Module another(other);
    *this = std::move(another);
    return *this;
  }
  
  Module& operator=(Module&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }
  
  friend void swap(Module& a, Module& b)
  {
    swap(static_cast<Unit&>(a), static_cast<Unit&>(b));
    std::swap(a.self, b.self);
  }

  template<class C>
  void add_component(const std::string& key, const C& component)
  {
    std::shared_ptr<Component> base
      = std::dynamic_pointer_cast<Component>(std::make_shared<C>(component));
    self->components.emplace(
      std::pair<std::string, std::shared_ptr<Component>>(key, base)
    );
  }

  template<class C>
  C& get_component(const std::string& key) const
  { return static_cast<C&>(*(self->components.at(key))); }

  const std::vector<std::shared_ptr<Component>> get_components() const
    {
      std::vector<std::shared_ptr<Component>> components;
      for(auto iter: self->components) {
        std::shared_ptr<Component> component = iter.second;
        components.push_back(component);
      }
      return components;
    }

  const std::vector<std::shared_ptr<Component>> get_all_components() const
  {
    std::vector<std::shared_ptr<Component>> all_components = get_components();
    std::vector<std::shared_ptr<Module>> submodules = get_all_submodules();
    for(std::shared_ptr<Module> submodule: submodules) {
      std::vector<std::shared_ptr<Component>> components = submodule->get_components();
      for(auto component: components) {
        all_components.push_back(component);
      }
    }
    return all_components;
  }

  void remove_component(const std::string& key)
  { self->components.erase(key); }

  void add_submodule(const std::string& key, const Module& submodule)
  {
    self->submodules.emplace(std::pair<std::string, std::shared_ptr<Module>>(key, std::make_shared<Module>(submodule)));
  }

  Module& get_submodule(const std::string& key) const
  { return *(self->submodules.at(key)); }

  const std::vector<std::shared_ptr<Module>> get_submodules() const
  {
    std::vector<std::shared_ptr<Module>> submodules;
    for(auto iter: self->submodules) {
      std::shared_ptr<Module> submodule = iter.second;
      submodules.push_back(submodule);
    }
    return submodules;
  }

  const std::vector<std::shared_ptr<Module>> get_all_submodules() const
  {
    std::vector<std::shared_ptr<Module>> all_submodules;
    for(auto iter: self->submodules) {
      std::vector<std::shared_ptr<Module>> submodules = iter.second->get_all_submodules();
      for(std::shared_ptr<Module> submodule: submodules) {
        all_submodules.push_back(submodule);
      }
    }
    return all_submodules;
  }

  void remove_submodule(const std::string& key)
  { self->submodules.erase(key); }
private:
  struct impl {
    std::unordered_map<std::string, std::shared_ptr<Component>> components;
    std::unordered_map<std::string, std::shared_ptr<Module>> submodules;
  }; std::shared_ptr<impl> self;
};

}
}

#endif

