#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Migdal_Analysis)


#----------------------------------------------------------------------------
# Find appropriate definitions for ROOT
#
find_package(ROOT)

#----------------------------------------------------------------------------
# Set the appropriate compile options
#
add_compile_options(-Wall -Wno-shadow -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-sign-compare)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.C)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executables, and link it to the ROOT libraries

foreach ( sourceFile ${sources} )
    get_filename_component(filename ${sourceFile} NAME)
    string( REPLACE ".C" "" source ${filename} )
    add_executable(${source} ${sourceFile} ${headers})
    target_link_libraries(${source} ${ROOT_LIBRARIES})
    set_target_properties(${source}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "."
    )
endforeach ( sourceFile ${sources} )

install(TARGETS)

