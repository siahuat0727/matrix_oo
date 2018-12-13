#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "matrix.h"

#define return_if_fail(p) \
    if(!(p)) { \
        printf("%s:%d Warning: "#p" failed.\n", \
        __func__, __LINE__); \
        return; \
    }
#define return_val_if_fail(p, ret) \
    if(!(p)) { \
        printf("%s:%d Warning: "#p" failed.\n", \
        __func__, __LINE__); \
        return (ret); \
    }


static bool equal(const Matrix, const Matrix, EleEqualFunc);
static Matrix mul(const Matrix a, const Matrix b, EleFunc ele_mul, EleFunc ele_add);
static void print(const Matrix m, ElePrintFunc);
static void mfree(Matrix *m);
static void *get_ele(const Matrix m, int i, int j);

MatrixOp matrix_op_impl()
{
    MatrixOp matrix_op = {
        .equal = equal,
        .mul = mul,
        .print = print,
        .mfree = mfree,
        .get_ele = get_ele,
    };
    return matrix_op;
}

Matrix matrix_impl(void* m, int num_row, int num_col, size_t sizeof_type)
{
    Matrix mat = {
        .sizeof_type = sizeof_type,
        .num_row = num_row,
        .num_col = num_col,
    };
    mat.values = (void **)malloc(sizeof(void *) * mat.num_row);
    mat.values[0] = malloc(sizeof_type * mat.num_col * mat.num_row);
    for (int i = 1; i < mat.num_row; ++i)
        mat.values[i] = (char*)mat.values[0] + i * sizeof_type * mat.num_col;
    if (m)
        memcpy(mat.values[0], m, num_row * num_col * sizeof_type);
    else
        memset(mat.values[0], 0, sizeof_type * mat.num_row * mat.num_col);
    return mat;
}


#define max(a, b) ({ \
        __typeof__(a) _a = a; \
        __typeof__(b) _b = b; \
        _a > _b ? _a : _b; \
        })

static void *get_ele(const Matrix m, int i, int j)
{
    return (char*)*(m.values) + m.sizeof_type * (i * m.num_col + j);
}

static Matrix mul(const Matrix a, const Matrix b, EleFunc ele_mul, EleFunc ele_add)
{
    return_val_if_fail(a.num_col == b.num_row, (Matrix){0});
    Matrix m = matrix_impl(NULL, a.num_row, b.num_col, max(a.sizeof_type, b.sizeof_type));

    for (int i = 0; i < m.num_row; ++i) {
        for (int j = 0; j < m.num_col; ++j) {
            for (int k = 0; k < a.num_col; ++k) {
                char temp[m.sizeof_type];
                ele_mul(get_ele(a, i, k), get_ele(b, k, j), temp);
                ele_add(get_ele(m, i, j), temp, get_ele(m, i, j));
            }
        }
    }
    return m;
}

static bool equal(const Matrix a, const Matrix b, EleEqualFunc ele_equal)
{
    if (a.num_row != b.num_row || a.num_col != b.num_col)
        return false;

    for (int i = 0; i < a.num_row; i++)
        for (int j = 0; j < a.num_col ; j++)
            if (!ele_equal(get_ele(a, i, j), get_ele(b, i, j)))
                return false;
    return true;
}

static void print(const Matrix m, ElePrintFunc ele_print)
{
    for (int i = 0; i < m.num_row; ++i) {
        for (int j = 0; j < m.num_col; ++j) {
            ele_print(get_ele(m, i, j));
            putchar(' ');
        }
        puts("");
    }
    puts("");
}

static void mfree(Matrix *m)
{
    free(m->values[0]);
    free((m->values));
    memset(m, 0, sizeof(Matrix));
}
