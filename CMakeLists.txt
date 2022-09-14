cmake_minimum_required(VERSION 3.15) # Indicate CMake, which version is required (minimum) [5]
project(HelloOpenGL)        # Sets up the name of the project [6]

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Fix the output directory for the archive artifacts [8]
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Fix the output directory for libraries [9]
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Fix the output directory for runtime files [10]

# Build the external dependencies
add_subdirectory(external/tclap)
add_subdirectory(external/GLFW)
add_subdirectory(external/glad)


add_subdirectory(HelloOpenGL)

