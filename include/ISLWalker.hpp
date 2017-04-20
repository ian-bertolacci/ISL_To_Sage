#ifndef ISLWALKER_HPP
#define ISLWALKER_HPP

#include <list>
#include "all_isl.hpp"

template<typename T>
class ISLWalker {
  protected:
    int depth;

  public:

    ISLWalker();

    // Generic visit switcher methods
    T visit( isl_ast_expr* node );
    T visit( isl_ast_node* node );

    // Operation visit switch method
    T visit_expr_op(isl_ast_expr* node);

    // Operands visitor methods
    std::list<T> visit_expr_operands(isl_ast_expr* node );
    T visit_op_operand( isl_ast_expr* node, int pos );
    T visit_op_lhs( isl_ast_expr* node );
    T visit_op_rhs( isl_ast_expr* node );
    T visit_op_unary_operand( isl_ast_expr* node );

    // Visit operation node methods
    T visit_op_error(isl_ast_expr* node);
    T visit_op_and(isl_ast_expr* node);
    T visit_op_and_then(isl_ast_expr* node);
    T visit_op_or(isl_ast_expr* node);
    T visit_op_or_else(isl_ast_expr* node);
    T visit_op_max(isl_ast_expr* node);
    T visit_op_min(isl_ast_expr* node);
    T visit_op_minus(isl_ast_expr* node);
    T visit_op_add(isl_ast_expr* node);
    T visit_op_sub(isl_ast_expr* node);
    T visit_op_mul(isl_ast_expr* node);
    T visit_op_div(isl_ast_expr* node);
    T visit_op_fdiv_q(isl_ast_expr* node);
    T visit_op_pdiv_q(isl_ast_expr* node);
    T visit_op_pdiv_r(isl_ast_expr* node);
    T visit_op_zdiv_r(isl_ast_expr* node);
    T visit_op_cond(isl_ast_expr* node);
    T visit_op_select(isl_ast_expr* node);
    T visit_op_eq(isl_ast_expr* node);
    T visit_op_le(isl_ast_expr* node);
    T visit_op_lt(isl_ast_expr* node);
    T visit_op_ge(isl_ast_expr* node);
    T visit_op_gt(isl_ast_expr* node);
    T visit_op_call(isl_ast_expr* node);
    T visit_op_access(isl_ast_expr* node);
    T visit_op_member(isl_ast_expr* node);
    T visit_op_address_of(isl_ast_expr* node);

    T visit_op_unknown(isl_ast_expr* node);

    // Visit literal expression methods
    T visit_expr_id(isl_ast_expr* node);
    T visit_expr_int(isl_ast_expr* node);

    T visit_expr_unknown(isl_ast_expr* node);
    T visit_expr_error(isl_ast_expr* node);

    // Visit statement node methods
    T visit_node_for(isl_ast_node* node);
    T visit_node_if(isl_ast_node* node);
    T visit_node_block(isl_ast_node* node);
    T visit_node_mark(isl_ast_node* node);

    T visit_node_user(isl_ast_node* node);

    T visit_node_unknown(isl_ast_node* node);
    T visit_node_error(isl_ast_node* node);
};

#endif
