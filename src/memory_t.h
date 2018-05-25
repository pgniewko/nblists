#ifndef MEMORY_T_H
#define MEMORY_T_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include "mc.h"

struct memory_t
{
    memory_t()
    {
        x = std::nullptr_t;
        y = std::nullptr_t;
        z = std::nullptr_t;
    }

    int n;
    int dim;
    double* x;
    double* y;
    double* z;

};

#endif  /* MEMORY_T_H */
