
/*
 * This program is used for a message station shown at
 * https://hackaday.io/project/9526-bring-your-own-pizero-byop
 */


/*
 * v0.3 2016 Feb 20
 *  - use equalsIgnoreCase() instead of equals() in Serial_replyToCommand()
 * v0.2 2016 Feb 20
 *  - fix Serial_readCommand() to handle CR code
 *  - add Serial_replyToCommand()
 * v0.1 2016 Feb 20
 *  - add Serial_readCommand()
 *  - add serial setup
 */

//-------------------------------------------------------------------------
// static declarations

static const char kCodeCR = '\r';
static const char kCodeLF = '\n';

static String s_serialReceiveBuff = "";
static bool s_serialClearOnNext = false;
//-------------------------------------------------------------------------
void setup() {
  Serial.begin(115200); 
}

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
  if (cmdline.equalsIgnoreCase("hello")) {
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
