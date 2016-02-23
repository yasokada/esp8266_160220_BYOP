
/*
 * This program is used for a message station shown at
 * https://hackaday.io/project/9526-bring-your-own-pizero-byop
 */


/*
 * v0.17 2016 Feb. 23
 *  - msgStorageLib: fix bug > MsgServer_Get1stMessage()
 * v0.16 2016 Feb. 23
 *  - msgStorageLib: impl MsgServer_Get1stMessage()
 *  - msgStorageLib: impl MsgServer_Remove1stMessage()
 *  - msgStorageLib: impl MsgServer_PostMessage()
 * v0.15 2016 Feb. 23
 *  - msgStorageLib: impl Test_MsgServer_postThenGet()
 *  - add [msgStorage.h]
 *  - msgStorageLib: design interface
 * v0.14 2016 Feb. 22
 *  - test
 *  - msgStorageLib: add Test_MsgServer_setupDummyMessages()
 *  - msgStorageLib: add s_messageList[], [s_messageCount]
 * v0.13 2016 Feb. 21
 *  - add (msgStorageLib)
 *  - change member variable names of [message_t]
 * v0.12 2016 Feb. 20
 *  - display reply on LCD in proc_bye()
 * v0.11 2016 Feb 20
 *  - display "ready" on 8x2 LCD just after power on
 *  - add [esp8266_160217_AQM0802.ino] to display characters on 8x2 LCD
 * v0.10 2016 Feb 20
 *  - fix proc_bye() to use nickName
 *  - move [kCmdList] and [CMD_e] to [dataStructure.h]
 *  - update proc_hello() to reply in Serial
 *  - add [message_t]
 * v0.9 2016 Feb 20
 *  - move [g_owner] to dataLib
 *  - move [Pizero_owner_t] to dataLib
 *  - add dataLib, dataStructure.h
 * v0.8 2016 Feb 20
 *  - move Serial_getCommandIdx() and those related
 *  - move Serial_replyToCommand() to UARTlib
 *  - add proc_check(), proc_get(), proc_post(), proc_bye()
 * v0.7 2016 Feb 20
 *  - add [g_owner]
 *  - add [Pizero_owner_t]
 *  - add proc_hello();
 *  - add debug_outputDebugString();
 * v0.6 2016 Feb 20
 *  - UARTlib > extractCsvRow: fix bug > "AAA" return "AAA","AAA","AAA"
 * v0.5 2016 Feb 20
 *  - add kCmdList String array and CMD_e enum
 *  - move extractCsvRow() and those related to StringLib
 * v0.4 2016 Feb 20
 *  - add extractCsvRow() / Test_extractCsvRow()
 * v0.3 2016 Feb 20
 *  - move static declaraions for UART to UARTlib
 *  - move Serial_readCommand to UARTlib
 *  - use equalsIgnoreCase() instead of equals() in Serial_replyToCommand()
 * v0.2 2016 Feb 20
 *  - fix Serial_readCommand() to handle CR code
 *  - add Serial_replyToCommand()
 * v0.1 2016 Feb 20
 *  - add Serial_readCommand()
 *  - add serial setup
 */

#include <Wire.h> // for 8x2 LCD
#include "dataStructure.h"

//-------------------------------------------------------------------------
// static declarations and enums

//-------------------------------------------------------------------------
// extern declarations
extern Pizero_owner_t g_owner;

//-------------------------------------------------------------------------
// for debug

void debug_outputDebugString(String prefix, String msg)
{
  // this function is used to turn off/on all the debug prints

  Serial.println("[DEBUG] " + prefix + " > " + msg);
}

//-------------------------------------------------------------------------
// functoins related to sequence

void setup() {
  Serial.begin(115200); 
  Wire.begin();
  AQM0802_Initialize(/* cursorOn=*/true, /* cursorPosOn=*/true, /* contrast=*/7);
//  Test_AQM0802_cursorOn_posOff_contrastLow();
//  Test_AQM0802_cursorOn_posOn_contrastLow();
//  Test_AQM0802_cursorOn_posOn_contrastHigh();  

  AQM0802_PutMessage("...........ready", /* x_st1=*/1, /* y_st1=*/1);
}

void loop() {
  String rcvd = Serial_readCommand();
  if (rcvd.length() > 0) {
    Serial_replyToCommand(rcvd);
  }
}

//-------------------------------------------------------------------------

bool proc_hello(String csvline)
{  
  String cmdstr = extractCsvRow(csvline, 0);
  String serno = extractCsvRow(csvline, 1);
  String nickname = extractCsvRow(csvline, 2);

  if (nickname.length() == 0) {
    return false;
  }

//  debug_outputDebugString("proc_hello", "line89 > " + csvline);

  g_owner.nickName = nickname;
  g_owner.serialNo = serno;
  
//  debug_outputDebugString("proc_hello", "line105 > [S/N]" + serno);
//  debug_outputDebugString("proc_hello", "line106 > [name]" + nickname);

  String reply = kCmdList[CMD_HELLO] + "," + g_owner.nickName;

  Serial.println(reply);

  AQM0802_Clear();
  AQM0802_PutMessage(reply, /* x_st1=*/1, /* y_st1=*/1);

  return true;
}

bool proc_check(String csvline)
{
  debug_outputDebugString("proc_check", "line116 > start");
}

bool proc_get(String csvline)
{
  debug_outputDebugString("proc_get", "line121 > start");
}

bool proc_post(String csvline)
{
  debug_outputDebugString("proc_post", "line126 > start");
}

bool proc_bye(String csvline)
{
  String reply = kCmdList[CMD_BYE] + "," + g_owner.nickName;

  Serial.println(reply);

  AQM0802_Clear();
  AQM0802_PutMessage(reply, /* x_st1=*/1, /* y_st1=*/1);  
}


