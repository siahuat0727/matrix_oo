#include <stdio.h>
#include "matrix.h"

void int_print(void *src)
{
    printf("%d", *(int*)src);
}

void double_print(void *src)
{
    printf("%.2f", *(double*)src);
}

void int_double_mul(void *src1, void *src2, void *dst)
{
    *(double*)dst = *(int*)src1 * *(double*)src2;
}

void int_double_add(void *src1, void *src2, void *dst)
{
    *(double*)dst = *(int*)src1 + *(double*)src2;
}


int main()
{
    MatrixOp mat_op = matrix_op_impl();

    Matrix m = MATRIX(((int[2][3]){ {0, 1, 2}, {3, 4, 5}, }));
    mat_op.print(m, int_print);

    Matrix n = MATRIX(((double[3][2]){ {0.5, 1.5}, {2.5, 3.5}, {5.5, 6.5}, }));
    mat_op.print(n, double_print);

    Matrix o = mat_op.mul(m, n, int_double_mul, int_double_add);
    mat_op.print(o, double_print);

    return 0;
}
