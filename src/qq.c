#include <ClipsInfoModelRulemachine1.h>
#include "CIMR1SimpleOneshot.h"
#include "CIMR1SimpleOneshot_ExternalFunctions.h"
#include "CIMR1SimpleOneshot_child.h"
#include <cwalk.h>
#include "CIMR1SimpleOneshot_reroute.h"
#include "CIMR1SimpleOneshot_import.h"

typedef enum {
	QQFILE,
	QQSTRING,
} QQType;

typedef struct s_QQ {
	QQType type;
	union {
		char* scriptpath;
		char* scriptstring;
	};

	CLIPSRerouteConfig* reroute_config;
	size_t scriptlen;
	CIMR1ImportInformation* importer;
	Environment *last_environment;
	bool watch_activations;
} QQ;

static bool check_no_error(Environment*);

void CIMR1Runner_add_import(QQ* runner, const char* riffile){
	int _err;
	if (riffile == NULL || runner == NULL) return;
	_err = CIMR1ImportInformation_add_location(runner->importer, riffile);
}

void CIMR1Runner_add_string_import(QQ* runner, const char* rifdata){
	int _err;
	if (rifdata == NULL || runner == NULL) return;
	_err = CIMR1ImportInformation_add_string(runner->importer, rifdata);
}

void CIMR1Runner_add_import_local_rocrate(QQ* runner, const char* testcrate){
	int _err;
	if (testcrate == NULL || runner == NULL) return;
	_err = CIMR1ImportInformation_add_local_rocrate(runner->importer, testcrate);
}

QQ* CIMR1Runner_from_string(const char* scriptstring, size_t scriptlen){
	if(scriptstring == NULL && scriptlen != 0) return NULL;
	QQ* ret = malloc(sizeof(QQ));
	if(ret == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return NULL;
	}
	ret->type = QQSTRING;
	ret->scriptstring = malloc(scriptlen);
	memcpy(ret->scriptstring, scriptstring, scriptlen);
	ret->scriptlen = scriptlen;
	ret->importer = CIMR1ImportInformation_new();
	ret->reroute_config = CLIPSRerouteConfig_new();
	ret->watch_activations = false;
	return ret;
}

QQ* CIMR1Runner_from_file(const char* scriptpath){
	if(scriptpath == NULL) return NULL;
	QQ* ret = malloc(sizeof(QQ));
	if(ret == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return NULL;
	}
	ret->type = QQFILE;
	ret->scriptpath = malloc(strlen(scriptpath) + 1);
	strcpy(ret->scriptpath, scriptpath);
	ret->importer = CIMR1ImportInformation_new();
	ret->reroute_config = CLIPSRerouteConfig_new();
	ret->watch_activations = false;
	return ret;
}

void QQ_watch_activations(QQ* x){
	if (x == NULL) return;
	x->watch_activations = true;
}
void QQ_unwatch_activations(QQ* x){
	if (x == NULL) return;
	x->watch_activations = false;
}


static int load_script_from_file(Environment* env, QQ* x){
	if(x==NULL || x->type != QQFILE){
		return 1;
	}
	switch (Load(env, x->scriptpath)){
		case LE_NO_ERROR:
			return 0;
		case LE_OPEN_FILE_ERROR:
			fprintf(stderr, "clips open file error while loading "
					"clips program: %s\n", x->scriptpath);
			return 1;
		case LE_PARSING_ERROR:
			fprintf(stderr, "clips program not parsable\n");
			return 1;
	}
}

static int load_script_from_string(Environment* env, QQ* x){
	if(x==NULL || x->type != QQSTRING){
		return 1;
	}
	if(LoadFromString(env, x->scriptstring, x->scriptlen)){
		fprintf(stderr, "succes load\n");
		return 0;
	}
	return 1;
}

uint32_t QQ_run(QQ* x){
	long long number_rules_run;
	int ret = 0;
	int err = 0;
	Environment* env;
	if(x == NULL){
		return RETQQRUN_SETUPERROR;
	}
	env = CreateEnvironment();
	if(0 != InfoModelRulemachine1Data_register(env)){
		DestroyEnvironment(env);
		return RETQQRUN_SETUPERROR;
	}
	if(0 != CIMR1SimpleOneshot_register_external_functions(env)){
		DestroyEnvironment(env);
		return RETQQRUN_SETUPERROR;
	}
	if(0 != Environment_register_CIMR1SimpeOneshot_child(env)){
		DestroyEnvironment(env);
		return RETQQRUN_SETUPERROR;
	}
	if(x->watch_activations){
		//Eval(env, "(watch activations)", NULL);
		Eval(env, "(watch facts)", NULL);
		Eval(env, "(watch rules)", NULL);
		Eval(env, "(seed 50)", NULL);
		Eval(env, "(set-strategy random)", NULL);
	}
	switch(x->type){
		case QQFILE:
			if(0 != load_script_from_file(env, x)){
				return RETQQRUN_SETUPERROR;
			}
			break;
		case QQSTRING:
			if(0 != load_script_from_string(env, x)){
				fprintf(stderr, "failed load_script_from_string\n");
				return RETQQRUN_SETUPERROR;
			}
			break;
	}

	CLIPSRerouteConfig_config(x->reroute_config, env);
	Reset(env);
	if(0 != CIMR1ImportInformation_use_imports(x->importer, env)){
		fprintf(stderr, "ERROR qq_run(call_import_file)\n");
		DestroyEnvironment(env);
		return RETQQRUN_SETUPERROR;
	}

	number_rules_run = Run(env, -1);
	fprintf(stderr, "Number rules run: %lld\n", number_rules_run);
	//Eval(env, "(println \"facts:\")", NULL);
	//Eval(env, "(facts)", NULL);
	if(check_no_error(env)){
		ret = RETQQRUN_NOERROR;
	} else {
		ret = RETQQRUN_FAIL;
	}
	DestroyEnvironment(env);
	return ret;
}


static bool check_no_error(Environment* env){
	CLIPSValue tmpcv;
	FunctionCallBuilder *fcb;
	fcb = CreateFunctionCallBuilder(env,0);
	FCBCall(fcb, "get-error", &tmpcv);
	FCBDispose(fcb);
	switch(tmpcv.header->type){
		case SYMBOL_TYPE:
			return tmpcv.lexemeValue == FalseSymbol(env);
		default:
			return false;
	}
}

void free_QQ(QQ* x){
	if (x == NULL) return;
	free(x->scriptpath);
	free_CLIPSRerouteConfig(x->reroute_config);
	free_CIMR1ImportInformation(x->importer);
	free(x);
}



void CIMR1Runner_add_stringcapture(QQ* x, const char* symbol){
	CLIPSRerouteConfig_add_stringcapture(x->reroute_config, symbol);
}

char* CIMR1Runner_extract_stringcapture(QQ* x, const char* symbol){
	return CLIPSRerouteConfig_extract_from_stringcapture(x->reroute_config, symbol);
}
