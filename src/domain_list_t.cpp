#include "domain_list_t.h"


domain_list_t::domain_list_t(int m, bool pbc)
{
    cfg.M = m;
    cfg.DIM = 3;
    cfg.PBC = pbc;
    cfg.N = cfg.M * cfg.M * cfg.M;
    init_domains();
}


void domain_list_t::init_domains()
{
    if (!initialized)
    {
        for (int i = 0; i < cfg.N; i++)
        {
            HEAD.push_back(-1);
            int_vec new_row;
            neighbors.push_back(new_row);
            neighs_num.push_back(0);
        }

        int index;
        int n_ix;

        for (int i = 0; i < cfg.M; i++)
        {
            for (int j = 0; j < cfg.M; j++)
            {
                for (int k = 0; k < cfg.M; k++)
                {

                    index = get_index(i, j, k);

                    for (int l = -1; l <= 1; l++)
                        for (int o = -1; o <= 1; o++)
                            for (int p = -1; p <= 1; p++)
                            {
                                n_ix = get_index(i + l, j + o, k + p);

                                if (n_ix != -1 && n_ix != index)
                                {
                                    neighbors[index].push_back(n_ix);
                                    neighs_num[index] += 1;
                                }
                            }
                }
            }
        }

        initialized = true;
    }
}


pairs_t domain_list_t::get_nb_lists(const dbl_vec& xyz, int n, double sigma)
{

    if (!initialized)
    {
        std::cout << "DOMAINS STRUCTURE IS NOT INITIALIZED YET!" << std::endl;
    }

    if ( ! check_size(sigma) )
    {
        std::cout << "SIGMA > DOMAIN SIZE! CALCULATION MAY BE ERRONEOUS" << std::endl;
    }

    pairs_t pairs;

    for (int i = 0; i < n; i++)
    {
        int_vec row;
        pairs.push_back( row );
    }

    // make sure that domains are initialized
    //
    for (int i = 0; i < cfg.N; i++)
    {
        HEAD[i] = -1;    // If -1 then the domain is empty
    }


    if (LIST.size() == 0)
    {
        for (int i = 0; i < n; i++)
        {
            LIST.push_back(-1);
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            LIST[i] = -1;
        }
    }


    int domain_idx;

    if (node_to_domain.size() == 0)
    {
        for (int i = 0; i < n; i++)
        {
            node_to_domain.push_back(0);
        }
    }


    double rx, ry, rz;

    for (int i = 0; i < n; i++)
    {
        rx = xyz[3*i+0];
        ry = xyz[3*i+1];
        rz = xyz[3*i+2];
        domain_idx = get_domain_index(rx, ry, rz);
        node_to_domain[i] = domain_idx; // In which domain a particle is sitting
        LIST[i] = HEAD[domain_idx];
        HEAD[domain_idx] = i;
    }

    int i, j, n_idomain;

    for (int idomain = 0; idomain < cfg.N; idomain++)
    {
        i = HEAD[idomain];

        while (i > -1) // Iterate over all particles in idomain
        {
            j = LIST[i];

            // Iterate over particles in the same domain
            while (j > -1)
            {
                pairs[i].push_back(j);
                pairs[j].push_back(i);
                j = LIST[j];
            }

            // Iterate over neighbor domains
            for (int nix = 0; nix < neighs_num[idomain]; nix++)
            {
                n_idomain = neighbors[idomain][nix];
                j = HEAD[n_idomain];

                while (j > -1)
                {
                    pairs[i].push_back(j);
                    //pairs[j].push_back(i);
                    j = LIST[j];
                }
            }

            i = LIST[i];
        }
    }

    return pairs;
}


int_vec domain_list_t::get_nb_lists(int idx)
{
    int_vec pairs;
    int j, idomain, n_idomain;

    idomain = node_to_domain[idx];

    j =  HEAD[idomain];

    // Iterate over particles in the same domain
    while (j > -1)
    {
        if (idx != j)
        {
            pairs.push_back(j);
        }

        j = LIST[j];
    }

    // Iterate over neighbor domains
    for (int nix = 0; nix < neighs_num[idomain]; nix++)
    {
        n_idomain = neighbors[idomain][nix];
        j = HEAD[n_idomain];

        while (j > -1)
        {
            pairs.push_back(j);
            j = LIST[j];
        }
    }

    return pairs;
}

void domain_list_t::update_domain_for_node(double x, double y, double z, int idx)
{
    int j, old_domain_idx, new_domain_idx;
    old_domain_idx = node_to_domain[idx];

    new_domain_idx = get_domain_index(x, y, z);

    if (old_domain_idx == new_domain_idx)
    {
        return;
    }

    if ( HEAD[old_domain_idx] == idx )
    {
        HEAD[old_domain_idx] = LIST[idx];
    }
    else
    {
        j = HEAD[old_domain_idx];

        while ( true ) // It should stop naturally
        {
            if (LIST[j] == idx)
            {
                LIST[j] = LIST[idx];
                break;
            }

            j = LIST[j];
        }
    }

    node_to_domain[idx] = new_domain_idx;
    LIST[idx] = HEAD[new_domain_idx];
    HEAD[new_domain_idx] = idx;

    return;
}

int domain_list_t::get_index(int i, int j, int k)
{
    bool pbc = cfg.PBC;
    int m = cfg.M;

    if (i < 0 && !pbc)
    {
        return -1;
    }
    else if (i  >= m && !pbc)
    {
        return -1;
    }

    if (j < 0 && !pbc)
    {
        return -1;
    }
    else if (j  >= m && !pbc)
    {
        return -1;
    }

    if (k < 0 && !pbc)
    {
        return -1;
    }
    else if (k  >= m && !pbc)
    {
        return -1;
    }

    if (i < 0 && pbc)
    {
        i += m;
    }
    else if (i  >= m && pbc)
    {
        i -= m;
    }

    if (j < 0 && pbc)
    {
        j += m;
    }
    else if (j  >= m && pbc)
    {
        j -= m;
    }

    if (k < 0 && pbc)
    {
        k += m;
    }
    else if (k  >= m && pbc)
    {
        k -= m;
    }

    return (i + j * m + k * m * m);
}

void domain_list_t::set_system_dims(double min_v, double max_v, int axis)
{
    if (axis == 0)
    {
        cfg.xmin = min_v;
        cfg.xmax = max_v;
    }
    else if (axis == 1)
    {
        cfg.ymin = min_v;
        cfg.ymax = max_v;
    }
    else if (axis == 2)
    {
        cfg.zmin = min_v;
        cfg.zmax = max_v;
    }
}


int domain_list_t::get_domain_index(double rx, double ry, double rz)
{
    int xix, yix, zix;
    double dx = (cfg.xmax - cfg.xmin) / cfg.M;
    double dy = (cfg.ymax - cfg.ymin) / cfg.M;
    double dz = (cfg.zmax - cfg.zmin) / cfg.M;

    double delx = rx - cfg.xmin;
    double dely = ry - cfg.ymin;
    double delz = rz - cfg.zmin;
    xix = floor(delx / dx);
    yix = floor(dely / dy);
    zix = floor(delz / dz);
    return get_index(xix, yix, zix);
}

bool domain_list_t::check_size(double sigma)
{
    if ( (cfg.xmax - cfg.xmin) / cfg.M < sigma )
    {
        return false;
    }

    if ( (cfg.ymax - cfg.ymin) / cfg.M < sigma )
    {
        return false;
    }

    if ( (cfg.zmax - cfg.zmin) / cfg.M < sigma )
    {
        return false;
    }

    return true;
}
