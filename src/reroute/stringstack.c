#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "stringstack.h"

#define STRINGLIST_LENGTH 512
typedef struct s_StringList {
	struct s_StringList* rest;
	char* first;
} StringList;

typedef struct s_StringStack {
	StringList* list;
} StringStack;

static StringList* StringList_append(StringList*, const char*);
static void free_StringList(StringList*);

StringStack* StringStack_new(){
	StringStack* ret = malloc(sizeof(StringStack));
	if(ret == NULL){
		fprintf(stderr, "CRITICAL ERROR malloc failed\n");
		return NULL;
	}
	ret->list = NULL;
	return ret;
}


char* StringStack_combine(StringStack* stack){
	char* ret, *tmp;
	size_t size=0;
	if(stack == NULL){
		ret = malloc(1);
		ret[0] = '\0';
		return ret;
	}
	for(StringList *l = stack->list; l!= NULL; l=l->rest){
		size += strlen(l->first);
	}
	ret = malloc(size + 1);
	tmp = ret;
	for(StringList *l = stack->list; l!= NULL; l=l->rest){
		strcpy(tmp, l->first);
		tmp += strlen(tmp);
	}
	return ret;
}


void StringStack_append(StringStack* x, const char* input){
	if(x == NULL) return;
	x->list = StringList_append(x->list, input);
}


void free_StringStack(StringStack* x){
	if(x == NULL) return;
	free_StringList(x->list);
	free(x);
}

static StringList* StringList_append(StringList* x, const char* input){
	if(input == NULL) return x;
	StringList *ret, *new;
	new = malloc(sizeof(StringList));
	if(x == NULL){
		ret = new;
	} else {
		ret = x;
		while(x->rest != NULL){
			x = x->rest;
		}
		x->rest = new;
	}
	new->first = malloc(strlen(input) + 1);
	strcpy(new->first, input);
	new->rest = NULL;
	return ret;
}

static void free_StringList(StringList* x){
	StringList *tmp;
	if(x == NULL) return;
	while(x != NULL){
		tmp = x->rest;
		free(x->first);
		free(x);
		x = tmp;
	}
}

