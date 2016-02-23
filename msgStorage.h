#ifndef MSG_STORAGE_H
#define MSG_STORAGE_H

// Message structure
typedef struct tag_message_t {
  String senderSerial; // serial number of the sender
  String senderName; // name of the sender
  String receiverName; // name of the receiver
  String message;
  bool isSecret; // sercrete message or normal message
} message_t;


#endif 



// TODO: 0m > remove below ( used for reference in implementation)
/*
static const int kMaxnum_MessageCount = 10; // TODO: 0m > check memory size
static message_t s_messageList[kMaxnum_MessageCount];
static int s_messageCount = 0;
*/