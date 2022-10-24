add_library (InputManager InputManager.cpp)

target_link_libraries (InputManager PUBLIC Commands)

target_include_directories (InputManager
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )