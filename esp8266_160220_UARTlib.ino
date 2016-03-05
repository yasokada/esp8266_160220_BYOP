
// UART communication library

/*
 * v0.3 2016 Mar. 5
 *   - add ADMIN_CMD_STORAGE
 * v0.2 2016 Feb. 27
 *   - add ADMIN_CMD_ALL_CLEAR
 * v0.1 2016 Feb 20
 *   - add [kCodeCR], [kCodeLF], [s_serialReceiveBuff], [s_serialClearOnNext]
 *   - add Serial_readCommand()
 */


#include "dataStructure.h"

//-------------------------------------------------------------------------
// static declarations

static const char kCodeCR = '\r';
static const char kCodeLF = '\n';

static String s_serialReceiveBuff = "";
static bool s_serialClearOnNext = false; // if true, s_serialReceiveBuff will be deleted
//-------------------------------------------------------------------------


String Serial_readCommand()
{
  if (Serial.available() == 0) {
    return "";  
  }

  if (s_serialClearOnNext) {
    s_serialClearOnNext = false;
    s_serialReceiveBuff = "";
  }
  
  char code;
  while(Serial.available() > 0) {
    code = Serial.read();
    if (code == kCodeCR) {
      continue;
    }
    if (code == kCodeLF) {
      s_serialClearOnNext = true;
      return s_serialReceiveBuff;
    } else {
      s_serialReceiveBuff += code;
    }
  }
}


void Serial_replyToCommand(String cmdline)
{  
  String strcmd = extractCsvRow(cmdline, 0);
  if (strcmd.length() == 0) {
    return; // is not a command
  }

  int cmdidx = Serial_getCommandIdx(strcmd);
  if (cmdidx < 0) {
    Serial.println(cmdline);    
  }

  switch(cmdidx) {
  case CMD_HELLO:
    proc_hello(cmdline);
    break;
  case CMD_CHECK:
    proc_check(cmdline);
    break;
  case CMD_GET:
    proc_get(cmdline);
    break;
  case CMD_POST:
    proc_post(cmdline);
    break;
  case CMD_BYE:
    proc_bye(cmdline);
    break;
  case ADMIN_CMD_ALL_CLEAR:
    proc_clear(cmdline);
    break;
  case ADMIN_CMD_STORAGE:
    proc_storageUsage(cmdline);
    break;
  default:
    // do nothing
    break;
  }
}

signed int Serial_getCommandIdx(String aCmd)
{
  aCmd.toLowerCase();
  for(int idx = 0; idx < SIZE_CMD; idx++) {
    if ( aCmd.equalsIgnoreCase(kCmdList[idx]) ) {
      return idx;
    }
  }
  return -1;
}
