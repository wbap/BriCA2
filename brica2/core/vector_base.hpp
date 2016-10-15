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

#include "brica2/core/types.hpp"
#include "brica2/core/utils.hpp"

#include <list>
#include <memory>
#include <cstring>
#include <climits>

#include <iostream>

namespace brica2 {
  namespace core {
    template<typename T>
    class Vector;

    template<typename T> inline std::string getdtype();

    template<> inline std::string getdtype<char     >() { return "int" + std::to_string(sizeof(char     ) * CHAR_BIT); }
    template<> inline std::string getdtype<short    >() { return "int" + std::to_string(sizeof(short    ) * CHAR_BIT); }
    template<> inline std::string getdtype<int      >() { return "int" + std::to_string(sizeof(int      ) * CHAR_BIT); }
    template<> inline std::string getdtype<long     >() { return "int" + std::to_string(sizeof(long     ) * CHAR_BIT); }
    template<> inline std::string getdtype<long long>() { return "int" + std::to_string(sizeof(long long) * CHAR_BIT); }

    template<> inline std::string getdtype<unsigned char     >() { return "uint" + std::to_string(sizeof(unsigned char     ) * CHAR_BIT); }
    template<> inline std::string getdtype<unsigned short    >() { return "uint" + std::to_string(sizeof(unsigned short    ) * CHAR_BIT); }
    template<> inline std::string getdtype<unsigned int      >() { return "uint" + std::to_string(sizeof(unsigned int      ) * CHAR_BIT); }
    template<> inline std::string getdtype<unsigned long     >() { return "uint" + std::to_string(sizeof(unsigned long     ) * CHAR_BIT); }
    template<> inline std::string getdtype<unsigned long long>() { return "uint" + std::to_string(sizeof(unsigned long long) * CHAR_BIT); }

    template<> inline std::string getdtype<float>() { return "float" + std::to_string(sizeof(float) * CHAR_BIT); }
    template<> inline std::string getdtype<double>() { return "float" + std::to_string(sizeof(double) * CHAR_BIT); }

    class VectorBase {
      template<typename U>
      friend class Vector;
    public:
      VectorBase(std::string dtype=getdtype<int>(), bool owner=true)
        : self(std::make_shared<impl>(dtype))
      {
        self->shape = {};
        self->offset = 0;
        self->owner = owner;
      }

      VectorBase(shape_t shape, std::size_t offset, std::string dtype=getdtype<int>())
        : self(std::make_shared<impl>(dtype))
      {
        self->shape = shape;
        self->offset = offset;
        self->owner = true;
      }

      VectorBase(const char* buffer, shape_t shape, std::size_t bytes, std::size_t offset=0, std::string dtype=getdtype<int>(), bool owner=false)
        : self(std::make_shared<impl>(dtype))
      {
        self->shape = shape;
        self->bytes = bytes;
        self->offset = 0;
        delete[] self->buffer;
        self->buffer = const_cast<char*>(buffer);
        self->owner = owner;
      }

      VectorBase(const VectorBase& other)
        : self(other.self) {}

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

      friend void swap(VectorBase& a, VectorBase& b)
      { std::swap(a.self, b.self); }

      char* buffer() const
      { return self->buffer; }

      shape_t& shape() const
      { return self->shape; }

      std::size_t& bytes() const
      { return self->bytes; }

      std::size_t& offset() const
      { return self->offset; }

      std::string dtype() const
      { return self->dtype; }

    protected:
      void point(char* ptr)
      { self->buffer = static_cast<char*>(ptr); }

    public:
      void detatch()
      {
        std::shared_ptr<impl> other = self;
        self = std::make_shared<impl>(other->dtype);
        self->shape = other->shape;
        self->bytes = other->bytes;
        self->offset = other->offset;
        self->owner = true;
        reallocate(self->bytes);
        memcpy(self->buffer, other->buffer, self->bytes);
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

      friend bool operator ==(VectorBase& a, VectorBase& b) {
        if(a.self->shape != b.self->shape) return false;
        if(a.self->offset != b.self->offset) return false;
        if(a.self->bytes != b.self->bytes) return false;
        for(std::size_t i = 0; i < a.self->bytes; ++i) {
          if(a.self->buffer[i] != b.self->buffer[i]) return false;
        }
        return true;
      }

      friend bool operator !=(VectorBase& a, VectorBase& b) { return !(a == b); }

    private:
      struct impl {
        impl(std::string dtype) : buffer(new char[1]), shape({}), bytes(1), offset(0), owner(true), dtype(dtype) {}
        ~impl() { if(owner) delete[] buffer; }
        char* buffer;
        shape_t shape;
        std::size_t bytes;
        std::size_t offset;
        bool owner;
        std::string dtype;
      }; std::shared_ptr<impl> self;
    };
  }
}

#endif
