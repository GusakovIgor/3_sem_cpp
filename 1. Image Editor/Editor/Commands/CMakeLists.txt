add_library (Commands Commands.cpp CommandFactory.cpp)

target_link_libraries (Commands PUBLIC BmpImage Image)

target_include_directories (Commands
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )