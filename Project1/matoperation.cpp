#include <iostream>
#include <immintrin.h>
#include <omp.h>
//#include <cblas.h>
#include "functions.h"

//Calculate mat_multi according to the definition
void mat_multi(struct matrix* m1, struct matrix* m2, struct matrix* c)
{
    using namespace std;
    if(m1->nColumn != m2->nRow) //Check the nRow1 and nColumn2 to make sure they can multiply
    {
        cout << "Wrong Input!\nColumn number of first matrix and row number of second matrix should be equal!" << endl;
        return; //exit from void function
    }
    else
    {
        for(int i = 0; i < (c->nRow); i++)
        {
            for(int j = 0; j < (c->nColumn); j++)
            {
                c->data[i][j] = 0.0f;
                for(int n = 0; n < (m1->nColumn); n++)
                {
                    c->data[i][j] += (m1->data[i][n]*m2->data[n][j]);
                }
            }
        }
    }
    
}

//Speed up method 1
void mat_multi1(struct matrix* m1, struct matrix* m2, struct matrix* c)
{
    using namespace std;
    if(m1->nColumn != m2->nRow) //Check the nRow1 and nColumn2 to make sure they can multiply
    {
        cout << "Wrong Input!\nColumn number of first matrix and row number of second matrix should be equal!" << endl;
        return; //exit from void function
    }
    else
    {
        if(m1->nColumn % 8 != 0)
        {
            std::cerr << "The column number of first matrix must be a multiple of 8." <<std::endl;
            return;
        }

        for(int i = 0; i < (c->nRow); i++)
        {
            for(int j = 0; j < (c->nColumn); j++)
            {
                c->data[i][j] = 0.0f;
                for(int n = 0; n < (m1->nColumn); n+=8)
                {
                    c->data[i][j] += (m1->data[i][n]*m2->data[n][j]);
                    c->data[i][j] += (m1->data[i][n+1]*m2->data[n+1][j]);
                    c->data[i][j] += (m1->data[i][n+2]*m2->data[n+2][j]);
                    c->data[i][j] += (m1->data[i][n+3]*m2->data[n+3][j]);
                    c->data[i][j] += (m1->data[i][n+4]*m2->data[n+4][j]);
                    c->data[i][j] += (m1->data[i][n+5]*m2->data[n+5][j]);
                    c->data[i][j] += (m1->data[i][n+6]*m2->data[n+6][j]);
                    c->data[i][j] += (m1->data[i][n+7]*m2->data[n+7][j]);
                }
            }
        }
    }
}

//Use SIMD to speed up
void mat_multi2(struct matrix* m1, struct matrix* m2, struct matrix* c)
{
    using namespace std;
    if(m1->nColumn != m2->nRow) //Check the nRow1 and nColumn2 to make sure they can multiply
    {
        cout << "Wrong Input!\nColumn number of first matrix and row number of second matrix should be equal!" << endl;
        return; //exit from void function
    }
    else
    {
        if(m1->nColumn % 8 != 0)
        {
            std::cerr << "The column number of first matrix must be a multiple of 8." <<std::endl;
            return;
        }

        for(int i = 0; i < (c->nRow); i++)
        {
            for(int j = 0; j < (c->nColumn); j++)
            {
                float sum[8] = {0};
                __m256 a, b;
                __m256 d = _mm256_setzero_ps();
                for(int n = 0; n < (m1->nColumn); n+=8)
                {
                   a = _mm256_load_ps(m1->data[i]);
                   b = _mm256_load_ps(m2->data[n]);
                   d =  _mm256_add_ps(d, _mm256_mul_ps(a, b));
                }
                _mm256_store_ps(sum, d);
                c->data[i][j] = sum[0]+sum[1]+sum[2]+sum[3]+sum[4]+sum[5]+sum[6]+sum[7];
            }
        }
    }
    
}

//Use OpenMP to speed up
/*
void mat_multi3(struct matrix* m1, struct matrix* m2, struct matrix* c)
{
    using namespace std;
    if(m1->nColumn != m2->nRow) //Check the nRow1 and nColumn2 to make sure they can multiply
    {
        cout << "Wrong Input!\nColumn number of first matrix and row number of second matrix should be equal!" << endl;
        return; //exit from void function
    }
    else
    {
        if(m1->nColumn % 8 != 0)
        {
            std::cerr << "The column number of first matrix must be a multiple of 8." <<std::endl;
            return;
        }

        #pragma omp parallel for
        for(int i = 0; i < (c->nRow); i++)
        {
            for(int j = 0; j < (c->nColumn); j++)
            {
                float sum[8] = {0};
                __m256 a, b;
                __m256 d = _mm256_setzero_ps();
                for(int n = 0; n < (m1->nColumn); n+=8)
                {
                   a = _mm256_load_ps((m1->data[i]));
                   b = _mm256_load_ps(m2->data[n]);
                   d =  _mm256_add_ps(d, _mm256_mul_ps(a, b));
                }
                _mm256_store_ps(sum, d);
                c->data[i][j] = sum[0]+sum[1]+sum[2]+sum[3]+sum[4]+sum[5]+sum[6]+sum[7];
            }
        }
    }
    
}

//Use OpenBLAS
void mat_multi4(struct matrix* m1, struct matrix* m2, struct matrix* c)
{
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1->nRow, m2->nColumn, m1->nRow, 1.0, m1->data, m1->nColumn, m2->data, m2->nColumn, 0.0, **matrix::data, 1);
}*/