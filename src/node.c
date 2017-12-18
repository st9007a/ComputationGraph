#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "matrix.h"
#include "graph.h"

#define NODE_INIT(n, dim, num_dims, TYPE)      \
    do {                                       \
        n = malloc(sizeof(Node));              \
        if (n == NULL) {                       \
            FATAL(MEMORY_EXHAUSTED_ERROR"\n"); \
        }                                      \
        strcpy(n->name, name);                 \
        MATRIX_INIT(n->data, dim, num_dims);   \
        MATRIX_INIT(n->grad, dim, num_dims);   \
        n->type = TYPE;                        \
        n->expr.type = DL_FUNC_NONE;           \
        n->expr.args[0] = NULL;                \
        n->expr.args[1] = NULL;                \
        n->ref = NULL;                         \
    } while(0);

void node_info(Node *n, int ignore_val)
{
    printf("---------------------\n");
    printf("Name: %s\n", n->name);
    printf("---------------------\n");
    printf("Type: ");
    switch (n->type) {
    case DL_CONST:
        printf("Constant\n");
        break;
    case DL_VAR:
        printf("Variable\n");
        break;
    case DL_PLACEHOLDER:
        printf("Placeholder\n");
        break;
    default:
        printf("Unknown type\n");
    }

    printf("Dimension: %d-D\n", n->data.num_dims);
    printf("Dimension Lenght: ");
    for (int i = 0; i < n->data.num_dims; i++) {
        printf("%u ", n->data.dim[i]);
    }
    printf("\n");

    if (ignore_val)
        goto print_expr;

    printf("Value: ");
    for (int i = 0; i < n->data.len; i++) {
        printf("%f ", n->data.val[i]);
    }
    printf("\n");

print_expr:

    printf("Expression Type: ");
    if (n->expr.type == DL_FUNC_NONE) {
        printf("None\n");
        goto print_ref;
    }

    switch (n->expr.type) {
    case DL_SCALAR_ADD:
        printf("Add\n");
        break;
    case DL_SCALAR_SUB:
        printf("Sub\n");
        break;
    case DL_SCALAR_MUL:
        printf("Mul\n");
        break;
    case DL_SCALAR_DIV:
        printf("Div\n");
        break;
    case DL_COST_MSE:
        printf("Mean Square Error\n");
    default:
        printf("Unknown\n");
    }

    printf("Arg 1: %s\n", n->expr.args[0]->name);
    printf("Arg 2: %s\n", n->expr.args[1]->name);

print_ref:

    if (n->ref != NULL) {
        printf("Ref: %s\n", n->ref->name);
    }
    printf("---------------------\n");
}

Node *node_variable(uint32_t *dim, uint32_t num_dims, char *name)
{
    Node *n;
    NODE_INIT(n, dim, num_dims, DL_VAR);

    matrix_init_random_norm(&n->data);

    return n;
}

Node *node_constant(float *data, uint32_t *dim, uint32_t num_dims, char *name)
{
    Node *n;
    NODE_INIT(n, dim, num_dims, DL_CONST);

    matrix_init_constant(&n->data, data, n->data.len);

    return n;
}

Node *node_placeholder(uint32_t *dim, uint32_t num_dims, char *name)
{
    Node *n;
    NODE_INIT(n, dim, num_dims, DL_PLACEHOLDER);

    return n;
}

Node *node_scalar_add(Node *n1, Node *n2, char *name)
{
    if (n2->data.num_dims != 0) {
        FATAL(UNEXPECTED_SHAPE_ERROR": Scalar operation require a 0-D Node\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] + n2->data.val[0];
        }
    }

    n->expr.type = DL_SCALAR_ADD;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_scalar_sub(Node *n1, Node *n2, char *name)
{
    if (n2->data.num_dims != 0) {
        FATAL(UNEXPECTED_SHAPE_ERROR": Scalar operation require a 0-D Node\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] - n2->data.val[0];
        }
    }

    n->expr.type = DL_SCALAR_SUB;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_scalar_mul(Node *n1, Node *n2, char *name)
{
    if (n2->data.num_dims != 0) {
        FATAL(UNEXPECTED_SHAPE_ERROR": Scalar operation require a 0-D Node\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] * n2->data.val[0];
        }
    }

    n->expr.type = DL_SCALAR_MUL;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_scalar_div(Node *n1, Node *n2, char *name)
{
    if (n2->data.num_dims != 0) {
        FATAL(UNEXPECTED_SHAPE_ERROR": Scalar operation require a 0-D Node\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] / n2->data.val[0];
        }
    }

    n->expr.type = DL_SCALAR_DIV;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_matrix_add(Node *n1, Node *n2, char *name)
{
    if (n1->data.len != n2->data.len || n1->data.num_dims != n2->data.num_dims) {
        FATAL(UNEXPECTED_SHAPE_ERROR": The shape of Node1 and Node2 is unmatched\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] + n2->data.val[i];
        }
    }

    n->expr.type = DL_MATRIX_ADD;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_matrix_sub(Node *n1, Node *n2, char *name)
{
    if (n1->data.len != n2->data.len || n1->data.num_dims != n2->data.num_dims) {
        FATAL(UNEXPECTED_SHAPE_ERROR": The shape of Node1 and Node2 is unmatched\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i++) {
            n->data.val[i] = n1->data.val[i] - n2->data.val[i];
        }
    }

    n->expr.type = DL_MATRIX_SUB;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

// FIXME: it only support 2D matrix
Node *node_matrix_mul(Node *n1, Node *n2, char *name)
{
    if (n1->data.dim[1] != n2->data.dim[0]) {
        FATAL(UNEXPECTED_SHAPE_ERROR": The shape of Node1 and Node2 is unmatched\n");
    }

    n1->data.dim[1] = n2->data.dim[1];
    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);
    n1->data.dim[1] = n2->data.dim[0];

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;

        matrix_init_zeros(&n->data);
        for (int i = 0; i < n1->data.dim[0]; i++) {
            for (int j = 0; j < n2->data.dim[1]; j++) {
                for (int k = 0; k < n1->data.dim[1]; k++) {
                    n->data.val[i * n1->data.dim[1] + j] += n1->data.val[i * n1->data.dim[1] + k] * n2->data.val[k * n2->data.dim[1] + j];
                }
            }
        }
    }

    n->expr.type = DL_MATRIX_MUL;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;
    n->ref = NULL;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_cost_mse(Node *logits, Node *labels, char *name)
{
    if (logits->data.len != labels->data.len || logits->data.num_dims != labels->data.num_dims) {
        FATAL(UNEXPECTED_SHAPE_ERROR": The shape of logits and labels is unmatched\n");
    }

    Node *cost = node_placeholder(NULL, 0, name);
    cost->expr.type = DL_COST_MSE;
    cost->expr.args[0] = logits;
    cost->expr.args[1] = labels;
    cost->ref = NULL;

    logits->ref = cost;
    labels->ref = cost;
    return cost;
}
