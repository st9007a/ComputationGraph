#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>

#include "matrix.h"

// if diff = 0, it will execute dst = val
// if diff = 1, it will execute dst = dst - val
#define assign(dst, val, diff) \
    dst = dst * diff - ((diff << 1) - 1) * (val);

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

void matrix_scalar_add(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    assert(res->len == m1->len && res->num_dims == m1->num_dims);
    assert(m2->len == 1 && m2->num_dims == 0);

    for (int i = 0; i < m1->len; i++) {
        assign(res->val[i], m1->val[i] + m2->val[0], diff);
    }
}

void matrix_scalar_sub(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    assert(res->len == m1->len && res->num_dims == m1->num_dims);
    assert(m2->len == 1 && m2->num_dims == 0);

    for (int i = 0; i < m1->len; i++) {
        assign(res->val[i], m1->val[i] - m2->val[0], diff);
    }
}

void matrix_scalar_mul(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    assert(res->len == m1->len && res->num_dims == m1->num_dims);
    assert(m2->len == 1 && m2->num_dims == 0);

    for (int i = 0; i < m1->len; i++) {
        assign(res->val[i], m1->val[i] * m2->val[0], diff);
    }
}

void matrix_scalar_div(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    assert(res->len == m1->len && res->num_dims == m1->num_dims);
    assert(m2->len == 1 && m2->num_dims == 0);

    for (int i = 0; i < m1->len; i++) {
        assign(res->val[i], m1->val[i] / m2->val[0], diff);
    }
}

void matrix_cost_mse(Matrix *res, Matrix *logits, Matrix *labels, int diff)
{
    assert(logits->len == labels->len && logits->num_dims == labels->num_dims);

    float sum = 0;
    for (int i = 0; i < logits->len; i++) {
        sum += pow(logits->val[i] - labels->val[i], 2);
    }
    assign(res->val[0], sum / logits->len, diff);
}
