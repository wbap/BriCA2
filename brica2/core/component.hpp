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

#include <map>
#include <memory>
#include <string>

namespace brica2 {
  namespace core {
    using Dictionary = std::map<std::string, VectorBase>;

    class Component : public Unit {
    public:
      Component(double interval=1.0, double offset=0.0);
      Component(const Component& other);
      Component(Component&& other) noexcept;
      ~Component();
      Component& operator =(const Component& other);
      Component& operator =(Component&& other) noexcept;
      friend void swap(Component& a, Component& b);
      void detatch();
      Component clone() const;
      virtual void make_in_port(std::string key, const VectorBase& init) final;
      virtual void remove_in_port(std::string) final;
      virtual void make_out_port(std::string key, const VectorBase& init) final;
      virtual void remove_out_port(std::string) final;
      VectorBase get_input(std::string key) const;
      void set_input(std::string key, const VectorBase& vector);
      VectorBase get_output(std::string key) const;
      void set_output(std::string key, const VectorBase& vector);
      void input(double time);
      void output(double time);
      void operator ()();
      virtual Dictionary& fire(Dictionary& inputs, Dictionary& outputs);
      void reset();
    private:
      struct impl; std::shared_ptr<impl> self;
    };
  }
}

#endif
