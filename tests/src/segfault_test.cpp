#include <cassert>
#include <utility>
#include <array>
#include <vector>
#include <stack>
#include <string>
#include <iostream>

#include "rose.h"

using namespace std;
using namespace SageBuilder;
using namespace SageInterface;

string AST_To_File( char* argv[], SgStatement* ( *producer )( ) ) {
  cout << "Writing template file" << endl;
  // Template file source
  string template_code( "#include <iostream>\nusing namespace std;\nint main(){ }" );
  std::string template_file_name( "__template_file__.cpp" );

  // Write out template file.
  ofstream template_file;
  template_file.open( template_file_name.c_str( ), ios::trunc | ios::out );
  assert( template_file.is_open( ) );
  template_file << template_code << endl;
  template_file.close( );

  // Create arguments to frontend to parse template file
  vector< string > project_argv;
  // Apparently it is necessary to have the executable name in the arguments.
  project_argv.push_back( string( argv[0] ) );
  project_argv.push_back( template_file_name );

  cout << "Calling Frontend" << endl;
  SgProject* project = frontend( project_argv );

  // Find the existing main() definition node in the tree.
  cout << "Finding target function definition" << endl;
  SgFunctionDefinition* target_defn = findFunctionDeclaration( project, "main", NULL, true )->get_definition( );

  // Build AST
  cout << "Calling producer" << endl;
  SgStatement* root = ( *producer )( );
  assert( root != NULL );
  // Append rendered ast to the main() body
  cout << "Inserting into main()" << endl;
  target_defn->append_statement( root );

  /* Write AST to dot file
  cout << "Writing to dot file" << endl;
  generateDOT( *project );
  */
  cout << "Unparsing" << endl;
  project->unparse( );

  return string( "rose_" ) + template_file_name;
}

