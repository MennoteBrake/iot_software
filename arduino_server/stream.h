#ifndef stream_h
#define stream_h

#define TOKEN_LEN 20
#define TOKEN_ARR_LEN                                      \
  TOKEN_LEN + 1 // one more to accomodate for null char

struct stream {
  int (*available)();
  char (*peek)();
  char (*read)();
};

//door mij toegevoegd
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

enum parserState {
  EXPECT_METHOD,    // 0 GET | POST
  EXPECT_WS_1,      // 1 SP
  EXPECT_TARGET,    // 2 /id /data
  EXPECT_WS_2,      // 3 SP
  EXPECT_VERSION,   // 4 HTTP/1.0
  EXPECT_CRLF_CRLF, // 5 CRLFCRLF
  ERROR,            // 6
  DONE,             // 7
  EXPECT_CONTENT_LENGTH,//8
  EXPECT_WS_3,      //9
  EXPECT_LENGTH, // 10
  EXPECT_CRLF_CRLF_2, // 11
  EXPECT_CONTENT_BODY, // 12 CONTENT BODY
  EXPECT_CRLF_CRLF_3 // 13
};

struct request {
  enum { METHOD_UNKNOWN, METHOD_ERROR, GET, POST } method;
  enum { TARGET_UNKNOWN, DATA, ID } target;
};

struct token getNextToken(struct stream stream);
enum parserState parseNextToken(struct token tok, enum parserState curr, struct request* request);

int isNumber(char c);
int isLetter(char c);
int isOther(char c);

#endif
