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

// Last update: November 22, 2001

#ifndef __Type_Interface_NullType_h__
#define __Type_Interface_NullType_h__

// $Header: /home/jason/cvs/Type/Interface/NullType.h,v 1.1 2006/07/18 19:23:52 jason Exp $

namespace nType
{

////////////////////////////////////////////////////////////////////////////////
// class NullType
// Used as a placeholder for "no type here"
// Useful as an end marker in typelists 
////////////////////////////////////////////////////////////////////////////////

    class NullType {};
    
}   // end namespace nType

////////////////////////////////////////////////////////////////////////////////
// Change log:
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
// November 22, 2001: minor change to support porting to boost
////////////////////////////////////////////////////////////////////////////////

#endif // __Type_Interface_NullType_h__

// $Log: NullType.h,v $
// Revision 1.1  2006/07/18 19:23:52  jason
// First version of Type.
//
// Revision 1.3  2006/01/16 19:05:09  rich_sposato
// Added cvs keywords.
//
