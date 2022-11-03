#include "tokenizer.h"
#include <string.h>

struct token getNextToken(struct stream stream) {
  struct token tok;
  memset(tok.value, '\0', TOKEN_ARR_LEN);

  if (!stream.available()) {
    tok.type = UNAVAILABLE;
    return tok;
  }

  int n = 0;

  if (stream.peek() == ' ') {
    tok.type = WS;
    while (stream.available()) {
      if (n == TOKEN_LEN) {
        tok.type = TOO_LONG;
        return tok;
      } else if (stream.peek() == ' ') {
        tok.value[n] = stream.read();
        n++;
      } else {
        return tok;
      }
    }
  } else if (isNumber(stream.peek())) {
    tok.type = NUMBER;
    while (stream.available()) {
      if (n == TOKEN_LEN) {
        tok.type = TOO_LONG;
        return tok;
      } else if (stream.peek() >= '0' && stream.peek() <= '9') {
        tok.value[n] = stream.read();
        n++;
      } else {
        return tok;
      }
    }
  } else if (isLetter(stream.peek()) || isOther(stream.peek())) {
    tok.type = WORD;
    while (stream.available()) {
      if (n == TOKEN_LEN) {
        tok.type = TOO_LONG;
        return tok;
      } else if (isLetter(stream.peek()) || isNumber(stream.peek()) || isOther(stream.peek())) {
        tok.value[n] = stream.read();
        n++;
      } else {
        return tok;
      }
    }
  } else if (stream.peek() == '\r' || stream.peek() == '\n') {
    tok.type = EOL;
    while (stream.available()) {
      if (n == TOKEN_LEN) {
        tok.type = TOO_LONG;
        return tok;
      } else if (stream.peek() == '\r' || stream.peek() == '\n') {
        tok.value[n] = stream.read();
        n++;
      } else {
        return tok;
      }
    }
  } else {
    tok.type = UNEXPECTED;
    return tok;
  }
  return tok;
}

int isNumber(char c) {
  return c >= '0' && c <= '9';
}

int isLetter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isOther(char c) {
  return c == '_' || c == '/' || c == '.' || c == '-' || c == ':';
}
