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

#include "LoopChainIR/FusionTransformation.hpp"
#include "LoopChainIR/ShiftTransformation.hpp"
#include "LoopChainIR/TileTransformation.hpp"
#include "LoopChainIR/Transformation.hpp"

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

    // Great breaking case
    //*
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
    {
      string lower[2] = { "a", "b" };
      string upper[2] = { "c", "d" };
      string symbols[4] = { "a", "b", "c", "d" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 4 ) ) );
    }

    // */

    /*
    // No symbols
    {
      string lower[2] = { "1", "2" };
      string upper[2] = { "10", "11" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
    }

    // One symbol
    {
      string lower[2] = { "a", "2" };
      string upper[2] = { "10", "11" };
      string symbols[1] = { "a" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
    }

    {
      string lower[2] = { "1", "a" };
      string upper[2] = { "10", "11" };
      string symbols[1] = { "a" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
    }

    {
      string lower[2] = { "1", "2" };
      string upper[2] = { "a", "11" };
      string symbols[1] = { "a" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
    }

    {
      string lower[2] = { "1", "2" };
      string upper[2] = { "10", "a" };
      string symbols[1] = { "a" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 1 ) ) );
    }

    // Two symbols
    {
      string lower[2] = { "a", "b" };
      string upper[2] = { "10", "11" };
      string symbols[2] = { "a", "b" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
    }

    {
      string lower[2] = { "a", "2" };
      string upper[2] = { "b", "11" };
      string symbols[2] = { "a", "b" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
    }

    {
      string lower[2] = { "1", "2" };
      string upper[2] = { "a", "b" };
      string symbols[2] = { "a", "b" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
    }

    {
      string lower[2] = { "a", "2" };
      string upper[2] = { "10", "b" };
      string symbols[2] = { "a", "b" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
    }

    {
      string lower[2] = { "1", "b" };
      string upper[2] = { "a", "11" };
      string symbols[2] = { "a", "b" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
    }


    // Three symbols
    {
      string lower[2] = { "a", "b" };
      string upper[2] = { "10", "c" };
      string symbols[3] = { "a", "b", "c" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 3 ) ) );
    }

    {
      string lower[2] = { "a", "b" };
      string upper[2] = { "c", "11" };
      string symbols[3] = { "a", "b", "c" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 3 ) ) );
    }

    {
      string lower[2] = { "a", "2" };
      string upper[2] = { "b", "c" };
      string symbols[3] = { "a", "b", "c" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 3 ) ) );
    }

    {
      string lower[2] = { "1", "a" };
      string upper[2] = { "b", "c" };
      string symbols[3] = { "a", "b", "c" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 3 ) ) );
    }
    // Four symbols
    {
      string lower[2] = { "a", "b" };
      string upper[2] = { "c", "d" };
      string symbols[4] = { "a", "b", "c", "d" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 4 ) ) );
    }

    // No symbols
    {
      string lower[2] = { "1", "2" };
      string upper[2] = { "10", "11" };
      chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
    }
    // */

    /*
    {
      string lower[2] = { "1", "2" };
      string upper[2] = { "10", "20" };
      //string symbols[2] = { "lb1", "ub1" };
      //chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
      chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
    }

    {
      string lower[2] = { "3", "4" };
      string upper[2] = { "11", "21" };
      //string symbols[2] = { "lb1", "ub1" };
      //chain.append( LoopNest( RectangularDomain( lower, upper, 2, symbols, 2 ) ) );
      chain.append( LoopNest( RectangularDomain( lower, upper, 2 ) ) );
    }
    // */
    // Create an ordered list of Transformations
    vector<Transformation*> schedulers;

    // Create an ordered list of loops to fuse
    // In this case we are fusing loops 0 and 1.
    // This will result in both loop bodies residing in a single loop, however
    // the body of loop 0 will come before the body of loop 1
    //vector<LoopChain::size_type> fuse_these;
    //fuse_these.push_back( (LoopChain::size_type) 0 );
    //fuse_these.push_back( (LoopChain::size_type) 1 );

    // Create a FusionTransformation object, passing it the list of loops to fuse
    //schedulers.push_back( new FusionTransformation( fuse_these ) );

    // Create schedule from loop cahin
    schedule = new Schedule( chain );
    // Apply transformation
    schedule->apply( schedulers );

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

    // Run ISL -> Sage walker over ISL tree, rendering it into Sage,
    if( verbose ) cout << "Calling SageTransformationWalker" << endl;
    SageTransformationWalker walker(schedule, verbose);
    SgStatement* body_stmt = isSgStatement( walker.getSageRoot() );

    if( body_stmt == NULL ){
      cerr << "Could not convert ISL AST into SgStatement" << endl;
      abort();
    }

    // Append rendered ast to the main() body
    if( verbose ) cout << "Inserting into main()" << endl;
    appendStatement( body_stmt, target_defn );
    //fixStatement ( target_defn );

    // Write AST to dot file
    /*
    if( verbose ) cout << "Writing to dot file" << endl;
    generateDOT( *project );
    */
    if( verbose ) cout << "Unparsing" << endl;
    project->unparse();

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
