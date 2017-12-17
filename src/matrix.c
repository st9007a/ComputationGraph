#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "matrix.h"

inline void matrix_init_random_norm(Matrix *m)
{
    int std = 1, mean = 0;

    for (int i = 0; i < m->len; i++) {
        m->val[i] = sqrt(-2 * log(rand() / (float)RAND_MAX)) * cos(2 * M_PI * rand() / (float)RAND_MAX) * std + mean;
    }
}

inline void matrix_init_constant(Matrix *m, float *data, size_t len)
{
    memcpy(m->val, data, sizeof(float) * len);
}

inline void matrix_init_ones(Matrix *m)
{
    for (int i = 0; i < m->len; i++) {
        m->val[i] = 1;
    }
}

inline void matrix_init_zeros(Matrix *m)
{
    memset(m->val, 0, sizeof(float) * m->len);
}

void matrix_scalar_add(Matrix *res, Matrix *m, float val)
{
    assert(res->len == m->len && res->num_dims == m->num_dims);

    for (int i = 0; i < m->len; i++) {
        res->val[i] = m->val[i] + val;
    }
}

void matrix_scalar_sub(Matrix *res, Matrix *m, float val)
{
    assert(res->len == m->len && res->num_dims == m->num_dims);

    for (int i = 0; i < m->len; i++) {
        res->val[i] = m->val[i] - val;
    }
}

void matrix_scalar_mul(Matrix *res, Matrix *m, float val)
{
    assert(res->len == m->len && res->num_dims == m->num_dims);

    for (int i = 0; i < m->len; i++) {
        res->val[i] = m->val[i] * val;
    }
}

void matrix_scalar_div(Matrix *res, Matrix *m, float val)
{
    assert(res->len == m->len && res->num_dims == m->num_dims);

    for (int i = 0; i < m->len; i++) {
        res->val[i] = m->val[i] / val;
    }
}
