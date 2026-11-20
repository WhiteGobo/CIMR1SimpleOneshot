#include "CIMR1SimpleOneshot_import.h"
#include "CIMR1ImportInformation.h"
#include <cwalk.h>

typedef struct s_ROCrateData {
	char* baseiri;
	char* dirpath;
	char* metairi;
	//char* metapath;
} ROCrateData;

static char* ROCrateData_relative2iri(const ROCrateData*, const char*);
static int ROCrateData_create(ROCrateData*, const char* rocrate_path, const char* rocrate_iri);
void release_ROCrateData(ROCrateData);

void release_ROCrateData(ROCrateData x){
	if(x.dirpath != NULL){
		free(x.dirpath);
	}
	if(x.baseiri != NULL){
		free(x.baseiri);
	}
	//if(x.metapath != NULL){
	//	free(x.metapath);
	//}
	if(x.metairi != NULL){
		free(x.metairi);
	}
}

static char* ROCrateData_relative2iri(const ROCrateData* data, const char* path){
	char* ret = malloc(strlen(data->baseiri) + strlen(path) + 1);
	if(ret == NULL){
		return NULL;
	}
	sprintf(ret, "%s%s", data->baseiri, path);
	return ret;
}

static int ROCrateData_create(ROCrateData* x, const char* rocrate_path, const char* rocrate_iri){
	if(rocrate_iri == NULL){
		rocrate_iri = "http://WhiteGobo.github.io/CIMR1SimpleOneshot/.well-known/werjh/bkxc/";
	}
	struct cwk_segment qq = {
		.path = NULL,
		.segments = NULL,
		.begin = NULL,
		.end = NULL,
		.size = 0,
	};
	x->baseiri = NULL;
	x->dirpath = NULL;
	x->metairi = NULL;
	//x->metapath = NULL;
	if(!cwk_path_get_last_segment(rocrate_path, &qq)){
		return 1;
	}
	fprintf(stderr, "qwertz1 %s %s\n", qq.begin, rocrate_path);
	if(0 == strcmp(qq.begin, "ro-crate-metadata.json")){
		fprintf(stderr, "qwertz2\n");
		size_t l = qq.begin - rocrate_path;
		x->dirpath = malloc(l + 1);
		memcpy(x->dirpath, rocrate_path, l);
		x->dirpath[l] = '\0';
	} else {
		fprintf(stderr, "qwertz3\n");
		x->dirpath = malloc(strlen(rocrate_path) + 1);
		strcpy(x->dirpath, rocrate_path);
	}
	x->baseiri = malloc(strlen(rocrate_iri) + 1);
	strcpy(x->baseiri, rocrate_iri);
	x->metairi = malloc(strlen(rocrate_iri) + 1 + sizeof("ro-crate-metadata.json"));
	sprintf(x->metairi, "%sro-crate-metadata.json", rocrate_iri);
	return 0;
}


int import_local_rocrate(Environment* env, const char* basepath){
	int err;
	ROCrateData rocrate_data;
	if(basepath == NULL){
		return 1;
	}
	err = ROCrateData_create(&rocrate_data, basepath, NULL);
	if (err != 0){
		fprintf(stderr, "ERROR import_local_rocrate(rocrate data)\n");
		return 1;
	}
	err = InfoModelRulemachine1Data_add_local_import_location(env,
				rocrate_data.baseiri, rocrate_data.dirpath);
        if (err != 0){
		release_ROCrateData(rocrate_data);
		fprintf(stderr, "ERROR import_local_rocrate(import_location)\n");
		return 1;
        }

	char* tmp = ROCrateData_relative2iri(&rocrate_data, "ro-crate-metadata.json");
	if(tmp == NULL){
		return 1;
	}
	CLIPSValue tmpcv;
	FunctionCallBuilder *fcb;
	fcb = CreateFunctionCallBuilder(env, 2);
	FCBAppendString(fcb, tmp);
	FCBAppendSymbol(fcb, "<http://www.w3.org/ns/entailment/RDF>");
	FCBCall(fcb, "<http://white.gobo/ClipsInfoModelRulemachine1#import>", &tmpcv);
	FCBDispose(fcb);
	free(tmp);
	release_ROCrateData(rocrate_data);
	return 0;
}



int CIMR1ImportInformation_add_local_rocrate(CIMR1ImportInformation* info, const char* location){
	if (info == NULL || location == NULL) return 1;
	info->rocratelocation = malloc(strlen(location) + 1);
	if(info->rocratelocation == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return 1;
	}
	strcpy(info->rocratelocation, location);
	return 0;
}
