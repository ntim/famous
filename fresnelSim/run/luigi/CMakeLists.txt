if(NOT EXISTS ${PROJECT_BINARY_DIR}/run/luigi)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/run/luigi)
endif()

file(GLOB resources RELATIVE ${PROJECT_SOURCE_DIR}/run/luigi *.py)
foreach(resource ${resources})
    configure_file(${resource} ${PROJECT_BINARY_DIR}/run/luigi/${resource})
endforeach()
