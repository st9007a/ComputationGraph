#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>
#include "matrix.h"

enum node_type {
    DL_CONST,
    DL_VAR,
    DL_PLACEHOLDER
};

enum node_func_type {
    DL_FUNC_NONE,

    DL_SCALAR_ADD,
    DL_SCALAR_SUB,
    DL_SCALAR_MUL,
    DL_SCALAR_DIV,

    DL_MATRIX_ADD,
    DL_MATRIX_SUB,
    DL_MATRIX_MUL,

    DL_ACT_RELU,
    DL_ACT_SIGMOID,
    DL_ACT_SOFTMAX,

    DL_COST_MSE,
};

typedef struct __DL_NODE Node;
typedef struct __DL_FEED_DICT FeedDict;

#define DL_NODE_NAME_LEN 128

struct __DL_NODE {
    enum node_type type;
    char name[DL_NODE_NAME_LEN];
    Matrix data;
    Matrix grad;
    struct {
        enum node_func_type type;
        Node *args[2];
    } expr;
    Node *ref;
};

struct __DL_FEED_DICT {
    size_t len;
    char key[DL_NODE_NAME_LEN];
    float *val;
};

void node_info(Node *n, int ignore_val);

Node *node_variable(uint32_t *dim, uint32_t num_dims, char *name);
Node *node_constant(float *val, uint32_t *dim, uint32_t num_dims, char *name);
Node *node_placeholder(uint32_t *dim, uint32_t num_dims, char *name);

Node *node_scalar_add(Node *n1, Node *n2, char *name);
Node *node_scalar_mul(Node *n1, Node *n2, char *name);
Node *node_scalar_div(Node *n1, Node *n2, char *name);
Node *node_scalar_sub(Node *n1, Node *n2, char *name);

Node *node_matrix_add(Node *n1, Node *n2, char *name);
Node *node_matrix_sub(Node *n1, Node *n2, char *name);
Node *node_matrix_mul(Node *n1, Node *n2, char *name);

Node *node_act_relu(Node *preact, char *name);
Node *node_act_sigmoid(Node *preact, char *name);
Node *node_act_softmax(Node *preact, char *name);

Node *node_cost_mse(Node *logits, Node *target, char *name);

extern const struct __DL_EVAL_FUNC_HOOK {
    int idx;
    void (*op_func)(Matrix *res, Matrix *m1, Matrix *m2, int diff);
} eval_funcs[12];

Matrix *node_eval(Node *target, FeedDict *feed, size_t feed_size);
void node_optimize(Node *target, float lr, FeedDict *feed, size_t feed_size);

#endif
