#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testing.h"
#include "matrix.h"

#define CHECK_MATRIX(func, test_mat, expect, check_val)                                   \
    do {                                                                                  \
        CHECK(func, test_mat.num_dims == expect.num_dims);                                \
        CHECK(func, test_mat.len == expect.len);                                          \
        CHECK(func, !memcmp(test_mat.dim, expect.dim, sizeof(int) * test_mat.num_dims));  \
                                                                                          \
        if (check_val) {                                                                  \
            CHECK(func, !memcmp(test_mat.val, expect.val, sizeof(float) * test_mat.len)); \
        }                                                                                 \
                                                                                          \
        CHECK_SUCCESS(func);                                                              \
    } while(0);

#define TEST_CASE(name) void test_##name()
#define RUN_TEST_CASE(name) test_##name()

TEST_CASE(matrix_create)
{
    uint32_t dim[2] = {2, 2};

    Matrix res;
    matrix_create(&res, dim, 2);

    Matrix expect = {
        .dim = {2, 2},
        .len = 4,
        .num_dims = 2,
    };

    CHECK_MATRIX("matrix_create()", res, expect, 0);

    free(res.val);
}

TEST_CASE(matrix_init_constant)
{
    uint32_t dim[2] = {2, 2};
    float data[4] = {1, 2, 3, 4};

    Matrix res;
    matrix_create(&res, dim, 2);

    matrix_init_constant(&res, data, 4);

    Matrix expect = {
        .dim = {2, 2},
        .len = 4,
        .num_dims = 2,
        .val = data
    };

    CHECK_MATRIX("matrix_init_constant()", res, expect, 1);

    free(res.val);
}

TEST_CASE(matrix_init_ones)
{
    uint32_t dim[2] = {2, 2};
    float expect_data[4] = {1, 1, 1, 1};
    Matrix res;
    matrix_create(&res, dim, 2);

    matrix_init_ones(&res);

    Matrix expect = {
        .len = 4,
        .dim = {2, 2},
        .num_dims = 2,
        .val = expect_data,
    };

    CHECK_MATRIX("matrix_init_ones()", res, expect, 1);

    free(res.val);
}

TEST_CASE(matrix_init_zeros)
{
    uint32_t dim[2] = {2, 2};
    float expect_data[4] = {0, 0, 0, 0};
    Matrix res;
    matrix_create(&res, dim, 2);

    matrix_init_zeros(&res);

    Matrix expect = {
        .len = 4,
        .dim = {2, 2},
        .num_dims = 2,
        .val = expect_data,
    };

    CHECK_MATRIX("matrix_init_zeros()", res, expect, 1);

    free(res.val);
}

TEST_CASE(matrix_scalar_add)
{
    uint32_t m1_dim[2] = {2, 2};
    float m1_data[4] = {1, 2, 3, 4};
    float m2_data = 3;
    float expect_data[4] = {4, 5, 6, 7};

    Matrix res, m1, m2;

    matrix_create(&res, m1_dim, 2);
    matrix_create(&m1, m1_dim, 2);
    matrix_create(&m2, NULL, 0);

    Matrix expect = {
        .len = 4,
        .dim = {2, 2},
        .num_dims = 2,
        .val = expect_data,
    };

    m1.val = m1_data;
    m2.val = &m2_data;
    matrix_scalar_add(&res, &m1, &m2, 0);

    CHECK_MATRIX("matrix_scalar_add()", res, expect, 1);

    free(res.val);
}

TEST_CASE(matrix_scalar_sub)
{
    uint32_t m1_dim[2] = {2, 2};
    float m1_data[4] = {1, 2, 3, 4};
    float m2_data = 3;
    float expect_data[4] = {-2, -1, 0, 1};

    Matrix res, m1, m2;

    matrix_create(&res, m1_dim, 2);
    matrix_create(&m1, m1_dim, 2);
    matrix_create(&m2, NULL, 0);

    Matrix expect = {
        .len = 4,
        .dim = {2, 2},
        .num_dims = 2,
        .val = expect_data,
    };

    m1.val = m1_data;
    m2.val = &m2_data;
    matrix_scalar_sub(&res, &m1, &m2, 0);

    CHECK_MATRIX("matrix_scalar_sub()", res, expect, 1);

    free(res.val);
}

