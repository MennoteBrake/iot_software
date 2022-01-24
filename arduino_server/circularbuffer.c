#include "circularbuffer.h"
#include <stddef.h>
#include <stdlib.h>

cbuffer* cbInit(int8_t size, enum cbmode mode) {

  cbuffer* var = malloc(sizeof(cbtype));

  if (!var) {
    return NULL; // moet null returnen.
  }
  var->data = malloc(size * sizeof(cbtype));
  if (!var->data){
    cbFree(var);//net toegevoegd.
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
  return &buffer;
}

int cbAvailable(cbuffer* buffer) {
	if(buffer->count == 0){
		return 0;
	}else {
		return 1;
	}
}

cbtype cbPeek(cbuffer* buffer) { 
	return buffer->data[buffer->start]; 
}

cbtype cbRead(cbuffer* buffer) {
  cbtype bufferVar = buffer->data[buffer->start];
  buffer->data[buffer->start] = NULL;
  buffer->start += 1;
  buffer->count -= 1;
  if(buffer->start == buffer->size){
	  buffer->start = 0;
  }
  return bufferVar;
}

int8_t cbAdd(cbuffer* buffer, cbtype value) {
  if (buffer->count >= buffer->size) {
    if (buffer->mode == OVERWRITE_IF_FULL) {
      buffer->data[buffer->start] = value;
      buffer->start += 1;
      if (buffer->start >= buffer->size) {
        buffer->start = 0;
      }
      return 1;
    } else {
      return 0;
    }

  } else {
    buffer->data[(buffer->start + buffer->count )%buffer->size] = value;
    buffer->count += 1;
    return 1;
  }
}
