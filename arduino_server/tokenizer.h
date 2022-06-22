#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "stream.h"

#define TOKEN_LEN 20
#define TOKEN_ARR_LEN                                      \
  TOKEN_LEN + 1 // one more to accomodate for null char

enum tokenType {
  UNEXPECTED,  // 0 unexpected character as start of token
  TOO_LONG,    // 1 too many characters in token
  UNAVAILABLE, // 2 no more characters in the stream
  WS,          // 3 series of space characters
  EOL,         // 4 series of end of line characters
  WORD,        // 5 series of lowercase characters
  NUMBER       // 6 series of number characters
};

struct token {
  enum tokenType type;
  char value[TOKEN_ARR_LEN];
};

struct token getNextToken(struct stream stream);

#endif