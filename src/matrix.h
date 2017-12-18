#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdint.h>

#define MATRIX_INIT(m, dim, num_dims)          \
    do {                                       \
        uint32_t len = 1;                      \
        for (int i = 0; i < num_dims; i++) {   \
            m.dim[i] = dim[i];                 \
            len *= dim[i];                     \
        }                                      \
        m.num_dims = num_dims;                 \
        m.len = len;                           \
        m.val = malloc(sizeof(float) * len);   \
        if (m.val == NULL) {                   \
            FATAL(MEMORY_EXHAUSTED_ERROR"\n"); \
        }                                      \
    } while(0);

typedef struct __MATRIX Matrix;

struct __MATRIX {
    float *val;
    uint32_t dim[4];
    uint32_t num_dims;
    uint32_t len;
};

void matrix_init_random_norm(Matrix *m);
void matrix_init_constant(Matrix *m, float *data, size_t len);
void matrix_init_ones(Matrix *m);
void matrix_init_zeros(Matrix *m);

void matrix_scalar_add(Matrix *res, Matrix *m1, Matrix *m2, int diff);
void matrix_scalar_sub(Matrix *res, Matrix *m1, Matrix *m2, int diff);
void matrix_scalar_mul(Matrix *res, Matrix *m1, Matrix *m2, int diff);
void matrix_scalar_div(Matrix *res, Matrix *m1, Matrix *m2, int diff);

void matrix_add(Matrix *res, Matrix *m1, Matrix *m2, int diff);
void matrix_sub(Matrix *res, Matrix *m1, Matrix *m2, int diff);
void matrix_mul(Matrix *res, Matrix *m1, Matrix *m2, int diff);

void matrix_cost_mse(Matrix *res, Matrix *logits, Matrix *labels, int diff);

#endif
