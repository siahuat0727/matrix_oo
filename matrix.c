#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum mat_type {INT_T, DOUBLE_T};

typedef struct __matrix_impl Matrix;
struct __matrix_impl {
    void **values;
    int num_row;
    int num_col;
    enum mat_type type;
    size_t sizeof_type;

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
            int: INT_T, double: DOUBLE_T \
    )

enum mat_type get_type_merge(const Matrix a, const Matrix b)
{
    switch(a.type) {
        case INT_T:
            switch(b.type) {
                case INT_T:
                    return get_type_num((int)0 * (int)0);
                case DOUBLE_T:
                    return get_type_num((int)0 * (double)0);
            }
        case DOUBLE_T:
            switch(b.type) {
                case INT_T:
                    return get_type_num((double)0 * (int)0);
                case DOUBLE_T:
                    return get_type_num((double)0 * (double)0);
            }
    }
    return -1;
}


#define get_element_select(X) \
    _Generic((X), \
            int: get_element_int, double: get_element_double \
    )


#define MATRIX(mat) ({ \
        Matrix m = { \
            .num_row = sizeof(mat) / sizeof(mat[0]), \
            .num_col = sizeof(mat[0]) / sizeof(mat[0][0]), \
            .print = mat_print_select(mat[0][0]), \
            .type = get_type_num(mat[0][0]), \
            .mul =  mul, \
            .sizeof_type = sizeof(mat[0][0]), \
        }; \
        m.values = (void **)malloc(sizeof(void *) * m.num_row); \
        for (int i = 0; i < m.num_row; ++i) \
            m.values[i] = malloc(sizeof(mat[0][0]) * m.num_col); \
        for (int i = 0; i < m.num_row; ++i) \
            for (int j = 0; j < m.num_col; ++j) \
                *((__typeof__(mat[0][0])*)m.values[i] + j) = mat[i][j]; \
        m; \
        })

#define mat_print_generic(T, format) \
    static void mat_print_##T(const Matrix m) \
    { \
        for (int i = 0; i < m.num_row; ++i) { \
            for (int j = 0; j < m.num_col; ++j) { \
                printf(format " ", *((T*)m.values[i]+j)); \
            } \
            puts(""); \
        } \
        puts(""); \
    }

mat_print_generic(int, "%d")
mat_print_generic(double, "%f")

void (*mat_print_select_num(enum mat_type type))(const Matrix)
{
    switch(type) {
        case INT_T:
            return mat_print_int;
        case DOUBLE_T:
            return mat_print_double;
    }
    return NULL;
}

#define get_element(m, i, j, T) \
    *((T*)m.values[i] + j)

#define max(a, b) ({ \
        __typeof__(a) _a = a; \
        __typeof__(b) _b = b; \
        _a > _b ? _a : _b; \
})

static Matrix mul(const Matrix a, const Matrix b)
{
    if (a.num_col != b.num_row) {
        puts("Can't mul!");
        return (Matrix){0};
    }
    Matrix m = {
        .num_row = a.num_row,
        .num_col = b.num_col,
        .type = get_type_merge(a, b),
        .print = mat_print_select_num(get_type_merge(a, b)),
        .sizeof_type = max(a.sizeof_type, b.sizeof_type),
    };
    m.values = (void **)malloc(sizeof(void *) * m.num_row);
    for (int i = 0; i < m.num_row; ++i) {
        m.values[i] = malloc(m.sizeof_type * m.num_col);
        memset(m.values[i], 0, m.num_col * m.sizeof_type);
    }
    for (int i = 0; i < m.num_row; ++i) {
        for (int j = 0; j < m.num_col; ++j) {
            for (int k = 0; k < a.num_col; ++k) {
                switch(a.type) {
                    case INT_T:
                        switch(b.type) {
                            case INT_T:
                                get_element(m, i, j, int) += get_element(a, i, j, int) * get_element(b, i, j, int);
                                break;
                            case DOUBLE_T:
                                get_element(m, i, j, double) += get_element(a, i, j, int) * get_element(b, i, j, double);
                                break;
                        }
                        break;
                    case DOUBLE_T:
                        switch(b.type) {
                            case INT_T:
                                get_element(m, i, j, double) += get_element(a, i, j, double) * get_element(b, i, j, int);
                                break;
                            case DOUBLE_T:
                                get_element(m, i, j, double) += get_element(a, i, j, double) * get_element(b, i, j, double);
                                break;
                        }
                        break;
                }
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
    Matrix m = MATRIX(((int[2][3]){ {0, 1, 2}, {3, 4, 5}, }));
    m.print(m);

    Matrix n = MATRIX(((int[3][2]){ {0, 1}, {2, 3}, {4, 5}, }));
    n.print(n);

    Matrix o = m.mul(m, n);
    o.print(o);

    return 0;
}
