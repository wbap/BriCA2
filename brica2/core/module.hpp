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
    public:
      Module();
      Module(const Module& other);
      Module(Module&& other) noexcept;
      ~Module();
      Module& operator =(const Module& other);
      Module& operator =(Module&& other) noexcept;
      friend void swap(Module& a, Module& b);
      void detatch();
      Module clone() const;
      void add_component(std::string key, const Component& component);
      Component& get_component(std::string key) const;
      std::list<Component> get_components() const;
      void remove_component(std::string key);
      void add_submodule(std::string key, const Module& submodule);
      Module& get_submodule(std::string key) const;
      std::list<Module> get_submodules() const;
      void remove_submodule(std::string key);
    private:
      struct impl; std::shared_ptr<impl> self;
    };
  }
}

#endif