TEST_CASE(matrix_scalar_mul)
{
    uint32_t m1_dim[2] = {2, 2};
    float m1_data[4] = {1, 2, 3, 4};
    float m2_data = 3;
    float expect_data[4] = {3, 6, 9, 12};

    Matrix res, m1, m2;

    matrix_create(&res, m1_dim, 2);
    matrix_create(&m1, m1_dim, 2);
    matrix_create(&m2, NULL, 0);

    Matrix expect = {
        .len = 4,
        .dim = {2, 2},
        .num_dims = 2,
        .val = expect_data,
    };

    m1.val = m1_data;
    m2.val = &m2_data;
    matrix_scalar_mul(&res, &m1, &m2, 0);

    CHECK_MATRIX("matrix_scalar_mul()", res, expect, 1);

    free(res.val);
}

TEST_CASE(matrix_scalar_div)
{
    uint32_t m1_dim[2] = {2, 2};
    float m1_data[4] = {2, 4, 6, 8};
    float m2_data = 2;
    float expect_data[4] = {1, 2, 3, 4};

    Matrix res, m1, m2;

    matrix_create(&res, m1_dim, 2);
    matrix_create(&m1, m1_dim, 2);
    matrix_create(&m2, NULL, 0);

    Matrix expect = {
        .len = 4,
        .dim = {2, 2},
        .num_dims = 2,
        .val = expect_data,
    };

    m1.val = m1_data;
    m2.val = &m2_data;
    matrix_scalar_div(&res, &m1, &m2, 0);

    CHECK_MATRIX("matrix_scalar_div()", res, expect, 1);

    free(res.val);
}

TEST_CASE(matrix_add)
{
    uint32_t dim[2] = {2, 2};
    float m1_data[4] = {1, 2, 3, 4};
    float m2_data[2] = {4, 3};
    float expect_data[4] = {5, 5, 7, 7};

    Matrix res, m1, m2;

    matrix_create(&res, dim, 2);
    matrix_create(&m1, dim, 2);
    matrix_create(&m2, dim, 1);

    Matrix expect = {
        .len = 4,
        .dim = {2, 2},
        .num_dims = 2,
        .val = expect_data,
    };

    m1.val = m1_data;
    m2.val = m2_data;
    matrix_add(&res, &m1, &m2, 0);

    CHECK_MATRIX("matrix_add()", res, expect, 1);

    free(res.val);
}

TEST_CASE(matrix_sub)
{
    uint32_t dim[2] = {2, 2};
    float m1_data[4] = {1, 2, 3, 4};
    float m2_data[4] = {4, 3, 2, 1};
    float expect_data[4] = {-3, -1, 1, 3};

    Matrix res, m1, m2;

    matrix_create(&res, dim, 2);
    matrix_create(&m1, dim, 2);
    matrix_create(&m2, dim, 2);

    Matrix expect = {
        .len = 4,
        .dim = {2, 2},
        .num_dims = 2,
        .val = expect_data,
    };

    m1.val = m1_data;
    m2.val = m2_data;
    matrix_sub(&res, &m1, &m2, 0);

    CHECK_MATRIX("matrix_sub()", res, expect, 1);

    free(res.val);
}

TEST_CASE(matrix_mul)
{
    uint32_t m1_dim[2] = {2, 3};
    uint32_t m2_dim[2] = {3, 2};
    uint32_t res_dim[2] = {2, 2};

    float m1_data[6] = {
        1, 2, 3,
        4, 5, 6,
    };

    float m2_data[6] = {
        1, 2,
        3, 4,
        5, 6,
    };

    float expect_data[4] = {
        22, 28,
        49, 64,
    };

    Matrix res, m1, m2;

    matrix_create(&res, res_dim, 2);
    matrix_create(&m1, m1_dim, 2);
    matrix_create(&m2, m2_dim, 2);

    Matrix expect = {
        .len = 4,
        .dim = {2, 2},
        .num_dims = 2,
        .val = expect_data,
    };

    m1.val = m1_data;
    m2.val = m2_data;
    matrix_mul(&res, &m1, &m2, 0);

    CHECK_MATRIX("matrix_mul()", res, expect, 1);

    free(res.val);
}

int main()
{
    RUN_TEST_CASE(matrix_create);

    RUN_TEST_CASE(matrix_init_constant);
    RUN_TEST_CASE(matrix_init_ones);
    RUN_TEST_CASE(matrix_init_zeros);

    RUN_TEST_CASE(matrix_scalar_add);
    RUN_TEST_CASE(matrix_scalar_sub);
    RUN_TEST_CASE(matrix_scalar_mul);
    RUN_TEST_CASE(matrix_scalar_div);

    RUN_TEST_CASE(matrix_add);
    RUN_TEST_CASE(matrix_sub);
    RUN_TEST_CASE(matrix_mul);

    return 0;
}
