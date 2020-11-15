/*
Nuofan Qiu - 11713013
*/
#include <iostream>
#include <limits>
#include <chrono>
#include "functions.h"

int main()
{
    using namespace std;
    matrix m1; //First matrix, m by n
    matrix m2; //Second matrix, n by p
    matrix c; // c = m1*m2, m by p
    char choice;

    get_info(&m1);
    get_info(&m2);

    c.nRow = m1.nRow; //set result's row equals to row1
    c.nColumn = m2.nColumn; //set result's column equals to column2

    allocate_data(&m1);
    allocate_data(&m2);
    allocate_data(&c);
    
    cout << "If you want to input elements by hand, please input \"1\"."
        << "\nIf you want generate elements by random, please input \"2\":";
    cin >> choice;
    if(choice == '1')
    {   
        get_data(&m1);
        get_data(&m2);
    }
    else if (choice == '2')
    {
        gen_data(&m1);
        gen_data(&m2);
    }
    else
    {
        cout << "Please input \"1\" or \"2\" to choice the way you want to initialize matrix." <<endl;
    }
    
    //gen_data(&m1);
    //gen_data(&m2);

    auto start = std::chrono::steady_clock::now();
    mat_multi(&m1, &m2, &c);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << "duration for mat_multi = " << duration << "ms" << endl;

    auto start1 = std::chrono::steady_clock::now();
    mat_multi1(&m1, &m2, &c);
    auto end1 = std::chrono::steady_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();
    cout << "duration for mat_multi1 = " << duration1 << "ms" << endl;

    auto start2 = std::chrono::steady_clock::now();
    mat_multi2(&m1, &m2, &c);
    auto end2 = std::chrono::steady_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
    cout << "duration for mat_multi2 = " << duration2 << "ms" << endl;

    /*auto start3 = std::chrono::steady_clock::now();
    mat_multi3(&m1, &m2, &c);
    auto end3 = std::chrono::steady_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - start3).count();
    cout << "duration for mat_multi3 = " << duration3 << "ms" << endl;*/

    auto start4 = std::chrono::steady_clock::now();
    mat_multi4(&m1, &m2, &c);
    auto end4 = std::chrono::steady_clock::now();
    auto duration4 = std::chrono::duration_cast<std::chrono::milliseconds>(end4 - start4).count();
    cout << "duration for OpenBLAS = " << duration4 << "ms" << endl;

    delete [] m1.data;
    delete [] m2.data;
    delete [] c.data;
    return 0;
}
