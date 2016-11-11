cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(famous)

# Boost
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost 1.50.0 COMPONENTS date_time program_options filesystem system regex REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})
set(CMAKE_CXX_FLAGS "-DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS ${CMAKE_CXX_FLAGS}")

# Find Geant4 package, activating all available UI and Vis drivers by default
option(WITH_GEANT4_UIVIS "Build with all Geant4 UIs and all visualization drivers" ON)
option(WITH_GEANT4_OPENGL "Build with Geant4 shell UI and OpenGl visualization" OFF)
option(WITH_GEANT4_QT "Build with Geant4 Qt UI" OFF)

if(WITH_GEANT4_OPENGL)
	find_package(Geant4 REQUIRED vis_opengl_x11 ui_tcsh)
elseif(WITH_GEANT4_QT)
	find_package(Geant4 REQUIRED qt)
elseif(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()

add_definitions(${Geant4_DEFINITIONS})
include(${Geant4_USE_FILE})
set(LIBS ${LIBS} ${Geant4_LIBRARIES})

# Switch for static / shared library building.
if(Geant4_shared_FOUND)
	set(LIBRARY_MODE "SHARED")
else()
	set(LIBRARY_MODE "STATIC")
endif()

# Add G4Sipm
set(CMAKE_CXX_FLAGS "-DWITH_ROOT ${CMAKE_CXX_FLAGS}")
add_subdirectory(externals/g4sipm)
include_directories(externals/g4sipm/g4sipm/include)
include_directories(externals/g4sipm/sample/include)
include_directories(externals/g4sipm/externals/jansson/src)
include_directories(externals/g4sipm/externals/gtest/include)
set(LIBS ${LIBS} g4sipm g4sipm_sample jansson)

# Add Root
execute_process(COMMAND root-config --libs OUTPUT_VARIABLE root_libs OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE root_cflags OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --incdir OUTPUT_VARIABLE root_includes OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions(${root_cflags})
include_directories(${root_includes})
set(LIBS ${LIBS} ${root_libs})

# Add submodules
add_subdirectory(gps)
include_directories(gps/include)
add_subdirectory(core)
include_directories(core/include)
add_subdirectory(fresnel)
include_directories(fresnel/include)
add_subdirectory(fresnelSim)
include_directories(fresnelSim/include)
add_subdirectory(wico)
include_directories(wico/include)
add_subdirectory(wicoSim)
include_directories(wicoSim/include)
add_subdirectory(fastSim)
include_directories(fastSim/include)
add_subdirectory(newtonSim)
include_directories(newtonSim/include)
add_subdirectory(hex)
include_directories(hex/include)
add_subdirectory(offline)
include_directories(offline/include)
add_subdirectory(sim)

# Copy macros.
file(GLOB macros *.mac)
foreach(macro ${macros})
    get_filename_component(macro-name ${macro} NAME)
    configure_file(${macro} ${PROJECT_BINARY_DIR}/${macro-name} COPYONLY)
endforeach()
