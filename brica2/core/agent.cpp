/******************************************************************************
 *
 * brica2/core/agent.cpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/07/01 Kotone Itaya -- Created!
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

#include "brica2/core/agent.hpp"

namespace brica2 {
  namespace core {
    Agent::Agent() {}
    
    Agent::Agent(const Agent& other) : Module(other) {}
    
    Agent::Agent(Agent&& other) noexcept : Module(other) {}
    
    Agent::~Agent() {}

    Agent& Agent::operator =(const Agent& other) {
      Agent another(other);
      *this = std::move(another);
      return *this;
    }
    
    Agent& Agent::operator =(Agent&& other) noexcept {
      swap(*this, other);
      return *this;
    }

    Agent Agent::clone() const {
      Agent other(*this);
      other.detatch();
      return other;
    }

    std::vector<std::shared_ptr<Component> > Agent::get_all_components() const {
      std::vector<std::shared_ptr<Component> > all_components = get_components();
      std::vector<Module> all_submodules = get_submodules();

      while(all_submodules.size()) {
        Module submodule = all_submodules.front();
        all_submodules.erase(all_submodules.begin());
        std::vector<std::shared_ptr<Component> > components = submodule.get_components();
        std::vector<Module> submodules = submodule.get_submodules();
        all_components.insert(all_components.end(), components.begin(), components.end());
        all_submodules.insert(all_submodules.end(), submodules.begin(), submodules.end());
      }

      return all_components;
    }
  }
}
