add_executable(CIMR1SimpleOneshot_clips_compiler
	clips_compiler.c
)

target_link_libraries(CIMR1SimpleOneshot_clips_compiler
	CIMR1SimpleOneshot
	ACDGetCurrentTimezone #default timezone is utc0. See AdvancedCDatatypes

	CIMR1SimpleOneshot_builtin_models#just testing
	Mime2Rdf4C
	CInterfaceClipsSerializer
)
