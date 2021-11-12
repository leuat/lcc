#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* string_token_type[] = {"id","{","}","(",")","[","]",";","integer_const","eof",
"int","char","=","==",",","asm","string","+","-","*","/","|","&","pointer"};



t_token create_token(enum_token_type type, char* str_value, int i_value) {
	t_token t;
	strcpy(t.str_value,str_value);
	t.ivalue = i_value;
	t.type = type;
	t.is_pointer = false;
	t.is_string = false;
	t.array_count = -1; // 0 = is array with unknown size, >=1 size
	t.line_number = data_current_line;
	t.large_string = NULL;
	t.next = NULL;
	return t;
}

t_token* create_dynamic_token(enum_token_type type, char* str_value, int i_value) {
	t_token* t = malloc (sizeof (struct t_token));

	strcpy(t->str_value,str_value);
	t->ivalue = i_value;
	t->type = type;
	t->is_pointer = false;
	t->is_string = false;
	t->array_count = -1; // 0 = is array with unknown size, >=1 size
	t->line_number = data_current_line;
	t->large_string = NULL;
	t->next = NULL;

	return t;
}


