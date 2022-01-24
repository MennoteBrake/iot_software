#include "stream.h"

unsigned int charLenght = 0;

struct token getNextToken(struct stream stream) {
  struct token tok;
  memset(tok.value, '\0',
         TOKEN_ARR_LEN); // zero all characters

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
      } else if (stream.peek() >= '0' &&
                 stream.peek() <= '9') {
        tok.value[n] = stream.read();
        n++;
      } else {
        return tok;
      }
    }
  } else if (isLetter(stream.peek()) ||
             isOther(stream.peek())) {
    tok.type = WORD;
    while (stream.available()) {
      if (n == TOKEN_LEN) {
        tok.type = TOO_LONG;
        return tok;
      } else if (isLetter(stream.peek()) ||
                 isNumber(stream.peek()) ||
                 isOther(stream.peek())) {
        tok.value[n] = stream.read();
        n++;
      } else {
        return tok;
      }
    }
  } else if (stream.peek() == '\r' ||
             stream.peek() == '\n') {
    tok.type = EOL;
    while (stream.available()) {
      if (n == TOKEN_LEN) {
        tok.type = TOO_LONG;
        return tok;
      } else if (stream.peek() == '\r' ||
                 stream.peek() == '\n') {
        tok.value[n] = stream.read();
        n++;
      } else {
        return tok;
      }
    }
  } else { /* unexpected begin of token */
    tok.type = UNEXPECTED;
    return tok;
  }
  return tok;
}

enum parserState parseNextToken(struct token tok, enum parserState curr, struct request* request) {
  switch (tok.type) {
    case UNEXPECTED:
          return ERROR;
      case TOO_LONG:
        return ERROR;
      case UNAVAILABLE:
        return ERROR;
      default:; // prevent warning as other states are treated
        // below
    }

    switch (curr) {
    case EXPECT_METHOD:
      if (tok.type == WORD) {
        // compare strings up till last null char
        if (strncmp(tok.value, "GET", TOKEN_ARR_LEN) == 0) {
          request->method = GET;
          return EXPECT_WS_1;
        } else if (strncmp(tok.value, "POST",
                           TOKEN_LEN - 1) == 0) {
          request->method = POST;
          return EXPECT_WS_1;
        } 
        else if (strncmp(tok.value, "PUT",
                           TOKEN_LEN - 1) == 0) {
          request->method = POST;
          return EXPECT_WS_1;
        } else {
          request->method = METHOD_ERROR;
          return ERROR;
        }
      } else {
        return ERROR;
      }
    case EXPECT_WS_1:
      if (tok.type == WS &&
          strncmp(tok.value, " ", TOKEN_ARR_LEN) == 0) {
        return EXPECT_TARGET;
      } else {
        return ERROR;
      }
    case EXPECT_TARGET:
      if (tok.type == WORD) {
        if (strncmp(tok.value, "/data", TOKEN_ARR_LEN) == 0) {
          request->target = DATA;
        } else if (strncmp(tok.value, "/id", TOKEN_ARR_LEN) ==
                   0) {
          request->target = ID;
        } else {
          request->target = TARGET_UNKNOWN;
        }
        return EXPECT_WS_2;
      } else {
        return ERROR;
      }
    case EXPECT_WS_2:
      if (tok.type == WS &&
          strncmp(tok.value, " ", TOKEN_ARR_LEN) == 0) {
        return EXPECT_VERSION;
      } else {
        return ERROR;
      }
    case EXPECT_VERSION:
      if (tok.type == WORD && strncmp(tok.value, "HTTP/1.0",
                                      TOKEN_ARR_LEN) == 0) {
        return EXPECT_CRLF_CRLF;
      } else {
        return ERROR;
      }
    case EXPECT_CRLF_CRLF:
      if (tok.type == EOL && strncmp(tok.value, "\r\n\r\n",
                                     TOKEN_ARR_LEN) == 0) {

        if (request->method == POST) {
          return EXPECT_CONTENT_LENGTH;
        } else {
          return DONE;
        }
      } else {
        return ERROR;
      }
    case DONE:
      return DONE;
    case EXPECT_CONTENT_LENGTH:
      if (tok.type == WORD && strncmp(tok.value, "Content-length:", TOKEN_ARR_LEN) == 0) {
        return EXPECT_WS_3;
      } else {
        return ERROR;
      }
    case EXPECT_WS_3:
      if (tok.type == WS && strncmp(tok.value, " ", TOKEN_ARR_LEN) == 0) {
        return EXPECT_LENGTH;
      } else {
        return ERROR;
      }
    case EXPECT_LENGTH:
      if (tok.type == NUMBER) {
        charLenght = atoi(tok.value);
        return EXPECT_CRLF_CRLF_2;
      } else {
        return ERROR;
      }
    case EXPECT_CRLF_CRLF_2:
      if (tok.type == EOL && strncmp(tok.value, "\r\n\r\n", TOKEN_ARR_LEN) == 0) {
        return EXPECT_CONTENT_BODY;
      } else {
        return ERROR;
      }
    case EXPECT_CONTENT_BODY:
      if (tok.type == WORD || tok.type == NUMBER) {
        charLenght = charLenght - strlen(tok.value);
        if (charLenght > 0) {
          return EXPECT_CONTENT_BODY;
        } else {
          return EXPECT_CRLF_CRLF_3;
        }
      } else {
        return ERROR;
      }
    case EXPECT_CRLF_CRLF_3:
      if (tok.type == EOL && strncmp(tok.value, "\r\n\r\n", TOKEN_ARR_LEN) == 0) {
        return DONE;
      } else {
        return ERROR;
      }
    default:
      return ERROR;
  }
}

int isNumber(char c) {
  return c >= '0' && c <= '9';
}

int isLetter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isOther(char c) {
  return c == '_' || c == '/' || c == '.' || c == '-' || c == ':' ;
}
