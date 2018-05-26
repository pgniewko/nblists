#ifndef DOMAIN_T_H
#define DOMAIN_T_H

#include <iostream>
#include <cstdlib>
#include <math.h>

struct domain_t
{
    int myid;
    int neigh_num;
    domain_t* n_ds = nullptr;
    
    domain_t() : myid(-1), neigh_num(0) {}

    ~domain_t()
    {
        if (n_ds ) delete[] n_ds;
        n_ds = nullptr;
    }
};

#endif  /* DOMAIN_T_H */
