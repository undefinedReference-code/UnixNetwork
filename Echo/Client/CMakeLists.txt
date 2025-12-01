cmake_minimum_required(VERSION 3.10)

project(Echo_Client LANGUAGES C CXX)

set(CLIENT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp)

set(PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

add_executable(client ${CLIENT_SRC}
  # include wrap sources from lib so symbols are available at link time
  ${PROJECT_LIB_DIR}/wrapsock.c
  ${PROJECT_LIB_DIR}/wrapunix.c
  ${PROJECT_LIB_DIR}/error.c
)

# Include directory so #include "unp.h" works
target_include_directories(client PRIVATE ${PROJECT_LIB_DIR})

# Set C/C++ standards if needed (client.cpp is C++)
set_target_properties(client PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# Link pthreads (wrapsock/wrapunix may require it)
find_package(Threads REQUIRED)
target_link_libraries(client PRIVATE Threads::Threads)

# If you want to place the binary in a specific folder:
set_target_properties(client PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)
