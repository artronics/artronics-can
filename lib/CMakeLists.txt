add_library(ring_buffer STATIC
        ring_buffer.c
        inc/ring_buffer.h
)
target_link_libraries(ring_buffer PRIVATE common)
