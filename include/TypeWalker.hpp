#ifndef TYPEWALKER_HPP
#define TYPEWALKER_HPP

#include "ISLWalker.hpp"
#include <string>

class TypeWalker : public ISLWalker<std::string>{

  public:
    TypeWalker();

};

#endif
