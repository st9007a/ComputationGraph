#ifndef MATRIX_H
#define MATRIX_H

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

void init_random_norm(Matrix *m);
void init_constant(Matrix *m, float *data, size_t len);
void init_ones(Matrix *m);
void init_zeros(Matrix *m);

#endif
