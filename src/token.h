#ifndef TOKEN_H
#define TOKEN_H

#define TOKEN_MAX_SIZE 64

#include "data.h"
#include <stdbool.h>


typedef enum {tt_id, tt_lcbracket,tt_rcbracket,tt_lparen,tt_rparen,tt_lbracket, tt_rbracket, tt_semicolon,
								tt_integer_const, tt_eof,tt_int,tt_char,tt_assign, tt_equals,tt_comma, tt_asm, tt_string,
							tt_asterisk, tt_long, tt_int64} enum_token_type;



extern char *string_token_type[];

typedef struct {
	enum_token_type type;
	char str_value[TOKEN_MAX_SIZE];
	char* large_string;
	int ivalue;
	bool is_pointer;
	bool is_string;
    int array_count;
	int line_number;
	bool is_reference;
} t_token;

t_token create_token(enum_token_type type, char* str_value, int i_value);




#endif
