add_subdirectory (Image)
add_subdirectory (Container)
add_subdirectory (Commands)
add_subdirectory (InputManager)
add_subdirectory (ContainerManager)

add_library (Editor Editor.cpp)

target_link_libraries (Editor PUBLIC Image Container Commands InputManager ContainerManager)

target_include_directories (Editor
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )