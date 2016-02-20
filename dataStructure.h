#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

// Information of owner
typedef struct tag_pizero_owner_t { 
  String serialNo;
  String nickName;
} Pizero_owner_t;

// Message structure
typedef struct tag_message_t {
  String serialSender; // serial number of the sender
  String msgSender; // name of the sender
  String msgReceiver; // name of the receiver
  String message;
  bool isSecret; // sercrete message or normal message
} message_t;

#endif 