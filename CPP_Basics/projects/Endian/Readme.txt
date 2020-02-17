Endian relates to the order bytes are stored on a machine.  This code works 
with big and little endian only at this point.  It provides fast determination 
of endiannes and helper functions to reverse the bytes for any type.  In order 
to provide additional speed, some specialized reversal methods are given.  
Because these specialized versions are not necessary, they are provided in 
addition include files with additional dependencies.  Use what you need.

The project is broken up into two main sections: the endian functionality and 
a number of tests to determine the working state of the main component.

The endian component is comprised of the Endian.h file that is the core 
functionality of the component.  It provides functions for determining the 
endianness of a machine and generic reversal functions that work for any 
type. It's destructor is protected in order to provide a mechanism for 
extension if anyone chooses to do so.  The next level of functionality is
the specializations given in Endian_Specializations.h.  These provide four 
core variants of the reversal code for signed and unsigned 32 and 64 bit 
integers.  These provide a specific conversion that is faster than that of 
the generic version.  The final layer allows for the use of SIMD extensions 
for reversing bytes in specialized hardware.  These take advantage of the 
SSSE3 instruction, pshufb.  This variant provides a class to wrap a stream 
of four byte units that are then simultaneously processed four at a time.

There are four test programs: basic, specializations, SSSE3 with assembly, 
and SSSE3 with intrinsics.  Information about each test is given in 
comment form within the source file.  Each one tests only their specific 
aspect of this component.  Read carefully as not all tests and functionality 
are available on a given machine.

The component is also broken into a number of directories:
Interface - The header files that provide the explicit functionality of the
            component.
Include   - The header files that provide support and would typically come 
            from other components as a part of a hierarchy.
Tests     - The Tests folder holds the source code for the four test cases.
Bin       - This is an empty folder provided for the Binaries to placed 
            for each test when using gcc.
Objs      - When using gcc, the intermediate files will be placed here.
VS2005    - This folder holds the folders for all the Visual Studio 
            projects.  They include the four test cases and the component. 
            There is also a Solution folder which holds the solution file 
            for the entire component.
Makefile_gcc - This folder contains the make files for each of the four 
               test cases when building with gcc.
license.txt - This contains the license this software falls under.

Note: This component is runtime based, not compile-time based.  Another 
      version is available that is compile-time based that utilizes boost.

