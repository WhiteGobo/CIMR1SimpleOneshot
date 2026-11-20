#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "CIMR1SimpleOneshot.h"
#include "CIMR1SimpleOneshot_builtin_models.h"

const char* script;
const char* testcrate;

static int parse_args(int argc, char *argv[]);
static int run_all_checks_in_testcrate();
static char* load_input_into_memory(const char* filepath);

int main(int argc, char *argv[]){
	if(0 != parse_args(argc, argv)){
		exit(EXIT_FAILURE);
	}
	if(0 != run_all_checks_in_testcrate()){
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

static int run_all_checks_in_testcrate(){
	int ret;
	int err;
	QQ* runner;
	RetGetModelAsString xx = get_testrunnermodel_as_string();
	fprintf(stderr, "brubru %s\n", xx.value);
	fprintf(stderr, "run_all_check_sin_testcrate not fuilly implemented\n");
	runner = CIMR1Runner_from_string(xx.value, xx.size);
	if (runner == NULL){
		fprintf(stderr, "CIMR1Runner_from_string failed\n");
		return 1;
	}
	char* scriptdata = load_input_into_memory(script);
	fprintf(stderr, "brubru5 %s %s\n", script, scriptdata);
	char* tmp = testrunner_input_script_as_rif_data(scriptdata);
	fprintf(stderr, "brubru7 %s\n", tmp);
	CIMR1Runner_add_string_import(runner, tmp);
	free(tmp);
	free(scriptdata);

	CIMR1Runner_add_import_local_rocrate(runner, testcrate);

	err = QQ_run(runner);
	free_QQ(runner);
	switch(err){
		case RETQQRUN_NOERROR:
			return 0;
		case RETQQRUN_FAIL:
			fprintf(stderr, "Program stopped with error\n");
			return 1;
		case RETQQRUN_SETUPERROR:
			fprintf(stderr, "Failed to load script\n");
			return 1;
		default:
			fprintf(stderr, "Unexpected return value of QQ_run\n");
			return 1;
	}
}

static struct option parse_options[] = {
	{"script", required_argument, NULL, 's'},
	{"testcrate", required_argument, NULL, 'i'},
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
				script = optarg;
				break;
			case 'i':
				testcrate = optarg;
				break;
			default:
				fprintf(stderr, "unrecognized argument %c\n", c);
				err = 1;
				break;
		}
	}
	if (script == NULL){
		fprintf(stderr, "no riffile given\n");
		err = 1;
	}
	if (testcrate == NULL){
		fprintf(stderr, "no testcrate given\n");
		err = 1;
	}
	return err;
}

static char* load_input_into_memory(const char* filepath){
	char *ret;
	long fsize;
	FILE *f = fopen(filepath, "rb");
	if (f == NULL){
		fprintf(stderr, "Failed to open file %s\n", filepath);
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	rewind(f);
	//fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

	ret = malloc(fsize + 1);
	fread(ret, fsize, 1, f);
	ret[fsize] = 0;
	fclose(f);
	return ret;
}
