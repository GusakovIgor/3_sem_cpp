add_library (Loader Loader.cpp)

target_link_libraries (Loader PUBLIC BmpImage Image)

target_include_directories (Loader
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )