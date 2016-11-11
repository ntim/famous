if(NOT EXISTS ${PROJECT_BINARY_DIR}/plots)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/plots)
endif()

file(GLOB plots RELATIVE ${PROJECT_SOURCE_DIR}/plots/ *.py)
foreach(plot ${plots})
    configure_file(${plot} ${PROJECT_BINARY_DIR}/plots/${plot})
endforeach()

add_subdirectory(luigi)