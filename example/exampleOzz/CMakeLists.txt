
set(PROJECT_NAME exampleOzz)

set(SRCROOT ${CMAKE_ROOT_DIR}/example/exampleOzz)

set(DIR_SRCS 
	${SRCROOT}/main.cpp
)

Add_Definitions(
	-D_HAS_ITERATOR_DEBUGGING=0
)

#add_exe(
#${PROJECT_NAME} 
#	FOLDER "example" 
#	SOURCES ${DIR_SRCS}
#)

aux_add_exe(${PROJECT_NAME} "example")
