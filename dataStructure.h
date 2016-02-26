#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

/*
 * v0.2 2016 Feb. 27
 *   - add [CMD_ALL_CLEAR]
 */

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
  "clear", // not for normal use
};

enum tag_CMD_e {
  CMD_HELLO = 0,
  CMD_CHECK,
  CMD_GET,
  CMD_POST,
  CMD_BYE,
  CMD_ALL_CLEAR, // not for normal use
  // below is special index
  SIZE_CMD,
} CMD_e;

enum tag_POST_IDX_e {
  POST_IDX_COMMAND = 0,
  POST_IDX_RECEIVER,
  POST_IDX_MESSAGE,
  POST_IDX_IS_SECRET,
} POST_IDX_e;

#endif 