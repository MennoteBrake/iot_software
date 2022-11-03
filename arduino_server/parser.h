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

/**
 * @brief parses the next token
 * 
 * @param tok The type of token
 * @param curr The current state
 * @param request The request
 * @param messageBody the message body
 * @return enum parserState 
 */
enum parserState parseNextToken(struct token tok, enum parserState curr, struct request* request, int* messageBody);

/**
 * @brief Checks the target of the request
 * 
 * @param tok The token to check
 * @param request The request
 */
void checkTarget(const struct token tok, struct request* request);

/**
 * @brief Checks for a known method
 * 
 * @param tok The token
 * @param request The request
 * @return enum parserState 
 */
enum parserState expectMethod(struct token tok, struct request* request);

/**
 * @brief Checks for a whitespace
 * 
 * @param tok The token
 * @param request The request
 * @param wsCounter The amount of whitespaces already counted
 * @return enum parserState 
 */
enum parserState expectWS(struct token tok, struct request* request, enum parserState wsCounter);

/**
 * @brief Checks for a CTRL CTRL
 * 
 * @param tok The token
 * @param request The request
 * @param ctrlCounter The amount of ctrl's already counted
 * @return enum parserState 
 */
enum parserState expectCTRL_CRLF(struct token tok, struct request* request, enum parserState ctrlCounter);

/**
 * @brief Checks for a content body
 * 
 * @param tok The token
 * @param request The request
 * @param messageBody The message body
 * @return enum parserState 
 */
enum parserState expectContentBody(struct token tok, struct request* request, int* messageBody);
#endif