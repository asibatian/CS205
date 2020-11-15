#include <iostream>

struct matrix
{
    int nRow;
    int nColumn;
    float** data;
};

void mat_multi(struct matrix* m1, struct matrix* m2, struct matrix* c);
void mat_multi1(struct matrix* m1, struct matrix* m2, struct matrix* c);
void mat_multi2(struct matrix* m1, struct matrix* m2, struct matrix* c);
void mat_multi3(struct matrix* m1, struct matrix* m2, struct matrix* c);
void mat_multi4(struct matrix* m1, struct matrix* m2, struct matrix* c);
void get_info(struct matrix* m);
void get_data(struct matrix* m);
void gen_data(struct matrix* m);
void allocate_data(struct matrix* m);