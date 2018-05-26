#include "domain_list_t.h"

domain_list_t::domain_list_t(int m, int dim, bool pbc)
{
    if (dim > 3 || dim  < 2)
    { 
        std::cout << "DIM must be 2 or 3" << std::endl;
    }

    cfg.M = m;
    cfg.N = pow(m,dim);
    cfg.DIM = dim;
    cfg.PBC = pbc;
}

void domain_list_t::init_domains()
{
    this->domains = new domain_t[this->cfg.N];

    for (int i = 0; i < this->cfg.N)
    {
        if ( this->cfg.DIM == 2)
           this->domains[i]->n_ds = new domain_t[9-1]
        else
           this->domains[i]->n_ds = new domain_t[27-1]
    }

    for (int i=0; i < this->cfg.N; i++) {
        for (int j=0; j < this->cfg.N; j++) {
            for (int k=0; k < this->cfg.N; k++) {


            }
        }
    }


    this->initialized = true;


}

