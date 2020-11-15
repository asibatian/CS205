#include <iostream>
#include "functions.h"

//allocate data
void allocate_data(struct matrix* m)
{
    m->data = new float*[m->nRow]; //allocate rows
    for(int i = 0; i < m->nRow; i++)
    {
        m->data[i] = new float[m->nColumn]; //allocate columns
    }
}