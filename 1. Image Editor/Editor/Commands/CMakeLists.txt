add_library (Commands Commands.cpp CommandFactory.cpp)

target_link_libraries (Commands PUBLIC Container)

target_include_directories (Commands
							INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
						   )