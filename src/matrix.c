#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "matrix.h"

// if diff = 0, it will execute dst = val
// if diff = 1, it will execute dst = dst - val
#define assign(dst, val, diff) \
    dst = dst * diff - ((diff << 1) - 1) * (val);

void matrix_create(Matrix *m, uint32_t *dim, uint32_t num_dims)
{
    uint32_t len = 1;
    for (int i = 0; i < num_dims; i++) {
        m->dim[i] = dim[i];
        len *= dim[i];
    }
    m->num_dims = num_dims;
    m->len = len;
    m->val = malloc(sizeof(float) * len);
    if (m->val == NULL) {
        FATAL(MEMORY_EXHAUSTED_ERROR"\n");
    }
}

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

void matrix_add(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    assert(m1->num_dims == m2->num_dims && m2->num_dims == res->num_dims);
    assert(m1->len == m2->len && m2->len == res->len);

    for (int i = 0; i < res->len; i++) {
        assign(res->val[i], m1->val[i] + m2->val[i], diff);
    }
}

void matrix_sub(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    assert(m1->num_dims == m2->num_dims && m2->num_dims == res->num_dims);
    assert(m1->len == m2->len && m2->len == res->len);

    for (int i = 0; i < res->len; i++) {
        assign(res->val[i], m1->val[i] - m2->val[i], diff);
    }
}

void matrix_mul(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    assert(m1->num_dims >= 2 && m2->num_dims >= 2);
    assert(m1->num_dims == m2->num_dims);
    assert(m1->dim[m1->num_dims - 1] == m2->dim[m2->num_dims - 2]);

    int block = res->dim[res->num_dims - 2] * res->dim[res->num_dims - 1];
    for (int i = 0; i < res->len; i += block) {
        for (int a = 0; a < res->dim[res->num_dims - 2]; a++) {
            for (int b = 0; b < res->dim[res->num_dims - 1]; b++) {
                float sum = 0;
                for (int c = 0; c < m1->dim[m1->num_dims - 1]; c++) {
                    sum += m1->val[i + a * m1->dim[m1->num_dims - 1] + c] * m2->val[i + c * m2->dim[m2->num_dims - 1] + b];
                }
                assign(res->val[i + a * res->dim[res->num_dims - 1] + b], sum, diff)
            }
        }
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
