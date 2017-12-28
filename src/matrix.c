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

static inline float *matrix_access(Matrix *m, uint32_t w, uint32_t x, uint32_t y, uint32_t z)
{
    assert(w < m->dim[0] && x < m->dim[1] && y < m->dim[2] && z < m->dim[3]);
    return m->val + w * m->stride[0] + x * m->stride[1] + y * m->stride[2] + z * m->stride[3];
}

void matrix_create(Matrix *m, uint32_t *dim, uint32_t num_dims)
{
    uint32_t len = 1;
    for (int i = 0; i < 4; i++) {
        m->dim[i] = 1;
        m->stride[i] = 0;
    }
    for (int i = 3; i >= 4 - num_dims; i--) {
        m->dim[i] = dim[i - 4 + num_dims];
        m->stride[i] = len;
        len *= m->dim[i];
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
    assert(!memcmp(m2->dim, m1->dim, m2->num_dims));
    assert(!memcmp(res->dim, m1->dim, res->num_dims));

    for (int i = 0; i < res->dim[0]; i++) {
        for (int j = 0; j < res->dim[1]; j++) {
            for (int k = 0; k < res->dim[2]; k++) {
                for (int l = 0; l < res->dim[3]; l++) {
                    assign(*matrix_access(res, i, j, k, l), *matrix_access(m1, i, j, k, l) + *matrix_access(m2, i, j, k, l), diff);
                }
            }
        }
    }


}

void matrix_sub(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    assert(!memcmp(m2->dim, m1->dim, m2->num_dims));
    assert(!memcmp(res->dim, m1->dim, res->num_dims));

    for (int i = 0; i < res->dim[0]; i++) {
        for (int j = 0; j < res->dim[1]; j++) {
            for (int k = 0; k < res->dim[2]; k++) {
                for (int l = 0; l < res->dim[3]; l++) {
                    assign(*matrix_access(res, i, j, k, l), *matrix_access(m1, i, j, k, l) - *matrix_access(m2, i, j, k, l), diff);
                }
            }
        }
    }
}

void matrix_mul(Matrix *res, Matrix *m1, Matrix *m2, int diff)
{
    // TODO: implement matrix mul
}

// FIXME: matrix_access
void matrix_shape_reshape(Matrix *res, Matrix *m, Matrix *hold, int diff)
{
    assert(res->len == m->len);

    for (int i = 0; i < res->len; i++) {
        assign(res->val[i], m->val[i], diff);
    }
}

// FIXME: swap shape and stride
void matrix_shape_transpose(Matrix *res, Matrix *m, Matrix *hold, int diff)
{
    assert(res->len == m->len);

    for (int i = 0; i < res->len; i++) {
        assign(res->val[i], m->val[i], diff);
    }
}

void matrix_act_relu(Matrix *res, Matrix *preact, Matrix *hold, int diff)
{
    assert(res->num_dims == preact->num_dims && res->len == preact->len);

    for (int i = 0; i < res->len; i++) {
        assign(res->val[i], preact->val[i] >= 0 ? preact->val[i] : 0, diff);
    }
}

void matrix_act_sigmoid(Matrix *res, Matrix *preact, Matrix *hold, int diff)
{
    assert(res->num_dims == preact->num_dims && res->len == preact->len);

    for (int i = 0; i < res->len; i++) {
        assign(res->val[i], 1 / (1 + exp(0 - preact->val[i])), diff);
    }
}

// FIXME: matrix_access
void matrix_act_softmax(Matrix *res, Matrix *preact, Matrix *hold, int diff)
{
    assert(res->num_dims == preact->num_dims && res->len == preact->len);

    float sum = 0;
    for (int i = 0; i < preact->len; i++) {
        sum += exp(preact->val[i]);
    }
    for (int i = 0; i < res->len; i++) {
        assign(res->val[i], exp(preact->val[i]) / sum, diff);
    }
}

// FIXME: matrix_access
void matrix_cost_mse(Matrix *res, Matrix *logits, Matrix *labels, int diff)
{
    assert(logits->len == labels->len && logits->num_dims == labels->num_dims);

    float sum = 0;
    for (int i = 0; i < logits->len; i++) {
        sum += pow(logits->val[i] - labels->val[i], 2);
    }
    assign(res->val[0], sum / logits->len, diff);
}
