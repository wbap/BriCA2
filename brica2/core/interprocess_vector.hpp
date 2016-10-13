/******************************************************************************
 *
 * ./brica2/core/interprocess_vector.hpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/10/11 Kotone Itaya -- Created!
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

#ifndef __BRICA2_CORE_INTERPROCESS_VECTOR__
#define __BRICA2_CORE_INTERPROCESS_VECTOR__

#include "./brica2/core/vector_base.hpp"
#include "./brica2/core/utils.hpp"

#include "boost/interprocess/shared_memory_object.hpp"
#include "boost/interprocess/mapped_region.hpp"
#include "boost/interprocess/sync/interprocess_mutex.hpp"
#include "boost/interprocess/sync/interprocess_condition.hpp"
#include "boost/interprocess/sync/scoped_lock.hpp"

#include <memory>
#include <iostream>

namespace brica2 {
  namespace core {
    namespace bip = boost::interprocess;

    class InterprocessVector : public VectorBase {
    public:
      InterprocessVector(const VectorBase& base) : VectorBase(base.dtype(), false), self(std::make_shared<impl>()) {
        self->name = utils::getAddressString((void*)this);
        bip::shared_memory_object::remove(self->name.c_str());
        self->shm = new bip::shared_memory_object(bip::create_only, self->name.c_str(), bip::read_write);

        self->size =
          sizeof(std::size_t) +                       // Size of shape
          sizeof(std::size_t) * base.shape().size() + // Region for shape (shape)
          sizeof(std::size_t) +                       // Region for offset
          sizeof(std::size_t) +                       // Length of dtype
          sizeof(char) * base.dtype().length() +      // Region for dtype
          sizeof(std::size_t) +                       // Size of buffer (bytes)
          sizeof(char) * base.bytes();                // Region for buffer (buffer)

        self->shm->truncate(self->size);
        self->region = new bip::mapped_region(*(self->shm), bip::read_write);

        char* ptr = static_cast<char*>(self->region->get_address());

        *(reinterpret_cast<std::size_t*>(ptr)) = base.shape().size();
        ptr += sizeof(std::size_t);

        for(std::size_t i = 0; i < base.shape().size(); ++i) {
          *(reinterpret_cast<std::size_t*>(ptr) + i) = base.shape()[i];
          ptr += sizeof(std::size_t);
        }

        *(reinterpret_cast<std::size_t*>(ptr)) = base.offset();
        ptr += sizeof(std::size_t);

        *(reinterpret_cast<std::size_t*>(ptr)) = base.dtype().length();
        ptr += sizeof(std::size_t);

        memcpy(ptr, base.dtype().c_str(), base.dtype().length());
        ptr += sizeof(char) * base.dtype().length();

        *(reinterpret_cast<std::size_t*>(ptr)) = base.bytes();
        ptr += sizeof(std::size_t);

        memcpy(ptr, base.buffer(), base.bytes());
        ptr += sizeof(char) * base.bytes();

        point(static_cast<char*>(self->region->get_address()));
      }

      InterprocessVector(const InterprocessVector& other) : VectorBase(other.dtype(), false), self(std::make_shared<impl>()) {
        self->name = other.name();
        self->size = other.size();
        self->shm = new bip::shared_memory_object(bip::open_only, self->name.c_str(), bip::read_write);
        self->shm->truncate(self->size);
        self->region = new bip::mapped_region(*(self->shm), bip::read_write);
        point(static_cast<char*>(self->region->get_address()));
      }

      VectorBase get() {
        std::size_t size;
        shape_t shape;
        std::size_t offset;
        std::size_t length;
        char* tmp;
        std::string dtype;
        std::size_t bytes;
        char* buffer;

        char* ptr = this->buffer();

        size = *(reinterpret_cast<std::size_t*>(ptr));
        ptr += sizeof(std::size_t);

        for(std::size_t i = 0; i < size; ++i) {
          shape.push_back(*(reinterpret_cast<std::size_t*>(ptr)));
          ptr += sizeof(std::size_t);
        }

        offset = *(reinterpret_cast<std::size_t*>(ptr));
        ptr += sizeof(std::size_t);

        length = *(reinterpret_cast<std::size_t*>(ptr));
        ptr += sizeof(std::size_t);

        tmp = new char[length];
        memcpy(tmp, ptr, length);
        dtype = std::string(tmp);
        delete[] tmp;
        ptr += sizeof(char) * length;

        bytes = *(reinterpret_cast<std::size_t*>(ptr));
        ptr += sizeof(std::size_t);

        buffer = new char[bytes];
        memcpy(buffer, ptr, bytes);
        ptr += sizeof(char) * bytes;

        return VectorBase(buffer, shape, bytes, offset, dtype, true);
      }

      void set(const VectorBase& target) {
        std::size_t size;
        shape_t shape;
        std::size_t offset;
        std::size_t length;
        char* tmp;
        std::string dtype;
        std::size_t bytes;
        char* buffer;

        char* ptr = this->buffer();

        size = *(reinterpret_cast<std::size_t*>(ptr));
        ptr += sizeof(std::size_t);

        for(std::size_t i = 0; i < size; ++i) {
          shape.push_back(*(reinterpret_cast<std::size_t*>(ptr)));
          ptr += sizeof(std::size_t);
        }

        offset = *(reinterpret_cast<std::size_t*>(ptr));
        ptr += sizeof(std::size_t);

        length = *(reinterpret_cast<std::size_t*>(ptr));
        ptr += sizeof(std::size_t);

        tmp = new char[length];
        memcpy(tmp, ptr, length);
        dtype = std::string(tmp);
        delete[] tmp;
        ptr += sizeof(char) * length;

        bytes = *(reinterpret_cast<std::size_t*>(ptr));
        ptr += sizeof(std::size_t);

        assert(shape == target.shape());
        assert(offset == target.offset());
        assert(dtype == target.dtype());
        assert(bytes == target.bytes());

        memcpy(ptr, target.buffer(), bytes);
      }

      const std::string& name() const {
        return self->name;
      }

      const std::size_t& size() const {
        return self->size;
      }
    private:
      struct impl {
        impl() : shm(nullptr), region(nullptr) {}
        ~impl() {
          if(region != nullptr) delete region;
          if(shm != nullptr) delete shm;
        }
        std::string name;
        std::size_t size;
        bip::shared_memory_object* shm;
        bip::mapped_region* region;
      }; std::shared_ptr<impl> self;
    };
  }
}

#endif
