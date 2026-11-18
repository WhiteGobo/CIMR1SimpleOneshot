add_executable(CIMR1SimpleOneshot_simplerunner
	simplerunner.c
)

target_link_libraries(CIMR1SimpleOneshot_simplerunner
	CIMR1SimpleOneshot
	ACDGetCurrentTimezone #default timezone is utc0. See AdvancedCDatatypes
)
