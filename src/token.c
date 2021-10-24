#include "token.h"
#include <string.h>

char* string_token_type[] = {"id","{","}","(",")","[","]",";","integer_const","eof",
"int","char","=","=="};


t_token create_token(enum_token_type type, char* str_value, int i_value) {
	t_token t;
	strcpy(t.str_value,str_value);
	t.ivalue = i_value;
	t.type = type;
	t.line_number = data_current_line;
	return t;
}
