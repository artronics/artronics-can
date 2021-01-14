#ifndef ARTRONICS_RING_BUFFER_H
#define ARTRONICS_RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RING_BUFFER_MAX 1

typedef struct {
    size_t s_elem;
    size_t n_elem;
    void *buffer;
} rb_attr_t;

typedef uint32_t rbd_t;

uint32_t RingBuffer_Init(rbd_t *rbd, rb_attr_t *attr);

#ifdef __cplusplus
}
#endif

#endif //ARTRONICS_RING_BUFFER_H
