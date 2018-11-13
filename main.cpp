#include <stdlib.h>
#include <domain_list_t.h>
#include <random>
#include <algorithm>

pairs_t naive_neighbors_list(int n)
{
    pairs_t pairs;

    for (int i = 0; i < n; i++)
    {
        int_vec row;
        pairs.push_back( row );
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if ( i != j )
            {
                pairs[i].push_back(j);
            }
        }
    }

    return pairs;
}

void test(dbl_vec x, dbl_vec y, dbl_vec z, pairs_t pairs1, pairs_t pairs2, int n, double sigma)
{
    pairs_t contacts1;
    pairs_t contacts2;

    for (int i = 0; i < n; i++)
    {
        int_vec row1;
        int_vec row2;
        contacts1.push_back( row1 );
        contacts2.push_back( row2 );
    }



    for (int i = 0; i < n; i++)
    {
        int_vec row = pairs1[i];
        std::sort ( row.begin(), row.end() );

        for (int j = 0; j < row.size(); j++)
        {
            int jx = row[j];
            double x1 = x[i];
            double y1 = y[i];
            double z1 = z[i];
            double x2 = x[jx];
            double y2 = y[jx];
            double z2 = z[jx];
            double dx = x1 - x2;
            double dy = y1 - y2;
            double dz = z1 - z2;
            double r2 = dx * dx + dy * dy + dz * dz;

            if (r2 <= sigma * sigma && i != jx)
            {
                contacts1[i].push_back(jx);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        int_vec row = pairs2[i];
        std::sort ( row.begin(), row.end() );

        for (int j = 0; j < row.size(); j++)
        {
            int jx = row[j];
            double x1 = x[i];
            double y1 = y[i];
            double z1 = z[i];
            double x2 = x[jx];
            double y2 = y[jx];
            double z2 = z[jx];
            double dx = x1 - x2;
            double dy = y1 - y2;
            double dz = z1 - z2;
            double r2 = dx * dx + dy * dy + dz * dz;

            if (r2 <= sigma * sigma && i != jx)
            {
                contacts2[i].push_back(jx);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        int_vec row1 = contacts1[i];
        int_vec row2 = contacts2[i];

        if ( row1.size() != row2.size() )
        {
            std::cout << "ERROR - different number of contacts for i=" << i << std::endl;
            return;
        }

        for (int j = 0; j < row1.size(); j++)
        {
            if (row1[j] != row2[j])
            {
                std::cout << "ERROR - different contacts for (" << i << "," << j << ")" << std::endl;
                return;
            }
        }
    }

    std::cout << "TEST - PASSED !" << std::endl;
}

int main(int argc, char** argv)
{

    // GENERATE RANDOM POINTS
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    int n = 10000;
    dbl_vec x;
    dbl_vec y;
    dbl_vec z;

    for (int i = 0; i < n; i++)
    {
        x.push_back(distribution(generator));
        y.push_back(distribution(generator));
        z.push_back(distribution(generator));
    }

    // CREATE LINKED-DOMAINS
    double EPS = 1e-6;
    double sigma = 0.25;
    int M = (1.0 - (-1.0) + EPS) / sigma;
    domain_list_t dl(M, false);
    dl.set_system_dims(-1, 1, 0);
    dl.set_system_dims(-1, 1, 1);
    dl.set_system_dims(-1, 1, 2);


    // GENERATE A NAIVE LIST OF POTENTIAL CONTACTS
    pairs_t pairs_naive = naive_neighbors_list(n);

    // GENERATE A LIST OF CONTACTS WITH NBLISTS
    pairs_t nbl = dl.get_nb_lists(x, y, z, n, sigma);

    // CHECK THE CORRECTNESS
    test(x, y, z, pairs_naive, nbl, n, sigma);


    // SCRAMBLING TEST GOES BELOW
    int p_idx, n_scrumbled = 50;

    for (int i = 0; i < n_scrumbled; i++)
    {
        p_idx = rand() % n;
        x[p_idx] = distribution(generator);
        y[p_idx] = distribution(generator);
        z[p_idx] = distribution(generator);
        dl.update_domain_for_node(x[p_idx], y[p_idx], z[p_idx], p_idx);
    }


    pairs_t pairs_naive_scrambled = naive_neighbors_list(n);
    pairs_t nbl_scrambled = dl.get_nb_lists(x, y, z, n, sigma);

    // CHECK THE CORRECTNESS
    test(x, y, z, pairs_naive_scrambled, nbl_scrambled, n, sigma);

    return 0;
}
