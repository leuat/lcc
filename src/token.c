#include "token.h"
#include <string.h>

char* string_token_type[] = {"id","{","}","(",")","[","]",";","integer_const","eof",
"int","char","=","=="};

#define NO_BUILTIN_TYPES 4
char* builtin_types[4] = {"void","int","char","bool"};

t_token create_token(enum_token_type type, char* str_value, int i_value) {
	t_token t;
	strcpy(t.str_value,str_value);
	t.ivalue = i_value;
	t.type = type;
	t.line_number = data_current_line;
	return t;
}

bool verify_type(char* type) {
	for (int i=0;i<NO_BUILTIN_TYPES;i++)
		if (strcmp(type,builtin_types[i])==0)
			return true;

	return false;
}
