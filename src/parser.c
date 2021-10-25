#include "parser.h"
#include "error.h"
#include "token.h"
#include "node.h"

t_token parser_current_token;

node* node_root;


void deb(char* s) {
  printf("Line:%d\n",data_current_line+2);
  printf("%s%s\n",s,parser_current_token.str_value);

}


enum_token_type curt() {
  return parser_current_token.type;
}

void parser_eat(enum_token_type t)
{
    if (parser_current_token.type == t) {
        parser_current_token = lexer_get_next_token();
//        printf("%s\n",parser_current_token.str_value);
//        if (m_pass==2)
  //          qDebug() << "Token : " <<m_currentToken.getType() <<m_currentToken.m_value << m_currentToken.m_intVal;
//        int cnt =0;
//        while (parser_current_token.type==TokenType::PREPROCESSOR && m_pass>PASS_PREPRE && m_pass!=PASS_OTHER  && cnt++<32 ) {
  //          HandlePreprocessorInParsing();

    }
    else {
      raise_error_p2("expected token vs actual token:",string_token_type[t],string_token_type[parser_current_token.type]);
    }
}

void gobble() {
  parser_eat(parser_current_token.type);
}

node* parse_block();


node* parse_type_spec() {
  t_token type = parser_current_token;
  if (!find_type(type.str_value,typespec_table))
    raise_error_p1("undefined type : ",type.str_value);

  gobble();
  return create_node(nt_type_spec, type);
}

node* parse_variable() {

//  deb("curr: ");
  //printf("here: %d\n",(parser_current_token.type==tt_id));
  node* n = create_node(nt_variable, parser_current_token);
  if (!symbol_find(parser_current_token.str_value))
    raise_error_p1("undefined variable : ",parser_current_token.str_value);

  gobble();
//  printf("Creating variable:%s\n",n->token.str_value);
  return n;
}



node* parse_factor()
{

/*    if (m_currentToken.m_type == TokenType::NOT || m_currentToken.m_type == TokenType::BITNOT) {
        Eat();

       auto t = m_currentToken;

        return NodeFactory::CreateBinop(t,TokenType::XOR,Factor(), NodeFactory::CreateNumber(t,255));
    }

*/


    t_token t = parser_current_token;

    if (t.type==tt_eof)
      raise_error("syntax error");


    if (t.type == tt_integer_const) {
      gobble();
//      printf("IS NUMBER %d",t.ivalue);
      return create_node(nt_number,t);
    }
/*
    if (t.m_type == TokenType::PLUS || t.m_type==TokenType::MINUS ){
        Eat(t.m_type);
        QSharedPointer<Node> expr = Factor();
        if (expr->isPureNumeric()) {
            bool isMinus = t.m_type==TokenType::MINUS;
            t.m_type = TokenType::INTEGER_CONST;
            t.m_value = "";
            t.m_intVal = expr->getValueAsInt(nullptr);
            if (isMinus) {
                if (t.m_intVal<256)
                    t.m_intVal = 256-t.m_intVal;
                else
                    t.m_intVal = 65536-t.m_intVal;
            }
            return QSharedPointer<NodeNumber>(new NodeNumber(t,t.m_intVal));


        }
        return QSharedPointer<NodeUnaryOp>(new NodeUnaryOp(t, expr));
    }
    */

/*
    if (t.m_type == TokenType::LPAREN) {
        Eat(TokenType::LPAREN);
        QSharedPointer<Node> node = Expr();
        Eat(TokenType::RPAREN);
        return node;

    }
    */
    if (t.type == tt_id) {
//        qDebug() << "FINDING PROCEDURE IN TERM: " << t.m_value;
        bool isAssign;
        node* n= NULL;
        //n  = FindProcedure(isAssign, nullptr);
        if (n!=NULL)
            return n;

/*        n = BuiltinFunction();
        if (node!=NULL)
            return n;*/

    }
    return parse_variable();
}



node* parse_term() {
  node* n = parse_factor();/*
while (m_currentToken.m_type == TokenType::Type::MUL || m_currentToken.m_type == TokenType::Type::DIV
|| m_currentToken.m_type == TokenType::Type::BITAND || m_currentToken.m_type == TokenType::Type::BITOR
 || m_currentToken.m_type == TokenType::Type::SHR || m_currentToken.m_type == TokenType::Type::SHL
       || m_currentToken.m_type == TokenType::Type::XOR

       ){
    Token t = m_currentToken;
    Eat(m_currentToken.m_type);

    node = QSharedPointer<NodeBinOP>(new NodeBinOP(node, t, Factor()));
}
*/
return n;

}


node* parse_expr() {
  node* n = parse_term();

/*

  while (m_currentToken.m_type == TokenType::Type::PLUS || m_currentToken.m_type == TokenType::Type::MINUS
          || m_currentToken.m_type == TokenType::Type::BITAND || m_currentToken.m_type == TokenType::Type::BITOR) {
      Token t = m_currentToken;

      Eat(m_currentToken.m_type);

      node = QSharedPointer<NodeBinOP>(new NodeBinOP(node, t, Term()));

  }
*/

  return n;
}

node* parse_assignstatement() {
  node* left = parse_variable();

  if (curt() != tt_assign) {
    raise_error_p1("error assigning variable ",left->token.str_value);
  }
  node* assign = create_node(nt_assign, parser_current_token);
  parser_eat(tt_assign);
  node* right = parse_expr();
  assign->left = left;
  assign->right = right;

//  printf("After assign statement: %s",parser_current_token.str_value);

  return assign;
}


node* inline_assembler() {
  parser_eat(tt_asm);
  parser_eat(tt_lparen);
  t_token cmd = parser_current_token;
  parser_eat(tt_string);
  parser_eat(tt_rparen);
//  deb(cmd.str_value);

  return create_node(nt_asm,cmd);
}

