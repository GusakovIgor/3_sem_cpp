add_subdirectory (BmpImage)
add_subdirectory (Loader)
add_subdirectory (Saver)

add_library (ContainerManager ContainerManager.cpp)

target_link_libraries (ContainerManager PUBLIC Container Commands BmpImage Loader Saver)

target_include_directories (ContainerManager
                            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )