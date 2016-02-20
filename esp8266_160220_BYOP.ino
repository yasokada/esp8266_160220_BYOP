
/*
 * This program is used for a message station shown at
 * https://hackaday.io/project/9526-bring-your-own-pizero-byop
 */


/*
 * v0.8 2016 Feb 20
 *  - move Serial_replyToCommand() to UARTlib
 *  - add proc_check(), proc_get(), proc_post(), proc_bye()
 * v0.7 2016 Feb 20
 *  - add [s_owner]
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

// Information of owner
typedef struct tag_pizero_owner_t { 
  String serialNo;
  String nickName;
} Pizero_owner_t;

Pizero_owner_t s_owner;

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
  String serno = extractCsvRow(csvline, 1);
  String nickname = extractCsvRow(csvline, 2);

  if (nickname.length() == 0) {
    return false;
  }

  debug_outputDebugString("proc_hello", "line89 > " + csvline);

  s_owner.nickName = nickname;
  s_owner.serialNo = serno;
  
  debug_outputDebugString("proc_hello", "line105 > [S/N]" + serno);
  debug_outputDebugString("proc_hello", "line106 > [name]" + nickname);

  // TODO: 0m > display "Hello,<CR><LF>[nickName]" to LCD

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
    Serial.println("bye,7of9");  
}

void loop() {
  String rcvd = Serial_readCommand();
  if (rcvd.length() > 0) {
    Serial_replyToCommand(rcvd);
  }
}
