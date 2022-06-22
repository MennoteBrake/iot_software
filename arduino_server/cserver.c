#include "cserver.h"

#include <stdio.h>

#define MSG_SIZE 100

// struct response handleRequest(struct stream stream) {

//   struct response resp = {.code =
//                               INTERNAL_SERVER_ERROR_500};

//   // getNextToken(struct stream stream);
//   // enum parserState parseNextToken(struct token tok,
//   //                               enum parserState curr,
//   //                               struct request*
//   request); return resp;
// }

struct response handleRequest(struct stream stream) {
  printf("Characters: %c \n");
  enum parserState state = EXPECT_METHOD;
  struct request request = {.method = METHOD_UNKNOWN,
                            .target = TARGET_UNKNOWN};
  struct response response = {INTERNAL_SERVER_ERROR_500,
                              51};

  char msg[MSG_SIZE] = {0};

  int done = 0;

  while (!done) {
    struct token token = getNextToken(stream);

#ifndef NDEBUG
    snprintf(msg, MSG_SIZE,
             "* token type %d, token value (\"%s\")\n",
             token.type, token.value);
//    logmsg/(msg);
#endif

    enum parserState next =
        parseNextToken(token, state, &request);

#ifndef NDEBUG
    snprintf(msg, MSG_SIZE, "* parser state %d\n", next);
//    logms/g(msg);
#endif

    done =
        state ==
        DONE; // readyToRespond(state, &request, &response);
    state = next;
  }

  return makeResponse(&request);
}

// struct response makeResponse(struct request* request)
// {
//   switch (request->method)
//   {
//     case GET:

//       break;
//     case POST:

//       break;
//     case PUT:

//       break;
//     case DELETE:

//       break;

//     default:
//       break;
//   }
// }

struct response makeResponse(struct request* request) {
  struct response response;
  switch (request->target) {
  case TARGET_UNKNOWN:
    // struct response response = { .code = NOT_FOUND_404,
    // .get_actual = 0 }; return response;
    break;
  case AVG1:
    // struct response response; // = { .code =
    // OK_200_GET_AVG, .get_avg = 51 };
    response.code = OK_200_GET_AVG;
    response.get_avg = 3;
    return response;
    // return OK_200_GET_AVG;
    response.code = OK_200_GET_AVG;
    response.get_avg = 3;
    return response;
    break;
  case AVG2:
    // return OK_200_GET_AVG;
    break;
  case STDEV1:
    // return OK_200_GET_STDEV;
    break;
  case STDEV2:
    // return OK_200_GET_STDEV;
    break;
  case ACTUAL1:
    // return OK_200_GET_ACTUAL;
    break;
  case ACTUAL2:
    // return OK_200_GET_ACTUAL;
    break;
  default:
    // return INTERNAL_SERVER_ERROR_500;
    break;
  }
}

// int readyToRespond(enum parserState, const struct
// request*,
//                    struct response*){
// return 0;
// }
