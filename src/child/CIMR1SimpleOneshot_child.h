#include <clips.h>
#include <ClipsInfoModelRulemachine1.h>
#include <ClipsInfoModelRulemachine1_literals.h>

#ifndef CIMR1_SIMPLE_ONESHOT_CHILD_INDEX
#define CIMR1_SIMPLE_ONESHOT_CHILD_INDEX USER_ENVIRONMENT_DATA + 13
#endif

typedef Environment Child;

int Environment_register_CIMR1SimpeOneshot_child(Environment*);
int Environment_generate_child(Environment*, CLIPSValue childid, CLIPSValue programid);

//static void add_end_condition(Environment *env, UDFContext *udfc, UDFValue *out);
Child* Environment_get_child(Environment*, CLIPSValue childid);
Child* UDFC_get_nth_argument_as_child(Environment*, UDFContext *udfc, int n);
Literal* Child_get_status(Environment* env, Child* child);
bool Child_check_rif_data(Child*, const char* data, const char* format);
void Environment_free_child(Environment*, CLIPSValue childid);
