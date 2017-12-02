/* This is free and unencumbered software released into the public domain. */

#ifndef OPUSXX_H
#define OPUSXX_H

/**
 * <opus++.h>
 */

#ifndef __cplusplus
#error "<opus++.h> requires a C++ compiler"
#endif

#if __cplusplus < 201103L
#error "<opus++.h> requires a C++11 compiler (CXXFLAGS='-std=c++11')"
#endif

////////////////////////////////////////////////////////////////////////////////

#include <opus.h> /* for libopus */

////////////////////////////////////////////////////////////////////////////////

namespace opus {
  class encoder;
  class decoder;
}

////////////////////////////////////////////////////////////////////////////////

#endif /* OPUSXX_H */
