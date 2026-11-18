#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "CIMR1SimpleOneshot.h"

const char* scriptpath;
static bool fail_on_exit = false;

static int parse_args(int argc, char *argv[]);
static bool QQ_run_with_expected_result(QQ*);

int main(int argc, char *argv[]){
	int err;
	QQ* runner;
	if(0 != parse_args(argc, argv)){
		exit(EXIT_FAILURE);
	}

	runner = CIMR1Runner_from_file(scriptpath);
	if (runner == NULL){
		fprintf(stderr, "QQ_new failed\n");
		exit(EXIT_FAILURE);
	}
	if(QQ_run_with_expected_result(runner)){
		err = EXIT_SUCCESS;
	} else {
		err = EXIT_FAILURE;
	}

	free_QQ(runner);
	exit(err);
}

static bool QQ_run_with_expected_result(QQ* runner){
	switch(QQ_run(runner)){
		case RETQQRUN_NOERROR:
			if(!fail_on_exit){
				return true;
			} else {
				fprintf(stderr, "Run exited without error, but "
						"an error was expected\n");
				return false;
			}
			break;
		case RETQQRUN_FAIL:
			if(fail_on_exit){
				return true;
			} else {
				fprintf(stderr, "Run exited with error\n");
				return false;
			}
			break;
		case RETQQRUN_SETUPERROR:
			fprintf(stderr, "Failed to setup\n");
			break;
		default:
			fprintf(stderr, "Unexpected return value of QQ_run\n");
			break;
	}
	return false;
}


static struct option parse_options[] = {
	{"script", required_argument, NULL, 's'},
	{"fail-on-exit", no_argument, NULL, 'x'},
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
			case 'x':
				fail_on_exit = true;
				break;
			case 's':
				scriptpath = optarg;
				break;
			default:
				fprintf(stderr, "unrecognized argument %c\n", c);
				fprintf(stderr, "qq %s\n", optarg);
				err = 1;
				break;
		}
	}
	if (scriptpath == NULL){
		fprintf(stderr, "no scriptpath given\n");
		err = 1;
	}
	return err;
}