/*
Constructs the AST:

{
  for (int c1 = 1; c1 <= b; c1 += 1)
    for (int c2 = a; c2 <= c; c2 += 1)
      statement_0(c1, c2);
  for (int c1 = 1; c1 <= 10; c1 += 1)
    for (int c2 = 2; c2 <= 11; c2 += 1)
      statement_1(c1, c2);
}

Few Notes:
This probably looks weird and excessively poor. Thats fine.
This is an approximately near identical constant example of how the SageTransformationWalker produces the above loop chain.
There is a lot of reuse. Thats fine.
There is a lot of reused variable names. Thats fine.

If you want a cleaner look, refer to the SageTransformationWalker implementation.
This is for demonstration, examination, and disection purposes.

This produces the behavior that is seen when SageTransformationWalker produces the above AST
Successful tests have been done with this code to produce working loop chains by chaning the bounds to be numerical contants.
*/
SgStatement* build_example( ) {
  int depth = -1;
  stack< SgScopeStatement* > scope_stack;
  SgGlobal* global = new SgGlobal( );
  scope_stack.push( global );

  SgBasicBlock* chain_block = buildBasicBlock( );
  scope_stack.push( chain_block );

  /*
  This block creates variable declarations for the symbolic constants of the loop bounds
  */
  {
    SgVariableDeclaration* var_decl = NULL;

    var_decl = buildVariableDeclaration( SgName( string( "a" ) ), buildIntType( ), NULL, scope_stack.top( ) );
    chain_block->append_statement( var_decl );
    var_decl->set_parent( chain_block );

    var_decl = buildVariableDeclaration( SgName( string( "b" ) ), buildIntType( ), NULL, scope_stack.top( ) );
    chain_block->append_statement( var_decl );
    var_decl->set_parent( chain_block );

    var_decl = buildVariableDeclaration( SgName( string( "c" ) ), buildIntType( ), NULL, scope_stack.top( ) );
    chain_block->append_statement( var_decl );
    var_decl->set_parent( chain_block );
  }

  /*
  This block builds the first loop nest:
  for (int c1 = 1; c1 <= b; c1 += 1)
    for (int c2 = a; c2 <= c; c2 += 1)
      statement_0(c1, c2);

  Starting with the outer loop:
  for (int c1 = 1; c1 <= b; c1 += 1)
  */
  {
    // Build inititialization statement
    SgStatement* initialization = NULL;
    SgName* outer_iterator = NULL;
    {
      // Create iterator symbol
      outer_iterator = new SgName( string( "c1" ) );

      // Create initialization expression
      SgExpression* init_exp = isSgExpression( buildIntVal( 1 ) );

      assert( init_exp != NULL );

      // Build variable declaration
      SgAssignInitializer* initalizer = buildAssignInitializer( init_exp, buildIntType( ) );
      SgVariableDeclaration* var_decl = buildVariableDeclaration( *outer_iterator, buildIntType( ), initalizer, scope_stack.top( ) );

      // Building the variable decl seems sufficient.
      initialization = var_decl;
    }

    // Create condition expression statment
    SgExprStatement* condition = NULL;
    {
      // c1
      SgExpression* lhs = isSgExpression( buildVarRefExp( *outer_iterator, scope_stack.top( ) ) );

      // b
      SgExpression* rhs = isSgExpression( buildVarRefExp( SgName( string( "b" ) ), scope_stack.top( ) ) );

      // c1 <= b
      SgLessOrEqualOp* exp = buildBinaryExpression< SgLessOrEqualOp >( lhs, rhs );

      // convert to SgExpression
      SgExpression* as_exp = isSgExpression( exp );

      assert( as_exp != NULL );

      condition = buildExprStatement( as_exp );
    }

    // Create increment expression statement
    SgExpression* increment = NULL;
    {
      SgVarRefExp* var_ref = buildVarRefExp( buildInitializedName( *outer_iterator, buildIntType( ) ), scope_stack.top( ) );

      SgExpression* increment_exp = isSgExpression( buildIntVal( 1 ) );
      assert( increment_exp != NULL );

      // Create increment expression: iterator = iterator + (increment_exp)
      increment = buildBinaryExpression< SgAssignOp >( var_ref, buildBinaryExpression< SgAddOp >( var_ref, increment_exp ) );
    }

    SgBasicBlock* body = buildBasicBlock( );

    // Construct for loop node
    SgForStatement* for_stmt = buildForStatement( initialization, condition, increment, body );
    {
      scope_stack.push( for_stmt );
      SgStatement* sg_stmt = NULL;

      /*
      This builds the inner loop:
      for (int c2 = a; c2 <= c; c2 += 1)
      */
      {
        // Build inititialization statement
        SgStatement* initialization = NULL;
        SgName* inner_iterator = NULL;
        {
          // Create iterator symbol
          inner_iterator = new SgName( string( "c2" ) );

          // Create initialization expression
          SgExpression* init_exp = isSgExpression( buildVarRefExp( SgName( string( "a" ) ), scope_stack.top( ) ) );

          assert( init_exp != NULL );

          // Build variable declaration
          SgAssignInitializer* initalizer = buildAssignInitializer( init_exp, buildIntType( ) );
          SgVariableDeclaration* var_decl = buildVariableDeclaration( *inner_iterator, buildIntType( ), initalizer, scope_stack.top( ) );

          // Building the variable decl seems sufficient.
          initialization = var_decl;
        }

        // Create condition expression statment
        SgExprStatement* condition = NULL;
        {
          SgNode* exp = NULL;
          {

            // c1
            SgExpression* lhs = isSgExpression( buildVarRefExp( *inner_iterator, scope_stack.top( ) ) );

            // b
            SgExpression* rhs = isSgExpression( buildVarRefExp( SgName( string( "c" ) ), scope_stack.top( ) ) );
            // Construct using templated buildBinaryExpression
            exp = buildBinaryExpression< SgLessOrEqualOp >( lhs, rhs );
          }

          SgExpression* as_exp = isSgExpression( exp );

          assert( as_exp != NULL );

          condition = buildExprStatement( as_exp );
        }

        // Create increment expression statement
        SgExpression* increment = NULL;
        {
          SgVarRefExp* var_ref = buildVarRefExp( buildInitializedName( *inner_iterator, buildIntType( ) ), scope_stack.top( ) );

          SgExpression* increment_exp = isSgExpression( buildIntVal( 1 ) );
          assert( increment_exp != NULL );

          // Create increment expression: iterator = iterator + (increment_exp)
          increment = buildBinaryExpression< SgAssignOp >( var_ref, buildBinaryExpression< SgAddOp >( var_ref, increment_exp ) );
        }

        SgBasicBlock* body = buildBasicBlock( );

        // Construct for loop node
        SgForStatement* for_stmt = buildForStatement( initialization, condition, increment, body );
        {
          scope_stack.push( for_stmt );
          SgStatement* sg_stmt = NULL;
          {

            // Create function name
            SgName func_name( string( "statement_0" ) );

            // Build parameters list
            vector< SgExpression* > parameter_expressions;
            {
              // c1
              SgExpression* as_exp = isSgExpression( buildVarRefExp( buildInitializedName( *outer_iterator, buildIntType( ) ), scope_stack.top( ) ) );

              parameter_expressions.push_back( as_exp );
            }

            {
              // c2
              SgExpression* as_exp = isSgExpression( buildVarRefExp( buildInitializedName( *inner_iterator, buildIntType( ) ), scope_stack.top( ) ) );

              parameter_expressions.push_back( as_exp );
            }

            SgExprListExp* parameters = buildExprListExp( parameter_expressions );

            // Build call
            sg_stmt = isSgStatement( buildFunctionCallStmt( func_name, buildVoidType( ), parameters, global ) );
          }
          scope_stack.pop( );

          assert( sg_stmt != NULL );

          if ( isSgBasicBlock( sg_stmt ) ) {
            body = isSgBasicBlock( sg_stmt );
          } else {
            body = isSgBasicBlock( for_stmt->get_loop_body( ) );

            assert( body != NULL );

            body->append_statement( sg_stmt );
            sg_stmt->set_parent( body );
          }
          body->set_parent( for_stmt );
          for_stmt->set_loop_body( body );
        }
        sg_stmt = for_stmt;
      }
      scope_stack.pop( );

      assert( sg_stmt != NULL );

      // Convert/Wrap inner statment (ie the inner loop statement)
      if ( isSgBasicBlock( sg_stmt ) ) {
        body = isSgBasicBlock( sg_stmt );
      } else {
        body = isSgBasicBlock( for_stmt->get_loop_body( ) );

        assert( body != NULL );

        body->append_statement( sg_stmt );
        sg_stmt->set_parent( body );
      }
      // Inject the inner loop into the body of the outer loop
      body->set_parent( for_stmt );
      for_stmt->set_loop_body( body );
    }

    // append second nest to end of chain block statement
    chain_block->append_statement( for_stmt );
  }

  /*
  This block builds the Second loop nest:
  for (int c1 = 1; c1 <= 10; c1 += 1)
    for (int c2 = 2; c2 <= 11; c2 += 1)
      statement_0(c1, c2);

  Starting with the outer loop:
  for (int c1 = 1; c1 <= b; c1 += 1)
  */
  {
    // Build inititialization statement
    SgStatement* initialization = NULL;
    SgName* outer_iterator = NULL;
    {
      // Create iterator symbol
      outer_iterator = new SgName( string( "c1" ) );

      // Create initialization expression
      SgExpression* init_exp = isSgExpression( buildIntVal( 1 ) );

      assert( init_exp != NULL );

      // Build variable declaration
      SgAssignInitializer* initalizer = buildAssignInitializer( init_exp, buildIntType( ) );
      SgVariableDeclaration* var_decl = buildVariableDeclaration( *outer_iterator, buildIntType( ), initalizer, scope_stack.top( ) );

      // Building the variable decl seems sufficient.
      initialization = var_decl;
    }

    // Create condition expression statment
    SgExprStatement* condition = NULL;
    {
      SgNode* exp = NULL;
      {

        // c1
        SgExpression* lhs = isSgExpression( buildVarRefExp( *outer_iterator, scope_stack.top( ) ) );

        // 10
        SgExpression* rhs = isSgExpression( buildIntVal( 10 ) );
        // Construct using templated buildBinaryExpression
        exp = buildBinaryExpression< SgLessOrEqualOp >( lhs, rhs );
      }

      SgExpression* as_exp = isSgExpression( exp );

      assert( as_exp != NULL );

      condition = buildExprStatement( as_exp );
    }

    // Create increment expression statement
    SgExpression* increment = NULL;
    {
      SgVarRefExp* var_ref = buildVarRefExp( buildInitializedName( *outer_iterator, buildIntType( ) ), scope_stack.top( ) );

      SgExpression* increment_exp = isSgExpression( buildIntVal( 1 ) );
      assert( increment_exp != NULL );

      // Create increment expression: iterator = iterator + (increment_exp)
      increment = buildBinaryExpression< SgAssignOp >( var_ref, buildBinaryExpression< SgAddOp >( var_ref, increment_exp ) );
    }

    SgBasicBlock* body = buildBasicBlock( );

    // Construct for loop node
    SgForStatement* for_stmt = buildForStatement( initialization, condition, increment, body );
    {
      scope_stack.push( for_stmt );
      SgStatement* sg_stmt = NULL;
      /*
      This builds the inner loop:
      for (int c2 = 2; c2 <= 11; c2 += 1)
      */
      {
        // Build inititialization statement
        SgStatement* initialization = NULL;
        SgName* inner_iterator = NULL;
        {
          // Create iterator symbol
          inner_iterator = new SgName( string( "c2" ) );

          // Create initialization expression
          SgExpression* init_exp = isSgExpression( buildIntVal( 2 ) );

          assert( init_exp != NULL );

          // Build variable declaration
          SgAssignInitializer* initalizer = buildAssignInitializer( init_exp, buildIntType( ) );
          SgVariableDeclaration* var_decl = buildVariableDeclaration( *inner_iterator, buildIntType( ), initalizer, scope_stack.top( ) );

          // Building the variable decl seems sufficient.
          initialization = var_decl;
        }

        // Create condition expression statment
        SgExprStatement* condition = NULL;
        {
          SgNode* exp = NULL;
          {

            // c2
            SgExpression* lhs = isSgExpression( buildVarRefExp( *inner_iterator, scope_stack.top( ) ) );

            // 11
            SgExpression* rhs = isSgExpression( buildIntVal( 11 ) );
            // Construct using templated buildBinaryExpression
            exp = buildBinaryExpression< SgLessOrEqualOp >( lhs, rhs );
          }

          SgExpression* as_exp = isSgExpression( exp );

          assert( as_exp != NULL );

          condition = buildExprStatement( as_exp );
        }

        // Create increment expression statement
        SgExpression* increment = NULL;
        {
          SgVarRefExp* var_ref = buildVarRefExp( buildInitializedName( *inner_iterator, buildIntType( ) ), scope_stack.top( ) );

          SgExpression* increment_exp = isSgExpression( buildIntVal( 1 ) );
          assert( increment_exp != NULL );

          // Create increment expression: iterator = iterator + (increment_exp)
          increment = buildBinaryExpression< SgAssignOp >( var_ref, buildBinaryExpression< SgAddOp >( var_ref, increment_exp ) );
        }

        SgBasicBlock* body = buildBasicBlock( );

        // Construct for loop node
        SgForStatement* for_stmt = buildForStatement( initialization, condition, increment, body );
        {
          scope_stack.push( for_stmt );
          SgStatement* sg_stmt = NULL;
          {

            // Create function name
            SgName func_name( string( "statement_1" ) );

            // Build parameters list
            vector< SgExpression* > parameter_expressions;
            {
              // c1
              SgExpression* as_exp = isSgExpression( buildVarRefExp( buildInitializedName( *outer_iterator, buildIntType( ) ), scope_stack.top( ) ) );

              parameter_expressions.push_back( as_exp );
            }

            {
              // c2
              SgExpression* as_exp = isSgExpression( buildVarRefExp( buildInitializedName( *inner_iterator, buildIntType( ) ), scope_stack.top( ) ) );

              parameter_expressions.push_back( as_exp );
            }

            SgExprListExp* parameters = buildExprListExp( parameter_expressions );

            // Build call
            sg_stmt = isSgStatement( buildFunctionCallStmt( func_name, buildVoidType( ), parameters, global ) );
          }
          scope_stack.pop( );

          assert( sg_stmt != NULL );

          if ( isSgBasicBlock( sg_stmt ) ) {
            body = isSgBasicBlock( sg_stmt );
          } else {
            body = isSgBasicBlock( for_stmt->get_loop_body( ) );

            assert( body != NULL );

            body->append_statement( sg_stmt );
            sg_stmt->set_parent( body );
          }
          body->set_parent( for_stmt );
          for_stmt->set_loop_body( body );
        }
        sg_stmt = for_stmt;
      }
      scope_stack.pop( );

      assert( sg_stmt != NULL );

      // Convert/Wrap inner statment (ie the inner loop statement)
      if ( isSgBasicBlock( sg_stmt ) ) {
        body = isSgBasicBlock( sg_stmt );
      } else {
        body = isSgBasicBlock( for_stmt->get_loop_body( ) );

        assert( body != NULL );

        body->append_statement( sg_stmt );
        sg_stmt->set_parent( body );
      }
      // Inject the inner loop into the body of the outer loop
      body->set_parent( for_stmt );
      for_stmt->set_loop_body( body );
    }

    // append second nest to end of chain block statement
    chain_block->append_statement( for_stmt );
  }

  // pop chain_block
  scope_stack.pop( );
  // pop global
  scope_stack.pop( );

  return chain_block;
}

int main( int argc, char* argv[] ) {
  string filenname = AST_To_File( argv, build_example );

  // Print generated code
  cout << "Generated Code:" << endl;
  string line;
  ifstream rose_output( filenname.c_str( ) );
  if ( rose_output.is_open( ) ) {
    while ( getline( rose_output, line ) != NULL ) {
      cout << line << endl;
    }
    rose_output.close( );
  }

  return 0;
}
