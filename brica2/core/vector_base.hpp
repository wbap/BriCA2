/******************************************************************************
 *
 * brica2/core/vector_base.hpp
 *
 * @author Copyright (C) 2016 Kotone Itaya
 * @version 1.0.0
 * @created  2016/06/28 Kotone Itaya -- Created!
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

#ifndef __BRICA2_CORE_VECTOR_BASE__
#define __BRICA2_CORE_VECTOR_BASE__

#include <list>
#include <memory>

namespace brica2 {
  namespace core {
    template<typename T>
    class Vector;

    using shape_t = std::list<std::size_t>;

    class VectorBase {
      template<typename U>
      friend class Vector;
    public:
      VectorBase()
        : self(std::make_shared<impl>())
      {
        self->shape = {};
        self->offset = 0;
        self->owner = true;
      }

      VectorBase(shape_t shape, std::size_t offset)
        : self(std::make_shared<impl>())
      {
        self->shape = shape;
        self->offset = offset;
        self->owner = true;
      }

      VectorBase(const char* buffer, shape_t shape)
        : self(std::make_shared<impl>())
      {
        self->shape = shape;
        self->offset = 0;
        delete[] self->buffer;
        self->buffer = const_cast<char*>(buffer);
        self->owner = false;
      }

      VectorBase(const VectorBase& other)
        : self(other.self) {
      }

      VectorBase(VectorBase&& other) noexcept
        : self(other.self) { other.self = nullptr; }

      ~VectorBase() {}

      VectorBase& operator =(const VectorBase& other)
      {
        VectorBase another(other);
        *this = std::move(another);
        return *this;
      }

      VectorBase& operator =(VectorBase&& other)
      {
        swap(*this, other);
        return *this;
      }

      void reallocate(const std::size_t& bytes)
      {
        if(self->owner) {
          delete[] self->buffer;
          self->bytes = bytes;
          self->buffer = new char[bytes];
          for(std::size_t i = 0; i < bytes; ++i) {
            self->buffer[i] = 0;
          }
        }
      }

      friend void swap(VectorBase& a, VectorBase& b, bool destructive=false)
      {
        if(destructive) {
          std::swap(a.self->buffer, b.self->buffer);
          std::swap(a.self->shape, b.self->shape);
          std::swap(a.self->bytes, b.self->bytes);
          std::swap(a.self->offset, b.self->offset);
          std::swap(a.self->owner, b.self->owner);
        } else {
          std::swap(a.self, b.self);
        }
      }

      char* buffer() const
      { return self->buffer; }

      shape_t& shape() const
      { return self->shape; }

      std::size_t& bytes() const
      { return self->bytes; }

      std::size_t& offset() const
      { return self->offset; }

      void detatch()
      {
        std::shared_ptr<impl> other = self;
        self = std::make_shared<impl>();
        self->shape = other->shape;
        self->bytes = other->bytes;
        self->offset = other->offset;
        self->owner = true;
        reallocate(self->bytes);
        std::memcpy(self->buffer, other->buffer, self->bytes);
      }

      VectorBase clone() const
      {
        VectorBase other(*this);
        other.detatch();
        return other;
      }

      template<typename T>
      operator Vector<T>()
      {
        return Vector<T>(*this);
      }

    private:
      struct impl {
        impl() : buffer(new char[1]), shape({}), bytes(1), offset(0), owner(true) {}
        ~impl() { if(owner) delete[] buffer; }
        char* buffer;
        shape_t shape;
        std::size_t bytes;
        std::size_t offset;
        bool owner;
      }; std::shared_ptr<impl> self;
    };
  }
}

#endif
