include_directories(inc)
add_library(ring_buffer
        ring_buffer.c
)

target_compile_definitions(ring_buffer PRIVATE
        -DRING_BUFFER_MAX=20
        )
