#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdint.h>
#include "dl.h"

struct __DL_MATRIX {
    float *val;
    uint32_t dim[4];
    uint32_t stride[4];
    uint32_t num_dims;
    uint32_t len;
};

void matrix_create(Matrix *m, uint32_t *dim, uint32_t num_dims);

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

void matrix_shape_reshape(Matrix *res, Matrix *m, Matrix *hold, int diff);
void matrix_shape_transpose(Matrix *res, Matrix *m, Matrix *hold, int diff);

void matrix_act_relu(Matrix *res, Matrix *preact, Matrix *hold, int diff);
void matrix_act_sigmoid(Matrix *res, Matrix *preact, Matrix *hold, int diff);
void matrix_act_softmax(Matrix *res, Matrix *preact, Matrix *hold, int diff);

void matrix_cost_mse(Matrix *res, Matrix *logits, Matrix *labels, int diff);

#endif
