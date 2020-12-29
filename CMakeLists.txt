cmake_minimum_required(VERSION 3.17)
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)

project(artronics)

enable_language(CXX C ASM)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

add_subdirectory(drivers)

SET(SOURCES
        ${HAL_SOURCES}
        startup_stm32f042x6.s
        src/main.c
        src/main.c
        src/can.c
        src/error_handler.c
        src/stm32f0xx_hal_msp.c
        src/stm32f0xx_it.c
        src/system_stm32f0xx.c
        )

set(EXECUTABLE ${PROJECT_NAME}.out)
add_executable(${EXECUTABLE} ${SOURCES})


target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32F042x6
        )

target_include_directories(${EXECUTABLE} PRIVATE
        inc
        drivers/CMSIS/Include
        drivers/CMSIS/Device/ST/STM32F0xx/Include
        drivers/STM32F0xx_HAL_Driver/Inc
        )

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m0
        -mthumb

        -fdata-sections
        -ffunction-sections

        -Wall

        $<$<CONFIG:Debug>:-Og>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32F042K6Tx_FLASH.ld
        -mcpu=cortex-m0
        -mthumb
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        )

add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE})

add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
