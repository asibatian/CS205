//matrix.cpp
#include <iostream>
#include "matrix.hpp"
using namespace std;


//Constructor:
Matrix::Matrix()
{
    row = 0;
    column = 0;
    *data = 0.0f;
}

Matrix::Matrix(int r, int c)
{
    row = r;
    column = c;
    size = r*c;
    data = new float[size];
}

Matrix::Matrix(int r, int c, float d[])
{
    row = r;
    column = c;
    size = r*c;
    data = d;
}

//Destrucotr:
Matrix::~Matrix()
{
    delete [] data;
}

//Assignment Operator:
Matrix & Matrix::operator =(const Matrix &m)
{
    if(this != &m)
    {
        row = m.row;
        column = m.column;
        size = m.size;
        data = m.data;
    }
    return *this;
}

//Operator Overloading:
std::ostream& operator <<(std::ostream& os, const Matrix &m)
{
    os << "\n[";
    for(int i = 0; i < m.size; i++)
    {
        os << m.data[i];
        if((i+1)%m.column != 0)
        {
            os << ", ";
        }
        else if((i+1)%m.column == 0 && i != (m.size-1))
        {
            os << ";\n";
        }
    }
    os << "]" << endl;
    return os;
}

//Matrix multipulication
Matrix Matrix::operator * (const Matrix &m)
{
    Matrix c = Matrix(row, m.column);
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < m.column; j++)
        {
            int index = i*row + j;
            c.data[index] = 0;
            for(int k = 0; k < column; k++)
            {
                c.data[index] += (data[i*column + k] * m.data[j + k*m.column]);
            }
        }
    }
    return c;
}

//Scalar multiply 1
Matrix operator * (double num, const Matrix &m)
{
    Matrix c = Matrix(m.row, m.column);
    for(int i = 0; i < m.size; i++)
    {
        c.data[i] = (num*m.data[i]);
    }
    return c;
}

//Scalar multiply 2
Matrix operator * (const Matrix &m, double num)
{
    Matrix c = Matrix(m.row, m.column);
    for(int i = 0; i < m.size; i++)
    {
        c.data[i] = (num*m.data[i]);
    }
    return c;
}

//Operator overloading +:
Matrix Matrix::operator + (const Matrix &m)
{
    if(row != m.row || column != m.column)
    {
        cout << "Dimension do not match!" << endl;
    }
    else
    {
        Matrix c = Matrix(row, column);
        for(int i = 0; i < size; i++)
        {
            c.data[i] = (data[i] + m.data[i]);
        }
        return c;
    }
}

//Operator overloading -:
Matrix Matrix::operator - (const Matrix &m)
{
    if(row != m.row || column != m.column)
    {
        cout << "Dimension do not match!" << endl;
    }
    else
    {
        Matrix d = Matrix(row, column);
        for(int i = 0; i < size; i++)
        {
            d.data[i] = (data[i] - m.data[i]);
        }
        return d;
    }   
}