#include <stdlib.h>

typedef struct __matrix_impl Matrix;
typedef void *(*EleGetFunc)(const Matrix, int, int);
struct __matrix_impl {
    void **values;
    size_t sizeof_type;
    int num_row;
    int num_col;
    EleGetFunc get_ele;
};

typedef struct __matrix_op_impl MatrixOp;

typedef bool (*EleEqualFunc)(void *, void *);
typedef bool (*MatrixEqualFunc)(const Matrix, const Matrix, EleEqualFunc);

typedef void (*EleFunc)(void *, void *, void *);
typedef Matrix (*MatrixMulFunc)(const Matrix, const Matrix, EleFunc, EleFunc);

typedef void (*ElePrintFunc)(void *);
typedef void (*MatrixPrintFunc)(const Matrix, ElePrintFunc);

struct __matrix_op_impl {
    MatrixEqualFunc equal;
    MatrixMulFunc mul;
    MatrixPrintFunc print;
};


MatrixOp matrix_op_impl();
Matrix matrix_impl(void* m, int num_row, int num_col, size_t sizeof_type);
