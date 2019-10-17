include(ExternalProject)

ExternalProject_Add(SFML
	GIT_SUBMODULES		3rd/SFML
	SOURCE_DIR			${CMAKE_ROOT_DIR}/3rd/SFML
	INSTALL_COMMAND		""
)
