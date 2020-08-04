cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME utils_lib)
project(${PROJECT_NAME})

find_package(OpenCV REQUIRED)

# Set up the compiler flags
set(CMAKE_CXX_FLAGS "-g -O3")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPILE_OPTS "")
set(LINK_OPTS "")

# Source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Compile source files into a library
add_library(utils_lib ${SRC_FILES})

target_include_directories(utils_lib PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(utils_lib PRIVATE ${OpenCV_INCLUDE_DIRS})
target_compile_options(utils_lib PRIVATE ${COMPILE_OPTS})
target_link_options(utils_lib PRIVATE ${LINK_OPTS})
target_link_libraries(utils_lib PRIVATE ${OpenCV_LIBS})

# Tests
add_subdirectory(test)
