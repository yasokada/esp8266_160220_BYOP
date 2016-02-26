
// File System Library

// TODO: 0c > some problem including <FS.h>, which cause invisibility for other library functions

//#include <FS.h>

/*
Fils sytem using [SPIFFS].
Checked on staging 2.1.0-rc1 Arduino IDE 1.6.6 (2016 Feb. 27)
*/


/*
 * v0.2 2016 Feb. 27
 *  - add File_init()
 * v0.1 2016 Feb. 27
 *  - prepare empty file
 */


bool FileSys_init()
{
//	bool res = SPIFFS.begin();
#if 1	
	return false;
#else
	return res;
#endif	
}

