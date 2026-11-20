#include "CIMR1SimpleOneshot_child.h"
#include "CIMR1SimpleOneshot_ExternalFunctions.h"
#include <ClipsInfoModelRulemachine1_data_importer.h>
#include <hashtable.h>
#include <RIFRDFQuery.h>
#include <Mime2Rdf4C.h>

#define INITIAL_HASHTABLE_SIZE 30

typedef CLIPSValue DKey;
typedef Child* DValue;

typedef struct s_CIMR1SO_child_data {
	HashTable cv2child;
} CIMR1SO_child_data;

typedef bool (*ht_iterate_t) (void* key, void* value, void* user);

static int insert_cv2child(Environment*, CIMR1SO_child_data*, DKey, DValue);
static int erase_cv2child(Environment*, CIMR1SO_child_data*, DKey);
static size_t hash_cv2child(DKey*, size_t);
static int compare_cv2child(DKey*, DKey*, size_t);
static Child* lookup_cv2child(CIMR1SO_child_data*, DKey);
static void init_cv2child(CIMR1SO_child_data*);

static Child* Child_new(Environment *parent, CLIPSValue programid);
static Child* Child_new_from_string(Environment *parent, const char* program);
static void free_Child(Child*);

static CIMR1SO_child_data* Environment_get_child_data(Environment*);
static void Environment_release_CIMR1SimpeOneshot_child(Environment*);
static size_t cv2hash(CLIPSValue);
static bool find_this_data_in(const char* query, const char* query_format, const char* data, const char* data_format);
static char* get_script_from_programid(Environment *env, CLIPSValue programid);

int Environment_register_CIMR1SimpeOneshot_child(Environment* env){
	bool success;
	if(env == NULL){
		return 1;
	}
	success = AllocateEnvironmentData(
				env, CIMR1_SIMPLE_ONESHOT_CHILD_INDEX,
				sizeof(CIMR1SO_child_data),
				Environment_release_CIMR1SimpeOneshot_child);
	if(!success){
		ExitRouter(env, EXIT_FAILURE);
		return 1;
	}
	CIMR1SO_child_data* data = Environment_get_child_data(env);
	init_cv2child(data);
	return 0;
}

static void init_cv2child(CIMR1SO_child_data* data){
	ht_setup(&(data->cv2child), sizeof(DKey), sizeof(DValue), INITIAL_HASHTABLE_SIZE);
	data->cv2child.hash = (hash_t) hash_cv2child;
	data->cv2child.compare = (comparison_t) compare_cv2child;
}


static void Environment_release_CIMR1SimpeOneshot_child(Environment* env){
	CIMR1SO_child_data* data = Environment_get_child_data(env);
	fprintf(stderr, "ERROR Environment_release_CIMR1SimpeOneshot_child not implemented\n");
}


int Environment_generate_child_from_string(Environment* env, CLIPSValue childid, const char* program)
{
	Child* new;
	if(childid.header->type == VOID_TYPE){
		return 1;
	}
	CIMR1SO_child_data* data = Environment_get_child_data(env);

	new = Child_new_from_string(env, program);
	if(0 != insert_cv2child(env, data, childid, new)){
		return 1;
	}
	return 0;
}


int Environment_generate_child(Environment* env, CLIPSValue childid, CLIPSValue programid)
{
	Child* new;
	if(childid.header->type == VOID_TYPE){
		return 1;
	}
	CIMR1SO_child_data* data = Environment_get_child_data(env);

	new = Child_new(env, programid);
	if(0 != insert_cv2child(env, data, childid, new)){
		return 1;
	}
	return 0;
}

static int insert_cv2child(Environment* env, CIMR1SO_child_data* data, DKey key, DValue value)
{
	int err;
	err = ht_insert(&(data->cv2child), &key, &value);
	if(err == 0){
		RetainCV(env, &key);
	}
}

Child* Environment_get_child(Environment* env, CLIPSValue childid){
	if(childid.header->type == VOID_TYPE){
		return NULL;
	}
	CIMR1SO_child_data* data = Environment_get_child_data(env);
	return lookup_cv2child(data, childid);
}

static Child* lookup_cv2child(CIMR1SO_child_data* x, DKey key){
	DValue* val;
	val = ht_lookup(&(x->cv2child), &key);
	return *val;
}

