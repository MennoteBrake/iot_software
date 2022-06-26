#include "parser.h"
#include "string.h"

unsigned int charLenght = 0;

enum parserState parseNextToken(struct token tok, enum parserState curr, struct request* request)
{
  switch (tok.type)
  {
    case UNEXPECTED:
      return ERROR;
    case TOO_LONG:
      return ERROR;
    case UNAVAILABLE:
      return ERROR;
    default:;
  }

  switch (curr)
  {
    case EXPECT_METHOD:
      if (tok.type == WORD)
      {
        printf("word \n");
        // compare strings up till last null char
        if (strncmp(tok.value, "GET", TOKEN_ARR_LEN) == 0)
        {
          printf("GET \n");
          request->method = GET;
          return EXPECT_WS_1;
        }
        else if (strncmp(tok.value, "POST", TOKEN_LEN - 1) == 0)
        {
          printf(" POST \n");
          request->method = POST;
          return EXPECT_WS_1;
        }
        else if (strncmp(tok.value, "PUT", TOKEN_LEN - 1) == 0)
        {
          printf("PUT \n");
          request->method = PUT;
          return EXPECT_WS_1;
        }
        else
        {
          request->method = METHOD_ERROR;
          return ERROR;
        }
      }
      else
      {
        return ERROR;
      }
    case EXPECT_WS_1:
      if (tok.type == WS && strncmp(tok.value, " ", TOKEN_ARR_LEN) == 0)
      {
        printf("WS \n");
        return EXPECT_TARGET;
      }
      else
      {
        return ERROR;
      }
    case EXPECT_TARGET:
      if (tok.type == WORD)
      {
        printf("expect target %i \n", tok.type);
        checkTarget(tok, request);
        return EXPECT_WS_2;
      }
      else
      {
        return ERROR;
      }
    case EXPECT_WS_2:
      if (tok.type == WS && strncmp(tok.value, " ", TOKEN_ARR_LEN) == 0)
      {
        printf("WS2 \n");
        return EXPECT_VERSION;
      }
      else
      {
        return ERROR;
      }
    case EXPECT_VERSION:
      if (tok.type == WORD && strncmp(tok.value, "HTTP/1.0", TOKEN_ARR_LEN) == 0)
      {
        printf("EXPECTED VERION \n");
        return EXPECT_CRLF_CRLF;
      }
      else
      {
        return ERROR;
      }
    case EXPECT_CRLF_CRLF:
      if (tok.type == EOL && strncmp(tok.value, "\r\n\r\n", TOKEN_ARR_LEN) == 0)
      {
        printf("EXPECT CRLF CRLF \n");
        if (request->method == POST)
        {
          return EXPECT_CONTENT_LENGTH;
        }
        else
        {
          return DONE;
        }
      }
      else
      {
        return ERROR;
      }
    case DONE:
      printf("DONE \n");
      return DONE;
    case EXPECT_CONTENT_LENGTH:
      if (tok.type == WORD && strncmp(tok.value, "Content-length:", TOKEN_ARR_LEN) == 0)
      {
        printf("CONTENT LENGTH \n");
        return EXPECT_WS_3;
      }
      else
      {
        return ERROR;
      }
    case EXPECT_WS_3:
      if (tok.type == WS && strncmp(tok.value, " ", TOKEN_ARR_LEN) == 0)
      {
        printf("EXPECT WS3 \n");
        return EXPECT_LENGTH;
      }
      else
      {
        return ERROR;
      }
    case EXPECT_LENGTH:

      if (tok.type == NUMBER)
      {
        printf("EXPECT LENGTH \n");
        charLenght = atoi(tok.value);
        return EXPECT_CRLF_CRLF_2;
      }
      else
      {
        return ERROR;
      }
    case EXPECT_CRLF_CRLF_2:
      if (tok.type == EOL && strncmp(tok.value, "\r\n\r\n", TOKEN_ARR_LEN) == 0)
      {
        printf("EXPECT CTRL CRLF 2 \n");
        return EXPECT_CONTENT_BODY;
      }
      else
      {
        return ERROR;
      }
    case EXPECT_CONTENT_BODY:
      if (tok.type == WORD || tok.type == NUMBER)
      {
        printf("EXPECT CONTENT BODY \n");
        charLenght = charLenght - strlen(tok.value);
        if (charLenght > 0)
        {
          return EXPECT_CONTENT_BODY;
        }
        else
        {
          return EXPECT_CRLF_CRLF_3;
        }
      }
      else
      {
        return ERROR;
      }
    case EXPECT_CRLF_CRLF_3:
      if (tok.type == EOL && strncmp(tok.value, "\r\n\r\n", TOKEN_ARR_LEN) == 0)
      {
        printf("EXPECT CTRL CRLF 3 \n");
        return DONE;
      }
      else
      {
        return ERROR;
      }
    default:
      return ERROR;
  }
}

