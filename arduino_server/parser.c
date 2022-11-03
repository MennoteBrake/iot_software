#include "parser.h"
#include "string.h"
#include <stdlib.h>

unsigned int charLenght = 0;

enum parserState parseNextToken(struct token tok, enum parserState curr, struct request* request, int* messageBody) {
  switch (tok.type) {
  case UNEXPECTED:
    return ERROR;
  case TOO_LONG:
    return ERROR;
  case UNAVAILABLE:
    return ERROR;
  default:;
  }

  switch (curr) {
  case EXPECT_METHOD:
    if (tok.type == WORD) {
      return expectMethod(tok, request);
    } else {
      return ERROR;
    }
  case EXPECT_WS_1:
    return expectWS(tok, request, EXPECT_WS_1);
  case EXPECT_TARGET:
    if (tok.type == WORD) {
      checkTarget(tok, request);
      return EXPECT_WS_2;
    } else {
      return ERROR;
    }
  case EXPECT_WS_2:
    return expectWS(tok, request, EXPECT_WS_2);
  case EXPECT_VERSION:
    if (tok.type == WORD && strncmp(tok.value, "HTTP/1.0", TOKEN_ARR_LEN) == 0) {
      return EXPECT_CRLF_CRLF;
    } else {
      return ERROR;
    }
  case EXPECT_CRLF_CRLF:
    return expectCTRL_CRLF(tok, request, EXPECT_CRLF_CRLF);
  case DONE:
    return DONE;
  case EXPECT_CONTENT_LENGTH:
    if (tok.type == WORD && strncmp(tok.value, "Content-Length:", TOKEN_ARR_LEN) == 0) {
      return EXPECT_WS_3;
    } else {
      return ERROR;
    }
  case EXPECT_WS_3:
    return expectWS(tok, request, EXPECT_WS_3);
  case EXPECT_LENGTH:
    if (tok.type == NUMBER) {
      charLenght = atoi(tok.value);
      return EXPECT_CRLF_CRLF_2;
    } else {
      return ERROR;
    }
  case EXPECT_CRLF_CRLF_2:
    return expectCTRL_CRLF(tok, request, EXPECT_CRLF_CRLF_2);
  case EXPECT_CONTENT_BODY:
    return expectContentBody(tok, request, messageBody);
  case EXPECT_CRLF_CRLF_3:
    return expectCTRL_CRLF(tok, request, EXPECT_CRLF_CRLF_3);
  default:
    return ERROR;
  }
}

void checkTarget(const struct token tok, struct request* request) {
  if (request->method == GET && strncmp(tok.value, "/sensors/1/avg", TOKEN_ARR_LEN) == 0) {
    request->target = AVG1;
  } else if (request->method == GET && strncmp(tok.value, "/sensors/2/avg", TOKEN_ARR_LEN) == 0) {
    request->target = AVG2;
  } else if (request->method == GET && strncmp(tok.value, "/sensors/1/stdev", TOKEN_ARR_LEN) == 0) {
    request->target = STDEV1;
  } else if (request->method == GET && strncmp(tok.value, "/sensors/2/stdev", TOKEN_ARR_LEN) == 0) {
    request->target = STDEV2;
  } else if (request->method == GET && strncmp(tok.value, "/sensors/1/actual", TOKEN_ARR_LEN) == 0) {
    request->target = ACTUAL1;
  } else if (request->method == GET && strncmp(tok.value, "/sensors/2/actual", TOKEN_ARR_LEN) == 0) {
    request->target = ACTUAL2;
  } else if (request->method == PUT && strncmp(tok.value, "/config/mode", TOKEN_ARR_LEN) == 0) {
    request->target = CONFIG_MODE;
  } else if (request->method == PUT && strncmp(tok.value, "/config/cbuffsize", TOKEN_ARR_LEN) == 0) {
    request->target = CONFIG_BUFF_SIZE;
  } else if (request->method == DELETE && strncmp(tok.value, "/sensors/1", TOKEN_ARR_LEN) == 0) {
    request->target = DEL_SENSOR1;
  } else if (request->method == DELETE && strncmp(tok.value, "/sensors/2", TOKEN_ARR_LEN) == 0) {
    request->target = DEL_SENSOR2;
  } else if (request->method == POST && strncmp(tok.value, "/sensors/1", TOKEN_ARR_LEN) == 0) {
    request->target = POST_SENSOR1;
  } else if (request->method == POST && strncmp(tok.value, "/sensors/2", TOKEN_ARR_LEN) == 0) {
    request->target = POST_SENSOR2;
  } else {
    request->target = TARGET_UNKNOWN;
  }
}

enum parserState expectMethod(struct token tok, struct request* request) {
  if (strncmp(tok.value, "GET", TOKEN_ARR_LEN) == 0) {
    request->method = GET;
    return EXPECT_WS_1;
  } else if (strncmp(tok.value, "POST", TOKEN_LEN - 1) == 0) {
    request->method = POST;
    return EXPECT_WS_1;
  } else if (strncmp(tok.value, "PUT", TOKEN_LEN - 1) == 0) {
    request->method = PUT;
    return EXPECT_WS_1;
  } else if (strncmp(tok.value, "DELETE", TOKEN_LEN - 1) == 0) {
    request->method = DELETE;
    return EXPECT_WS_1;
  } else {
    request->method = METHOD_UNKNOWN;
    return ERROR;
  }
}

enum parserState expectWS(struct token tok, struct request* request, enum parserState wsCounter) {
  if (tok.type == WS && strncmp(tok.value, " ", TOKEN_ARR_LEN) == 0) {
    switch (wsCounter) {
    case EXPECT_WS_1:
      return EXPECT_TARGET;
    case EXPECT_WS_2:
      return EXPECT_VERSION;
    case EXPECT_WS_3:
      return EXPECT_LENGTH;
    default:
      return ERROR;
    }
  } else {
    return ERROR;
  }
}

enum parserState expectCTRL_CRLF(struct token tok, struct request* request, enum parserState ctrlCounter) {
  if ((tok.type == EOL && strncmp(tok.value, "\r\n\r\n", TOKEN_ARR_LEN) == 0) ||
      (tok.type == EOL && strncmp(tok.value, "\r\n", TOKEN_ARR_LEN) == 0)) {
    switch (ctrlCounter) {
    case EXPECT_CRLF_CRLF:
      if (request->method == POST || request->method == PUT) {
        return EXPECT_CONTENT_LENGTH;
      } else {
        return DONE;
      }
    case EXPECT_CRLF_CRLF_2:
      return EXPECT_CONTENT_BODY;
    case EXPECT_CRLF_CRLF_3:
      return DONE;
    default:
      return ERROR;
    }
  } else {
    return ERROR;
  }
}

enum parserState expectContentBody(struct token tok, struct request* request, int* messageBody) {
  if (tok.type == WORD || tok.type == NUMBER) {
    if (charLenght == strlen(tok.value)) {
      if (tok.type == WORD) {
        if (strncmp(tok.value, "passive", TOKEN_ARR_LEN) == 0) {
          *messageBody = 1;
        } else if (strncmp(tok.value, "active", TOKEN_ARR_LEN) == 0) {
          *messageBody = 0;
        }
      }
      if (tok.type == NUMBER) {
        *messageBody = atoi(tok.value);
      }
      return DONE;
    } else {
      return ERROR;
    }
  } else {
    return ERROR;
  }
}