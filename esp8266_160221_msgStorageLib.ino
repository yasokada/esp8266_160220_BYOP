
// String library

#include "dataStructure.h"

/*
 * v0.3 2016 Feb. 23
 *  - 
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
	{ "001", "7of9", "Vital", "hello,Vital", 0 },
	{ "007", "Vital", "7of9", "hello,7of9", 0 },
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

bool MsgServer_PostMessage(String snder, String rcver, String msg)
{
	// TODO: 0m > add message 
}


//-------------------------------------------------------------------------
// test functions

void Test_MsgServer_setupDummyMessages()
{
	int size = sizeof(s_dummyMsg) / sizeof(s_dummyMsg[0]);

	for(int idx = 0; idx < size; idx++) {
		s_messageList[idx].senderSerial = s_dummyMsg[idx].senderSerial;
		s_messageList[idx].senderName 	= s_dummyMsg[idx].senderName;
		s_messageList[idx].receiverName = s_dummyMsg[idx].receiverName;
		s_messageList[idx].message 		= s_dummyMsg[idx].message;
		s_messageList[idx].isSecret 	= s_dummyMsg[idx].isSecret;
	}

	String work;
	for(int idx = 0; idx < size; idx++) {
		work = s_messageList[idx].senderSerial
			+ "," + s_messageList[idx].senderName
			+ "," + s_messageList[idx].receiverName
			+ "," + s_messageList[idx].message
			+ "," + s_messageList[idx].isSecret
			;
		debug_outputDebugString("Test_MsgServer_setupDummyMessages", "Line79 > " + work);
	}
}