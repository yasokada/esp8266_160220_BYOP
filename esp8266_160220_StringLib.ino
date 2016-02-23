
// String library

/*
 * v0.2 2016 Feb 20
 *  - extractCsvRow: fix bug > "AAA" return "AAA","AAA","AAA"
 * v0.1 2016 Feb 20
 *  - add Test_extractCsvRow()
 *  - add extractCsvRow()
 */

//-------------------------------------------------------------------------

void Test_extractCsvRow()
{
  // test1 >> full
  String csvline = "AAA,BBB,CCC";
  String tmp;
  for(int idx = 0; idx < 3; idx++) {
    tmp = extractCsvRow(csvline, idx);
    Serial.println(tmp);    
  }

  // test2 >> only 1st
  csvline = "AAA";
  for(int idx = 0; idx < 3; idx++) {
    tmp = extractCsvRow(csvline, idx);
    if (tmp.length() == 0) {
      continue;
    }
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

    if (posIdx != getIdx) {
      return "";
    }
    
    commaPos = srcline.indexOf(',', nextOfComma);
    if (commaPos < 0) {
      return srcline.substring(nextOfComma);
    } else {
      return srcline.substring(nextOfComma, commaPos);
    }
}

// TODO: 0z > add function to get item count
