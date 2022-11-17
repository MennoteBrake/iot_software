#include "circularbuffer.h"
#include <stddef.h>
#include <stdlib.h>

cbuffer* cbInit(int8_t size, enum cbmode mode) {
  cbuffer* var = malloc(sizeof(cbuffer));

  if (!var) {
    return NULL;
  }
  var->data = malloc(size * sizeof(cbtype));
  if (!var->data) {
    cbFree(var);
    return NULL;
  }
  var->mode = mode;
  var->size = size;
  var->start = 0;
  var->count = 0;

  return var;
}

cbuffer* cbFree(cbuffer* buffer) {

  if (buffer == NULL) {
    return NULL;
  }

  free(buffer->data);
  buffer->data = NULL;
  free(buffer);

  return NULL;
}

int cbAvailable(cbuffer* buffer) {
  if (buffer == NULL)
    return 0;
  if (buffer->count == 0) {
    return 0;
  } else {
    return 1;
  }
}

cbtype cbPeek(cbuffer* buffer) {
  if (buffer == NULL)
    return 0;
  return buffer->data[buffer->start];
}

cbtype cbRead(cbuffer* buffer) {
  if (buffer == NULL){
    return 0;
  }
  cbtype bufferVar = buffer->data[buffer->start];
  buffer->start++;
  buffer->count--;

  if (buffer->start >= buffer->size) {
    buffer->start = 0;
  }
  return bufferVar;
}

int8_t cbAdd(cbuffer* buffer, cbtype value) {
  if (buffer == NULL) {
    return 0;
  }
  uint8_t pos = buffer->start + buffer->count;

  if (buffer->count >= buffer->size) {
    if (buffer->mode == OVERWRITE_IF_FULL) {
      buffer->data[buffer->start] = value;
      buffer->start = (buffer->start >= buffer->size) ? 0 : buffer->start + 1;
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

cbtype cbPeekPos(cbuffer* buffer, int position) {
  if (buffer == NULL){
    return 0;
  }
  if (position >= buffer->size) {
    position = buffer->size - 1;
  }

  uint8_t pos = buffer->start + position;

  if (pos >= buffer->size) {
    pos -= buffer->size;
  }
  return buffer->data[pos];
}