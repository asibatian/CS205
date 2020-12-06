# CS205 C/C++ Programming - Assignment 4

**Name:** 邱诺凡(Nuofan Qiu)

**SID:** 11713013

## Part 1 - Analysis

本次作业的主要实现包括创造了一个`Matrix`类，并在其中定义了多个 constructors，一个 destructor。还有 `=`, `<<`, `+`, `-`, `*` 等运算符实现重载。

- 类的构造：

    ```c++
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
    ```

    类中的 `row` 用来存储矩阵的行数，`column` 用来存储矩阵的列数，`size` 则用来表示矩阵的元素个数（大小）。

- Constructors：

    ```c++
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
    ```

    实现了三种 constructor，其中第一种为默认constructor，另外两种则根据特定的参数列表进行构造。

- Destructor：

    ```c++
    //Destrucotr:
    Matrix::~Matrix()
    {
        delete [] data;
    }
    ```

    

- `=` 的重载：

    ```c++
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
    ```

- `<<` 的重载：

    ```c++
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
    ```

    通过对 `<<` 运算符的重载，以此来实现矩阵的格式化输出。

- `*` 的重载

    ```c++
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
    
    ```

    通过对 `*` 运算符进行重载，来实矩阵乘法以及矩阵数乘的实现。

- `+` 的重载：

    ```c++
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
    
    ```

- `-` 的重载：

    ```c++
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
    ```



## Part 2 - Code

完整代码请点击[这里](https://github.com/asibatian/CS205/tree/main/Assignment4)



## Part 3 - Result & Verification

在PC的命令行中，运行以下指令：

```
cmake .
make
./Matrix.exe
```

得到如下结果：![image-20201206203100963](C:\Users\61648\OneDrive\桌面\image-20201206203100963.png)

矩阵C是矩阵乘法运算得到的。矩阵D和矩阵E分别是矩阵A左乘以及右乘一个常数得到。矩阵H和矩阵I则分别是运行矩阵的加法和减法运算得到的。

