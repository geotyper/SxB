
set(EXAMPLE_INC_DIR 
	${CMAKE_ROOT_DIR}/3rd/SFML/include
	${CMAKE_ROOT_DIR}/3rd/bgfx/include
	${CMAKE_ROOT_DIR}/3rd/bx/include
	${CMAKE_ROOT_DIR}/3rd/bimg/include
	${CMAKE_ROOT_DIR}/3rd
	${CMAKE_ROOT_DIR}/include
)

set(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++1y)

IF (OS_IS_WINDOWS)
	set(EXAMPLE_INC_DIR ${EXAMPLE_INC_DIR} ${CMAKE_ROOT_DIR}/include/bx/compat/msvc)
ELSEIF (OS_IS_MACOS)
	set(EXAMPLE_INC_DIR ${EXAMPLE_INC_DIR} ${CMAKE_ROOT_DIR}/include/bx/compat/osx)
ENDIF()

set(EXAMPLE_LINK_LIBS 
	bgfx
	bx
	bimg
	sfml-system-d
	sfml-window-d
	sxbCommon
)
if(OS_IS_IOS)
	set(EXAMPLE_LINK_LIBS ${EXAMPLE_LINK_LIBS}
		sfml-main
	)
endif()

if(OS_IS_MACOS)
	link_directories(${CMAKE_ROOT_DIR}/3rd/SFML/Solution/macOS/lib)
endif()

include_directories(${EXAMPLE_INC_DIR})
link_libraries(${EXAMPLE_LINK_LIBS})

if(OS_IS_WINDOWS)
    link_libraries(winmm gdi32)
elseif(OS_IS_MACOS)
    link_libraries("-framework Foundation" "-framework AppKit" "-framework IOKit" "-framework Carbon" "-framework Metal" "-framework QuartzCore")
elseif(OS_IS_IOS)
    link_libraries("-framework Foundation" "-framework UIKit" "-framework CoreGraphics" "-framework QuartzCore" "-framework CoreMotion" "-framework Metal" "-framework OpenGLES" "-framework OpenAL")
elseif(OS_IS_ANDROID)
    link_libraries(  android)
endif()

	
#add_subdirectory("example_00-HelloBGFX")
add_subdirectory("example_00-HelloWorld")
#add_subdirectory("example_11-bob")
#add_subdirectory("example_11-coc")
#add_subdirectory("example_12-lod")
#add_subdirectory("example_13-stencil")
#add_subdirectory("example_33-pom")
