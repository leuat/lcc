#include "parser.h"
#include "error.h"
#include "token.h"
#include "node.h"
#include "a_eval.h"

t_token parser_current_token;

node* node_root;

bool is_string;
bool is_array_list;

bool next_is_assign = false;


void deb(char* s) {
  printf("Line:%d\n",data_current_line+2);
  printf("%s%s\n",s,parser_current_token.str_value);

}


enum_token_type curt() {
  return parser_current_token.type;
}
// fwd
node* parse_expr();
node* parse_for_loop();

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


int parse_const_int() {
  char str[256];
  int pos=0;
  str[0]=0;
  is_string = false;
  is_array_list = false;
  while (lexer_current_char!=';' && lexer_current_char!='}') {
    if (lexer_current_char=='\"') {
      is_string = true;
      return -1;
    }
    if (lexer_current_char=='{') {
      is_array_list = true;
      return -1;
    }
    
    if (lexer_current_char!=' ' && lexer_current_char!=',') {
      str[pos] = lexer_current_char;      
//      lexer_advance();
      pos++;
      str[pos]=0;
    }
    lexer_advance();
    if (lexer_current_char==',') {
      break;
    }


  }
  while (curt()!=tt_semicolon && curt()!=tt_comma && curt()!=tt_rcbracket)
    gobble();

//  printf("%s\n",str);

  return evaluate_int(str);
}

 // Forward decls
node* parse_block();
t_token get_variable_name();
node* parser_function_parameter_declarations();


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


  if (curt()==tt_lbracket) {
    gobble();
    node* expr = parse_expr();
    parser_eat(tt_rbracket);
    n->center = expr;
  }
  if (curt()==tt_plus_plus || curt()==tt_minus_minus) {
      if (curt()==tt_plus_plus)
        n->token.ivalue = plusplus_post;
      else
      if (curt()==tt_minus_minus)
        n->token.ivalue = minusminus_post;

      gobble();
      
  }
//  printf("Creating variable:%s\n",n->token.str_value);
//  deb(n->token.str_value);
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

    if (curt() == tt_lparen) {
      gobble();
      node* n = parse_expr();
      parser_eat(tt_rparen);
      return n;      
    }
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
  node* n = parse_factor();
  
  while (curt() == tt_plus || 
        curt() == tt_minus ||
        curt() == tt_mul ||
        curt() == tt_div) {
          t_token t = parser_current_token;
          gobble();          
          node* bop = create_node(nt_binop, t);// QSharedPointer<NodeBinOP>(new NodeBinOP(node, t, Factor()));
          bop->left = n;
          bop->block = parse_factor();
          n = bop;
        }


return n;

}


node* parse_expr() {
  node* n = parse_term();
  while (curt()==tt_plus || curt()==tt_minus || curt()==tt_bitand || curt()==tt_bitor 
    || curt()==tt_gt || curt() == tt_lt || curt() == tt_equals || curt() == tt_not_equals
  ) {
    t_token t = parser_current_token;
    gobble();
    node_type nt = nt_binop;
    if (t.type == tt_gt || t.type==tt_lt) {
      if (next_is_assign)
        nt = nt_clause_expr;
      else
        nt = nt_clause;
    }

    node* bop = create_node(nt, t);
    bop->left = n;
    bop->block = parse_term();
    n = bop;



  }

  return n;
}

