add_library (BmpImage BmpImage.cpp)

target_include_directories (BmpImage
							INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
						   )