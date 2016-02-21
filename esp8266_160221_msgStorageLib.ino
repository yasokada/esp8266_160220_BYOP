
// String library

#include "dataStructure.h"

/*
 * v0.2 2016 Feb. 22
 *  - add s_messageList[], [s_messageCount]
 *  - add Test_MsgServer_setupDummyMessages()
 * v0.1 2016 Feb. 21
 *  - prepare empty functions
 */

//-------------------------------------------------------------------------
// file scope static functions / variables

static const int kMaxnum_MessageCount = 10; // TODO: 0m > check memory size
static message_t s_messageList[kMaxnum_MessageCount];
static int s_messageCount = 0;
//-------------------------------------------------------------------------

void MsgServer_Clear()
{

}

int MsgServer_GetCount_ReceiverOf(String rcver)
{

}

int MsgServer_GetIndex_ReceiverOf(String rcver)
{

}

String MsgServer_GetMessage(String rcver, int idx)
{

}

void MsgServer_PostMessage(String snder, String rcver, String msg)
{

}

void MsgServer_RemoveMessage(int idx)
{

}

void Test_MsgServer_setupDummyMessages()
{

}