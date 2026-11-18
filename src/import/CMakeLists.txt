set(name CIMR1SimpleOneshot_import)
add_library(${name}
	CIMR1SimpleOneshot_import.h
	import.c
	rocrate.c
)


target_link_libraries(${name} PUBLIC
	ClipsInfoModelRulemachine1
	clips
)

target_link_libraries(${name} PRIVATE
	ClipsInfoModelRulemachine1_literals
	cwalk
)

target_include_directories(${name} PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/"
)
