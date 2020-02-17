/*

 * Copyright 2009 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information in the license.txt file or at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * The code given here deals with aligning data to specific memory 
 *   boundaries.

 */

#ifndef __Alignment_h__
#define __Alignment_h__

// These macros allow for creation of static data that is aligned to a 
//   specified number of bytes.  This information can easily be found 
//   all over the internet including MSDN.  However, the website I drew 
//   these from was 
//
// http://www.gamasutra.com/view/feature/3942/data_alignment_part_1.php?print=1
//
#if defined(_MSC_VER) || defined(VISUAL_STUDIO)
#  define DATA_ALIGN(declaration, alignment) __declspec(align(alignment)) declaration
#elif defined(__GNUC__)
#  define DATA_ALIGN(declaration, alignment) declaration __attribute__ ((aligned (alignment)))
#else
#  error "Unknown version in Alignment.h"
#endif

#endif // __Alignment_h__
