
/*
 * This program is used for a message station shown at
 * https://hackaday.io/project/9526-bring-your-own-pizero-byop
 */


/*
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
// static declrations

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
} CMD_e;

//-------------------------------------------------------------------------

void setup() {
  Serial.begin(115200); 
}

signed int getCommandIdx(String aCmd)
{
  aCmd.toLowerCase();
}

void Serial_replyToCommand(String cmdline)
{  
  String strcmdr = extractCsvRow(cmdline, 0);
  if (strcmdr.length() == 0) {
    return; // is not a command
  }

  if (strcmdr.equalsIgnoreCase("hello")) {
    Serial.println("hello, 7of9");
  } else {
    Serial.println(cmdline);
  }
}

void loop() {
  String rcvd = Serial_readCommand();
  if (rcvd.length() > 0) {
    Serial_replyToCommand(rcvd);
  }
}
