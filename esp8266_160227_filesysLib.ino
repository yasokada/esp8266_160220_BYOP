
// File System Library

// some problem including <FS.h>, which cause invisibility for other library functions
// instead, start using EEPROM library (2016 Feb. 27)

//#include <FS.h>
#include <EEPROM.h>

/*
File system
Checked on staging 2.1.0-rc1 Arduino IDE 1.6.6 (2016 Feb. 27)
*/

/*
 * v0.4 2016 Feb. 27
 *  - add read/write uint8_t()
 *		+ add Test_read_write_uint8_t()
 *  	+ add FileSys_read_uint8_t()
 *  	+ add FileSys_write_uint8_t()
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

//-------------------------------------------------------------------------
// low level functions

void FileSys_write_uint8_t(uint8_t addr, uint8_t srcval)
{
	EEPROM.write(addr, srcval);
}

void FileSys_commit()
{
	EEPROM.commit();	
}

uint8_t FileSys_read_uint8_t(uint8_t addr)
{
	uint8_t retval = EEPROM.read(addr);
	return retval;	
}

void Test_read_write_uint8_t()
{
	uint8_t wrval = 79;
	uint8_t addr = 0;
	uint8_t rdval;

	// 1. write
	FileSys_write_uint8_t(addr, wrval);
	FileSys_commit();

	// 2. read
	rdval = FileSys_read_uint8_t(addr);

//	debug_outputDebugString("Test_read_write_uint8_t","Line55 > " + String(rdval));
}

//-------------------------------------------------------------------------

