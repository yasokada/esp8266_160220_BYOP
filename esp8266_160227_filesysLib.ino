
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
 * v0.7 2016 Feb. 27
 *  - add read/write String
 *  	+ add Test_read_write_String()
 *  	+ add FileSys_readString()
 *  	+ add FileSys_writeString()
 * v0.6 2016 Feb. 27
 *  - add read/write multiple data
 *  	+ add Test_read_write_data()
 *      + add FileSys_readData()
 *  	+ add FileSys_writeData()
 * v0.5 2016 Feb. 27
 *  - add FileSys_terminate()
 * v0.4 2016 Feb. 27
 *  - add read/write uint8_t()
 *  	+ add Test_read_write_uint8_t()
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

//---


void FileSys_writeString(uint8_t strtadr, String wrstr)
{
	char wrdat[1000]; // 1000: arbitrary

	int len = wrstr.length();
	wrstr.toCharArray(wrdat, len);

	uint8_t pos = strtadr;
	// 1. write size 
	FileSys_write_uint8_t( pos++, len );

	// 2. write strings
	for(int idx=0; idx < len; idx++) {
		FileSys_write_uint8_t( pos++, wrdat[idx] );
	}
	FileSys_commit();
}

String FileSys_readString(uint8_t strtadr, uint8_t *szptr)
{
	char rddat[1000] = { 0 }; // 1000: arbitrary

	uint8_t pos = strtadr;

	// 1. read size
	int len = FileSys_read_uint8_t(pos++);
	if (len == 0) {
		return "";
	}

	// 2. read strings
	for(int idx=0; idx < len; idx++) {
		rddat[idx] = FileSys_read_uint8_t(pos++);
	}
	rddat[len] = 0x00; // terminator

	if (szptr != NULL) {
		*szptr = strlen(rddat);
	}

	return String(rddat);
}

void Test_read_write_String()
{
	debug_outputDebugString("Test_read_write_String", "start");

	uint8_t strtadr = 0; // start address
	String datstr = "Hello,7of9,This is Vital.";

	FileSys_writeString(strtadr, datstr);

	datstr = "";
	debug_outputDebugString("Test_read_write_String", "Line191 > " + datstr);

	uint8_t len;
	datstr = FileSys_readString(strtadr, &len);
	debug_outputDebugString("Test_read_write_String", "Line199 > " + datstr);
	debug_outputDebugString("Test_read_write_String", "Line200 > " + String(len));
}

//---

void Test_read_write_structeredData()
{
	typedef struct _tag_dmystrct {
		String name;
		bool isSecret;
		uint8_t u8val;
		String suffix;
	} dmystrct_t;

	dmystrct_t data[2];

	// set ------------------------------------------
	// - 1st
	data[0].name = "7of9";
	data[0].isSecret = 1;
	data[0].u8val = 79;
	data[0].suffix = "Resistance is futile...";
	// - 2nd
	data[1].name = "Vital";
	data[1].isSecret = 0;
	data[1].u8val = 47;
	data[1].suffix = "Over my dead body...";

	debug_outputDebugString("Test_read_write_structeredData", "Line228 > " + data[1].name);

	int startaddr = 0; // start address of the write
	for(int idx=0; idx < 2; idx++) {
		String wrstr = "";
		wrstr = wrstr + data[idx].name
			+ "," + String(data[idx].isSecret)
			+ "," + String(data[idx].u8val)
			+ "," + data[idx].suffix;
		FileSys_writeString(startaddr, wrstr);
		startaddr += wrstr.length();
	}

	// clear ---------------------------------------
	// - 1st
	data[0].name = "";
	data[0].isSecret = 0;
	data[0].u8val = 0;
	data[0].suffix = "";
	// - 2nd
	data[1].name = "";
	data[1].isSecret = 0;
	data[1].u8val = 0;
	data[1].suffix = "";

	debug_outputDebugString("Test_read_write_structeredData", "Line254 > " + data[1].name);

	startaddr = 0; // start address of the write
	uint8_t len;

	for(int idx=0; idx < 2; idx++) {
		String rdstr = "";
		rdstr = FileSys_readString(startaddr, &len);
		startaddr += rdstr.length();
		debug_outputDebugString("Test_read_write_structeredData", "Line282 > " + rdstr);
	}	

}