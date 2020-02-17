/*

 * Copyright 2006 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information at the bottom of this file or the copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __Type_Interface_QuantityUnits_h__
#define __Type_Interface_QuantityUnits_h__

/*

 * This include standardizes the builtin types across the different
 *   operating systems and compilers.  If one is missing, simply add it
 *   and an appropriate typedef.  At a later date, new builtin types
 *   can easily be added regardless of their existence in other systems.
 *   However, if a new type is already defined for another system and 
 *   is then incorporated into another system, please make sure to 
 *   use the same name for compatibility.

 * This include is also meant to standardize the builtin data types 
 *   with a specific name based on the footprint size.  These units
 *   represent quantity size not numbers or types.

 * Jason Stredwick May 29, 2006 Changed the define for Microsoft
 *   Visual C++ from __MSCV__ to _MSC_VER.  I hope this is the last
 *   change to this define and that it always works without having
 *   to actually specify the define to the compiler.

 * Jason Stredwick May 7, 2006
 *   Made byte types lowercase to match with the other units
 *   Changed the define for Microsoft Visual C++ from __TWIN32__ to
 *     __MSVC__.
 *   Added the QuantityUnit class.

 * Jason Stredwick March 27, 2006 Finalized first version of these types

 */

#include <cstring>

namespace nType
{

#ifdef _MSC_VER

    typedef unsigned __int8  byte;
    typedef unsigned __int16 word;
    typedef unsigned __int32 dword;
    typedef unsigned __int64 qword;

    typedef unsigned __int8  uint8;
    typedef unsigned __int16 uint16;
    typedef unsigned __int32 uint32;
    typedef unsigned __int64 uint64;

    typedef   signed __int8  int8;
    typedef   signed __int16 int16;
    typedef   signed __int32 int32;
    typedef   signed __int64 int64;

#else

    typedef unsigned char          byte;
    typedef unsigned short         word;
    typedef unsigned int           dword;
    typedef long long unsigned int qword;

    typedef unsigned char          uint8;
    typedef unsigned short         uint16;
    typedef unsigned int           uint32;
    typedef long long unsigned int uint64;

    typedef signed char            int8;
    typedef signed short           int16;
    typedef signed int             int32;
    typedef long long signed int   int64;

#endif

} // end namespace nType

#endif //__Type_Interface_QuantityUnits_h__

/*

 * Boost Software License - Version 1.0 - August 17th, 2003

 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

 */
