#include "CIMR1SimpleOneshot_reroute.h"


typedef struct s_StringCaptureList {
	StringCapture* first;
	struct s_StringCaptureList* rest;
} StringCaptureList;

typedef struct s_CLIPSRerouteConfig {
	StringCaptureList* string_captures;
} CLIPSRerouteConfig;

static StringCaptureList* StringCaptureList_insert(StringCaptureList*, StringCapture*);
static void free_StringCaptureList(StringCaptureList*);

CLIPSRerouteConfig* CLIPSRerouteConfig_new(){
	CLIPSRerouteConfig* ret = malloc(sizeof(CLIPSRerouteConfig));
	if(ret == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return NULL;
	}
	ret->string_captures = NULL;
	return ret;
}

void free_CLIPSRerouteConfig(CLIPSRerouteConfig* x){
	if(x == NULL) return;
	free(x);
}


void CLIPSRerouteConfig_config(CLIPSRerouteConfig* x, Environment* env){
	if(x == NULL || env == NULL) return;
	StringCaptureList* sc_list = x->string_captures;
	while(sc_list != NULL){
		StringCaptureList* ret_sc_list = sc_list->rest;
		StringCapture_config(sc_list->first, env);
		sc_list = ret_sc_list;
	}
}


void CLIPSRerouteConfig_add_stringcapture(CLIPSRerouteConfig* x, const char* symbol)
{
	if(x==NULL || symbol == NULL) return;
	StringCapture* new = StringCapture_new(symbol);
	if(new == NULL) return;
	x->string_captures = StringCaptureList_insert(x->string_captures, new);
}


char* CLIPSRerouteConfig_extract_from_stringcapture(CLIPSRerouteConfig* cfg, const char* symbol)
{
	for(StringCaptureList* x = cfg->string_captures; x!=NULL; x=x->rest){
		if(StringCapture_check_symbol(x->first, symbol)){
			return StringCapture_extract(x->first);
		}
	}
	return NULL;
}


static StringCaptureList* StringCaptureList_insert(StringCaptureList* old, StringCapture* new)
{
	StringCaptureList* ret = malloc(sizeof(StringCaptureList));
	if(ret == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return old;
	}
	ret->first = new;
	ret->rest = old;
	return ret;
}

static void free_StringCaptureList(StringCaptureList* x){
	StringCaptureList* tmp;
	while(x!=NULL){
		tmp = x->rest;
		free_StringCapture(x->first);
		free(x);
		x = tmp;
	}
}














static bool myquery(Environment*, const char* logicalName, void* cntxt);
static void mywrite(Environment*, const char *logicalName, const char *str, void *cntxt);
static void myexit(Environment*, int exitCode, void *cntxt);


typedef struct s_Reroute {
	char* symbol;
	FILE* target;
} Reroute;

typedef struct s_RerouteList {
	Reroute first;
	struct s_RerouteList* rest;
} RerouteList;

RerouteList* RerouteList_add(RerouteList* x, const char* symbol, FILE* fp){
	RerouteList* tmp;
	RerouteList* new = malloc(sizeof(RerouteList));
	if(new == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return x;
	}
	new->first.symbol = malloc(strlen(symbol) + 1);
	strcpy(new->first.symbol, symbol);
	new->first.target = fp;
	new->rest = NULL;
	if(x == NULL){
		return new;
	} else {
		tmp = x;
		while(tmp->rest != NULL){
			tmp = tmp->rest;
		}
		tmp->rest = new;
		return x;
	}
}

void free_RerouteList(RerouteList* x){
	RerouteList* y;
	while(x != NULL){
		y = x->rest;
		free(x->first.symbol);
		free(x);
		x = y;
	}
}

void Environment_add_reroutes(Environment* env, RerouteList* start){
	for(RerouteList *x = start; x!=NULL; x = x->rest){
		AddRouter(env, x->first.symbol, 20,
				myquery, mywrite, NULL, NULL, myexit,
				x->first.target);
	}
}


static bool myquery(Environment*, const char* logicalName, void* cntxt)
{
	if (strcmp(logicalName, STDERR) == 0){
	       return true;
	} else if (strcmp(logicalName, STDOUT) == 0){
	       return true;
	}
	return false;
}

static void mywrite(Environment*, const char *logicalName, const char *str, void *cntxt)
{
	FILE *fp = (FILE *) cntxt;
	fprintf(fp, "%s", str);
}

static void myexit(Environment*, int exitCode, void *cntxt){
}

