
/*
 * This program is used for a message station shown at
 * https://hackaday.io/project/9526-bring-your-own-pizero-byop
 */


/*
 *  - add (filesysLib)
 * v0.24 2016 Feb. 27
 *  - update proc_clear() to call MsgServer_Clear()
 *  - add [kAuthorizationCode], which is used to protect to clear all the messages
 * v0.23 2016 Feb. 27
 *  - add [CMD_ALL_CLEAR] in Serial command
 *  - update proc_get() > add 3rd parameter [isSecret(0|1)] (ref. protocol v0.7)
 *  - fix bug > proc_get() > mistake in secret message display on LCD
 * v0.22 2016 Feb. 26
 *  - hide secret message (do not show on LCD)
 *    + update proc_get()
 *    + msgStorageLib: add MsgServer_GetIsSecretOf1stMessage()
 *  - fix bug > proc_get() > did not correctly treat message string
 *  - tested proc_post()
 * v0.21 2016 Feb. 25
 *  - proc_post() reply in serial 
 *  - proc_get() can get also [isSecret] to show / hide message on LCD
 *  - replace print for debug with debug_outputDebugString()
 *    + to clarify the debug print from communication strings
 * v0.20 2016 Feb. 23
 *  - turn off debug ifdef
 *  - impl proc_post()
 *  - add [POST_IDX_e]
 *  - impl proc_post()
 * v0.19 2016 Feb. 23
 *  - update proc_get()
 *  - msgStorageLib: fix Test_MsgServer_setupDummyMessages()
 *  - impl proc_get()
 * v0.18 2016 Feb. 23
 *  - impl proc_check()
 *  - msgStorageLib: add Test_MsgServer_Clear() 
 *  - msgStorageLib: fix MsgServer_Remove1stMessage()
 * v0.17 2016 Feb. 23
 *  - msgStorageLib: impl MsgServer_Clear()
 *  - msgStorageLib: impl MsgServer_GetMessageCount()
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

// serail number of the 1st commit of [esp8266_160220_BYOP]
static const String kAuthorizationCode = "7975f53";

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

  g_owner.nickName = nickname;
  g_owner.serialNo = serno;
  
  String reply = kCmdList[CMD_HELLO] + "," + g_owner.nickName;

  Serial.println(reply);

  AQM0802_Clear();
  AQM0802_PutMessage(reply, /* x_st1=*/1, /* y_st1=*/1);

  return true;
}

bool proc_check(String csvline)
{
  int msgcnt = MsgServer_GetMessageCount(g_owner.nickName);
  String reply = kCmdList[CMD_CHECK] + "," + String(msgcnt);
  Serial.println(reply);

  AQM0802_Clear();
  AQM0802_PutMessage(reply, /* x_st1=*/1, /* y_st1=*/1);  
  return true;
}

bool proc_get(String csvline)
{
  String rcver = g_owner.nickName;

  int msgcnt = MsgServer_GetMessageCount(rcver);
  if (msgcnt == 0) {
    return false; // TODO: 0m > what to reply???
  }
  String msgstr = MsgServer_Get1stMessage(rcver);

  debug_outputDebugString("proc_get", "msgstr:" + msgstr);
;

  bool isScrt = MsgServer_GetIsSecretOf1stMessage(rcver);

  // ---
  // When the user reads the message, the message is removed.
  MsgServer_Remove1stMessage(rcver);
  // ---

  String reply = kCmdList[CMD_GET] + "," + msgstr;
  if (isScrt) {
    reply = reply + ",1";
  } else {
    reply = reply + ",0";    
  }
  Serial.println(reply);

  if (isScrt) {
    // replace message in order not to show the message
    msgstr = "smsg,rcvd";
  }
  AQM0802_Clear();
  AQM0802_PutMessage(msgstr, /* x_st1=*/1, /* y_st1=*/1);  
  return true;
}

bool proc_post(String csvline)
{
  debug_outputDebugString("proc_post", "line126 > " + csvline);

  String msgstr = extractCsvRow(csvline, POST_IDX_MESSAGE);
  String rcver = extractCsvRow(csvline, POST_IDX_RECEIVER);
  String strIsScret = extractCsvRow(csvline, POST_IDX_IS_SECRET);

  if (strIsScret.length() == 0) {
    return false; // error
  }

  bool isSecret = strIsScret.toInt();

  debug_outputDebugString("proc_post", "rcver:" + rcver);
  debug_outputDebugString("proc_post", "msg:" + msgstr);

  MsgServer_PostMessage(g_owner.serialNo, g_owner.nickName , rcver, msgstr, isSecret);

  String reply = kCmdList[CMD_POST] + ",ok";
  Serial.println(reply);

  String lcdmsg = "msg,posted";
  AQM0802_Clear();
  AQM0802_PutMessage(lcdmsg, /* x_st1=*/1, /* y_st1=*/1);  

  return true;
}

bool proc_bye(String csvline)
{
  String reply = kCmdList[CMD_BYE] + "," + g_owner.nickName;

  Serial.println(reply);

  AQM0802_Clear();
  AQM0802_PutMessage(reply, /* x_st1=*/1, /* y_st1=*/1);  
}

bool proc_clear(String csvline)
{
  debug_outputDebugString("proc_clear", "Line251 > start");

  String athcd = extractCsvRow(csvline, 1);
  if (athcd != kAuthorizationCode) {
    return false; // error
  }

  debug_outputDebugString("proc_clear", "Line263 > cleared");

  MsgServer_Clear(); /*******/

  String reply = kCmdList[CMD_ALL_CLEAR] + ",ok";
  Serial.println(reply);

  AQM0802_Clear();
  AQM0802_PutMessage(reply, /* x_st1=*/1, /* y_st1=*/1);  

  return true;
}


// TODO: 0z > CMD_DEBUG_ON // on default, debug_outputDebugString() does not show any string

