#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct __matrix_impl Matrix;
struct __matrix_impl {
    void **values;
    int num_row;
    int num_col;
    int type;

    /* operations */
    bool (*equal)(const Matrix, const Matrix);
    Matrix (*mul)(const Matrix, const Matrix);
    void (*print)(const Matrix);
};

#define mat_print_select(X) \
    _Generic((X), \
            int: mat_print_int, double: mat_print_double \
    )

#define get_type_num(X) \
    _Generic((X), \
            int: 0, double: 1 \
    )

#define MATRIX(mat) ({ \
        Matrix m = { \
            .num_row = sizeof(mat) / sizeof(mat[0]), \
            .num_col = sizeof(mat[0]) / sizeof(mat[0][0]), \
            .print = mat_print_select(mat[0][0]), \
            .type = get_type_num(mat[0][0]), \
            .mul =  mul, \
        }; \
        m.values = (void **)malloc(sizeof(void *) * m.num_row); \
        for (int i = 0; i < m.num_row; ++i) \
            m.values[i] = malloc(sizeof(__typeof__(mat[0][0])) * m.num_col); \
        for (int i = 0; i < m.num_row; ++i) \
            for (int j = 0; j < m.num_col; ++j) \
                *((__typeof__(mat[0][0])*)(m.values[i]) + j) = mat[i][j]; \
        m; \
        })

#define mat_print_generic(type, format) \
    static void mat_print_##type(const Matrix m) \
    { \
        for (int i = 0; i < m.num_row; ++i) { \
            for (int j = 0; j < m.num_col; ++j) { \
                printf(format " ", *((type*)(m.values[i])+j)); \
            } \
            puts(""); \
        } \
        puts(""); \
    }

mat_print_generic(int, "%d")
mat_print_generic(double, "%f")

#define get_element(mat, i, j, T) *((T*)mat.values[i] + j)

static Matrix mul(const Matrix a, const Matrix b)
{
    if (a.num_col != b.num_row) {
        puts("Can't mul!");
        return (Matrix){0};
    }
    Matrix m = {
        .num_row = a.num_row,
        .num_col = b.num_col,
        .print = mat_print_select(get_element(a, 0, 0, int) * get_element(b, 0, 0, double)),
        .type = get_type_num(get_element(a, 0, 0, int) * get_element(b, 0, 0, double)),
    };
    m.values = (void **)malloc(sizeof(void *) * m.num_row);
    for (int i = 0; i < m.num_row; ++i) {
        m.values[i] = malloc(
                sizeof(get_element(a, 0, 0, int) * get_element(b, 0, 0, double)) * m.num_col);
    }
    for (int i = 0; i < m.num_row; ++i) {
        for (int j = 0; j < m.num_col; ++j) {
            get_element(m, i, j, double) = 0;
            for (int k = 0; k < a.num_col; ++k) {
                get_element(m, i, j, double) +=
                    get_element(a, i, k, int) * get_element(b, k, j, double);
            }
        }
    }
    return m;
}

// static const float epsilon = 1 / 10000.0;

// static bool equal(const Matrix a, const Matrix b)
// {
//     for (int i = 0; i < 4; i++)
//         for (int j = 0; j < 4; j++)
//             if (a.values[i][j] + epsilon < b.values[i][j]
//                     || b.values[i][j] + epsilon < a.values[i][j])
//                 return false;
//     return true;
// }

int main()
{
    int m1[2][3] = { {0, 1, 2}, {3, 4, 5}, };
    double m2[3][2] = { {0, 1}, {2, 3}, {4, 5}, };

    Matrix m = MATRIX(m1);
    m.print(m);

    Matrix n = MATRIX(m2);
    n.print(n);

    Matrix o = m.mul(m, n);
    o.print(o);

    return 0;
}
