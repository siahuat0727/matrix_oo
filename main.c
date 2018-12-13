#include <stdio.h>
#include "matrix.h"

#define epsilon 1e-8

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


bool double_equal(void *src1, void *src2)
{
    double d1 = *(double*)src1;
    double d2 = *(double*)src2;
    if (d1 + epsilon < d2 || d2 + epsilon < d1)
        return false;
    return true;
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

    Matrix ans = MATRIX(((double[2][2]){ {11.00, 13.00}, {27.50, 32.50}, }));
    bool is_equal = mat_op.equal(o, ans, double_equal);
    puts(is_equal ? "Correct!" : "Wrong!");

    return 0;
}
