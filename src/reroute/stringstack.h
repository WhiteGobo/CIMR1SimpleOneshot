#pragma once

typedef struct s_StringStack StringStack;

StringStack* StringStack_new();
char* StringStack_combine(StringStack*);
void StringStack_append(StringStack*, const char*);
void free_StringStack(StringStack*);
