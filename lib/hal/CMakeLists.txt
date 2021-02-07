set(hal_arch stm32f0xx)

add_library(hal_can STATIC
        ${hal_arch}/hal_can.c
        ../inc/hal/hal_can.h
        )
target_link_libraries(hal_can PRIVATE common)

add_library(hal_module_id STATIC
        ${hal_arch}/hal_module_id.c
        ../inc/hal/hal_module_id.h
        )
target_link_libraries(hal_module_id PRIVATE common)
