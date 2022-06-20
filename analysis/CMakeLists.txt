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
add_compile_options(-Wall -Wno-shadow -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.C)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the ROOT libraries
#
add_executable(analysis test.C ${sources} ${headers})
target_link_libraries(analysis ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Set output directory
#
set_target_properties(analysis
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "."
)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS analysis)

