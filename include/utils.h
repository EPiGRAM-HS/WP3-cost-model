#ifndef UTILS_H
#define UTILS_H

#include "typedefs.h"

namespace CostModel {
  unsigned int unorderedCantor(unsigned int A, unsigned int B) {
    // Cantor pairing of two positive integers
    // A and B are swapped if B > A so that f(A,B) == f(B,A)

    if ( B > A ) {
      unsigned int tmp = A;
      A = B;
      B = tmp;
    }

    return ((A+B) * (A+B+1)) / 2 + B;
  }
}

#endif
