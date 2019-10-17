
set(PROJECT_NAME exampleOzz)

Add_Definitions(
	-D_HAS_ITERATOR_DEBUGGING=0
)

aux_add_exe(${PROJECT_NAME} "example")
