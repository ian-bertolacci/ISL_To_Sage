#include "ISLWalker.hpp"
#include <cassert>

using namespace std;

template<typename T>
ISLWalker<T>::ISLWalker(): depth(-1){}

template<typename T>
T ISLWalker<T>::visit( isl_ast_expr* node ){
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
      this->visit_expr_unknown( node );
  }
}

template<typename T>
T ISLWalker<T>::visit( isl_ast_node* node ){
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
      this->visit_node_unknown( node );
  }
}

template<typename T>
T ISLWalker<T>::visit_expr_op(isl_ast_expr* node){
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

template<typename T>
list<T> ISLWalker<T>::visit_expr_operands(isl_ast_expr* node ){
  list<T> operands_list;

  for( int i = 0; i < isl_ast_expr_get_op_n_arg(node); i += 1 ){
    operands_list.push_back( this->visit_op_operand( node, i ) );
  }

  return operands_list;
}

template<typename T>
T ISLWalker<T>::visit_op_operand( isl_ast_expr* node, int pos ){
  assert( isl_ast_expr_get_op_n_arg(node) > pos );
  return this->visit( isl_ast_expr_get_op_arg( node, pos ) );
}

template<typename T>
T ISLWalker<T>::visit_op_lhs( isl_ast_expr* node ){
  return visit_op_operand(node, 0);
}

template<typename T>
T ISLWalker<T>::visit_op_rhs( isl_ast_expr* node ){
  return visit_op_operand(node, 1);
}

template<typename T>
T ISLWalker<T>::visit_op_unary_operand( isl_ast_expr* node ){
  return visit_op_operand(node, 0);
}
