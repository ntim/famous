cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set project name.
project(offline)

# Set include paths.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate sources and headers.
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Create library.
add_library(${PROJECT_NAME} ${LIBRARY_MODE} ${sources})

# Add link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
