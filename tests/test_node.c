#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testing.h"
#include "graph.h"

#define TEST_DATA {2.0, 1.0, 4.0, 3.0}
#define TEST_DIM {2, 2}
#define TEST_NUM_DIMS 2

#define TEST_START                   \
    float test_data[4] = TEST_DATA;  \
    uint32_t test_dim[2] = TEST_DIM; \
    uint32_t test_num_dims = 2;

#define TEST_ARGS test_data, test_dim, test_num_dims
#define TEST_CASE(name) void test_##name(float *data, uint32_t *dim, uint32_t num_dims)
#define RUN_TEST_CASE(name) test_##name(TEST_ARGS);

#define CHECK_NODE(func, test_node, expect)                                                      \
    do {                                                                                         \
        CHECK(func, test_node->type == expect.type);                                             \
        CHECK(func, !strcmp(test_node->name, expect.name));                                      \
                                                                                                 \
        CHECK(func, test_node->data.num_dims == expect.data.num_dims);                           \
        CHECK(func, !memcmp(test_node->data.dim, expect.data.dim, sizeof(int) * TEST_NUM_DIMS)); \
                                                                                                 \
        CHECK(func, test_node->expr.type == expect.expr.type);                                   \
        CHECK(func, test_node->expr.args[0] == expect.expr.args[0]);                             \
        CHECK(func, test_node->expr.args[1] == expect.expr.args[1]);                             \
                                                                                                 \
        CHECK_SUCCESS(func);                                                                     \
    } while(0);

TEST_CASE(node_variable)
{
    Node expect = {
        .name = "var",
        .type = DL_VAR,
        .data = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .grad = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .expr = {
            .type = DL_FUNC_NONE,
            .args = {NULL, NULL},
        }
    };
    Node *test_node = node_variable(dim, num_dims, "var");

    CHECK_NODE("node_variable()", test_node, expect);
    free(test_node);
}

TEST_CASE(node_placeholder)
{
    Node expect = {
        .name = "holder",
        .type = DL_PLACEHOLDER,
        .data = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .grad = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .expr = {
            .type = DL_FUNC_NONE,
            .args = {NULL, NULL},
        }
    };
    Node *test_node = node_placeholder(dim, num_dims, "holder");
    CHECK_NODE("node_placeholder()", test_node, expect);
    free(test_node);
}

TEST_CASE(node_constant)
{
    Node expect = {
        .name = "const",
        .type = DL_CONST,
        .data = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .grad = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .expr = {
            .type = DL_FUNC_NONE,
            .args = {NULL, NULL},
        }
    };
    Node *test_node = node_constant(data, dim, num_dims, "const");
    CHECK("node_constant()", !memcmp(test_node->data.val, data, sizeof(float) * test_node->data.len));
    CHECK_NODE("node_constant()", test_node, expect);
    free(test_node);
}

TEST_CASE(node_scalar_add)
{
    Node *n1 = node_placeholder(dim, num_dims, "node1");
    Node *n2 = node_variable(NULL, 0, "scalar");

    Node expect = {
        .name = "scalar_add",
        .type = DL_PLACEHOLDER,
        .data = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .grad = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .expr = {
            .type = DL_SCALAR_ADD,
            .args = {n1, n2},
        },
    };

    Node *test_node = node_scalar_add(n1, n2, "scalar_add");
    CHECK_NODE("node_scalar_add()", test_node, expect);

    free(test_node);
    free(n1);
    free(n2);
}

TEST_CASE(node_scalar_sub)
{
    Node *n1 = node_placeholder(dim, num_dims, "node1");
    Node *n2 = node_variable(NULL, 0, "scalar");

    Node expect = {
        .name = "scalar_sub",
        .type = DL_PLACEHOLDER,
        .data = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .grad = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .expr = {
            .type = DL_SCALAR_SUB,
            .args = {n1, n2},
        },
    };

    Node *test_node = node_scalar_sub(n1, n2, "scalar_sub");
    CHECK_NODE("node_scalar_sub()", test_node, expect);

    free(test_node);
    free(n1);
    free(n2);
}

