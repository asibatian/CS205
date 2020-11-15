#include <iostream>
#include <limits>
#include "functions.h"

//generate random elements
void gen_data(struct matrix* m)
{
    float float_max = std::numeric_limits<float>::max();
    for(int i = 0; i < (m->nRow); i++)
    {
        for(int j = 0; j < (m->nColumn); j++)
        {
            //get element from 0.0 to float_max
            m->data[i][j] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX/float_max)); 
        }
    }
}