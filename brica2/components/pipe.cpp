/******************************************************************************
 *
 * brica2/components/pipe.cpp
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

#include "brica2/components/pipe.hpp"

namespace brica2 {
  namespace components {
    using namespace core;

    Pipe::Pipe(std::string from, std::string to, const VectorBase& init, double interval, double offset)
      : Component(interval, offset), from(from), to(to) {
      make_in_port(from, init.clone());
      make_out_port(to, init.clone());
    }

    Dictionary Pipe::fire(Dictionary& inputs) {
      Dictionary outputs;
      outputs[to] = inputs[from];
      return outputs;
    }
  }
}
