#pragma once

#include <stdint.h>

typedef struct s_QQ QQ;

typedef enum {
	RETQQRUN_NOERROR = 0,
	RETQQRUN_FAIL = 1,
	RETQQRUN_SETUPERROR = 1 << 1,
} RETQQRUN;

QQ* CIMR1Runner_from_file(const char* scriptpath);
QQ* CIMR1Runner_from_string(const char* scriptstring, size_t scriptlen);
void free_QQ(QQ*);
void QQ_watch_activations(QQ*);
void QQ_unwatch_activations(QQ*);

void CIMR1Runner_add_stringcapture(QQ*, const char* symbol);
char* CIMR1Runner_extract_stringcapture(QQ*, const char* symbol);

void CIMR1Runner_add_import(QQ* runner, const char* riffile);
void CIMR1Runner_add_string_import(QQ* runner, const char* rifdata);
void CIMR1Runner_add_import_local_rocrate(QQ*, const char* testcrate);

/*
 *
 * return value one of RETQQRUN
 */
uint32_t QQ_run(QQ*);
