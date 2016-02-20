
/*
 * This program is used for a message station shown at
 * https://hackaday.io/project/9526-bring-your-own-pizero-byop
 */


/*
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
void Test_extractCsvRow()
{
  String csvline = "AAA,BBB,CCC";
  String tmp;
  for(int idx = 0; idx < 3; idx++) {
    tmp = extractCsvRow(csvline, idx);
    Serial.println(tmp);    
  }
}

String extractCsvRow(String srcline, int getIdx)
{
    int commaPos = -1; // to start from 0 by (commadPos + 1), this requires -1 here
    int posIdx = 0;
    int nextOfComma;

    while(1) {
      nextOfComma = commaPos + 1;
      commaPos = srcline.indexOf(',', nextOfComma);
      if (commaPos < 0) {
        break;
      }
      if (posIdx == getIdx) {
        break; 
      }
      posIdx++;
    }
    commaPos = srcline.indexOf(',', nextOfComma);
    if (commaPos < 0) {
      return srcline.substring(nextOfComma);
    } else {
      return srcline.substring(nextOfComma, commaPos);
    }
}
//-------------------------------------------------------------------------

void setup() {
  Serial.begin(115200); 
}


void Serial_replyToCommand(String cmdline)
{  
  if (cmdline.equalsIgnoreCase("hello")) {
    Serial.println("hello, 7of9");
  } else {
#if 1
    Test_extractCsvRow();
#else      
    Serial.println(cmdline);    
#endif    
  }
}

void loop() {
  String rcvd = Serial_readCommand();
  if (rcvd.length() > 0) {
    Serial_replyToCommand(rcvd);
  }
}
