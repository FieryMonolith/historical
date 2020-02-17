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
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

// July 7, 2006 Jason Stredwick.  Moved into separate file.

#ifndef __Functor_Inteface_Exceptions_h__
#define __Functor_Inteface_Exceptions_h__

#include <stdexcept>

namespace nFunctor
{

  class BadCallableEntity : public std::runtime_error
  {
  public:
    BadCallableEntity(void)
    : std::runtime_error("Bad pointer value for nFunctor::CallableEntityType.")
    { return; }

    BadCallableEntity(const char *error)
    : std::runtime_error(error)
    { return; }
  };

}

#endif
