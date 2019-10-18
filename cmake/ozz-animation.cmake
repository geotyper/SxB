include(ExternalProject)

ExternalProject_Add(ozz-animation
	#EP_PREFIX			${CMAKE_ROOT_DIR}/bin-ozz
	#GIT_SUBMODULES		3rd/ozz-animation
	#PREFIX				${CMAKE_ROOT_DIR}/Solution
	SOURCE_DIR			${CMAKE_ROOT_DIR}/3rd/ozz-animation
	INSTALL_COMMAND 	cmake . -DCMAKE_INSTALL_PREFIX=${CMAKE_ROOT_DIR}/3rdlib && cmake --build . --config Debug --target install
	#INSTALL_COMMAND 	"make DESTDIR='${CMAKE_ROOT_DIR}/bin-ozz' install"
	#INSTALL_COMMAND 	"set(CMAKE_INSTALL_PREFIX ${CMAKE_ROOT_DIR}/bin-ozz)"
	#INSTALL_COMMAND		"install TARGETS ozz-animation CONFIGURATIONS Debug RUNTIME DESTINATION  ${CMAKE_ROOT_DIR}/bin-ozz"
	#INSTALL_COMMAND		"RUNTIME DESTINATION bin LIBRARY DESTINATION ${CMAKE_ROOT_DIR}/lib"
)

