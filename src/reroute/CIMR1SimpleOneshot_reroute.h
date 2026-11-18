#pragma once

#include <clips.h>

typedef struct s_CLIPSRerouteConfig CLIPSRerouteConfig;
typedef struct s_StringCapture StringCapture;

CLIPSRerouteConfig* CLIPSRerouteConfig_new();
void free_CLIPSRerouteConfig(CLIPSRerouteConfig*);
void CLIPSRerouteConfig_config(CLIPSRerouteConfig*, Environment*);
void CLIPSRerouteConfig_add_stream(CLIPSRerouteConfig*, const char* symbol, FILE*);
void CLIPSRerouteConfig_add_stringcapture(CLIPSRerouteConfig*, const char* symbol);
char* CLIPSRerouteConfig_extract_from_stringcapture(CLIPSRerouteConfig*, const char* symbol);


StringCapture* StringCapture_new(const char* symbol);
void StringCapture_config(StringCapture*, Environment*);

bool StringCapture_check_symbol(StringCapture*, const char* symbol);
char* StringCapture_extract(StringCapture*);
void free_StringCapture(StringCapture*);
void StringCapture_append(StringCapture*, const char*);




typedef struct s_RerouteList RerouteList;
/*

RerouteList* RerouteList_add(RerouteList*, const char* symbol, FILE*);
void free_RerouteList(RerouteList*);
void Environment_add_reroutes(Environment*, RerouteList*);
*/
