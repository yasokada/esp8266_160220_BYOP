
/*
 * This program is used for a message station shown at
 * https://hackaday.io/project/9526-bring-your-own-pizero-byop
 */


/*
 * v0.7 2016 Feb 20
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

//-------------------------------------------------------------------------
// static declrations and enums

static String kCmdList[] = {
  "hello",
  "check",
  "get",
  "post",
  "bye",
};

enum tag_CMD_e {
  CMD_HELLO,
  CMD_CHECK,
  CMD_GET,
  CMD_POST,
  CMD_BYE,
  // below is special index
  SIZE_CMD = CMD_BYE + 1,
} CMD_e;

//-------------------------------------------------------------------------
// for debug

void debug_outputDebugString(String prefix, String msg)
{
  // this function is used to turn off/on all the debug prints

  Serial.println("[DEBUG] " + prefix + " > " + msg);
}

//-------------------------------------------------------------------------

void setup() {
  Serial.begin(115200); 
}

signed int getCommandIdx(String aCmd)
{
  aCmd.toLowerCase();
  for(int idx = 0; idx < SIZE_CMD; idx++) {
    if ( aCmd.equalsIgnoreCase(kCmdList[idx]) ) {
      return idx;
    }
  }
  return -1;
}

bool proc_hello(String csvline)
{  
  String cmdstr = extractCsvRow(csvline, 0);
  String serNo = extractCsvRow(csvline, 1);
  String nickName = extractCsvRow(csvline, 2);

  if (nickName.length() == 0) {
    return false;
  }

  debug_outputDebugString("proc_hello", "line89 > " + csvline);

#if 1
  Serial.println("Your S/N:" + serNo);
  Serial.println("Your name:" + nickName);
#endif

  return true;
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
  case CMD_BYE:
    Serial.println("bye,7of9");
    break;
  default:
    // do nothing
    break;
  }
}

void loop() {
  String rcvd = Serial_readCommand();
  if (rcvd.length() > 0) {
    Serial_replyToCommand(rcvd);
  }
}
