#ifndef MATRIX_H
#define MATRIX_H

#define MATRIX_INIT(m, dim, num_dims)        \
	do {                                     \
		uint32_t len = 1;                    \
		for (int i = 0; i < num_dims; i++) { \
			m.dim[i] = dim[i];               \
			len *= dim[i];                   \
		}                                    \
		m.num_dims = num_dims;               \
		m.len = len;                         \
	} while(0);

typedef struct __MATRIX Matrix;

struct __MATRIX {
	float *val;
	uint32_t dim[4];
	uint32_t num_dims;
	uint32_t len;
};

#endif
