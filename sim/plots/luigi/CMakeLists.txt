if(NOT EXISTS ${PROJECT_BINARY_DIR}/plots/luigi)
    execute_process(COMMAND mkdir ${PROJECT_BINARY_DIR}/plots/luigi)
endif()

file(GLOB resources RELATIVE ${PROJECT_SOURCE_DIR}/plots/luigi *.py)
foreach(resource ${resources})
    configure_file(${resource} ${PROJECT_BINARY_DIR}/plots/luigi/${resource})
endforeach()
