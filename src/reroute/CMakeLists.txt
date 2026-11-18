set(name CIMR1SimpleOneshot_reroute)
add_library(${name}
	CIMR1SimpleOneshot_reroute.h
	reroute.c
	stringcapture.c
	stringstack.c
	stringstack.h
)


target_link_libraries(${name} PUBLIC
	ClipsInfoModelRulemachine1
	clips
)

target_include_directories(${name} PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/"
)
