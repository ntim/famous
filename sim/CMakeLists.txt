cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Set project name.
project(sim)

# Set include paths.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Require some extra libraries.
set(LIBS ${LIBS} g4sipm_sample core hex fresnel wico offline)

# Locate sources and headers.
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add library
set(LIB_NAME famous_sim)
add_library(${LIB_NAME} ${LIBRARY_MODE} ${sources})
target_link_libraries(${LIB_NAME} ${LIBS})

# Create executable.
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cc ${headers})
target_link_libraries(${PROJECT_NAME} ${LIBS} ${LIB_NAME})

# Create results directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/results)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/results)
endif()

# Copy macros.
file(GLOB macros *.mac)
foreach(macro ${macros})
    get_filename_component(macro-name ${macro} NAME)
    configure_file(${macro} ${PROJECT_BINARY_DIR}/${macro-name} COPYONLY)
endforeach()

add_subdirectory(run)
add_subdirectory(plots)
