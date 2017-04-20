#include <iostream>
#include <sstream>
#include <iterator>
#include <cassert>

#include "util.hpp"
#include "PrintNodeWalker.hpp"

using namespace std;

PrintNodeWalker::PrintNodeWalker()
: depth(-1), isl_id_prefix( "isl_id_" )
{}

string PrintNodeWalker::getTab(){
  return string(this->depth*2, ' ');
}

string PrintNodeWalker::visit( isl_ast_expr* node ){
  switch( isl_ast_expr_get_type(node) ){
    case isl_ast_expr_error:
      return this->visit_expr_error( node );

    case isl_ast_expr_op:
      return this->visit_expr_op( node );

    case isl_ast_expr_id:
      return this->visit_expr_id( node );

    case isl_ast_expr_int:
      return this->visit_expr_int( node );

    default:
      return this->visit_expr_unknown( node );
  }
}

string PrintNodeWalker::visit( isl_ast_node* node ){
  switch( isl_ast_node_get_type(node) ){
    case isl_ast_node_error:
      return this->visit_node_error( node );

    case isl_ast_node_for:
      return this->visit_node_for( node );

    case isl_ast_node_if:
      return this->visit_node_if( node );

    case isl_ast_node_block:
      return this->visit_node_block( node );

    case isl_ast_node_mark:
      return this->visit_node_mark( node );

    case isl_ast_node_user:
      return this->visit_node_user( node );

    default:
      return this->visit_node_unknown( node );
  }
}

string PrintNodeWalker::visit_expr_op(isl_ast_expr* node){
  switch( isl_ast_expr_get_op_type(node) ){
    case isl_ast_op_error:
      return this->visit_op_error( node );

    case isl_ast_op_and:
      return this->visit_op_and( node );

    case isl_ast_op_and_then:
      return this->visit_op_and_then( node );

    case isl_ast_op_or:
      return this->visit_op_or( node );

    case isl_ast_op_or_else:
      return this->visit_op_or_else( node );

    case isl_ast_op_max:
      return this->visit_op_max( node );

    case isl_ast_op_min:
      return this->visit_op_min( node );

    case isl_ast_op_minus:
      return this->visit_op_minus( node );

    case isl_ast_op_add:
      return this->visit_op_add( node );

    case isl_ast_op_sub:
      return this->visit_op_sub( node );

    case isl_ast_op_mul:
      return this->visit_op_mul( node );

    case isl_ast_op_div:
      return this->visit_op_div( node );

    case isl_ast_op_fdiv_q:
      return this->visit_op_fdiv_q( node );

    case isl_ast_op_pdiv_q:
      return this->visit_op_pdiv_q( node );

    case isl_ast_op_pdiv_r:
      return this->visit_op_pdiv_r( node );

    case isl_ast_op_zdiv_r:
      return this->visit_op_zdiv_r( node );

    case isl_ast_op_cond:
      return this->visit_op_cond( node );

    case isl_ast_op_select:
      return this->visit_op_select( node );

    case isl_ast_op_eq:
      return this->visit_op_eq( node );

    case isl_ast_op_le:
      return this->visit_op_le( node );

    case isl_ast_op_lt:
      return this->visit_op_lt( node );

    case isl_ast_op_ge:
      return this->visit_op_ge( node );

    case isl_ast_op_gt:
      return this->visit_op_gt( node );

    case isl_ast_op_call:
      return this->visit_op_call( node );

    case isl_ast_op_access:
      return this->visit_op_access( node );

    case isl_ast_op_member:
      return this->visit_op_member( node );

    case isl_ast_op_address_of:
      return this->visit_op_address_of( node );

    default:
      return this->visit_op_unknown( node );
  }
}

list<string> PrintNodeWalker::visit_expr_operands(isl_ast_expr* node ){
  list<string> operands_list;

  for( int i = 0; i < isl_ast_expr_get_op_n_arg(node); i += 1 ){
    operands_list.push_back( this->visit_op_operand( node, i ) );
  }

  return operands_list;
}

string PrintNodeWalker::visit_op_operand( isl_ast_expr* node, int pos ){
  assert( isl_ast_expr_get_op_n_arg(node) > pos );
  return this->visit( isl_ast_expr_get_op_arg( node, pos ) );
}

string PrintNodeWalker::visit_op_lhs( isl_ast_expr* node ){
  return visit_op_operand(node, 0);
}

