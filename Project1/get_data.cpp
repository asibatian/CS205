#include <iostream>
#include "functions.h"

//get elements from input
void get_data(struct matrix* m) 
{
    for(int i = 0; i < (m->nRow); i++)
    {
        for(int j = 0; j < (m->nColumn); j++)
        {
            float value = 0.0f;
            std::cout << "Please input element of " << i+1 <<"th row and " << j+1 << "th column: ";
            std::cin >> value;
            m->data[i][j] = value;           
        }
    }
}