#include <iostream>
#include <iterator>
#include "TypeWalker.hpp"

using namespace std;

TypeWalker::TypeWalker(){}

string TypeWalker::visit_op_error(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation error\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_and(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation and\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_and_then(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation and_then\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_or(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation or\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_or_else(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation or_else\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_max(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation max\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_min(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation min\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_minus(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation minus\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_add(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation add\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_sub(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation sub\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_mul(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation mul\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_div(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation div\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_fdiv_q(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation fdiv_q\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_pdiv_q(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation pdiv_q\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_pdiv_r(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation pdiv_r\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_zdiv_r(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation zdiv_r\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_cond(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation cond\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_select(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation select\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_eq(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation eq\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_le(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation le\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_lt(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation lt\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_ge(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation ge\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_gt(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation gt\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_call(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation call\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_access(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation access\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_member(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation member\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_address_of(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation address_of\n")+ string_stream.str();
  this->depth -= 1;
}

string TypeWalker::visit_op_unknown(isl_ast_expr* node){
  this->depth += 1;
  ostream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,"\n"));
  string result = string(this->depth*2, ' ') + string("Operation unknown\n")+ string_stream.str();
  this->depth -= 1;
}


string TypeWalker::visit_expr_error(isl_ast_expr* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  cout << tab << "Expression error" << endl;
  this->depth -= 1;
}

string TypeWalker::visit_expr_id(isl_ast_expr* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  isl_id* isl_ident = isl_ast_expr_get_id(node);
  string id( isl_id_get_name( isl_ident ) );
  cout << tab << "Expression id: " << id << endl;
  this->depth -= 1;
}

string TypeWalker::visit_expr_int(isl_ast_expr* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  isl_val* isl_value = isl_ast_expr_get_val( node );
  long value = isl_val_get_num_si( isl_value );
  long den = isl_val_get_den_si( isl_value );
  cout << tab << "Expression int: " << value << " / " << den << endl;
  this->depth -= 1;
}

string TypeWalker::visit_expr_unknown(isl_ast_expr* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  cout << tab << "Expression unknown" << endl;
  this->depth -= 1;
}


string TypeWalker::visit_node_error(isl_ast_node* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  cout << tab << "Node error" << endl;
  this->depth -= 1;
}

string TypeWalker::visit_node_for(isl_ast_node* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  cout << tab << "Node for" << endl;
  this->visit( isl_ast_node_for_get_iterator( node ) );
  this->visit( isl_ast_node_for_get_init( node ) );
  this->visit( isl_ast_node_for_get_cond( node) );
  this->visit( isl_ast_node_for_get_inc( node ) );
  this->visit( isl_ast_node_for_get_body( node ) );

  this->depth -= 1;
}

string TypeWalker::visit_node_if(isl_ast_node* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  cout << tab << "Node if" << endl;
  this->visit( isl_ast_node_if_get_cond(node) );
  this->visit( isl_ast_node_if_get_then(node) );
  if( isl_ast_node_if_has_else( node ) )
    this->visit( isl_ast_node_if_get_else(node) );

  this->depth -= 1;
}

string TypeWalker::visit_node_block(isl_ast_node* node){
  this->depth += 1;

  isl_ast_node_list* list = isl_ast_node_block_get_children(node);
  for( int i = 0; i < isl_ast_node_list_n_ast_node(list); i += 1 ){
    isl_ast_node* node = isl_ast_node_list_get_ast_node(list, i);
    this->visit( node );
  }

  this->depth -= 1;
}

string TypeWalker::visit_node_mark(isl_ast_node* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  cout << tab << "Node mark" << endl;
  this->depth -= 1;
}

string TypeWalker::visit_node_user(isl_ast_node* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  cout << tab << "Node user" << endl;
  this->visit( isl_ast_node_user_get_expr(node) );
  this->depth -= 1;
}

string TypeWalker::visit_node_unknown(isl_ast_node* node){
  this->depth += 1;
  string tab(this->depth*2, ' ');
  cout << tab << "Node unknown" << endl;
  this->depth -= 1;
}
