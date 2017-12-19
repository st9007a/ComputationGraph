#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

#define CHECK(func, cond)                \
    if (!(cond)) {                       \
        printf("Test "func": Failed\n"); \
        printf("Rule: "#cond"\n");       \
        exit(1);                         \
    }

#define CHECK_SUCCESS(func) \
    printf("Test "func": Success\n");

void test_create_op(float *data, uint32_t *dim, uint32_t num_dims)
{
    Node *test_node = node_variable(dim, num_dims, "var");

    CHECK("node_variable()", test_node->type == DL_VAR);
    CHECK("node_variable()", test_node->data.num_dims == num_dims);
    CHECK("node_variable()", !strcmp(test_node->name, "var"));
    CHECK("node_variable()", !memcmp(test_node->data.dim, dim, sizeof(int) * num_dims));

    CHECK_SUCCESS("node_variable()");
    free(test_node);

    test_node = node_placeholder(dim, num_dims, "holder");

    CHECK("node_placeholder()", test_node->type == DL_PLACEHOLDER);
    CHECK("node_placeholder()", test_node->data.num_dims == num_dims);
    CHECK("node_placeholder()", !strcmp(test_node->name, "holder"));
    CHECK("node_placeholder()", !memcmp(test_node->data.dim, dim, sizeof(int) * num_dims));

    CHECK_SUCCESS("node_placeholder()");
    free(test_node);

    test_node = node_constant(data, dim, num_dims, "const");

    CHECK("node_constant()", test_node->type == DL_CONST);
    CHECK("node_constant()", test_node->data.num_dims == num_dims);
    CHECK("node_constant()", !strcmp(test_node->name, "const"));
    CHECK("node_constant()", !memcmp(test_node->data.dim, dim, sizeof(int) * num_dims));
    CHECK("node_constant()", !memcmp(test_node->data.val, data, sizeof(int) * test_node->data.len));

    CHECK_SUCCESS("node_constant()");
    free(test_node);
}

void test_scalar_op(uint32_t *dim, uint32_t num_dims)
{
    Node *n1 = node_placeholder(dim, num_dims, "node1");
    Node *n2 = node_variable(NULL, 0, "scalar");

    Node *test_node = node_scalar_add(n1, n2, "scalar_add");

    CHECK("node_scalar_add()", test_node->type == DL_PLACEHOLDER);
    CHECK("node_scalar_add()", test_node->expr.type == DL_SCALAR_ADD);
    CHECK("node_scalar_add()", test_node->data.num_dims == n1->data.num_dims);
    CHECK("node_scalar_add()", !memcmp(test_node->data.dim, n1->data.dim, sizeof(int) * test_node->data.num_dims));

    CHECK_SUCCESS("node_scalar_add()");
    free(test_node);

    test_node = node_scalar_sub(n1, n2, "scalar_sub");

    CHECK("node_scalar_sub()", test_node->type == DL_PLACEHOLDER);
    CHECK("node_scalar_sub()", test_node->expr.type == DL_SCALAR_SUB);
    CHECK("node_scalar_sub()", test_node->data.num_dims == n1->data.num_dims);
    CHECK("node_scalar_sub()", !memcmp(test_node->data.dim, n1->data.dim, sizeof(int) * test_node->data.num_dims));

    CHECK_SUCCESS("node_scalar_sub()");
    free(test_node);

    test_node = node_scalar_mul(n1, n2, "scalar_mul");

    CHECK("node_scalar_mul()", test_node->type == DL_PLACEHOLDER);
    CHECK("node_scalar_mul()", test_node->expr.type == DL_SCALAR_MUL);
    CHECK("node_scalar_mul()", test_node->data.num_dims == n1->data.num_dims);
    CHECK("node_scalar_mul()", !memcmp(test_node->data.dim, n1->data.dim, sizeof(int) * test_node->data.num_dims));

    CHECK_SUCCESS("node_scalar_mul()");
    free(test_node);

    test_node = node_scalar_div(n1, n2, "scalar_div");

    CHECK("node_scalar_div()", test_node->type == DL_PLACEHOLDER);
    CHECK("node_scalar_div()", test_node->expr.type == DL_SCALAR_DIV);
    CHECK("node_scalar_div()", test_node->data.num_dims == n1->data.num_dims);
    CHECK("node_scalar_div()", !memcmp(test_node->data.dim, n1->data.dim, sizeof(int) * test_node->data.num_dims));

    CHECK_SUCCESS("node_scalar_div()");
    free(test_node);

    free(n1);
    free(n2);
}

void test_matrix_op(uint32_t *dim, uint32_t num_dims)
{
    Node *n1 = node_placeholder(dim, num_dims, "node1");
    Node *n2 = node_variable(dim, num_dims, "node2");

    Node *test_node = node_matrix_add(n1, n2, "matrix_add");

    CHECK("node_matrix_add()", test_node->type == DL_PLACEHOLDER);
    CHECK("node_matrix_add()", test_node->expr.type == DL_MATRIX_ADD);
    CHECK("node_matrix_add()", test_node->data.num_dims == n1->data.num_dims);
    CHECK("node_matrix_add()", !memcmp(test_node->data.dim, n1->data.dim, sizeof(int) * test_node->data.num_dims));

    CHECK_SUCCESS("node_matrix_add()");

    free(test_node);

    test_node = node_matrix_sub(n1, n2, "matrix_sub");

    CHECK("node_matrix_sub()", test_node->type == DL_PLACEHOLDER);
    CHECK("node_matrix_sub()", test_node->expr.type == DL_MATRIX_SUB);
    CHECK("node_matrix_sub()", test_node->data.num_dims == n1->data.num_dims);
    CHECK("node_matrix_sub()", !memcmp(test_node->data.dim, n1->data.dim, sizeof(int) * test_node->data.num_dims));

    CHECK_SUCCESS("node_matrix_sub()");

    free(test_node);

    free(n1);
    free(n2);
}

int main()
{
    float test_data[4] = { 2.0, 2.0, 3.0, 4.0 };
    uint32_t test_dim[2] = { 2, 2 };

    test_create_op(test_data, test_dim, 2);
    test_scalar_op(test_dim, 2);
    test_matrix_op(test_dim, 2);

    return 0;
}
