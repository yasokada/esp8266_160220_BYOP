#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

// Information of owner
typedef struct tag_pizero_owner_t { 
  String serialNo;
  String nickName;
} Pizero_owner_t;

#if 0
// Message structure
typedef struct tag_message_t {
  String senderSerial; // serial number of the sender
  String senderName; // name of the sender
  String receiverName; // name of the receiver
  String message;
  bool isSecret; // sercrete message or normal message
} message_t;
#endif

String kCmdList[] = {
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


#endif 