cmake_minimum_required(VERSION 2.8.7)
project(vmir)

set(CMAKE_BINARY_DIR bin)
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)

include_directories(tlsf/)

# vmir library
set_source_files_properties(src/vmir_lib.cpp PROPERTIES language CXX)
add_library(vmir src/vmir.c src/vmir_lib.cpp tlsf/tlsf.c)
target_link_libraries(vmir m)

# legacy standalone executable
add_executable(vmir_legacy src/main.c)
target_link_libraries(vmir_legacy vmir)

# standalone executable
add_executable(vmir_exe src/main.cpp)
target_link_libraries(vmir_exe vmir)

# compiler flags
#set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Werror -Wmissing-prototypes")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED on)
set(CMAKE_C_FLAGS "-Wmissing-prototypes")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -std=gnu99 -O0 -DDEBUG -ggdb -DVM_DONT_USE_COMPUTED_GOTO")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -std=gnu99 -O3 -DNDEBUG -DVMIR_USE_TLSF")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS} -std=c++14 -O0 -DDEBUG -ggdb -DVM_DONT_USE_COMPUTED_GOTO")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS} -std=c++14 -O3 -DNDEBUG -DVMIR_USE_TLSF")