///////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
///////////////////////////////////////////////////////////////////////////////

// June 22, 2006 Jason Stredwick.  Created to allow more callable entity types.

#ifndef __Type_Inteface_PtrAccessibleType_h__
#define __Type_Inteface_PtrAccessibleType_h__

/*!
 *
 * \class PtrAccessibleType
 *
 * \ingroup nType
 *
 * PtrAccessibleType- This class takes any type and stores it.  It then gives
 *   access to that data through a pointer called p, and by accessor functions
 *   operator() and Pointer.  A Value function is also available which will
 *   return a copy of the data, ie a value for values, pointer for pointers,
 *   etc.  This class also defines three typedefs Type (original type),
 *   ParameterType (optimized for function passing), and StorageType (the data
 *   pointer).
 *
 * For value parameters, a copy is created that the pointer p then points to.
 *   Changes to this structure will copy the incoming data but the pointer
 *   will not change.
 *
 */

#include "Type/Interface/TypeManip.h"
#include "Type/Interface/TypeTraits.h"

namespace nType
{

  template <typename T> struct PtrAccessibleType;

  // T is a pointer const
  template <typename T> struct PtrAccessibleType<T* const> {
    typedef T* ParameterType;
    typedef T* StorageType;
    typedef T* const Type;

    /*** Data *************/
    StorageType p;

    // Using default copy constructor, assignment, and destructor.
    PtrAccessibleType(ParameterType t) : p(t) { return; }
    ParameterType operator=(ParameterType t) { p = t; return t; }

    const StorageType &Value(void) const { return p; }
          StorageType &Value(void)       { return p; }

    const StorageType &Pointer(void) const { return p; }
          StorageType &Pointer(void)       { return p; }

    const StorageType &operator()(void) const { return p; }
          StorageType &operator()(void)       { return p; }
  };

  // T is a pointer
  template <typename T>
  struct PtrAccessibleType<T*> {
    typedef T* ParameterType;
    typedef T* StorageType;
    typedef T* Type;

    /*** Data *************/
    StorageType p;

    // Using default copy constructor, assignment, and destructor.
    PtrAccessibleType(ParameterType t) : p(t) { return; }
    ParameterType operator=(ParameterType t) { p = t; return t; }

    const StorageType &Value(void) const { return p; }
          StorageType &Value(void)       { return p; }

    const StorageType &Pointer(void) const { return p; }
          StorageType &Pointer(void)       { return p; }

    const StorageType &operator()(void) const { return p; }
          StorageType &operator()(void)       { return p; }
  };

  // T is a reference
  template <typename T> struct PtrAccessibleType<T&> {
    typedef T& ParameterType;
    typedef T* StorageType; // Allows reassignment, wouldn't be if T&
    typedef T& Type;

    /*** Data *************/
    StorageType p;

    // Using default copy constructor, assignment, and destructor
    PtrAccessibleType(ParameterType t) : p(&t) { return; }
    ParameterType operator=(ParameterType t) { p = &t; return t; }

    const StorageType &Value(void) const { return p; }
          StorageType &Value(void)       { return p; }

    const StorageType &Pointer(void) const { return p; }
          StorageType &Pointer(void)       { return p; }

    const StorageType &operator()(void) const { return p; }
          StorageType &operator()(void)       { return p; }
  };

  // T is a copy
  template <typename T> struct PtrAccessibleType {
    typedef typename nType::TypeTraits<T>::ParameterType ParameterType;
    typedef T* StorageAccessType;
    typedef T  StorageType;
    typedef T  Type;

    /*** Data *************/
    StorageAccessType p;
    StorageType c;

    // Using default destructor.  Need to define the rest to ensure
    //   that p always points to this object's c
    PtrAccessibleType(ParameterType t) : p(&c), c(t) { return; }
    PtrAccessibleType(const PtrAccessibleType &t) : p(&c), c(t.c) { return; }
    const PtrAccessibleType &operator=(const PtrAccessibleType &t) {
      if(&t != this) { c = t.c; }
      return *this;
    }
    ParameterType operator=(ParameterType t) { c = t; return t; }

    const StorageType &Value(void) const { return c; }
          StorageType &Value(void)       { return c; }

    const StorageAccessType &Pointer(void) const { return p; }
          StorageAccessType &Pointer(void)       { return p; }

    const StorageAccessType &operator()(void) const { return p; }
          StorageAccessType &operator()(void)       { return p; }
  };

  // Comparison operators
  template <typename T>
  const bool operator==(const PtrAccessibleType<T> &lhs,
                        const PtrAccessibleType<T> &rhs)
  {
    if(lhs.Value() == rhs.Value()) { return true; }
    return false;
  }

  template <typename T>
  const bool operator!=(const PtrAccessibleType<T> &lhs,
                        const PtrAccessibleType<T> &rhs)
  {
    if(lhs.Value() != rhs.Value()) { return true; }
    return false;
  }

} // end namespace nType

#endif // __Type_Interface_PtrAccessibleType_h__
