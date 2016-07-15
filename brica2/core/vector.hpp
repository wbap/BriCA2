/******************************************************************************
 *
 * brica2/core/vector.hpp
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

#ifndef __BRICA2_CORE_VECTOR__
#define __BRICA2_CORE_VECTOR__

#include "brica2/core/types.hpp"
#include "brica2/core/vector_base.hpp"
#include "brica2/core/utils.hpp"

#include <iostream>
#include <exception>
#include <vector>

namespace brica2 {
  namespace core {
    template<typename T>
    class Vector : public VectorBase {
      static constexpr std::size_t type_size = sizeof(T) / sizeof(char);
      template<typename U>
      friend class Vector;
    public:
      Vector(std::vector<T> values, shape_t shape)
        : VectorBase(shape, 0, getdtype<T>())
        , _shape(shape)
        , _size(utils::accumulate(_shape))
        , _rank(_shape.size())
        , _length(_rank ? _shape.front() : 0)
        , _offset(0)
      {
        reallocate(_size * type_size);
        for(std::size_t i = 0; i < _size; ++i) {
          reinterpret_cast<T*>(self->buffer)[i] = values[i];
        }
      }

      Vector(shape_t shape, std::size_t offset=0)
        : VectorBase(shape, offset, getdtype<T>())
        , _shape(shape)
        , _size(utils::accumulate(_shape))
        , _rank(_shape.size())
        , _length(_rank ? _shape.front() : 0)
        , _offset(offset)
      {
        reallocate(_size * type_size);
      }

      Vector(const VectorBase& base)
        : VectorBase(base)
        , _shape(base.shape())
        , _size(utils::accumulate(_shape))
        , _rank(_shape.size())
        , _length(_rank ? _shape.front() : 0)
        , _offset(base.offset())
      {
        if(_size * type_size != self->bytes) {
          throw std::bad_cast();
        }
      }

      Vector(const char* buffer, shape_t shape)
        : VectorBase(buffer, shape, utils::accumulate(_shape) * type_size, getdtype<T>())
        , _shape(shape)
        , _size(utils::accumulate(_shape))
        , _rank(shape.size())
        , _length(_rank ? shape.front() : 0)
        , _offset(0)
      {}

      Vector(const Vector& other)
        : VectorBase(other)
        , _shape(other._shape)
        , _size(other._size)
        , _rank(other._rank)
        , _length(other._length)
        , _offset(other._offset)
      {}

      Vector(Vector&& other) noexcept
        : VectorBase(other)
        , _shape(other._shape)
        , _size(other._size)
        , _rank(other._rank)
        , _length(other._length)
        , _offset(other._offset)
      {}

      Vector(const Vector& other, std::size_t i)
        : Vector(other)
      {
        _shape.pop_front();
        _size = utils::accumulate(_shape);
        _rank = _shape.size();
        _length = _rank ? _shape.front() : 0;
        _offset += i * _size;
      }

      ~Vector() {}

      Vector& operator =(const Vector& other)
      {
        Vector another(other);
        *this = std::move(another);
        return *this;
      }

      Vector& operator =(Vector&& other) noexcept
      {
        swap(*this, other);
        return *this;
      }

      Vector& operator =(const VectorBase& base)
      {
        Vector another(base);
        *this = std::move(another);
        return *this;
      }

      friend void swap(Vector& a, Vector& b)
      {
        swap(dynamic_cast<VectorBase&>(a), dynamic_cast<VectorBase&>(b));
        std::swap(a._shape, b._shape);
        std::swap(a._size, b._size);
        std::swap(a._rank, b._rank);
        std::swap(a._length, b._length);
        std::swap(a._offset, b._offset);
      }

      void operator =(std::vector<T> values) {
        for(std::size_t i = 0; i < _length; ++i) {
          (*this)[i] = values[i];
        }
      }

      Vector operator [](std::size_t i) const
      {
        if(_rank > 0) {
          return Vector(*this, i);
        }
        return Vector(*this);
      }

      void operator =(const T& value)
      {
        reinterpret_cast<T*>(self->buffer)[_offset] = value;
      }

      operator T&()
      {
        return reinterpret_cast<T*>(self->buffer)[_offset];
      }

      Vector reshape(shape_t new_shape)
      {
        std::size_t new_size = utils::accumulate(new_shape);
        if(_size != new_size) {
          throw std::bad_cast();
        }
        _shape = new_shape;
        _rank = _shape.size();
        _length = _rank ? _shape.front() : 0;
        return *this;
      }

      template<typename U>
      Vector<U> astype() const
      {
        Vector<U> other(_shape, _offset);
        for(std::size_t i = 0; i < _size; ++i) {
          reinterpret_cast<U*>(other.self->buffer)[i] = static_cast<U>(reinterpret_cast<T*>(self->buffer)[i]);
        }
        return other;
      }

      void detatch()
      {
        VectorBase::detatch();
      }

      Vector clone() const
      {
        Vector other(*this);
        other.detatch();
        return other;
      }

      std::ostream& format(std::ostream& ostream, std::size_t depth=0, bool first=false) const
      {
        if(!first) {
          for(std::size_t i = 0; i < depth; ++i) {
            ostream << " ";
          }
        }
        ostream << "[";
        for(std::size_t i = 0; i < _length; ++i) {
          if(_rank > 1) {
            (*this)[i].format(ostream, depth + 1, i == 0);
            if(_length - i > 1) {
              for(std::size_t i = 0; i < _rank - 1; ++i) {
                ostream << std::endl;
              }
            }
          } else if(_rank == 1) {
            ostream << static_cast<T>((*this)[i]);
            if(_length - i > 1) {
              ostream << " ";
            }
          }
        }
        return ostream << "]";
      }

      friend std::ostream& operator <<(std::ostream& ostream, const Vector& v)
      {
        return v.format(ostream);
      }

      const shape_t& shape() const
      { return _shape; }

      const std::size_t& size() const
      { return _size; }

      const std::size_t& rank() const
      { return _rank; }

      const std::size_t& length() const
      { return _length; }

      const std::size_t& offset() const
      { return _offset; }

      const T* values() const
      { return reinterpret_cast<T*>(self->buffer); }


      friend bool operator ==(Vector& a, std::vector<T> b) {
        Vector c = Vector(b, a._shape);
        for(std::size_t i = 0; i < a._length; ++i) {
          if(a[i] != b[i]) return false;
        }
        return true;
      }
      friend bool operator !=(Vector& a, std::vector<T> b) { return !(a == b); }
      friend bool operator ==(std::vector<T> a, Vector& b) { return b == a; }
      friend bool operator !=(std::vector<T> a, Vector& b) { return !(a == b); }
    private:
      shape_t _shape;
      std::size_t _size;
      std::size_t _rank;
      std::size_t _length;
      std::size_t _offset;
    };
  }
}

#endif