TEST_CASE(node_scalar_mul)
{
    Node *n1 = node_placeholder(dim, num_dims, "node1");
    Node *n2 = node_variable(NULL, 0, "scalar");

    Node expect = {
        .name = "scalar_mul",
        .type = DL_PLACEHOLDER,
        .data = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .grad = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .expr = {
            .type = DL_SCALAR_MUL,
            .args = {n1, n2},
        },
    };

    Node *test_node = node_scalar_mul(n1, n2, "scalar_mul");
    CHECK_NODE("node_scalar_mul()", test_node, expect);

    free(test_node);
    free(n1);
    free(n2);
}

TEST_CASE(node_scalar_div)
{
    Node *n1 = node_placeholder(dim, num_dims, "node1");
    Node *n2 = node_variable(NULL, 0, "scalar");

    Node expect = {
        .name = "scalar_div",
        .type = DL_PLACEHOLDER,
        .data = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .grad = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .expr = {
            .type = DL_SCALAR_DIV,
            .args = {n1, n2},
        },
    };

    Node *test_node = node_scalar_div(n1, n2, "scalar_div");
    CHECK_NODE("node_scalar_div()", test_node, expect);

    free(test_node);
    free(n1);
    free(n2);
}

TEST_CASE(node_matrix_add)
{
    Node *n1 = node_placeholder(dim, num_dims, "node1");
    Node *n2 = node_variable(dim, num_dims, "node2");

    Node expect = {
        .name = "matrix_add",
        .type = DL_PLACEHOLDER,
        .data = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .grad = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .expr = {
            .type = DL_MATRIX_ADD,
            .args = {n1, n2},
        },
    };

    Node *test_node = node_matrix_add(n1, n2, "matrix_add");

    CHECK_NODE("node_matrix_add()", test_node, expect);

    free(test_node);
    free(n1);
    free(n2);
}

TEST_CASE(node_matrix_sub)
{
    Node *n1 = node_placeholder(dim, num_dims, "node1");
    Node *n2 = node_variable(dim, num_dims, "node2");

    Node expect = {
        .name = "matrix_sub",
        .type = DL_PLACEHOLDER,
        .data = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .grad = {
            .dim = TEST_DIM,
            .num_dims = TEST_NUM_DIMS,
        },
        .expr = {
            .type = DL_MATRIX_SUB,
            .args = {n1, n2},
        },
    };

    Node *test_node = node_matrix_sub(n1, n2, "matrix_sub");

    CHECK_NODE("node_matrix_sub()", test_node, expect);

    free(test_node);
    free(n1);
    free(n2);
}

TEST_CASE(node_matrix_mul)
{
    int n1_dim[2] = {2, 3};
    int n2_dim[2] = {3, 2};

    Node *n1 = node_placeholder(n1_dim, 2, "node1");
    Node *n2 = node_variable(n2_dim, 2, "node2");

    Node *test_node = node_matrix_mul(n1, n2, "matrix_mul");

    Node expect = {
        .type = DL_PLACEHOLDER,
        .name = "matrix_mul",
        .data = {
            .dim = {2, 2},
            .num_dims = 2,
        },
        .grad = {
            .dim = {2, 2},
            .num_dims = 2,
        },
        .expr = {
            .type = DL_MATRIX_MUL,
            .args = {n1, n2},
        },
    };

    CHECK_NODE("node_matrix_mul()", test_node, expect);
    free(test_node);
    free(n1);
    free(n2);
}

int main()
{

    TEST_START;

    RUN_TEST_CASE(node_variable);
    RUN_TEST_CASE(node_constant);
    RUN_TEST_CASE(node_placeholder);

    RUN_TEST_CASE(node_scalar_add);
    RUN_TEST_CASE(node_scalar_sub);
    RUN_TEST_CASE(node_scalar_mul);
    RUN_TEST_CASE(node_scalar_div);

    RUN_TEST_CASE(node_matrix_add);
    RUN_TEST_CASE(node_matrix_sub);
    RUN_TEST_CASE(node_matrix_mul);

    return 0;
}
