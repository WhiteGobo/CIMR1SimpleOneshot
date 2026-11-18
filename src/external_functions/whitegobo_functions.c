#include "helper.h"
#include "namespace_WG.h"

#include "ClipsInfoModelRulemachine1_literals.h"


const UDFDescription builtinFunctionList_whitegobo[];

static void new_blanknode(Environment *env, UDFContext *udfc, UDFValue *out);
static void set_graph_in_errorstate(Environment *env, UDFContext *udfc, UDFValue *out);
static void import(Environment *env, UDFContext *udfc, UDFValue *out);
static void literal2clipsconstant(Environment *env, UDFContext *udfc, UDFValue *out);
static void iri2clipsconstant(Environment *env, UDFContext *udfc, UDFValue *out);
static void local2clipsconstant(Environment *env, UDFContext *udfc, UDFValue *out);

const UDFDescription builtinFunctionList_whitegobo[] = {
	{"<"_WG_new_blanknode_">", "*", 0, 0, "*",
		new_blanknode, "new_blanknode"},
	{"<"_WG_set_graph_in_errorstate_">", "*", 0, 1, "*",
		set_graph_in_errorstate, "set_graph_in_errorstate"},
	{"<"_WG_import_">", "*", 1, 2, "*",
		import, "import"},
	{"<"_WG_literal_to_clipsconstant_">", "*", 1, 1, "*",
		literal2clipsconstant, "literal2clipsconstant"},
	{"<"_WG_iri_to_clipsconstant_">", "*", 1, 1, "*",
		iri2clipsconstant, "iri2clipsconstant"},
	{"<"_WG_local_to_clipsconstant_">", "*", 2, 2, "*",
		local2clipsconstant, "local2clipsconstant"},
	//{"<"_WG__">", "*", 0, 0, "*",
	//	, ""},
	{NULL, NULL, 0,0, NULL, NULL, NULL},
};

static void new_blanknode(Environment *env, UDFContext *udfc, UDFValue *out){
	CLIPSValue tmpcv;
	Literal* ret = Literal_new_localnode(env, "local");
	if(ret == NULL){
			out->voidValue = VoidConstant(env);
			return;
	}
	tmpcv = Literal_to_CLIPSValue(env, ret);
	out->value = tmpcv.value;
	free_Literal(ret);
}

static void set_graph_in_errorstate(Environment *env, UDFContext *udfc, UDFValue *out){
	CLIPSValue tmpcv;
	Literal* tmplit;
	tmplit = UDF_get_nth_as_literal(udfc, 1);
	fprintf(stderr, "ERROR set_graph_in_errorstate(%s)\n",
			Literal_get_value(tmplit));
	tmpcv.lexemeValue = CreateString(env, "set_graph_in_errorstate doesnt support reason");
	SetErrorValue(env, tmpcv.header);
	free_Literal(tmplit);
	Halt(env);
			out->voidValue = VoidConstant(env);
}

static void import(Environment *env, UDFContext *udfc, UDFValue *out){
			out->voidValue = VoidConstant(env);
	fprintf(stderr, "Not implemented external function(import)\n");
}

static void literal2clipsconstant(Environment *env, UDFContext *udfc, UDFValue *out){
	char *tmpstr;
	char numstr[100];
	Literal* ret;
	UDFValue tmpudf;
	CLIPSValue tmpcv;
	UDFFirstArgument(udfc, ANY_TYPE_BITS, &tmpudf);
	tmpcv.value = tmpudf.value;
	switch(tmpcv.header->type){
		case STRING_TYPE:
			fprintf(stderr, "brubru1\n");
			tmpstr = malloc(strlen(tmpcv.lexemeValue->contents) + 5);
			sprintf(tmpstr, "\"%s\"", tmpcv.lexemeValue->contents);
			ret = Literal_new(tmpstr, NULL);
			free(tmpstr);
			break;
		case INTEGER_TYPE:
			fprintf(stderr, "brubru3\n");
			sprintf(numstr, "%ld", tmpcv.integerValue->contents);
			ret = Literal_new(numstr, NULL);
			break;
		case FLOAT_TYPE:
			sprintf(numstr, "%lf", tmpcv.integerValue->contents);
			ret = Literal_new(numstr, NULL);
			break;
		case SYMBOL_TYPE:
		default:
			out->voidValue = VoidConstant(env);
			fprintf(stderr, "ERROR literal2clipsconstant(type)\n");
			return;
	}
	fprintf(stderr, "brubru5 ");
	fprintf_Literal(stderr, ret);
	fprintf(stderr, "\n");
	tmpcv = Literal_to_CLIPSValue(env, ret);
	free_Literal(ret);
	out->value = tmpcv.value;
}

static void iri2clipsconstant(Environment *env, UDFContext *udfc, UDFValue *out){
	char *tmpstr;
	const char *tmplitstr;
	Literal* ret, *tmplit;
	UDFValue tmpudf;
	CLIPSValue tmpcv;
	UDFFirstArgument(udfc, ANY_TYPE_BITS, &tmpudf);
	tmpcv.value = tmpudf.value;
	switch(tmpcv.header->type){
		case SYMBOL_TYPE:
			ret = Literal_new(tmpcv.lexemeValue->contents, NULL);
			break;
		case STRING_TYPE:
			tmplit = Literal_from_CLIPSValue(env, tmpcv);
			tmplitstr = Literal_get_value(tmplit);
			tmpstr = malloc(strlen(tmplitstr) + 5);
			sprintf(tmpstr, "<%s>", tmplitstr);
			ret = Literal_new(tmpstr, NULL);
			free_Literal(tmplit);
			free(tmpstr);
			break;
		case INTEGER_TYPE:
		case FLOAT_TYPE:
		default:
			out->voidValue = VoidConstant(env);
			fprintf(stderr, "ERROR iri2clipsconstant(type)\n");
			return;
	}
	fprintf(stderr, "brubru4 ");
	fprintf_Literal(stderr, ret);
	fprintf(stderr, "\n");
	tmpcv = Literal_to_CLIPSValue(env, ret);
	free_Literal(ret);
	out->value = tmpcv.value;
}

static void local2clipsconstant(Environment *env, UDFContext *udfc, UDFValue *out){
	fprintf(stderr, "Not implemented external function(local2clipsconstant)\n");
			out->voidValue = VoidConstant(env);
}
