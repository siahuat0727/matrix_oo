#include <stdio.h>
#include "matrix.h"

void int_print(void *src)
{
    printf("%d", *(int*)src);
}

void int_mul(void *src1, void *src2, void *dst)
{
    *(int*)dst = *(int*)src1 * *(int*)src2;
}

void int_add(void *src1, void *src2, void *dst)
{
    *(int*)dst = *(int*)src1 + *(int*)src2;
}


int main()
{
    MatrixOp mat_op = matrix_op_impl();

    Matrix m = MATRIX(((int[2][3]){ {0, 1, 2}, {3, 4, 5}, }));
    mat_op.print(m, int_print);

    Matrix n = MATRIX(((int[3][2]){ {0, 1}, {2, 3}, {4, 5}, }));
    mat_op.print(n, int_print);

    Matrix o = mat_op.mul(m, n, int_mul, int_add);
    mat_op.print(o, int_print);

    return 0;
}
