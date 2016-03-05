
// Message Storage/Load Library

#include <stdint.h>
#include "msgStorage.h"

/*
 * v0.14 2016 Mar. 5
 *  - change [kMaxnum_MessageCount] from 10 to 500
 * v0.13 2016 Mar. 5
 *	- add MsgServer_GetStorageUsage()
 * v0.12 2016 Mar. 4
 *  - add MsgServer_GetSenderNameOf1stMessage()
 * v0.11 2016 Feb. 23
 *	- fix MsgServer_Save() > for s_messageCount==0, FileSys_commit() was necessary
 *  - fix MsgServer_Load() > assign to s_messageCount
 *  - add message save/load
 *		+ add Test_MsgServer_SaveLoad()
 *		+ add MsgServer_Load();
 *		+ add MsgServer_Save();
 * v0.10 2016 Feb. 23
 *  - add MsgServer_GetIsSecretOf1stMessage()
 * v0.9 2016 Feb. 23
 *  - fix Test_MsgServer_setupDummyMessages()
 * v0.8 2016 Feb. 23
 *  - add Test_MsgServer_Clear()
 *  - fix MsgServer_Remove1stMessage()
 * v0.7 2016 Feb. 23
 *  - impl MsgServer_Clear()
 *  - impl MsgServer_GetMessageCount()
 *	- fix bug > MsgServer_Get1stMessage()
 * v0.6 2016 Feb. 23
 *	- impl MsgServer_Get1stMessage()
 *  - impl MsgServer_Remove1stMessage()
 *  - impl MsgServer_PostMessage()
 * v0.5 2016 Feb. 23
 *  - impl Test_MsgServer_postThenGet()
 * v0.4 2016 Feb. 23
 *  - use [msgStorage.h]
 * v0.3 2016 Feb. 23
 *  - add Test_MsgServer_postThenGet()
 *  - add MsgServer_PostMessage()
 *  - add MsgServer_Remove1stMessage()
 *  - add MsgServer_Get1stMessage()
 *  - add MsgServer_GetMessageCount()
 *  - add MsgServer_Clear()
 * v0.2 2016 Feb. 22
 *  - add Test_MsgServer_setupDummyMessages()
 *  - add s_messageList[], [s_messageCount]
 * v0.1 2016 Feb. 21
 *  - prepare empty functions
 */

//-------------------------------------------------------------------------
// file scope static functions / variables

// [200] is selected to avoid boot problem (300 or more was not good)
static const int kMaxnum_MessageCount = 200; 

static message_t s_messageList[kMaxnum_MessageCount] __attribute__ ((section(".noinit")));; 
static int s_messageCount = 0;
//-------------------------------------------------------------------------

static const message_t s_dummyMsg[] =
{
	// senderSerial, senderName, receiverName, message, isSecret 
	{ "000000000000000d", "7of9", "Vital", "hello_Vital", 0 },
	{ "000000000000002g", "Vital", "7of9", "hello_7of9", 0 },
	{ "000000000000000d", "7of9", "Vital", "meet at the Arctic", 1 },
	{ "000000000000002g", "Vital", "7of9", "No_thank you.", 1 },
};
//-------------------------------------------------------------------------


void MsgServer_Clear()
{
	message_t *dstPtr;

	for(int idx = 0; idx < s_messageCount; idx++) {
		dstPtr = &(s_messageList[idx]);
		dstPtr->senderSerial = "";
		dstPtr->senderName = "";
		dstPtr->receiverName = "";
		dstPtr->message = "";
		dstPtr->isSecret = 0;
	}

	s_messageCount = 0;
}

String MsgServer_GetStorageUsage()
{
	String resstr = String(s_messageCount) + "/" + String(kMaxnum_MessageCount);
	return resstr;
}

int MsgServer_GetMessageCount(String rcver)
{
	// Return count of message for the receiver
	//
	int cnt = 0;
	for(int idx = 0; idx < s_messageCount; idx++) {
//		String msg97 = "Line97 > " + s_messageList[idx].receiverName;
//		debug_outputDebugString("MsgServer_GetMessageCount", msg97);
		if (s_messageList[idx].receiverName == rcver) {
			cnt++;
		}
	}
	return cnt;
}

bool MsgServer_GetIsSecretOf1stMessage(String rcver)
{
	// Return [isSecret] of the 1st message for the receiver
	//

	for(int idx = 0; idx < s_messageCount; idx++) {
		if (s_messageList[idx].receiverName == rcver) {
			return s_messageList[idx].isSecret;
		}
	}
	return false; // nothing to remove
}

