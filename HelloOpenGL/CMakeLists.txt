add_executable(          # This command will instruct CMake to generate a executable [7]
  HelloOpenGL            # This parameter is the name of the executable
  main.cpp               # This parameter (and possibly subsequent) are the source code files
                         # participating in the compilation.
)

target_link_libraries(HelloOpenGL PRIVATE TCLAP) # [4]
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE GLFWApplication)
target_link_libraries(${PROJECT_NAME} PRIVATE GeometricTools)
