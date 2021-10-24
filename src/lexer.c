#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

t_buffer* lexer_source = 0;
bool next_is_reference = false;

void lexer_initialise(t_buffer* buf) {
	lexer_source = buf;
	lexer_current_char = buf->data[0];
	lexer_current_pos = 0;
	lexer_previous_pos = 0;
	lexer_local_pos = 0;
	data_current_line = 0;
	lexer_is_finished = false;

}


void lexer_advance() {
    lexer_previous_pos = lexer_current_pos;
    lexer_current_pos++;
    lexer_local_pos++;
    if (lexer_current_pos>lexer_source->length-1) {
        lexer_is_finished = true;
    }
    else {
        lexer_current_char = lexer_source->data[lexer_current_pos];
        if (lexer_current_char=='n') {
        	data_current_line++;
            lexer_local_pos = 0;

        }


    }
		char str[2];
		str[0] = lexer_current_char;
		str[1] = 0;
    printf("%s",str);

}

void lexer_skip_whitespace()
{
    while (!lexer_is_finished && (lexer_current_char==' '|| lexer_current_char=='\n' || lexer_current_char=='\t'))
        lexer_advance();
}


t_token get_id()
{
    char result[ MAX_ID_SIZE ];
    //qDebug() << "ID: " << m_currentChar;
		int pos = 0;
    while (!lexer_is_finished && is_alnum(char_to_string(lexer_current_char))) {
        result[pos] = lexer_current_char;
				pos+=1;
				result[pos]=0;
        lexer_advance();
    }
    bool is_ref = next_is_reference;
    next_is_reference = false;
  //  return Syntax::s.GetID(result,isRef);
/*
		QString org = val;

	for (Token& t: reserved_words)
			if (val.toUpper()==t.m_value) {
					t.m_lineNumber = Pmm::Data::d.lineNumber;
					return t;
			}
*/
	//exit(1);
	t_token t = create_token(tt_id,result,0);
	t.is_reference = is_ref;
	return t;

}

t_token lexer_get_next_token() {
	while (!lexer_is_finished && !abort_all) {
		 if (lexer_current_char==' ' || lexer_current_char=='\n' || lexer_current_char=='\t') {
            lexer_skip_whitespace();
            continue;
        }
        // ignore tab
        if (lexer_current_char == '\t' ) {
            lexer_advance();
            continue;
        }
        if (lexer_current_char=='{') {
            lexer_advance();
            return create_token(tt_lcbracket,"{",0);
        }
        if (lexer_current_char=='}') {
            lexer_advance();
            return create_token(tt_rcbracket,"}",0);
        }
        if (lexer_current_char=='[') {
            lexer_advance();
            return create_token(tt_lbracket,"[",0);
        }
				if (lexer_current_char==']') {
            lexer_advance();
            return create_token(tt_rbracket,"]",0);
        }
				if (lexer_current_char=='(') {
            lexer_advance();
            return create_token(tt_lparen,"()",0);
        }
				if (lexer_current_char==')') {
            lexer_advance();
            return create_token(tt_rparen,")",0);
        }
				if (lexer_current_char=='&') {
					lexer_advance();
					next_is_reference = true;
					return get_id();
				}

				if (is_digit(char_to_string(lexer_current_char))) {
            char keep = lexer_current_char;
            int pos = lexer_current_pos;
            int ppos = lexer_previous_pos;
            bool ok=true;
            t_token number = lexer_number(&ok);

            if (ok)
                return number;
            else {
                lexer_current_char = keep;
                lexer_current_pos = pos;
                lexer_previous_pos = ppos;
            }
        }
				if (is_alpha(char_to_string(lexer_current_char)) || lexer_current_char=='$') {
            return get_id();
        }

				raise_error_p1("Error lexing: ",char_to_string(lexer_current_char));

	}

	return create_token(tt_eof,"eof",0);

}


t_token lexer_number(bool* isOk)
{
    char res[64];
    char org[64];
		int cur=0;
    while (!lexer_is_finished && is_digit_all(char_to_string(lexer_current_char))) {
        res[cur]=lexer_current_char;
				cur+=1;
				res[cur]=0;
        lexer_advance();
    }
		strcpy(org,res);
    bool ok = false;
    long val = 0;
    // Memory address
    bool isConstant = true;
    int base = 10;
/*
    if (res.contains("%")) {
        res.remove("%");
        base=2;
    }

    if (res.contains("$")) {
        res.remove("$");
        base=16;
    }
		*/
		char * pEnd;
		*isOk = true;
 	  val = strtol (res,&pEnd,10);
		if (pEnd==NULL)
			*isOk = false;
//    val = atoi(res);

//    qDebug() << val;
    return create_token(tt_integer_const, "",val);

}
