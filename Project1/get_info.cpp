#include <iostream>
#include<limits>
#include "functions.h"

//get row and column numbers
void get_info(struct matrix* m)
{
    using namespace std;

    cout << "Please input the row number of matrix: " << endl;
    cin >> m->nRow;
    while(1) //cheak input if is integer
    {
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"You have entered wrong input, please input an integer for row number:"<<endl;
            cin >> m->nRow;
        }
        if(!cin.fail())
        {
            break;
        }
    }
    

    cout << "Please input the column number of matrix: " << endl;
    cin >> m->nColumn;
    while(1)
    {
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"You have entered wrong input, please input an integer for column number:"<<endl;
            cin >> m->nRow;
        }
        if(!cin.fail())
        {
            break;
        }
    }
}