String MsgServer_Get1stMessage(String rcver)
{
	// Return 1st message for the receiver
	//

	for(int idx = 0; idx < s_messageCount; idx++) {
		if (s_messageList[idx].receiverName == rcver) {
			return s_messageList[idx].message;
		}
	}
	return ""; // nothing to remove
}

String MsgServer_GetSenderNameOf1stMessage(String rcver)
{
  // Return [senderSerial] of the 1st message for the receiver
  //

  for(int idx = 0; idx < s_messageCount; idx++) {
    if (s_messageList[idx].receiverName == rcver) {
      return s_messageList[idx].senderName;
    }
  }
  return ""; // nothing to remove
}

void MsgServer_Remove1stMessage(String rcver)
{
	// remove 1st message for the receiver
	//

	// 1. Find target to remove
	String work;
	int8_t removeIdx = -1;
	for(int idx = 0; idx < s_messageCount; idx++) {
		work = s_messageList[idx].receiverName;
		if (work == rcver) {
			removeIdx = idx;
			break;
		}
	}
	if (removeIdx < 0) {
		return; // nothing to remove
	}

	// 2. Remove
	int nextIdx;
	int startIdx = removeIdx;
	for(int idx = startIdx; idx < s_messageCount; idx++) {
		nextIdx = (idx + 1);
		s_messageList[idx].senderSerial = s_messageList[nextIdx].senderSerial;
		s_messageList[idx].senderName 	= s_messageList[nextIdx].senderName;
		s_messageList[idx].receiverName = s_messageList[nextIdx].receiverName;
		s_messageList[idx].message 		= s_messageList[nextIdx].message;
		s_messageList[idx].isSecret 	= s_messageList[nextIdx].isSecret;
	}	
	s_messageCount--;
}

bool MsgServer_PostMessage(String srl, String snder, String rcver, String msg, bool isSecret)
{
	// add message to the list
	//

	if ( s_messageCount >= (kMaxnum_MessageCount - 1) ) {
		return false;
	}

	int nextIdx = s_messageCount;
	s_messageList[nextIdx].senderSerial = srl;
	s_messageList[nextIdx].senderName 	= snder;
	s_messageList[nextIdx].receiverName = rcver;
	s_messageList[nextIdx].message 		= msg;
	s_messageList[nextIdx].isSecret 	= isSecret;

	s_messageCount++;
}

void MsgServer_Save()
{
//	debug_outputDebugString("MsgServer_Save", "Line169 > start");	

	int startaddr = 0; // start address of the write

	// 1. write count
	FileSys_write_uint8_t(startaddr++, s_messageCount);
	FileSys_commit(); // for the case (s_messageCount==0)

	// 2. write list
	for(int idx=0; idx < s_messageCount; idx++) {
		String wrstr = "";
		wrstr = wrstr + s_messageList[idx].senderSerial
			+ "," + s_messageList[idx].senderName
			+ "," + s_messageList[idx].receiverName
			+ "," + s_messageList[idx].message
			+ "," + String(s_messageList[idx].isSecret);
		FileSys_writeString(startaddr, wrstr);
		startaddr += wrstr.length();
		startaddr++; // for terminator

//		debug_outputDebugString("MsgServer_Save", "Line185 > " + wrstr);
	}

}

void MsgServer_Load()
{
//	debug_outputDebugString("MsgServer_Load", "Line174 > start");

	int startaddr = 0; // start address of the write

	int lstcnt = FileSys_read_uint8_t(startaddr++);
	s_messageCount = lstcnt;

	uint8_t len;
	for(int idx=0; idx < lstcnt; idx++) {
		String rdstr = FileSys_readString(startaddr, &len);
		startaddr += len;
		startaddr++; // for terminator

		if (rdstr.length() == 0) {
			continue;
		}
		s_messageList[idx].senderSerial = extractCsvRow(rdstr, 0);
		s_messageList[idx].senderName = extractCsvRow(rdstr, 1);
		s_messageList[idx].receiverName = extractCsvRow(rdstr, 2);
		s_messageList[idx].message = extractCsvRow(rdstr, 3);
		String istr = extractCsvRow(rdstr, 4);
		s_messageList[idx].isSecret = (bool)istr.toInt();

		debug_outputDebugString("MsgServer_Load", "Line221 > " + rdstr);
//		debug_outputDebugString("MsgServer_Load", "Line223 > " + s_messageList[idx].receiverName);
	}
}

