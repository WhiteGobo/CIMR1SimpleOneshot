#include "CIMR1SimpleOneshot_import.h"
#include "CIMR1ImportInformation.h"
#include <ClipsInfoModelRulemachine1_literals.h>

#include <cwalk.h>

static int call_import_file(Environment* env, const char* filepath);
static int call_import_string(Environment* env, const char* data, const char* ext);

CIMR1ImportInformation* CIMR1ImportInformation_new(){
	CIMR1ImportInformation* ret = malloc(sizeof(CIMR1ImportInformation));
	if(ret == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return NULL;
	}
	ret->importlocation = NULL;
	ret->importstring = NULL;
	ret->rocratelocation = NULL;
	return ret;
}

int CIMR1ImportInformation_add_string(CIMR1ImportInformation* info, const char* data){
	if (info == NULL || data == NULL) return 1;
	info->importstring = malloc(strlen(data) + 1);
	if(info->importstring == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return 1;
	}
	strcpy(info->importstring, data);
	return 0;
}

int CIMR1ImportInformation_add_location(CIMR1ImportInformation* info, const char* location)
{
	if (info == NULL || location == NULL) return 1;
	info->importlocation = malloc(strlen(location) + 1);
	if(info->importlocation == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return 1;
	}
	strcpy(info->importlocation, location);
	return 0;
}

typedef struct s_RetFilepath2URI {
	int err;
	char* fileuri;
	char* baseuri;
	char* basepath;
} RetFilepath2URI;

static void release_RetFilepath2URI(RetFilepath2URI x){
	free(x.fileuri);
	free(x.baseuri);
	free(x.basepath);
}

static RetFilepath2URI filepath2uri(const char* filepath){
	size_t tmps;
	char abspath[255];
	const char* baseuri = "http://WhiteGobo.github.io/CIMR1SimpleOneshot/.well-known/";
	RetFilepath2URI ret = {
		.err = 0,
		.fileuri = NULL,
		.baseuri = NULL,
		.basepath=NULL,
	};
	cwk_path_get_absolute("/", filepath, abspath, 255);
	cwk_path_get_dirname(abspath, &tmps);
	ret.basepath = malloc(tmps + 1);
	memcpy(ret.basepath, abspath, tmps);
	ret.basepath[tmps] = '\0';
	ret.baseuri = malloc(strlen(baseuri) + 1);
	strcpy(ret.baseuri, baseuri);
	ret.fileuri = malloc(strlen(baseuri) + strlen(abspath+tmps) + 1);
	memcpy(ret.fileuri, baseuri, strlen(baseuri));
	strcpy(ret.fileuri + strlen(baseuri), abspath+tmps);
	return ret;
}

static int call_import_string(Environment* env, const char* data, const char* ext){
	CLIPSValue tmpcv, datacv;
	Literal* data_lit;
	FunctionCallBuilder *fcb;
	data_lit = Literal_new(data, NULL);
	if(data_lit == NULL){
		return 1;
	}
	datacv = Literal_to_CLIPSValue(env, data_lit);
	free_Literal(data_lit);

	fcb = CreateFunctionCallBuilder(env, 2);
	if(fcb == NULL){
		return 1;
	}
	FCBAppend(fcb, &datacv);
	FCBAppendString(fcb, ext);
	//FCBAppendString(fcb, "http://www.w3.org/ns/entailment/RDF");
	FCBCall(fcb, "<http://white.gobo/ClipsInfoModelRulemachine1#load>", &tmpcv);
	FCBDispose(fcb);
	return 0;
}


int call_import_file(Environment* env, const char* filepath){
	int err;
	RetFilepath2URI tmp_fp2uri = filepath2uri(filepath);
	if(tmp_fp2uri.err != 0){
		fprintf(stderr, "Failed to process filepath %s\n", filepath);
		return 1;
	}

	err = InfoModelRulemachine1Data_add_local_import_location(
				env, tmp_fp2uri.baseuri, tmp_fp2uri.basepath);
        if (err != 0){
                fprintf(stderr, "Failed to add local import location\n");
		return 1;
        }

	CLIPSValue tmpcv;
	FunctionCallBuilder *fcb;
	fcb = CreateFunctionCallBuilder(env, 2);
	FCBAppendString(fcb, tmp_fp2uri.fileuri);
	FCBAppendSymbol(fcb, "<http://www.w3.org/ns/entailment/RDF>");
	FCBCall(fcb, "<http://white.gobo/ClipsInfoModelRulemachine1#import>", &tmpcv);
	FCBDispose(fcb);
	fprintf(stderr, "WARNING missing control if import failed\n");
	release_RetFilepath2URI(tmp_fp2uri);
	return 0;
}


int CIMR1ImportInformation_use_imports(CIMR1ImportInformation* info, Environment* env){
	int err;
	if(info->importlocation != NULL){
		err = call_import_file(env, info->importlocation);
		if(err != 0){
			return 1;
		}
	}
	if(info->importstring != NULL){
		err = call_import_string(env, info->importstring, "ttl");
		if(err != 0){
			return 1;
		}
	}
	if(info->rocratelocation != NULL){
		err = import_local_rocrate(env, info->rocratelocation);
		if(err != 0){
			return 1;
		}
	}
	return 0;
}


void free_CIMR1ImportInformation(CIMR1ImportInformation* x){
	if(x == NULL) return;
	if(x->importlocation != NULL){
		free(x->importlocation);
	}
	if(x->importstring != NULL){
		free(x->importstring);
	}
	free(x);
}
