cmake_minimum_required(VERSION 3.25)
project(ThreadPoolLib)

set(CMAKE_CXX_STANDARD 23)

# Enable DEBUG directives in code.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")

include_directories(include
                    examples/support)

set(SOURCES
        src/ThreadPool.cpp)

set(TEST
        examples/examples.cpp examples/support/Foo.cpp examples/support/Foo.h)

# Add static and dynamic libraries
add_library(thread_pool_lib_static STATIC ${SOURCES})
add_library(thread_pool_lib_shared SHARED ${SOURCES})

add_executable(test ${TEST})
target_link_libraries(test thread_pool_lib_shared)
