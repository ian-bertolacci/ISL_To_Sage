#ifndef UTIL_HPP
#define UTIL_HPP

#include <sstream>
#include <stdexcept>
#include <string>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

#endif
