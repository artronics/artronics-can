project(artronics_tests)
set(CMAKE_CXX_STANDARD 17)
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


add_executable(ring_buffer_tests src/test_ring_buffer.cpp)
add_executable(state_tests src/test_state.cpp fixtures/StateFixture.cpp)
add_executable(message_processor_tests src/test_message_processor.cpp fixtures/MessageProcessorFixture.cpp)

target_link_libraries(ring_buffer_tests gtest_main gmock ring_buffer)
target_link_libraries(state_tests gtest_main gmock state)
target_link_libraries(message_processor_tests gtest_main gmock message_processor)

add_test(NAME ring_buffer_tests COMMAND ring_buffer)
add_test(NAME state_tests COMMAND state)
add_test(NAME message_processor_tests COMMAND message_processor)
