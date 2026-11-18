add_library(CIMR1SimpleOneshot
	CIMR1SimpleOneshot.h
	qq.c
)

target_link_libraries(CIMR1SimpleOneshot PRIVATE
	ClipsInfoModelRulemachine1
	CIMR1SimpleOneshot_ExternalFunctions
	cwalk
	CIMR1SimpleOneshot_reroute
	CIMR1SimpleOneshot_import
	CIMR1SimpleOneshot_child
	GetCurrentTimezonePosixA
)

target_include_directories(CIMR1SimpleOneshot PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/"
)

add_subdirectory(external_functions)
add_subdirectory(builtin_models)
add_subdirectory(reroute)
add_subdirectory(import)
add_subdirectory(child)

include(testrunner.cmake)
include(simplerunner.cmake)
include(clips_compiler.cmake)
