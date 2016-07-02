/******************************************************************************
 *
 * brica2/core/port.hpp
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

#ifndef __BRICA2_CORE_PORT__
#define __BRICA2_CORE_PORT__

#include "brica2/core/vector_base.hpp"

#include <memory>

namespace brica2 {
  namespace core {
    class Port {
    public:
      Port();
      Port(const VectorBase& init);
      Port(const Port& other);
      Port(Port&& other) noexcept;
      ~Port();
      Port& operator =(const Port& other);
      Port& operator =(Port&& other) noexcept;
      friend void swap(Port& a, Port& b);
      void detatch();
      Port clone() const;
      VectorBase& get_buffer() const;
      void set_buffer(VectorBase& buffer);
      void connect(Port target);
      bool sync();
    private:
      struct impl; std::shared_ptr<impl> self;
    };
  }
}

#endif
