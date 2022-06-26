#include "tokenizer.h"

struct token getNextToken(struct stream stream)
{
  // printf("getnexttoken: \n");
  struct token tok;
  memset(tok.value, '\0',
         TOKEN_ARR_LEN);  // zero all characters

  if (!stream.available())
  {
    tok.type = UNAVAILABLE;
    return tok;
  }

  int n = 0;

  if (stream.peek() == ' ')
  {
    printf("whitespace \n");
    tok.type = WS;
    while (stream.available())
    {
      if (n == TOKEN_LEN)
      {
        tok.type = TOO_LONG;
        return tok;
      }
      else if (stream.peek() == ' ')
      {
        tok.value[n] = stream.read();
        n++;
      }
      else
      {
        return tok;
      }
    }
  }
  else if (isNumber(stream.peek()))
  {
    printf("number \n");
    tok.type = NUMBER;
    while (stream.available())
    {
      if (n == TOKEN_LEN)
      {
        tok.type = TOO_LONG;
        return tok;
      }
      else if (stream.peek() >= '0' && stream.peek() <= '9')
      {
        tok.value[n] = stream.read();
        n++;
      }
      else
      {
        return tok;
      }
    }
  }
  else if (isLetter(stream.peek()) || isOther(stream.peek()))
  {
    printf("letter \n");
    tok.type = WORD;
    while (stream.available())
    {
      if (n == TOKEN_LEN)
      {
        tok.type = TOO_LONG;
        return tok;
      }
      else if (isLetter(stream.peek()) || isNumber(stream.peek()) || isOther(stream.peek()))
      {
        tok.value[n] = stream.read();
        n++;
      }
      else
      {
        return tok;
      }
    }
  }
  else if (stream.peek() == '\r' || stream.peek() == '\n')
  {
    tok.type = EOL;
    while (stream.available())
    {
      if (n == TOKEN_LEN)
      {
        tok.type = TOO_LONG;
        return tok;
      }
      else if (stream.peek() == '\r' || stream.peek() == '\n')
      {
        tok.value[n] = stream.read();
        n++;
      }
      else
      {
        printf("eol \n");
        return tok;
      }
    }
  }
  else
  { /* unexpected begin of token */
    tok.type = UNEXPECTED;
    return tok;
  }
  return tok;
}