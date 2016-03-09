#ifndef MSG_STORAGE_H
#define MSG_STORAGE_H

// Message structure
typedef struct tag_message_t {
  String senderSerial; // serial number of the sender
  String senderName; // name of the sender
  String receiverName; // name of the receiver
  String message;
  bool isSecret; // sercret message or normal message
  String postDate; // date of the post
} message_t;


#endif 

