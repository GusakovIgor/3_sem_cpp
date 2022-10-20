add_library (Image Image.cpp)

target_include_directories (Image
							INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
						   )