#ifndef DOMAIN_LIST_T_H
#define DOMAIN_LIST_T_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include "domain_t.h"
#include "nblist_t.h"

struct config_t
{
    int N;
    int M;
    int DIM;
    bool PBC;

    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
};

struct domain_list_t
{
    config_t cfg;
    bool initialized = false;
    domain_t* domains = nullptr;

    domain_list_t() = delete;
    domain_list_t(int,int,bool);
   
    void init_domains();

    ~domain_list_t()
    {
        if ( domains ) delete[] domains;
        domains = nullptr;
    }

};

#endif  /* DOMAIN_LIST_T_H */
