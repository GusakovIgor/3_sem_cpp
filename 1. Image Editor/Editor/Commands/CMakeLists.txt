add_library (Commands Commands.cpp CommandFactory.cpp)

target_include_directories (Commands
							INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
						   )