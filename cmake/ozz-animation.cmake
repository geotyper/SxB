include(ExternalProject)

ExternalProject_Add(ozz-animation
	#GIT_SUBMODULES		3rd/ozz-animation
	#PREFIX				${CMAKE_ROOT_DIR}/Solution
	SOURCE_DIR			${CMAKE_ROOT_DIR}/3rd/ozz-animation
	INSTALL_COMMAND 	cmake . -DCMAKE_INSTALL_PREFIX=${CMAKE_ROOT_DIR}/3rdlib && cmake --build . --config Debug --target install
)

