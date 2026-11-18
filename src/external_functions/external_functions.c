#include "CIMR1SimpleOneshot_ExternalFunctions.h"
#include "namespace_CIMR1SO.h"
#include "helper.h"
#include <ClipsInfoModelRulemachine1_literals.h>
#include "CIMR1SimpleOneshot_import.h"
#include "CIMR1SimpleOneshot_child.h"


extern const UDFDescription builtinFunctionList_whitegobo[];
static const UDFDescription builtinFunctionList_local[];
static bool add_literal_user_functions_list(Environment*, const UDFDescription*);
static CLIPSValue get_cv(Environment* env, UDFContext* udfc, int n);

int CIMR1SimpleOneshot_register_external_functions(Environment* env){
	if (!add_literal_user_functions_list(env, builtinFunctionList_local)){
		return 1;
	}
	if (!add_literal_user_functions_list(env, builtinFunctionList_whitegobo)){
		return 1;
	}
	return 0;
}


static bool add_literal_user_functions_list(Environment* env, const UDFDescription* descriptions)
{
	void* cntxt = NULL;
	for (UDFDescription *x = descriptions; x->clipsName != NULL; x++){
		AddUDFError err = AddUDF(env, x->clipsName, x->returnTypes,
				x->minArgs, x->maxArgs, x->argTypes, x->cfp,
				x->cName, cntxt);
		//fprintf(stderr, "register external function %s\n", x->clipsName);
		switch (err) {
			case AUE_NO_ERROR:
				break;
			case AUE_MIN_EXCEEDS_MAX_ERROR:
			case AUE_FUNCTION_NAME_IN_USE_ERROR:
			case AUE_INVALID_ARGUMENT_TYPE_ERROR:
			case AUE_INVALID_RETURN_TYPE_ERROR:
				fprintf(stderr, "Failed to register: %s\n",
						x->clipsName);
				return false;
		}
	}
	return true;
}


static void myfail(Environment *env, UDFContext *udfc, UDFValue *out);
static void myexit(Environment *env, UDFContext *udfc, UDFValue *out);
static void myimportrocrate(Environment *env, UDFContext *udfc, UDFValue *out);

static void generate_child(Environment *env, UDFContext *udfc, UDFValue *out);
static void add_deffacts(Environment *env, UDFContext *udfc, UDFValue *out);
static void add_end_condition(Environment *env, UDFContext *udfc, UDFValue *out);
static void run_rules(Environment *env, UDFContext *udfc, UDFValue *out);
static void get_status(Environment *env, UDFContext *udfc, UDFValue *out);
static void check_data(Environment *env, UDFContext *udfc, UDFValue *out);
static void free_child(Environment *env, UDFContext *udfc, UDFValue *out);

static const UDFDescription builtinFunctionList_local[] = {
	{"<"_CIMR1SO_fail_">", "*", 0, 0, "*", myfail, "myfail"},
	{"<"_CIMR1SO_exit_">", "*", 0, 0, "*", myexit, "myexit"},
	{"<"_CIMR1SO_import_rocrate_">", "*", 1, 1, "*",
		myimportrocrate, "myimportrocrate"},

	{"<"_CIMR1SO_generate_child_">", "*", 2, 2, "*",
		generate_child, "generate_child"},
	{"<"_CIMR1SO_add_deffacts_">", "*", 2, 2, "*",
		add_deffacts, "add_deffacts"},
	{"<"_CIMR1SO_add_end_condition_">", "*", 2, 2, "*",
		add_end_condition, "add_end_condition"},
	{"<"_CIMR1SO_run_rules_">", "*", 1, 1, "*",
		run_rules, "run_rules"},
	{"<"_CIMR1SO_get_status_">", "*", 1, 1, "*",
		get_status, "get_status"},
	{"<"_CIMR1SO_check_data_">", "*", 2, 2, "*",
		check_data, "check_data"},
	{"<"_CIMR1SO_free_child_">", "*", 1, 1, "*",
		free_child, ""},
	{NULL, NULL, 0,0, NULL, NULL, NULL},
};


static void myexit(Environment *env, UDFContext *udfc, UDFValue *out){
	//ClearErrorValue(env)
	Halt(env);
}


static void myfail(Environment *env, UDFContext *udfc, UDFValue *out){
	CLIPSValue tmpcv;
	tmpcv.lexemeValue = CreateString(env, "not specified error");
	SetErrorValue(env, tmpcv.header);
	Halt(env);
}