string PrintNodeWalker::visit_op_rhs( isl_ast_expr* node ){
  return visit_op_operand(node, 1);
}

string PrintNodeWalker::visit_op_unary_operand( isl_ast_expr* node ){
  return visit_op_operand(node, 0);
}

string PrintNodeWalker::visit_op_error(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation error\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_and(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation and\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_and_then(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation and_then\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_or(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation or\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_or_else(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation or_else\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_max(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation max\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_min(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation min\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_minus(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation minus\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_add(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation add\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_sub(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation sub\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_mul(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation mul\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_div(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation div\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_fdiv_q(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation fdiv_q\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_pdiv_q(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation pdiv_q\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_pdiv_r(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation pdiv_r\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_zdiv_r(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation zdiv_r\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_cond(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation cond\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_select(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation select\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_eq(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation eq\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_le(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation le\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_lt(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation lt\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_ge(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation ge\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_gt(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation gt\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_call(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation call\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_access(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation access\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_member(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation member\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_address_of(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation address_of\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_op_unknown(isl_ast_expr* node){
  this->depth += 1;
  ostringstream string_stream;
  list<string> children = this->visit_expr_operands(node);
  copy(children.begin(), children.end(), ostream_iterator<string>(string_stream,""));
  string result = this->getTab() + string("Operation unknown\n")+ string_stream.str();
  this->depth -= 1;
  return result;
}


string PrintNodeWalker::visit_expr_error(isl_ast_expr* node){
  this->depth += 1;
  string result = this->getTab() + string("Expression error\n");
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_expr_id(isl_ast_expr* node){
  this->depth += 1;
  isl_id* isl_ident = isl_ast_expr_get_id(node);
  string result = this->getTab() + string( "Expression id: " ) + this->isl_id_prefix + string( isl_id_get_name( isl_ident ) ) + string("\n");
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_expr_int(isl_ast_expr* node){
  this->depth += 1;
  isl_val* isl_value = isl_ast_expr_get_val( node );
  long value = isl_val_get_num_si( isl_value );
  long den = isl_val_get_den_si( isl_value );
  string result = this->getTab() + string( "Expression int: " ) + to_string( value ) + string( " / " ) + to_string( den ) + string("\n");
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_expr_unknown(isl_ast_expr* node){
  this->depth += 1;
  string result = this->getTab() + string( "Expression unknown\n" );
  this->depth -= 1;
  return result;
}


string PrintNodeWalker::visit_node_error(isl_ast_node* node){
  this->depth += 1;
  string result = this->getTab() + string( "Node error\n" );
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_node_for(isl_ast_node* node){
  this->depth += 1;

  string result = this->getTab() + string("Node for\n");
  result += this->visit( isl_ast_node_for_get_iterator( node ) );
  result += this->visit( isl_ast_node_for_get_init( node ) );
  result += this->visit( isl_ast_node_for_get_cond( node) );
  result += this->visit( isl_ast_node_for_get_inc( node ) );
  result += this->visit( isl_ast_node_for_get_body( node ) );

  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_node_if(isl_ast_node* node){
  this->depth += 1;
  string result = this->getTab() + string( "Node if\n" );
  result += this->visit( isl_ast_node_if_get_cond(node) );
  result += this->visit( isl_ast_node_if_get_then(node) );
  if( isl_ast_node_if_has_else( node ) )
    result += this->visit( isl_ast_node_if_get_else(node) );

  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_node_block(isl_ast_node* node){
  this->depth += 1;

  string result = this->getTab() + string("Node Block\n");

  isl_ast_node_list* list = isl_ast_node_block_get_children(node);
  for( int i = 0; i < isl_ast_node_list_n_ast_node(list); i += 1 ){
    isl_ast_node* node = isl_ast_node_list_get_ast_node(list, i);
    result += this->visit( node );
  }

  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_node_mark(isl_ast_node* node){
  this->depth += 1;
  string result = this->getTab() + string( "Node mark\n" );
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_node_user(isl_ast_node* node){
  this->depth += 1;
  string result = this->getTab() + string( "Node user\n" );
  result += this->visit( isl_ast_node_user_get_expr(node) );
  this->depth -= 1;
  return result;
}

string PrintNodeWalker::visit_node_unknown(isl_ast_node* node){
  this->depth += 1;
  string result = this->getTab() + string( "Node unknown\n" );
  this->depth -= 1;
  return result;
}
