#include "CIMR1SimpleOneshot_reroute.h"
#include "stringstack.h"

typedef struct s_StringCapture {
	char* symbol;
	StringStack* stack;
} StringCapture;


StringCapture* StringCapture_new(const char* symbol){
	StringCapture* ret = malloc(sizeof(StringCapture));
	if(ret == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return NULL;
	}
	ret->symbol = malloc(strlen(symbol) + 1);
	if(ret->symbol == NULL){
		free(ret);
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return NULL;
	}
	strcpy(ret->symbol, symbol);
	ret->stack = StringStack_new();
	return ret;
}

bool StringCapture_check_symbol(StringCapture* x, const char* symbol){
	return strcmp(x->symbol, symbol) == 0;
}


char* StringCapture_extract(StringCapture* x){
	char* ret = StringStack_combine(x->stack);
	free_StringStack(x->stack);
	x->stack = StringStack_new();
	return ret;
}


void free_StringCapture(StringCapture* x){
	if(x == NULL) return;
	free(x->symbol);
	free_StringStack(x->stack);
	free(x);
}

void StringCapture_append(StringCapture* x, const char* input){
	StringStack_append(x->stack, input);
}


static bool myquery(Environment*, const char*, void* cntxt);
static void mywrite(Environment*, const char*, const char *input, void *cntxt);
static void myexit(Environment*, int exitCode, void *cntxt);
void StringCapture_config(StringCapture* x, Environment* env){
	AddRouter(env, x->symbol, 20,
			myquery, mywrite, NULL, NULL, myexit,
			x);
}

static bool myquery(Environment*, const char* logicalName, void* cntxt){
	return StringCapture_check_symbol((StringCapture*) cntxt, logicalName);
}

static void mywrite(Environment*, const char *logicalName, const char *input, void *cntxt)
{
	StringCapture_append((StringCapture*) cntxt, input);
}

static void myexit(Environment*, int exitCode, void *cntxt){}
