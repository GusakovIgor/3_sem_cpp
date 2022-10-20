add_library (Saver Saver.cpp)

target_link_libraries (Saver PUBLIC BmpImage Image)

target_include_directories (Saver
							INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
						   )