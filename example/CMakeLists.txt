
set(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++1y)

set(INC_DIR 
	${CMAKE_ROOT_DIR}/3rd/bgfx/include
	${CMAKE_ROOT_DIR}/3rd/bx/include
	${CMAKE_ROOT_DIR}/3rd/bimg/include
	${CMAKE_ROOT_DIR}/3rd
	${CMAKE_ROOT_DIR}/include
	${CMAKE_THIRDLIB_ROOTDIR}/include
)


IF (OS_IS_WINDOWS)
	set(INC_DIR ${INC_DIR} ${CMAKE_ROOT_DIR}/3rd/bx/include/compat/msvc)
ELSEIF (OS_IS_MACOS)
	set(INC_DIR ${INC_DIR} ${CMAKE_ROOT_DIR}/3rd/bx/include/compat/osx)
ENDIF()

include_directories(${INC_DIR})

set(LINK_LIBS 
	bgfx
	bx
	bimg
	sxbCommon
	#ozz_base
	#ozz_options
	#ozz_animation
)
link_libraries(${LINK_LIBS})

if(OS_IS_IOS)
	set(EXAMPLE_LINK_LIBS ${EXAMPLE_LINK_LIBS}
		sfml-main
	)
endif()

set(THIRD_LIBS
	${CMAKE_THIRDLIB_ROOTDIR}/lib/libsfml-system-d.dylib
	${CMAKE_THIRDLIB_ROOTDIR}/lib/libsfml-window-d.dylib
	${CMAKE_THIRDLIB_ROOTDIR}/lib/libozz_base_d.a
	${CMAKE_THIRDLIB_ROOTDIR}/lib/libozz_options_d.a
	${CMAKE_THIRDLIB_ROOTDIR}/lib/libozz_animation_d.a
)

link_libraries(${THIRD_LIBS})

if(OS_IS_WINDOWS)
    link_libraries(winmm gdi32)
elseif(OS_IS_MACOS)
    link_libraries("-framework Foundation" "-framework AppKit" "-framework IOKit" "-framework Carbon" "-framework Metal" "-framework QuartzCore")
elseif(OS_IS_IOS)
    link_libraries("-framework Foundation" "-framework UIKit" "-framework CoreGraphics" "-framework QuartzCore" "-framework CoreMotion" "-framework Metal" "-framework OpenGLES" "-framework OpenAL")
elseif(OS_IS_ANDROID)
    link_libraries(  android)
endif()

	
add_subdirectory("exampleOzz")
#add_subdirectory("example_00-HelloBGFX")
add_subdirectory("example_00-HelloWorld")
#add_subdirectory("example_11-bob")
#add_subdirectory("example_11-coc")
#add_subdirectory("example_12-lod")
#add_subdirectory("example_13-stencil")
add_subdirectory("example_18-ibl")
#add_subdirectory("example_33-pom")
