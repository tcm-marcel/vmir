cmake_minimum_required(VERSION 2.8.7)
project(vmir C)

set(CMAKE_BINARY_DIR bin)
set(EXECUTABLE_OUTPUT_PATH bin)

include_directories(tlsf/)

# vmir library
add_library(vmir src/vmir.c tlsf/tlsf.c)
target_link_libraries(vmir m)

# standalone executable
add_executable(vmir_exe src/main.c)
target_link_libraries(vmir_exe m vmir)

# compiler flags
#set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Werror -Wmissing-prototypes")
set(CMAKE_C_FLAGS "-std=gnu99 -Wmissing-prototypes")
set_property(TARGET vmir PROPERTY C_STANDARD 99)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -Og -DDEBUG -ggdb -DVM_DONT_USE_COMPUTED_GOTO")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -DNDEBUG -DVMIR_USE_TLSF")