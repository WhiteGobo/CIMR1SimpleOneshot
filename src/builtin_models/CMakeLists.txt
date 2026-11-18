set(OLDMODEL_RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/oldmodel.clp")
set(OLDMODEL_RESOURCE_OBJ "${CMAKE_CURRENT_BINARY_DIR}/oldmodel.o")
set(OLDMODEL_PREFIX "_binary_oldmodel_clp")
set(TESTRUNNERMODEL_RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/testrunnermodel.clp")
set(TESTRUNNERMODEL_RESOURCE_OBJ "${CMAKE_CURRENT_BINARY_DIR}/testrunnermodel.o")
set(TESTRUNNERMODEL_PREFIX "_binary_testrunnermodel_clp")

add_library(CIMR1SimpleOneshot_builtin_models
	testrunnermodel.c
	bm.c
	CIMR1SimpleOneshot_builtin_models.h
	${OLDMODEL_RESOURCE_OBJ}
	${TESTRUNNERMODEL_RESOURCE_OBJ}
)
target_link_libraries(CIMR1SimpleOneshot_builtin_models PUBLIC
	clips
)
target_link_libraries(CIMR1SimpleOneshot_builtin_models PRIVATE
	RIFImporter
	Mime2Rdf4C
)
target_include_directories(CIMR1SimpleOneshot_builtin_models PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/"
)


# Run objcopy to generate the object file
#See `https://linuxvox.com/blog/c-c-with-gcc-statically-add-resource-files-to-executable-library/#prerequisites`_
add_custom_command(
	OUTPUT ${OLDMODEL_RESOURCE_OBJ}
	COMMAND objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 --rename-section .data=.rodata,alloc,load,readonly,data,contents ${OLDMODEL_RESOURCE_FILE} ${OLDMODEL_RESOURCE_OBJ} --binary-symbol-prefix ${OLDMODEL_PREFIX}
	DEPENDS ${OLDMODEL_RESOURCE_FILE}  # Regenerate if resource changes
	COMMENT "Embedding resource: ${OLDMODEL_RESOURCE_FILE}"
)

add_custom_command(
	OUTPUT ${TESTRUNNERMODEL_RESOURCE_OBJ}
	COMMAND objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 --rename-section .data=.rodata,alloc,load,readonly,data,contents ${TESTRUNNERMODEL_RESOURCE_FILE} ${TESTRUNNERMODEL_RESOURCE_OBJ} --binary-symbol-prefix ${TESTRUNNERMODEL_PREFIX}
	DEPENDS ${TESTRUNNERMODEL_RESOURCE_FILE}  # Regenerate if resource changes
	COMMENT "Embedding resource: ${TESTRUNNERMODEL_RESOURCE_FILE}"
)
