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
            this->neighbors[i] = new int[26];


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


pairs_t domain_list_t::get_nb_lists(double* x,double* y, double* z, int n, double sigma)
{

    if ( ! check_size(sigma) )
        std::cout << "SIGMA > DOMAIN SIZE! CALCULATION MAY BE ERRONEOUS" << std::endl;

    pairs_t pairs;
    for (int i = 0; i < n; i++)
    {
        std::vector< int > row;
        pairs.push_back( row );
    }
    // make sure that domains are initialized
    //
    int domain_idx;
    for (int i = 0; i < this->cfg.N; i++)
        this->HEAD[i] = -1; // If -1 then the domain is empty

    if (!this->LIST)
        this->LIST = new int[n];
 
    for (int i = 0; i < n; i++)
        this->LIST[i] = -1;
   
    if(!this->node_to_domain)
        this->node_to_domain = new int[n];
    

    double rx, ry, rz;
    for (int i = 0; i < n; i++)
    {
        rx = x[i];
        ry = y[i];
        rz = z[i];
        domain_idx = get_domain_index(rx, ry, rz);
        this->node_to_domain[i] = domain_idx; // In which domain a particle is sitting
        LIST[i] = this->HEAD[domain_idx];
        this->HEAD[domain_idx] = i;
    }

    int i, j, n_idomain;
    for (int idomain = 0; idomain < this->cfg.N; idomain++)
    {
        i = HEAD[idomain];
        while(i > -1) // Iterate over all particles in idomain
        {
            j = this->LIST[i];
            // Iterate over particles in the same domain
            while(j > -1)
            {
                pairs[i].push_back(j);
                pairs[j].push_back(i);
                j = this->LIST[j];
            }

            // Iterate over neighbor domains
            for (int nix = 0; nix < this->neighs_num[idomain]; nix++)
            {
                n_idomain = this->neighbors[idomain][nix];
                j = this->HEAD[n_idomain];
                while(j > -1)
                {
                    pairs[i].push_back(j);
                    //pairs[j].push_back(i);
                    j = this->LIST[j];
                }
            }
            i = this->LIST[i];
        }
    }
 
    return pairs;
}


std::vector< int > domain_list_t::get_nb_lists(int idx)
{
    std::vector< int > pairs;
    int j, idomain, n_idomain;
    
    idomain = this->node_to_domain[idx];

    j =  HEAD[idomain];
    // Iterate over particles in the same domain
    while(j > -1)
    {
        if (idx != j)
            pairs.push_back(j);
        
        j = this->LIST[j];
    }

    // Iterate over neighbor domains
    for (int nix = 0; nix < this->neighs_num[idomain]; nix++)
    {
        n_idomain = this->neighbors[idomain][nix];
        j = this->HEAD[n_idomain];
        while(j > -1)
        {
            pairs.push_back(j);
            j = this->LIST[j];
        }
    }
    return pairs; 
}

void domain_list_t::update_domain_for_node(double x, double y, double z, int idx)
{
    int j, old_domain_idx, new_domain_idx;
    old_domain_idx = this->node_to_domain[idx];

    new_domain_idx = get_domain_index(x, y, z);
    if (old_domain_idx == new_domain_idx)
       return;

    if ( this->HEAD[old_domain_idx] == idx )
    {
       this->HEAD[old_domain_idx] = this->LIST[idx];
    }
    else
    {
        j = this->HEAD[old_domain_idx];
        while( true ) // It should stop naturally
        {
            if (this->LIST[j] == idx)
            {
                this->LIST[j] = this->LIST[idx];
                break;
            }
            j = this->LIST[j];
        }
    }

    this->node_to_domain[idx] = new_domain_idx;
    this->LIST[idx] = this->HEAD[new_domain_idx];
    this->HEAD[new_domain_idx] = idx;
    
    return;
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

bool domain_list_t::check_size(double sigma)
{
    if ( (this->cfg.xmax - this->cfg.xmin)/this->cfg.M < sigma )
        return false;
    if ( (this->cfg.ymax - this->cfg.ymin)/this->cfg.M < sigma )
        return false;
    if ( (this->cfg.zmax - this->cfg.zmin)/this->cfg.M < sigma )
        return false;

    return true;
}
