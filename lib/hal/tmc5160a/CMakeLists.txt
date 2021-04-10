add_library(hal_tmc5160 STATIC
        hal_tmc5160a.c
        TMC5160.c
        )
target_link_libraries(hal_tmc5160 PRIVATE hal_spi PRIVATE common)
include_directories(${CMAKE_SOURCE_DIR}/lib/inc/hal/tmc)
