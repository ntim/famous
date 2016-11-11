cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set project name.
project(gps_test)

file(GLOB testHeaders *.hh)
add_executable(${PROJECT_NAME} test.cc ${testHeaders})
target_link_libraries(${PROJECT_NAME} gps ${LIBS} gtest gtest_main)