node* parse_assignstatement() {
  next_is_assign = true;
  node* left = parse_variable();

 if (curt()==tt_semicolon || curt()==tt_rparen) {
    node* assign = create_node(nt_assign, parser_current_token);
   return left;
 }



  if (curt() != tt_assign) {
    raise_error_p1("error assigning variable ",left->token.str_value);
  }
  node* assign = create_node(nt_assign, parser_current_token);
  parser_eat(tt_assign);
  node* right = parse_expr();
  assign->left = left;
  assign->block = right;

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

node* parse_function_parameters(node* func) {
  node* params = NULL;
  node* next = NULL;
  int cur = 0;
  int req = func->token.ivalue;
  while (curt()!=tt_rparen) {
    cur++;
    node* n = parse_expr();
    if (params==NULL) {
      params = n;
      next = params;
    }
    else {
      next->right = n;
      next = n;
    }
    if (curt()!=tt_rparen) 
      parser_eat(tt_comma);
  }  
  if (cur!=req)
    raise_error_p1("incorrect number of parameters when calling function: ",func->token.str_value);
//    printf("parse func params: %d\n",params->type);
  return params;
}


node* parse_call_function() {
  node* func = find_function(parser_current_token.str_value);
  t_token name = parser_current_token;
  if (func == NULL) 
    return NULL;

//  deb(func->token.str_value);

  gobble();

  parser_eat(tt_lparen);
  int count = 0;
  node* params = parse_function_parameters(func);

  parser_eat(tt_rparen);

  node* call = create_node(nt_func,name);
  call->left = params;
  return call;
}

bool prev_was_block = false;


node* parse_conditional() {
  next_is_assign = false;
  node* cond = create_node(nt_conditional,parser_current_token);
  parser_eat(tt_if);
  node* clause = parse_expr();
  cond->center = clause;
  cond->block = parse_block();
  if (curt()==tt_else) {
    gobble();
    cond->left = parse_block();
  }
  prev_was_block = true;  

  return cond;
}




node* parse_statement() {
//  printf("Parsing statement : %s\n","nada");
  prev_was_block = false;  
  node* n = NULL;
    if (curt() == tt_lcbracket) {
        n = parse_block();
    }
    else if (curt() == tt_id) {
//        bool isAssign;
        n = parse_call_function();

/*      if (node==nullptr)
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
    else if (curt() == tt_if) {
        return parse_conditional();
    }
    else if (curt() == tt_for) {
        return parse_for_loop();
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

node* parse_for_loop() {
  node* for_loop = create_node(nt_for,parser_current_token);
  parser_eat(tt_for);
  parser_eat(tt_lparen);
  for_loop->left = parse_statement();
  parser_eat(tt_semicolon);
  for_loop->center = parse_expr();
  parser_eat(tt_semicolon);
  for_loop->extra = parse_statement();
//  parser_eat(tt_semicolon);
  parser_eat(tt_rparen);
 
  for_loop->block = parse_block();
  
  return for_loop;
}




node* parse_statementlist() {
  node* results = create_node(nt_statement_list, parser_current_token);
  node* s = parse_statement();
  results->right = s;


  // attatch statements
  while (parser_current_token.type == tt_semicolon || prev_was_block) {
    if (!prev_was_block)
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
  push_symbol_table();

  node* params = NULL;

  parser_eat(tt_lparen);
  // Parameter declarations - to do
  int count = 0;
  if (curt()!=tt_rparen) // no parameters
    params = parser_function_parameter_declarations(&count); // Parse params  parser_eat(tt_rparen);

  parser_eat(tt_rparen);


  node* block = parse_block();
  
  node* proc = create_node(nt_func_decl, name);

  proc->token.ivalue = count;

  proc->block = block;
  proc->right = params;

//  printf("Parser define function %s\n",name.str_value);

  if (!find_function(name.str_value))
    define_function(proc);
  else
    raise_error_p1("function already defined: ",name.str_value);


  pop_symbol_table();

  return proc;

}
bool parse_array_or_pointer(t_token* name)  {
  // char* c
  // char[] c;
  if (curt()==tt_lbracket) {
    gobble();
    name->array_count=0;
    if (curt()==tt_rbracket) {
      gobble();
      return true;
    }
    name->array_count = parse_const_int();
    parser_eat(tt_rbracket);
    return true;
  }
  return false;
}

node* parse_variable_declaration(node* func_type, t_token name) {
  // TO DO
//  printf("OH NOES parse declaration not implemented yet");
 parse_array_or_pointer(&name); // * or [] 
 

  if (curt()==tt_assign) {

    name.ivalue = parse_const_int();
      if (is_string) {
        parser_eat(tt_assign);
//        printf("wooot %s\n",parser_current_token.large_string);
        name.large_string = parser_current_token.large_string;
        name.is_string = true;
        parser_eat(tt_string);
      }
      if (is_array_list) {
//        deb("HERE");

        parser_eat(tt_assign);
        bool done = false;
//        parser_eat(tt_lcbracket);

        t_token* cur = &name;
        
        while (curt()!=tt_rbracket && lexer_current_char!='}') {
          int val = parse_const_int();
          sprintf(temp_buffer, "%d", val);
          t_token* t = create_dynamic_token(tt_integer_const,temp_buffer,val);
          cur->next = t;
          cur = cur->next;
        }
        while (curt()!=tt_rcbracket)
          gobble();
        parser_eat(tt_rcbracket);
      }
  
  }




  node* decl = create_node(nt_var_decl,name);
  decl->left = func_type;

  if (symbol_find(name.str_value))
    raise_error_p1("symbol already defined: ",name.str_value);


  symbol_type* type = find_type(func_type->token.str_value,typespec_table);
  if (name.is_pointer)
   type = find_type("pointer",typespec_table);
//  printf("FOUND TYPE: %s\n",type->type.str_value);
  define_symbol(create_symbol(name,type));



  return decl;
}


node* parser_function_parameter_declarations(int* count) {
  node* n_type = parse_type_spec();
  t_token name = get_variable_name();

  gobble(); // Gobbles the NAME
  (*count)++;
  // can be int a,b,c;
  node* decl = parse_variable_declaration(n_type, name);
  while (curt()==tt_comma) {
    gobble(); // eat the comma
    node* n_type = parse_type_spec(); // type decl

    node* n = parse_variable_declaration(n_type, get_variable_name());
    gobble();
    n->right = decl;
    decl = n;
    (*count)++;
  }
  return decl;
}




t_token get_variable_name() {
   bool next_is_pointer = false;
  if (curt()==tt_asterisk) {
      next_is_pointer = true;
      gobble();
  }
 t_token name = parser_current_token;
 name.is_pointer = next_is_pointer;
 return name;
}


node* declaration_body() {

  node* n_type = parse_type_spec();
  t_token name = get_variable_name();

  gobble(); // Gobbles the NAME

  if (parser_current_token.type == tt_lparen)
    return parse_declare_function(n_type, name);

  // can be int a,b,c;
  node* decl = parse_variable_declaration(n_type, name);
  while (curt()==tt_comma) {
    gobble(); // eat the comma
    node* n = parse_variable_declaration(n_type, get_variable_name());
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
