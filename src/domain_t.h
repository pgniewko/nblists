#ifndef DOMAIN_T_H
#define DOMAIN_T_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include "mc.h"

struct domain_t
{
    domain_t()
    {
        myid = -1;
    }

    int myid;
    int neigh_num;
    domain_t* next = NULL;
};

#endif  /* DOMAIN_T_H */
