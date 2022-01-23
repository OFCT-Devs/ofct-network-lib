#ifndef OFCT_NETWORK_debug_debug_mode_hpp
#define OFCT_NETWORK_debug_debug_mode_hpp

#ifndef NDEBUG
#include <cstdio>
#include <cerrno>
#endif

namespace OFCT {

constexpr bool debug_mode =
#ifndef NDEBUG
false;
#else
true;
#endif
}

#endif