//-------------------------------------------------------------------------
// test functions

void Test_MsgServer_setupDummyMessages()
{
	int dmysiz = sizeof(s_dummyMsg) / sizeof(s_dummyMsg[0]);

	for(int idx = 0; idx < dmysiz; idx++) {
		MsgServer_PostMessage(
			 s_dummyMsg[idx].senderSerial
			,s_dummyMsg[idx].senderName
			,s_dummyMsg[idx].receiverName
			,s_dummyMsg[idx].message
			,s_dummyMsg[idx].isSecret
		);
	}

	String work;
	for(int idx = 0; idx < s_messageCount; idx++) {
		work = s_messageList[idx].senderSerial
			+ "," + s_messageList[idx].senderName
			+ "," + s_messageList[idx].receiverName
			+ "," + s_messageList[idx].message
			+ "," + s_messageList[idx].isSecret
			;
//		debug_outputDebugString("Test_MsgServer_setupDummyMessages", "Line79 > " + work);
	}
}

void Test_MsgServer_postThenGet()
{

	int dmysiz = sizeof(s_dummyMsg) / sizeof(s_dummyMsg[0]);

	String srl; // seraial of the sender
	String snder; // sender
	String rcver; // receiver
	String msg; // message
	bool isScr; // is secret message?

	// 1. post
	for(int idx = 0; idx < dmysiz; idx++) {
		srl = s_dummyMsg[idx].senderSerial;
		snder = s_dummyMsg[idx].senderName;
		rcver = s_dummyMsg[idx].receiverName;
		msg = s_dummyMsg[idx].message;
		isScr = s_dummyMsg[idx].isSecret;
		MsgServer_PostMessage(srl, snder, rcver, msg, isScr);
	}

	// 2. get
	// 2-1. check by 7of9
	String iam = "7of9";
	int msgCnt = MsgServer_GetMessageCount(iam);

	String msgToMe;
	for(int idx = 0; idx < msgCnt; idx++) {
		msgToMe = MsgServer_Get1stMessage(iam);
		debug_outputDebugString("Test_MsgServer_postThenGet", "Line214 > " + msgToMe);		
		MsgServer_Remove1stMessage(iam);
	}
	// 2-2. check by Vital
	iam = "Vital";
	msgCnt = MsgServer_GetMessageCount(iam);
	for(int idx = 0; idx < msgCnt; idx++) {
		msgToMe = MsgServer_Get1stMessage(iam);
		debug_outputDebugString("Test_MsgServer_postThenGet", "Line221 > " + msgToMe);		
		MsgServer_Remove1stMessage(iam);
	}
}

void Test_MsgServer_Clear()
{
	int dmysiz = sizeof(s_dummyMsg) / sizeof(s_dummyMsg[0]);

	String srl; // seraial of the sender
	String snder; // sender
	String rcver; // receiver
	String msg; // message
	bool isScr; // is secret message?

	// 1. post
	for(int idx = 0; idx < dmysiz; idx++) {
		srl = s_dummyMsg[idx].senderSerial;
		snder = s_dummyMsg[idx].senderName;
		rcver = s_dummyMsg[idx].receiverName;
		msg = s_dummyMsg[idx].message;
		isScr = s_dummyMsg[idx].isSecret;
		MsgServer_PostMessage(srl, snder, rcver, msg, isScr);
	}

	// 2. check
	String iam = "7of9";
	int msgCnt = MsgServer_GetMessageCount(iam);

	String msg1 = "Count:" + String(msgCnt);
	debug_outputDebugString("Test_MsgServer_Clear", "Line255 > " + msg1);	
	
	MsgServer_Clear(); // 

	msgCnt = MsgServer_GetMessageCount(iam);
	msg1 = "Count:" + String(msgCnt);
	debug_outputDebugString("Test_MsgServer_Clear", "Line259 > " + msg1);	
}

void Test_MsgServer_SaveLoad()
{
	Test_MsgServer_setupDummyMessages();

	MsgServer_Save();

	debug_outputDebugString("Test_MsgServer_SaveLoad", "Line325 > " + s_messageList[0].message);

	MsgServer_Clear();
	debug_outputDebugString("Test_MsgServer_SaveLoad", "Line327 > " + s_messageList[0].message);

	MsgServer_Load();
	debug_outputDebugString("Test_MsgServer_SaveLoad", "Line331 > " + s_messageList[0].message);
}
