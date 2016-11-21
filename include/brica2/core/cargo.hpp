/******************************************************************************
 *
 * brica2/core/cargo.hpp
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

#ifndef __BRICA2_CORE_CARGO__
#define __BRICA2_CORE_CARGO__

#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <iostream>

namespace brica2 {
namespace core {

class Cargo {
public:
  template<typename T>
  Cargo(const T& value) : wrapper(std::make_shared<Wrapper>(value)) {}

  template<typename T>
  Cargo(T* const value) : wrapper(std::make_shared<Wrapper>(value)) {}

  Cargo() : wrapper(std::make_shared<Wrapper>()) {}

  Cargo(const Cargo& other) noexcept : wrapper(other.wrapper) {}

  Cargo(Cargo&& other) noexcept : wrapper(other.wrapper)
  { other.wrapper = nullptr; }

  ~Cargo() {}

  Cargo& operator=(const Cargo& other)
  {
    Cargo another(other);
    *this = std::move(another);
    return *this;
  }

  Cargo& operator=(Cargo&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }

  template<typename T>
  void operator=(const T& value) { set(value); }

  template<typename T>
  operator T&() const { return wrapper->get<T>(); }

  template<typename T>
  T& get() const { return wrapper->get<T>(); }

  template<typename T>
  void set(const T& value) { wrapper->set(value); }

  template<typename T>
  void set(T* const value) { wrapper->set(value); }

  friend void swap(Cargo& a, Cargo& b) { std::swap(a.wrapper, b.wrapper); }

  const std::size_t use_count() const { return wrapper.use_count(); }

  const bool unique() const { return wrapper.unique(); }

  const bool empty() const { return wrapper->empty(); }
private:
  class HolderBase {
  public:
    virtual ~HolderBase() {}
    virtual HolderBase* clone() const=0;
  };

  template<typename T>
  class Holder : public HolderBase {
  public:
    Holder(const T& value) : ptr(new T(value)) {}

    Holder(T* const value) : ptr(value) {}

    ~Holder() { delete ptr; }

    virtual HolderBase* clone() const { return new Holder(*ptr); }

    T& get() { if(ptr == nullptr) throw std::bad_cast(); return *ptr; }

    void set(const T& value) { delete ptr; ptr = new T(value); }

    void set(T* const value) { delete ptr; ptr = value; }
  private:
    T* ptr;
  };

  class Wrapper {
  public:
    Wrapper() : holder(nullptr) {}

    template<typename T>
    Wrapper(const T& value) : holder(new Holder<T>(value)) {}

    template<typename T>
    Wrapper(T* const value) : holder(new Holder<T>(value)) {}

    ~Wrapper() { delete holder; }

    template<typename T>
    T& get() const
    {
      if(empty()) throw std::bad_cast();
      return static_cast<Holder<T>*>(holder)->get();
    }

    template<typename T>
    void set(const T& value)
    {
      if(empty())
        holder = static_cast<HolderBase*>(new Holder<T>(value));
      else
        static_cast<Holder<T>*>(holder)->set(value);
    }

    template<typename T>
    void set(T* const value)
    {
      if(empty())
        holder = static_cast<HolderBase*>(new Holder<T>(value));
      else
        static_cast<Holder<T>*>(holder)->set(value);
    }

    const bool empty() const { return holder == nullptr; }
  private:
    HolderBase* holder;
  };

  std::shared_ptr<Wrapper> wrapper;
};

}
}

#endif
