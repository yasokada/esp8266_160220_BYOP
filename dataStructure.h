#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

// Information of owner
typedef struct tag_pizero_owner_t { 
  String serialNo;
  String nickName;
} Pizero_owner_t;

String kCmdList[] = {
  "hello",
  "check",
  "get",
  "post",
  "bye",
};

enum tag_CMD_e {
  CMD_HELLO = 0,
  CMD_CHECK,
  CMD_GET,
  CMD_POST,
  CMD_BYE,
  // below is special index
  SIZE_CMD = CMD_BYE + 1,
} CMD_e;

enum tag_POST_IDX_e {
  POST_IDX_COMMAND = 0,
  POST_IDX_RECEIVER,
  POST_IDX_MESSAGE,
  POST_IDX_IS_SECRET,
} POST_IDX_e;


#endif 