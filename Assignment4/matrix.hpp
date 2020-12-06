//matrix.hpp
#include <iostream>
using namespace std;

class Matrix
{
    private:
        int row;
        int column;
        int size;
        float *data;

    public:
        //Constructors:
        Matrix();
        Matrix(int r, int c);
        Matrix(int r, int c, float d[]);
        
        Matrix & operator =(const Matrix &m);
        friend std::ostream& operator <<(std::ostream& os, const Matrix &m);
        Matrix operator * (const Matrix &m);
        friend Matrix operator * (double num, const Matrix &m);
        friend Matrix operator * (const Matrix &m, double num);
        Matrix operator + (const Matrix &m);
        Matrix operator - (const Matrix &m);

        //Destructor:
        ~Matrix();
};
