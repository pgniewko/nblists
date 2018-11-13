#ifndef DOMAIN_LIST_T_H
#define DOMAIN_LIST_T_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>

typedef std::vector<int> int_vec;
typedef std::vector<double> dbl_vec;
typedef std::vector< int_vec > pairs_t;

struct config_t
{
    config_t() : N(0), M(0), DIM(0), PBC(true),
                 xmin(0), xmax(0), ymin(0), ymax(0),
                 zmin(0), zmax(0)
    {  
    }

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
    int_vec neighs_num;
    pairs_t neighbors;
    int_vec node_to_domain;

    int_vec HEAD;
    int_vec LIST;
 
    // CONSTRUCTORS
    domain_list_t() = delete;
    domain_list_t(int, bool);
   
    // PUBLIC METHODS
    void init_domains();
    int get_index(int, int, int);
    void set_system_dims(double, double, int);
    pairs_t get_nb_lists(dbl_vec, dbl_vec, dbl_vec, int, double);
    int_vec get_nb_lists(int);
    void update_domain_for_node(double, double, double, int);
    int get_domain_index(double, double, double);
    bool check_size(double);

    // DESTRUCTOR
    ~domain_list_t() {}
};

#endif  /* DOMAIN_LIST_T_H */
