set(hal_arch stm32f0xx)

add_library(hal_module_id STATIC
        ${hal_arch}/hal_module_id.c
        ../inc/hal/hal_module_id.h
        )
target_link_libraries(hal_module_id PRIVATE common)
