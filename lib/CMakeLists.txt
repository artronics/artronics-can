include_directories(inc)
add_library(ring_buffer STATIC
        ring_buffer.c
)
target_link_libraries(ring_buffer PRIVATE common)

target_compile_definitions(ring_buffer INTERFACE
        -DRING_BUFFER_MAX=32
        )
