#pragma once

#include <ClipsInfoModelRulemachine1.h>
#include <clips.h>

typedef struct s_CIMR1ImportInformation CIMR1ImportInformation;

CIMR1ImportInformation* CIMR1ImportInformation_new();
int CIMR1ImportInformation_add_location(CIMR1ImportInformation*, const char*);
int CIMR1ImportInformation_add_string(CIMR1ImportInformation*, const char*);
int CIMR1ImportInformation_add_local_rocrate(CIMR1ImportInformation*, const char*);
int CIMR1ImportInformation_use_imports(CIMR1ImportInformation*, Environment*);
void free_CIMR1ImportInformation(CIMR1ImportInformation*);

int import_local_rocrate(Environment*, const char* basepath);

