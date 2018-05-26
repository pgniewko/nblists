#ifndef DOMAIN_LIST_T_H
#define DOMAIN_LIST_T_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include "nblists.h"

struct config_t
{
    config_t() : N(0),M(0),DIM(0),PBC(true),
                 xmin(0),xmax(0),ymin(0),ymax(0),
                 zmin(0),zmax(0){}

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
    int* neighs_num = nullptr;
    int** neighbors = nullptr;

    int* HEAD = nullptr;

    domain_list_t() = delete;
    domain_list_t(int,bool);
   
    void init_domains();
    int get_index(int,int,int);
    void set_system_dims(double,double,int);

    nblists_t* get_nb_lists(double*,double*,double*,int,double);
    int get_domain_index(double,double,double);

    void print()
    {
        for (int i = 0; i < this->cfg.N; i++)
        {
            std::cout << "#n (" << i << ") = " << this->neighs_num[i] << ": ";

            for (int j = 0; j < this->neighs_num[i]; j++)
            {
                std::cout << this->neighbors[i][j]  << " ";
            }
            std::cout << std::endl;
        }
    }

    ~domain_list_t()
    {
        if ( neighs_num ) delete[] neighs_num;
        neighs_num = nullptr;
       
        if ( neighbors )
        {
            for(int i = 0; i < this->cfg.N; i++)
                delete[] neighbors[i];
            
            delete[] neighbors;
        }
        neighbors = nullptr;
        
        if ( HEAD ) delete[] HEAD;
        HEAD = nullptr;
    }
};

#endif  /* DOMAIN_LIST_T_H */
