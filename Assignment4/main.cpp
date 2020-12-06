//main.cpp
#include <iostream>
#include "matrix.hpp"
using namespace std;

int main()
{
    float a[6]={1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f};
    float b[6]={1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    Matrix A = Matrix(2, 3, a);
    Matrix B = Matrix(3, 2, b);

    Matrix C = A*B;
    cout << "Matrix C = " << C << endl;
    Matrix D = 2*A;
    cout << "Matrix D = " << D << endl;
    Matrix E = A*3;
    cout << "Matrix E = " << E << endl;

    Matrix F = Matrix(2, 3, a);
    Matrix G = Matrix(2, 3, b);
    Matrix H = (F+G);
    cout << "Matrix H = " << H << endl;
    Matrix I = (F-G);
    cout << "Matrix I = " << I << endl;

    return 0;
}