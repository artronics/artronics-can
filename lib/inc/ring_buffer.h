#ifndef ARTRONICS_RING_BUFFER_H
#define ARTRONICS_RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    size_t size_elem;
    size_t num_elem;
    void *buffer;
} RingBufferInit;

typedef unsigned int RingBufferHandler;

int RingBuffer_Init(RingBufferHandler *rbd, RingBufferInit *rb_init);
int RingBuffer_Put(RingBufferHandler rbd, const void *data);
int RingBuffer_Get(RingBufferHandler rbd, void *data);
int RingBuffer_Foo(int i);

#ifdef __cplusplus
}
#endif

#endif //ARTRONICS_RING_BUFFER_H
