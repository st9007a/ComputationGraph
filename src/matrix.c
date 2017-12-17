#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "matrix.h"

inline void init_random_norm(Matrix *m)
{
    int std = 1, mean = 0;

    for (int i = 0; i < m->len; i++) {
        m->val[i] = sqrt(-2 * log(rand() / (float)RAND_MAX)) * cos(2 * M_PI * rand() / (float)RAND_MAX) * std + mean;
    }
}

inline void init_constant(Matrix *m, float *data, size_t len)
{
    memcpy(m->val, data, sizeof(float) * len);
}

inline void init_ones(Matrix *m)
{
    for (int i = 0; i < m->len; i++) {
        m->val[i] = 1;
    }
}

inline void init_zeros(Matrix *m)
{
    memset(m->val, 0, sizeof(float) * m->len);
}
