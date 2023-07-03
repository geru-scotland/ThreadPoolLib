cmake_minimum_required(VERSION 3.25)
project(ThreadPoolLib)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

set(SOURCES
        src/ThreadPool.cpp)

set(APP
        app/main.cpp)

# Add static and dynamic libraries
add_library(thread_pool_lib_static STATIC ${SOURCES})
add_library(thread_pool_lib_shared SHARED ${SOURCES})

add_executable(app ${APP})
target_link_libraries(app thread_pool_lib_shared)
