#ifndef DOMAIN_LIST_T_H
#define DOMAIN_LIST_T_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>

typedef std::vector<std::vector<int>> pairs_t;

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
    int* node_to_domain = nullptr;

    int* HEAD = nullptr;
    int* LIST = nullptr;
 
    // CONSTRUCTORS
    domain_list_t() = delete;
    domain_list_t(int,bool);
   
    // PUBLIC METHODS
    void init_domains();
    int get_index(int,int,int);
    void set_system_dims(double,double,int);
    pairs_t get_nb_lists(double*,double*,double*,int,double);
    std::vector<int> get_nb_lists(int);
    void update_domain_for_node(double, double, double, int);
    int get_domain_index(double,double,double);
    bool check_size(double);

    // DESTRUCTOR
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
        
        if ( this->HEAD ) delete[] this->HEAD;
        this->HEAD = nullptr;
        
        if ( this->LIST ) delete[] this->LIST;
        this->LIST = nullptr;

        if ( this->node_to_domain ) delete[] this->node_to_domain; 
        this->node_to_domain = nullptr;
    }
};

#endif  /* DOMAIN_LIST_T_H */
