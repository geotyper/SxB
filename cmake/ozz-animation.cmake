include(ExternalProject)

ExternalProject_Add(ozz-animation
	GIT_SUBMODULES		3rd/ozz-animation
	SOURCE_DIR			${CMAKE_ROOT_DIR}/3rd/ozz-animation
	INSTALL_COMMAND		""
	#INSTALL_COMMAND		"RUNTIME DESTINATION bin LIBRARY DESTINATION ${CMAKE_ROOT_DIR}/lib"
)

