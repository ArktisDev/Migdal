#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Migdal_Acquisition)


##----------------------------------------------------------------------------
## Find appropriate definitions for ROOT
##
#find_package(ROOT)

#----------------------------------------------------------------------------
# Set the appropriate compile options
#
add_compile_options(-Wall -Wno-shadow -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp)

#----------------------------------------------------------------------------
# Add the executables, and link it to the ROOT libraries

foreach ( sourceFile ${sources} )
    get_filename_component(filename ${sourceFile} NAME)
    string( REPLACE ".cpp" "" source ${filename} )
    add_executable(${source} ${sourceFile} ${headers})
    target_link_libraries(${source} -pthread)
    set_target_properties(${source}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "."
        CXX_STANDARD 17
    )
endforeach ( sourceFile ${sources} )

install(TARGETS)

