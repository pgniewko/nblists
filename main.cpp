#include <stdlib.h>
#include <nbody.h>
#include "random.h"

int main(int argc, char** argv)
{

    srand (time(NULL));
    unsigned long init[4]={(unsigned long)rand(), (unsigned long)rand(), (unsigned long)rand(), (unsigned long)rand()}, length=4;
    init_by_array(init, length);
    
    return 0;
}
