#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "CIMR1SimpleOneshot.h"
#include "CIMR1SimpleOneshot_builtin_models.h"
#include <CInterfaceClipsSerializer.h>
#include <Mime2Rdf4C.h>

const char* riffile;
char* serializer_input = NULL;

static int parse_args(int argc, char *argv[]);
static int generate_serializer_input();
static int serialize_clips(const char*);

int main(int argc, char *argv[]){
	int err;
	if(0 != parse_args(argc, argv)){
		exit(EXIT_FAILURE);
	}
	if(0 != generate_serializer_input()){
		exit(EXIT_FAILURE);
	}
	if(serializer_input == NULL){
		fprintf(stderr, "ERROR missing serializer_input\n");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "Serializer input: %s\n", serializer_input);
	err = serialize_clips(serializer_input);
	free(serializer_input);
	if(err != 0){
		fprintf(stderr, "Serialize clips program failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

static int serialize_clips(const char* input){
	int err;
	Mime2Rdf4C_ParserConfig* parser = Mime2Rdf4C_get_parser_from_ext("ttl");
	if(parser == NULL){
		fprintf(stderr, "Mime2Rdf4C_get_parser_from_ext failed\n");
		return 1;
	}
	ClipsSerializer* serializer = ClipsSerializer_new();
	if(serializer == NULL){
		fprintf(stderr, "ClipsSerializer_new failed\n");
		return 1;
	}
	err = Mime2Rdf4C_parse(input, (TripleHandler*) ClipsSerializer_add,
				serializer, parser);
	if (err != 0){
		fprintf(stderr, "Mime2Rdf4C_parser failed\n");
		return 1;
	}
	char* output = ClipsSerializer_finish(serializer);
	if (output == NULL){
		fprintf(stderr, "ClipsSerializer_finish failed\n");
		return 1;
	}
	fprintf(stdout, "out:%s\n", output);
	free(output);
	return 0;
}


static int generate_serializer_input(){
	QQ* runner;
	int ret = 0;
	RetGetModelAsString xx = get_oldmodel_as_string();

	runner = CIMR1Runner_from_string(xx.value, xx.size);
	if (runner == NULL){
		fprintf(stderr, "CIMR1Runner_from_string failed\n");
		return 1;
	}
	CIMR1Runner_add_import(runner, riffile);
	CIMR1Runner_add_stringcapture(runner, "stdout");
	//QQ_watch_activations(runner);

	switch(QQ_run(runner)){
		case RETQQRUN_NOERROR:
			ret = 0;
			break;
		case RETQQRUN_FAIL:
			ret = 1;
			fprintf(stderr, "Program stopped with error\n");
			break;
		case RETQQRUN_SETUPERROR:
			ret = 1;
			fprintf(stderr, "Failed to load script\n");
			break;
		default:
			ret = 1;
			fprintf(stderr, "Unexpected return value of QQ_run\n");
			break;
	}
	serializer_input = CIMR1Runner_extract_stringcapture(runner, "stdout");
	free_QQ(runner);
	return ret;
}

static struct option parse_options[] = {
	{"riffile", required_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
};

static int parse_args(int argc, char *argv[]){
	int err = 0;
	int c = 0;
	int option_index;
	while(c != -1){
		c = getopt_long(argc, argv, "",
				parse_options, &option_index);
		switch(c){
			case -1: //end of arguments
				break;
			case 's':
				riffile = optarg;
				break;
			default:
				fprintf(stderr, "unrecognized argument %c\n", c);
				fprintf(stderr, "qq %s\n", optarg);
				err = 1;
				break;
		}
	}
	if (riffile == NULL){
		fprintf(stderr, "no riffile given\n");
		err = 1;
	}
	return err;
}
