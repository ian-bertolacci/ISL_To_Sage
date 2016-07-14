#ifndef SAGETRANSFORMATIONWALKER_HPP
#define SAGETRANSFORMATIONWALKER_HPP

#include "all_isl.hpp"
#include "rose.h"
#include <list>
#include <map>
#include <stack>

class SageTransformationWalker{
  protected:
    int depth;
    bool verbose;
    std::stack<SgScopeStatement*> scope_stack;
    isl_ast_node* isl_root;

    std::vector<SgFunctionCallExp*> statement_macros;
    SgNode* sage_root;

  public:
    SageTransformationWalker( isl_ast_node* isl_root );
    SageTransformationWalker( isl_ast_node* isl_root, bool verbose );

    std::vector<SgFunctionCallExp*>* getStatementMacroNodes();
    SgNode* getSageRoot();

  protected:
    // Generic visit switcher methods
    SgNode* visit( isl_ast_node* node );
    SgNode* visit( isl_ast_expr* node );

    // Operation visit switch method
    SgNode* visit_expr_op(isl_ast_expr* node);

    // Operands visitor methods
    std::list<SgNode*> visit_expr_operands(isl_ast_expr* node );
    SgExpression* visit_op_operand( isl_ast_expr* node, int pos );
    SgExpression* visit_op_lhs( isl_ast_expr* node );
    SgExpression* visit_op_rhs( isl_ast_expr* node );
    SgExpression* visit_op_unary_operand( isl_ast_expr* node );
    SgExpression* visit_op_cond_condition_operand( isl_ast_expr* node );
    SgExpression* visit_op_cond_then_operand( isl_ast_expr* node );
    SgExpression* visit_op_cond_else_operand( isl_ast_expr* node );

    // Visit operation node methods
    SgExpression* visit_op_error(isl_ast_expr* node);
    SgExpression* visit_op_and(isl_ast_expr* node);
    SgExpression* visit_op_and_then(isl_ast_expr* node);
    SgExpression* visit_op_or(isl_ast_expr* node);
    SgExpression* visit_op_or_else(isl_ast_expr* node);
    SgExpression* visit_op_max(isl_ast_expr* node);
    SgExpression* visit_op_min(isl_ast_expr* node);
    SgExpression* visit_op_minus(isl_ast_expr* node);
    SgExpression* visit_op_add(isl_ast_expr* node);
    SgExpression* visit_op_sub(isl_ast_expr* node);
    SgExpression* visit_op_mul(isl_ast_expr* node);
    SgExpression* visit_op_div(isl_ast_expr* node);
    SgExpression* visit_op_fdiv_q(isl_ast_expr* node);
    SgExpression* visit_op_pdiv_q(isl_ast_expr* node);
    SgExpression* visit_op_pdiv_r(isl_ast_expr* node);
    SgExpression* visit_op_zdiv_r(isl_ast_expr* node);
    SgExpression* visit_op_cond(isl_ast_expr* node);
    SgExpression* visit_op_select(isl_ast_expr* node);
    SgExpression* visit_op_eq(isl_ast_expr* node);
    SgExpression* visit_op_le(isl_ast_expr* node);
    SgExpression* visit_op_lt(isl_ast_expr* node);
    SgExpression* visit_op_ge(isl_ast_expr* node);
    SgExpression* visit_op_gt(isl_ast_expr* node);
    SgExprStatement* visit_op_call(isl_ast_expr* node);
    SgExpression* visit_op_access(isl_ast_expr* node);
    SgExpression* visit_op_member(isl_ast_expr* node);
    SgExpression* visit_op_address_of(isl_ast_expr* node);

    SgExpression* visit_op_unknown(isl_ast_expr* node);

    // Visit literal expression methods
    SgVarRefExp* visit_expr_id(isl_ast_expr* node);
    SgIntVal* visit_expr_int(isl_ast_expr* node);

    SgNode* visit_expr_unknown(isl_ast_expr* node);
    SgNode* visit_expr_error(isl_ast_expr* node);

    // Visit statement node methods
    SgNode* visit_node_for(isl_ast_node* node);
    SgNode* visit_node_if(isl_ast_node* node);
    SgNode* visit_node_block(isl_ast_node* node);
    SgNode* visit_node_mark(isl_ast_node* node);

    SgNode* visit_node_user(isl_ast_node* node);

    SgNode* visit_node_unknown(isl_ast_node* node);
    SgNode* visit_node_error(isl_ast_node* node);

};

#endif
