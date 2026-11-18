#pragma once

#include <clips.h>

/**
 *
 * See rules clips documentation "8.2 Registering User Defined Functions"
 *
 * return or arg types:
 * 'b' Boolean
 * 'd' Double Precision Float
 * 'e' External Address
 * 'f' Fact Address
 * 'i' Instance Address
 * 'l' Long Long Integer
 * 'm' Multifield
 * 'n' Instance Name
 * 's' String
 * 'y' Symbol
 * 'v' Void—No Return Value
 * '*' Any Type
 *
 * min/max: any or UNBOUNDED
 *
 * example: 
 * {"<"_CIMR1_import_">", "*", 1, 2, "*", CIMR1_import_rdf, "CIMR1_import_rdf"}
 */
typedef struct s_UDFDescription {
	const char *clipsName;
	const char *returnTypes;
	unsigned short minArgs;
	unsigned short maxArgs;
	const char *argTypes;
	UserDefinedFunction *cfp;
	const char *cName;
} UDFDescription;
