set(CMAKE_CXX_STANDARD 20)
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_executable(test_ring_buffer src/test_ring_buffer.cpp)
add_executable(test_can src/test_can.cpp fixtures/CanFixture.cpp)
add_executable(test_state src/test_state.cpp fixtures/StateFixture.cpp)
add_executable(test_message_processor src/test_message_processor.cpp fixtures/MessageProcessorFixture.cpp)
add_executable(test_hal_can src/hal/stm32f0xx/test_hal_can.cpp)
add_executable(test_hal_spi src/hal/stm32f0xx/test_hal_spi.cpp)
add_executable(test_hal_tmc5160 src/hal/tmc/test_hal_tmc5160.cpp fixtures/SpiFixture.cpp)
add_executable(test_hal_module_id src/hal/stm32f0xx/test_module_id.cpp)

set_property(TARGET
        test_hal_can
        test_hal_spi
        test_hal_tmc5160
        test_hal_module_id
        APPEND PROPERTY COMPILE_DEFINITIONS
        USE_HAL_DRIVER
        STM32F042x6
        )

set(test_targets
        test_ring_buffer
        test_can
        test_state
        test_message_processor
        test_hal_can
        test_hal_spi
        test_hal_tmc5160
        test_hal_module_id
        )

foreach(target IN LISTS test_targets)
    string(SUBSTRING ${target} 5 -1 lib_name) # remove test_ so we get the lib's name
    target_link_libraries(${target} gtest_main gmock ${lib_name})
    add_test(NAME ${target} COMMAND ${target})
endforeach()

add_custom_target(all_tests DEPENDS ${test_targets})