node* parse_call_function() {
  node* n = find_function(parser_current_token.str_value);
  t_token name = parser_current_token;
  if (n==NULL)
    return NULL;
  gobble();
  parser_eat(tt_lparen);
  parser_eat(tt_rparen);

  node* call = create_node(nt_func,name);

  return call;
}


node* parse_statement() {
//  printf("Parsing statement : %s\n","nada");
  node* n = NULL;
    if (curt() == tt_lcbracket) {
        n = parse_block();
    }
    else if (curt() == tt_id) {
//        bool isAssign;
        n = parse_call_function();

/*        node = FindProcedure(isAssign, nullptr);
        if (isAssign) {
            return Empty();
        }
        if (node==nullptr)
            node = BuiltinFunction();
*/
        if (n==NULL)
          n = parse_assignstatement();

//          deb("Cur");

    }
    else
    if (curt() == tt_asm) {
      return inline_assembler();
    }

    /*
    else if (parser_current_token.type == TokenType::ADDRESS) {
        if (node==nullptr)
            node = AssignStatement();

    }
    else if (parser_current_token.type == TokenType::IF) {
        Eat(TokenType::IF);
        node = Conditional();
    }
    else if (parser_current_token.type == TokenType::FORI) {
        node = ForLoop(true);
    }
    else if (parser_current_token.type == TokenType::FOR) {
        node = ForLoop(false);
    }
    else if (parser_current_token.type == TokenType::BREAK || parser_current_token.type == TokenType::CONTINUE || parser_current_token.type == TokenType::RETURN) {
        node = QSharedPointer<NodeControlStatement>(new NodeControlStatement(m_currentToken));
        Eat();
        if (parser_current_token.type==TokenType::LPAREN) {
            Eat(TokenType::LPAREN);
            Eat(TokenType::RPAREN);
        }
    }
    else if (parser_current_token.type == TokenType::WHILE) {
        Eat(TokenType::WHILE);
        node = Conditional(true);
    }
    else if (parser_current_token.type == TokenType::REPEAT) {
        node = RepeatUntil();
    }
    else if (parser_current_token.type == TokenType::ASM) {
        return InlineAssembler();

    } else if (parser_current_token.type == TokenType::CASE) {
        return Case();
    }

    else {
        //ErrorHandler::e.Error("Unknown method " + m_currentToken.getType());
        return Empty();
    }


*/
  if (parser_current_token.type == tt_rcbracket)
    return NULL;


  if (n==NULL)
    raise_error("Node is nullpointer. Should not happen. Contact leuat@irio.co.uk and slap him.");

  return n;


}




node* parse_statementlist() {
  node* results = create_node(nt_statement_list, parser_current_token);
  node* s = parse_statement();
  results->right = s;

  // attatch statements
  while (parser_current_token.type==tt_semicolon) {
      parser_eat(tt_semicolon);
      node* cur = s;
      s = parse_statement();
      cur->right = s;

  }
  if (parser_current_token.type==tt_id)
//        ErrorHandler::e.Error("Parser::Statementlist SYNTAX ERROR : Token should not be ID", m_currentToken.m_lineNumber);
    raise_error("Did you forget a semicolon?");


  return results;
}


node* parse_block() {
  node* block = create_node(nt_block,parser_current_token);
  parser_eat(tt_lcbracket);

  node* statement = parse_statementlist();

  block->right = statement->right;

  parser_eat(tt_rcbracket);

  return block;
}

node* parse_declare_function(node* func_type, t_token name) {
  parser_eat(tt_lparen);
  // Parameter declarations - to do
  parser_eat(tt_rparen);


  node* block = parse_block();

  node* proc = create_node(nt_func_decl, name);

  proc->block = block;

//  printf("Parser define function %s\n",name.str_value);

  if (!find_function(name.str_value))
    define_function(proc);
  else
    raise_error_p1("function already defined: ",name.str_value);

  return proc;

}

node* parse_variable_declaration(node* func_type, t_token name) {
  // TO DO
//  printf("OH NOES parse declaration not implemented yet");
  if (curt()==tt_assign)
    raise_error("initializing variables not implemented yet.");


  node* decl = create_node(nt_var_decl,name);
  decl->left = func_type;

  if (symbol_find(name.str_value))
    raise_error_p1("symbol already defined: ",name.str_value);


  symbol_type* type = find_type(func_type->token.str_value,typespec_table);
//  printf("FOUND TYPE: %s\n",type->type.str_value);
  define_symbol(create_symbol(name,type));



  return decl;
}


node* declaration_body() {

  node* n_type = parse_type_spec();
  t_token name = parser_current_token;

  gobble();
  if (parser_current_token.type == tt_lparen)
    return parse_declare_function(n_type, name);

  // can be int a,b,c;
  node* decl = parse_variable_declaration(n_type, name);
  while (curt()==tt_comma) {
    gobble(); // eat the comma
    node* n = parse_variable_declaration(n_type, parser_current_token);
    gobble();
    n->right = decl;
    decl = n;
  }
  parser_eat(tt_semicolon);

  return decl;
}

void init_symtab() {
  initialize_symboltable();
  functions = NULL;
}


void parse(t_buffer* buf) {
  lexer_initialise(buf);
  init_symtab();

  parser_current_token = lexer_get_next_token();

  node_root = create_node(nt_root, parser_current_token);
  node* nxt = NULL;
  while (!lexer_is_finished) {
//    parser_eat(parser_current_token.type);

    node* decl = declaration_body();
    if (node_root->center==NULL)
      node_root->center = decl;
    else
      nxt->center = decl;

    nxt = decl;

//    decl->right = node_root->right;
  //  node_root->right = decl;
  }
}
