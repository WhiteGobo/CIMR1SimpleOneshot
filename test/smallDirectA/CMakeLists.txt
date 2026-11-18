set(testsuffix "CIMR1SimpleOneshot::smallDirectA::")
set(basepath "${CMAKE_CURRENT_SOURCE_DIR}/")

set(testname "${testsuffix}test00")
set(script "${CMAKE_CURRENT_SOURCE_DIR}/test00.clp")
add_test(NAME ${testname}
	COMMAND CIMR1SimpleOneshot_simplerunner
	"--script" "${basepath}test00.clp"
)
set_property(TEST "${testname}" PROPERTY LABELS "CIMR1SimpleOneshot")

set(testname "${testsuffix}test01")
set(script "${CMAKE_CURRENT_SOURCE_DIR}/test01.clp")
add_test(NAME ${testname}
	COMMAND CIMR1SimpleOneshot_simplerunner
	"--script" "${basepath}test01.clp"
	"--fail-on-exit"
)
set_property(TEST "${testname}" PROPERTY LABELS "CIMR1SimpleOneshot")
