cmake_minimum_required(VERSION 3.10)

project(Echo_Server LANGUAGES C CXX)

set(CLIENT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp)

set(PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

add_executable(server ${CLIENT_SRC}
  # include wrap sources from lib so symbols are available at link time
  ${PROJECT_LIB_DIR}/wrapsock.c
  ${PROJECT_LIB_DIR}/wrapunix.c
  ${PROJECT_LIB_DIR}/error.c
)

# Include directory so #include "unp.h" works
target_include_directories(server PRIVATE ${PROJECT_LIB_DIR})

# Set C/C++ standards if needed (server.cpp is C++)
set_target_properties(server PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# Link pthreads (wrapsock/wrapunix may require it)
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)

# If you want to place the binary in a specific folder:
set_target_properties(server PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)
