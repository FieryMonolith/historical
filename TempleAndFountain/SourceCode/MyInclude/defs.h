/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __DEFS_H__
#define __DEFS_H__

#ifndef WIN32
typedef char          int8;
typedef short         int16;
typedef int           int32;
typedef long long int int64;
#else
typedef __int8        int8;
typedef __int16       int16;
typedef __int32       int32;
typedef __int64       int64;
#endif

#endif
