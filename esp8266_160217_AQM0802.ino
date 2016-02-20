#include <Wire.h>

/*
 * v0.3 2016 Feb. 18
 *  - add message display function
 *    + add Test_AQM0802_showDateTime()
 *    + add AQM0802_PutMessage()
 * v0.2 2016 Feb. 17
 *  - add test functions()
 *    + add Test_AQM0802_cursorOn_posOn_contrastHigh()
 *    + add Test_AQM0802_cursorOn_posOn_contrastLow()
 *    + add Test_AQM0802_cursorOn_posOff_contrastLow()
 * v0.1 2016 Feb. 17
 *  - add AQM0802_Initialize()
 *  - add AQM0802_Clear()
 *  - add AQM0802_WriteData()
 *  - add AQM0802_WriteInstruction()
 *  - add writeToDevice()
 *
 * -------
 * Special thanks to exabugs for ( http://qiita.com/exabugs/items/9d1b66aa1d22805afbc8 )
 */

static const uint8_t kDeviceAddr = 0x3e;

static const int kMaxXsize = 8;
static const int kMaxYsize = 2;

static uint8_t ControlByteList[] = {
  0x00, // Instruction write operation. ( Co=0, Rs=0 )
  0x40, // Data write operation. ( Co=0, Rs=1 )
};

enum {
  TYPE_INSTRUCITON = 0,
  TYPE_DATA,
};

//---------------------------------------------------------------------------------

// private functions ---------------------------
void writeToDevice(int type, uint8_t *dataByte, size_t len) 
{
  for (int idx = 0; idx < len; idx++) {
    Wire.beginTransmission(kDeviceAddr);
    Wire.write(ControlByteList[type]);
    Wire.write(dataByte[idx]);
    Wire.endTransmission();
    delayMicroseconds(27); // 26.3us
  }
}

//---------------------------------------------------------------------------------

// public functions ---------------------------
void AQM0802_WriteSingleInstruction(uint8_t data) 
{ 
  size_t len = 1;
  uint8_t list[] = {data};
  writeToDevice(TYPE_INSTRUCITON, list, len); 
}

void AQM0802_WriteInstructions(uint8_t *data, int len) 
{ 
  writeToDevice(TYPE_INSTRUCITON, data, len); 
}

void AQM0802_WriteData(uint8_t *data, size_t len) 
{ 
  writeToDevice(TYPE_DATA, data, len); 
}

void AQM0802_Clear()
{
    AQM0802_WriteSingleInstruction(0x01);
}

void AQM0802_Initialize(bool cursorOn, bool cursorPosOn, uint8_t contrast)
{ 
  // 
  delay(40); // Wait time > 40ms after VDD stable

  // Function set
  AQM0802_WriteSingleInstruction(0x38);

  // Function set
  AQM0802_WriteSingleInstruction(0x39);

  // Internal OSC frequency
  AQM0802_WriteSingleInstruction(0x14);

  // { Contrast set -----------------------------
  uint8_t ctrst = contrast;
  if (ctrst > 0b1111) {
    ctrst = 0b1111;
  }
  AQM0802_WriteSingleInstruction(0x70 | ctrst);
  // } Contrast set -----------------------------

  // Power/ICON/Contrast control
  AQM0802_WriteSingleInstruction(0x56);

  // Follower control
  AQM0802_WriteSingleInstruction(0x6C);

  // Wait time > 200mS (for power stable)
  delay(200);

  // Function set
  AQM0802_WriteSingleInstruction(0x38);

  // { Display ON/OFF control -----------------------
  uint8_t extra = 0x0;
  if (cursorOn) {
    extra = extra | 0b10;
  }
  if (cursorPosOn) {
    extra = extra | 0b11;    
  }
  AQM0802_WriteSingleInstruction(0x0C | extra);
  // } Display ON/OFF control -----------------------

  // Clear Display
  AQM0802_WriteSingleInstruction(0x01);

  // Wait time > 1.08ms
  delay(2);
}

void AQM0802_PutMessage(String msg, uint8_t x_st1, uint8_t y_st1)
{
  // _st1 : index starting from 1
  if ((x_st1 > kMaxXsize) || (y_st1 > kMaxYsize)) {
    return; // error
  }

  uint8_t pos = 0x80 | ((y_st1 - 1) * 0x40);
  pos = pos | (x_st1 - 1);

  AQM0802_WriteSingleInstruction(pos);
  AQM0802_WriteData( (uint8_t *)msg.c_str(), msg.length() );  
}

//---------------------------------------------------------------------------------

// test functions ---------------------------

void Test_AQM0802_cursorOn_posOff_contrastLow()
{
  AQM0802_Initialize(/* cursorOn=*/true, /* cursorPosOn=*/false, /* contrast=*/1);
}

void Test_AQM0802_cursorOn_posOn_contrastLow()
{
  AQM0802_Initialize(/* cursorOn=*/true, /* cursorPosOn=*/true, /* contrast=*/1);
}

void Test_AQM0802_cursorOn_posOn_contrastHigh()
{
  AQM0802_Initialize(/* cursorOn=*/true, /* cursorPosOn=*/true, /* contrast=*/15);
}

void Test_AQM0802_showDateTime(String datstr, String timstr)
{
  AQM0802_PutMessage(datstr, /* x_st1=*/1, /* y_st1=*/1);
  AQM0802_PutMessage(timstr, /* x_st1=*/3, /* y_st1=*/2);
}