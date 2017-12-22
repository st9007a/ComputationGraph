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

#define NODE_INIT(n, dim, num_dims, TYPE)       \
    do {                                        \
        n = malloc(sizeof(Node));               \
        if (n == NULL) {                        \
            FATAL(MEMORY_EXHAUSTED_ERROR"\n");  \
        }                                       \
        strcpy(n->name, name);                  \
        matrix_create(&n->data, dim, num_dims); \
        matrix_create(&n->grad, dim, num_dims); \
        n->type = TYPE;                         \
        n->expr.type = DL_FUNC_NONE;            \
        n->expr.args[0] = NULL;                 \
        n->expr.args[1] = NULL;                 \
        n->ref = NULL;                          \
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

    if (n->expr.args[0] != NULL) {
        printf("Arg 1: %s\n", n->expr.args[0]->name);
    }
    if (n->expr.args[1] != NULL) {
        printf("Arg 2: %s\n", n->expr.args[1]->name);
    }

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

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_matrix_add(Node *n1, Node *n2, char *name)
{
    if (memcmp(n2->data.dim, n1->data.dim + n1->data.num_dims - n2->data.num_dims, n2->data.num_dims * sizeof(int))) {
        FATAL(UNEXPECTED_SHAPE_ERROR": The shape of Node1 and Node2 is unmatched\n");
    }

    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;
        for (int i = 0; i < n1->data.len; i += n2->data.len) {
            for (int j = 0; j < n2->data.len; j++) {
                n->data.val[i + j] = n1->data.val[i + j] + n2->data.val[j];
            }
        }
    }

    n->expr.type = DL_MATRIX_ADD;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;

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

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_matrix_mul(Node *n1, Node *n2, char *name)
{
    if (n1->data.num_dims < 2 ||
            n2->data.num_dims < 2 ||
            n1->data.num_dims != n2->data.num_dims ||
            memcmp(n1->data.dim, n2->data.dim, sizeof(int) * (n1->data.num_dims - 2)) ||
            n1->data.dim[n1->data.num_dims - 1] != n2->data.dim[n2->data.num_dims - 2]
       ) {

        FATAL(UNEXPECTED_SHAPE_ERROR": The shape of Node1 and Node2 is unmatched\n");
    }

    n1->data.dim[n1->data.num_dims - 1] = n2->data.dim[n2->data.num_dims - 1];
    Node *n = node_placeholder(n1->data.dim, n1->data.num_dims, name);
    n1->data.dim[n1->data.num_dims - 1] = n2->data.dim[n2->data.num_dims - 2];

    if (n1->type == DL_CONST && n2->type == DL_CONST) {
        n->type = DL_CONST;

        matrix_init_zeros(&n->data);

        int block = n->data.dim[n->data.num_dims - 2] * n->data.dim[n->data.num_dims - 1];

        for (int i = 0; i < n->data.len; i += block) {
            for (int a = 0; a < n->data.dim[n->data.num_dims - 2]; a++) {
                for (int b = 0; b < n->data.dim[n->data.num_dims - 1]; b++) {
                    float sum = 0;
                    for (int c = 0; c < n1->data.dim[n1->data.num_dims - 1]; c++) {
                        sum += n1->data.val[i + a * n1->data.dim[n1->data.num_dims - 1] + c] * n2->data.val[i + c * n2->data.dim[n2->data.num_dims - 1] + b];
                    }
                    n->data.val[i + a * n->data.dim[n->data.num_dims - 1] + b] = sum;
                }
            }
        }
    }

    n->expr.type = DL_MATRIX_MUL;
    n->expr.args[0] = n1;
    n->expr.args[1] = n2;

    n1->ref = n;
    n2->ref = n;

    return n;
}

Node *node_shape_reshape(Node *n, uint32_t *dim, uint32_t num_dims, char *name)
{
    uint32_t len = 1;
    for (int i = 0; i < num_dims; i++) {
        len *= dim[i];
    }

    if (len != n->data.len) {
        FATAL(UNEXPECTED_SHAPE_ERROR": new and old shape are unmatched\n");
    }

    Node *reshape_n = node_placeholder(dim, num_dims, name);
    if (n == DL_CONST) {
        reshape_n->type = DL_CONST;
        for (int i = 0; i < n->data.len; i++) {
            reshape_n->data.val[i] = n->data.val[i];
        }
    }

    reshape_n->expr.type = DL_SHAPE_RESHAPE;
    reshape_n->expr.args[0] = n;

    n->ref = reshape_n;
    return reshape_n;
}

Node *node_act_relu(Node *preact, char *name)
{
    Node *activate = node_placeholder(preact->data.dim, preact->data.num_dims, name);

    if (preact->type == DL_CONST) {
        activate->type = DL_CONST;
        for (int i = 0; i < activate->data.len; i++) {
            activate->data.val[i] = preact->data.val[i] >= 0 ? preact->data.val[i] : 0;
        }
    }

    activate->expr.type = DL_ACT_RELU;
    activate->expr.args[0] = preact;

    preact->ref = activate;

    return activate;
}

Node *node_act_sigmoid(Node *preact, char *name)
{
    Node *activate = node_placeholder(preact->data.dim, preact->data.num_dims, name);

    if (preact->type == DL_CONST) {
        activate->type = DL_CONST;
        for (int i = 0; i < activate->data.len; i++) {
            activate->data.val[i] = 1 / (1 + exp(0 - preact->data.val[i]));
        }
    }

    activate->expr.type = DL_ACT_SIGMOID;
    activate->expr.args[0] = preact;

    preact->ref = activate;

    return activate;
}

//FIXME: it will softmax all element
Node *node_act_softmax(Node *preact, char *name)
{
    Node *activate = node_placeholder(preact->data.dim, preact->data.num_dims, name);

    if (preact->type == DL_CONST) {
        activate->type = DL_CONST;
        float sum = 0;
        for (int i = 0; i < preact->data.len; i++) {
            sum += exp(preact->data.val[i]);
        }
        for (int i = 0; i < activate->data.len; i++) {
            activate->data.val[i] = exp(preact->data.val[i]) / sum;
        }
    }

    activate->expr.type = DL_ACT_SOFTMAX;
    activate->expr.args[0] = preact;

    preact->ref = activate;

    return activate;
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

    logits->ref = cost;
    labels->ref = cost;
    return cost;
}
