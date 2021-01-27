#ifndef ARTRONICS_RING_BUFFER_H
#define ARTRONICS_RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RING_BUFFER_ERROR -1
#define RING_BUFFER_OK 0

typedef struct {
    size_t size_elem;
    size_t num_elem;
    void *buffer;
} RingBufferInit;

typedef unsigned int RingBufferHandler;

int RingBuffer_Init(RingBufferHandler *rbd, RingBufferInit *rb_init);
int RingBuffer_Put(RingBufferHandler rbd, const void *data);
int RingBuffer_Get(RingBufferHandler rbd, void *data);

#ifdef __cplusplus
}
#endif

#endif //ARTRONICS_RING_BUFFER_H
