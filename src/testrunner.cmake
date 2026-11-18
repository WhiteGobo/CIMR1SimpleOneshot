add_executable(CIMR1SimpleOneshot_testrunner
	testrunner.c
)

target_link_libraries(CIMR1SimpleOneshot_testrunner
	CIMR1SimpleOneshot
	ACDGetCurrentTimezone #default timezone is utc0. See AdvancedCDatatypes

	CIMR1SimpleOneshot_builtin_models#just testing
)