Child* UDFC_get_nth_argument_as_child(Environment* env, UDFContext *udfc, int n){
	Child* ret;
	UDFValue tmpudf;
	CLIPSValue tmpcv;
	if(!UDFNthArgument(udfc, n, ANY_TYPE_BITS, &tmpudf)){
		fprintf(stderr, "ERROR UDFC_get_nth_argument_as_child"
				"(no %dth argument)\n", n);
		return NULL;
	}
	tmpcv.header = tmpudf.header;
	if(tmpcv.header->type == VOID_TYPE){
		return NULL;
	}
	ret = Environment_get_child(env, tmpcv);
	return ret;
}


Literal* Child_get_status(Environment* env, Child* child){
	CLIPSValue tmpcv;
	FunctionCallBuilder *fcb;
	if (child == NULL){
		return NULL;
	}
	fcb = CreateFunctionCallBuilder(env, 0);
	FCBCall(fcb, "get-error", &tmpcv);
	FCBDispose(fcb);
	if(tmpcv.header->type == SYMBOL_TYPE){
		if (tmpcv.lexemeValue == FalseSymbol(env)){
			tmpcv.lexemeValue = TrueSymbol(env);
		} else if (tmpcv.lexemeValue == TrueSymbol(env)){
			tmpcv.lexemeValue = FalseSymbol(env);
		}
	}
	return Literal_from_CLIPSValue(env, tmpcv);
}

bool Child_check_rif_data(Child* child, const char* data, const char* format){
	if(child == NULL || data == NULL || format == NULL){
		fprintf(stderr, "ERROR Child_check_rif_data(input)\n");
		return false;
	}
	bool ret;
	char* conclusion_data;
	const char* conclusion_format = "ttl";

	conclusion_data = InfoModelRulemachine1Data_export_rif_data(child, conclusion_format);
	if(conclusion_data == NULL){
		fprintf(stderr, "ERROR Child_check_rif_data\n");
		return false;
	}
	ret = find_this_data_in(data, format, conclusion_data, conclusion_format);
	free(conclusion_data);
	return ret;
}

static RIFRDFQuery_Graph* myRIFRDFQuery_Graph_from_data(const char* data, const char* format);

static bool find_this_data_in(const char* query, const char* query_format, const char* data, const char* data_format)
{
	int is_same;
	RIFRDFQuery_Graph *conclusion_graph, *expected_graph;

	expected_graph = myRIFRDFQuery_Graph_from_data(query, query_format);
	if (expected_graph == NULL){
		fprintf(stderr, "ERROR Child_check_rif_data(parse query): %s\n", query);
		return false;
	}
	conclusion_graph = myRIFRDFQuery_Graph_from_data(data, data_format);
	if (conclusion_graph == NULL){
		fprintf(stderr, "ERROR Child_check_rif_data(parse data): %s\n", data);
		free_RIFRDFQuery_Graph(expected_graph);
		return false;
	}
	is_same = RIFRDFQuery_Graph_query_rif_data(conclusion_graph, expected_graph);
	free_RIFRDFQuery_Graph(conclusion_graph);
	free_RIFRDFQuery_Graph(expected_graph);
	if (is_same == 0){
		return true;
	} else {
		return false;
	}
}


static RIFRDFQuery_Graph* myRIFRDFQuery_Graph_from_data(const char* data, const char* format)
{
	if(data == NULL || format == NULL) return NULL;
	int err;
	RIFRDFQuery_Graph* ret;
	Mime2Rdf4C_ParserConfig* parser = Mime2Rdf4C_get_parser_from_ext(format);
	if(parser == NULL){
		fprintf(stderr, "ERROR RIFRDFQuery_Graph_from_data\n");
		return NULL;
	}
	ret = RIFRDFQuery_Graph_new();
	if (ret == NULL) {
		fprintf(stderr, "ERROR RIFRDFQuery_Graph_from_data\n");
		return NULL;
	}
	err = Mime2Rdf4C_parse(data,
			(TripleHandler*) RIFRDFQuery_Graph_add, ret,
			parser);
	free_Mime2Rdf4CParserConfig(parser);
	if (err != 0){
		fprintf(stderr, "ERROR RIFRDFQuery_Graph_from_data\n");
		free_RIFRDFQuery_Graph(ret);
		return NULL;
	}
	return ret;
}


