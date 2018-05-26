#include "domain_list_t.h"


domain_list_t::domain_list_t(int m, bool pbc)
{
    cfg.M = m;
    cfg.DIM = 3;
    cfg.PBC = pbc;

    cfg.N = cfg.M*cfg.M*cfg.M;

    init_domains();
}


void domain_list_t::init_domains()
{
    if ( ! this->initialized )
    {
        this->HEAD = new int[this->cfg.N];

        this->neighs_num = new int[this->cfg.N];
        this->neighbors = new int*[this->cfg.N];

        for (int i = 0; i < this->cfg.N; i++)
            this->neighs_num[i] = 0;

        for (int i = 0; i < this->cfg.N; i++)
        {
            this->neighbors[i] = new int[26];
        }


        int index;
        int n_ix;
        int current_n_ix;
        
        for (int i=0; i < this->cfg.M; i++) {
            for (int j=0; j < this->cfg.M; j++) {
                for (int k=0; k < this->cfg.M; k++) {
               
                index = get_index(i, j, k);
                
                for (int l = -1; l <= 1; l++)
                    for (int o = -1; o <= 1; o++)
                        for (int p = -1; p <= 1; p++)
                        {
                            n_ix = get_index(i + l, j + o, k + p);
                            if (n_ix != -1 && n_ix != index)
                            {
                                current_n_ix = this->neighs_num[index];   
                                this->neighbors[ index ][ current_n_ix ] = n_ix;
                                this->neighs_num[index] += 1;

                            }    
                        }
                
                }
            }
        }
    
        this->initialized = true;
    }
}


nblists_t* domain_list_t::get_nb_lists(double* x,double* y, double* z, int n, double sigma)
{
    int domain_idx;
    for (int i = 0; i < this->cfg.N; i++)
        this->HEAD[i] = -1;
    
    int* LIST = new int[n];
   
    double rx, ry, rz;
    for (int i = 0; i < n; i++)
    {
        rx = x[i];
        ry = y[i];
        rz = z[i];
        domain_idx = get_domain_index(rx, ry, rz);
        LIST[i] = this->HEAD[domain_idx];
        this->HEAD[domain_idx] = i;
    }

    for (int i = 0; i < this->cfg.N; i++)
        std::cout << "HEAD[" << i << "]" << " " << HEAD[i] << std::endl;
    
    for (int i = 0; i < n; i++)
        std::cout << "LIST[" << i << "]" << " " << LIST[i] << std::endl;

    delete[] LIST;
    return nullptr;
}

int domain_list_t::get_index(int i, int j, int k)
{
    bool pbc = this->cfg.PBC;
    int m = this->cfg.M;

    if (i < 0 && !pbc)
    {
        return -1;
    }
    else if (i  >= m  && !pbc)
    {
        return -1;
    }

    if (j < 0 && !pbc)
    {
        return -1;
    }
    else if (j  >= m  && !pbc)
    {
        return -1;
    }

    if (k < 0 && !pbc)
    {
        return -1;
    }
    else if (k  >= m  && !pbc)
    {
        return -1;
    }

    if (i < 0 && pbc)
    {
        i += m;
    }
    else if (i  >= m  && pbc)
    {
        i -= m;
    }

    if (j < 0 && pbc)
    {
        j += m;
    }
    else if (j  >= m  && pbc)
    {
        j -= m;
    }

    if (k < 0 && pbc)
    {
        k += m;
    }
    else if (k  >= m  && pbc)
    {
        k -= m;
    }

    return (i + j * m + k * m * m);
}

void domain_list_t::set_system_dims(double min_v, double max_v, int axis) 
{
    if (axis == 0)
    {
        this->cfg.xmin = min_v;
        this->cfg.xmax = max_v;
    }
    else if (axis == 1)
    {
        this->cfg.ymin = min_v;
        this->cfg.ymax = max_v;
    }
    else if (axis == 2)
    {
        this->cfg.zmin = min_v;
        this->cfg.zmax = max_v;
    }
}


int domain_list_t::get_domain_index(double rx, double ry, double rz)
{
    int xix, yix, zix;
    double dx = (this->cfg.xmax - this->cfg.xmin)/this->cfg.M;
    double dy = (this->cfg.ymax - this->cfg.ymin)/this->cfg.M;
    double dz = (this->cfg.zmax - this->cfg.zmin)/this->cfg.M;
    
    double delx = rx - this->cfg.xmin;
    double dely = ry - this->cfg.ymin;
    double delz = rz - this->cfg.zmin;
    xix = floor(delx / dx);
    yix = floor(dely / dy);
    zix = floor(delz / dz);
    return this->get_index(xix, yix, zix);

}
