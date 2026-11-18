include(CTest)
add_subdirectory(smallDirectA)
add_subdirectory(testrunner)

set(testsuffix "CIMR1SimpleOneshot::")

set(testname "${testsuffix}clips_compiler::simple_generate")
add_test(NAME ${testname}
	COMMAND CIMR1SimpleOneshot_clips_compiler
	"--riffile" "${CMAKE_CURRENT_SOURCE_DIR}/data00.rifps"
)

set_property(TEST "${testname}" PROPERTY LABELS "CIMR1SimpleOneshot")