static int erase_cv2child(Environment* env, CIMR1SO_child_data* data, DKey key){
	Child* val;
	int err;
	val = lookup_cv2child(data, key);
	if(val == NULL){
		return 1;
	}
	free_Child(val);
	err = ht_erase(&(data->cv2child), &key);
	if(err == 0){
		ReleaseCV(env, &key);
	}
	return err;
}

void Environment_free_child(Environment* env, CLIPSValue childid){
	CIMR1SO_child_data* data = Environment_get_child_data(env);
	erase_cv2child(env, data, childid);
}


static CIMR1SO_child_data* Environment_get_child_data(Environment* env){
	CIMR1SO_child_data* ret;
	ret = (CIMR1SO_child_data*) GetEnvironmentData(
					env, CIMR1_SIMPLE_ONESHOT_CHILD_INDEX);
	return ret;
}

static size_t hash_cv2child(DKey* key, size_t){
	return cv2hash(*key);
}

static size_t cv2hash(CLIPSValue cv){
	switch(cv.header->type){
		case SYMBOL_TYPE:
		case STRING_TYPE:
			return HashSymbol(cv.lexemeValue->contents,
					strlen(cv.lexemeValue->contents));
		case FACT_ADDRESS_TYPE:
			fprintf(stderr, "cv2hash fact address\n");
			break;
		case EXTERNAL_ADDRESS_TYPE:
			fprintf(stderr, "cv2hash external address\n");
			break;
		case FLOAT_TYPE:
			fprintf(stderr, "cv2hash float address\n");
			break;
		case INSTANCE_ADDRESS_TYPE:
			fprintf(stderr, "cv2hash instance address\n");
			break;
		case INSTANCE_NAME_TYPE:
			fprintf(stderr, "cv2hash instance name\n");
			break;
		case INTEGER_TYPE:
			fprintf(stderr, "cv2hash integer\n");
			break;
		case MULTIFIELD_TYPE:
			fprintf(stderr, "cv2hash multifield\n");
			break;
		case VOID_TYPE:
			return 0;
		default:
	}
	fprintf(stderr, "ERROR cv2hash(not supported type)\n");
	return 1;
}

static int compare_cv2child(DKey* x, DKey* y, size_t){
	int cmp;
	cmp = x->header->type - y->header->type;
	if(cmp != 0){
		return cmp;
	}
	switch(x->header->type){
		case SYMBOL_TYPE:
		case STRING_TYPE:
			return strcmp(x->lexemeValue->contents, y->lexemeValue->contents);
	}
	fprintf(stderr, "ERROR compare_cv2child not fully implemented\n");
	return 1;
}


static Child* Child_new(Environment *parent, CLIPSValue programid){
	char* script = get_script_from_programid(parent, programid);
	if(script == NULL){
		fprintf(stderr, "ERROR Child_new(programid)");
		return NULL;
	}
	return Child_new_from_string(parent, script);
}


static Child* Child_new_from_string(Environment *parent, const char* script){
	if(script == NULL) return NULL;
	Child* env = CreateEnvironment();
	if(0 != InfoModelRulemachine1Data_register(env)){
		free_Child(env);
		return NULL;
	}
	if(0 != CIMR1SimpleOneshot_register_external_functions(env)){
		free_Child(env);
		return NULL;
	}
	if(0 != Environment_register_CIMR1SimpeOneshot_child(env)){
		free_Child(env);
		return NULL;
	}
	if(!LoadFromString(env, script, strlen(script))){
		fprintf(stderr, "ERROR Child_new(broken script)\n");
		free_Child(env);
		return NULL;
	}
	return env;
}


static char* get_script_from_programid(Environment *env, CLIPSValue programid){
	const char* val;
	Literal* tmplit = Literal_from_CLIPSValue(env, programid);
	if(tmplit == NULL){
		fprintf(stderr, "ERROR programid has no value\n");
		return NULL;
	}
	val = Literal_get_value(tmplit);
	DataDescription data = Environment_get_import_data_from_iri(env, val);
	if(data.data == NULL){
		fprintf(stderr, "ERROR not data for \"%s\"\n", val);
		free_Literal(tmplit);
		return NULL;
	}
	free_Literal(tmplit);
	free(data.format);
	return data.data;
}

static void free_Child(Child* env){
	DestroyEnvironment(env);
}
