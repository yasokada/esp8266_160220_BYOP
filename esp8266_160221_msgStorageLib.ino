
// String library

#include "msgStorage.h"

/*
 * v0.6 2016 Feb. 23
    - impl MsgServer_PostMessage()
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

static const int kMaxnum_MessageCount = 10; // TODO: 0m > check memory size
static message_t s_messageList[kMaxnum_MessageCount];
static int s_messageCount = 0;
//-------------------------------------------------------------------------

static const message_t s_dummyMsg[] =
{
	{ "000000000000000d", "7of9", "Vital", "hello,Vital", 0 },
	{ "000000000000002g", "Vital", "7of9", "hello,7of9", 0 },
	{ "000000000000000d", "7of9", "Vital", "meet at the Arctic", 1 },
	{ "000000000000002g", "Vital", "7of9", "No,thank you.", 1 },
};
//-------------------------------------------------------------------------


void MsgServer_Clear()
{
}

int MsgServer_GetMessageCount(String rcver)
{
	// TODO: 0m > return count of message for the receiver
}

String MsgServer_Get1stMessage(String rcver)
{
	// TODO: 0m > return 1st message for the receiver
}

void MsgServer_Remove1stMessage(String rcver)
{
	// TODO: 0m > remove 1st message for the receiver
}

bool MsgServer_PostMessage(String srl, String snder, String rcver, String msg, bool isSecret)
{
	if (s_messageCount >= (kMaxnum_MessageCount - 1)) {
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


//-------------------------------------------------------------------------
// test functions

void Test_MsgServer_setupDummyMessages()
{
	int dmysiz = sizeof(s_dummyMsg) / sizeof(s_dummyMsg[0]);

	for(int idx = 0; idx < dmysiz; idx++) {
		s_messageList[idx].senderSerial = s_dummyMsg[idx].senderSerial;
		s_messageList[idx].senderName 	= s_dummyMsg[idx].senderName;
		s_messageList[idx].receiverName = s_dummyMsg[idx].receiverName;
		s_messageList[idx].message 		= s_dummyMsg[idx].message;
		s_messageList[idx].isSecret 	= s_dummyMsg[idx].isSecret;
	}

	String work;
	for(int idx = 0; idx < dmysiz; idx++) {
		work = s_messageList[idx].senderSerial
			+ "," + s_messageList[idx].senderName
			+ "," + s_messageList[idx].receiverName
			+ "," + s_messageList[idx].message
			+ "," + s_messageList[idx].isSecret
			;
		debug_outputDebugString("Test_MsgServer_setupDummyMessages", "Line79 > " + work);
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
		debug_outputDebugString("Test_MsgServer_postThenGet", "Line125 > " + msgToMe);		
		MsgServer_Remove1stMessage(iam);
	}
	// 2-2. check by Vital
	iam = "Vital";
	for(int idx = 0; idx < msgCnt; idx++) {
		msgToMe = MsgServer_Get1stMessage(iam);
		debug_outputDebugString("Test_MsgServer_postThenGet", "Line125 > " + msgToMe);		
		MsgServer_Remove1stMessage(iam);
	}

	// TODO: 0m > use MsgServer_Clear();
}