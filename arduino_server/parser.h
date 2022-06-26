#ifndef parser_h
#define parser_h
#include "tokenizer.h"
#include <stdint.h>

enum parserState {
  EXPECT_METHOD,         // 0 GET | POST
  EXPECT_WS_1,           // 1 SP
  EXPECT_TARGET,         // 2
  EXPECT_WS_2,           // 3 SP
  EXPECT_VERSION,        // 4 HTTP/1.0
  EXPECT_CRLF_CRLF,      // 5 CRLFCRLF
  ERROR,                 // 6
  DONE,                  // 7 done
  EXPECT_CONTENT_LENGTH, // 8 content lenght
  EXPECT_WS_3,           // 9 SP
  EXPECT_LENGTH,         // 10 exected lenght
  EXPECT_CRLF_CRLF_2,    // 11 CRLFCRLF
  EXPECT_CONTENT_BODY,   // 12 CONTENT BODY
  EXPECT_CRLF_CRLF_3     // 13 CRLFCRLF
};

struct request {
  enum {
    METHOD_UNKNOWN,
    METHOD_ERROR,
    GET,
    POST,
    PUT,
    DELETE
  } method;
  enum {
    TARGET_UNKNOWN,
    AVG1,
    AVG2,
    STDEV1,
    STDEV2,
    ACTUAL1,
    ACTUAL2,
    CONFIG_MODE,
    CONFIG_BUFF_SIZE,
    DEL_SENSOR1,
    DEL_SENSOR2,
    POST_SENSOR1,
    POST_SENSOR2
  } target;
};

enum parserState parseNextToken(struct token tok,
                                enum parserState curr,
                                struct request* request, int* messageBody);

void checkTarget(const struct token tok,
                 struct request* request);

enum parserState expectMethod(struct token tok, struct request* request);
enum parserState expectWS(struct token tok, struct request* request, enum parserState wsCounter);
enum parserState expectCTRL_CRLF(struct token tok, struct request* request, enum parserState ctrlCounter);
enum parserState expectContentBody(struct token tok, struct request* request, int* messageBody);

int isNumber(char c);
int isLetter(char c);
int isOther(char c);

#endif