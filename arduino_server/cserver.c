#include "cserver.h"

#define MSG_SIZE 100

struct response handleRequest(struct stream stream) {
  struct response resp = {.code =
                              INTERNAL_SERVER_ERROR_500};

  return resp;
}
