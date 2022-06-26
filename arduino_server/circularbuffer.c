#include "circularbuffer.h"
#include <stddef.h>
#include <stdlib.h>

cbuffer* cbInit(int8_t size, enum cbmode mode) {

  cbuffer* var = malloc(sizeof(cbtype));

  if (!var) {
    return NULL; // moet null returnen.
  }
  var->data = malloc(size * sizeof(cbtype));
  if (!var->data) {
    cbFree(var);
    return NULL;
  }
  var->mode = mode;
  var->size = size;
  var->count = 0;
  var->start = 0;

  return var;
}

cbuffer* cbFree(cbuffer* buffer) {
  free(buffer);
  return NULL;
}

int cbAvailable(cbuffer* buffer) {
  if (buffer->count == 0) {
    return 0;
  } else {
    return 1;
  }
}

cbtype cbPeek(cbuffer* buffer) {
  return buffer->data[buffer->start];
}

cbtype cbRead(cbuffer* buffer) {
  cbtype bufferVar = buffer->data[buffer->start];
  buffer->start++;
  buffer->count--;

  if (buffer->start >= buffer->size) {
    buffer->start = 0;
  }
  return bufferVar;
}

int8_t cbAdd(cbuffer* buffer, cbtype value) {
  uint8_t pos = buffer->start + buffer->count;

  if (buffer->count >= buffer->size) {
    if (buffer->mode == OVERWRITE_IF_FULL) {
      buffer->data[buffer->start] = value;
      buffer->start = (buffer->start >= buffer->size)
                          ? 0
                          : buffer->start + 1;
      return 1;
    } else {
      return 0;
    }
  } else {
    if (pos >= buffer->size) {
      pos -= buffer->size;
    }

    *(buffer->data + pos) = value;
    buffer->count++;
    return 1;
  }
}