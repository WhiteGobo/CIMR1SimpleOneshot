set(testsuffix "CIMR1SimpleOneshot::testrunner::")

set(testname "${testsuffix}test00")
add_test(NAME ${testname}
	COMMAND CIMR1SimpleOneshot_testrunner
	"--script" "${CMAKE_CURRENT_SOURCE_DIR}/premise-00.clp"
	"--testcrate" "${CMAKE_CURRENT_SOURCE_DIR}/data00/"
)

set_property(TEST "${testname}" PROPERTY LABELS "CIMR1SimpleOneshot")
