
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
 *  - add read/write multiple data
 *		+ add Test_read_write_data()
 *      + add FileSys_readData()
 *  	+ add FileSys_writeData()
 * v0.5 2016 Feb. 27
 *  - add FileSys_terminate()
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

//-------------------------------------------------------------------------

static const int kMaxsizeFileSystem_byte = 100; // up to 4096 for ESP-WROOM-02

//-------------------------------------------------------------------------
// low level functions
//
// - init/terminate
void FileSys_init()
{
	EEPROM.begin(kMaxsizeFileSystem_byte);
}
void FileSys_terminate()
{
	EEPROM.end();
}
// - read/write
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
// upper level functions
//

void FileSys_writeData(uint8_t strtadr, uint8_t sz, uint8_t *srcPtr)
{
	if (srcPtr == NULL) {
		return; // error
	}

	for(int idx=0; idx < sz; idx++) {
		FileSys_write_uint8_t( strtadr + idx, srcPtr[idx] );
	}
	FileSys_commit();
}

void FileSys_readData(uint8_t strtadr, uint8_t sz, uint8_t *dstPtr)
{
	if (dstPtr == NULL) {
		return; // error
	}

	for(int idx=0; idx < sz; idx++) {
		dstPtr[idx] = FileSys_read_uint8_t(strtadr + idx);
	}
}

void Test_read_write_data()
{
	debug_outputDebugString("Test_read_write_data", "start");

	uint8_t strtadr = 0; // start address
	uint8_t data[] = { 3, 1, 4, 1, 5, 9, 2 };
	int len = sizeof(data);

	FileSys_writeData(strtadr, len, &data[0]);

	for(int idx=0; idx < len; idx++) {
		data[idx] = 0; // to clear once
		int val = data[idx];
		debug_outputDebugString("Test_read_write_data", "Line121 > val:" + String(data[idx]));
	}

	FileSys_readData(strtadr, len, &data[0]);

	for(int idx=0; idx < len; idx++) {
		int val = data[idx];
		debug_outputDebugString("Test_read_write_data", "Line128 > val:" + String(data[idx]));
	}

}