////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef __Type_Interface_ReassignableType_h__
#define __Type_Interface_ReassignableType_h__

/*!
 *
 * \class ReassignableType
 *
 * \ingroup nType
 *
 * ReassignableType- This class is intended to provide a method of
 *   storing any type of data: value, reference, or pointer and allow it
 *   to be reassignable.  To do this, the reference must be stored as a
 *   pointer.  By requiring a reference as input, it is impossible to pass
 *   in a null value, thus dereferencing the value to retrieve the original
 *   data should never be null.  Thus, an error should only occur for
 *   situations such as dangling pointers, out of scope original reference,
 *   etc.
 *
 */

#include "Type/Interface/TypeTraits.h"

namespace nType
{

  // Forward declaration
  template <typename T> class ReassignableType;

  // Stores reference types as a pointer and define the optimized
  //   version of their type for parameter passing.
  template <typename T> class ReassignableType<T&> {
  public:
    typedef const T& ParameterType;
    typedef T* StorageType;
    typedef T& Type;

  private:
    StorageType d_data;

  public:
    // Using default copy constructor, assignment, and destructor.
    ReassignableType(void) { return; }
    ReassignableType(ParameterType t) : d_data(&t) { return; }
    ParameterType operator=(ParameterType t) { d_data = &t; return t; }

    const T &Value(void) const { return *d_data; }
          T &Value(void)       { return *d_data; }

    const T &operator()(void) const { return *d_data; }
          T &operator()(void)       { return *d_data; }
  };

  // Stores * const types as a pointer and define the optimized
  //   version of their type for parameter passing.
  template <typename T> class ReassignableType<T* const> {
  public:
    typedef const T* const ParameterType;
    typedef T* StorageType;
    typedef T* const Type;

  private:
    StorageType d_data;

  public:
    // Using default copy constructor, assignment, and destructor.
    ReassignableType(void) { return; }
    ReassignableType(ParameterType t) : d_data(&t) { return; }
    ParameterType operator=(ParameterType t) { d_data = t; return t; }

    Type Value(void) const { return d_data; }

    Type operator()(void) const { return d_data; }
  };

  // Stores non-reference types as they are and define the optimized
  //   version of their type for parameter passing.
  template <typename T> class ReassignableType {
  public:
    typedef typename TypeTraits<T>::ParameterType ParameterType;
    typedef T StorageType;
    typedef T Type;

  private:
    StorageType d_data;

  public:
    // Using default copy constructor, assignment, and destructor.
    ReassignableType(void) { return; }
    ReassignableType(ParameterType t) : d_data(t) { return; }
    ParameterType operator=(ParameterType t) { d_data = t; return t; }

    const Type Value(void) const { return d_data; }
          Type Value(void)       { return d_data; }

    const Type operator()(void) const { return d_data; }
          Type operator()(void)       { return d_data; }
  };

  // Comparison operators
  template <typename T>
  const bool operator==(const ReassignableType<T> &lhs,
                        const ReassignableType<T> &rhs)
  {
    if(lhs.Value() == rhs.Value()) { return true; }
    return false;
  }

  template <typename T>
  const bool operator!=(const ReassignableType<T> &lhs,
                        const ReassignableType<T> &rhs)
  {
    if(lhs.Value() != rhs.Value()) { return true; }
    return false;
  }

} // end namespace nType

#endif // __Type_Interface_ReassignableType_h__
