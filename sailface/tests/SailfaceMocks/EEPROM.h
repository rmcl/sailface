/* A Mock of the Arduino EEPROM library */


#ifndef __EEPROM_h__
#define __EEPROM_h__

#include <stdint.h>

class EEPROM_ {
  public:
    template< typename T > T &get( int idx, T &t );
    template<typename T> const T &put(int address, const T &t);
};
extern EEPROM_ EEPROM;

#endif
