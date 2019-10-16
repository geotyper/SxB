
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
)

if(${THE_PROJECT_PREFIX}_OS_IOS)
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
)
			
include_directories(${INC_DIR})

add_lib(
	${PROJECT_NAME} 
	FOLDER "libs" 
	SOURCES ${DIR_SRCS} ${MESHOPT_SRCS}
)
