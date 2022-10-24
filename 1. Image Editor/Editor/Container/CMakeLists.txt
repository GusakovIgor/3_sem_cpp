add_library (Container Container.cpp)

target_link_libraries (Container PUBLIC BmpImage Image Commands)

target_include_directories (Container
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )