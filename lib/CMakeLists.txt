add_library(ring_buffer STATIC
        ring_buffer.c
        inc/ring_buffer.h
)
target_link_libraries(ring_buffer PRIVATE common)

add_library(state STATIC
        state.c
        inc/state.h
        )
target_link_libraries(state PRIVATE common)

add_library(can STATIC
        can.c
        inc/can.h
        )
target_link_libraries(can PRIVATE common)

add_library(message_processor STATIC
        message_processor.c
        inc/message_processor.h
        )
target_link_libraries(message_processor PRIVATE common)

