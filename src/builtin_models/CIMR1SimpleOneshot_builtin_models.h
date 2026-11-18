#pragma once
#include <stddef.h>
#include <clips.h>

typedef struct s_RetGetOldmodelAsString {
	const unsigned char* value;
	size_t size;
} RetGetModelAsString;

RetGetModelAsString get_oldmodel_as_string();
RetGetModelAsString get_testrunnermodel_as_string();
char* testrunner_input_script_as_rif_data(const char* script);
//void testrunnermodel_set_testcrate(Environment*, const char* testcrate);
