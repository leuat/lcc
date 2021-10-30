#ifndef __LEXER
#define __LEXER

#include "util.h"
#include "data.h"
#include "token.h"
#include <stdbool.h>


#define MAX_ID_SIZE 64
#define MAX_STRING_SIZE 5192

char lexer_current_char;
int lexer_current_pos;
// Local pos in current line
int lexer_local_pos;
int lexer_previous_pos;

bool lexer_is_finished;



extern t_buffer* lexer_source;


t_token lexer_array_list();

void lexer_initialise(t_buffer* buf);

void lexer_advance();

t_token lexer_get_next_token();

t_token lexer_number(bool* ok);

#endif
