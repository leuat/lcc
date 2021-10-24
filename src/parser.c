#include "parser.h"
#include "error.h"
t_token parser_current_token;

void parser_eat(enum_token_type t)
{
    if (parser_current_token.type == t) {
        parser_current_token = lexer_get_next_token();
//        if (m_pass==2)
  //          qDebug() << "Token : " <<m_currentToken.getType() <<m_currentToken.m_value << m_currentToken.m_intVal;
//        int cnt =0;
//        while (m_currentToken.m_type==TokenType::PREPROCESSOR && m_pass>PASS_PREPRE && m_pass!=PASS_OTHER  && cnt++<32 ) {
  //          HandlePreprocessorInParsing();

    }
    else {
      raise_error_p2("Expected token vs actual token:",string_token_type[t],string_token_type[parser_current_token.type]);
    }
}



void parse(t_buffer* buf) {
  lexer_initialise(buf);
  parser_current_token = lexer_get_next_token();
  while (!lexer_is_finished) {
    parser_eat(parser_current_token.type);
  }
}