// mem isue met tok.type

void checkTarget(const struct token tok, struct request* request)
{
  // printf("check target  \n");
  printf("expect target %i \n", tok.type);
  printf("expect target %i \n", request->method);

  if (request->method == GET && strncmp(tok.value, "/sensors/1/avg", TOKEN_ARR_LEN) == 0)
  {
    // printf("AVG SENSOR1  \n");
    request->target = AVG1;
    printf("AVG SENSOR1 %i \n", AVG1);
    printf("req target %i \n", request->target);
  }
  else if (request->method == GET && strncmp(tok.value, "/sensors/1/avg", TOKEN_ARR_LEN) == 0)
  {
    printf("AVG SENSOR2 %i \n", AVG2);
    request->target = AVG2;
    printf("req target %i \n", request->target);
  }
  //   else if(request->method == GET && strncmp(tok->value,
  //   "/sensors/1/stdev", TOKEN_ARR_LEN) == 0){
  //     // request->target = STDEV1;
  //   }
  //    else if(request->method == GET &&
  //    strncmp(tok->value  //    else
  //    if(strncmp(request->method == GET &&,
  //    "/sensors/2/stdev", TOKEN_ARR_LEN) == 0){
  //     request->target = STDEV2;
  //   }
  //    else if(strncmp(request->method == GET &&
  //    tok->value, "/sensors/1/actual", TOKEN_ARR_LEN) ==
  //    0){
  //     request->target = ACTUAL1;
  //   }
  //    else if(strncmp(request->method == GET &&
  //    tok->value, "/sensors/2/actual", TOKEN_ARR_LEN) ==
  //    0){
  //     request->target = ACTUAL2;
  //   }
  //   else if(strncmp(request->method == PUT && tok->value,
  //   "/config/mode", TOKEN_ARR_LEN) == 0){
  //     request->target = CONFIG_MODE;
  //   }
  //   else if(strncmp(request->method == PUT && tok->value,
  //   "/config/cbuffsize", TOKEN_ARR_LEN) == 0){
  //     request->target = CONFIG_BUFF_SIZE;
  //   }
  //   else if(strncmp(request->method == DELETE &&
  //   tok->value, "/sensors/1 ", TOKEN_ARR_LEN) == 0){
  //     request->target = POST_SENSOR1;
  //   }
  //   else if(strncmp(request->method == DELETE &&
  //   tok->value, "/sensors/2 ", TOKEN_ARR_LEN) == 0){
  //     request->target = POST_SENSOR2;
  //   }
  //   else if(strncmp(request->method == POST &&
  //   tok->value, "/sensors/1", TOKEN_ARR_LEN) == 0){
  //     request->target = POST_SENSOR1;
  //   }
  //   else if(strncmp(request->method == POST &&
  //   tok->value, "/sensors/1", TOKEN_ARR_LEN) == 0){
  //     request->target = POST_SENSOR2;
  //   }
  else
  {
    printf("target unknown  \n");
    request->target = TARGET_UNKNOWN;
  }
}

int isNumber(char c)
{
  return c >= '0' && c <= '9';
}

int isLetter(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isOther(char c)
{
  return c == '_' || c == '/' || c == '.' || c == '-' || c == ':';
}
