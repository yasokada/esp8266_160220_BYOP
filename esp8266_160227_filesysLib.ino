
// File System Library

// TODO: 0c > some problem including <FS.h>, which cause invisibility for other library functions
// instead, use EEPROM library (2016 Feb. 27)

//#include <FS.h>
#include <EEPROM.h>

/*
Fils sytem
Checked on staging 2.1.0-rc1 Arduino IDE 1.6.6 (2016 Feb. 27)
*/

/*
 * v0.3 2016 Feb. 27
 *  - add FileSys_write_uint8_t()
 * v0.2 2016 Feb. 27
 *  - add File_init()
 * v0.1 2016 Feb. 27
 *  - prepare empty file
 */


static const int kMaxsizeFileSystem = 100; // TOOD: 0m > check size (byte?)

void FileSys_init()
{
	EEPROM.begin(kMaxsizeFileSystem);
}

void FileSys_write_uint8_t(uint8_t addr, uint8_t srcval)
{
	EEPROM.write(addr, srcval);
	EEPROM.commit();
}



