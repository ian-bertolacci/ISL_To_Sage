#include "all_isl.hpp"
#include "ISLWalker.hpp"
#include "PrintNodeWalker.hpp"

int main( int argc, char** argv){
  // Produce ISL AST
  isl_ast_node* isl_ast;
  {
    isl_ctx* ctx;
    isl_union_set* domain;
    isl_union_map* schedule;
    isl_ast_build* build;

    isl_printer* p;

    ctx = isl_ctx_alloc();

    domain = isl_union_set_read_from_str(ctx,
        //"[T,N]->{ S[t,i] : 1<=t<=T and 1<=i<=N }"
        //"{ S[t,i] : 1<=t<=10 and 1<=i<=20 }"
        //"{S[t,i] : 1<=t<=10 and 1<=i<=20}"
        //"{ S[t] : 1<=t<=10 }"
        //"{ s1[i] : 0 < i <= 10; s2[j] : 10 <= j <= 20 }"
      );
    schedule = isl_union_map_read_from_str(ctx,
        //"{ S[t,i] -> [t,i+t] }"
        "{ S[t,i] -> S[t,i] }"
        //"{ S[t] -> S[t] }"
        //"{ s1[i] -> [0, i, 0]; s2[i] -> [0, -9 + i, 1] }"
      );
    schedule = isl_union_map_intersect_domain(schedule, domain);

    build = isl_ast_build_alloc(ctx);
    isl_ast = isl_ast_build_node_from_schedule_map(build, schedule);
    isl_ast_build_free(build);

    cout << "Their Printer:" << endl;
    p = isl_printer_to_file(ctx, stdout);
    p = isl_printer_set_output_format(p, ISL_FORMAT_C);
    p = isl_printer_print_ast_node(p, isl_ast);
    cout << endl;
    isl_printer_free(p);

    // isl_ast_node_free(tree);
    // isl_ctx_free(ctx);
  }

  {
    ISL_PrintWalker walker;
    cout << "Types:" << endl;
    walker.visit( isl_ast );
  }

  {
    ISL_CodePrinter walker;
    cout << "Our Printer:" << endl;
    walker.visit( isl_ast );
    cout << endl;
  }

  {
    cout << "Testing ISL->Sage" << endl;

    // "Sorry, not implemented" as of rose -v 0.9.7.39 (6.23.2016)
    setSourcePositionClassificationMode( e_sourcePositionCompilerGenerated );

    // Write out a template file.
    ofstream template_file;
    string template_code( "#include <iostream>\nint main(){ /* int A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z;\nA = (B = (C = (D = (E = (F = (G = (H = (I = (J = (K = (L = (M = (N = (O = (P = (Q = (R = (S = (T = (U = (V = (W = (X = (Y = (Z = (a = (b = (c = (d = (e = (f = (g = (h = (i = (j = (k = (l = (m = (n = (o = (p = (q = (r = (s = (t = (u = (v = (w = (x = (y = (z = 1234))))))))))))))))))))))))))))))))))))))))))))))))))); */ }");
    std::string template_file_name( "__template_file__.cpp" );

    template_file.open( template_file_name.c_str(), ios::trunc | ios::out );
    assert( template_file.is_open() );

    template_file << template_code << endl;

    template_file.close();

    vector<string> project_argv;
    // Apparently it is necessary to have the executable name in the arguments.
    project_argv.push_back( string(argv[0]) );
    project_argv.push_back( template_file_name );

    SgProject* project = frontend( project_argv );

    // Find the existing declaration node in the tree.
    // We need the global scope produced by the source file.
    // This is _not_ what is returned by SgProject.get_globalScopeAcrossFiles()
    SgFunctionDeclaration* target_decl = NULL;
    SgName target_func_name( "main" );

    auto decl_list = isSgSourceFile((*project)[0])->get_globalScope()->get_declarations();

    for( auto iter = decl_list.begin(); iter != decl_list.end(); ++iter ){
      SgNode* sg_node = *iter;
      SgFunctionDeclaration* decl_node = isSgFunctionDeclaration( sg_node );
      if( (decl_node != NULL) && (decl_node->get_name() == target_func_name) ){
        cout << "Found " << target_func_name.getString() << endl;
        target_decl = decl_node;
        break;
      }
    }

    // Run ISL -> Sage walker over ISL tree, rendering it into Sage,
    ISL_To_Sage_Walker walker;

    SgNode* walker_rendered_ast = walker.visit( isl_ast );
    // 'Cast' into SgStatement (there is not reason, in my mind why this should fail)
    SgStatement* body_stmt = isSgStatement( walker_rendered_ast );

    if( body_stmt == NULL ){
      cerr << "Could not convert ISL AST into SgStatement" << endl;
      abort();
    }

    target_decl->get_definition()->append_statement( body_stmt );

    cout << "Fixing Var References" << endl;
    fixVariableReferences( target_decl->get_definition() );

    // Write AST to dot file
    cout << "Writing to dot file" << endl;
    generateDOT( *project );
    cout << "Calling backend" << endl;
    project->skipfinalCompileStep(true);
    backend( project );
  }

  return 0;
}
