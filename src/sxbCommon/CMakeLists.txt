
set(PROJECT_NAME sxbCommon)

set(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++1y -fno-objc-arc)

set(INCROOT ${CMAKE_ROOT_DIR}/include/sxbCommon)
set(SRCROOT ${CMAKE_ROOT_DIR}/src/sxbCommon)

set(DIR_SRCS 
			${INCROOT}/Config.h
			${INCROOT}/defines.h
			${INCROOT}/utils.h
			${SRCROOT}/utils.cpp
			${INCROOT}/Mesh.h
			${SRCROOT}/Mesh.cpp
			${INCROOT}/Camera.h
			${SRCROOT}/Camera.cpp
			${INCROOT}/DataStruct.h
			${INCROOT}/Cursor.h
			${SRCROOT}/Cursor.cpp
			${INCROOT}/Header.h
			${INCROOT}/IEntry.h
			${SRCROOT}/IEntry.cpp
			${INCROOT}/imgui.h
			${SRCROOT}/imgui.cpp
			${INCROOT}/PlaybackController.h
			${SRCROOT}/PlaybackController.cpp

)

if(OS_IS_IOS)
	set(DIR_SRCS ${DIR_SRCS}
		${SRCROOT}/utils_p.mm
	)
else()
	set(DIR_SRCS ${DIR_SRCS}
		${SRCROOT}/utils_p.cpp
	)
endif()
		
set(MESHOPT_DIR ${CMAKE_ROOT_DIR}/3rd/bgfx/3rdparty/meshoptimizer/src)
set(MESHOPT_SRCS 
	${MESHOPT_DIR}/allocator.cpp
	${MESHOPT_DIR}/clusterizer.cpp
	${MESHOPT_DIR}/indexcodec.cpp
	${MESHOPT_DIR}/indexgenerator.cpp
	${MESHOPT_DIR}/meshoptimizer.h
	${MESHOPT_DIR}/overdrawanalyzer.cpp
	${MESHOPT_DIR}/overdrawoptimizer.cpp
	${MESHOPT_DIR}/simplifier.cpp
	${MESHOPT_DIR}/spatialorder.cpp
	${MESHOPT_DIR}/stripifier.cpp
	${MESHOPT_DIR}/vcacheanalyzer.cpp
	${MESHOPT_DIR}/vcacheoptimizer.cpp
	${MESHOPT_DIR}/vertexcodec.cpp
	${MESHOPT_DIR}/vfetchanalyzer.cpp
	${MESHOPT_DIR}/vfetchoptimizer.cpp
)
			
source_group("meshoptimizer" FILES ${MESHOPT_SRCS})	

set(EMBEDDED_SHADER_DIR ${SRCROOT})
set(EMBEDDED_SHADER
	${EMBEDDED_SHADER_DIR}/fs_imgui_image.bin.h
	${EMBEDDED_SHADER_DIR}/fs_ocornut_imgui.bin.h
	${EMBEDDED_SHADER_DIR}/vs_imgui_image.bin.h
	${EMBEDDED_SHADER_DIR}/vs_ocornut_imgui.bin.h
)
source_group("embedded_shader" FILES ${EMBEDDED_SHADER})

		
IF (OS_IS_WINDOWS)
	set(INC_DIR ${CMAKE_ROOT_DIR}/3rd/bx/include/bx/compat/msvc)
	Add_Definitions(-D_HAS_ITERATOR_DEBUGGING=0)
ELSEIF (OS_IS_MACOS)
	set(INC_DIR ${CMAKE_ROOT_DIR}/3rd/bx/include/bx/compat/osx)
ENDIF()

set(INC_DIR ${INC_DIR}
	${CMAKE_ROOT_DIR}/3rd/bx/include
	${CMAKE_ROOT_DIR}/3rd/bgfx/include
	${CMAKE_ROOT_DIR}/3rd/bimg/include
	${CMAKE_ROOT_DIR}/3rd/SFML/include
)
			
include_directories(${INC_DIR})

add_lib(
	${PROJECT_NAME} 
	FOLDER "libs" 
	SOURCES ${DIR_SRCS} ${MESHOPT_SRCS} ${EMBEDDED_SHADER}
)

target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_ROOT_DIR}/include )
