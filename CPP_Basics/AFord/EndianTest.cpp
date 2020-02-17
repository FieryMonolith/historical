#include <iostream>
#include <iterator>

#include "Endian.h"


static const std::size_t n = 4U;


int main(void) {
   const int originalValue( 10 );
   int machineValue( originalValue ),
       littleValue( 0 ),
       bigValue( 0 );

   Endian::ToBigEndian( bigValue, originalValue );
   Endian::ToLittleEndian( littleValue, originalValue );

   std::cout << "Original value = " << originalValue << std::endl
             << "Machine endian value = " << machineValue << std::endl
             << "Big endian value = " << bigValue << std::endl
             << "Little endian value = " << littleValue << std::endl
             << "This machine is " << ( originalValue == bigValue ? "big" : "little" ) << " endian." << std::endl;

   Endian::FromBigEndian( machineValue, bigValue );
   std::cout << "\nFrom big endian to machine endian yields " << machineValue << std::endl;

   Endian::FromLittleEndian( machineValue, littleValue );
   std::cout << "From little endian to machine endian yields " << machineValue << std::endl;

   std::cout << "\nTesting arrays..." << std::endl;

   const int originalArray[n] = { 13, 10, 27, 555 };
   int machineArray[n] = { 0 },
       littleArray[n] = { 0 },
       bigArray[n] = { 0 };

   Endian::ToBigEndian( bigArray, originalArray );
   Endian::ToLittleEndian( littleArray, originalArray );

   std::cout << "Original array:  ";
   std::copy( originalArray, originalArray + n,
              std::ostream_iterator< int >( std::cout, "  " ) );
   std::cout << "\nMachine array:  ";
   std::copy( machineArray, machineArray + n,
              std::ostream_iterator< int >( std::cout, "  " ) );
   std::cout << "\nBig array:  ";
   std::copy( bigArray, bigArray + n,
              std::ostream_iterator< int >( std::cout, "  " ) );
   std::cout << "\nLittle array:  ";
   std::copy( littleArray, littleArray + n,
              std::ostream_iterator< int >( std::cout, "  " ) );

   Endian::FromBigEndian( machineArray, bigArray );
   std::cout << "\nFrom big array to machine array yields:  ";
   std::copy( machineArray, machineArray + n,
              std::ostream_iterator< int >( std::cout, "  " ) );

   Endian::FromLittleEndian( machineArray, littleArray );
   std::cout << "\nFrom little array to machine array yields:  ";
   std::copy( machineArray, machineArray + n,
              std::ostream_iterator< int >( std::cout, "  " ) );

   std::cout << std::endl;

  return 0;
}
