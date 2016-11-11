cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set project name.
project(hex_test)

file(GLOB testHeaders *.hh)
add_executable(${PROJECT_NAME} test.cc ${testHeaders})
target_link_libraries(${PROJECT_NAME} hex ${LIBS} gtest gtest_main)