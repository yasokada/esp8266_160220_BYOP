
// UART communication library

/*
 * v0.1 2016 Feb 20
 *   - add [kCodeCR], [kCodeLF], [s_serialReceiveBuff], [s_serialClearOnNext]
 *   - add Serial_readCommand()
 */

//-------------------------------------------------------------------------
// static declarations

static const char kCodeCR = '\r';
static const char kCodeLF = '\n';

static String s_serialReceiveBuff = "";
static bool s_serialClearOnNext = false;
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

  int cmdidx = getCommandIdx(strcmd);
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
  default:
    // do nothing
    break;
  }
}

