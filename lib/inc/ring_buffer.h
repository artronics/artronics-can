#ifndef ARTRONICS_RING_BUFFER_H
#define ARTRONICS_RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RING_BUFFER_MAX 1

typedef struct {
    size_t size_elem;
    size_t num_elem;
    void *buffer;
} rb_attr_t;

typedef unsigned int rbd_t;

int RingBuffer_Init(rbd_t *rbd, rb_attr_t *attr);
int RingBuffer_Put(rbd_t rbd, const void *data);
int RingBuffer_Get(rbd_t rbd, void *data);

#ifdef __cplusplus
}
#endif

#endif //ARTRONICS_RING_BUFFER_H