static void myimportrocrate(Environment *env, UDFContext *udfc, UDFValue *out){
	Literal* input;
	int err;
	input = UDF_get_nth_as_literal(udfc, 1);
	if (input == NULL){
		out->voidValue = VoidConstant(env);
		fprintf(stderr, "CIMR1:import-rocrate failed(input)\n");
		UDFThrowError(udfc);
		return;
	}
	err = import_local_rocrate(env, Literal_get_value(input));
	out->lexemeValue = CreateBoolean(env, err == 0);
	out->lexemeValue = CreateBoolean(env, false);
}


static void generate_child(Environment *env, UDFContext *udfc, UDFValue *out){
	int err;
	UDFValue tmpudf;
	CLIPSValue childid, programid;
	if(!UDFNthArgument(udfc, 1, ANY_TYPE_BITS, &tmpudf)){
		fprintf(stderr, "Failed to get 1th argument\n");
		return;
	}
	childid.header = tmpudf.header;
	if(!UDFNthArgument(udfc, 2, ANY_TYPE_BITS, &tmpudf)){
		fprintf(stderr, "Failed to get 2th argument\n");
		return;
	}
	programid.header = tmpudf.header;
	err = Environment_generate_child(env, childid, programid);
	if(err != 0){
		fprintf(stderr, "ERROR generate_child\n");
	}
}


static void add_deffacts(Environment *env, UDFContext *udfc, UDFValue *out){
	fprintf(stderr, "ERROR add_deffacts not implemented\n");
	out->lexemeValue = CreateBoolean(env, false);
}


static void add_end_condition(Environment *env, UDFContext *udfc, UDFValue *out){
	fprintf(stderr, "ERROR add_end_condition not implemented\n");
	out->lexemeValue = CreateBoolean(env, false);
}


static void run_rules(Environment *env, UDFContext *udfc, UDFValue *out){
	Child *child;
	long long number_rules_run;
	CLIPSValue childid = get_cv(env, udfc, 1);
	if(childid.header->type == VOID_TYPE){
		fprintf(stderr, "ERROR run_rules\n");
		return;
	}
	child = Environment_get_child(env, childid);
	if(child == NULL){
		fprintf(stderr, "ERROR run_rules(got no child for id)\n");
		return;
	}
	number_rules_run = Run(child, -1);
	fprintf(stderr, "WARNING run_rules doesnt check failure\n");
	out->integerValue = CreateInteger(env, number_rules_run);
}

static void get_status(Environment *env, UDFContext *udfc, UDFValue *out){
	CLIPSValue retcv;
	Literal* ret_lit;
	Child* child;
	child = UDFC_get_nth_argument_as_child(env, udfc, 1);
	if(child == NULL){
		fprintf(stderr, "ERROR get_status\n");
		return;
	}
	ret_lit = Child_get_status(env, child);
	if(ret_lit == NULL){
		fprintf(stderr, "WARNING get_status not fully implemented\n");
		out->lexemeValue = CreateBoolean(env, true);
		return;
	}
	retcv = Literal_to_CLIPSValue(env, ret_lit);
	free_Literal(ret_lit);
	out->value = retcv.value;

}


static void check_data(Environment *env, UDFContext *udfc, UDFValue *out){
	bool ret;
	Child* child;
	child = UDFC_get_nth_argument_as_child(env, udfc, 1);
	if(child == NULL){
		fprintf(stderr, "ERROR check_data(no child found)\n");
		out->lexemeValue = CreateBoolean(env, false);
		return;
	}
	fprintf(stderr, "ERROR check_Data not implemented\n");
	ret = Child_check_rif_data(child, "<http://example.com/A>()", "rifps");
	out->lexemeValue = CreateBoolean(env, ret);
}


static void free_child(Environment *env, UDFContext *udfc, UDFValue *out){
	CLIPSValue childid = get_cv(env, udfc, 1);
	if(childid.header->type == VOID_TYPE){
		return;
	}
	Environment_free_child(env, childid);
}


static CLIPSValue get_cv(Environment* env, UDFContext* udfc, int n){
	UDFValue tmpudf;
	CLIPSValue tmpcv;
	if(!UDFNthArgument(udfc, n, ANY_TYPE_BITS, &tmpudf)){
		fprintf(stderr, "Failed to get %dth argument\n", n);
		tmpcv.voidValue = VoidConstant(env);
		return tmpcv;
	}
	tmpcv.header = tmpudf.header;
	return tmpcv;
}
