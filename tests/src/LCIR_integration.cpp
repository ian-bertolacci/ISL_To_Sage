#include <cassert>
#include <utility>
#include <array>
#include <vector>
#include <string>
#include <iostream>

#include "rose.h"

#include "all_isl.hpp"

#include "SageTransformationWalker.hpp"
#include "PrintNodeWalker.hpp"

#include "FusionTransformation.hpp"
#include "ShiftTransformation.hpp"
#include "TileTransformation.hpp"
#include "Transformation.hpp"

using namespace std;
using namespace SageBuilder;
using namespace SageInterface;
using namespace LoopChainIR;

int main( int argc, char** argv ){
  bool verbose = true;
  Schedule* schedule = NULL;
  {
    // Create loop chain
    LoopChain chain;

    //* Working Case
    {
      string lower[2] = { "1", "a" };
      string upper[2] = { "10", "b" };
      string symbols[2] = { "a", "b" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
    }
    // */

    /*
    Great breaking case.
    Produces errors:
    1. Segfault when generateDOT
    2. the following error when unparsing:
      LCIR_integration: ../../../rose_dev/src/backend/unparser/nameQualificationSupport.C:5081: virtual NameQualificationInheritedAttribute NameQualificationTraversal::evaluateInheritedAttribute(SgNode*, NameQualificationInheritedAttribute): Assertion `initializedName != __null' failed.
      Aborted
    */
    /*
    {
      string lower[2] = { "1", "a" };
      string upper[2] = { "10", "b" };
      string symbols[2] = { "a", "b" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
    }

    {
      string lower[2] = { "1", "2" };
      string upper[2] = { "10", "11" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
    }
    // */

    /*
    Great alternative breaking case.
    Produces code:
    for (int c1 = 1; c1 <= 10; c1 = c1 + 1) {
      for (int c2 = c2; c2 <= c1; c2 = c2 + 1) {
        statement_0(a,c2);
      }
    }
    for (int c1 = 1; c1 <= 10; c1 = c1 + 1) {
      for (int c2 = 2; c2 <= a; c2 = c2 + 1) {
        statement_1(c1,c2);
      }
    }
    */
    /*
    {
      string lower[2] = { "1", "a" };
      string upper[2] = { "10", "b" };
      string symbols[2] = { "a", "b" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
    }

    {
      string lower[2] = { "1", "2" };
      string upper[2] = { "10", "a" };
      string symbols[1] = { "a" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1) ) );
    }
    // */

    // Create an ordered list of Transformations
    vector<Transformation*> transformations;

        // Create schedule from loop cahin
    schedule = new Schedule( chain );
    // Apply transformations
    schedule->apply( transformations );

    // print domains and Transformations
    cout << *schedule << endl;

    // print codegen from ISL's perspective
    cout << "ISL's codegen:\n" << schedule->codegen() << endl;
  }

  if( verbose ){
    PrintNodeWalker walker;
    cout << "PrintNodeWalker:" << endl;
    cout << walker.visit( schedule->codegenToIslAst()->root ) << endl;
  }

  {
    cout << "SageTransformationWalker:" << endl;

    // Template file source
    //string template_code( "#include <iostream>\nusing namespace std;\nint main(){\n int A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z;\nA = (B = (C = (D = (E = (F = (G = (H = (I = (J = (K = (L = (M = (N = (O = (P = (Q = (R = (S = (T = (U = (V = (W = (X = (Y = (Z = (a = (b = (c = (d = (e = (f = (g = (h = (i = (j = (k = (l = (m = (n = (o = (p = (q = (r = (s = (t = (u = (v = (w = (x = (y = (z = 1234)))))))))))))))))))))))))))))))))))))))))))))))))));\n }");
    string template_code( "#include <iostream>\nusing namespace std;\nint main(){ }");
    std::string template_file_name( "__template_file__.cpp" );

    // Write out template file.
    ofstream template_file;
    template_file.open( template_file_name.c_str(), ios::trunc | ios::out );
    assert( template_file.is_open() );
    template_file << template_code << endl;
    template_file.close();

    // Create arguments to frontend to parse template file
    vector<string> project_argv;
    // Apparently it is necessary to have the executable name in the arguments.
    project_argv.push_back( string(argv[0]) );
    project_argv.push_back( template_file_name );

    if( verbose ) cout << "Calling Frontend" << endl;
    SgProject* project = frontend( project_argv );

    // Find the existing main() definition node in the tree.
    if( verbose ) cout << "Finding target function definition" << endl;
    SgFunctionDefinition* target_defn = findFunctionDeclaration( project, "main", NULL, true)->get_definition();

    if( verbose ) cout << "Synthesizing symbol definitions" << endl;
    LoopChain chain = schedule->getChain();
    set<string> symbols;
    for( LoopChain::size_type nest_idx = 0; nest_idx != chain.length(); ++nest_idx ){
      RectangularDomain& domain = chain.getNest( nest_idx ).getDomain();
      for( RectangularDomain::size_type symbol_idx = 0; symbol_idx != domain.symbolics(); ++symbol_idx ){
        symbols.insert( domain.getSymbol( symbol_idx ) );
      }
    }

    if( verbose ) cout << "Injecting symbol definitions" << endl;
    for( auto symbol = symbols.begin(); symbol != symbols.end(); ++symbol ){
      SgVariableDeclaration* var_decl = buildVariableDeclaration( *symbol, buildIntType(), NULL, target_defn );
      target_defn->append_statement( var_decl );
    }

    if( verbose ) cout << "Injecting walker injection site" << endl;
    SgBasicBlock* injection_site = buildBasicBlock();
    target_defn->append_statement( injection_site );

    // Run ISL -> Sage walker over ISL tree, rendering it into Sage,
    if( verbose ) cout << "Calling SageTransformationWalker" << endl;
    SageTransformationWalker walker(schedule->codegenToIslAst()->root, injection_site, verbose);

    if( verbose ) cout << "Unparsing" << endl;
    project->unparse();

    // Write AST to dot file
    if( verbose ) cout << "Writing to dot file" << endl;
    generateDOT( *project );

    // Print generated code
    cout << "Generated Code:" << endl;
    string line;
    ifstream rose_output( (string("rose_") + template_file_name).c_str() );
    if( rose_output.is_open() ){
      while( getline( rose_output, line ) != NULL ){
        cout << line << endl;
      }
      rose_output.close();
    }
  }
}
