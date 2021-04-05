add_library(hal_can STATIC
        hal_can.c
        )
target_compile_definitions(hal_can PRIVATE
        -DSTM32F042x6
)
target_link_libraries(hal_can PRIVATE common)

add_library(hal_module_id STATIC
        hal_module_id.c
        )
target_compile_definitions(hal_module_id PRIVATE
        -DSTM32F042x6
        )
target_link_libraries(hal_module_id PRIVATE common)

add_library(hal_spi STATIC
        hal_spi.c
        )
target_compile_definitions(hal_spi PRIVATE
        -DSTM32F042x6
        )
target_link_libraries(hal_spi PRIVATE common)
