add_library(CIMR1SimpleOneshot_child
	CIMR1SimpleOneshot_child.h
	child.c
)

target_link_libraries(CIMR1SimpleOneshot_child PUBLIC
	clips
	ClipsInfoModelRulemachine1
	ClipsInfoModelRulemachine1_literals
)
target_link_libraries(CIMR1SimpleOneshot_child PRIVATE
	RIFRDFQuery
	CIMR1SimpleOneshot_ExternalFunctions
	ClipsInfoModelRulemachine1_data_importer
	hashtable
	Mime2Rdf4C
)

target_include_directories(CIMR1SimpleOneshot_child PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/"
)
