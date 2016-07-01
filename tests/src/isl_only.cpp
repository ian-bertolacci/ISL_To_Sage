#include <cassert>
#include <utility>
#include <array>
#include <vector>
#include <string>
#include <iostream>

#include "all_isl.hpp"
#include "PrintNodeWalker.hpp"

using namespace std;

isl_union_set* domain_from_domains( isl_ctx* ctx, vector<string> domains ){
  assert( !domains.empty() );

  auto iter = domains.begin();
  isl_union_set* domain = isl_union_set_read_from_str(ctx, (*iter).c_str() );
  ++iter;

  for( ; iter != domains.end(); ++iter ){
    domain = isl_union_set_union( domain, isl_union_set_read_from_str(ctx, (*iter).c_str() ) );
  }

  return domain;
}

isl_union_map* map_from_maps( isl_ctx* ctx, vector<string> maps ){
  assert( !maps.empty() );

  auto iter = maps.begin();
  isl_union_map* map = isl_union_map_read_from_str(ctx, (*iter).c_str() );
  ++iter;

  for( ; iter != maps.end(); ++iter ){
    map = isl_union_map_apply_range( map, isl_union_map_read_from_str(ctx, (*iter).c_str() ) );
  }

  return map;
}

void example( vector<string> domains, vector<string> maps ){
  // Produce ISL AST
  isl_ast_node* isl_ast;
  {
    isl_ctx* ctx = isl_ctx_alloc();
    isl_union_set* domain = domain_from_domains(ctx, domains );
    isl_union_map* schedule = map_from_maps(ctx, maps );
    isl_ast_build* build;
    isl_printer* p;

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
    PrintNodeWalker walker;
    cout << "PrintNodeWalker:" << endl;
    cout << walker.visit( isl_ast ) << endl;
  }

}


int main( int argc, char** argv){
  vector< pair< vector<string>, vector<string> > > tests;

  {
    vector<string> domains = {
      string( "{S[i] : 1 <= i <= 10}")
    };

    vector<string> maps = {
      string( "{S[i] -> [0,i,0]}" )
    };

    tests.push_back( make_pair( domains, maps) );
  }

  {
    vector<string> domains = {
      string( "{S1[i] : 1 <= i <= 10}"),
      string( "{S2[j] : 1 <= j <= 10}")
    };

    vector<string> maps = {
      string( "{S1[i] -> [0,i,0] ; S2[j] -> [1,j,0]}" )
    };

    tests.push_back( make_pair( domains, maps) );
  }

  cout << "===============================================\n" << std::endl;
  for( auto iter = tests.begin(); iter != tests.end(); ++iter ){
    auto doms = (*iter).first;
    auto maps = (*iter).second;
    example( doms, maps );
    cout << "\n===============================================\n" << std::endl;
  }

  return 0;
}
