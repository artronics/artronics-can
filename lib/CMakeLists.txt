add_subdirectory(hal)

set(ART_SOURCES
        ring_buffer.c
        message_processor.c
        state.c
        can.c
        hal/stm32f0xx/hal_can.c
        hal/stm32f0xx/hal_module_id.c
        hal/stm32f0xx/hal_spi.c
        )

add_library(art ${ART_SOURCES})
target_compile_definitions(art PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32F042x6
        -DUSE_HAL_CAN_REGISTER_CALLBACKS=1
        -DRING_BUFFER_MAX=8
        )
target_link_libraries(art PRIVATE common)

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
